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
		enum CacheType
		{
			_CacheTypeSinglePlayer,
			_CacheTypeMultiplayer,
			_CacheTypeMainmenu,
			_CacheTypeShared,
			_CacheTypeSharedSp,
			_CacheType,
		};

		enum Cache
		{
			k_CacheSizeLimitSinglePlayer= 0x11800000,
			k_CacheSizeLimitMultiplayer	= 0x05000000,
			k_CacheSizeLimitMainmenu	= 0x20800000,
			k_CacheSizeLimitShared		= 0x0B400000,
			k_CacheSizeLimitSharedSp	= 0x20800000,

			k_CacheHeaderHead			= 'head',
			k_CacheHeaderTail			= 'foot',
			k_CacheHeaderVersion		= 8,
			k_CacheIndexSignature		= 'tags',
		};
	};

	namespace Cache
	{
		struct s_header
		{
			// 'head'
			tag HeadSignature;
			// Should be 8
			uint32 Version;
			// Refer to Enums::_Cache::_CacheSizeLimit* for map type sizes
			uint32 Length;
			UNKNOWN_TYPE(int32);
			// Offset in the file to the CacheIndex
			int32 IndexOffset;
			// Size of the index data (includes index, group tags, index datums, and some unknown binary data)
			int32 IndexSize;
			int32 TagDataSize;
			int32 UnknownSize;
			// If this map was loaded by the game, this is filled in with the original file path
			string256 OrginalPath;
			// Build Version
			tag_string Build;
			// Enums::CacheType
			_enum Type;
			PAD16;
			TAG_PAD(int32, 2);
			FILETIME FileTime;

			uint32 UNKNOWN(2);
			int32 BadTagListOffset;
			uint32 UNKNOWN(3);

			struct _StringTable {
				int32 Offset;
				int32 Count;
				struct _Compressed {
					int32 Size;
					int32 IndicesOffset;
					int32 Offset;
				}Compressed;
			}StringTable;

			struct {
				bool Required[4]; // [0] mainmenu, [1] shared, [2] shared sp, [4] pad
				FILETIME BuildDate[4]; // [0] this, [1] mainmenu, [2] shared, [3] shared sp
			}Dependancy;

			// Name of the cache
			tag_string Name;
			PAD32;
			// Scenario tag path
			string256 Scenario;
			// > 0 if it needs shared
			uint32 NeedsShared; // i don't feel like making more pad fields for this pussy bool

#if PLATFORM_ID != PLATFORM_H2_ALPHA
			struct _TagPaths {
				uint32 Count;
				int32 Offset;
				int32 Size;
				int32 IndicesOffset;
			}TagPaths;
#else
			PAD32; PAD32; PAD32; PAD32;
#endif
			// The Xor checksum value of the cache
			uint32 Checksum;

			TAG_PAD(int32, 330);
			// 'foot'
			tag FootSignature;
		}; BOOST_STATIC_ASSERT( sizeof(s_header) == 2048 );

		// Cache Index Item structure
		struct s_tag_instance
		{
			// Group type
			tag GroupTag;
#if PLATFORM_ID == PLATFORM_H2_ALPHA
			PAD_TYPE(tag);
			PAD_TYPE(tag);
#endif
			// Datum Identifier
			datum_index Datum;
#if PLATFORM_ID == PLATFORM_H2_ALPHA
			cstring Filename;
#endif
			// Address of the Definition
			void* Definition;
			// Size of the definition
			uint32 Size;
#if PLATFORM_ID == PLATFORM_H2_ALPHA
			PAD32;
#endif
		};

		struct s_tag_index
		{
			// Cache Tag Group Item structure
			struct ItemTagGroup
			{
				tag Group;
				tag Parents[2];
			};

#if PLATFORM_ID != PLATFORM_H2_ALPHA
			// Address where the Cache Index is loaded to
			struct _TagGroups
			{
				ItemTagGroup* Groups;
				uint32 Count;
			}TagGroups;
#endif

			s_tag_instance* Items;
			// Scenario tag datum
			datum_index Scenario;

#if PLATFORM_ID != PLATFORM_H2_ALPHA
			// Globals tag datum
			datum_index Globals;
#endif

			uint32 UNKNOWN(0);
			uint32 Count;
			// 'tags'
			tag Signature;
		};

		void Initialize();
		void InitializeForNewMap();
		void Dispose();
	};

	namespace GameState
	{
		Cache::s_header* _CacheHeader();

		Cache::s_tag_index* _TagIndex();

		Cache::s_tag_instance* _TagInstances();
	};
};