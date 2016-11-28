/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
using System;
using System.ComponentModel;
using System.Xml;
using System.Xml.Serialization;

namespace BlamLib.Render.COLLADA.Core
{
	[SerializableAttribute()]
	[XmlTypeAttribute(AnonymousType = true)]
	public partial class ColladaContributor : ColladaElement
	{
		#region Fields
		ColladaObjectValue<string> _author;
		ColladaObjectValue<string> _authoringTool;
		ColladaObjectValue<string> _comments;
		ColladaObjectValue<string> _copyright;
		ColladaObjectValue<string> _sourceData;
		#endregion

		#region Children
		[XmlElement("author"), DefaultValue("")]
		public string Author
		{ get { return _author.Value; } set { _author.Value = value; } }

		[XmlElement("authoring_tool"), DefaultValue("")]
		public string AuthoringTool
		{ get { return _authoringTool.Value; } set { _authoringTool.Value = value; } }

		[XmlElement("comments"), DefaultValue("")]
		public string Comments
		{ get { return _comments.Value; } set { _comments.Value = value; } }

		[XmlElement("copyright"), DefaultValue("")]
		public string Copyright
		{ get { return _copyright.Value; } set { _copyright.Value = value; } }

		[XmlElement("source_data"), DefaultValue("")]
		public string SourceData
		{ get { return _sourceData.Value; } set { _sourceData.Value = value; } }
		#endregion

		public ColladaContributor() : base(Enums.ColladaElementType.Core_Contributor)
		{
			Fields.Add(_author = new ColladaObjectValue<string>(""));
			Fields.Add(_authoringTool = new ColladaObjectValue<string>(""));
			Fields.Add(_comments = new ColladaObjectValue<string>(""));
			Fields.Add(_copyright = new ColladaObjectValue<string>(""));
			Fields.Add(_sourceData = new ColladaObjectValue<string>(""));
		}
	}
}