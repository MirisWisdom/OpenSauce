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
	public partial class ColladaAsset : ColladaElement
	{
		#region Fields
		ColladaObjectElementList<ColladaContributor> _contributor;
		ColladaObjectValue<DateTime> _created;
		ColladaObjectValue<string> _keywords;
		ColladaObjectValue<DateTime> _modified;
		ColladaObjectValue<string> _revision;
		ColladaObjectValue<string> _subject;
		ColladaObjectValue<string> _title;
		ColladaObjectElement<ColladaAssetUnit> _unit;
		ColladaObjectValue<Enums.ColladaUpAxisEnum> _upAxis;
		#endregion

		#region Children
		[XmlElement("contributor")]
		public List<ColladaContributor> Contributor
		{ get { return _contributor.Value; } set { _contributor.Value = value; } }

		[XmlElement("created")]
		public DateTime Created
		{ get { return _created.Value; } set { _created.Value = value; } }

		[XmlElement("keywords"), DefaultValue("")]
		public string Keywords
		{ get { return _keywords.Value; } set { _keywords.Value = value; } }

		[XmlElement("modified")]
		public DateTime Modified
		{ get { return _modified.Value; } set { _modified.Value = value; } }

		[XmlElement("revision"), DefaultValue("")]
		public string Revision
		{ get { return _revision.Value; } set { _revision.Value = value; } }

		[XmlElement("subject"), DefaultValue("")]
		public string Subject
		{ get { return _subject.Value; } set { _subject.Value = value; } }

		[XmlElement("title"), DefaultValue("")]
		public string Title
		{ get { return _title.Value; } set { _title.Value = value; } }

		[XmlElement("unit")]
		public ColladaAssetUnit Unit
		{ get { return _unit.Value; } set { _unit.Value = value; } }

		[XmlElement("up_axis"), DefaultValue(Enums.ColladaUpAxisEnum.Y_UP)]
		public Enums.ColladaUpAxisEnum UpAxis
		{ get { return _upAxis.Value; } set { _upAxis.Value = value; } }
		#endregion

		public ColladaAsset() : base(Enums.ColladaElementType.Core_Asset)
		{
			Fields.Add(_contributor = new ColladaObjectElementList<ColladaContributor>());
			Fields.Add(_created = new ColladaObjectValue<DateTime>());
			Fields.Add(_keywords = new ColladaObjectValue<string>(""));
			Fields.Add(_modified = new ColladaObjectValue<DateTime>());
			Fields.Add(_revision = new ColladaObjectValue<string>(""));
			Fields.Add(_subject = new ColladaObjectValue<string>(""));
			Fields.Add(_title = new ColladaObjectValue<string>(""));
			Fields.Add(_unit = new ColladaObjectElement<ColladaAssetUnit>());
			Fields.Add(_upAxis = new ColladaObjectValue<Enums.ColladaUpAxisEnum>(Enums.ColladaUpAxisEnum.Y_UP));

			ValidationTests.Add(new ColladaIsNull(Enums.ColladaElementType.All, _contributor));
			ValidationTests.Add(new ColladaListMinCount<ColladaContributor>(Enums.ColladaElementType.All, _contributor, 1));
		}
	}
}
