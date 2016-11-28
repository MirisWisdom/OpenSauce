/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include "Networking/HTTP/HTTP.hpp"

namespace Yelo
{
	namespace Networking { namespace HTTP
	{
		static const char* g_unreserved_characters = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789-_.!~*'()";

		const s_status_code g_status_codes[Enums::_http_status_code] =
		{
			// information status codes not available in http 1.0
#if (HTTP_VERSION_MAJOR == 1) && (HTTP_VERSION_MINOR != 0)
			{ 100, "Continue" },						//_http_status_code_informational_continue,
			{ 101, "Switching Protocols" },				//_http_status_code_informational_switching_protocols,
#endif

			{ 200, "OK" },								//_http_status_code_successful_ok,
			{ 201, "Created" },							//_http_status_code_successful_created,
			{ 202, "Accepted" },						//_http_status_code_successful_accepted,
			{ 203, "Non-Authoritative Information" },	//_http_status_code_successful_non_authorative_information,
			{ 204, "No Content" },						//_http_status_code_successful_no_content,
			{ 205, "Reset Content" },					//_http_status_code_successful_reset_content,
			{ 206, "Partial Content" },					//_http_status_code_successful_partial_content,

			{ 300, "Multiple Choices" },				//_http_status_code_redirection_multiple_choices,
			{ 301, "Moved Permanently" },				//_http_status_code_redirection_moved_permanently,
			{ 302, "Found" },							//_http_status_code_redirection_found,
			{ 303, "See Other" },						//_http_status_code_redirection_see_other,
			{ 304, "Not Modified" },					//_http_status_code_redirection_not_modified,
			{ 305, "Use Proxy" },						//_http_status_code_redirection_use_proxy,
			// 306 is unused
			{ 307, "Temporary Redirect" },				//_http_status_code_redirection_temporary_redirect,

			{ 400, "Bad Request" },						//_http_status_code_client_error_bad_request,
			{ 401, "Unauthorized" },					//_http_status_code_client_error_unauthorized,
			{ 402, "Payment Required" },				//_http_status_code_client_error_payment_required,
			{ 403, "Forbidden" },						//_http_status_code_client_error_forbidden,
			{ 404, "Not Found" },						//_http_status_code_client_error_not_found,
			{ 405, "Method Not Allowed" },				//_http_status_code_client_error_method_not_allowed,
			{ 406, "Not Acceptable" },					//_http_status_code_client_error_not_acceptable,
			{ 407, "Proxy Authentication Required" },	//_http_status_code_client_error_proxy_authentication_required,
			{ 408, "Request Timeout" },					//_http_status_code_client_error_request_timeout,
			{ 409, "Conflict" },						//_http_status_code_client_error_conflict,
			{ 410, "Gone" },							//_http_status_code_client_error_gone,
			{ 411, "Length Required" },					//_http_status_code_client_error_length_required,
			{ 412, "Precondition Failed" },				//_http_status_code_client_error_precondition_failed,
			{ 413, "Request Entity Too Large" },		//_http_status_code_client_error_request_entity_too_large,
			{ 414, "Request-URI Too Long" },			//_http_status_code_client_error_request_uri_too_long,
			{ 415, "Unsupported Media Type" },			//_http_status_code_client_error_unsupported_media_type,
			{ 416, "Requested Range Not Satisfiable" },	//_http_status_code_client_error_requested_range_not_satisfiable,
			{ 417, "Expectation Failed" },				//_http_status_code_client_error_expectation_failed,

			{ 500, "Internal Server Error" },			//_http_status_code_server_error_internal_server_error,
			{ 501, "Not Implemented" },					//_http_status_code_server_error_not_implemented,
			{ 502, "Bad Gateway" },						//_http_status_code_server_error_bad_gateway,
			{ 503, "Service Unavailable" },				//_http_status_code_server_error_service_unavailable,
			{ 504, "Gateway Timeout" },					//_http_status_code_server_error_gateway_timeout,
			{ 505, "HTTP Version Not Supported" },		//_http_status_code_server_error_http_version_not_supported,
		};

		/*!
		 * \brief
		 * Returns whether this query has a field value.
		 * 
		 * \returns
		 * Returns true if the query has a field value, otherwise returns false.
		 * 
		 * Returns whether this query has a field value.
		 */
		bool c_query_pair::HasField()
		{
			return m_field.length() > 0;
		}

		/*!
		 * \brief
		 * Returns whether this query has a value.
		 * 
		 * \returns
		 * Returns true if the query has a value, otherwise returns false.
		 * 
		 * Returns whether this query has a value.
		 */
		bool c_query_pair::HasValue()
		{
			return m_value.length() > 0;
		}

		/*!
		 * \brief
		 * Resets the field and value strings.
		 * 
		 * Resets the field and value strings.
		 */
		void c_query_pair::Clear()
		{
			m_field.assign("");
			m_value.assign("");
		}

		/*!
		 * \brief
		 * Sets the field an value strings of the query.
		 * 
		 * \param field
		 * Pointer to the queries field string.
		 * 
		 * \param value
		 * Pointer to the queries value string.
		 * 
		 * Sets the field an value strings of the query.
		 * 
		 * \remarks
		 * To set up a query pair the field name cannot be NULL or empty, however the value can.
		 */
		void c_query_pair::Set(const char* field, const char* value)
		{
			YELO_ASSERT_DISPLAY(!is_null_or_empty(field), "attempting to set a query pair with a null or zero-length string");

			m_field.assign(field);

			if(value)
				m_value.assign(value);
			else
				m_value.assign("");
		}

		/*!
		 * \brief
		 * Adds a header-value pair to the header list.
		 * 
		 * \param header
		 * Pointer to the header name string.
		 * 
		 * \param value
		 * Pointer to the header value string.
		 * 
		 * Adds a header-value pair to the header list.
		 */
		void c_http_header::AddHeader(const char* header, const char* value)
		{
			if(header && value)
			{
				s_header new_header;

				new_header.m_header.assign(header);
				new_header.m_value.assign(value);

				m_headers.push_back(new_header);
			}
			return;
		}

		/*!
		 * \brief
		 * Generates a single string containing all of the headers in the list.
		 * 
		 * \returns
		 * Returns a string containing all of the headers in a HTTP valid format.
		 * 
		 * Generates a single string containing all of the headers in the list.
		 */
		std::string c_http_header::GetHeaders()
		{
			std::string header_string("");

			for (auto& header : m_headers)
			{
				header_string.append(header.m_header);
				header_string.append(":");
				header_string.append(header.m_value);
				header_string.append("\r\n");
			}
			return header_string;
		}

		/*!
		 * \brief
		 * Splits a string in to two parts either side of a single or set of delimiter(s).
		 * 
		 * \param string_in
		 * The string to split. This contains the remaining string after the split section is removed.
		 * 
		 * \param string_out
		 * The string that was found.
		 * 
		 * \param delimiter
		 * Pointer to a character string containing either a single string to use as a delimiter, or a string of characters to use.
		 * 
		 * \param forward
		 * Sets whether to search for the delimiter from left to right (true) or right to left (false).
		 * 
		 * \param any_char
		 * Sets whether the entire string must be used as the delimiter (false) of in any character in the string can be used (true).
		 * 
		 * Splits a string in to two parts either side of a single or set of delimiter(s).
		 * 
		 * \remarks
		 * If the/a delimiter is found the delimited section is removed from string_in and assigned to string_out. string_in will contain the remaining string.
		 */
		void c_url_interface::SplitString(std::string& string_in, std::string& string_out, const char* delimiter, const bool forward, bool any_char)
		{
			std::string::size_type offset;

			// look for and split the string from left to right (forward) or right to left (!forward)
			if(forward)
			{
				// look for either the whole delimiter, or any character in the delimiter
				if(any_char)
					offset = string_in.find_first_of(delimiter);
				else
					offset = string_in.find(delimiter);

				// if the/a delimiter was found, copy the string segment, remove the delimiter and
				// remove the segment from the source string
				if(std::string::npos != offset)
				{
					string_out = string_in.substr(0, offset);

					std::string::size_type delimiter_length;
					if(any_char)
						delimiter_length = 1;
					else
						delimiter_length = strlen(delimiter);

					string_in.replace(0, offset + delimiter_length, "");
				}
			}
			else
			{
				if(any_char)
					offset = string_in.find_last_of(delimiter);
				else
					offset = string_in.rfind(delimiter);

				// if the/a delimiter was found, copy the string segment, remove the delimiter and
				// remove the segment from the source string
				if(std::string::npos != offset)
				{
					std::string::size_type copy_start = offset;

					// move the offset past the delimiter
					if(any_char)
						copy_start++;
					else
						copy_start += strlen(delimiter);

					// copy the relevant segment to the output string
					std::string::size_type copy_length = string_in.length() - copy_start;

					if(copy_length)
						string_out = string_in.substr(copy_start, copy_length);

					string_in.replace(offset, copy_length + strlen(delimiter), "");
				}
			}
		}

		/*!
		 * \brief
		 * Replaces percent-encoded characters in a string with their unencoded characters.
		 * 
		 * \param url_string
		 * The string to unescape.
		 * 
		 * \param plus_as_spaces
		 * Sets whether "+" should be replaced with a space (true) or not (false).
		 * 
		 * \returns
		 * Returns the unescaped string.
		 * 
		 * Replaces percent-encoded characters in a string with their unencoded characters.
		 */
		std::string c_url_interface::Unescape(std::string url_string, bool plus_as_spaces)
		{
			std::string::size_type offset;

			// in some cases + can be used to escape spaces
			if(plus_as_spaces)
			{
				offset = 0;
				while(std::string::npos != (offset = url_string.find("+")))
					url_string.replace(offset, 1, " ");
			}

			offset = 0;
			while(std::string::npos != (offset = url_string.find("%")))
			{
				const char* escaped = url_string.c_str() + offset;
				char unescaped[2] = { 0, 0 };

				// if count is zero then something is wrong with the url, but we don't want a drastic
				// exception or anything so we just ignore it
				int count = sscanf_s(escaped, "%%%02X", &unescaped[0], 1);
				if(count == 1)
					url_string.replace(offset, 3, unescaped);
			}
			return url_string;
		}

		/*!
		 * \brief
		 * Replaces any reserved URL characters in a string with their percent-encoded equivalent.
		 * 
		 * \param url_string
		 * The string to escape.
		 * 
		 * \returns
		 * Returns the escaped string.
		 * 
		 * Replaces any reserved URL characters in a string with their percent-encoded equivalent.
		 */
		std::string c_url_interface::Escape(std::string url_string)
		{
			std::string::size_type offset;

			// look for any invalid characters in the string
			offset = 0;
			while(std::string::npos != (offset = url_string.find_first_not_of(g_unreserved_characters, offset)))
			{
				// replace the invalid character with its hex code
				char escaped[4] = "";
				sprintf_s(escaped, sizeof(escaped), "%%%02X", url_string[offset]);
				url_string.replace(offset, 1, escaped);

				// move past the percent encoded character
				offset += 3;
			}
			return url_string;
		}

		/*!
		 * \brief
		 * Removes all of the queries from the query list.
		 * 
		 * Removes all of the queries from the query list.
		 */
		void c_url_interface::ClearQueries()
		{
			// clear the query list
			for (auto& query : m_queries)
				query.Clear();

			m_query_count = 0;
		}

		/*!
		 * \brief
		 * Adds a query pair to the queries list.
		 * 
		 * \param field
		 * Pointer to the field name string.
		 * 
		 * \param value
		 * Pointer to the value string.
		 * 
		 * \returns
		 * Returns true if the query is valid and was added successfully, otherwise returns false.
		 * 
		 * Adds a query pair to the queries list.
		 */
		bool c_url_interface::AddQuery(const char* field, const char* value)
		{
			// set the values to the first query with no field
			for (auto& query : m_queries)
			{
				if(!query.HasField())
				{
					query.Set(field, value);
					m_query_count++;
					return true;
				}
			}
			return false;
		}

		/*!
		 * \brief
		 * Searches the query list for a specific query.
		 * 
		 * \param field
		 * Pointer to the field name to search for.
		 * 
		 * \param value_out
		 * String to set the fields value to if a matching query is found.
		 * 
		 * \returns
		 * Returns true if a matching query is found, otherwise returns false.
		 * 
		 * Searches the query list for a specific query.
		 */
		bool c_url_interface::GetQueryValue(const char* field, std::string& value_out)
		{
			value_out.assign("");

			// find a query with a matching field and returns its value
			for(uint32 i = 0; i < m_query_count; i++)
			{
				if(m_queries[i].m_field.compare(field) == 0)
				{
					value_out.assign(m_queries[i].m_value);
					return true;
				}
			}

			return false;
		}

		/*!
		 * \brief
		 * Appends a path or resource to the URL's path section.
		 * 
		 * \param path
		 * Pointer to the path string to append.
		 * 
		 * \param unescape
		 * Sets whether to unescape the directory/resource name(s).
		 * 
		 * Appends a path or resource to the URL's path section.
		 */
		void c_url_interface::AppendPath(const char* path, bool unescape)
		{
			// /directory/directory/directory/resource

			std::string path_string(path);

			if(!path_string.length())
				return;

			std::string::size_type offset;

			// remove trailing and leading slashes
			offset = 0;
			if((std::string::npos != (offset = path_string.find("/"))) && (offset == 0))
				path_string.replace(0, 1, "");

			offset = 0;
			if((std::string::npos != (offset = path_string.rfind("/"))) && (offset == path_string.length() - 1))
				path_string.replace(path_string.length() - 1, 1, "");

			// replace directory traversal dots with empty strings
			offset = 0;
			while(std::string::npos != (offset = path_string.find("..")))
				path_string.replace(offset, 2, "");

			// replace double slashes with single slashes
			offset = 0;
			while(std::string::npos != (offset = path_string.find("//")))
				path_string.replace(offset, 2, "/");

			do
			{
				std::string directory;

				SplitString(path_string, directory, "/", true);

				if((directory.length() == 0) && path_string.length())
				{
					directory = path_string;
					path_string.clear();
				}

				if(directory.length())
				{
					if(unescape)
						Unescape(directory);
					m_path.push_back(directory);
				}
			}while(path_string.length());
		}

		/*!
		 * \brief
		 * Returns the last directory/resource of the URL's path.
		 * 
		 * \returns
		 * If the path has a directory/resource it is returned, otherwise returns an empty string.
		 * 
		 * Returns the last directory/resource of the URL's path.
		 */
		std::string c_url_interface::GetResource()
		{
			// return the last path string
			if(m_path.size() == 0)
				return "";
			return m_path[m_path.size() - 1];
		}

		/*!
		 * \brief
		 * Splits a urls network address into it's constituent parts.
		 * 
		 * \param network_location
		 * The network location string to parse.
		 * 
		 * Splits a urls network address into it's constituent parts.
		 */
		void c_url_interface::ParseNetworkLocation(std::string network_location)
		{
			//username:password@address:port

			// get the username and password if present
			std::string user_details;

			SplitString(network_location, user_details, "@", true);
			SplitString(user_details, m_username, ":", true);
			m_password = user_details;

			m_username = Unescape(m_username);
			m_password = Unescape(m_password);

			// get the port
			std::string port;
			SplitString(network_location, port, ":", false);

			if(port.length() != 0)
			{
				int count = sscanf_s(port.c_str(), "%i", &m_port);
				if(count == 0)
					m_port = 80;
			}

			// whatever is left is the address
			m_address = network_location;
		}

		/*!
		 * \brief
		 * Splits a URL path into its individual directories and resource.
		 * 
		 * \param path
		 * Pointer to the path string to parse.
		 * 
		 * Splits a URL path into its individual directories and resource.
		 */
		void c_url_interface::ParsePath(const char* path)
		{
			// TODO: what's this [is].empty() call for?
			m_path.empty();

			AppendPath(path, true);
		}

		/*!
		 * \brief
		 * Splits a URL query string into individual field-value pairs.
		 * 
		 * \param query
		 * Pointer to the query string to parse.
		 * 
		 * Splits a URL query string into individual field-value pairs.
		 */
		void c_url_interface::ParseQuery(const char* query)
		{
			// field=value&field=&field=value

			// remove any existing queries
			ClearQueries();

			if(!query)
				return;

			std::string query_string(query);

			do
			{
				// split the query string at the next & if present to get the next query
				std::string current_query;
				SplitString(query_string, current_query, "&", true);

				if(current_query.length() == 0)
				{
					// this is the last query so set the string to whatever is left
					current_query = query_string;
					query_string.clear();
				}

				// check the string contains an "="
				if(std::string::npos == current_query.find("="))
				{
					// the query string is invalid, clear the query list and return
					ClearQueries();
					return;
				}

				std::string field;
				std::string value;
				// split at the "=" for the field and set the value to whatever is left
				SplitString(current_query, field, "=", true);

				field = Unescape(field, true);
				value = Unescape(current_query, true);

				AddQuery(field.c_str(), value.c_str());
			}while(query_string.length());
		}

		/*!
		 * \brief
		 * Splits a URL into its constituent parts.
		 * 
		 * \param url
		 * Pointer to the URL string to parse.
		 * 
		 * Splits a URL into its constituent parts.
		 * 
		 * \remarks
		 * The URL string passed to this function must be an escaped URL, attempting to parse an unescaped URL will have unknown results.
		 */
		void c_url_interface::ParseURL(const char* url)
		{
			// scheme://username:password@address:port/directory/directory/resource?field=value&field=&field=value#fragment

			std::string url_string(url);

			std::string query;
			std::string network_location;

			// forward parsing
			// get the scheme
			SplitString(url_string, m_scheme, "://", true, false);

			if(m_scheme.length() == 0)
				m_scheme.assign("http");

			// get the network location
			SplitString(url_string, network_location, "/", true);
			if(!network_location.length())
			{
				network_location = url_string;
				url_string.clear();
			}

			// reverse parsing
			// get the fragment
			SplitString(url_string, m_fragment, "#", false);
			m_fragment = Unescape(m_fragment);

			// get the query
			SplitString(url_string, query, "?", false);

			// get the path
			ParseNetworkLocation(network_location);
			ParsePath(url_string.c_str());
			ParseQuery(query.c_str());
		}

		/*!
		 * \brief
		 * Builds a complete URL string, combining the values contained in the URL interface instance.
		 * 
		 * \returns
		 * Returns a string containing the URL.
		 * 
		 * Builds a complete URL string, combining the values contained in the URL interface instance.
		 */
		std::string c_url_interface::GetURL()
		{
			std::ostringstream url_stream("");

			// add the scheme
			if(m_scheme.length() == 0)
				url_stream << "http://";
			else
				url_stream << m_scheme << "://";

			// add the username and password
			url_stream << Escape(m_username);
			if(m_password.length())
				url_stream << ":" << Escape(m_password);

			// add the address and port (don't need to escape the address as it cannot have invalid characters)
			url_stream << m_address;
			if(m_port != 80)
				url_stream << ":" << m_port;

			// add the path
			if(m_path.size())
			{
				for(auto iter = m_path.begin(); iter != m_path.end(); ++iter)
					url_stream << "/" << Escape(*iter);
			}
			else
				url_stream << "/";

			// add the queries
			if(m_query_count)
			{
				url_stream << "?";
				for(uint32 i = 0; i < m_query_count; i++)
				{
					if(i > 0) url_stream << "&";

					url_stream << Escape(m_queries[i].m_field) << "=" << Escape(m_queries[i].m_value);
				}
			}

			// add the fragment
			if(m_fragment.length())
				url_stream << "#" << Escape(m_fragment);

			return url_stream.str();
		}
	};};
};