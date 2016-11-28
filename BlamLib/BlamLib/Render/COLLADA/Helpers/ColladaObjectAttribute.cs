/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
using System;
using System.Collections.Generic;
using System.Xml.Serialization;

namespace BlamLib.Render.COLLADA
{
	public class ColladaObjectAttribute<T> : ColladaObject
	{
		public Type ObjectType { get { return typeof(T); } }

		public T Value { get; set; }

		public override object GetValue() { return Value; }
		public override Type GetObjectType() { return typeof(T); }
		public override string GetTypeName() { return typeof(T).Name; }

		public ColladaObjectAttribute(T initial_value)
		{
			Value = initial_value;
		}
	};
}