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
	/// Class for generating local and word transfrom matricies for a list of connected bones
	/// </summary>
	public class ColladaBoneMatrix
	{
		private SlimDX.Vector3 translation;
		private SlimDX.Quaternion rotationQuaternion;
		private float scale;

		private SlimDX.Matrix transformMatrixWorld;
		private SlimDX.Matrix transformMatrixLocal;

		public SlimDX.Matrix TransformMatrixWorld
		{
			get { return transformMatrixWorld; }
		}
		public SlimDX.Matrix TransformMatrixLocal
		{
			get { return transformMatrixLocal; }
		}

		private ColladaBoneMatrix parentNode;
		public ColladaBoneMatrix ParentNode
		{
			get { return parentNode; }
			set { parentNode = value; }
		}

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

			transformMatrixLocal = SlimDX.Matrix.Identity;

			// multiply the matrices together
			transformMatrixLocal *= scale_matrix;
			transformMatrixLocal *= rotate_matrix;
			transformMatrixLocal *= translate_matrix;

			// multiply by the parents world matrix (if present) to get this bones world matrix
			transformMatrixWorld = transformMatrixLocal;
			if (parentNode != null)
				transformMatrixWorld = transformMatrixLocal * parentNode.TransformMatrixWorld;
		}
	};
}
