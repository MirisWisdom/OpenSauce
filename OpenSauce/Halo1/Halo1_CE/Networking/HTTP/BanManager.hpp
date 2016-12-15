/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#if PLATFORM_IS_DEDI

namespace Yelo
{
	namespace Enums
	{
		enum http_ip_ban_ip_version
		{
			_http_ip_ban_ip_version_ipv4,
			_http_ip_ban_ip_version_ipv6, // mongoose is only ipv4 atm, but that could change

			_http_ip_ban_ip_version
		};
	};
	namespace Networking { namespace HTTP { namespace Server { namespace BanManager
	{
		bool IPBanned(uint32 ip);
		bool IPBanned(Enums::http_ip_ban_ip_version version, uint16 a, uint16 b, uint16 c, uint16 d, uint16 e, uint16 f, uint16 g, uint16 h);

		void AddConnection(uint32 ip);
		void UpdateConnectionBans(real delta);

		void Initialize();
		void Dispose();

		void* HTTPServerSetConnectionBan(void** arguments);
		void* HTTPServerBanlist();
		void* HTTPServerBanlistFile(void** arguments);
		void* HTTPServerBanIP(void** arguments);
		void* HTTPServerUnbanIP(void** arguments);
	};};};};
};
#endif