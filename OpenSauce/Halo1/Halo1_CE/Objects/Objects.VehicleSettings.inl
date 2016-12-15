/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/

#include "Game/Players.hpp"

namespace Yelo
{
	namespace Objects
	{
		namespace Vehicle
		{
#if !PLATFORM_IS_DEDI
			struct vehicle_globals {
				size_t presets_count;
				struct s_seat_preset
				{
					int32 index;
					real_vector3d offset;
				};
				struct s_preset {
					char name[Enums::k_vehicle_view_name_length+1];
					std::array<uint32, BIT_VECTOR_SIZE_IN_DWORDS(Enums::k_vehicle_view_max_seat_presets)> 
						seats_bitvector;
					std::array<s_seat_preset, Enums::k_vehicle_view_max_seat_presets> 
						seats;
				};
				std::array<s_preset, Enums::k_vehicle_view_max_vehicle_presets> presets;

				s_preset* AddPreset(cstring name)
				{
					if(presets_count == presets.size())
						return nullptr;

					s_preset* preset = &presets[presets_count++];
					strcpy_s(preset->name, NUMBEROF(preset->name), name);

					return preset;
				}

			private:
				s_unit_datum* GetCurrentVehicle(int16* current_seat_index)
				{
					datum_index player_index = Players::LocalPlayerIndex();
					s_unit_datum* vehicle = Players::GetVehicle(player_index, current_seat_index);

					return vehicle ? vehicle : nullptr;
				}
			}_vehicle_globals;
#endif
		};
	};
};