/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
using System;
using System.Collections.Generic;

namespace BlamLib.Render.COLLADA
{
	public class ColladaObjectElement<T> : ColladaObject
		where T : new()
	{
		public T Value { get; set; }

		public override object GetValue() { return Value; }
		public override Type GetObjectType() { return typeof(T); }
		public override string GetTypeName() { return typeof(T).Name; }

		public override void ValidateField(Enums.ColladaElementType parent_type)
		{
			if (Value == null)
				return;

			ColladaElement value = Value as ColladaElement;
			if (value != null)
				value.ValidateElement(parent_type);
		}
	};
}