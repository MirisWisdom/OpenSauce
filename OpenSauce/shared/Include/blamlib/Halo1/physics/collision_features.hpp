/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

namespace Yelo
{
	namespace Enums
	{
		enum {
			k_maximum_collision_features_per_test = 256,
			k_maximum_points_per_collision_prism = 8,
		};

		enum collision_feature
		{
			_collision_feature_sphere,
			_collision_feature_cylinder,
			_collision_feature_prism,

			k_number_of_collision_features
		};
	};

	namespace Physics
	{
		struct s_collision_features
		{
			int16 count[Enums::k_number_of_collision_features];
			PAD16;

			byte spheres[0x1C][Enums::k_maximum_collision_features_per_test];
			// 0x0 ?
			// 0xC real_point3d base
			// 0x18 real_point3d height
			// 0x24 real radius
			byte cylinders[0x28][Enums::k_maximum_collision_features_per_test];
			// 0x24 int32 point_count
			byte prisms[0x68][Enums::k_maximum_collision_features_per_test];
		}; BOOST_STATIC_ASSERT( sizeof(s_collision_features) == 0xAC08 );
	};
};