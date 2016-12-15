/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#include <blamlib/Halo1/objects/object_types.hpp>

namespace Yelo
{
	namespace Objects
	{
		struct s_object_placement_data;
		struct s_object_iterator;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Object iterator next. </summary>
		///
		/// <typeparam name="ObjectType">	The object type parameter. </typeparam>
		/// <param name="iter">	[out] The iterator. </param>
		///
		/// <returns>	null if it fails, else a object data pointer. </returns>
		template<typename ObjectType>
		ObjectType* IteratorNext(const s_object_iterator& iter);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Creates a new copy of an object's placement data. </summary>
		///
		/// <param name="data">				  	[in] The sourcedata. </param>
		/// <param name="source_object_index">	Datum index of the source object. </param>
		/// <param name="tag_index_override"> 	(Optional) the tag index override. </param>
		/// <param name="owner_object_index"> 	(Optional) the object that owns this item. </param>
		void PlacementDataNewAndCopy(s_object_placement_data& data
			, const datum_index source_object_index
			, datum_index tag_index_override = datum_index::null
			, const datum_index owner_object_index = datum_index::null);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// 	Walks the parent object datums of [object_index] until it gets to the upper most parent
		/// 	and returns that parent's datum index.
		/// </summary>
		///
		/// <param name="object_index">	Datum index of the object. </param>
		///
		/// <returns>	The ultimate object. </returns>
		datum_index GetUltimateObject(const datum_index object_index);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// 	Walks the next object datums of [object_index] [step_count] amount of times, and returns
		/// 	the result datum_index.
		/// </summary>
		///
		/// <remarks>
		/// 	Say, if [step_count] is zero, it will return the datum_index that is returned by
		/// 	[object_index]'s GetNextObjectIndex.
		/// </remarks>
		///
		/// <param name="object_index">	Datum index of the object. </param>
		/// <param name="step_count">  	(Optional) the step count to walk down the object tree. </param>
		///
		/// <returns>	The next object after the specified steps. </returns>
		datum_index GetNextObjectN(datum_index object_index, int32 step_count = 0);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets the object distance from a point. </summary>
		///
		/// <param name="object_index">			Datum index of the object. </param>
		/// <param name="destination_point">	Destination point. </param>
		///
		/// <returns>	The object distance from point. </returns>
		real GetObjectDistanceFromPoint(const datum_index object_index, const real_vector3d& destination_point);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Delete all children of the specified type(s) on the parent object. </summary>
		///
		/// <param name="parent">		   	The parent. </param>
		/// <param name="object_type_mask">	The object type mask. </param>
		void DeleteChildrenByType(const datum_index parent, const long_flags object_type_mask);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Detach all children of the specified type(s) from the parent object. </summary>
		///
		/// <param name="parent">		   	The parent. </param>
		/// <param name="object_type_mask">	The object type mask. </param>
		void DetachChildrenByType(const datum_index parent, const long_flags object_type_mask);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Delete all child actors on the parent object. </summary>
		///
		/// <param name="parent">		   	The parent. </param>
		void DeleteChildActors(const datum_index parent);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Detach all child actors from the parent object. </summary>
		///
		/// <param name="parent">		   	The parent. </param>
		void DetachChildActors(const datum_index parent);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// 	Destroys all children of the specified definition type from the parent object.
		/// </summary>
		///
		/// <param name="parent">	 	The parent. </param>
		/// <param name="definition">	The object definition index. </param>
		void DestroyChildrenByDefinition(const datum_index parent, const datum_index definition);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// 	Deletes all children of the specified definition type from the parent object.
		/// </summary>
		///
		/// <param name="parent">	 	The parent. </param>
		/// <param name="definition">	The object definition index. </param>
		void DeleteChildrenByDefinition(const datum_index parent, const datum_index definition);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// 	Detach all children of the specified definition type from the parent object.
		/// </summary>
		///
		/// <param name="parent">	 	The parent. </param>
		/// <param name="definition">	The object definition index. </param>
		void DetachChildrenByDefinition(const datum_index parent, const datum_index definition);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Predict the object memory pool usage. </summary>
		///
		/// <param name="type">						The object type. </param>
		/// <param name="node_count">				Number of nodes. </param>
		/// <param name="include_yelo_upgrades">	(Optional) include yelo upgrades. </param>
		///
		/// <returns>	The memory usage for the specified type. </returns>
		size_t PredictMemoryPoolUsage(const Enums::object_type type
			, const int32 node_count
			, const bool include_yelo_upgrades = false);
	};
};