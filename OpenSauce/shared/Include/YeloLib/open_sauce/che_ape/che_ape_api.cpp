/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#include "Common/Precompile.hpp"
#include <YeloLib/open_sauce/che_ape/che_ape_api.hpp>

namespace Yelo
{
	namespace CheApe
	{
		cstring const k_cache_file_name = "CheApe.map";

		cstring s_cache_header::GetInvalidReasonString(tag engine_sig, void* base_address) const
		{
			if(!SignaturesValid())
				return "Invalid header signatures";
			else if(!VersionIsValid())
				return "Invalid cache version";
			else if((engine_sig == Enums::k_cheape_cache_signature_halo1 && !IsFirstGenCache()) ||
					(engine_sig == Enums::k_cheape_cache_signature_halo2 && !IsSecondGenCache()) )
				return "Invalid engine signature";
			else if(BaseAddress != base_address)
				return "Invalid base address";

			return nullptr;
		}
	};
};