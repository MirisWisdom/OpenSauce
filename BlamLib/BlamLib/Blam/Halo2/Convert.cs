/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
using System;
using System.Collections.Generic;
using System.Text;

namespace BlamLib.Blam.Halo2
{
	/// <summary>
	/// Utility class for converting Halo 1 tags to Halo 2
	/// </summary>
	public class Converter
	{
		/// <summary>
		/// Convert a halo 1 sound tag to the halo 2 version
		/// </summary>
		/// <param name="halo1"></param>
		/// <param name="halo2"></param>
		/// <returns></returns>
		public bool Definitions(
			Blam.Halo1.Tags.sound_group halo1,
			Tags.sound_group halo2
			)
		{
			return true;
		}
		/// <summary>
		/// Convert a halo 1 render model to the halo 2 version
		/// </summary>
		/// <param name="halo1"></param>
		/// <param name="halo2"></param>
		/// <returns></returns>
		public bool Definitions(
			Blam.Halo1.Tags.gbxmodel_group halo1,
			Tags.render_model_group halo2
			)
		{
			return true;
		}
		/// <summary>
		/// Convert a halo 1 collision model to the halo 2 version
		/// </summary>
		/// <param name="halo1"></param>
		/// <param name="halo2"></param>
		/// <returns></returns>
		public bool Definitions(
			Blam.Halo1.Tags.model_collision_group halo1,
			Tags.collision_model_group halo2
			)
		{
			return true;
		}
	};
}