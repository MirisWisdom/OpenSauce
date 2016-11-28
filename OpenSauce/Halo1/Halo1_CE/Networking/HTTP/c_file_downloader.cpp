/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include "Networking/HTTP/c_file_downloader.hpp"

namespace Yelo
{
	namespace Networking { namespace HTTP { namespace Client
	{
		/*!
		 * \brief
		 * Determines whether the download should be restarted in the event of a failure.
		 * 
		 * \returns
		 * Returns true if the max number of attempts has not been reached.
		 * 
		 * Determines whether the download should be restarted in the event of a failure.
		 * 
		 * \remarks
		 * This is not called directly by c_file_downloader, but should be used in child classes, as they control the final status of the download.
		 * If a child class permits restarting of downloads the should call this if status is failed, and change the status to retry if it returns true.
		 */
		bool c_file_downloader::Retry()
		{
			if(m_file_downloader.m_attempts.current_attempt++ >= m_file_downloader.m_attempts.max_attempts)
				return false;

			m_file_downloader.m_attempts.elapsed = 0;
			return true;
		}

		/*!
		 * \brief
		 * Aborts the downloaders HTTP request if its in progress.
		 * 
		 * Aborts the downloaders HTTP request if its in progress.
		 */
		void c_file_downloader::CleanupHTTPRequest()
		{
			if(-1 != m_file_downloader.m_http.request_index)
				HTTPRequestAbort(m_file_downloader.m_http.request_index);

			m_file_downloader.m_http.elapsed = 0.0f;
		}

		/*!
		 * \brief
		 * Starts a HTTP get request using argument values set by the child class.
		 * 
		 * \param arguments
		 * Struct containing the arguments set by the child class.
		 * 
		 * \returns
		 * Returns true if the request started successfully, otherwise returns false.
		 * 
		 * Starts a HTTP get request using argument values set by the child class.
		 */
		bool c_file_downloader::StartDownload(s_request_args& arguments)
		{
			// if no url has been set, return false
			if(is_null_or_empty(m_file_downloader.m_url))
				return false;

			// start the HTTP get request
			m_file_downloader.m_http.request_index = HTTPRequestGet(m_file_downloader.m_url,
				arguments.m_headers,
				arguments.m_component,
				arguments.m_component_data,
				CAST_PTR(char*,arguments.m_buffer),
				arguments.m_buffer_size,
				arguments.m_delete_buffer,
				arguments.m_blocking,
				arguments.m_manual_think);

			m_file_downloader.m_http.timeout = arguments.m_request_timeout;
			m_file_downloader.m_http.elapsed = 0.0f;
			m_file_downloader.m_flags.manual_think = arguments.m_manual_think;

			if(-1 != m_file_downloader.m_http.request_index)
			{
				// the get request started successfully
				m_file_downloader.m_download_status = Enums::_http_file_download_status_in_progress;
				return true;
			}
			else
			{
				// the get request could not start
				m_file_downloader.m_download_status = Enums::_http_file_download_status_failed;
				return false;
			}
		}

		/*!
		 * \brief
		 * Returns the HTTP request index of the downloader.
		 * 
		 * \returns
		 * Returns the HTTP request index of the downloader.
		 * 
		 * Returns the HTTP request index of the downloader.
		 */
		int32 c_file_downloader::HTTPRequestIndex()
		{
			return m_file_downloader.m_http.request_index;
		}

		/*!
		 * \brief
		 * Sets the URL of the file to download.
		 * 
		 * \param url
		 * The URL of the file to download.
		 * 
		 * \returns
		 * Returns true If the URL was set successfully, otherwise returns false.
		 * 
		 * Sets the URL of the file to download.
		 */
		bool c_file_downloader::SetURL(const char* url)
		{
			m_file_downloader.m_url[0] = 0;
			if(0 != strcpy_s(m_file_downloader.m_url, url))
			{
				m_file_downloader.m_url[0] = 0;
				return false;
			}

			return true;
		}

		/*!
		 * \brief
		 * Returns the status of the file download.
		 * 
		 * \returns
		 * Returns the status of the file download.
		 * 
		 * Returns the status of the file download.
		 */
		Enums::http_file_download_status& c_file_downloader::Status()
		{
			return m_file_downloader.m_download_status;
		}

		/*!
		 * \brief
		 * Starts the download.
		 * 
		 * Starts the download.
		 */
		void c_file_downloader::Start()
		{
			s_request_args request_args;
			request_args.Ctor();

			// get the request arguments from the child class
			GetArguments(request_args);

			// start the download, setting the status to in progress or failed depending on whether it started successfully
			if(StartDownload(request_args))
			{
				m_file_downloader.m_flags.is_downloading = true;
				m_file_downloader.m_download_status = Enums::_http_file_download_status_in_progress;
			}
			else
			{
				m_file_downloader.m_flags.is_downloading = false;
				m_file_downloader.m_download_status = Enums::_http_file_download_status_failed;
			}
		}

		/*!
		 * \brief
		 * Updates the file download.
		 * 
		 * \param delta
		 * The time in seconds since the last update.
		 * 
		 * Updates the file download.
		 */
		void c_file_downloader::Update(real delta)
		{
			// if the status has been set to retry, clean up the previous request and try to start the download again
			if(m_file_downloader.m_download_status == Enums::_http_file_download_status_retry)
			{
				if(m_file_downloader.m_http.request_index != -1)
					CleanupHTTPRequest();

				// try to start the download again if the delay time has elapsed
				if(ProgressDelay(m_file_downloader.m_attempts.elapsed, delta, false, m_file_downloader.m_attempts.retry_delay))
					Start();
			}

			if(m_file_downloader.m_flags.is_downloading)
			{
				// fail the download if it takes too long to start
				if(ProgressDelay(m_file_downloader.m_http.elapsed, delta, false, m_file_downloader.m_http.timeout) &&
					(GHTTPConnecting >= QueryDownloadState(m_file_downloader.m_http.request_index)))
				{
					m_file_downloader.m_flags.is_downloading = false;
					m_file_downloader.m_download_status = Enums::_http_file_download_status_failed;
				}
				else if(m_file_downloader.m_flags.manual_think)
				{
					// update the HTTP request manually
					HTTPRequestThink(m_file_downloader.m_http.request_index);
				}
			}
		}

		/*!
		 * \brief
		 * Aborts an in progress download.
		 * 
		 * Aborts an in progress download.
		 */
		void c_file_downloader::Stop()
		{
			CleanupHTTPRequest();
			m_file_downloader.m_flags.is_downloading = false;
		}

		void* c_file_downloader::DownloadCompleteCallback(const bool download_succeeded, const char* buffer, const GHTTPByteCount buffer_length, void* component_data)
		{
			m_file_downloader.m_flags.is_downloading = false;

			return component_data;
		}

		void* c_file_downloader::DownloadCancelledCallback(void* component_data)
		{
			m_file_downloader.m_flags.is_downloading = false;

			return component_data;
		}
	};};};
};