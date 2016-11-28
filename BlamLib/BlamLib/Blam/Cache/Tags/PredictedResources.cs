/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
#pragma warning disable 1591 // "Missing XML comment for publicly visible type or member"
using System;
using System.Collections.Generic;
using System.Text;

namespace BlamLib.Blam.Cache
{
	public class PredictedResources
	{
		/* Halo1
		 * Shaders - All bitmap references
		 * Model - Shader block
		 * Object - Model tag
		 * Weapon - zoom in\out sounds
		 * Particle -
		 * Effect - 
		 * Weapon (FP shit) -	'ready effect'
		 *						'overheated'
		 *						'first person model'
		 *						'Triggers->Firing Effects.firing effect'
		 *						'Triggers->Firing Effects.misfire effect'
		 *						'Triggers->Firing Effects.empty effect'
		 * Scenario - bitmaps, Sky-Model tag
		 * Structure bsp - Clusters PR = 'Lightmaps->Materials.Shader', based on the material for that surface
		*/
	};

	public abstract class predicted_resource_block : TagInterface.Definition
	{
		public TagInterface.Enum Type;
		public TagInterface.ShortInteger ResourceIndex;
		public TagInterface.LongInteger TagIndex;

		protected predicted_resource_block(int field_count) : base(field_count) {}
	};
}