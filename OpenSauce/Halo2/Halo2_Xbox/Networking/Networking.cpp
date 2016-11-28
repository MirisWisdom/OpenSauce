/*
	Yelo: Open Sauce SDK
		Halo 2 (Xbox) Edition

	See license\OpenSauce\Halo2_Xbox for specific license information
*/
#include "Common/Precompile.hpp"
#include "Networking/Networking.hpp"

#include "Networking/NetworkingDefinitions.hpp"

namespace Yelo
{
#define __EL_INCLUDE_ID			__EL_INCLUDE_NETWORKING
#define __EL_INCLUDE_FILE_ID	__EL_NETWORKING_NETWORKING
#include "Memory/_EngineLayout.inl"

	namespace Networking
	{
		sockaddr_in* transport_address::to_sockaddr_in(sockaddr_in* sd) // however we could call 0x2FE510\0xB5470...
		{
			sd->sin_family = AF_INET;
			sd->sin_addr.s_addr =	((ip_ulong << 24) & 0xff000000) | // ntohl/htonl(ip_ulong)
									((ip_ulong <<  8) & 0x00ff0000) |
									((ip_ulong >>  8) & 0x0000ff00) |
									((ip_ulong >> 24) & 0x000000ff);
			sd->sin_port = (port << 8) | (port >> 8);
			return sd;
		}

		transport_address* transport_address_create(transport_address* address, Enums::transport_type type, int16 port)
		{
			address->address_length = type == Enums::_ipv4 ? Enums::k_ipv4_address_length : Enums::k_ipv6_address_length;
			address->port = port;
			return address;
		}

		transport_address* transport_address_create(transport_address* address, Enums::transport_type type, cstring ip, int16 port)
		{
			transport_address_create(address, type, port);
			uint32 ip_addr = XboxStd.inet_addr(ip);
			address->ip_ulong =	((ip_addr << 24) & 0xff000000) | // ntohl/htonl(ip_ulong)
								((ip_addr <<  8) & 0x00ff0000) | 
								((ip_addr >>  8) & 0x0000ff00) | 
								((ip_addr >> 24) & 0x000000ff);
			return address;
		}


		API_FUNC_NAKED transport_endpoint* transport_endpoint_create(_enum transport_type)
		{
			static uint32 CALL_ADDR = GET_FUNC_PTR(TRANSPORT_ENDPOINT_CREATE);

			API_FUNC_NAKED_START()

				movsx	eax, transport_type
				push	eax
				call	CALL_ADDR

			API_FUNC_NAKED_END(1);
		}

		API_FUNC_NAKED bool transport_endpoint_get_option(transport_endpoint* endpoint, long_enum socket_option)
		{
			static uint32 CALL_ADDR = GET_FUNC_PTR(TRANSPORT_ENDPOINT_GET_OPTION);

			API_FUNC_NAKED_START()
				
#if PLATFORM_ID != PLATFORM_H2_ALPHA
				mov		eax, socket_option
				push	eax
				mov		eax, endpoint
				push	eax
#else /*alpha*/
				mov		eax, endpoint
				push	eax
				mov		eax, socket_option
#endif
				call	CALL_ADDR

			API_FUNC_NAKED_END(2);
		}

		API_FUNC_NAKED bool transport_endpoint_set_option(transport_endpoint* endpoint, long_enum socket_option)
		{
			static uint32 CALL_ADDR = GET_FUNC_PTR(TRANSPORT_ENDPOINT_SET_OPTION);

			API_FUNC_NAKED_START()

#if PLATFORM_ID != PLATFORM_H2_ALPHA
				mov		eax, socket_option
				push	eax
				mov		eax, endpoint
				push	eax
#else /*alpha*/
				mov		eax, endpoint
				push	eax
				mov		eax, socket_option
#endif
				call	CALL_ADDR

			API_FUNC_NAKED_END(2);
		}

		API_FUNC_NAKED bool transport_endpoint_bind(transport_endpoint* endpoint, transport_address* address)
		{
			static uint32 CALL_ADDR = GET_FUNC_PTR(TRANSPORT_ENDPOINT_BIND);

			API_FUNC_NAKED_START()

#if PLATFORM_ID != PLATFORM_H2_ALPHA
				mov		eax, endpoint
				push	eax
				mov		eax, address
#else /*alpha*/
				mov		eax, address
				push	eax
				mov		eax, endpoint
#endif
				call	CALL_ADDR

			API_FUNC_NAKED_END(2);
		}

		API_FUNC_NAKED void transport_endpoint_close(transport_endpoint* endpoint)
		{
			static uint32 CALL_ADDR = GET_FUNC_PTR(TRANSPORT_ENDPOINT_CLOSE);

			API_FUNC_NAKED_START()
				push	esi

				mov		esi, endpoint
				call	CALL_ADDR

				pop		esi
			API_FUNC_NAKED_END(1);
		}

		API_FUNC_NAKED int16 transport_endpoint_read(transport_endpoint* endpoint, void* buffer, int16 buffer_length)
		{
			static uint32 CALL_ADDR = GET_FUNC_PTR(TRANSPORT_ENDPOINT_READ);

			API_FUNC_NAKED_START()

#if PLATFORM_ID != PLATFORM_H2_ALPHA
				push	esi

				movsx	eax, buffer_length
				push	eax
				mov		eax, buffer
				push	eax
				mov		esi, endpoint
				call	CALL_ADDR

				pop		esi
#else /*alpha*/

				mov		ecx, endpoint
				mov		ebx, buffer
				movsx	eax, buffer_length
				call	CALL_ADDR

#endif

			API_FUNC_NAKED_END(3);
		}

		API_FUNC_NAKED int16 transport_endpoint_write(transport_endpoint* endpoint, void* buffer, int16 buffer_length)
		{
			static uint32 CALL_ADDR = GET_FUNC_PTR(TRANSPORT_ENDPOINT_WRITE);

			API_FUNC_NAKED_START()

#if PLATFORM_ID != PLATFORM_H2_ALPHA
				push	esi

				movsx	eax, buffer_length
				push	eax
				mov		eax, buffer
				push	eax
				mov		esi, endpoint
				call	CALL_ADDR

				pop		esi
#else /*alpha*/

				mov		ecx, endpoint
				mov		ebx, buffer
				movsx	eax, buffer_length
				call	CALL_ADDR

#endif

			API_FUNC_NAKED_END(3);
		}

		API_FUNC_NAKED int16 transport_endpoint_read_from(transport_endpoint* endpoint, sockaddr_in* source, void* buffer, int16 buffer_length)
		{
			static uint32 CALL_ADDR = GET_FUNC_PTR(TRANSPORT_ENDPOINT_READ_FROM);

			API_FUNC_NAKED_START()

#if PLATFORM_ID != PLATFORM_H2_ALPHA
				push	esi

				movsx	eax, buffer_length
				push	eax
				mov		eax, buffer
				push	eax
				mov		eax, endpoint
				push	eax
				mov		esi, source
				call	CALL_ADDR

				pop		esi
#else /*alpha*/

				mov		ecx, endpoint
				mov		eax, source
				push	eax
				movsx	eax, buffer_length
				push	eax
				mov		eax, buffer
				call	CALL_ADDR

#endif

				API_FUNC_NAKED_END(4);
		}

		API_FUNC_NAKED int16 transport_endpoint_write_to(transport_endpoint* endpoint, transport_address* address, void* buffer, int16 buffer_length)
		{
			// fuck the alpha's function man, fuck it -_-
			static uint32 CALL_ADDR = GET_FUNC_PTR(TRANSPORT_ENDPOINT_WRITE_TO);

			API_FUNC_NAKED_START()
				push	esi

#if PLATFORM_ID != PLATFORM_H2_ALPHA
				movsx	esi, buffer_length
				push	esi
				mov		esi, buffer
				push	esi
				mov		esi, endpoint
				push	esi
				mov		esi, address
				call	CALL_ADDR
#else /*alpha*/
				xor		eax, eax
#endif

				pop		esi
			API_FUNC_NAKED_END(4);
		}

		API_FUNC_NAKED bool transport_endpoint_connect(transport_endpoint* endpoint, transport_address* address)
		{
			static uint32 CALL_ADDR = GET_FUNC_PTR(TRANSPORT_ENDPOINT_CONNECT);

			API_FUNC_NAKED_START()

#if PLATFORM_ID != PLATFORM_H2_ALPHA
				mov		eax, endpoint
				push	eax
				mov		eax, address
				call	CALL_ADDR
#else /*alpha*/

				mov		ecx, endpoint
				mov		eax, address
				call	CALL_ADDR

#endif

			API_FUNC_NAKED_END(2);
		}

		API_FUNC_NAKED bool transport_endpoint_async_is_connected(transport_endpoint* endpoint, bool* is_connected)
		{
			static uint32 CALL_ADDR = GET_FUNC_PTR(TRANSPORT_ENDPOINT_ASYNC_IS_CONNECTED);

			API_FUNC_NAKED_START()
				push	esi

				mov		esi, is_connected
				push	esi
				mov		esi, endpoint
				call	CALL_ADDR

				pop		esi
			API_FUNC_NAKED_END(2);
		}
	};
};