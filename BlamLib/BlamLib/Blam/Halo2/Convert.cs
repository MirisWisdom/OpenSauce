/*
    BlamLib: .NET SDK for the Blam Engine

    Copyright (C) 2005-2010  Kornner Studios (http://kornner.com)

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
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