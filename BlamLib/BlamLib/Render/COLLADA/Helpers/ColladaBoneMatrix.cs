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

namespace BlamLib.Render.COLLADA
{
	/// <summary>
	/// Class for generating local and word transform matricies for a list of connected bones
	/// </summary>
	public class ColladaBoneMatrix
	{
		SlimDX.Vector3 translation;
		SlimDX.Quaternion rotationQuaternion;
		float scale;

		public SlimDX.Matrix TransformMatrixWorld { get; private set; }
		public SlimDX.Matrix TransformMatrixLocal { get; private set; }

		public ColladaBoneMatrix ParentNode { get; set; }

		public ColladaBoneMatrix(TagInterface.RealPoint3D bone_translation,
			float bone_translation_scale,
			TagInterface.RealQuaternion bone_rotation,
			float bone_scale)
		{
			translation = new SlimDX.Vector3(bone_translation.X * bone_translation_scale, bone_translation.Y * bone_translation_scale, bone_translation.Z * bone_translation_scale);
			rotationQuaternion = new SlimDX.Quaternion(-bone_rotation.I, -bone_rotation.J, -bone_rotation.K, bone_rotation.W);
			scale = bone_scale;
		}

		/// <summary>
		/// Creates the local and world transform matrices for the bone
		/// </summary>
		public void CreateMatrices()
		{
			// creates the individual matrix components
			SlimDX.Matrix scale_matrix = SlimDX.Matrix.Scaling(scale, scale, scale);
			SlimDX.Matrix rotate_matrix = SlimDX.Matrix.RotationQuaternion(rotationQuaternion);
			SlimDX.Matrix translate_matrix = SlimDX.Matrix.Translation(translation);

			TransformMatrixLocal = SlimDX.Matrix.Identity;

			// multiply the matrices together
			TransformMatrixLocal *= scale_matrix;
			TransformMatrixLocal *= rotate_matrix;
			TransformMatrixLocal *= translate_matrix;

			// multiply by the parents world matrix (if present) to get this bones world matrix
			TransformMatrixWorld = TransformMatrixLocal;
			if (ParentNode != null)
				TransformMatrixWorld = TransformMatrixLocal * ParentNode.TransformMatrixWorld;
		}
	};
}