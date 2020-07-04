/*
	Yelo: Open Sauce SDK
		Halo 2 (Xbox) Edition

	See license\OpenSauce\Halo2_Xbox for specific license information
*/
#pragma once
#include "HS/HS.hpp"

namespace Yelo
{
	namespace Scenario
	{
		struct hs_script
		{
			tag_string Name;
			_enum ScriptType;
			_enum ReturnType;
			datum_index RootExpressionIndex;
		};

		struct hs_global_internal
		{
			tag_string Name;
			_enum Type;
			PAD16;
			datum_index InitializationExpressionIndex;
		};

		struct cs_point
		{
			tag_string Name;
			real_point3d Position;
			int16 ReferenceFrame;
			PAD16;
			int32 SurfaceIndex;
			real_euler_angles2d FacingDirection;
		};
		struct cs_point_set
		{
			tag_string Name;
			TagBlock<cs_point> Points;
			int16 BspIndex;
			int16 ManualReferenceFrame;
			long_flags Flags;
		};
		struct cs_script_data
		{
			TagBlock<cs_point_set> PointSets;
			TAG_PAD(int32, 30);
		};


		struct s_hs_unit_seat_mapping
		{
			datum_index Unit; // unit tag index
			uint32 Unknown; // maybe a bit-vector?
		};
	};
};