/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once
#include "Networking/HTTP/c_file_downloader.hpp"

namespace Yelo
{
	namespace Networking { namespace HTTP { namespace Client
	{
		class c_xml_downloader : public c_file_downloader
		{
			struct
			{
				TiXmlDocument	document;
			}m_xml_downloader;

		protected:
			void GetArguments(s_request_args& request_args);

		public:
			void Ctor()
			{
				c_file_downloader::Ctor();

				m_xml_downloader.document.Clear();
			}

			void Dtor()
			{
				c_file_downloader::Dtor();

				m_xml_downloader.document.Clear();
			}

			TiXmlDocument& Document();

			void* DownloadCompleteCallback(const bool download_succeeded, const char* buffer, const GHTTPByteCount buffer_length, void* component_data);
			void* DownloadCancelledCallback(void* component_data);
		};
	};};};
};