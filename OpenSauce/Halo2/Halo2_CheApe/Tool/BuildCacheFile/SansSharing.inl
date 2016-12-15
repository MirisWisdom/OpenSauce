/*
    Yelo: Open Sauce SDK
		Halo 2 (Editing Kit) Edition

	See license\OpenSauce\Halo2_CheApe for specific license information
*/

/* shared tag scheme removal changes

.text:005887DF                 push    1               ; CHANGE THIS TO FALSE
.text:005887E1                 mov     esi, eax
.text:005887E3                 call    _postprocess_animation_data

.text:00588C21                 push    1               ; CHANGE THIS TO FALSE
.text:00588C23                 push    edx
.text:00588C24                 mov     esi, eax
.text:00588C26                 call    _build_cache_file_prepare_sound_gestalt

.text:00588D52                 push    1               ; CHANGE THIS TO FALSE
.text:00588D54                 push    0
.text:00588D56                 push    0
.text:00588D58                 push    0
.text:00588D5A                 lea     edx, [esp+1F94h+multiplayer_shared_dependency_graph]
.text:00588D61                 mov     esi, eax
.text:00588D63                 push    edx
.text:00588D64                 lea     eax, [esp+1F98h+cache_header]
.text:00588D6B                 push    eax
.text:00588D6C                 lea     edx, [esp+1F9Ch+scenario_name_wide]
.text:00588D70                 call    _build_cache_file_add_tags ; ecx = s_shared_tag_index

.text:00588DCD                 push    1               ; CHANGE THIS TO FALSE
.text:00588DCF                 mov     ecx, edi
.text:00588DD1                 mov     esi, eax
.text:00588DD3                 call    _build_cache_file_add_sound_samples

.text:00588E37                 push    1               ; CHANGE THIS TO FALSE
.text:00588E39                 push    ecx
.text:00588E3A                 mov     esi, eax
.text:00588E3C                 call    _build_cache_file_sound_gestalt

.text:00588E99                 push    1               ; CHANGE THIS TO FALSE
.text:00588E9B                 lea     edx, [esp+1F88h+cache_header]
.text:00588EA2                 push    edx
.text:00588EA3                 push    edi
.text:00588EA4                 mov     esi, eax
.text:00588EA6                 call    _build_cache_file_add_geometry_blocks

.text:0058908E                 push    1               ; CHANGE THIS TO FALSE
.text:00589090                 mov     esi, eax
.text:00589092                 call    _language_packing

.text:005890A7                 push    1               ; CHANGE THIS TO FALSE
.text:005890A9                 push    ecx
.text:005890AA                 call    _build_cache_file_add_language_packs

.text:00589105                 push    1               ; CHANGE THIS TO FALSE
.text:00589107                 push    edi
.text:00589108                 mov     esi, eax
.text:0058910A                 call    _build_cache_file_add_bitmap_pixels

.text:00589181                 push    1               ; CHANGE THIS TO FALSE
.text:00589183                 push    1400000h        ; base address
.text:00589188                 push    edx
.text:00589189                 mov     esi, eax
.text:0058918B                 push    edi
.text:0058918C                 lea     eax, [esp+1F94h+multiplayer_shared_dependency_graph]
.text:00589193                 push    eax
.text:00589194                 lea     ecx, [esp+1F98h+cache_header]
.text:0058919B                 push    ecx
.text:0058919C                 mov     ecx, [esp+1F9Ch+multiplayer_shared_tag_index_sorted]
.text:005891A0                 lea     edx, [esp+1F9Ch+scenario_name_wide]
.text:005891A4                 call    _build_cache_file_add_tags ; ecx = s_shared_tag_index

*/

namespace build_cache_file_ex_hacks
{
	namespace sans_sharing
	{
		enum {
			k_tag_sharing_expected_tag_count = 0x2710,
			k_tag_memory_virtual_address_pc = 0x1400000, 
		};

		static Cache::s_shared_dependency_graph* build_cache_shared_dependency_graph;
		static Cache::s_shared_tag_index* build_cache_shared_tag_index_sorted;

		static void* TAG_SHARING_LOAD_MULTIPLAYER_SHARED		= CAST_PTR(void*, 0x5813C0);
		static void* TAG_SHARING_LOAD_MULTIPLAYER_SHARED_CALL	= CAST_PTR(void*, 0x5883DE);
		static API_FUNC_NAKED bool PLATFORM_API tag_sharing_load_multiplayer_shared_nullsub(Cache::s_shared_tag_index* shared_tag_index, Cache::s_shared_tag_index* out_shared_tag_index_sorted)
		{
			API_FUNC_NAKED_START()

				mov		eax, out_shared_tag_index_sorted
				mov		build_cache_shared_tag_index_sorted, eax
				mov		build_cache_shared_dependency_graph, ecx
				//xor		esi, esi // we do this trick here so some later code doesn't use the fucking shared tag index
				xor		eax, eax
				mov		al, 1

			API_FUNC_NAKED_END(0)
		}

		static void shared_dependency_graph_clear(Cache::s_shared_dependency_graph* graph)
		{
			Cache::s_shared_dependency_graph_buffer* buffer = graph->buffer;
			if(buffer != NULL)
				for(int32 x = 0; x < buffer->count; x++)
					if(!buffer->entries[x].IsNull())
						buffer->entries[x].Nullify();
		}
		static void shared_tag_index_clear(Cache::s_shared_tag_index* shared_tag_index)
		{
			memset(shared_tag_index->entries, 0, sizeof(shared_tag_index->entries));
			shared_tag_index->count = 0;
		}
		static void API_FUNC shared_tag_data_clear(Cache::s_shared_tag_index* shared_tag_index)
		{
			shared_dependency_graph_clear(build_cache_shared_dependency_graph);
			shared_tag_index_clear(shared_tag_index);
			//shared_tag_index_clear(build_cache_shared_tag_index_sorted);
		}
		static void* TAG_GROUPS_INITIALIZE_FOR_BUILD_CACHE_CALL	= CAST_PTR(void*, 0x588410);
		static API_FUNC_NAKED void PLATFORM_API tag_groups_initialize_for_build_cache_interceptor(Cache::s_shared_tag_index* shared_tag_index)
		{
			static uint32 CALL_ADDRESS = 0x580AF0;
			static uint32 global_shared_tag_index = 0xA801DC;

			API_FUNC_NAKED_START()

				push	shared_tag_index
				call	shared_tag_data_clear

				push	shared_tag_index
				call	CALL_ADDRESS
				add		esp, 4 * 1

				mov		eax, global_shared_tag_index
				mov		[eax], 0

			API_FUNC_NAKED_END(0)
		}

		static void* BUILD_CACHE_FILE_FOR_SCENARIO__ADD_DEBUG_TAG_NAMES_CALL = CAST_PTR(void*, 0x589031);
		static API_FUNC_NAKED void build_cache_file_for_scenario__intercept_add_debug_tag_names(void* cache_header, void* shared_tag_index)
		{
			static uint32 CALL_ADDRESS = 0x580AF0;
			static uint32 JMP_ADDRESS = 0x589036;

			API_FUNC_NAKED_START()

				push	0
				push	cache_header
				call	CALL_ADDRESS
				add		esp, 4 * 2
				//jmp		JMP_ADDRESS

			API_FUNC_NAKED_END(0)
		}

		static int32* _tag_sharing_expected_tag_count		= CAST_PTR(int32*,0x5817C7);
		static bool* _postprocess_animation_data			= CAST_PTR(bool*, 0x5887E0);
		static bool* _build_cache_file_prepare_sound_gestalt= CAST_PTR(bool*, 0x588C22);
		static bool* _build_cache_file_add_tags__preprocess	= CAST_PTR(bool*, 0x588D53);
		static bool* _build_cache_file_add_sound_samples	= CAST_PTR(bool*, 0x588DCE);
		static bool* _build_cache_file_sound_gestalt		= CAST_PTR(bool*, 0x588E38);
		static bool* _build_cache_file_add_geometry_blocks	= CAST_PTR(bool*, 0x588E9A);
		static bool* _language_packing						= CAST_PTR(bool*, 0x58908F);
		static bool* _build_cache_file_add_language_packs	= CAST_PTR(bool*, 0x5890A8);
		static bool* _build_cache_file_add_bitmap_pixels	= CAST_PTR(bool*, 0x589106);
		static bool* _build_cache_file_add_tags				= CAST_PTR(bool*, 0x589182);
		static void** _tag_memory_virtual_address			= CAST_PTR(void**,0x589184);

		void Initialize(bool scenario_is_mp, bool use_tags_base_address = false, void* tags_base_address = NULL)
		{
//			*_tag_sharing_expected_tag_count							= 0;
			Memory::WriteRelativeCall(&tag_sharing_load_multiplayer_shared_nullsub, TAG_SHARING_LOAD_MULTIPLAYER_SHARED_CALL);
			Memory::WriteRelativeCall(&tag_groups_initialize_for_build_cache_interceptor, TAG_GROUPS_INITIALIZE_FOR_BUILD_CACHE_CALL);

	//		Memory::WriteRelativeCall(&build_cache_file_for_scenario__intercept_add_debug_tag_names, 
	//			BUILD_CACHE_FILE_FOR_SCENARIO__ADD_DEBUG_TAG_NAMES_CALL);

								*_postprocess_animation_data			= false;
			if(!scenario_is_mp)	*_build_cache_file_prepare_sound_gestalt= false;
								*_build_cache_file_add_tags__preprocess	= false;
								*_build_cache_file_add_sound_samples	= false;
			if(!scenario_is_mp)	*_build_cache_file_sound_gestalt		= false;
								*_build_cache_file_add_geometry_blocks	= false;
			if(!scenario_is_mp)	*_language_packing						= false;
			if(!scenario_is_mp)	*_build_cache_file_add_language_packs	= false;
								*_build_cache_file_add_bitmap_pixels	= false;
								*_build_cache_file_add_tags				= false;

			if(use_tags_base_address)
				*_tag_memory_virtual_address = tags_base_address;
		}
		void Dispose()
		{
//			*_tag_sharing_expected_tag_count		= k_tag_sharing_expected_tag_count;
//			Memory::WriteRelativeCall(TAG_SHARING_LOAD_MULTIPLAYER_SHARED, TAG_SHARING_LOAD_MULTIPLAYER_SHARED_CALL);

			*_postprocess_animation_data			= true;
//			*_build_cache_file_prepare_sound_gestalt= true;
			*_build_cache_file_add_tags__preprocess	= true;
			*_build_cache_file_add_sound_samples	= true;
//			*_build_cache_file_sound_gestalt		= true;
			*_build_cache_file_add_geometry_blocks	= true;
//			*_language_packing						= true;
//			*_build_cache_file_add_language_packs	= true;
			*_build_cache_file_add_bitmap_pixels	= true;
			*_build_cache_file_add_tags				= true;

			*_tag_memory_virtual_address = CAST_PTR(void*, k_tag_memory_virtual_address_pc);
		}
	};
};