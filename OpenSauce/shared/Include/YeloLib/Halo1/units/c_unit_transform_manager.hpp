/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#include <blamlib/Halo1/game/game_allegiance.hpp>

#include <YeloLib/tag_files/tag_groups_base_yelo.hpp>

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
		struct s_actor_variant_transform;
	};

	namespace Objects { namespace Units { namespace Transform
	{
		class c_unit_transform_manager
		{
			struct s_unit_transform_state
			{
				const TagGroups::s_actor_variant_transform* m_actor_variant_transform;
				Enums::game_team m_instigator_team;
				PAD16;
			};

			bool m_transforms_allowed;
			PAD24;
			std::map<datum_index::index_t, s_unit_transform_state> m_transforms_in_progress;

		public:
			c_unit_transform_manager();

		private:
			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Searches for a matching actor variant transform definition. </summary>
			///
			/// <param name="transformations">			The transformations list. </param>
			/// <param name="unit_index">				Datum index of the unit. </param>
			/// <param name="instigator_unit_index">	Datum index of the instigator unit. </param>
			/// <param name="damage_is_melee">			Whether the damage is melee damage. </param>
			///
			/// <returns>	null if it fails, else the found actor transform. </returns>
			const TagGroups::s_actor_variant_transform* FindTransform(
#if PLATFORM_IS_EDITOR
			const TagBlock<TagGroups::s_actor_variant_transform>& transformations
#else
			const TagBlock<const TagGroups::s_actor_variant_transform>& transformations
#endif
				, const datum_index& unit_index
				, const datum_index& instigator_unit_index
				, const bool damage_is_melee);

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Deletes attachments on an object that match those in the transform definition. </summary>
			///
			/// <param name="unit_datum">		   	The unit datum. </param>
			/// <param name="transform_definition">	The transform definition. </param>
			void DeleteAttachments(const datum_index unit_index, const TagGroups::s_unit_transform_definition& transform_definition);

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
			/// <param name="attachment_team">		   	The attachment's team. </param>
			/// <param name="attachment_scale">		   	The attachment's scale. </param>
			void AttachObject(const datum_index unit_index
				, const s_unit_datum* unit_datum
				, const datum_index object_type
				, const tag_string& object_marker_name
				, const tag_string& destination_marker_name
				, const int32 destination_marker_index
				, const Enums::game_team attachment_team
				, const real attachment_scale);

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Attach the transform definition's attachment objects to the target unit. </summary>
			///
			/// <param name="unit_index">		   	Datum index of the unit. </param>
			/// <param name="unit_datum">		   	The unit datum. </param>
			/// <param name="instigator_team">	   	The instigator team. </param>
			/// <param name="transform_definition">	The transform definition. </param>
			void AttachObjects(const datum_index unit_index
				, const s_unit_datum* unit_datum
				, const Enums::game_team instigator_team
				, const TagGroups::s_unit_transform_definition& transform_definition);

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Starts a the transform out stage on the target unit. </summary>
			///
			/// <param name="unit_index">		   	Datum index of the unit. </param>
			/// <param name="unit_datum">		   	[in,out] The unit datum. </param>
			/// <param name="instigator_team">	   	The instigator team. </param>
			/// <param name="transform_definition">	The transform definition. </param>
			void TransformOut(const datum_index unit_index
				, s_unit_datum* unit_datum
				, const Enums::game_team instigator_team
				, const TagGroups::s_actor_variant_transform& transform_definition);

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Start's the transform in stage on the target unit. </summary>
			///
			/// <param name="unit_index">	Datum index of the unit. </param>
			/// <param name="unit_datum">	The unit datum. </param>
			void TransformIn(const datum_index unit_index
				, s_unit_datum* unit_datum
				, const s_unit_transform_state& transform_state);

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Ends the transform by making the created actor vulnerable. </summary>
			///
			/// <param name="unit_datum">	[in] If non-null, the unit datum. </param>
			void TransformEnd(s_unit_datum* unit_datum);

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

			/// <summary>	Clears the in progress transforms. </summary>
			void ClearInProgressTransforms();

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