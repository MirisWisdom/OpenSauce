/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

namespace Yelo
{
	namespace Objects
	{
		struct s_damage_data;
	};

	namespace AI
	{
		void Initialize();
		void Dispose();

#if PLATFORM_TYPE == PLATFORM_SAPIEN
		void InitializeForNewGameState();
		void InitializeForNewMap();
		void DisposeFromOldMap();

		void ObjectsUpdate();
		void UnitDamageAftermath(const datum_index object_index, const Objects::s_damage_data* damage_data);
#endif
		namespace Transform
		{
			bool CollateTransformCollections();
		}
	};
};