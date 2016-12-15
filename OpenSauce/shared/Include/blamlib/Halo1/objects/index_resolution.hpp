/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

namespace Yelo
{
	namespace MessageDeltas
	{
		struct s_index_resolution_table;
	};

	namespace Objects
	{
	};

	namespace blam
	{
		int32 PLATFORM_API index_resolution_table_translate(MessageDeltas::s_index_resolution_table& table, datum_index local_index);
	};
};