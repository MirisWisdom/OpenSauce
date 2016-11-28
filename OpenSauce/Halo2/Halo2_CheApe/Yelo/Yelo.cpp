/*
    Yelo: Open Sauce SDK
		Halo 2 (Editing Kit) Edition

	See license\OpenSauce\Halo2_CheApe for specific license information
*/
#include "Common/Precompile.hpp"
#include "CheApeInterface.hpp"

#include "TagGroups/TagGroups.hpp"

namespace Yelo
{
	// Maybe we should just rename the individual namespaces 
	// to just 'HEK' ?
	namespace PLATFORM_VALUE(Guerilla, Tool, Sapien)
	{
		void Initialize();
		void Dispose();
	};

	void Initialize()
	{
		PLATFORM_VALUE(Guerilla, Tool, Sapien)::Initialize();

		//TagGroups::Initialize();
	}

	void Dispose()
	{
		PLATFORM_VALUE(Guerilla, Tool, Sapien)::Dispose();

		TagGroups::Dispose();
	}
};