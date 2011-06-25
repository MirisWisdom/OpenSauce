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