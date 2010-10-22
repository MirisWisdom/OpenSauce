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