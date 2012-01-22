/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"

namespace Yelo
{
	void PrepareToDropError(cstring text)
	{
		if(text == NULL) text = "(null)";

		MessageBox(NULL, text, "Prepare to Drop!", MB_OK | MB_ICONEXCLAMATION);
	}
};