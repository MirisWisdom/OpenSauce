/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
using System;
using System.Collections.Generic;

namespace BlamLib.Render.COLLADA
{
	public class ColladaObjectElementList<T> : ColladaObject
		where T : new()
	{
		public List<T> Value { get; set; }

		public override object GetValue() { return Value; }
		public override Type GetObjectType() { return typeof(T); }
		public override string GetTypeName() { return typeof(T).Name; }

		public override void ValidateField(Enums.ColladaElementType parent_type)
		{
			if (Value != null && typeof(T).IsSubclassOf(typeof(ColladaElement)))
				foreach (var element in Value)
					(element as ColladaElement).ValidateElement(parent_type);
		}
	};
}