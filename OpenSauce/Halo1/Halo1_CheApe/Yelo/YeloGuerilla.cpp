/*
	Yelo: Open Sauce SDK
		Halo 1 (Editing Kit) Edition

	See license\OpenSauce\Halo1_CheApe for specific license information
*/
#include "Common/Precompile.hpp"

#if PLATFORM_TYPE == PLATFORM_GUERILLA
#include "Guerilla/field_dialog_extensions.hpp"
#include "Guerilla/guerilla_application_extensions.hpp"
#include "Guerilla/string_dialog_extensions.hpp"

namespace Yelo
{
	namespace Guerilla
	{
		void IntializeBeforeTagGroupsInitalize()
		{
		}

		void Initialize()
		{
			string_dialog_extensions::Initialize();

			field_dialog_string_id_extensions::Initialize();

			application_extensions::Initialize();
		}

		void Dispose()
		{
		}
	};
};
#endif