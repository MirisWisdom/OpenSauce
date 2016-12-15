/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
using System;
using System.ComponentModel;
using System.Xml;
using System.Xml.Serialization;
using System.Collections.Generic;
using BlamLib.Render.COLLADA.Validation;

namespace BlamLib.Render.COLLADA.Core
{
	[SerializableAttribute()]
	[XmlTypeAttribute(AnonymousType = true)]
	public partial class ColladaAccessor : ColladaElement
	{
		#region Fields
		ColladaObjectAttribute<uint> _count;
		ColladaObjectAttribute<uint> _offset;
		ColladaObjectAttribute<string> _source;

		ColladaObjectElementList<ColladaParam> _param;
		#endregion

		/// <summary>
		/// Use when the stride spans more than a single value
		/// </summary>
		[XmlIgnore]
		public uint StrideOverride = 0;

		#region Attributes
		[XmlAttribute("count")]
		public uint Count
		{ get { return _count.Value; } set { _count.Value = value; } }

		[XmlAttribute("offset"), DefaultValue(0)]
		public uint Offset
		{ get { return _offset.Value; } set { _offset.Value = value; } }

		[XmlAttribute("source"), ColladaURI]
		public string Source
		{ get { return _source.Value; } set { _source.Value = value; } }

		[XmlAttribute("stride")]
		public uint Stride
		{
			get
			{
				if (StrideOverride != 0)
					return StrideOverride;

				if (Param == null) return 1;
				else
					return (uint)Param.Count;
			}
			set { }
		}
		#endregion

		#region Children
		[XmlElement("param")]
		public List<ColladaParam> Param
		{ get { return _param.Value; } set { _param.Value = value; } }
		#endregion

		public ColladaAccessor() : base(Enums.ColladaElementType.Core_Accessor)
		{
			Fields.Add(_count = new ColladaObjectAttribute<uint>(0));
			Fields.Add(_offset = new ColladaObjectAttribute<uint>(0));
			Fields.Add(_source = new ColladaObjectAttribute<string>(""));
			Fields.Add(_param = new ColladaObjectElementList<ColladaParam>());

			ValidationTests.Add(new ColladaIsNull(Enums.ColladaElementType.All, _source));
			ValidationTests.Add(new ColladaEmptyString(Enums.ColladaElementType.All, _source));
		}

		/// <summary>
		/// Sets the Count attribute by dividing source_count by Stride
		/// </summary>
		/// <param name="source_count">The number of elements in the associated source array</param>
		public void SetCount(uint source_count) { Count = source_count / Stride; }
	}
}