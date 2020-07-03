/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
using System;
using System.Collections.Generic;
using System.Reflection;
using System.Text;

namespace BlamLib.TagInterface
{
	#region FieldAttributes
	/// <summary>
	/// Generic tag field description
	/// </summary>
	[AttributeUsage(AttributeTargets.Field, AllowMultiple = true)]
	public class FieldAttribute : System.Attribute
	{
		#region Type
		/// <summary>
		/// Field Type of this tag field
		/// </summary>
		public readonly TagInterface.FieldType Type;
		#endregion
	
		#region Name
		/// <summary>
		/// Display name of the tag field
		/// </summary>
		public string Name = string.Empty;
		#endregion

		#region Help
		/// <summary>
		/// String to provide further insight on
		/// the tag field
		/// </summary>
		public string Help = string.Empty;
		#endregion

		#region Units
		/// <summary>
		/// The unit of measurement this tag field
		/// is treated to be in
		/// </summary>
		public string Units = string.Empty;
		#endregion

		#region IsBlockName
		/// <summary>
		/// If this tag field is a child of a tag block,
		/// then it replaces the current tag block's
		/// element display name with its <c>ToString</c> value
		/// </summary>
		public bool IsBlockName = false;
		#endregion

		#region IsReadonly
		/// <summary>
		/// This tag field can be viewed by a user,
		/// but cannot be changed by them
		/// </summary>
		public bool IsReadonly = false;
		#endregion

		#region IsHidden
		/// <summary>
		/// This tag field should not be displayed
		/// by default to the user unless they are
		/// in "expert" mode
		/// </summary>
		public bool IsHidden = false;
		#endregion

		#region DefinitionIndex
		/// <summary>
		/// The index in the definition this field
		/// is associated with
		/// </summary>
		public int DefinitionIndex = -1;
		#endregion

		#region Constructors
		public static readonly FieldAttribute Null;
		static FieldAttribute()
		{
			Null = new FieldAttribute(BlamLib.TagInterface.FieldType.None);
		}

		/// <summary>
		/// Constructs a tag field description from an xml stream
		/// </summary>
		/// <param name="type"></param>
		/// <param name="s"></param>
		internal protected FieldAttribute(FieldType type, IO.XmlStream s)
		{
			Type = type;
			//s.ReadAttribute("type", ref Type);
			s.ReadAttributeOpt("name", ref Name);
			s.ReadAttributeOpt("help", ref Help);
			s.ReadAttributeOpt("units", ref Units);

			s.ReadAttributeOpt("blockname", ref IsBlockName);
			s.ReadAttributeOpt("readonly", ref IsReadonly);
			s.ReadAttributeOpt("hidden", ref IsHidden);
		}

		/// <summary>
		/// Constructs a tag field description that only specifies the type
		/// </summary>
		/// <param name="_type">Field Type of the tag field</param>
		protected FieldAttribute(TagInterface.FieldType _type) { Type = _type; }

		/// <summary>
		/// Constructs a tag field description that specifies the type and the display name of the field
		/// </summary>
		/// <param name="_type">Field Type of the tag field</param>
		/// <param name="_name">Display name of the tag field</param>
		public FieldAttribute(TagInterface.FieldType _type, string _name) : this(_type) { Name = _name; }

		/// <summary>
		/// Constructs a tag field description that specifies the type, display name, and
		/// a string to further describe this field to a user
		/// </summary>
		/// <param name="_type">Field Type of the tag field</param>
		/// <param name="_name">Display name of the tag field</param>
		/// <param name="_help">The string that will appear when help is requested on this tag field</param>
		public FieldAttribute(TagInterface.FieldType _type, string _name, string _help) : this(_type, _name) { Help = _help; }
		#endregion
	};

	/// <summary>
	/// Applies to a tag field which should be skipped when performing
	/// introspection
	/// </summary>
	[AttributeUsage(AttributeTargets.Field, AllowMultiple = true)]
	public class SkipFieldAttribute : FieldAttribute
	{
		const string k_Name = "skip";
		const string k_Help = "nothing to see here people";

		/// <summary>
		/// Constructs a skip field from an xml stream
		/// </summary>
		/// <param name="s"></param>
		internal protected SkipFieldAttribute(IO.XmlStream s) : this() {}
		/// <summary>
		/// Creates a regular skip field
		/// </summary>
		public SkipFieldAttribute() : base(BlamLib.TagInterface.FieldType.None, k_Name, k_Help) { }
		/// <summary>
		/// Creates a regular skip field, allowing the user to supply the field type
		/// </summary>
		/// <param name="field_type"></param>
		public SkipFieldAttribute(BlamLib.TagInterface.FieldType field_type) : base(field_type, k_Name, k_Help) { }
	};

	/// <summary>
	/// Applies to any tag field that has integer based fields
	/// </summary>
	[AttributeUsage(AttributeTargets.Field, AllowMultiple = true)]
	public class BoundedIntegerFieldAttribute : FieldAttribute
	{
		#region Min
		/// <summary>
		/// Minimum allowed value for this field
		/// </summary>
		public readonly int Min;
		#endregion

		#region Max
		/// <summary>
		/// Maximum allowed value for this field
		/// </summary>
		public readonly int Max;
		#endregion


		#region Constructors
		/// <summary>
		/// Constructs a bounded integer tag field description from an xml stream
		/// </summary>
		/// <param name="type"></param>
		/// <param name="s"></param>
		internal protected BoundedIntegerFieldAttribute(FieldType type, IO.XmlStream s) : base(type, s)
		{
			if(!s.ReadAttributeOpt("min", ref Min)) Min = 0;
			if(!s.ReadAttributeOpt("max", ref Max)) Max = 0;

			if (Units == string.Empty && Min != 0 && Max != 0)
				Units = string.Format("[{0},{1}]", Min.ToString(), Max.ToString());
		}

		/// <summary>
		/// Constructs a bounded integer tag field description with a set min and max value
		/// </summary>
		/// <param name="_type">The type of this tag field</param>
		/// <param name="_name">Display name of this tag field</param>
		/// <param name="_min">Minimum allowed value for this field</param>
		/// <param name="_max">Maximum allowed value for this field</param>
		public BoundedIntegerFieldAttribute(TagInterface.FieldType _type, string _name, int _min, int _max) : base(_type, _name)
		{
			Min = _min;
			Max = _max;
			Units = string.Format("[{0},{1}]", _min.ToString(), _max.ToString());
		}

		/// <summary>
		/// Constructs a bounded integer tag field description with a set min and max value
		/// </summary>
		/// <param name="_type">The type of this tag field</param>
		/// <param name="_name">Display name of this tag field</param>
		/// <param name="_help">The string that will appear when help is requested on this tag field</param>
		/// <param name="_min">Minimum allowed value for this field</param>
		/// <param name="_max">Maximum allowed value for this field</param>
		public BoundedIntegerFieldAttribute(TagInterface.FieldType _type, string _name, string _help, int _min, int _max) : this(_type, _name, _min, _max) { Help = _help; }

		/// <summary>
		/// Constructs a bounded short integer tag field description with a set min and max value
		/// </summary>
		/// <param name="_name">Display name of this tag field</param>
		/// <param name="_min">Minimum allowed value for this field</param>
		/// <param name="_max">Maximum allowed value for this field</param>
		public BoundedIntegerFieldAttribute(string _name, int _min, int _max) : this(BlamLib.TagInterface.FieldType.ShortInteger, _name, _min, _max) { }

		/// <summary>
		/// Constructs a bounded short integer tag field description with a set min and max value
		/// </summary>
		/// <param name="_name">Display name of this tag field</param>
		/// <param name="_help">The string that will appear when help is requested on this tag field</param>
		/// <param name="_min">Minimum allowed value for this field</param>
		/// <param name="_max">Maximum allowed value for this field</param>
		public BoundedIntegerFieldAttribute(string _name, string _help, int _min, int _max) : this(BlamLib.TagInterface.FieldType.ShortInteger, _name, _help, _min, _max) { }
		#endregion
	};

	/// <summary>
	/// Applies to any tag field that has floating point based fields
	/// </summary>
	[AttributeUsage(AttributeTargets.Field, AllowMultiple = true)]
	public class BoundedRealFieldAttribute : FieldAttribute
	{
		#region Min
		/// <summary>
		/// Minimum allowed value for this field
		/// </summary>
		public readonly float Min;
		#endregion

		#region Max
		/// <summary>
		/// Maximum allowed value for this field
		/// </summary>
		public readonly float Max;
		#endregion


		#region Constructors
		/// <summary>
		/// Constructs a bounded floating point tag field description from an xml stream
		/// </summary>
		/// <param name="type"></param>
		/// <param name="s"></param>
		internal protected BoundedRealFieldAttribute(FieldType type, IO.XmlStream s) : base(type, s)
		{
			if (!s.ReadAttributeOpt("min", ref Min)) Min = 0;
			if (!s.ReadAttributeOpt("max", ref Max)) Max = 0;

			if (Units == string.Empty && Min != 0 && Max != 0)
				Units = string.Format("[{0},{1}]", Min.ToString(), Max.ToString());
		}

		/// <summary>
		/// Constructs a bounded floating point tag field description with a set min and max value
		/// </summary>
		/// <param name="_type">The type of this tag field</param>
		/// <param name="_name">Display name of this tag field</param>
		/// <param name="_min">Minimum allowed value for this field</param>
		/// <param name="_max">Maximum allowed value for this field</param>
		public BoundedRealFieldAttribute(TagInterface.FieldType _type, string _name, float _min, float _max) : base(_type, _name)
		{
			Min = _min;
			Max = _max;
			Units = string.Format("[{0},{1}]", _min.ToString(), _max.ToString());
		}

		/// <summary>
		/// Constructs a bounded floating point tag field description with a set min and max value
		/// </summary>
		/// <param name="_type">The type of this tag field</param>
		/// <param name="_name">Display name of this tag field</param>
		/// <param name="_help">The string that will appear when help is requested on this tag field</param>
		/// <param name="_min">Minimum allowed value for this field</param>
		/// <param name="_max">Maximum allowed value for this field</param>
		public BoundedRealFieldAttribute(TagInterface.FieldType _type, string _name, string _help, float _min, float _max) : this(_type, _name, _min, _max) { Help = _help; }

		/// <summary>
		/// Constructs a bounded real tag field description with a set min and max value
		/// </summary>
		/// <param name="_name">Display name of this tag field</param>
		/// <param name="_min">Minimum allowed value for this field</param>
		/// <param name="_max">Maximum allowed value for this field</param>
		public BoundedRealFieldAttribute(string _name, float _min, float _max) : this(BlamLib.TagInterface.FieldType.Real, _name, _min, _max) { }

		/// <summary>
		/// Constructs a bounded real tag field description with a set min and max value
		/// </summary>
		/// <param name="_name">Display name of this tag field</param>
		/// <param name="_help">The string that will appear when help is requested on this tag field</param>
		/// <param name="_min">Minimum allowed value for this field</param>
		/// <param name="_max">Maximum allowed value for this field</param>
		public BoundedRealFieldAttribute(string _name, string _help, float _min, float _max) : this(BlamLib.TagInterface.FieldType.Real, _name, _help, _min, _max) { }
		#endregion
	};

	/// <summary>
	/// 
	/// </summary>
	[AttributeUsage(AttributeTargets.Field, AllowMultiple = true)]
	public class BlockIndexFieldAttribute : FieldAttribute
	{
		#region Block
		/// <summary>
		/// Block that this index field relies on
		/// </summary>
		public readonly Type Block;
		#endregion

		/// <summary>
		/// 
		/// </summary>
		/// <param name="type"></param>
		/// <param name="s"></param>
		internal protected BlockIndexFieldAttribute(FieldType type, IO.XmlStream s) : base(type, s)
		{
		}

		/// <summary>
		/// 
		/// </summary>
		/// <param name="_type"></param>
		/// <param name="_name"></param>
		/// <param name="_block"></param>
		public BlockIndexFieldAttribute(TagInterface.FieldType _type, string _name, Type _block) : base(_type, _name) { Block = _block; }

		/// <summary>
		/// 
		/// </summary>
		/// <param name="_name"></param>
		/// <param name="_block"></param>
		public BlockIndexFieldAttribute(string _name, Type _block) : this(TagInterface.FieldType.ShortBlockIndex, _name, _block) {}
	};

	/// <summary>
	/// Used to describe a range of tag fields
	/// </summary>
	[AttributeUsage(AttributeTargets.Field, AllowMultiple = true)]
	public class ExplanationFieldAttribute : FieldAttribute
	{
		internal protected ExplanationFieldAttribute(FieldType type, IO.XmlStream s) : base(type, s)
		{
		}

		/// <summary>
		/// Constructs a tag field Explanation
		/// </summary>
		/// <param name="heading">Heading text that summarizes the fields in one or two words</param>
		/// <param name="text">Long description of the fields</param>
		public ExplanationFieldAttribute(string heading, string text) : base(BlamLib.TagInterface.FieldType.Custom, heading, text) { }
	};

	/// <summary>
	/// Used to signify that a custom editor should be used
	/// </summary>
	[AttributeUsage(AttributeTargets.Field, AllowMultiple = true)]
	public class CustomFieldAttribute : FieldAttribute
	{
		internal protected CustomFieldAttribute(FieldType type, IO.XmlStream s) : base(type, s)
		{
		}

		/// <summary>
		/// Constructs a custom tag field
		/// </summary>
		/// <param name="tag">group tag of the custom field editor</param>
		public CustomFieldAttribute(string tag) : base(BlamLib.TagInterface.FieldType.Custom, tag) { }
	};

	#region EnumeratedFieldAttributes
	/// <summary>
	/// Abstract field description used on enumerated fields
	/// </summary>
	public abstract class EnumeratedFieldAttribute : FieldAttribute
	{
		#region Definition
		/// <summary>
		/// The type or string list that holds the enumerated members with <see cref="EnumAttribute"/>s
		/// </summary>
		public readonly object Definition;
		#endregion

		#region Members
		protected List<EnumAttribute> members;
		public List<EnumAttribute> GetMembers() { return members; }

		protected static void BuildMembersList(EnumeratedFieldAttribute attr)
		{
			try
			{
				if (attr.Definition is Type)
				{
					uint[] values = System.Enum.GetValues(attr.Definition as Type) as uint[];
					FieldInfo[] fields = (attr.Definition as Type).GetFields();
					//bool is_flag = attr is FlagFieldAttribute;
					object[] attrs = null;
					EnumAttribute enumattr = null;

					int index = 0;
					foreach (FieldInfo fi in fields)
					{
						if (fi.IsSpecialName) continue;

						attrs = fi.GetCustomAttributes(typeof(EnumeratedFieldAttribute), false);

						if (attrs.Length > 0)
						{
							foreach (Attribute a in attrs) // TODO: WTC cooper, this is redundant, fix it then verify this shit!
								if (a is EnumAttribute)
								{
									enumattr = a as EnumAttribute;
									break;
								}

							if (enumattr.IsIgnored())
								enumattr.Name = "unused" + index;

							enumattr.SetValue(values[index]);
							attr.members.Add(enumattr);
							enumattr = null;
						}

						++index;
					}
				}
				else if (attr.Definition is string[])
				{
					foreach (string s in attr.Definition as string[])
						attr.members.Add(new EnumAttribute(s));
				}
				else
					throw new Debug.ExceptionLog("EnumeratedFieldAttribute was given a bad definition. {0}", attr.Definition.GetType().FullName);
			}
			catch (Exception ex)
			{
				Debug.LogFile.WriteLine("EnumeratedFieldAttribute::BuildMembersList error \r\n{0}", ex);
			}
		}
		#endregion

		/// <summary>
		/// Constructs a enumerated field type
		/// </summary>
		/// <param name="_type">Enumerated type</param>
		/// <param name="_name">Display of this tag field</param>
		/// <param name="def">Type containing the enum members</param>
		protected EnumeratedFieldAttribute(TagInterface.FieldType _type, string _name, Type def) : base(_type, _name)
		{
			Definition = def;

			if (_type == BlamLib.TagInterface.FieldType.ByteFlags)		members = new List<EnumAttribute>(8);
			else if (_type == BlamLib.TagInterface.FieldType.WordFlags)	members = new List<EnumAttribute>(16);
			else if (_type == BlamLib.TagInterface.FieldType.LongFlags)	members = new List<EnumAttribute>(32);
			else														members = new List<EnumAttribute>();

			BuildMembersList(this);
		}

		/// <summary>
		/// Constructs a enumerated field type
		/// </summary>
		/// <param name="_type">Enumerated type</param>
		/// <param name="_name">Display of this tag field</param>
		/// <param name="def">string list containing the enum members</param>
		protected EnumeratedFieldAttribute(TagInterface.FieldType _type, string _name, params string[] def) : base(_type, _name)
		{
			Definition = def;

			if (_type == BlamLib.TagInterface.FieldType.ByteFlags)		members = new List<EnumAttribute>(8);
			else if (_type == BlamLib.TagInterface.FieldType.WordFlags)	members = new List<EnumAttribute>(16);
			else if (_type == BlamLib.TagInterface.FieldType.LongFlags)	members = new List<EnumAttribute>(32);
			else														members = new List<EnumAttribute>();

			BuildMembersList(this);
		}
	};

	/// <summary>
	/// Tag field description used on enum fields
	/// </summary>
	[AttributeUsage(AttributeTargets.Field, AllowMultiple = true)]
	public class EnumFieldAttribute : EnumeratedFieldAttribute
	{
		/// <summary>
		/// Constructs a byte, word, or long enum tag field
		/// </summary>
		/// <param name="_type">Enum type</param>
		/// <param name="_name">Display of this tag field</param>
		/// <param name="def">Type containing the enum members</param>
		public EnumFieldAttribute(TagInterface.FieldType _type, string _name, Type def) : base(_type, _name, def) {}

		/// <summary>
		/// Constructs a word enum tag field
		/// </summary>
		/// <param name="_name">Display of this tag field</param>
		/// <param name="def">Type containing the enum members</param>
		public EnumFieldAttribute(string _name, Type def) : base(BlamLib.TagInterface.FieldType.Enum, _name, def) { }

		/// <summary>
		/// Constructs a byte, word, or long enum tag field
		/// </summary>
		/// <param name="_type">Enum type</param>
		/// <param name="_name">Display of this tag field</param>
		/// <param name="def">string list containing the enum members</param>
		public EnumFieldAttribute(TagInterface.FieldType _type, string _name, params string[] def) : base(_type, _name, def) { }

		/// <summary>
		/// Constructs a word enum tag field
		/// </summary>
		/// <param name="_name">Display of this tag field</param>
		/// <param name="def">string list containing the enum members</param>
		public EnumFieldAttribute(string _name, params string[] def) : base(BlamLib.TagInterface.FieldType.Enum, _name, def) { }
	};

	/// <summary>
	/// Tag field description used on flag fields
	/// </summary>
	[AttributeUsage(AttributeTargets.Field, AllowMultiple = true)]
	public class FlagFieldAttribute : EnumeratedFieldAttribute
	{
		/// <summary>
		/// Constructs a byte, word, or long flags tag field
		/// </summary>
		/// <param name="_type">Flags type</param>
		/// <param name="_name">Display of this tag field</param>
		/// <param name="def">Type containing the enum members</param>
		public FlagFieldAttribute(TagInterface.FieldType _type, string _name, Type def) : base(_type, _name, def) {}

		/// <summary>
		/// Constructs a long flags tag field
		/// </summary>
		/// <param name="_name">Display of this tag field</param>
		/// <param name="def">Type containing the enum members</param>
		public FlagFieldAttribute(string _name, Type def) : base(BlamLib.TagInterface.FieldType.LongFlags, _name, def) { }

		/// <summary>
		/// Constructs a byte, word, or long flags tag field
		/// </summary>
		/// <param name="_type">Flags type</param>
		/// <param name="_name">Display of this tag field</param>
		/// <param name="def">string list containing the enum members</param>
		public FlagFieldAttribute(TagInterface.FieldType _type, string _name, params string[] def) : base(_type, _name, def) { }

		/// <summary>
		/// Constructs a long flags tag field
		/// </summary>
		/// <param name="_name">Display of this tag field</param>
		/// <param name="def">string list containing the enum members</param>
		public FlagFieldAttribute(string _name, params string[] def) : base(BlamLib.TagInterface.FieldType.LongFlags, _name, def) { }
	};
	#endregion

	/// <summary>
	/// Advanced tag field description for tag block fields
	/// </summary>
	[AttributeUsage(AttributeTargets.Field, AllowMultiple = true)]
	public class BlockFieldAttribute : FieldAttribute
	{
		#region MaxElements
		/// <summary>
		/// Max elements this tag block can hold
		/// </summary>
		/// <remarks>0 defaults to infinite</remarks>
		public readonly int MaxElements = 0;
		#endregion

		#region DefaultBlockName
		/// <summary>
		/// The default display name for this
		/// tag block's element names
		/// </summary>
		/// <remarks>The display name is generated in the format of "<c>{0}. DefaultBlockName</c>" 
		/// where <c>{0}</c> is replaced with the element index
		/// </remarks>
		public readonly string DefaultBlockName = string.Empty;
		#endregion

		#region ElementNames
		/// <summary>
		/// The list of display names that
		/// each element should use
		/// </summary>
		/// <remarks><c>ElementNames[0]</c> would for instance be the display name of the 0th element</remarks>
		public readonly string[] ElementNames = null;
		#endregion

		#region Definition
		/// <summary>
		/// A <c>System.Type</c> object of a <c>TagInterface.Definition</c> 
		/// object that describes this tag block's format
		/// </summary>
		public readonly Type Definition;
		#endregion


		#region Constructors
		/// <summary>
		/// Constructs a tag block description that specifies the structure definition type
		/// and the display name of this tag block
		/// </summary>
		/// <param name="def">Structure definition to use for this tag block</param>
		/// <param name="_name">Display name of this tag block</param>
		public BlockFieldAttribute(Type def, string _name) : base(BlamLib.TagInterface.FieldType.Block, _name)
		{
			Definition = def;
			Name = _name;

			DefaultBlockName = def.Name;
		}

		/// <summary>
		/// Constructs a tag block description that specifies the structure definition type,
		/// the display name, the default element display name, and the max amount of elements
		/// this tag block can hold
		/// </summary>
		/// <param name="def">Structure definition to use for this tag block</param>
		/// <param name="_name">Display name of this tag block</param>
		/// <param name="default_name">Default display name for this tag block's elements</param>
		/// <param name="max">Max element count this tag block can hold</param>
		public BlockFieldAttribute(Type def, string _name, string default_name, int max) : base(BlamLib.TagInterface.FieldType.Block, _name)
		{
			Definition = def;
			MaxElements = max;
			DefaultBlockName = default_name;
		}

		/// <summary>
		/// Constructs a tag block description that specifies the structure definition type,
		/// display name, the max amount of elements this tag block can hold, and the list of
		/// display names this tag block uses.
		/// </summary>
		/// <param name="def">Structure definition to use for this tag block</param>
		/// <param name="_name">Display name of this tag block</param>
		/// <param name="max">Max element count this tag block can hold</param>
		/// <param name="element_names">List of display names for each element</param>
		public BlockFieldAttribute(Type def, string _name, int max, params string[] element_names) : this(def, _name)
		{
			MaxElements = max;
			ElementNames = element_names;
		}
		#endregion
	};

	/// <summary>
	/// Advanced tag field description for struct fields
	/// </summary>
	[AttributeUsage(AttributeTargets.Field, AllowMultiple = true)]
	public class StructFieldAttribute : FieldAttribute
	{
		#region GroupIndex
		/// <summary>
		/// Index of the group tag for this struct
		/// </summary>
		public readonly int GroupIndex = -1;
		#endregion

		#region Definition
		/// <summary>
		/// A <c>System.Type</c> object of a <c>TagInterface.Definition</c> 
		/// object that describes this tag struct's format
		/// </summary>
		public readonly Type Definition;
		#endregion

		public StructFieldAttribute(string _name, int group_tag, Type def) : base(BlamLib.TagInterface.FieldType.Struct, _name)
		{
			GroupIndex = group_tag;
			Definition = def;
		}
	};

	/// <summary>
	/// Advanced tag field description for tag data fields
	/// </summary>
	[AttributeUsage(AttributeTargets.Field, AllowMultiple = true)]
	public class DataFieldAttribute : FieldAttribute
	{
		#region MaxSize
		/// <summary>
		/// Max amount of bytes this tag data field can hold
		/// </summary>
		/// <remarks>If MaxSizePC is used, this field is treated as the xbox size</remarks>
		public readonly int MaxSize = 0;
		#endregion

		#region MaxSizePC
		/// <summary>
		/// Max amount of bytes this tag data field can hold
		/// when built for a pc or mac platform
		/// </summary>
		public readonly int MaxSizePC;
		#endregion


		#region Constructors
		/// <summary>
		/// Constructs a tag data field description that specifics the display name
		/// </summary>
		/// <param name="_name">Display name of this data field</param>
		public DataFieldAttribute(string _name) : base(BlamLib.TagInterface.FieldType.Data, _name) {}

		/// <summary>
		/// Constructs a tag data field description that specifics the display name,
		/// and max size of this data field
		/// </summary>
		/// <param name="_name">Display name of this data field</param>
		/// <param name="max">Max size in bytes of this data field</param>
		public DataFieldAttribute(string _name, int max) : this(_name) { MaxSize = MaxSizePC = max; }

		/// <summary>
		/// Constructs a tag data field description that specifics the display name,
		/// and max size of this data field on both the xbox and pc target platforms
		/// </summary>
		/// <param name="_name">Display name of this data field</param>
		/// <param name="max_xbox">Max size in bytes of this data field on the xbox platform</param>
		/// <param name="max_pc">Max size in bytes of this data field on a pc platform</param>
		public DataFieldAttribute(string _name, int max_xbox, int max_pc) : this(_name) { MaxSize = max_xbox; MaxSizePC = max_pc; }
		#endregion
	};

	/// <summary>
	/// Advanced tag field description for tag reference fields
	/// </summary>
	[AttributeUsage(AttributeTargets.Field, AllowMultiple = true)]
	public class ReferenceFieldAttribute : FieldAttribute
	{
		#region GroupIndex
		/// <summary>
		/// Index of the group tag this field references
		/// </summary>
		/// <remarks>Index is relative to the <c>Blam.[Game].TagGroups.Groups</c> collection</remarks>
		public readonly int GroupIndex = -1;
		#endregion

		#region GroupIndices
		/// <summary>
		/// Indices of group tags this field is capable of referencing
		/// </summary>
		/// <remarks>Each index is relative to the <c>Blam.[Game].TagGroups.Groups</c> collection</remarks>
		public readonly int[] GroupIndices = null;
		#endregion
	

		#region Constructors
		/// <summary>
		/// Constructs a tag reference description that can reference any tag group
		/// </summary>
		/// <param name="_name">Display name of this tag field</param>
		public ReferenceFieldAttribute(string _name) : base(BlamLib.TagInterface.FieldType.TagReference, _name) {}

		/// <summary>
		/// Constructs a tag reference description that can reference only a single tag group
		/// </summary>
		/// <param name="_name">Display name of this tag field</param>
		/// <param name="group_tag">Group tag to reference</param>
		public ReferenceFieldAttribute(string _name, int group_tag) : base(BlamLib.TagInterface.FieldType.TagReference, _name) { GroupIndex = group_tag; }

		/// <summary>
		/// Constructs a tag reference description that can reference a list of different tag groups
		/// </summary>
		/// <param name="_name">Display name of this tag field</param>
		/// <param name="group_tags">Group tags to reference</param>
		public ReferenceFieldAttribute(string _name, params int[] group_tags) : base (BlamLib.TagInterface.FieldType.TagReference, _name) { GroupIndices = group_tags; }
		#endregion
	};
	#endregion

	/// <summary>
	/// Attribute applied to a enum member to define its
	/// required editor information
	/// </summary>
	[AttributeUsage(AttributeTargets.All, AllowMultiple = false)] // not really all, but thats our only option
	public class EnumAttribute : System.Attribute
	{
		/// <summary>
		/// The value that is given to the <see cref="ValueOverride"/> field
		/// when its not used
		/// </summary>
		public const uint OverrideDefault = Program.Constants.SentinalUInt32;

		#region Name
		/// <summary>
		/// Display name of the enum member
		/// </summary>
		/// <remarks>this will be set "unused" if this object <see cref="IsIgnored"/></remarks>
		public string Name;
		#endregion

		#region Help
		private string help;
		/// <summary>
		/// String to provide further insight on the enum member
		/// </summary>
		public readonly string Help;
		#endregion
	
		#region ValueOverride
		/// <summary>
		/// Value override to use, instead of just incrementing the value by one using the last enum member
		/// </summary>
		public readonly uint ValueOverride;
		#endregion

		#region Value
		protected uint value = OverrideDefault;
		/// <summary>
		/// Set the runtime value of this enum member
		/// </summary>
		/// <param name="value"></param>
		public void SetValue(uint value) { this.value = value; }
		/// <summary>
		/// Get the runtime value of this enum member
		/// </summary>
		/// <returns></returns>
		public uint GetValue() { return value; }
		#endregion


		#region Constructors
		/// <summary>
		/// Constructs a enum member description that says the member should be ignored
		/// </summary>
		/// <remarks>At runtime, this object's <c>value</c> will be set and its name will be set to "unused"</remarks>
		public EnumAttribute()
		{
			Name = help = string.Empty;
			ValueOverride = OverrideDefault;
		}

		/// <summary>
		/// Constructs a enum member description
		/// </summary>
		/// <param name="_name">Display name of this member</param>
		public EnumAttribute(string _name)
		{
			Name = _name;
			Help = string.Empty;
			ValueOverride = OverrideDefault;
		}

		/// <summary>
		/// Constructs a enum member description
		/// </summary>
		/// <param name="_name">Display name of this member</param>
		/// <param name="_help">The string that will appear when help is requested on this enum member</param>
		public EnumAttribute(string _name, string _help)
		{
			Name = _name;
			Help = _help;
			ValueOverride = OverrideDefault;
		}

		/// <summary>
		/// Constructs a enum member description with a value override
		/// </summary>
		/// <param name="_name">Display name of this member</param>
		/// <param name="_value">Value to use</param>
		public EnumAttribute(string _name, uint _value)
		{
			Name = _name;
			Help = string.Empty;
			ValueOverride = _value;
		}

		/// <summary>
		/// Constructs a enum member description with a value override
		/// </summary>
		/// <param name="_name">Display name of this member</param>
		/// <param name="_help">The string that will appear when help is requested on this enum member</param>
		/// <param name="_value">Value to use</param>
		public EnumAttribute(string _name, string _help, uint _value)
		{
			Name = _name;
			Help = _help;
			ValueOverride = _value;
		}
		#endregion

		/// <summary>
		/// Checks whether if this enum member is used in the members list
		/// </summary>
		/// <returns>True if this member should be ignored</returns>
		public bool IsIgnored() { return ValueOverride == OverrideDefault; }
		public override bool IsDefaultAttribute() { return Name == string.Empty || help == string.Empty && ValueOverride == OverrideDefault; }
	};

	#region Definition attributes
	/// <summary>
	/// Attribute applied to a Definition class to define its 
	/// required editor information
	/// </summary>
	[AttributeUsage(AttributeTargets.Class, AllowMultiple = false, Inherited = false)]
	public class DefinitionAttribute : System.Attribute
	{
		#region Version
		/// <summary>
		/// Version of this structure definition
		/// </summary>
		public readonly short Version;
		#endregion

		#region SizeOf
		/// <summary>
		/// Size of this structure definition
		/// </summary>
		public readonly int SizeOf;
		#endregion

		#region Type
		/// <summary>
		/// Type object of the Definition this attribute applies to
		/// </summary>
		public readonly Type Type;
		#endregion

		public /*readonly*/ Guid StructureGuid = Guid.Empty;

		/// <summary>
		/// Key = Field attribute of a certain field in Type
		/// Value = Index of the field
		/// </summary>
		public Dictionary<FieldAttribute, int> Fields = new Dictionary<FieldAttribute, int>();

		/// <summary>
		/// Constructs a Definition description
		/// </summary>
		public DefinitionAttribute() { Version = 1; SizeOf = -1; }

		/// <summary>
		/// Constructs a Definition description with a specific version identifier
		/// </summary>
		/// <param name="version">Version of this structure definition</param>
		public DefinitionAttribute(short version) { Version = version; SizeOf = -1; }

		/// <summary>
		/// Constructs a Definition description with a specific version identifier and size
		/// </summary>
		/// <param name="version">Version of this structure definition</param>
		/// <param name="size_of">Size of this structure definition</param>
		public DefinitionAttribute(short version, int size_of) { Version = version; SizeOf = size_of; }

		/// <summary>
		/// Constructs a Definition description with a specific version identifier and size
		/// </summary>
		/// <param name="version">Version of this structure definition</param>
		/// <param name="size_of">Size of this structure definition</param>
		/// <param name="guid">Structure identifier. String format: "{dddddddd-dddd-dddd-dddd-dddddddddddd}"</param>
		public DefinitionAttribute(short version, int size_of, string guid) : this(version, size_of) { StructureGuid = new Guid(guid); }

		public static DefinitionAttribute FromType(Type t)
		{
			object[] attrs = t.GetCustomAttributes(typeof(DefinitionAttribute), false);
			if (attrs.Length == 0)
				throw new Debug.ExceptionLog("Type {0} doesn't have this attribute applied to it", t.FullName);
			else
				return (DefinitionAttribute)attrs[0]; // attrs will have DefinitionAttribute only objects, so the 'as' operator would be redundant here
		}
	};

	/// <summary>
	/// Attribute applied to a Definition class to define its 
	/// required editor information
	/// </summary>
	[AttributeUsage(AttributeTargets.Class, AllowMultiple = false, Inherited = false)]
	public class StructAttribute : DefinitionAttribute
	{
		#region GroupIndex
		/// <summary>
		/// Index of this struct's <see cref="TagInterface.TagGroup"/> definition
		/// </summary>
		/// <remarks>Index is relative to the <c>Blam.[Game].TagGroups.Structs</c> collection</remarks>
		public readonly int GroupIndex;
		#endregion

		/// <summary>
		/// Constructs a Struct description
		/// </summary>
		/// <param name="group_tag"></param>
		public StructAttribute(int group_tag) : base() { GroupIndex = group_tag; }

		/// <summary>
		/// Constructs a Struct description with a specific version identifier
		/// </summary>
		/// <param name="group_tag"></param>
		/// <param name="version">Version of this structure definition</param>
		public StructAttribute(int group_tag, short version) : base(version) { GroupIndex = group_tag; }

		/// <summary>
		/// Constructs a Struct description with a specific version identifier
		/// </summary>
		/// <param name="group_tag"></param>
		/// <param name="version">Version of this structure definition</param>
		/// <param name="size_of">Size of this structure definition</param>
		public StructAttribute(int group_tag, short version, int size_of) : base(version, size_of) { GroupIndex = group_tag; }

		/// <summary>
		/// Constructs a Struct description with a specific version identifier
		/// </summary>
		/// <param name="group_tag"></param>
		/// <param name="version">Version of this structure definition</param>
		/// <param name="size_of">Size of this structure definition</param>
		/// <param name="guid">Structure identifier. String format: "{dddddddd-dddd-dddd-dddd-dddddddddddd}"</param>
		public StructAttribute(int group_tag, short version, int size_of, string guid) : base(version, size_of, guid) { GroupIndex = group_tag; }

		public static new StructAttribute FromType(Type t)
		{
			object[] attrs = t.GetCustomAttributes(typeof(StructAttribute), false);
			if (attrs.Length == 0)
				throw new Debug.ExceptionLog("Type {0} doesn't have this attribute applied to it", t.FullName);
			else
				return (StructAttribute)attrs[0]; // attrs will have StructAttribute only objects, so the 'as' operator would be redundant here
		}
	};

	/// <summary>
	/// Attribute applied to a Definition class to define its 
	/// required editor information
	/// </summary>
	[AttributeUsage(AttributeTargets.Class, AllowMultiple = false, Inherited = false)]
	public class TagGroupAttribute : DefinitionAttribute
	{
		#region GroupIndex
		/// <summary>
		/// Index of this tag group's <see cref="TagInterface.TagGroup"/> definition
		/// </summary>
		/// <remarks>Index is relative to the <c>Blam.[Game].TagGroups.Groups</c> collection</remarks>
		public readonly int GroupIndex;
		#endregion

		#region TagVersion
		/// <summary>
		/// Version of this tag definition
		/// </summary>
		public readonly short TagVersion;
		#endregion

		public readonly Type ParentType;

		// TODO: Guid ctor variants

		public TagGroupAttribute(int group_tag, short tag_version, short block_version, int size_of, Type parent) : base(block_version, size_of)
		{
			GroupIndex = group_tag;
			TagVersion = tag_version;
			ParentType = parent;
		}

		/// <summary>
		/// Constructs a Tag Group description with a specific version identifier
		/// </summary>
		/// <param name="group_tag"></param>
		/// <param name="tag_version">Version of this structure definition</param>
		public TagGroupAttribute(int group_tag, short tag_version) : this(group_tag, tag_version, 1, -1, null) { }

		/// <summary>
		/// Constructs a Tag Group description with a specific version identifier
		/// </summary>
		/// <param name="group_tag"></param>
		/// <param name="tag_version">Version of this structure definition</param>
		/// <param name="size_of">Size of this structure definition</param>
		public TagGroupAttribute(int group_tag, short tag_version, int size_of) : this(group_tag, tag_version, 1, size_of, null) { }

		/// <summary>
		/// Constructs a Tag Group description with a specific version identifier
		/// </summary>
		/// <param name="group_tag"></param>
		/// <param name="tag_version">Version of this structure definition</param>
		/// <param name="size_of">Size of this structure definition</param>
		/// <param name="parent"></param>
		public TagGroupAttribute(int group_tag, short tag_version, int size_of, Type parent) : this(group_tag, tag_version, 1, size_of, parent) { }

		/// <summary>
		/// Constructs a Tag Group description with a specific version identifier
		/// </summary>
		/// <param name="group_tag"></param>
		/// <param name="tag_version">Version of this structure definition</param>
		/// <param name="block_version">Version of the tag group's block</param>
		/// <param name="size_of">Size of this structure definition</param>
		public TagGroupAttribute(int group_tag, short tag_version, short block_version, int size_of) : this(group_tag, tag_version, block_version, size_of, null) { }

		public static new TagGroupAttribute FromType(Type t)
		{
			object[] attrs = t.GetCustomAttributes(typeof(TagGroupAttribute), false);
			if (attrs.Length == 0)
				throw new Debug.ExceptionLog("Type {0} doesn't have this attribute applied to it", t.FullName);
			else
				return (TagGroupAttribute)attrs[0]; // attrs will have TagGroupAttribute only objects, so the 'as' operator would be redundant here
		}
	};
	#endregion

	/// <summary>
	/// Utility class for helping with the interfacing between the Forms
	/// code and the attributes applied to a object
	/// </summary>
	public static class AttributeUtil
	{
		#region Field Controls
		private static readonly Type[] FieldControlsForms = new Type[] {
			null,//typeof(Forms.String),
			null,//typeof(Forms.StringId),

//			typeof(Forms.ByteInteger),
			null,//typeof(Forms.ShortInteger),
//			typeof(Forms.LongInteger),
//			typeof(Forms.Angle),
			null,//typeof(Forms.Tag),

//			typeof(Forms.Enum),
//			typeof(Forms.Enum),
//			typeof(Forms.Enum),

//			typeof(Forms.Flags),
//			typeof(Forms.Flags),
//			typeof(Forms.Flags),

			null,//typeof(Forms.Point2D),
			null,//typeof(Forms.Rectangle2D),

//			typeof(Forms.Color),
//			typeof(Forms.Color),

			null,//typeof(Forms.Real),
			null,//typeof(Forms.RealFraction),
			
			null,//typeof(Forms.RealPoint2D),
			null,//typeof(Forms.RealPoint3D),
			null,//typeof(Forms.RealVector2D),
			null,//typeof(Forms.RealVector3D),
			null,//typeof(Forms.RealQuaternion),
			null,//typeof(Forms.RealPlane2D),
			null,//typeof(Forms.RealPlane3D),

			null,//typeof(Forms.RealColor),
			null,//typeof(Forms.RealColor),

			null,//typeof(Forms.ShortBounds),
//			typeof(Forms.AngleBounds),
			null,//typeof(Forms.RealBounds),
			null,//typeof(Forms.RealFractionBounds),

			null,//typeof(Forms.TagReference),
//			typeof(Forms.Block),

//			typeof(Forms.BlockIndex),
//			typeof(Forms.BlockIndex),
//			typeof(Forms.BlockIndex),

//			typeof(Forms.Data),

			null, // Struct
			null, // ArrayStart
			null, // ArrayEnd
			null, // Pad
			null, // UnknownPad
			null, // Skip
			null, // Custom. TODO: This needs special code construction

			null, // AnimationBlock

			null, // Terminator
		};
		#endregion

		//private static void FieldFromDefinitionSetup(DefinitionAttribute owner, FieldAttribute attr, Forms.Field field)
		//{
		//}

		//public static Forms.Field FieldFromDefinition(DefinitionAttribute owner, FieldAttribute fi)
		//{
		//	return null;
		//}

		private static DefinitionAttribute GetDefinition(Type def_class)
		{
			object[] attr = def_class.GetCustomAttributes(typeof(DefinitionAttribute), false);
			if (attr.Length == 0) return null;

			return attr[0] as DefinitionAttribute;
		}

// 		/// <summary>
// 		/// Create an editor for a block from a Definition class
// 		/// </summary>
// 		/// <param name="def_class">Type object of the Definition class</param>
// 		public static Forms.Block BlockFromDefinition(Type def_class)
// 		{
// 			DefinitionAttribute da = GetDefinition(def_class);
// 
// 			return null;
// 		}

		/// <summary>
		/// Create an editor for a tag group from a Definition class
		/// </summary>
		/// <param name="def_class">Type object of the Definition class</param>
		public static void TagGroupFromDefintion(Type def_class)
		{
			DefinitionAttribute da = GetDefinition(def_class);
		}

		public static bool FieldTypeIsValidForAttribute(FieldType t)
		{
			switch(t)
			{
				case FieldType.VertexBuffer:
				//case FieldType.Struct:
				case FieldType.ArrayStart:
				case FieldType.ArrayEnd:
				case FieldType.Pad:
				case FieldType.UnknownPad:
				case FieldType.UselessPad:
				case FieldType.Skip: // we use skip for hidding fields from the UI
				case FieldType.Terminator:
				//case FieldType.None:
					return false;

				default: return true;
			}
		}
	};
}