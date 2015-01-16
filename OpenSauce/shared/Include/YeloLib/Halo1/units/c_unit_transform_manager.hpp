/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#include <blamlib/Halo1/game/game_allegiance.hpp>

namespace Yelo
{
	namespace Objects
	{
		struct s_damage_data;
		struct s_unit_datum;
	};

	namespace TagGroups
	{
		struct s_unit_transform_definition;
		struct s_unit_transform_target;
	};

	namespace Objects { namespace Units { namespace Transform
	{
		class c_unit_transform_manager
		{
			typedef std::vector<const TagGroups::s_unit_transform_definition*> unit_transform_definition_list_t;
			
			struct s_unit_transform_set
			{
				const TagGroups::s_unit_transform_definition* m_default_transform;
				unit_transform_definition_list_t m_transforms;

				s_unit_transform_set()
					: m_default_transform(nullptr)
					, m_transforms()
				{ }
			};

			struct s_unit_transform_state
			{
				const TagGroups::s_unit_transform_definition* m_unit_transform;
				Enums::game_team m_instigator_team;
			};

			bool m_transforms_allowed;
			PAD24;
			std::map<datum_index::index_t, s_unit_transform_set> m_unit_transform_map;
			std::map<datum_index::index_t, s_unit_transform_state> m_transforms_in_progress;

		public:
			c_unit_transform_manager();

		private:
			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Searches for a matching unit transform definition. </summary>
			///
			/// <param name="unit_tag_index">	   	Datum index of the unit's tag. </param>
			/// <param name="instigator_tag_index">	Datum index of the instigator's tag. </param>
			/// <param name="damage_is_melee">	   	Whether the damage is melee damage. </param>
			///
			/// <returns>	null if it fails, else the found unit transform. </returns>
			const TagGroups::s_unit_transform_definition* FindUnitTransform(const datum_index unit_tag_index
				, const datum_index instigator_tag_index
				, const bool damage_is_melee);

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Gets a random transform target. </summary>
			///
			/// <param name="transform_definition">	The transform definition. </param>
			///
			/// <returns>	null if it fails, else the transform target. </returns>
			const TagGroups::s_unit_transform_target& GetTransformTarget(const TagGroups::s_unit_transform_definition& transform_definition);

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>
			/// 	Attach the transform definition's attachment objects to the target unit at the specific
			/// 	marker permutation.
			/// </summary>
			///
			/// <param name="unit_index">			   	Datum index of the unit. </param>
			/// <param name="unit_datum">			   	The unit datum. </param>
			/// <param name="object_type">			   	The type of object. </param>
			/// <param name="object_marker_name">	   	The object marker to attach at. </param>
			/// <param name="destination_marker_name"> 	The destination marker name. </param>
			/// <param name="destination_marker_index">	The index of the destination marker. </param>
			void TransformAttachObject(const datum_index unit_index
				, const s_unit_datum* unit_datum
				, const datum_index object_type
				, const tag_string& object_marker_name
				, const tag_string& destination_marker_name
				, const int32 destination_marker_index);

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Attach the transform definition's attachment objects to the target unit. </summary>
			///
			/// <param name="unit_index">		   	Datum index of the unit. </param>
			/// <param name="unit_datum">		   	The unit datum. </param>
			/// <param name="transform_definition">	The transform definition. </param>
			void TransformAttachObjects(const datum_index unit_index
				, const s_unit_datum* unit_datum
				, const TagGroups::s_unit_transform_definition& transform_definition);

			void TransformEnd(s_unit_datum* unit_datum);

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Start's the transform in stage on the target unit. </summary>
			///
			/// <param name="unit_index">	Datum index of the unit. </param>
			/// <param name="unit_datum">	The unit datum. </param>
			void TransformIn(const datum_index unit_index
				, s_unit_datum* unit_datum
				, const s_unit_transform_state& transform_state);

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Starts a the transform out stage on the target unit. </summary>
			///
			/// <param name="unit_index">		   	Datum index of the unit. </param>
			/// <param name="unit_datum">		   	The unit datum. </param>
			/// <param name="transform_definition">	The transform definition. </param>
			void TransformOut(const datum_index unit_index
				, s_unit_datum* unit_datum
				, const TagGroups::s_unit_transform_definition& transform_definition);

		public:
			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Gets whether transforms allowed. </summary>
			///
			/// <returns>	true if allowed, false if not. </returns>
			bool GetTransformsAllowed();
			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Sets whether transforms are allowed. </summary>
			///
			/// <param name="value">	The value. </param>
			void SetTransformsAllowed(const bool value);

			/// <summary>	Loads all transform definitions. </summary>
			void LoadTransformDefinitions();

			/// <summary>	Unload transform definitions. </summary>
			void UnloadTransformDefinitions();

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Attempts to start a unit transform on a damaged unit if applicable. </summary>
			///
			/// <param name="unit_index"> 	Datum index of the unit. </param>
			/// <param name="damage_data">	Information describing the damage. </param>
			void UnitDamaged(const datum_index unit_index, const Objects::s_damage_data* damage_data);

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Unit transform update. </summary>
			///
			/// <param name="unit_index">	Datum index of the unit. </param>
			void UnitUpdate(const datum_index unit_index);
		};
	};};}
};