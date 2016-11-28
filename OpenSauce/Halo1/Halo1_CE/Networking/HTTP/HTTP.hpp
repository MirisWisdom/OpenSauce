/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#define HTTP_VERSION_MAJOR 1
#define HTTP_VERSION_MINOR 1

#define K_HTTP_CLIENT_ID "OpenSauce_Halo1_CE_Client"
#define K_HTTP_DEDI_ID "OpenSauce_Halo1_CE_Dedi"

#include <vector>
#include <string>
#include <sstream>

namespace Yelo
{
	namespace Enums
	{
		enum http_status_code
		{
			// information status codes not available in http 1.0
#if (HTTP_VERSION_MAJOR == 1) && (HTTP_VERSION_MINOR != 0)
			_http_status_code_informational_continue,
			_http_status_code_informational_switching_protocols,
#endif

			_http_status_code_successful_ok,
			_http_status_code_successful_created,
			_http_status_code_successful_accepted,
			_http_status_code_successful_non_authorative_information,
			_http_status_code_successful_no_content,
			_http_status_code_successful_reset_content,
			_http_status_code_successful_partial_content,

			_http_status_code_redirection_multiple_choices,
			_http_status_code_redirection_moved_permanently,
			_http_status_code_redirection_found,
			_http_status_code_redirection_see_other,
			_http_status_code_redirection_not_modified,
			_http_status_code_redirection_use_proxy,
			_http_status_code_redirection_temporary_redirect,

			_http_status_code_client_error_bad_request,
			_http_status_code_client_error_unauthorized,
			_http_status_code_client_error_payment_required,
			_http_status_code_client_error_forbidden,
			_http_status_code_client_error_not_found,
			_http_status_code_client_error_method_not_allowed,
			_http_status_code_client_error_not_acceptable,
			_http_status_code_client_error_proxy_authentication_required,
			_http_status_code_client_error_request_timeout,
			_http_status_code_client_error_conflict,
			_http_status_code_client_error_gone,
			_http_status_code_client_error_length_required,
			_http_status_code_client_error_precondition_failed,
			_http_status_code_client_error_request_entity_too_large,
			_http_status_code_client_error_request_uri_too_long,
			_http_status_code_client_error_unsupported_media_type,
			_http_status_code_client_error_requested_range_not_satisfiable,
			_http_status_code_client_error_expectation_failed,

			_http_status_code_server_error_internal_server_error,
			_http_status_code_server_error_not_implemented,
			_http_status_code_server_error_bad_gateway,
			_http_status_code_server_error_service_unavailable,
			_http_status_code_server_error_gateway_timeout,
			_http_status_code_server_error_http_version_not_supported,

			_http_status_code
		};

		enum
		{
			k_max_url_query_count = 64,
			k_max_url_length = 2000, // ~2000 is the max for IE
			k_max_ip_string_length = 42 + 1, // [0000:0000:0000:0000:0000:0000:0000:0000](NULL)
			k_max_ip_port_string_length = k_max_ip_string_length + 1 + 5 + 1, // [0000:0000:0000:0000:0000:0000:0000:0000]:65535(NULL)
		};
	};

	namespace Networking { namespace HTTP
	{
		typedef char http_url_t[Enums::k_max_url_length];

		class c_query_pair
		{
		public:
			std::string m_field;
			std::string m_value;

			bool HasField();
			bool HasValue();
			void Clear();
			void Set(const char* field, const char* value);
		};

		class c_http_header
		{
			struct s_header
			{
				std::string m_header;
				std::string m_value;
			};

			std::vector<s_header>	m_headers;

		public:
			void AddHeader(const char* header, const char* value);

			std::string GetHeaders();
		};

		class c_url_interface
		{
		public:
			std::string					m_scheme;

			std::string					m_username;
			std::string					m_password;
			std::string					m_address;
			int32						m_port;

			std::vector<std::string>	m_path;

			uint32						m_query_count;
			c_query_pair				m_queries[Enums::k_max_url_query_count];

			std::string					m_fragment;

			c_url_interface() : m_port(80), m_query_count(0)
			{ }

		private:
			void SplitString(std::string& string_in, std::string& string_out, const char* delimiter, const bool forward, bool any_char = true);
			void ClearQueries();
			void ParseNetworkLocation(std::string network_location);
			std::string Unescape(std::string url_string, bool plus_as_spaces = false);
			std::string Escape(std::string url_string);
		public:
			bool AddQuery(const char* field, const char* value);
			bool GetQueryValue(const char* field, std::string& value_out);
			void ParseQuery(const char* query);
			void ParsePath(const char* path);
			void AppendPath(const char* path, bool unescape = false);
			std::string GetResource();
			void ParseURL(const char* url);
			std::string GetURL();
		};

		struct s_status_code
		{
			const uint32 m_code;
			const char* m_reason;
		};

		extern const s_status_code g_status_codes[Enums::_http_status_code];
	};};
};