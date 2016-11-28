/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
using System;
using System.Collections.Generic;

namespace BlamLib.Render.COLLADA
{
	public class ColladaObjectValue<T> : ColladaObject
	{
		public T Value { get; set; }

		public override object GetValue() { return Value; }
		public override Type GetObjectType() { return typeof(T); }
		public override string GetTypeName() { return typeof(T).Name; }

		public ColladaObjectValue() {}
		public ColladaObjectValue(T initial_value) { Value = initial_value; }
	};
}