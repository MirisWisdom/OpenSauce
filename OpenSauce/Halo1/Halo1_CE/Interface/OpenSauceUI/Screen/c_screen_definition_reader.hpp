/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#if !PLATFORM_IS_DEDI

#include <YeloLib/Halo1/files/packed_file.hpp>

#include "Interface/OpenSauceUI/Definitions/c_screen_definition.hpp"

namespace Yelo
{
	namespace Interface { namespace OpenSauceUI { namespace Screen
	{
		/// <summary>	A screen definition registry. </summary>
		class c_screen_definition_registry final
		{
		public:
			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Gets a screen definition. </summary>
			///
			/// <param name="ui_package">	[in] The ui package. </param>
			/// <param name="screen_id"> 	Identifier for the screen. </param>
			/// <param name="output">	 	[out] The output definition. </param>
			///
			/// <returns>	true if it succeeds, false if it fails. </returns>
			static bool GetScreenDefinition(c_packed_file& ui_package, const std::string& screen_id, Definitions::c_screen_definition& output);
		};
	};};};
};
#endif