/*
    Yelo: Open Sauce SDK
		Halo 1 (CE) Edition
    Copyright (C) 2005-2010  Kornner Studios (http://kornner.com)

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "Common/Precompile.hpp"
#include "Networking/VersionCheckClient.hpp"

#if PLATFORM_IS_USER && defined(YELO_VERSION_CHECK_ENABLE)
#include "Interface/TextBlock.hpp"

#include "Game/GameState.hpp"

namespace Yelo
{
	namespace Networking { namespace VersionCheck
	{
		/////////////////////////////////////////
		//c_version_check_manager_user
		/////////////////////////////////////////
		
		/////////////////////////////////////////
		//non-static functions
		/*!
		 * \brief
		 * Initialises the display manager.

		 * Initialises the display manager.
		 */
		void		c_version_check_manager_user::Initialize()
		{
			c_version_check_manager_base::Initialize();
			c_version_display_manager::g_instance.Initialize();
		}
		/*!
		 * \brief
		 * Disposes of the display manager.
		 * 
		 * Disposes of the display manager.
		 */
		void		c_version_check_manager_user::Dispose()
		{
			c_version_display_manager::g_instance.Dispose();
			c_version_check_manager_base::Dispose();
		}
		/*!
		 * \brief
		 * Sets up the display managers Direct3D resources.
		 * 
		 * \param pDevice
		 * The current render device.
		 * 
		 * \param pParameters
		 * Pointer to the parameters the device was created with.
		 * 
		 * Initialises the display managers Direct3D resources, 
		 * then sets its version strings to their initial values.
		 */
		void		c_version_check_manager_user::Initialize3D(IDirect3DDevice9* pDevice, D3DPRESENT_PARAMETERS* pParameters)
		{
			c_version_display_manager::g_instance.Initialize3D(pDevice, pParameters);

			wchar_t current_string[32];
			swprintf_s(current_string, 32, L"v%i.%i.%i", m_current_version.m_major, m_current_version.m_minor, m_current_version.m_build);
			c_version_display_manager::g_instance.SetCurrentVersionString(current_string);

			c_version_display_manager::g_instance.SetAvailableVersionString(L"");
		}
		void		c_version_check_manager_user::OnLostDevice()
		{
			c_version_display_manager::g_instance.OnLostDevice();
		}
		void		c_version_check_manager_user::OnResetDevice(D3DPRESENT_PARAMETERS* pParameters)
		{
			c_version_display_manager::g_instance.OnResetDevice(pParameters);
		}
		/*!
		 * \brief
		 * Renders the display manager if the map is a main menu.

		 * Renders the display manager if the map is a main menu.
		 */
		void		c_version_check_manager_user::Render()
		{
			// if we are in the main menu, show the version number
			if(m_states_user.is_in_menu)
				c_version_display_manager::g_instance.Render();
		}
		void		c_version_check_manager_user::Release()
		{
			c_version_display_manager::g_instance.Release();
		}
		/*!
		 * \brief
		 * Checks for a new version when the Main Menu is loaded.
		 * 
		 * If the game is loading the main menu, then check for the available online version.
		 * The update check will only occur once per day.
		 * If the update does not complete before a new map is loaded, the http requests
		 * are canceled, and the update is re-run the next time the game is in the main menu.
		 * If the update check has been completed, and a new version is available,
		 * the display manager will be set to display it's animation for 20 seconds, each time
		 * the main menu is loaded.
		 */
		void		c_version_check_manager_user::InitializeForNewMap()
		{
			// find out if we are on the main menu
			m_states_user.is_in_menu = GameState::MainGlobals()->map.main_menu_scenario_loaded;

			// if we are on the main menu, and a new version is available play the animation
			if(m_states_user.is_in_menu && m_states.is_new_version)
				c_version_display_manager::g_instance.StartUpdateDisplay(20);
			else
				c_version_display_manager::g_instance.ResetDisplay();

			// if we are on the main menu and the update check hasn't been done yet, set it going
			// otherwise if we are loading a new map and the version check is not complete, cancel 
			// the requests
			if(m_states_user.is_in_menu && !m_states.checked_today)
				CheckForUpdates(false);
			else if(m_states.is_request_in_progress)
			{
				for(int i = 0; i < NUMBEROF(m_xml_sources); i++)
				{
					ghttpCancelRequest(m_xml_sources[i].request_id);

					m_xml_sources[i].request_id = 0;
					m_xml_sources[i].request_get_attempted = false;
					m_xml_sources[i].request_get_completed = false;
					m_xml_sources[i].request_get_succeeded = false;
					delete m_xml_sources[i].data;
					m_xml_sources[i].data = NULL;
				}
				m_states.is_request_in_progress = false;
			}
		}
		/*!
		 * \brief
		 * Updates the display manager.
		 * 
		 * \param delta_time
		 * The time in seconds that has passed since the last update.

		 * Updates the display manager.
		 */
		void		c_version_check_manager_user::Update(real delta_time)
		{	
			c_version_check_manager_base::Update(delta_time);
			// if we are in the main menu, update the display manager
			if(m_states_user.is_in_menu)
				c_version_display_manager::g_instance.Update(delta_time);
		}
		/*!
		 * \brief
		 * Updates the state of the display manager.
		 *
		 * If a new version is available, the display manager strings are updated
		 * and it is instructed to display its animation for 20 seconds.
		 */
		void		c_version_check_manager_user::UpdateState()
		{
			c_version_check_manager_base::UpdateState();

			if(!m_states.is_new_version) return;

			wchar_t available_string[32];
			swprintf_s(available_string, 32, L"v%i.%i.%i available!", m_available_version.m_major, m_available_version.m_minor, m_available_version.m_build);
			c_version_display_manager::g_instance.SetAvailableVersionString(available_string);

			c_version_display_manager::g_instance.StartUpdateDisplay(20);
		}

		
		/////////////////////////////////////////
		//c_version_check_manager_user
		/////////////////////////////////////////
		
		/////////////////////////////////////////
		//static variables
		c_version_display_manager c_version_display_manager::g_instance;
		
		/////////////////////////////////////////
		//non-static functions
		/*!
		 * \brief
		 * Allocates Direct3D resources and memory for the TextBlocks.
		 * 
		 * \param pDevice
		 * The current render device.
		 * 
		 * \param pParameters
		 * A pointer to the parameters the device was created with.
		 * 
		 * The TextBlocks are set up here by first allocating their memory
		 * and then setting their initial values.
		 */
		void		c_version_display_manager::Initialize3D(IDirect3DDevice9* pDevice, D3DPRESENT_PARAMETERS* pParameters)
		{
			// allocate memory for the textblocks
			m_textblocks.current_version = new TextBlock(pDevice, pParameters);
			m_textblocks.available_version = new TextBlock(pDevice, pParameters);

			// set the text blocks initial values
			m_textblocks.current_version->SetFade(false);
			m_textblocks.current_version->SetFont("Lucida Sans Unicode", 14, FW_NORMAL, false, 6);
			m_textblocks.current_version->SetDimensions(96, 32);
			m_textblocks.current_version->SetTextAlign(DT_LEFT);
			m_textblocks.current_version->SetBackColor(0);
			m_textblocks.current_version->SetTextColor(D3DXCOLOR(0.5f, 0.5f, 0.5f, 0.75f));
			m_textblocks.current_version->SetPadding(4);
			m_textblocks.current_version->Attach(Enums::_attach_method_bottom_left, 0, 0, 0, 0);

			m_textblocks.available_version->SetFade(false);
			m_textblocks.available_version->SetFont("Lucida Sans Unicode", 14, FW_NORMAL, false, 6);
			m_textblocks.available_version->SetDimensions(96, 32);
			m_textblocks.available_version->SetTextAlign(DT_LEFT);
			m_textblocks.available_version->SetBackColor(0);
			m_textblocks.available_version->SetTextColor(D3DXCOLOR(0.7f, 0.7f, 0.7f, 0.75f));
			m_textblocks.available_version->SetPadding(4);
			m_textblocks.available_version->Attach(Enums::_attach_method_bottom_left, 0, 0, 0, 0);
		}
		/*!
		 * \brief
		 * Inform the TextBlocks that the device has been lost
		 */
		void		c_version_display_manager::OnLostDevice()
		{
			m_textblocks.available_version->OnLostDevice();
			m_textblocks.current_version->OnLostDevice();
		}
		/*!
		 * \brief
		 * Inform the TextBlocks that the device has been reset
		 */
		void		c_version_display_manager::OnResetDevice(D3DPRESENT_PARAMETERS* pParameters)
		{
			m_textblocks.available_version->OnResetDevice(pParameters);
			m_textblocks.current_version->OnResetDevice(pParameters);
		}
		/*!
		 * \brief
		 * Draws the version display TextBlocks.
		 * 
		 * Draws the TextBlocks, which show the user the current
		 * and available versions. 
		 * 
		 * \remarks
		 * The available version TextBlock is only displayed when
		 * the animation cycle is not complete.
		 */
		void		c_version_display_manager::Render()
		{
			// refresh the text blocks and render them
			m_textblocks.current_version->Refresh();
			m_textblocks.current_version->Render();

			if(!m_animation.is_cycle_complete)
			{
				m_textblocks.available_version->Refresh();
				m_textblocks.available_version->Render();
			}
		}
		/*!
		 * \brief
		 * Releases all Direct3D resources.
		 * 
		 * Releases all Direct3D resources and deletes the memory
		 * allocated to the TextBlocks.
		 */
		void		c_version_display_manager::Release()
		{
			// release direct3D resources
			m_textblocks.current_version->Release();
			m_textblocks.available_version->Release();

			// delete text block memory
			delete m_textblocks.current_version;
			m_textblocks.current_version = NULL;

			delete m_textblocks.available_version;
			m_textblocks.available_version = NULL;
		}
		/*!
		 * \brief
		 * Updates the display managers animation.
		 * 
		 * \param delta_time
		 * The time in seconds that has passed since the last update.
		 * 
		 * Updates the display managers animation.
		 * 
		 * \remarks
		 * The fade animation will always be displayed until its 
		 * cycle is complete. This is to prevent it from suddenly
		 * changing mid-fade.
		 */
		void		c_version_display_manager::Update(real delta_time)
		{
			// the fade animation will always play until the current fade cycle is complete
			// to prevent it from sharply disappearing
			if(!m_animation.do_animation && m_animation.is_cycle_complete)
			{
				m_textblocks.current_version->SetTextColor(
					D3DXCOLOR(
					0.5f, 
					0.5f, 
					0.5f, 
					0.75f));
				m_textblocks.current_version->SetText(m_strings.current_version);

				m_textblocks.available_version->SetTextColor(
					D3DXCOLOR(
					0.7f, 
					0.7f, 
					0.7f, 
					0.75f));
				m_textblocks.available_version->SetText(m_strings.available_version);
				return;
			}

			// if the animation should be playing continue to count the time that has passed
			if(m_animation.do_animation)
			{
				m_animation.current_time += delta_time;
				if(m_animation.current_time >= m_animation.display_time)
				{
					m_animation.do_animation = false;
					m_animation.current_time = 0.0f;
				}
			}

			// when true decrease the current_position variable to fade the text out
			// when false increase the current_position variable to fade the text in
			if(m_animation.do_decrease)
			{
				m_animation.current_position -= CAST(real, (1.0f / m_animation.cycle_time) * delta_time);
				if(m_animation.current_position <= 0.0f)
				{
					// position has reached zero so the cycle is complete and we can now increase
					m_animation.is_cycle_complete = true;
					m_animation.do_decrease = false;
					m_animation.current_position = 0.0f;
				}
			}
			else
			{
				m_animation.current_position += CAST(real, (1.0f / m_animation.cycle_time) * delta_time);
				if(m_animation.current_position >= 1.0f)
				{
					// position has reached zero so we can now decrease
					m_animation.do_decrease = true;
					m_animation.current_position = 1.0f;
				}
				// the cycle is only complete when the position is zero
				m_animation.is_cycle_complete = false;
			}

			// update the text block text colours
			m_textblocks.current_version->SetTextColor(
				D3DXCOLOR(
				0.5f, 
				0.5f, 
				0.5f, 
				0.75f * (1.0f - m_animation.current_position)));

			m_textblocks.available_version->SetTextColor(
				D3DXCOLOR(
				0.7f, 
				0.7f, 
				0.7f, 
				0.95f * m_animation.current_position));
		}
		/*!
		 * \brief
		 * Sets the current version string to a new value.
		 * 
		 * \param version_string
		 * A wide character pointer containing the new current version string.
		 * 
		 * Sets the current version string to a new value.
		 */
		void		c_version_display_manager::SetCurrentVersionString(wcstring version_string)
		{
			m_strings.current_version[0] = L'\0';
			wcscat_s(m_strings.current_version, version_string);
			m_textblocks.current_version->SetText(m_strings.current_version);
		}		
		/*!
		 * \brief
		 * Sets the available version string to a new value.
		 * 
		 * \param version_string
		 * A wide character pointer containing the new available version string.
		 * 
		 * Sets the available version string to a new value.
		 */
		void		c_version_display_manager::SetAvailableVersionString(wcstring version_string)
		{
			m_strings.available_version[0] = L'\0';
			wcscat_s(m_strings.available_version, version_string);
			m_textblocks.available_version->SetText(m_strings.available_version);
		}
		/*!
		 * \brief
		 * Tells the display manager to start its animation.
		 * 
		 * \param time
		 * The time in seconds that the animation should be played for.
		 * 
		 * Tells the display manager to start its animation.
		 */
		void		c_version_display_manager::StartUpdateDisplay(const real time)
		{
			m_animation.do_animation = true;
			m_animation.is_cycle_complete = true;
			m_animation.do_decrease = false;
			m_animation.display_time = time;
			m_animation.current_time = 0.0f;
			m_animation.current_position = 0.0f;
		}		
		/*!
		 * \brief
		 * Resets the display managers animation, stopping it immediately.
		 * 
		 * Resets the display managers animation, stopping it immediately.
		 */
		void		c_version_display_manager::ResetDisplay()
		{
			m_animation.do_animation = false;
			m_animation.is_cycle_complete = true;
			m_animation.do_decrease = false;
			m_animation.display_time = 0.0f;
			m_animation.current_time = 0.0f;
			m_animation.current_position = 0.0f;
		}
	}; };
};
#endif