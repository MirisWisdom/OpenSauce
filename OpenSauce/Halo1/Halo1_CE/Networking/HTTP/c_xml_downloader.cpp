/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include "Networking/HTTP/c_xml_downloader.hpp"

namespace Yelo
{
	namespace Networking { namespace HTTP { namespace Client
	{
		void c_xml_downloader::GetArguments(s_request_args& request_args)
		{
			request_args.m_headers = nullptr;
			request_args.m_component = Enums::_http_client_component;
			request_args.m_component_data = nullptr;
			request_args.m_buffer = nullptr;
			request_args.m_buffer_size = 0;
			request_args.m_delete_buffer = true;
			request_args.m_blocking = false;
			request_args.m_manual_think = false;
			request_args.m_request_timeout = 0.0f;
		}

		/*!
		 * \brief
		 * Reference to the downloaded xml document.
		 * 
		 * \returns
		 * Returns a reference to the xml downloaders document.
		 * 
		 * Reference to the downloaded xml document.
		 */
		TiXmlDocument& c_xml_downloader::Document()
		{
			return m_xml_downloader.document;
		}

		/*!
		 * \brief
		 * Parses the downloaded data, treating it as an xml text file.
		 * 
		 * \param download_succeeded
		 * Boolean stating whether the download succeeded or not.
		 * 
		 * \param buffer
		 * Pointer to the downloaded data.
		 * 
		 * \param buffer_length
		 * Length of the data pointed to by buffer
		 * 
		 * \param component_data
		 * Pointer to data that was passed to the HTTP request.
		 * 
		 * \returns
		 * Returns the component_data parameters value.
		 * 
		 * Parses the downloaded data, treating it as an xml text file. If the buffer is empty or parsing fails,
		 * the downloaders status is set to failed.
		 */
		void* c_xml_downloader::DownloadCompleteCallback(const bool download_succeeded, const char* buffer, const GHTTPByteCount buffer_length, void* component_data)
		{
			c_file_downloader::DownloadCompleteCallback(download_succeeded, buffer, buffer_length, component_data);

			// if the download failed or the buffer was empty, set the downloaders status to failed
			if(!download_succeeded || !buffer || (buffer_length == 0))
			{
				Status() = Enums::_http_file_download_status_failed;
				return component_data;
			}

			// attempt to parse the downloaded data as if it were an xml text file
			m_xml_downloader.document.Parse(buffer);

			// if the parsing failed, set the downloaders status to failed and clear the document
			if(m_xml_downloader.document.Error())
			{
				m_xml_downloader.document.Clear();
				Status() = Enums::_http_file_download_status_failed;
			}

			Status() = Enums::_http_file_download_status_succeeded;

			return component_data;
		}

		/*!
		 * \brief
		 * Clears the document and sets the downloaders status to failed.
		 * 
		 * \param component_data
		 * Pointer to data that was stored with the HTTP request.
		 * 
		 * \returns
		 * Returns the value of component_data.
		 * 
		 * Clears the document and sets the downloaders status to failed.
		 */
		void* c_xml_downloader::DownloadCancelledCallback(void* component_data)
		{
			c_file_downloader::DownloadCancelledCallback(component_data);

			m_xml_downloader.document.Clear();

			Status() = Enums::_http_file_download_status_cancelled;

			return component_data;
		}
	};};};
};