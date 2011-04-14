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

namespace Yelo
{
	namespace Networking { namespace VersionCheck
	{
		c_version_display_manager c_version_display_manager::g_instance;

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
			SetCurrentVersionStringImpl(version_string);
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
			SetAvailableVersionStringImpl(version_string);
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