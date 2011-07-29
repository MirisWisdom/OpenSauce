/*
	Yelo: Open Sauce SDK
		Halo 2 (Xbox) Edition

	See license\OpenSauce\Halo2_Xbox for specific license information
*/
#include "Common/Precompile.hpp"
#include "Memory/Config.hpp"

namespace Yelo
{
	namespace Config
	{
		void Initialize()
		{
			Current().Input.Tolerance = real(0.225f);
		}

		void Dispose()
		{
		}

		void InitializeForNewConfig()
		{
		}

		void DisposeFromOldConfig()
		{
		}

		static int32 CurrentDefinition = 0;
		int32& CurrentIndex() { return CurrentDefinition; }

		s_definition& Current()
		{
			static s_definition _Definitions[Enums::k_config_definition_count] ={
				{0},
				{0},
				{0}
			};
			return _Definitions[CurrentDefinition];
		}
	};
};