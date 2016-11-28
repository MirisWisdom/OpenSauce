/*
	Yelo: Open Sauce SDK
		Halo 2 (Xbox) Edition

	See license\OpenSauce\Halo2_Xbox for specific license information
*/
#pragma once
#include "Objects/ObjectDefinitions.hpp"

namespace Yelo
{
	namespace GameState
	{
		struct s_device_data : TStructImpl(Enums::k_object_size_device - Enums::k_object_size_object)
		{
			enum { DATA_OFFSET = Enums::k_object_size_object };
		};

		struct s_device_machine_data : TStructImpl(Enums::k_object_size_machine - Enums::k_object_size_device)
		{
			enum { DATA_OFFSET = Enums::k_object_size_device };
			// hasn't changed since halo1
		};

		struct s_device_control_data : TStructImpl(Enums::k_object_size_control - Enums::k_object_size_device)
		{
			enum { DATA_OFFSET = Enums::k_object_size_device };
			// hasn't changed since halo1
		};

		struct s_device_lightfixture_data : TStructImpl(Enums::k_object_size_light_fixture - Enums::k_object_size_device)
		{
			enum { DATA_OFFSET = Enums::k_object_size_device };
			// hasn't changed since halo1
		};


		struct s_device_datum
		{
			s_object_data object;
			s_device_data device;
		};

		struct s_device_machine_datum : s_device_datum
		{
			s_device_machine_data machine;
		};

		struct s_device_control_datum : s_device_datum
		{
			s_device_control_data control;
		};

		struct s_device_lightfixture_datum : s_device_datum
		{
			s_device_lightfixture_data lightfixture;
		};
	};

	namespace TagGroups
	{
	};
};