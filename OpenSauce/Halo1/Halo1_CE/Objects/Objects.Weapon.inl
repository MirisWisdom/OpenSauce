/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Rasterizer/Rasterizer.hpp"
#include "Game/Camera.hpp"
#include "Game/EngineFunctions.hpp"

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

				int32 presets_count;
				struct s_preset {
					char name[Enums::k_weapon_view_name_length];
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
				s_item_datum* GetCurrentWeapon()
				{
					datum_index player_index = Players::LocalPlayerIndex();
					int16 curr_weapon_index ;
					datum_index* weapons = Players::GetWeapons(player_index, &curr_weapon_index);

					datum_index weapon_index;
					if(weapons && !(weapon_index = weapons[curr_weapon_index]).IsNull())
						return (*Objects::ObjectHeader())[weapon_index]->Type._item;

					return NULL;
				}

				static cstring GetWeaponName(s_item_datum* weapon)
				{
					static char weapon_name[Enums::k_weapon_view_name_length];

					datum_index definition_index = *weapon->object.GetTagDefinition();
					if(!definition_index.IsNull())
					{
						TagGroups::s_item_definition* definition = TagGroups::Instances()[ definition_index.index ]
							.Definition<TagGroups::s_item_definition>();

						if(definition != NULL) // WTF! HOW? idk. also, lazy :(
						{
							int32 msg_index = definition->item.message_index;

							wcstring msg = Engine::Interface::HudGetItemMessage(msg_index);

							memset(weapon_name, 0, sizeof(weapon_name));
							wstring_to_string_lazy(weapon_name, NUMBEROF(weapon_name)-1, msg);
						}
						else return NULL;
					}
					else return NULL;

					return weapon_name;
				}

			public:
				cstring GetCurrentWeaponName()
				{
					s_item_datum* weapon = GetCurrentWeapon();

					if(weapon != NULL)
						return GetWeaponName(weapon);

					return NULL;
				}

				s_preset* GetCurrentPreset()
				{
					cstring name = GetCurrentWeaponName();

					if(name != NULL)
					{
						for(int32 x = 0; x < NUMBEROF(this->presets); x++)
							if( !strcmp(name, presets[x].name) )
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
					bool valid_indices[NUMBEROF(presets)];
					int32 valid_indices_count = 0;
					for(int32 x = 0; x < presets_count; x++)
					{
						valid_indices[x] = !(presets[x].offset.i == 0.0f &&	presets[x].offset.j == 0.0f && presets[x].offset.k == 0.0f);
						if(valid_indices[x]) valid_indices_count++;
					}

					for(int32 x = 0; x < presets_count; x++)
					{
						if(!valid_indices[x]) continue;
						s_preset& p = presets[x];

						TiXmlElement* entry = new TiXmlElement("entry");
							weapons_element->LinkEndChild(entry);
						entry->SetAttribute("name", p.name);

						TiXmlElement* position = new TiXmlElement("position");
							entry->LinkEndChild(position);
						Settings::XmlWriteReal3D(position, CAST(real*, p.offset));
					}
				}

			}_weapon_globals;

			void PLATFORM_API ApplyWeaponPreset()
			{
				static uint32 FIRST_PERSON_WEAPON_RENDER_UPDATE = GET_FUNC_PTR(FIRST_PERSON_WEAPON_RENDER_UPDATE);

				weapon_globals::s_preset* preset = _weapon_globals.GetCurrentPreset();

				if(preset)
				{
					real_vector3d* position = CAST(real_vector3d*, Rasterizer::RenderGlobals()->camera.point);
					const Camera::s_observer* obs = Camera::Observer();

					real_vector3d right_vec;
					obs->GetRightVector(right_vec);
					*position += 
						obs->forward * preset->offset.i +
						right_vec * preset->offset.j +
						obs->up * preset->offset.k;
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

			bool AdjustSettings()
			{
				cstring name = _weapon_globals.GetCurrentWeaponName();
				if(name == NULL) return true;

				weapon_globals::s_preset* preset = _weapon_globals.GetCurrentPreset();
				if(preset == NULL && (preset = _weapon_globals.AddPreset(name)) == NULL)
					return true;

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

				return false;
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