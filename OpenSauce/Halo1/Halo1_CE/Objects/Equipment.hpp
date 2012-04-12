/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

namespace Yelo
{
	namespace Objects { 

		struct s_object_type_definition;
		
		namespace Equipment {
		void InitializeType(s_object_type_definition* equipment_type);

		bool PLATFORM_API New(datum_index equipment_index);
		void PLATFORM_API Delete(datum_index equipment_index);
		bool PLATFORM_API Update(datum_index equipment_index);
		void PLATFORM_API ExportFunctionValues(datum_index equipment_index);
	}; };
};