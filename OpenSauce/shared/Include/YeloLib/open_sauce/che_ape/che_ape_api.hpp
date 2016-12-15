/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#pragma once

// CheApeApi declarations
extern "C" {
	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Get the __TIMESTAMP__ value of CheApe's precompiled header. </summary>
	///
	/// <param name="nBufferLength">	Length of the buffer in characters. </param>
	/// <param name="lpBuffer">			The (ASCII) character buffer to store the result. </param>
	///
	/// <returns>	true if it succeeds, false if it fails. </returns>
	bool __declspec( dllexport ) CheApeApi_GetPchBuildDateA(__in DWORD nBufferLength,
		__out_ecount_part_opt(nBufferLength, return + 1) LPSTR lpBuffer);
	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Get the __TIMESTAMP__ value of CheApe's precompiled header. </summary>
	///
	/// <param name="nBufferLength">	Length of the buffer in characters. </param>
	/// <param name="lpBuffer">			The (UNICODE) character buffer to store the result. </param>
	///
	/// <returns>	true if it succeeds, false if it fails. </returns>
	bool __declspec( dllexport ) CheApeApi_GetPchBuildDateW(__in DWORD nBufferLength,
		__out_ecount_part_opt(nBufferLength, return + 1) LPWSTR lpBuffer);

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Get the __FILE__ value of CheApe's precompiled header. </summary>
	///
	/// <param name="nBufferLength">	Length of the buffer in characters. </param>
	/// <param name="lpBuffer">			The (ASCII) character buffer to store the result. </param>
	///
	/// <returns>	true if it succeeds, false if it fails. </returns>
	bool __declspec( dllexport ) CheApeApi_GetPchPathA(__in DWORD nBufferLength,
		__out_ecount_part_opt(nBufferLength, return + 1) LPSTR lpBuffer);
	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Get the __FILE__ value of CheApe's precompiled header. </summary>
	///
	/// <param name="nBufferLength">	Length of the buffer in characters. </param>
	/// <param name="lpBuffer">			The (UNICODE) character buffer to store the result. </param>
	///
	/// <returns>	true if it succeeds, false if it fails. </returns>
	bool __declspec( dllexport ) CheApeApi_GetPchPathW(__in DWORD nBufferLength,
		__out_ecount_part_opt(nBufferLength, return + 1) LPWSTR lpBuffer);

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Get the tool name this build of CheApe is for. </summary>
	///
	/// <param name="nBufferLength">	Length of the buffer in characters. </param>
	/// <param name="lpBuffer">			The (UNICODE) character buffer to store the result. </param>
	///
	/// <returns>	true if it succeeds, false if it fails. </returns>
	/// <remarks>	Either, "Guerilla", "Tool", or "Sapien". </remarks>
	bool __declspec( dllexport ) CheApeApi_GetTargetToolNameW(__in DWORD nBufferLength,
		__out_ecount_part_opt(nBufferLength, return + 1) LPWSTR lpBuffer);
};

namespace Yelo
{
	namespace Enums
	{
		enum {
			k_cheape_cache_header_signature = 'head',
			k_cheape_cache_footer_signature = 'tail',
			k_cheape_cache_version = 2,

			k_cheape_cache_signature_halo1 = 'blm1',
			k_cheape_cache_signature_halo2 = 'blm2',

			k_max_number_of_new_tag_groups = 64,

			// Address where the CheApe.map memory resources are based at
			k_cheape_physical_memory_map_address=	0x50000000,
			// Size of the memory allocation for the CheApe.map resources
			k_cheape_physical_memory_map_size=		0x00600000,
		};
	};

	namespace CheApe
	{
		// Name of the memory map file with definitions for CheApe to load
		extern cstring const k_cache_file_name;

		struct s_cache_header
		{
			tag Head;
			uint32 Version;
			tag EngineSignature;	// Signature for the engine this is for (eg, Halo1, Halo2, etc)
			PAD32;

			void* BaseAddress;
			uint32 DataOffset;
			struct {
				int32 Size;
				void* Address;
			}StringPool;

			struct {
				struct {
					int32 Count;
					// address to a list of pointers to the scripting definitions
					void** Address;
				}Functions, Globals;
			}Scripting;

			struct {
				int32 Count;
				// address to a list of pointers to the fix-up definitions
				void** Address;
			}Fixups;
			PAD32;
			PAD32;

			uint32 Pad[495];
			tag Tail;

			inline bool SignaturesValid() const	{ return Head == Enums::k_cheape_cache_header_signature && Tail == Enums::k_cheape_cache_footer_signature; }
			inline bool VersionIsValid() const	{ return Version == Enums::k_cheape_cache_version; }
			// Is the cache for 1st gen engine tools? IE, Halo Custom Edition
			inline bool IsFirstGenCache() const	{ return EngineSignature == Enums::k_cheape_cache_signature_halo1 || EngineSignature == 0; }
			// Is the cache for 2nd gen engine tools? IE, Halo 2 Vista
			inline bool IsSecondGenCache() const{ return EngineSignature == Enums::k_cheape_cache_signature_halo2; }

			// Returns the reason why this header is invalid or NULL if everything appears gravy
			cstring GetInvalidReasonString(tag engine_sig, void* base_address) const;
		}; BOOST_STATIC_ASSERT( sizeof(s_cache_header) == 0x800 );
		s_cache_header& GlobalCacheHeader();

		/*!
		 * \brief
		 * Get a debug resource buffer to add to a cache file which uses CheApe data
		 * 
		 * \param buffer
		 * On return, this will point to allocated memory which represents the CheApe resource 
		 * which is to be added to a cache file being built. If this function fails, this will be null.
		 * 
		 * \param size
		 * The raw-size of [buffer]. If [buffer] is internally compressed, this will be the decompressed size
		 * 
		 * \param compressed_size
		 * The size of [buffer]. If no compression is used, this will be 0.
		 * 
		 * \returns
		 * True if this function succeeded.
		 * 
		 * \remarks
		 * User code MUST 'delete' the memory pointed to by [buffer].
		 */
		bool GetCacheFileResourceBuffer(__out void*& buffer, __out uint32& size, __out uint32& compressed_size);
	};
};