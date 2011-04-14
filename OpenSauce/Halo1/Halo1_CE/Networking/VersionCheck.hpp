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
#pragma once

#ifdef VERSION_CHECK_ENABLE
#include "ghttp/ghttp.h"
#include "Interface/TextBlock.hpp"

namespace Yelo
{
	namespace Networking { namespace VersionCheck
	{
		void		Initialize();
		void		Dispose();
		void		Initialize3D(IDirect3DDevice9* pDevice, D3DPRESENT_PARAMETERS* pParameters);
		void		OnLostDevice();
		void		OnResetDevice(D3DPRESENT_PARAMETERS* pParameters);
		void		Render();
		void		Release();
		void		LoadSettings(TiXmlElement* dx9_element);
		void		SaveSettings(TiXmlElement* dx9_element);
		void		InitializeForNewMap();
		void		Update(real delta_time);

		/*!
		 * \brief
		 * Manages how a new version is visually shown to the user.
		 * 
		 * The display manager handles how an available version update
		 * is displayed to the user. The version manager currently 
		 * draws a small piece of text in the lower left corner of
		 * the main menu displaying the current version, which is
		 * then replaced by a brighter piece of text showing the
		 * available version, which fades in and out.
		 */
		class c_version_display_manager
		{			
			enum{
				/*!
				 * \brief
				 * The maximum length of the version strings.
				 */
				k_max_update_string_length = 32,
			};

			struct{
				TextBlock* current_version;
				TextBlock* available_version;
			}m_textblocks;

			struct{
				wchar_t* current_version;
				wchar_t* available_version;
			}m_strings;

			struct{
				/// Should the animation be played
				bool do_animation;
				/// Has the animation cycle reached 0
				bool is_cycle_complete;
				/// Toggles whether to increase or decrease current_position
				bool do_decrease;
				PAD8;
				/// The time it takes to get from 0 to 1
				float cycle_time;
				/// The current position in the animation cycle
				float current_position;

				/// The amount of time the animation should play for in total
				float display_time;
				/// The amount of time the animation has played for
				float current_time;
			}m_animation;

		public:
			/*!
			 * \brief
			 * Initialises the classes variables
			 * 
			 * The constructor sets all of the values to zero
			 * and then initialises then to their default values.
			 */
			c_version_display_manager()
			{
				memset(&m_textblocks, 0, sizeof(m_textblocks));
				memset(&m_strings, 0, sizeof(m_strings));
				memset(&m_animation, 0, sizeof(m_animation));

				m_animation.cycle_time = 1.0f;
				m_animation.is_cycle_complete = true;
			}

			/*!
			 * \brief
			 * Allocates memory for the update version strings.
			 */
			void		Initialize() 
			{
				m_strings.current_version = new wchar_t[k_max_update_string_length];
				m_strings.available_version = new wchar_t[k_max_update_string_length];
			}
			/*!
			 * \brief
			 * Deletes the memory used by the version strings
			 */
			void		Dispose() 
			{
				delete m_strings.current_version;
				m_strings.current_version = NULL;
				delete m_strings.available_version;
				m_strings.available_version = NULL;
			}

			void		Initialize3D(IDirect3DDevice9* pDevice, D3DPRESENT_PARAMETERS* pParameters);
			void		OnLostDevice();
			void		OnResetDevice(D3DPRESENT_PARAMETERS* pParameters);
			void		Render();
			void		Release();

			void		Update(real delta_time);

			void		SetCurrentVersionString(const wcstring version_string);
			void		SetAvailableVersionString(const wcstring version_string);

			void		StartUpdateDisplay(const float time);
			void		ResetDisplay();			
		};

		/*!
		 * \brief
		 * Checks for a newer OS version and informs the user.
		 * 
		 * The version check manager will download an XML file from
		 * a predetermined location, which contains the currently
		 * available version of OpenSauce. It will then display this
		 * to the user if a newer version is available.
		 */
		class c_version_check_manager
		{
		private:
			static c_version_check_manager g_instance;
			/// A hardcoded fallback xml location used when no other location is provided.
			static cstring g_fallback_xml_location;

		private:
			/** Callback function for the ghttp get calls. */
			static GHTTPBool GetRequestComplete(GHTTPRequest request,
				GHTTPResult result,
				char* buffer,
				GHTTPByteCount bufferLen,
				char* headers,
				void* param);
		public:
			/** Returns a reference to a static instance of c_version_check_manager. */
			static c_version_check_manager& VersionChecker() { return g_instance; }

		private:
			c_version_display_manager m_display_manager;

			struct{
				/// Has the version xml been downloaded this session
				bool checked_this_session;
				/// Has the version xml been downloaded today
				bool checked_today;
				/// Is the game currently on the main menu
				bool is_in_menu;
				/// Is there a new version available
				bool is_new_version;

				/// The day that the available version was last checked
				int last_checked_day;
				/// The month that the available version was last checked
				int last_checked_month;
				/// The year that the available version was last checked
				int last_checked_year;
			}m_states;

			struct s_xml_source{
				/// The http address of the xml file
				char*			xml_address;

				GHTTPRequest	request_id;

				bool			request_get_attempted;
				bool			request_get_completed;
				bool			request_get_succeeded;
				PAD8;

				char*			data;
			}m_xml_sources[3];

			struct s_version{
				int	m_major;
				int	m_minor;
				int	m_build;

				void SetBuild(const int major, const int minor, const int build)
				{
					m_major = major;
					m_minor = minor;
					m_build = build;
				}
			};
			/// The version of the current OS installation
			s_version m_current_version;
			/// The version of OS that is available online
			s_version m_available_version;
		public:
			void			Initialize();
			void			Dispose();

			void			Initialize3D(IDirect3DDevice9* pDevice, D3DPRESENT_PARAMETERS* pParameters);
			void			OnLostDevice();
			void			OnResetDevice(D3DPRESENT_PARAMETERS* pParameters);
			void			Render();
			void			Release();

			void			LoadSettings(TiXmlElement* xml_element);
			void			SaveSettings(TiXmlElement* xml_element);

			void			InitializeForNewMap();
			void			Update(real delta_time);

		private:
			void			LoadXmlServers(TiXmlElement* server_list);

			void			CheckForUpdates();
			void			ProcessVersionXml();
			void			UpdateState();
		};
	};};
};

#else

namespace Yelo
{
	namespace Networking { namespace VersionCheck
	{
		void		Initialize();
		void		Dispose();

		void		Initialize3D(IDirect3DDevice9* pDevice, D3DPRESENT_PARAMETERS* pParameters);
		void		OnLostDevice();
		void		OnResetDevice(D3DPRESENT_PARAMETERS* pParameters);
		void		Render();
		void		Release();

		void		LoadSettings(TiXmlElement* dx9_element);
		void		SaveSettings(TiXmlElement* dx9_element);

		void		InitializeForNewMap();
		void		Update(real delta_time);
	};};
};
#endif