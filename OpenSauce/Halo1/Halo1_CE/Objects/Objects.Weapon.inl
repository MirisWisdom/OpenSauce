/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Rasterizer/Rasterizer.hpp"
#include "Game/Camera.hpp"
#include "Game/EngineFunctions.hpp"
#include "Interface/GameUI.hpp"

namespace Yelo
{
	namespace Objects
	{
		namespace Weapon
		{
#if PLATFORM_IS_DEDI
			void Initialize()	{}
			void Dispose()		{}
#else

			struct weapon_globals {
				TextBlock* menu;

				datum_index weapon_index_from_last_update;

				int32 presets_count;
				struct s_preset {
					char name[Enums::k_weapon_view_name_length+1];
					real_vector3d offset;
				}presets[Enums::k_weapon_view_max_weapon_presets];


				void Dispose()
				{
					if(menu != NULL)
					{
						delete menu;
						menu = NULL;
					}
				}

				s_preset* AddPreset(cstring name)
				{
					if(presets_count == Enums::k_weapon_view_max_weapon_presets-1)
						return NULL;

					s_preset* preset = &presets[presets_count++];
					strcpy_s(preset->name, NUMBEROF(preset->name), name);
					preset->offset.Set(.0f,.0f,.0f);

					return preset;
				}


			private:
				s_item_datum* GetCurrentWeapon(datum_index& return_weapon_index)
				{
					const GameUI::s_first_person_weapon& fp_weapon = GameUI::FirstPersonWeapons()->local_players[0];
					return_weapon_index = *fp_weapon.GetWeaponIndex();
					if(!return_weapon_index.IsNull())
						return (*Objects::ObjectHeader())[return_weapon_index]->_item;

					return NULL;
				}

				static cstring GetWeaponName(s_item_datum* weapon)
				{
					static char weapon_name[Enums::k_weapon_view_name_length+1];

					datum_index definition_index = weapon->object.definition_index;
					if(!definition_index.IsNull())
					{
						const TagGroups::s_item_definition* definition = TagGroups::TagGet<TagGroups::s_item_definition>(definition_index);
						if(definition != NULL) // WTF! HOW? idk. also, lazy :(
						{
							int32 msg_index = definition->item.message_index;

							wcstring msg = Engine::Interface::HudGetItemMessage(msg_index);

							wstring_to_string_lazy(weapon_name, Enums::k_weapon_view_name_length+1, msg);
						}
						else return NULL;
					}
					else return NULL;

					return weapon_name;
				}

				cstring GetCurrentWeaponName(datum_index& return_weapon_index)
				{
					s_item_datum* weapon = GetCurrentWeapon(return_weapon_index);

					if(weapon != NULL)
						return GetWeaponName(weapon);

					return NULL;
				}
			public:
				s_preset* GetCurrentPreset(datum_index& return_weapon_index, cstring& return_name)
				{
					return_weapon_index = datum_index::null;
					return_name = GetCurrentWeaponName(return_weapon_index);

					if(this->presets_count == 0) return NULL;

					if(return_name != NULL)
					{
						for(int32 x = 0; x < this->presets_count; x++)
							if( !strcmp(return_name, presets[x].name) )
								return &presets[x];
					}

					return NULL;
				}

				void LoadSettings(TiXmlElement* weapons_element)
				{
					memset(presets, 0, sizeof(presets));

					if(weapons_element != NULL)
					{
						for(TiXmlElement* entry = weapons_element->FirstChildElement("entry");
							entry != NULL && presets_count < NUMBEROF(presets);
							entry = entry->NextSiblingElement("entry"), presets_count++)
						{
							s_preset& p = presets[presets_count];

							cstring name = entry->Attribute("name");
							strcpy_s(p.name, name);

							Settings::XmlReadReal3D(entry->FirstChildElement("position"), CAST(real*, p.offset));
						}
					}
				}

				void SaveSettings(TiXmlElement* weapons_element)
				{
					for(int32 x = 0; x < presets_count; x++)
					{
						bool valid_preset = !(presets[x].offset.i == 0.0f &&	presets[x].offset.j == 0.0f && presets[x].offset.k == 0.0f);

						if(!valid_preset) continue;
						s_preset& p = presets[x];

						TiXmlElement* entry = new TiXmlElement("entry");
							weapons_element->LinkEndChild(entry);
						entry->SetAttribute("name", p.name);

						TiXmlElement* position = new TiXmlElement("position");
							entry->LinkEndChild(position);
						Settings::XmlWriteReal3D(position, CAST(real*, p.offset));
					}
				}

			}_weapon_globals = {
				NULL,
				datum_index::null,
			};

			static void PLATFORM_API ApplyWeaponPreset()
			{
				static const uintptr_t FIRST_PERSON_WEAPON_RENDER_UPDATE = GET_FUNC_PTR(FIRST_PERSON_WEAPON_RENDER_UPDATE);

				datum_index weapon_index;
				cstring name;
				weapon_globals::s_preset* preset = _weapon_globals.GetCurrentPreset(weapon_index, name);

				if(preset != NULL)
				{
					real_vector3d* position = CAST(real_vector3d*, Render::RenderGlobals()->camera.point);
					const Camera::s_observer* obs = Camera::Observer();

					real_vector3d right_vec;
					obs->GetRightVector(right_vec);
					*position += 
						obs->origin.forward * preset->offset.i +
						right_vec * preset->offset.j +
						obs->origin.up * preset->offset.k;
				}

				__asm {
					push	edx
					call	FIRST_PERSON_WEAPON_RENDER_UPDATE
					pop		edx
				}
			}

			void Initialize()
			{
				Memory::WriteRelativeCall(ApplyWeaponPreset, 
					GET_FUNC_VPTR(RENDER_WINDOW_CALL_HOOK_FIRST_PERSON_WEAPON_RENDER_UPDATE));
			}

			void Dispose()
			{
				_weapon_globals.Dispose();
			}

			Enums::settings_adjustment_result AdjustSettings()
			{
				datum_index weapon_index;
				cstring name;
				weapon_globals::s_preset* preset = _weapon_globals.GetCurrentPreset(weapon_index, name);

				if(	name == NULL ||
					(!_weapon_globals.weapon_index_from_last_update.IsNull() && 
					  _weapon_globals.weapon_index_from_last_update != weapon_index)
					)
				{
					_weapon_globals.weapon_index_from_last_update = datum_index::null;
					return Enums::_settings_adjustment_result_cancel;
				}
				else
					_weapon_globals.weapon_index_from_last_update = weapon_index;

				if(preset == NULL && (preset = _weapon_globals.AddPreset(name)) == NULL)
					return Enums::_settings_adjustment_result_cancel;

				if(Input::GetMouseButtonState(Enums::_MouseButton3) == 1)
					preset->offset.Set(.0f,.0f,.0f);

				const real factor = 3000.f;

				if(Input::GetKeyState(Enums::_KeyLShift) || Input::GetKeyState(Enums::_KeyRShift))
				{
					preset->offset.j += Input::GetMouseAxisState(Enums::_MouseAxisX)/factor;
					preset->offset.k += Input::GetMouseAxisState(Enums::_MouseAxisY)/factor;
				}
				else
					preset->offset.i += Input::GetMouseAxisState(Enums::_MouseAxisY)/factor;

				_weapon_globals.menu->Render();

				return Enums::_settings_adjustment_result_not_finished;
			}

			void LoadSettings(TiXmlElement* weapons_element)
			{
				_weapon_globals.LoadSettings(weapons_element);
			}

			void SaveSettings(TiXmlElement* weapons_element)
			{
				_weapon_globals.SaveSettings(weapons_element);
			}

#if defined(DX_WRAPPER)
			void Initialize3D(IDirect3DDevice9 *pDevice, D3DPRESENT_PARAMETERS *pPP)
			{
				_weapon_globals.menu = new TextBlock(pDevice,pPP);
				_weapon_globals.menu->ApplyScheme();
				_weapon_globals.menu->SetDimensions(350,0);
				_weapon_globals.menu->Attach(Enums::_attach_method_center, 0,0,0,0);
				_weapon_globals.menu->SetTextAlign(DT_CENTER);

				_weapon_globals.menu->SetText(
					L"Mouse \x2195 = Distance\n"
					L"Shift+Mouse \x21BA = Axis\n"
					L"\n"
					L"Left-Click to Save.\n"
					L"Right-Click to Reset.");
				_weapon_globals.menu->Refresh();
			}

			void OnLostDevice()
			{
				_weapon_globals.menu->OnLostDevice();
			}

			void OnResetDevice(D3DPRESENT_PARAMETERS *pPP)
			{
				_weapon_globals.menu->OnResetDevice(pPP);
				_weapon_globals.menu->Refresh();
			}

			void Render() {}
			void Release() 
			{
				_weapon_globals.menu->Release();
			}

#elif PLATFORM_IS_USER
			// TODO: Need fallback initialization for when we're not using the DX_WRAPPER
#endif

#endif
		};
	};
};