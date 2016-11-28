/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
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
		public string Name { get; private set; }

		SlimDX.Vector3 translationVector;
		SlimDX.Quaternion rotationQuaternion;
		float scaleAmount;

		public SlimDX.Matrix TransformMatrixWorld { get; private set; }
		public SlimDX.Matrix TransformMatrixLocal { get; private set; }

		public ColladaBoneMatrix ParentNode { get; set; }

		public ColladaBoneMatrix(string name,
			LowLevel.Math.real_point3d translation,
			LowLevel.Math.real_quaternion rotation,
			float scale)
		{
			Name = name;
			translationVector = new SlimDX.Vector3(translation.X, translation.Y, translation.Z);
			rotationQuaternion = new SlimDX.Quaternion(rotation.Vector.I, rotation.Vector.J, rotation.Vector.K, rotation.W);
			scaleAmount = scale;
		}

		/// <summary>
		/// Creates the local and world transform matrices for the bone
		/// </summary>
		public void CreateMatrices()
		{
			// creates the individual matrix components
			SlimDX.Matrix scale_matrix = SlimDX.Matrix.Scaling(scaleAmount, scaleAmount, scaleAmount);
			SlimDX.Matrix rotate_matrix = SlimDX.Matrix.RotationQuaternion(rotationQuaternion);
			SlimDX.Matrix translate_matrix = SlimDX.Matrix.Translation(translationVector);

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