/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include <blamlib/Halo1/interface/first_person_weapons.hpp>
#include <blamlib/Halo1/interface/hud.hpp>

#include <YeloLib/configuration/c_configuration_container.hpp>
#include <YeloLib/configuration/c_configuration_value.hpp>
#include <YeloLib/configuration/c_configuration_value_list.hpp>
#include <YeloLib/configuration/c_configuration_container_list.hpp>
#include <YeloLib/configuration/type_containers/c_real_vector3d_container.hpp>
#include <YeloLib/settings/c_settings_singleton.hpp>

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
#pragma region Settings
			class c_settings_container
				: public Configuration::c_configuration_container
			{
			public:
				class c_weapon_position
					: public Configuration::c_configuration_container
				{
				public:
					Configuration::c_configuration_value<std::string> m_name;
					Configuration::c_real_vector3d_container m_position;

					c_weapon_position()
						: Configuration::c_configuration_container("Weapon")
						, m_name("Name", "")
						, m_position("Position")
					{ }
					
				protected:
					const std::vector<i_configuration_value* const> GetMembers() final override
					{
						return std::vector<i_configuration_value* const>
						{
							&m_name,
							&m_position
						};
					}
				};

				Configuration::c_configuration_container_list<c_weapon_position> m_weapon_positions;

				c_settings_container()
					: Configuration::c_configuration_container("Objects.Weapon.Positions")
					, m_weapon_positions("Weapon", [](){ return c_weapon_position(); })
				{ }
				
			protected:
				const std::vector<i_configuration_value* const> GetMembers() final override
				{
					return std::vector<i_configuration_value* const> { &m_weapon_positions };
				}
			};

			class c_settings_weapons
				: public Settings::c_settings_singleton<c_settings_container, c_settings_weapons>
			{ };
#pragma endregion

#if PLATFORM_IS_DEDI
			void Initialize()	{}
			void Dispose()		{}
#else

			struct weapon_globals {
				TextBlock* menu;

				datum_index weapon_index_from_last_update;

				void Dispose()
				{
					if(menu != nullptr)
					{
						delete menu;
						menu = nullptr;
					}
				}

				c_settings_container::c_weapon_position* AddPreset(cstring name)
				{
					auto& settings_instance(c_settings_weapons::Instance().Get());
					if(Enums::k_weapon_view_max_weapon_presets == settings_instance.m_weapon_positions.Get().size())
						return nullptr;

					c_settings_container::c_weapon_position& weapon_position(settings_instance.m_weapon_positions.AddEntry());
					weapon_position.m_name.Get() = name;
					weapon_position.m_position.Get().Set(.0f,.0f,.0f);

					return &weapon_position;
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
				c_settings_container::c_weapon_position* GetCurrentPreset(datum_index& return_weapon_index, cstring& return_name)
				{
					return_weapon_index = datum_index::null;
					return_name = GetCurrentWeaponName(return_weapon_index);
					
					auto& settings_instance(c_settings_weapons::Instance().Get());
					if(settings_instance.m_weapon_positions.Get().size() == 0) return nullptr;

					if(return_name != nullptr)
					{
						auto found_value = std::find_if(settings_instance.m_weapon_positions.begin(), settings_instance.m_weapon_positions.end(),
							[return_name](c_settings_container::c_weapon_position& entry)
							{
								return entry.m_name.Get() == std::string(return_name);
							}
						);

						if(found_value != settings_instance.m_weapon_positions.end())
						{
							return &(*found_value);
						}
					}

					return nullptr;
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
				c_settings_container::c_weapon_position* preset = _weapon_globals.GetCurrentPreset(weapon_index, name);

				if(preset != nullptr)
				{
					auto* position = CAST(real_vector3d*, Render::RenderGlobals()->camera.point);
					const Camera::s_observer* obs = Camera::Observer();

					real_vector3d right_vec;
					obs->GetRightVector(right_vec);
					*position += 
						obs->origin.forward * preset->m_position.Get().i +
						right_vec * preset->m_position.Get().j +
						obs->origin.up * preset->m_position.Get().k;
				}

				__asm {
					push	edx
					call	FIRST_PERSON_WEAPON_RENDER_UPDATE
					pop		edx
				}
			}

			void Initialize()
			{
				c_settings_weapons::Instance().Register(Settings::Manager());

				Memory::WriteRelativeCall(ApplyWeaponPreset, 
					GET_FUNC_VPTR(RENDER_WINDOW_CALL_HOOK_FIRST_PERSON_WEAPON_RENDER_UPDATE));
			}

			void Dispose()
			{
				_weapon_globals.Dispose();
				
				c_settings_weapons::Instance().Unregister(Settings::Manager());
			}

			Enums::settings_adjustment_result AdjustSettings()
			{
				datum_index weapon_index;
				cstring name;
				c_settings_container::c_weapon_position* preset = _weapon_globals.GetCurrentPreset(weapon_index, name);

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

				auto& weapon_position(preset->m_position.Get());

				if(Input::GetMouseButtonState(Enums::_MouseButton3) == 1)
					weapon_position.Set(.0f,.0f,.0f);

				const real factor = 3000.f;

				if(Input::GetKeyState(Enums::_KeyLShift) || Input::GetKeyState(Enums::_KeyRShift))
				{
					weapon_position.j += Input::GetMouseAxisState(Enums::_MouseAxisX)/factor;
					weapon_position.k += Input::GetMouseAxisState(Enums::_MouseAxisY)/factor;
				}
				else
					weapon_position.i += Input::GetMouseAxisState(Enums::_MouseAxisY)/factor;

				_weapon_globals.menu->Render();

				return Enums::_settings_adjustment_result_not_finished;
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