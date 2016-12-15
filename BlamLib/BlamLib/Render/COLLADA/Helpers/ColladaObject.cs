/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
using System;

namespace BlamLib.Render.COLLADA
{
	public abstract class ColladaObject
	{
		public abstract object GetValue();
		public abstract Type GetObjectType();
		public abstract string GetTypeName();
		public virtual void ValidateField(Enums.ColladaElementType parent_type) { }
	};
}