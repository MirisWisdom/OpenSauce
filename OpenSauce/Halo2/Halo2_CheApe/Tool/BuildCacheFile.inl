/*
    Yelo: Open Sauce SDK
		Halo 2 (Editing Kit) Edition

	See license\OpenSauce\Halo2_CheApe for specific license information
*/

#include "Tool/BuildCacheFile/SansSharing.inl"
#include "Tool/BuildCacheFile/CampaignSharing.inl"
#include "Tool/BuildCacheFile/CompactableDataCodeFlow.inl"

static void PLATFORM_API build_cache_file_ex_proc(wcstring* arguments)
{
	static _tool_command_proc _build_cache_file = CAST_PTR(_tool_command_proc, 0x5894A0);

	void* base_address = NULL;
	wcstring base_address_arg = arguments[1];
	if(base_address_arg != NULL)
		base_address = CAST_PTR(void*, _wtol(base_address_arg));

	build_cache_file_ex_hacks::sans_sharing::Initialize(true, base_address_arg != NULL, base_address);
	//build_cache_file_ex_hacks::campaign_sharing::Initialize();
	//build_cache_file_ex_hacks::compactable_data_code_flow::Initialize();
	_build_cache_file(arguments);
}