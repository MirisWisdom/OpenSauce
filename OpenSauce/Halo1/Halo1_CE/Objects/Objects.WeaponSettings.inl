/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include <blamlib/Halo1/interface/first_person_weapons.hpp>
#include <blamlib/Halo1/interface/hud.hpp>

#include "Rasterizer/Rasterizer.hpp"
#include "Game/Camera.hpp"
#include "Game/EngineFunctions.hpp"
#include "Interface/Controls.hpp"
#include "Interface/GameUI.hpp"
#include "Interface/TextBlock.hpp"

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

				size_t presets_count;
				struct s_preset {
					char name[Enums::k_weapon_view_name_length+1];
					real_vector3d offset;
				};
				std::array<s_preset, Enums::k_weapon_view_max_weapon_presets> presets;


				void Dispose()
				{
					if(menu != nullptr)
					{
						delete menu;
						menu = nullptr;
					}
				}

				s_preset* AddPreset(cstring name)
				{
					if(presets_count == presets.size())
						return nullptr;

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
						return Objects::ObjectHeader()[return_weapon_index]->_item;

					return nullptr;
				}

				static cstring GetWeaponName(s_item_datum* weapon)
				{
					static char weapon_name[Enums::k_weapon_view_name_length+1];

					datum_index definition_index = weapon->object.definition_index;
					if(!definition_index.IsNull())
					{
						const auto* definition = TagGroups::TagGet<TagGroups::s_item_definition>(definition_index);
						if(definition != nullptr) // WTF! HOW? idk. also, lazy :(
						{
							int16 msg_index = definition->item.message_index;

							wcstring msg = blam::hud_get_item_string(msg_index);

							wstring_to_string_lazy(weapon_name, Enums::k_weapon_view_name_length+1, msg);
						}
						else return nullptr;
					}
					else return nullptr;

					return weapon_name;
				}

				cstring GetCurrentWeaponName(datum_index& return_weapon_index)
				{
					s_item_datum* weapon = GetCurrentWeapon(return_weapon_index);

					if(weapon != nullptr)
						return GetWeaponName(weapon);

					return nullptr;
				}
			public:
				s_preset* GetCurrentPreset(datum_index& return_weapon_index, cstring& return_name)
				{
					return_weapon_index = datum_index::null;
					return_name = GetCurrentWeaponName(return_weapon_index);

					if(this->presets_count == 0) return nullptr;

					if(return_name != nullptr)
					{
						for(size_t x = 0; x < this->presets_count; x++)
							if( !strcmp(return_name, presets[x].name) )
								return &presets[x];
					}

					return nullptr;
				}

				void LoadSettings(TiXmlElement* weapons_element)
				{
					presets.fill(s_preset());

					if(weapons_element != nullptr)
					{
						for(TiXmlElement* entry = weapons_element->FirstChildElement("entry");
							entry != nullptr && presets_count < presets.size();
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
					for(size_t x = 0; x < presets_count; x++)
					{
						s_preset& p = presets[x];

						bool valid_preset = !(	p.offset.i == 0.0f &&
												p.offset.j == 0.0f &&
												p.offset.k == 0.0f);

						if(!valid_preset)
							continue;

						TiXmlElement* entry = new TiXmlElement("entry");
							weapons_element->LinkEndChild(entry);
						entry->SetAttribute("name", p.name);

						TiXmlElement* position = new TiXmlElement("position");
							entry->LinkEndChild(position);
						Settings::XmlWriteReal3D(position, CAST(real*, p.offset));
					}
				}

			}_weapon_globals = {
				nullptr,
				datum_index::null,
			};

			static void PLATFORM_API ApplyWeaponPreset()
			{
				static const uintptr_t FIRST_PERSON_WEAPON_RENDER_UPDATE = GET_FUNC_PTR(FIRST_PERSON_WEAPON_RENDER_UPDATE);

				datum_index weapon_index;
				cstring name;
				weapon_globals::s_preset* preset = _weapon_globals.GetCurrentPreset(weapon_index, name);

				if(preset != nullptr)
				{
					auto* position = CAST(real_vector3d*, Render::RenderGlobals()->camera.point);
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

				if(	name == nullptr ||
					(!_weapon_globals.weapon_index_from_last_update.IsNull() && 
					  _weapon_globals.weapon_index_from_last_update != weapon_index)
					)
				{
					_weapon_globals.weapon_index_from_last_update = datum_index::null;
					return Enums::_settings_adjustment_result_cancel;
				}
				else
					_weapon_globals.weapon_index_from_last_update = weapon_index;

				if(preset == nullptr && (preset = _weapon_globals.AddPreset(name)) == nullptr)
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