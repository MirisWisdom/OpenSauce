/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include "Objects/Equipment.hpp"

#include <blamlib/Halo1/items/equipment_definitions.hpp>
#include <blamlib/Halo1/items/equipment_structures.hpp>

#include "Objects/Objects.hpp"

namespace Yelo
{
	namespace Objects { namespace Equipment {
#define __EL_INCLUDE_ID			__EL_INCLUDE_OBJECTS
#define __EL_INCLUDE_FILE_ID	__EL_OBJECTS_EQUIPMENT
#include "Memory/_EngineLayout.inl"

		void InitializeType(s_object_type_definition*);
		void Initialize()
		{
			if(true) return; // TODO: finish the equipment code then remove this

			InitializeType(ObjectTypeDefinitions()[Enums::_object_type_equipment]);
		}

		static bool PLATFORM_API New(datum_index equipment_index)
		{
			s_equipment_datum* equipment = Objects::ObjectHeader()[equipment_index]->_equipment;
			auto const* definition = GetObjectDefinition<TagGroups::s_equipment_definition>(equipment_index);

			return true;
		}

		static void PLATFORM_API Delete(datum_index equipment_index)
		{
		}

		static bool PLATFORM_API Update(datum_index equipment_index)
		{
			return true;
		}

		static void PLATFORM_API ExportFunctionValues(datum_index equipment_index)
		{
			s_equipment_datum* equipment = Objects::ObjectHeader()[equipment_index]->_equipment;
			auto const* definition = GetObjectDefinition<TagGroups::s_equipment_definition>(equipment_index);
		}

		static void InitializeType(s_object_type_definition* equipment_type)
		{
			equipment_type->new_ = &New;
			equipment_type->delete_ = &Delete;
			equipment_type->update = &Update;
			equipment_type->export_function_values = &ExportFunctionValues;
		}

	}; };
};