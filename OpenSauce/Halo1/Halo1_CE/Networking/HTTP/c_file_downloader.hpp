/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once
#include "Networking/HTTP/HTTP.hpp"
#include "Networking/HTTP/HTTPClient.hpp"

namespace Yelo
{
	namespace Enums
	{
		enum http_file_download_status
		{
			_http_file_download_status_idle,
			_http_file_download_status_in_progress,
			_http_file_download_status_cancelled,
			_http_file_download_status_failed,
			_http_file_download_status_retry,
			_http_file_download_status_succeeded,

			_http_file_download_status
		};
	};
	namespace Networking { namespace HTTP { namespace Client
	{
		class c_file_downloader
		{
		protected:
			struct s_request_args
			{
				const char*						m_headers;
				Enums::http_client_component	m_component;
				void*							m_component_data;
				byte*							m_buffer;
				uint32							m_buffer_size;
				bool							m_delete_buffer;
				bool							m_blocking;
				bool							m_manual_think;
				PAD8;
				real							m_request_timeout;

				void Ctor()
				{
					m_headers = nullptr;
					m_component = Enums::_http_client_component;
					m_component_data = nullptr;
					m_buffer = nullptr;
					m_buffer_size = 0;
					m_delete_buffer = true;
					m_blocking = false;
					m_manual_think = false;
					m_request_timeout = 0.0f;
				}
			};


			struct
			{
				struct
				{
					bool	is_downloading;
					bool	manual_think;
					PAD16;
				}m_flags;

				struct
				{
					int32	request_index;
					real	timeout;
					real	elapsed;
				}m_http;

				struct
				{
					uint32	max_attempts;
					uint32	current_attempt;
					real	retry_delay;
					real	elapsed;
				}m_attempts;

				char								m_url[Enums::k_max_url_length];

				Enums::http_file_download_status	m_download_status;
			}m_file_downloader;

		protected:
			virtual void	GetArguments(s_request_args& arguments) = 0;
			bool			Retry();

		private:
			void			CleanupHTTPRequest();
			bool			StartDownload(s_request_args& arguments);

		public:
			virtual void	Ctor()
			{
				m_file_downloader.m_flags.is_downloading = false;
				m_file_downloader.m_flags.manual_think = false;

				m_file_downloader.m_http.request_index = NONE;
				m_file_downloader.m_http.timeout = 0.0f;
				m_file_downloader.m_http.elapsed = 0.0f;

				m_file_downloader.m_attempts.max_attempts = 1;
				m_file_downloader.m_attempts.current_attempt = 0;
				m_file_downloader.m_attempts.retry_delay = 0.0f;
				m_file_downloader.m_attempts.elapsed = 0.0f;

				m_file_downloader.m_download_status = Enums::_http_file_download_status_idle;

				m_file_downloader.m_url[0] = 0;
			}

			virtual void	Dtor()
			{
				YELO_ASSERT_DISPLAY(m_file_downloader.m_http.request_index == NONE, "a http request has not been cleaned up before being destroyed");

				m_file_downloader.m_flags.is_downloading = false;
				m_file_downloader.m_flags.manual_think = false;

				m_file_downloader.m_http.request_index = NONE;
				m_file_downloader.m_http.timeout = 0.0f;
				m_file_downloader.m_http.elapsed = 0.0f;

				m_file_downloader.m_attempts.max_attempts = 1;
				m_file_downloader.m_attempts.current_attempt = 0;
				m_file_downloader.m_attempts.retry_delay = 0.0f;
				m_file_downloader.m_attempts.elapsed = 0.0f;

				m_file_downloader.m_download_status = Enums::_http_file_download_status_idle;

				m_file_downloader.m_url[0] = '\0';
			}

			int32			HTTPRequestIndex();
			Enums::http_file_download_status& Status();

			bool			SetURL(const char* url);
			void			Start();
			virtual void	Update(real delta);
			void			Stop();

			virtual void*	DownloadCompleteCallback(const bool download_succeeded, const char* buffer, const GHTTPByteCount buffer_length, void* component_data);
			virtual void*	DownloadCancelledCallback(void* component_data);
		};

	};};};
};