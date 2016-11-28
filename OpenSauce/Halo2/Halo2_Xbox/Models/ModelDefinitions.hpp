/*
	Yelo: Open Sauce SDK
		Halo 2 (Xbox) Edition

	See license\OpenSauce\Halo2_Xbox for specific license information
*/
#pragma once

namespace Yelo
{
	namespace Enums
	{
		enum geometry_block_resource_type
		{
			_geometry_block_resource_type_block,		// tag_block
			_geometry_block_resource_type_data,			// tag_data
			_geometry_block_resource_type_vertex_buffer,
			_geometry_block_resource_type,
		};
	};

	namespace Models
	{
		struct geometry_block_resource
		{
			byte_enum Type;
			PAD24;
			uint16 PrimaryLocator, SecondaryLocator;
			uint32 ResourceDataSize;
			uint32 ResourceDataOffset;
		};
		struct geometry_block_info
		{
			uint32 BlockOffset;
			uint32 BlockSize;
			uint32 SectionDataSize;
			uint32 ResourceDataSize;
			TagBlock<geometry_block_resource> Resources;
			PAD32;
			uint16 OwnerTagSectionOffset;
			PAD16;
			PAD32;
		};
	};
};