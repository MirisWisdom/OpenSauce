/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

namespace Yelo
{
	namespace Objects { namespace Units
	{
		void Initialize();
		void Dispose();
		void InitializeForNewMap();
		void DisposeFromOldMap();
		
		void ObjectsUpdate();

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>Initialize the Units system for a new map before any of the actual initialize_for_new_map procedures are
		/// 	executed This way we're able to ascertain state changes from the old map with the new map. Eg, differing unit
		/// 	grenade type counts.
		/// </summary>
		void InitializeForNewMapPrologue();

		void InitializeForYeloGameState(bool enabled);
	}; };
};