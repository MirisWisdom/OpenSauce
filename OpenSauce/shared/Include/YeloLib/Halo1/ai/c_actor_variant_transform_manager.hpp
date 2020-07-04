/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#include <YeloLib/Halo1/units/unit_transform_definition.hpp>

namespace Yelo
{
	namespace Enums
	{
		enum game_team : _enum;
		enum unit_animation_keyframe : _enum;
	};

	namespace Objects
	{
		struct s_damage_data;
		struct s_unit_datum;
	};

	namespace AI { namespace Transform
	{
		/// <summary>	Manager for actor variant transforms. </summary>
		class c_actor_variant_transform_manager final
		{
			enum { k_max_concurrent_transforms = 20 };

			enum transform_stage : byte
			{
				_transform_stage_begin,
				_transform_stage_transform_out_begun,
				_transform_stage_transform_out_ended,
				_transform_stage_transform_in_begun,
				_transform_stage_transform_in_ended,
			};

			struct s_actor_variant_transform_state
			{
				datum_index::index_t m_unit_index;
				Enums::game_team m_instigator_team;
				datum_index m_instigator_encounter;
				int16 m_instigator_squad;
				sbyte m_transform_entry_index;
				sbyte m_transform_index;
				sbyte m_target_index;
				transform_stage m_transform_stage;
			};

			bool m_transforms_enabled;
			PAD24;
			s_actor_variant_transform_state* m_transform_states;
			TagGroups::actor_variant_transform_collection_definition* m_transform_collection;

		public:
			/// <summary>	Default constructor. </summary>
			c_actor_variant_transform_manager();

		private:
#pragma region Validation
			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Returns whether a unit is valid for transforming. </summary>
			///
			/// <param name="unit_index">	Datum index of the unit. </param>
			///
			/// <returns>	true if valid, false if not. </returns>
			bool UnitIsValid(const datum_index unit_index) const;

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Returns whether an actor is valid for transforming. </summary>
			///
			/// <param name="unit_index">	Datum index of the actor. </param>
			///
			/// <returns>	true if valid, false if not. </returns>
			bool ActorIsValid(const datum_index actor_index) const;
#pragma endregion

#pragma region Find Transform
			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Tests the vitality transform criteria for a unit. </summary>
			///
			/// <param name="unit_index">   	Datum index of the unit. </param>
			/// <param name="transform_out">	The transform out. </param>
			///
			/// <returns>	true if the test passes, false if the test fails. </returns>
			bool TestVitalityCriteria(const datum_index unit_index, const TagGroups::actor_variant_transform_out_definition& transform_out_definition) const;

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Gets a transform using the provided selection function. </summary>
			///
			/// <param name="transformations">	The transformations. </param>
			/// <param name="select_func">	  	The select function. </param>
			///
			/// <returns>	The found transform. </returns>
			sbyte FindTransform(const TagBlock<TagGroups::actor_variant_transform_collection_transform>& transformations
				, std::function<bool (const TagGroups::actor_variant_transform_collection_transform&)> select_func) const;

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>
			/// 	Searches for a matching actor variant transform definition based on damage received.
			/// </summary>
			///
			/// <param name="transformations">			[in] The transformations list. </param>
			/// <param name="unit_index">				Datum index of the unit. </param>
			/// <param name="instigator_unit_index">	Datum index of the instigator unit. </param>
			/// <param name="damage_is_melee">			Whether the damage is melee damage. </param>
			///
			/// <returns>	Index of the found transform. Returns NONE if no transform was found. </returns>
			sbyte FindDamageTransform(const TagBlock<TagGroups::actor_variant_transform_collection_transform>& transformations
				, const datum_index unit_index
				, const datum_index instigator_unit_index
				, const bool damage_is_melee) const;

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Searches for the first transform matching the criteria for a unit update. </summary>
			///
			/// <param name="transformations">	The transformations. </param>
			/// <param name="unit_index">	  	Datum index of the unit. </param>
			///
			/// <returns>	The found update transform. </returns>
			sbyte FindUpdateTransform(const TagBlock<TagGroups::actor_variant_transform_collection_transform>& transformations
				, const datum_index unit_index) const;

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Searches for the first transforms entry for an actor variant tag. </summary>
			///
			/// <param name="tag_index">	Datum index of the actor variant tag. </param>
			///
			/// <returns>	The found transforms entry. </returns>
			sbyte FindTransformsEntry(const datum_index tag_index) const;
#pragma endregion

#pragma region Tag Options
			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Overrides a units health and shield values. </summary>
			///
			/// <param name="operation"> 	The override operation. </param>
			/// <param name="unit_datum">	[in] If non-null, the old unit's datum. </param>
			/// <param name="health">	 	The health value. </param>
			/// <param name="shield">	 	The shield value. </param>
			void HandleVitality(const Enums::actor_variant_transform_in_vitality_handling operation
				, Objects::s_unit_datum* unit_datum
				, const real health
				, const real shield) const;

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>
			/// 	Gets a team based on whether it is from the attacked, attacker or an override.
			/// </summary>
			///
			/// <param name="option">			The team handling option. </param>
			/// <param name="attacked_team">	The attacked team. </param>
			/// <param name="attacker_team">	The attacker team. </param>
			/// <param name="override_team">	The override team. </param>
			///
			/// <returns>	The chosen team. </returns>
			Enums::game_team HandleTeam(const Enums::actor_variant_transform_team_handling option
				, const Enums::game_team attacked_team
				, const Enums::game_team attacker_team
				, const Enums::game_team override_team) const;
#pragma endregion

#pragma region Unit Creation
			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Creates the unit's actor. </summary>
			///
			/// <param name="unit_index">			  	Datum index of the unit. </param>
			/// <param name="target">				  	The transform target definition. </param>
			/// <param name="source_actor_index">	  	Datum index of the source actor. </param>
			/// <param name="instigator_encounter">   	Datum index of the instigators encounter. </param>
			/// <param name="instigator_squad">		  	Index of the instigators squad. </param>
			void CreateUnitActor(const datum_index unit_index
				, const TagGroups::actor_variant_transform_in_target& target
				, const datum_index source_actor_index
				, const datum_index instigator_encounter
				, const int16 instigator_squad) const;

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Creates a new unit reusing the old unit. </summary>
			///
			/// <param name="unit_index">	  	Datum index of the old unit. </param>
			/// <param name="unit_datum">	  	[in] The old unit's datum. </param>
			/// <param name="team_option">	  	The team handling option. </param>
			/// <param name="instigator_team">	The instigator team. </param>
			/// <param name="override_team">  	The override team. </param>
			///
			/// <returns>	The new unit index. </returns>
			datum_index CreateUnitReuse(const datum_index unit_index
				, Objects::s_unit_datum* unit_datum
				, const Enums::actor_variant_transform_team_handling team_option
				, const Enums::game_team instigator_team
				, const Enums::game_team override_team) const;

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Creates a new unit. </summary>
			///
			/// <param name="unit_index">	  	Datum index of the old unit. </param>
			/// <param name="unit_datum">	  	[in] The old unit's datum. </param>
			/// <param name="new_unit_type">  	Type of the new unit. </param>
			/// <param name="team_option">	  	The team handling option. </param>
			/// <param name="instigator_team">	The instigator team. </param>
			/// <param name="State">		  	The state. </param>
			///
			/// <returns>	The new unit index. </returns>
			datum_index CreateUnitNew(const datum_index unit_index
				, Objects::s_unit_datum* unit_datum
				, const datum_index new_unit_type
				, const Enums::actor_variant_transform_team_handling team_option
				, const Enums::game_team instigator_team
				, const Enums::game_team override_team) const;
#pragma endregion

#pragma region Transform State
			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Searches for a transform state. </summary>
			///
			/// <param name="unit_index">	Datum index of the unit. </param>
			///
			/// <returns>	null if it fails, else the found transform state. </returns>
			s_actor_variant_transform_state* FindTransformState(const datum_index::index_t unit_index) const;

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Searches for a transform state. </summary>
			///
			/// <param name="unit_index">	Datum index of the unit. </param>
			///
			/// <returns>	null if it fails, else the found transform state. </returns>
			s_actor_variant_transform_state* FindTransformState(const datum_index unit_index) const;

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Allocate a transform state entry. </summary>
			///
			/// <param name="unit_index">	Datum index of the unit. </param>
			///
			/// <returns>	null if it fails, else a s_actor_variant_transform_state*. </returns>
			s_actor_variant_transform_state* AllocateTransformState(const datum_index unit_index);

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Frees the transform state described by unit_index. </summary>
			///
			/// <param name="unit_index">	Datum index of the unit. </param>
			void FreeTransformState(const datum_index unit_index);
#pragma endregion

#pragma region Attachments
			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>
			/// 	Destroys attachments on an object that match those in the transform definition.
			/// </summary>
			///
			/// <param name="unit_index">		   	The unit datum. </param>
			/// <param name="transform_definition">	[in] The transform out definition. </param>
			void DestroyAttachments(const datum_index unit_index, const TagGroups::actor_variant_transform_out_definition& transform_definition) const;

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
				, const Objects::s_unit_datum* unit_datum
				, const datum_index object_type
				, const tag_string& object_marker_name
				, const tag_string& destination_marker_name
				, const int32 destination_marker_index
				, const Enums::game_team attachment_team
				, const real attachment_scale) const;

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Attach the transform definition's attachment objects to the target unit. </summary>
			///
			/// <param name="unit_index">		   	Datum index of the unit. </param>
			/// <param name="unit_datum">		   	The unit datum. </param>
			/// <param name="instigator_team">	   	The instigator team. </param>
			/// <param name="transform_definition">	[in] The transform out definition. </param>
			void AttachObjects(const datum_index unit_index
				, const Objects::s_unit_datum* unit_datum
				, const Enums::game_team instigator_team
				, const TagGroups::actor_variant_transform_out_definition& transform_definition) const;
#pragma endregion

#pragma region Transform Stages
			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Starts the transform out stage on the target unit. </summary>
			///
			/// <param name="unit_index">			   	Datum index of the unit. </param>
			/// <param name="unit_datum">			   	[in] The unit datum. </param>
			/// <param name="instigator_team">		   	The instigator team. </param>
			/// <param name="transform_out_definition">	[in] The transform out definition. </param>
			/// <param name="transform_in_definition"> 	[in] The transform in definition. </param>
			void TransformOut(const datum_index unit_index
				, Objects::s_unit_datum* unit_datum
				, const Enums::game_team instigator_team
				, const TagGroups::actor_variant_transform_out_definition& transform_out_definition
				, const TagGroups::actor_variant_transform_in_definition& transform_in_definition
				, s_actor_variant_transform_state& transform_state) const;

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Start's the transform in stage on the target unit. </summary>
			///
			/// <param name="unit_index">	  	Datum index of the unit. </param>
			/// <param name="unit_datum">	  	[in] The unit datum. </param>
			/// <param name="transform_state">	State of the transform. </param>
			void TransformIn(const datum_index unit_index
				, Objects::s_unit_datum* unit_datum
				, s_actor_variant_transform_state& transform_state) const;

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Ends the transform by making the created actor vulnerable. </summary>
			///
			/// <param name="unit_datum">	[in] If non-null, the unit datum. </param>
			void TransformEnd(const datum_index unit_index, Objects::s_unit_datum* unit_datum) const;
#pragma endregion

#pragma region Animation
			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Executes the transform keyframe action. </summary>
			///
			/// <param name="unit_index">	Datum index of the unit. </param>
			/// <param name="action">	 	The action. </param>
			void DoTransformKeyframeAction(const datum_index unit_index, const TagGroups::actor_variant_transform_keyframe_action& action);

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Transforming out keyframe handler. </summary>
			///
			/// <param name="unit_index">	Datum index of the unit. </param>
			/// <param name="unit_datum">	[in] If non-null, the unit datum. </param>
			/// <param name="keyframe">  	The keyframe. </param>
			void TransformingOutKeyframe(const datum_index unit_index
				, Objects::s_unit_datum* unit_datum
				, const Enums::unit_animation_keyframe keyframe);

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Transforming in keyframe handler. </summary>
			///
			/// <param name="unit_index">	Datum index of the unit. </param>
			/// <param name="unit_datum">	[in] If non-null, the unit datum. </param>
			/// <param name="keyframe">  	The keyframe. </param>
			void TransformingInKeyframe(const datum_index unit_index
				, Objects::s_unit_datum* unit_datum
				, const Enums::unit_animation_keyframe keyframe);
#pragma endregion

		public:
			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Gets/Sets whether transforms are enabled. </summary>
			///
			/// <returns>	true if enabled, false if not. </returns>
			bool& TransformsEnabled();
			
#pragma region Transform State
			/// <summary>	Allocates game state memory. </summary>
			void AllocateGameStateMemory();

			/// <summary>	Clears the in progress transforms. </summary>
			void ClearInProgressTransforms();
#pragma endregion
			
#pragma region Game State
			/// <summary>	Loads the actor variant transform collection tag. </summary>
			void LoadActorVariantTransforms();

			/// <summary>	Unloads the actor variant transform collection tag. </summary>
			void UnloadActorVariantTransforms();
#pragma endregion
			
#pragma region Transform Trigger/Update
			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Attempts to start a transform on a damaged unit if applicable. </summary>
			///
			/// <param name="unit_index"> 	Datum index of the unit. </param>
			/// <param name="damage_data">	Information describing the damage. </param>
			void UnitDamaged(const datum_index unit_index, const Objects::s_damage_data* damage_data);

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Unit transform update. </summary>
			///
			/// <param name="unit_index">	Datum index of the unit. </param>
			void UnitUpdate(const datum_index unit_index);
#pragma endregion

#pragma region Animation
			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Triggers a unit transform keyframe. </summary>
			///
			/// <param name="unit_index">	Datum index of the unit. </param>
			/// <param name="keyframe">  	The keyframe. </param>
			void TriggerUnitTransformKeyframe(const datum_index unit_index, const Enums::unit_animation_keyframe keyframe);
#pragma endregion

#pragma region Scripting
			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Transforms an actor to the specified transform. </summary>
			///
			/// <param name="unit_index">	Datum index of the unit. </param>
			/// <param name="transform">  	The transform name. </param>
			/// <param name="target">	  	The target name. </param>
			///
			/// <returns>	true if it succeeds, false if it fails. </returns>
			bool TransformActor(const datum_index unit_index
				, cstring transform_name
				, cstring target_name);

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Transforms a list of actors to the specified transform. </summary>
			///
			/// <param name="unit_index_list">	Datum index of the unit list. </param>
			/// <param name="transform_name"> 	The transform name. </param>
			/// <param name="target_name">	  	The target name. </param>
			///
			/// <returns>	true if it succeeds, false if it fails. </returns>
			bool TransformActors(const datum_index unit_index_list
				, cstring transform_name
				, cstring target_name);

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>
			/// 	Transforms actors in a list of a particular type to the specified transform.
			/// </summary>
			///
			/// <param name="unit_index_list">	Datum index of the unit list. </param>
			/// <param name="tag_index">	  	Datum index of the actor variant tag. </param>
			/// <param name="transform_name"> 	The transform name. </param>
			/// <param name="target_name">	  	The target name. </param>
			///
			/// <returns>	true if it succeeds, false if it fails. </returns>
			bool TransformActorsByType(const datum_index unit_index_list
				, const datum_index tag_index
				, cstring transform_name
				, cstring target_name);

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Returns whether an actor is transforming. </summary>
			///
			/// <param name="unit_index">	Datum index of the actor. </param>
			///
			/// <returns>	true if it succeeds, false if it fails. </returns>
			bool ActorIsTransforming(const datum_index unit_index);
#pragma endregion
		};
	};};
};