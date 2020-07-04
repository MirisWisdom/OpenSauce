/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
using System;
using System.Collections.Generic;
using System.Reflection;

namespace BlamLib.TagInterface
{
	#region Runtime data
	/// <summary>
	/// Runtime calculated counts of special case <see cref="FieldType"/>s
	/// </summary>
	/// <remarks>Not including any inline structures</remarks>
	struct DefinitionRuntimeFieldCounts
	{
		#region Flags
		/// <summary>
		/// Has tag reference field(s)
		/// </summary>
		internal const uint kFlagHasTagReference = 1 << 0;
		/// <summary>
		/// Has tag block field(s)
		/// </summary>
		internal const uint kFlagHasTagBlock = 1 << 1;
		/// <summary>
		/// Has tag data field(s)
		/// </summary>
		internal const uint kFlagHasTagData = 1 << 2;
		/// <summary>
		/// Has pad, unknown pad or skip field(s)
		/// </summary>
		internal const uint kFlagHasUnmarkedData = 1 << 3;
		/// <summary>
		/// Has useless padding field(s)
		/// </summary>
		internal const uint kFlagHasUselessPadding = 1 << 4;
		/// <summary>
		/// Has tag struct field(s)
		/// </summary>
		internal const uint kFlagHasTagStruct = 1 << 5;
		/// <summary>
		/// Has "anal" fields (ie, pc only fields)
		/// </summary>
		internal const uint kFlagHasAnalFields = 1 << 6;
		/// <summary>
		/// Has <see cref="FieldType.CustomData"/> fields
		/// </summary>
		internal const uint kFlagHasCustomDataFields = 1 << 7;

		uint flags;
		public uint Flags { get { return flags; } }

		internal void SetHasAnalFields() { Util.Flags.Add(ref flags, kFlagHasAnalFields); }
		#endregion

		int fieldCountTagReferenes, fieldCountTagBlocks, fieldCountTagData, fieldCountTagStructs,
			fieldCountUnmarkedData, // Pad, UnknownPad or Skip
			fieldCountUselessPadding
			;

		internal void ProcessField(BlamVersion engine, DefinitionState owner, Field f, ref int old_string_id_count)
		{
			switch(f.FieldType)
			{
				case FieldType.OldStringId:	old_string_id_count++;		break;
				case FieldType.TagReference:fieldCountTagReferenes++;	break;
				case FieldType.Block:		fieldCountTagBlocks++;		break;
				case FieldType.Data:		fieldCountTagData++;		break;

				#region Struct
				case FieldType.Struct:
					fieldCountTagStructs++;
					if ((engine & BlamVersion.Halo2) != 0)
					{
						DefinitionState struct_state = (f as IStruct).GetDefinition();

						owner.RuntimeSizes.RuntimeSize -= struct_state.Attribute.SizeOf - struct_state.RuntimeSizes.RuntimeSize;
						owner.RuntimeSizes.AddSizeInformation(struct_state);
					}
					break;
				#endregion
				#region Pad
				case FieldType.Pad:
					fieldCountUnmarkedData++;
					owner.RuntimeSizes.TotalPadSize += (f as Pad).Value;
					break;
				#endregion
				#region UnknownPad
				case FieldType.UnknownPad:
					fieldCountUnmarkedData++;
					owner.RuntimeSizes.TotalPadSize += (f as UnknownPad).Value;
					break;
				#endregion
				#region UselessPad
				case FieldType.UselessPad:
					fieldCountUselessPadding++;
					owner.RuntimeSizes.TotalUselessPad += (f as UselessPad).Value;
					break;
				#endregion
				#region Skip
				case FieldType.Skip:
					fieldCountUnmarkedData++;
					owner.RuntimeSizes.TotalSkipSize += (f as Skip).Value;
					break;
				#endregion
				#region CustomData
				case FieldType.CustomData:
					flags |= kFlagHasCustomDataFields;
					break;
				#endregion
			}
		}

		internal void ProcessTypesFinishCalculations(BlamVersion engine, DefinitionState owner, int old_string_id_count)
		{
			int runtime_size = owner.RuntimeSizes.RuntimeSize;

			if (old_string_id_count > 0)
				owner.RuntimeSizes.TotalOldStringIdSize += (old_string_id_count * 28);
			if (fieldCountTagReferenes > 0)
			{
				flags |= kFlagHasTagReference;
				// Isn't the case for Halo 2 pre-ship builds, but whatever...
				if ((engine & BlamVersion.Halo2) != 0)
					runtime_size -= (8 * fieldCountTagReferenes);
			}
			if (fieldCountTagBlocks > 0)
			{
				flags |= kFlagHasTagBlock;
				// Isn't the case for Halo 2 pre-ship builds, but whatever...
				if ((engine & BlamVersion.Halo2) != 0)
					runtime_size -= (4 * fieldCountTagBlocks);
			}
			if (fieldCountTagData > 0)
			{
				flags |= kFlagHasTagData;
				// Isn't the case for Halo 2 pre-ship builds, but whatever...
				if ((engine & BlamVersion.Halo2) != 0)
					runtime_size -= (12 * fieldCountTagData);
			}
			if (fieldCountTagStructs > 0)		flags |= kFlagHasTagStruct;
			if (fieldCountUselessPadding > 0)	flags |= kFlagHasUselessPadding;

			owner.RuntimeSizes.RuntimeSize = runtime_size;
		}
	};
	/// <summary>
	/// Runtime calculated sizes of the overall definition and special case field types
	/// </summary>
	struct DefinitionRuntimeSizes
	{
		public int SizeOf { get; private set; }

		public int TotalOldStringIdSize { get; internal set; }

		#region TotalPadSize
		/// <summary>
		/// Total size in bytes of padding
		/// </summary>
		public int TotalPadSize { get; internal set; }
		/// <summary>
		/// Returns true if this definition has any padding fields
		/// </summary>
		public bool HasPadding { get { return TotalPadSize != 0; } }
		#endregion

		#region TotalUselessPad
		/// <summary>
		/// Total size in bytes of useless padding (not actually included in
		/// most recent structures, but still exist in old tag formats)
		/// </summary>
		public int TotalUselessPad { get; internal set; }
		#endregion

		#region TotalSkipSize
		/// <summary>
		/// Total size in bytes of the skipped fields
		/// </summary>
		public int TotalSkipSize { get; internal set; }
		/// <summary>
		/// Returns true if this definition has any skipped fields
		/// </summary>
		public bool HasSkipped { get { return TotalSkipSize != 0; } }
		#endregion

		#region RuntimeSize
		/// <summary>
		/// Size of the definition for runtime builds
		/// </summary>
		/// <remarks>Currently doesn't access inline tag struct runtime sizes</remarks>
		public int RuntimeSize { get; internal set; }
		#endregion

		internal void AddSizeInformation(DefinitionState add_state)
		{
			TotalUselessPad += add_state.RuntimeSizes.TotalUselessPad;
			TotalOldStringIdSize += add_state.RuntimeSizes.TotalOldStringIdSize;
			TotalSkipSize += add_state.RuntimeSizes.TotalSkipSize;
		}
		internal void ProcessSizeInformation(DefinitionState owner)
		{
			RuntimeSize = SizeOf = owner.Attribute.SizeOf;

			if (owner.ParentState != null)
			{
				SizeOf += owner.ParentState.RuntimeSizes.SizeOf;
				RuntimeSize += owner.ParentState.RuntimeSizes.RuntimeSize;
			}
		}
	};

	sealed class DefinitionRuntimeAnalFields
	{
		/// <summary>
		/// Fields that exist only in the source tag
		/// </summary>
		internal List<int> SourceFields = null;
		/// <summary>
		/// Fields that exist only in the cache tag
		/// </summary>
		internal List<int> CacheFields = null;
		/// <summary>
		/// Fields that exist only in the xbox tag
		/// </summary>
		internal List<int> XboxFields = null;
		/// <summary>
		/// Fields that exist only in the pc tag
		/// </summary>
		internal List<int> PcFields = null;

		internal void ProcessAnalFields(DefinitionState owner)
		{
			if (SourceFields != null ||
				CacheFields != null ||
				XboxFields != null ||
				PcFields != null)
				owner.RuntimeFieldCounts.SetHasAnalFields();
		}
	};
	#endregion

	/// <summary>
	/// Describes the runtime and meta data state of a single definition
	/// </summary>
	public sealed partial class DefinitionState
	{
		#region Engine
		BlamVersion engine;
		/// <summary>
		/// Engine this definition belongs to
		/// </summary>
		public BlamVersion Engine	{ get { return engine; } }
		#endregion

		#region Handle
		Blam.DatumIndex handle = Blam.DatumIndex.Null;
		/// <summary>
		/// Handle to a group tag if this is a tag group or tag struct definition
		/// </summary>
		public Blam.DatumIndex Handle
		{
			get { return handle; }
			internal set { handle = value; }
		}
		#endregion

		#region DefinitionType
		Type defType;
		/// <summary>
		/// Type information of the <see cref="Definition"/> this state data is for
		/// </summary>
		public Type DefinitionType { get { return defType; } }
		#endregion

		DefinitionState parentState;
		/// <summary>
		/// State of the parent (read: base) of this definition, if one exists
		/// </summary>
		public DefinitionState ParentState { get { return parentState; } }

		internal DefinitionRuntimeFieldCounts RuntimeFieldCounts;
		internal DefinitionRuntimeSizes RuntimeSizes;
		internal DefinitionRuntimeAnalFields RuntimeAnalFields = new DefinitionRuntimeAnalFields();


		#region Ctor
		/// <summary>
		/// Create state data from a <see cref="Definition"/> instance
		/// </summary>
		/// <param name="engine"></param>
		/// <param name="definition"></param>
		internal DefinitionState(BlamVersion engine, Type definition)
		{
			this.engine = engine;
			defType = definition;
			ProcessAttributes();
			FindNewInstanceCtor();
			FindNewInstanceVersionCtor();

			// TODO: for definitions with no explicit Guid in their DefinitionAttribute, switch 
			// on its associated game and assign it a Guid from the constants in Blam.BlamExtensions
		}
		bool isPostProcessed = false;
		/// <summary>
		/// Exposed as a hack for checking to see if a definition needs to be forced to postprocess. This 
		/// was done due to issues in Halo2 where a an older tag definition used a now deprecated structure.
		/// Since none of the latest definitions referenced it in the default ctor, it never got postprocessed.
		/// </summary>
		internal bool IsPostProcessed { get { return isPostProcessed; } }
		/// <summary>
		/// Build the finishing touches of the state data (eg, <see cref="DefinitionState.TypeCodes"/>)
		/// </summary>
		internal void PostProcess()
		{
			if (!isPostProcessed)
			{
				Definition inst = (Definition)defaultCtor.Invoke(null);
				//ProcessFieldAttributes(inst);
				RuntimeSizes.ProcessSizeInformation(this);
				ProcessTypes(inst);
				RuntimeAnalFields.ProcessAnalFields(this);

				isPostProcessed = true;
			}
		}
		#endregion

		/// <summary>
		/// Returns the type info string of the definition this describes
		/// </summary>
		public override string ToString() { return defType.ToString(); }

		#region Anal field code
		/// <summary>
		/// Add a field to this definition that only gets I\O'd in
		/// source files
		/// </summary>
		/// <param name="owner"></param>
		/// <param name="item"></param>
		public void AddSource(Definition owner, Field item)
		{
			if (RuntimeAnalFields.SourceFields == null)
				RuntimeAnalFields.SourceFields = new List<int>();
			RuntimeAnalFields.SourceFields.Add(owner.Count);
		}

		/// <summary>
		/// Add a field to this definition that only gets I\O'd in
		/// cache files
		/// </summary>
		/// <param name="owner"></param>
		/// <param name="item"></param>
		public void AddCache(Definition owner, Field item)
		{
			if (RuntimeAnalFields.CacheFields == null)
				RuntimeAnalFields.CacheFields = new List<int>();
			RuntimeAnalFields.CacheFields.Add(owner.Count);
		}

		/// <summary>
		/// Add a field to this definition that only gets I\O'd in
		/// xbox tags
		/// </summary>
		/// <param name="owner"></param>
		/// <param name="item"></param>
		public void AddXbox(Definition owner, Field item)
		{
			if (RuntimeAnalFields.XboxFields == null)
				RuntimeAnalFields.XboxFields = new List<int>();
			RuntimeAnalFields.XboxFields.Add(owner.Count);
		}

		/// <summary>
		/// Add a field to this definition that only gets I\O'd in
		/// pc tags
		/// </summary>
		/// <param name="owner"></param>
		/// <param name="item"></param>
		public void AddPc(Definition owner, Field item)
		{
			if (RuntimeAnalFields.PcFields == null)
				RuntimeAnalFields.PcFields = new List<int>();
			RuntimeAnalFields.PcFields.Add(owner.Count);
		}
		#endregion

		#region TypeCodes
		List<FieldType> typeCodes = new List<FieldType>();
		/// <summary>
		/// Gets a list of all the field types used in this definition
		/// </summary>
		public List<FieldType> TypeCodes { get { return typeCodes; } }

		#region Code Hashes
		int typeCodesHash = -1;
		/// <summary>
		/// Gets the hash value of the type code list
		/// </summary>
		public int TypeCodesHash { get { return typeCodesHash; } }

		int typeCodesEditorHash = -1;
		/// <summary>
		/// Gets the hash value of the type codes which are viewable in the editor
		/// </summary>
		public int TypeCodesEditorHash { get { return typeCodesEditorHash; } }
		#endregion

		/// <summary>
		/// Build the <see cref="TypeCodes"/> field with ALL the fields contained by the <see cref="Definition"/> object
		/// </summary>
		/// <param name="inst"></param>
		/// <remarks>Also calcuates useless pad size</remarks>
		void ProcessTypes(Definition inst)
		{
			if (typeCodes.Count != 0) return; // cases for structs which we force ProcessTypes on

#if false // TODO: For validation of structure sizes
			totalSize = 0;
#endif
			int field_index = 0, field_index_editor = 0;
			// NOTE: uses a rolling "byte" index for shifting the FieldType values by 8-bits so 
			// we use the entire 16-bit (not 32 since we use the other 16 for field count)
			// value when building the hash
			bool field_type_shift = false;
			typeCodesHash = 0;
			typeCodesEditorHash = 0;

			// HACK: If there are any platform specific fields, this will NOT take that fact 
			// into account. However, since cache files haven't (thus far) contained debug data 
			// such as field-set headers or anything like that, we shouldn't have to worry about 
			// having an explicitly matching runtimeSize or what have you
			int old_string_id_count = 0;
			foreach (Field f in inst)
			{
				typeCodes.Add(f.FieldType);

				int field_hash = (int)f.FieldType;
				if (field_type_shift = !field_type_shift)
					field_hash <<= 8;
				typeCodesHash ^= field_hash;
				if (!f.FieldType.IsNonEditorField())
				{
					typeCodesEditorHash ^= field_hash; // still has shifted value
					field_index_editor++;
				}

				// force the struct to process its types so we can get a valid runtime size
				if (f.FieldType == FieldType.Block)
				{
					DefinitionState state = (f as IBlock).GetDefinition();
					state.PostProcess();
				}
				else if (f.FieldType == FieldType.Struct || f.FieldType == FieldType.StructReference)
				{
					DefinitionState state = (f as IStruct).GetDefinition();
					state.PostProcess();
				}

				RuntimeFieldCounts.ProcessField(engine, this, f, ref old_string_id_count);

#if false // TODO: For validation of structure sizes
				totalSize += FieldUtil.Sizeof(f, engine, false, 0);
#endif

				field_index++;
			}

			RuntimeFieldCounts.ProcessTypesFinishCalculations(engine, this, old_string_id_count);

			typeCodesHash <<= 16;
			typeCodesHash |= field_index;
			typeCodesEditorHash <<= 16;
			typeCodesEditorHash |= field_index_editor;

#if false // TODO: For validation of structure sizes
			if (attribute.SizeOf != -1 && totalSize != -1)
				Debug.Warn.If(totalSize == attribute.SizeOf, "Inconsistent size: {0,5} !={1,5}{2}\t{3}", attribute.SizeOf, totalSize,
					SourceFields != null || CacheFields != null || XboxFields != null || PcFields != null ? "\tSPEC FIELDS" : "",
					defType.FullName);

//			Debug.Warn.If(inst.Capacity == inst.Count, "Inconsistent field counts: {0}", defType.FullName);
#endif
		}

		/// <summary>
		/// Converts the <see cref="Definition"/> to a string
		/// </summary>
		/// <returns>A string with this <see cref="Definition"/>'s type in a string format, plus all the fields in this Definition's type name</returns>
		public string ToTypeCodesString()
		{
			System.Text.StringBuilder builder1 = new System.Text.StringBuilder();
			builder1.Append(defType);
			foreach (FieldType f in typeCodes)
				builder1.Append("," + f);

			return builder1.ToString();
		}
		#endregion

		#region FieldInfo
		List<FieldInfo> fieldInfo;
		/// <summary>
		/// Lists all the FieldInfo attributes applied to the <see cref="Definition"/>
		/// object's class fields used to create this state data
		/// </summary>
		public List<FieldInfo> FieldInfo { get { return fieldInfo; } }

		/// <summary>
		/// Retrieves the class members of the definition and puts them in <see cref="FieldInfo"/>
		/// </summary>
		/// <remarks>Returns an empty list if this object's type tree stops at <see cref="Definition"/></remarks>
		/// <returns>Returns a list of FieldInfo of all fields declared in the inheriting class</returns>
		void ProcessFieldInfo()
		{
			fieldInfo = new List<FieldInfo>();
			FieldInfo[] fields = defType.GetFields(BindingFlags.Instance | BindingFlags.NonPublic | BindingFlags.Public);
			foreach (FieldInfo f in fields)
				if (
					f.DeclaringType.IsSubclassOf(typeof(Definition)) &&
					f.GetType().IsSubclassOf(typeof(TagInterface.Field))
					)
					fieldInfo.Add(f);
		}
		#endregion

		#region Attribute
		DefinitionAttribute attribute;
		/// <summary>
		/// Definition attribute applied to the <c>Definition</c> class used to
		/// construct this state data
		/// </summary>
		public DefinitionAttribute Attribute { get { return attribute; } }

		/// <summary>
		/// Process a field in this definition so we can find its editor information (or lack thereof)
		/// </summary>
		/// <param name="inst">Used for dynamic calculating of field indexes</param>
		/// <param name="field">Field to inspect</param>
		/// <param name="field_index">Index in this definition to start looking for this field</param>
		void ProcessFieldAttributes(Definition inst, FieldInfo field, ref int field_index)
		{
			#region Filter out the non-editor fields
			if (
				field.GetType() == typeof(TagInterface.Pad) ||
				field.GetType() == typeof(TagInterface.UnknownPad) ||
				field.GetType() == typeof(TagInterface.UselessPad) ||
				field.GetType() == typeof(TagInterface.Skip) ||
				field.GetType() == typeof(TagInterface.VertexBuffer)
				)
			{
				field_index++;
				return;
			}
			#endregion

			#region Get the field atrribute
			object[] attrs = null;
			attrs = field.GetCustomAttributes(typeof(FieldAttribute), false);

			Debug.Assert.If(attrs.Length != 1, "{0} in {1} has no attribute.", field.Name, field.DeclaringType.FullName);
			Debug.Assert.If(attrs.Length == 1, "{0} in {1} has more than one attribute: {2}.", field.Name, field.DeclaringType.FullName, attrs.Length);

			// We should allow multiple attributes to be applied,
			// just the first one MUST be the one for the actual field.
			// This will help combat custom fields and explanations,
			// although the explanation fields will look funky since
			// they will be in the field before the field group begins =x
			FieldAttribute fa = attrs[0] as FieldAttribute;
			#endregion

			#region Definition index finding
			for (int x = field_index; x < inst.Count; x++)
			{
				if (inst[x].FieldType == fa.Type)
				{
					fa.DefinitionIndex = x;
					field_index = ++x;
					break;
				}
			}
			#endregion

			attribute.Fields.Add(fa, fa.DefinitionIndex);
		}

		/// <summary>
		/// Processes field attributes
		/// </summary>
		/// <param name="inst"></param>
		void ProcessFieldAttributes(Definition inst)
		{
			//if (true) return;
			ProcessFieldInfo();
			int field_index = 0;
			foreach (FieldInfo fi in fieldInfo) ProcessFieldAttributes(inst, fi, ref field_index);
		}

		/// <summary>
		/// Processes attributes applied to one of the inheriting classes of this class
		/// </summary>
		void ProcessAttributes()
		{
			#region Get definition attribute
			object[] attr = defType.GetCustomAttributes(typeof(DefinitionAttribute), false);
			if (attr.Length == 0) return;
			Debug.Warn.If(attr.Length == 1, "{0}, {1}", defType.Name, attr);

			DefinitionAttribute da = attr[0] as DefinitionAttribute;
			attribute = da;
			#endregion

			if (attribute is TagGroupAttribute)
			{
				var tga = attribute as TagGroupAttribute;

				if (tga.ParentType != null)
				{
					var dic = DefinitionStatePool.CollectionFromType(tga.ParentType);

					if (!dic.TryGetValue(tga.ParentType, out parentState))
						throw new KeyNotFoundException(string.Format(
							"There was a problem getting the parent ('{0}') definition's state. " + 
							"Did you initialize the parent TagGroup (in a TagGroupInit.cs) object first?",
							tga.ParentType));
				}
			}
		}
		#endregion

		#region VersionCtor
		ConstructorInfo defaultCtor = null;
		/// <summary>
		/// Constructor method used to create the default version of the definition
		/// </summary>
		internal ConstructorInfo DefaultCtor { get { return defaultCtor; } }

		/// <summary>
		/// Finds ctor used to create the default version of the definition
		/// </summary>
		void FindNewInstanceCtor() { defaultCtor = defType.GetConstructor(Type.EmptyTypes); }

		/// <summary>
		/// Attributes that are applied to <c>versionCtor</c>
		/// </summary>
		internal VersionCtorAttribute[] versionCtorAttributes = null;

		ConstructorInfo versionCtor = null;
		/// <summary>
		/// Constructor method used to create explicit versions of a definition
		/// </summary>
		internal ConstructorInfo VersionCtor { get { return versionCtor; } }

		/// <summary>
		/// Finds all the needed meta data needed to run explicit versioning
		/// on this definition
		/// </summary>
		void FindNewInstanceVersionCtor()
		{
			ConstructorInfo cinfo = versionCtor; // ctor to use for construction
			VersionCtorAttribute[] attrs = versionCtorAttributes; // ctor info to validate operation

			if (cinfo == null) // we've already found the version ctor, don't waste time with our hacks
			{
				ConstructorInfo[] ctors = defType.GetConstructors();
				ParameterInfo[] cparams;
				foreach (ConstructorInfo ctor in ctors) // try to find the fucking ctor
				{
					cparams = ctor.GetParameters(); // and here is where things get anal...
					if (cparams.Length == 2 &&
						cparams[0].ParameterType == typeof(int) &&// major
						cparams[1].ParameterType == typeof(int))	// minor
					{
						attrs = (VersionCtorAttribute[])ctor.GetCustomAttributes(typeof(VersionCtorAttribute), false);
						if (attrs.Length != 0) // versioning ctor MUST supply at LEAST ONE attribute. yah, rly.
						{
							cinfo = versionCtor = ctor;
							versionCtorAttributes = attrs;
							break;
						}
					}
				}
			}
		}

		/// <summary>
		/// Returns a string in the form of:
		/// "MAJ=[major] MIN=[minor]"
		/// </summary>
		/// <param name="versionCtorAttributeUsed">index of the version ctor used</param>
		/// <remarks>If the definition has no version ctor or the index is -1, <c>string.Empty</c> is returned</remarks>
		/// <returns></returns>
		internal string ToVersionString(int versionCtorAttributeUsed)
		{
			if (versionCtorAttributes == null || versionCtorAttributeUsed == -1) return string.Empty;
			return string.Format("MAJ={0} MIN={1}",
				versionCtorAttributes[versionCtorAttributeUsed].Major,
				versionCtorAttributes[versionCtorAttributeUsed].Minor);
		}

		/// <summary>
		/// Tries to find the ctor for a specific build of the blam engine
		/// </summary>
		/// <param name="engine"></param>
		/// <returns>null if there isn't a specific definition for <paramref name="engine"/></returns>
		internal VersionCtorAttribute VersionForEngine(BlamVersion engine)
		{
			if (versionCtorAttributes != null)
			{
				foreach (VersionCtorAttribute vctor in versionCtorAttributes)
					if (vctor.Engine == engine) return vctor;
			}
			return null;
		}

		/// <summary>
		/// Confirms whether or not major and minor are valid
		/// versions for this definition
		/// </summary>
		/// <param name="major">major version</param>
		/// <param name="minor">minor version</param>
		/// <returns></returns>
		internal bool VersionIsValid(int major, int minor)
		{
			if (versionCtorAttributes != null) // this solves IsDynamicDefinition cases too
			{
				foreach (VersionCtorAttribute vctor in versionCtorAttributes)
					if (vctor.Major == major && vctor.Minor == minor) return true;
			}
			return false;
		}

		/// <summary>
		/// Confirms whether or not major and minor are valid
		/// versions for this definition
		/// </summary>
		/// <param name="group_tag">verion's group tag index</param>
		/// <param name="major">major version</param>
		/// <param name="minor">minor version</param>
		/// <returns></returns>
		internal bool VersionIsValid(int group_tag, int major, int minor)
		{
			if (versionCtorAttributes != null) // this solves IsDynamicDefinition cases too
			{
				foreach (VersionCtorAttribute vctor in versionCtorAttributes)
					if (vctor.GroupTag == group_tag && vctor.Major == major && vctor.Minor == minor) return true;
			}
			return false;
		}
		#endregion

		#region Construction
		/// <summary>
		/// Creates a new Definition
		/// </summary>
		/// <param name="parent">Object to use as the parent of the new instance</param>
		/// <returns></returns>
		public Definition NewInstance(IStructureOwner parent)
		{
			Definition inst = null;

			inst = DefaultCtor.Invoke(null) as Definition;

			Debug.Assert.If(inst != null, "Failed to create new instance of {0}: {1}", defType.FullName, parent);

			inst.PostProcess(true);
			inst.SetOwnerObject(parent);

			return inst;
		}

		/// <summary>
		/// Creates a new Definition using explicit version info
		/// </summary>
		/// <param name="parent">Object to use as the parent of the new instance</param>
		/// <param name="major">Major version of the definition</param>
		/// <param name="minor">Minor version of the definition</param>
		/// <returns>Null if <c>IsDynamicDefinition</c> is true or if no constructor (that can do the major\minor version) was found</returns>
		public Definition NewInstance(IStructureOwner parent, int major, int minor)
		{
			if (versionCtor == null) return null;

			Definition inst = null; // returned object
			ConstructorInfo cinfo = versionCtor; // ctor to use for construction
			VersionCtorAttribute[] attrs = versionCtorAttributes; // ctor info to validate operation

			int index = 0;
			foreach (VersionCtorAttribute vctor in attrs)
			{
				if (vctor.Major == major && vctor.Minor == minor) // make sure its a legal version
				{
					inst = cinfo.Invoke(new object[] { major, minor }) as Definition; // call the versioning ctor
					inst.PostProcess(true); // post-process
					inst.versionCtorAttributeUsed = index; // inform the new born of their heritage
					break;
				}
				index++;
			}

			Debug.Assert.If(inst != null, "Failed to create new instance of {0}: {1}\t[{2}:{3}]", defType.FullName, parent, major, minor);
			inst.SetOwnerObject(parent);

			return inst; // by the time we get here, it will either contain lullypops or null
		}
		#endregion


		#region WriteFieldData
		/// <summary>
		/// Writes to a text file, the field types and their offsets of this <see cref="Definition"/>,
		/// among other things
		/// </summary>
		/// <remarks>
		/// If it finds a nested <see cref="Definition"/> (due to a <see cref="Block{T}"/> or <see cref="Struct{T}"/>) it will
		/// call that <see cref="DefinitionState"/>'s <see cref="DefinitionState.WriteFieldData"/> method
		/// </remarks>
		/// <param name="stream">Text stream to write to</param>
		/// <param name="game">Which game platform the field data is for</param>
		/// <param name="cache">If the field data should be treated as if it was from a cache file, instead of a tag file</param>
		/// <returns>The total size of this <see cref="Definition"/> PLUS any other <see cref="Definition"/>s in it</returns>
		public int WriteFieldData(System.IO.StreamWriter stream, BlamVersion game, bool cache)
		{
			int total_size = 0;
			stream.Write("For: {0}{1} ", game, cache ? "'s cache file" : "");
			WriteFieldDataRecursive(stream, 0, game, cache, ref total_size);
			stream.WriteLine("Total Size: {0}", total_size.ToString("X"));
			stream.Flush();
			return total_size;
		}

		void WriteFieldDataRecursive(System.IO.StreamWriter stream, int padding, BlamVersion game, bool cache, ref int offset)
		{
			WriteFieldDataRecursive(stream, padding, NewInstance(null), game, cache, ref offset);
		}

		void WriteFieldDataRecursive(System.IO.StreamWriter stream, int padding, Definition inst, BlamVersion game, bool cache, ref int offset)
		{
			string padstring = string.Empty;
			if (padding != 0)
			{
				System.Text.StringBuilder pad = new System.Text.StringBuilder(padding);
				for (int x = 0; x < padding; x++) pad.Append('\t');
				padstring = pad.ToString();
			}

			stream.WriteLine("{0}{1}", padstring, inst.GetType());

			foreach (Field f in inst)
			{
				if (f.FieldType == FieldType.Struct)
					(f as IStruct).GetDefinition().WriteFieldDataRecursive(stream, padding + 1, game, cache, ref offset);
				else if (f.FieldType == FieldType.Block)
				{
					int total_size = 0;
					(f as IBlock).GetDefinition().WriteFieldDataRecursive(stream, padding + 1, game, cache, ref total_size);
					stream.WriteLine("{0}\tTotal Size: 0x{1}", padstring, total_size.ToString("X"));
				}
				else
				{
					if (f.FieldType != FieldType.Pad || f.FieldType != FieldType.UselessPad)
						stream.WriteLine("{0}{1} @ 0x{2}", padstring, f.FieldType, offset.ToString("X"));
					else if (
						f.FieldType == FieldType.UselessPad && 
						(game == BlamVersion.Halo2_Alpha || game == BlamVersion.Halo2_Epsilon))
						stream.WriteLine("{0}{1} @ 0x{2} ({3} bytes)", padstring, f.FieldType, offset.ToString("X"), ((int)f.FieldValue).ToString());
					else if (f.FieldType == FieldType.Pad)
						stream.WriteLine("{0}{1} @ 0x{2} ({3} bytes)", padstring, f.FieldType, offset.ToString("X"), ((int)f.FieldValue).ToString());
					else																		throw new Debug.Exceptions.UnreachableException(f.FieldType);
					offset += FieldUtil.Sizeof(f, game, cache, 0);
				}
			}

			stream.WriteLine();
			stream.Flush();
		}
		#endregion
	};

	/// <summary>
	/// Internal helper class for keeping track of all <see cref="Definition"/> classes
	/// </summary>
	static class DefinitionStatePool
	{
		static Dictionary<Type, DefinitionState>
			Global = new Dictionary<Type, DefinitionState>(8),			//   1 - 2010-02-05
			Halo1 = new Dictionary<Type, DefinitionState>(350),			// 336 - 2010-02-05
			Halo2 = new Dictionary<Type, DefinitionState>(1000),		// 973 - 2010-02-05
			Halo3 = new Dictionary<Type, DefinitionState>(/*512*/),		//  86 - 2010-02-05
			Stubbs = new Dictionary<Type, DefinitionState>(8),			//   3 - 2010-02-05
			HaloOdst = new Dictionary<Type, DefinitionState>(/*512*/),
			HaloReach = new Dictionary<Type, DefinitionState>(/*512*/),
			Halo4 = new Dictionary<Type, DefinitionState>(/*512*/)
			;
		const string kNsHalo1		= "BlamLib.Blam.Halo1";
		const string kNsHalo2		= "BlamLib.Blam.Halo2";
		const string kNsHalo3		= "BlamLib.Blam.Halo3";
		const string kNsStubbs		= "BlamLib.Blam.Stubbs";
		const string kNsHaloOdst	= "BlamLib.Blam.HaloOdst";
		const string kNsHaloReach	= "BlamLib.Blam.HaloReach";
		const string kNsHalo4		= "BlamLib.Blam.Halo4";

		internal static void PostProcess()
		{
			// We perform a copy since PostProcess will call setup child Block\Struct fields and 
			// their states which will end up calling [Add] which would invalidate the Enumerator

			var temp = new Dictionary<Type, DefinitionState>(Global);
			foreach (var s in temp.Values)		s.PostProcess();

			temp = new Dictionary<Type, DefinitionState>(Halo1);
			foreach (var s in temp.Values)		s.PostProcess();

			temp = new Dictionary<Type, DefinitionState>(Halo2);
			foreach (var s in temp.Values)		s.PostProcess();

			temp = new Dictionary<Type, DefinitionState>(Halo3);
			foreach (var s in temp.Values)		s.PostProcess();

			temp = new Dictionary<Type, DefinitionState>(Stubbs);
			foreach (var s in temp.Values)		s.PostProcess();

			temp = new Dictionary<Type, DefinitionState>(HaloOdst);
			foreach (var s in temp.Values)		s.PostProcess();

			temp = new Dictionary<Type, DefinitionState>(HaloReach);
			foreach (var s in temp.Values)		s.PostProcess();

			temp = new Dictionary<Type, DefinitionState>(Halo4);
			foreach (var s in temp.Values)		s.PostProcess();
		}

		/// <summary>
		/// Builds or finds the state information for a definition type
		/// </summary>
		/// <param name="inst">Target definition</param>
		/// <returns>State data for <paramref name="inst"/></returns>
		public static DefinitionState Add(Definition inst)
		{
			Type t = inst.GetType();
			BlamVersion engine;
			Dictionary<Type, DefinitionState> coll = CollectionFromType(t, out engine);
			DefinitionState state = null;
			if (!coll.TryGetValue(t, out state))
			{
				state = new DefinitionState(engine, t);
				coll.Add(t, state);
			}

			return state;
		}

		#region Util
		[System.Diagnostics.Conditional("DEBUG")]
		public static void Dump(BlamVersion engine, System.IO.StreamWriter s)
		{
			Dictionary<Type, DefinitionState> pool = DefinitionStatePool.CollectionFromEngine(engine);

			foreach(var kv in pool)
				s.WriteLine("{0,5}{1,5}\t{2}", kv.Value.TypeCodes.Count, kv.Value.RuntimeSizes.SizeOf, kv.Key.Name);
		}

		/// <summary>
		/// Figures out the engine which <paramref name="t"/> belongs to
		/// </summary>
		/// <param name="t"></param>
		/// <returns></returns>
		public static BlamVersion EngineFromType(Type t)
		{
			if		(t.Namespace.StartsWith(kNsHalo1))		return BlamVersion.Halo1;
			else if (t.Namespace.StartsWith(kNsHalo2))		return BlamVersion.Halo2;
			else if (t.Namespace.StartsWith(kNsHalo3))		return BlamVersion.Halo3;
			else if (t.Namespace.StartsWith(kNsStubbs))		return BlamVersion.Stubbs;
			else if (t.Namespace.StartsWith(kNsHaloOdst))	return BlamVersion.HaloOdst;
			else if (t.Namespace.StartsWith(kNsHaloReach))	return BlamVersion.HaloReach;
			else if (t.Namespace.StartsWith(kNsHalo4))		return BlamVersion.Halo4;
			else											return BlamVersion.Unknown;
		}

		/// <summary>
		/// Figures out the collection to use based on the location of <paramref name="t"/>
		/// </summary>
		/// <param name="t"></param>
		/// <returns></returns>
		internal static Dictionary<Type, DefinitionState> CollectionFromType(Type t)
		{
			if		(t.Namespace.StartsWith(kNsHalo1))		return Halo1;
			else if (t.Namespace.StartsWith(kNsHalo2))		return Halo2;
			else if (t.Namespace.StartsWith(kNsHalo3))		return Halo3;
			else if (t.Namespace.StartsWith(kNsStubbs))		return Stubbs;
			else if (t.Namespace.StartsWith(kNsHaloOdst))	return HaloOdst;
			else if (t.Namespace.StartsWith(kNsHaloReach))	return HaloReach;
			else if (t.Namespace.StartsWith(kNsHalo4))		return Halo4;
			else											return Global;
		}

		/// <summary>
		/// Figures out the collection to use based on the location of <paramref name="t"/>
		/// </summary>
		/// <param name="t"></param>
		/// <param name="engine"></param>
		/// <returns></returns>
		static Dictionary<Type, DefinitionState> CollectionFromType(Type t, out BlamVersion engine)
		{
			if		(t.Namespace.StartsWith(kNsHalo1))		{ engine = BlamVersion.Halo1;		return Halo1; }
			else if (t.Namespace.StartsWith(kNsHalo2))		{ engine = BlamVersion.Halo2;		return Halo2; }
			else if (t.Namespace.StartsWith(kNsHalo3))		{ engine = BlamVersion.Halo3;		return Halo3; }
			else if (t.Namespace.StartsWith(kNsStubbs))		{ engine = BlamVersion.Stubbs;		return Stubbs; }
			else if (t.Namespace.StartsWith(kNsHaloOdst))	{ engine = BlamVersion.HaloOdst;	return HaloOdst; }
			else if (t.Namespace.StartsWith(kNsHaloReach))	{ engine = BlamVersion.HaloReach;	return HaloReach; }
			else if (t.Namespace.StartsWith(kNsHalo4))		{ engine = BlamVersion.Halo4;		return Halo4; }
			else											{ engine = BlamVersion.Unknown;		return Global; }
		}

		/// <summary>
		/// Returns the state collection pool for the associated engine build
		/// </summary>
		/// <param name="engine"></param>
		/// <remarks>Use <see cref="BlamVersion.Unknown"/> to retrieve the global pool</remarks>
		/// <returns></returns>
		public static Dictionary<Type, DefinitionState> CollectionFromEngine(BlamVersion engine)
		{
			if		((engine & BlamVersion.Halo1) != 0)		return Halo1;
			else if ((engine & BlamVersion.Halo2) != 0)		return Halo2;
			else if ((engine & BlamVersion.Halo3) != 0)		return Halo3;
			else if ((engine & BlamVersion.Stubbs) != 0)	return Stubbs;
			else if ((engine & BlamVersion.HaloOdst) != 0)	return HaloOdst;
			else if ((engine & BlamVersion.HaloReach) != 0)	return HaloReach;
			else if ((engine & BlamVersion.Halo4) != 0)		return Halo4;
			else											return Global;
		}
		#endregion
	};

	/// <summary>
	/// Class that is capable of describing a structure of bytes, composed of various data,
	/// using <see cref="Field"/> objects.
	/// </summary>
	/// <remarks>
	/// <para>The structure is assumed to be byte aligned, and the structure's fields are assumed 
	/// to appear in a linear order as they do in the <see cref="Definition"/>'s list of <see cref="Field"/>s.</para>
	/// <para>The <see cref="Definition"/> object only operates with the <see cref="Field"/> objects added to it using
	/// its <c>Add</c> method override (from the <c>List</c> class)</para>
	/// </remarks>
	/// <see cref="BlamLib.TagInterface.Struct{T}"/>
	public class Definition : List<Field>, IIOProcessable, Blam.Cache.ICacheObject, IStructureOwner, ICloneable
	{
		#region Anal field code
		/// <summary>
		/// Is the field only for source tag variants?
		/// </summary>
		/// <param name="fi"></param>
		/// <returns></returns>
		/// <remarks>Do NOT call this code from versioning ctors unless you know what the FUCK you are doing!</remarks>
		protected bool IsSourceField(Field fi)
		{
			if (state.RuntimeAnalFields.SourceFields == null) return false;
			foreach (int i in state.RuntimeAnalFields.SourceFields)
				if (object.ReferenceEquals(this[i], fi)) return true;
			return false;
		}
		/// <summary>
		/// Is the field only for cache tag variants?
		/// </summary>
		/// <param name="fi"></param>
		/// <returns></returns>
		/// <remarks>Do NOT call this code from versioning ctors unless you know what the FUCK you are doing!</remarks>
		protected bool IsCacheField(Field fi)
		{
			if (state.RuntimeAnalFields.CacheFields == null) return false;
			foreach (int i in state.RuntimeAnalFields.CacheFields)
				if (object.ReferenceEquals(this[i], fi)) return true;
			return false;
		}
		/// <summary>
		/// Is the field only for xbox tag variants?
		/// </summary>
		/// <param name="fi"></param>
		/// <returns></returns>
		/// <remarks>Do NOT call this code from versioning ctors unless you know what the FUCK you are doing!</remarks>
		protected bool IsXboxField(Field fi)
		{
			if (state.RuntimeAnalFields.XboxFields == null) return false;
			foreach (int i in state.RuntimeAnalFields.XboxFields)
				if (object.ReferenceEquals(this[i], fi)) return true;
			return false;
		}
		/// <summary>
		/// Is the field only for pc tag variants?
		/// </summary>
		/// <param name="fi"></param>
		/// <returns></returns>
		/// <remarks>Do NOT call this code from versioning ctors unless you know what the FUCK you are doing!</remarks>
		protected bool IsPcField(Field fi)
		{
			if (state.RuntimeAnalFields.PcFields == null) return false;
			foreach (int i in state.RuntimeAnalFields.PcFields)
				if (object.ReferenceEquals(this[i], fi)) return true;
			return false;
		}
		#endregion

		#region Add
		/// <summary>
		/// Add a field to this Definition
		/// </summary>
		/// <param name="item"></param>
		public new void Add(Field item)
		{
			if (item == null)
				Debug.Assert.If(false, "Definition was passed a null field. {0}", this.GetType());
// 			if (item.FieldType == FieldType.UselessPad) return;

			item.OnAddToDefinition(this);
		}

		/// <summary>
		/// Add a list of fields to this definition
		/// </summary>
		/// <param name="items"></param>
		public void Add(params Field[] items)
		{
			foreach (Field f in items)
				this.Add(f);
		}

		/// <summary>
		/// Should be only called by Field.OnAddToDefinition
		/// </summary>
		/// <param name="item"></param>
		internal void AddField(Field item) { base.Add(item); }

		/// <summary>
		/// Add a field to this definition that only gets I\O'd in
		/// source files
		/// </summary>
		/// <param name="item"></param>
		public void AddSource(Field item)
		{
			state.AddSource(this, item);
			this.Add(item);
		}

		/// <summary>
		/// Add a field to this definition that only gets I\O'd in
		/// cache files
		/// </summary>
		/// <param name="item"></param>
		public void AddCache(Field item)
		{
			state.AddCache(this, item);
			this.Add(item);
		}

		/// <summary>
		/// Add a field to this definition that only gets I\O'd in
		/// xbox tags
		/// </summary>
		/// <param name="item"></param>
		public void AddXbox(Field item)
		{
			state.AddXbox(this, item);
			this.Add(item);
		}

		/// <summary>
		/// Add a field to this definition that only gets I\O'd in
		/// pc tags
		/// </summary>
		/// <param name="item"></param>
		public void AddPc(Field item)
		{
			state.AddPc(this, item);
			this.Add(item);
		}
		#endregion

		#region IStructureOwner
		Blam.DatumIndex IStructureOwner.OwnerId
		{
			get
			{
				if (owner != null) return owner.OwnerId;

				return Blam.DatumIndex.Null;
			}
		}

		Blam.DatumIndex IStructureOwner.TagIndex
		{
			get
			{
				if (owner != null) return owner.TagIndex;

				return Blam.DatumIndex.Null;
			}
		}

		protected IStructureOwner owner = null;
		IStructureOwner IStructureOwner.OwnerObject	{ get { return owner; } }

		internal void SetOwnerObject(IStructureOwner owner) { this.owner = owner; }
		#endregion

		#region State
		/// <summary>
		/// VERY HUGLY HACK!!!
		/// We need this because before hand we were doing all the state calculations
		/// before the constructors were ever finished executing so we would end up with
		/// data in the state that was incomplete.
		/// </summary>
		/// <remarks>
		/// Automatically finds and assigns runtime meta data needed
		/// to do advance stuff with this definition (like versioning)
		/// </remarks>
		internal void SetupState()
		{
			if (state == null) state = DefinitionStatePool.Add(this);
		}

		protected DefinitionState state;
		/// <summary>
		/// Information which needs only be generated once and every instance uses it
		/// </summary>
		public DefinitionState State { get { return state; } }
		#endregion

		#region Construction
		/// <summary>
		/// Creates a new Definition, with entirely no fields.
		/// Be sure to add them if you plan on using it!
		/// </summary>
		public Definition() : base() { SetupState(); }

		/// <summary>
		/// Creates a new Definition, with an explicit set number of field references preallocated
		/// to help with memory stress when doing extensive tag work
		/// </summary>
		/// <param name="field_count"></param>
		protected Definition(int field_count) : base(field_count) { SetupState(); }

		/// <summary>
		/// Creates a new Definition
		/// </summary>
		/// <param name="group"></param>
		/// <param name="g">Engine version this definition is for</param>
		/// <returns>Definition for something of <paramref name="g"/> engine version</returns>
		public static Definition FromFile(TagGroup group, BlamVersion g)
		{
			DefinitionFile df = new DefinitionFile(group, g);
			df.SetupParse();
			Definition d = df.Parse();
			df.Close();
			df = null;
			return d;
		}

		/// <summary>
		/// Performs a deep-copy of this Definition, so everything in this and 
		/// any data this object references is completely copied so something
		/// that modifies the result won't be modifying our data
		/// </summary>
		/// <returns></returns>
		public object Clone()
		{
			Definition def = (Definition)State.DefaultCtor.Invoke(null);

			for (int x = 0; x < base.Count; x++)
				this[x].CopyTo(def[x]);

			return def;
		}

		/// <summary>
		/// Performs a deep-copy of this Definition, so everything in this and 
		/// any data this object references is completely copied so something
		/// that modifies the result won't be modifying our data
		/// </summary>
		/// <param name="owner"></param>
		/// <returns></returns>
		public object Clone(IStructureOwner owner)
		{
			Definition def = (Definition)State.DefaultCtor.Invoke(null);

			for (int x = 0; x < base.Count; x++)
				this[x].CopyTo(def[x]);

			def.SetOwnerObject(owner);

			return def;
		}

		/// <summary>
		/// Creates a new Definition that has
		/// the exact field layout as this one by either
		/// calling the inheriting class's empty constructor
		/// or by creating a new Definition object and explicitly
		/// adding the field layout by enumerating through
		/// this Definition's fields
		/// </summary>
		/// <param name="owner">Object to use as the owner of the new instance</param>
		/// <returns>New Definition based off this one</returns>
		public Definition NewInstance(IStructureOwner owner)
		{
			Definition inst = (Definition)State.DefaultCtor.Invoke(null);
			inst.PostProcess(true);

			inst.SetOwnerObject(owner);

			return inst;
		}

		/// <summary>
		/// Creates a new Definition using explicit version info
		/// </summary>
		/// <param name="owner">Object to use as the owner of the new instance</param>
		/// <param name="major">Major version of the definition</param>
		/// <param name="minor">Minor version of the definition</param>
		/// <returns>Null if no constructor (that can do the major\minor version) was found</returns>
		public Definition NewInstance(IStructureOwner owner, int major, int minor)
		{
			return State.NewInstance(owner, major, minor);
		}

		/// <summary>
		/// If this object is already of type <typeparamref name="T"/> then
		/// it just returns this as <typeparamref name="T"/>.
		/// If this object isn't, it creates a new <typeparamref name="T"/>
		/// and sets all the result's fields to this fields' data
		/// </summary>
		/// <typeparam name="T">Definition class to convert to</typeparam>
		/// <returns></returns>
		public T ToDefinition<T>() where T : Definition, new()
		{
// 			if (!IsDynamicDefinition)
// 			{
				Debug.Assert.If(typeof(T) == this.GetType(), "Failed to cast definition");
				return this as T;
// 			}
// 			else
// 			{
// 				T def = new T();
// 				Debug.Assert.If(def.Count == this.Count, "Field count mismatch against types: {0} != {1}", GetType(), typeof(T));
// 
// 				int count = this.Count;
// 				for (int x = 0; x < count; x++)
// 				{
// 					// We warn because some fields can be set from others even if they're not the same types *exactly*...
// 					Debug.Warn.If(def[x].FieldType == this[x].FieldType, "Field type mismatch: {0} != {1}", def[x].FieldType, this[x].FieldType);
// 					def[x].FieldValue = this[x].FieldValue;
// 				}
// 				return def;
// 			}
		}
		#endregion

		/// <summary>
		/// Converts this <see cref="Definition"/> to a string
		/// </summary>
		/// <remarks>If this definition has no state data, will return <c>null</c></remarks>
		/// <returns>A string with this <see cref="Definition"/>'s type in a string format, plus all the fields in this Definition's type name</returns>
		public override string ToString() { if (state != null) return State.ToTypeCodesString(); return null; }

		#region Reflection
		#region VersionCtor
		/// <summary>
		/// Index of the attribute used to create and load this definition
		/// </summary>
		/// <remarks>Aids in the post-processing code for upgrading</remarks>
		internal int versionCtorAttributeUsed = -1; // NOTE: only made 'internal' for the Definition class
		/// <summary>
		/// Get the attribute used to create and load this definition
		/// </summary>
		protected VersionCtorAttribute VersionCtorAttributeUsed { get { return State.versionCtorAttributes[versionCtorAttributeUsed]; } }

		/// <summary>
		/// Tries to find the ctor for a specific build of the blam engine
		/// </summary>
		/// <param name="engine"></param>
		/// <returns>null if there isn't a specific definition for <paramref name="engine"/></returns>
		internal VersionCtorAttribute VersionForEngine(BlamVersion engine) { return State.VersionForEngine(engine); }

		/// <summary>
		/// Confirms whether or not major and minor are valid
		/// versions for this definition
		/// </summary>
		/// <param name="major">major version</param>
		/// <param name="minor">minor version</param>
		/// <returns></returns>
		internal bool VersionIsValid(int major, int minor) { return State.VersionIsValid(major, minor); }

		/// <summary>
		/// Confirms whether or not major and minor are valid
		/// versions for this definition
		/// </summary>
		/// <param name="group_tag">version's group tag index</param>
		/// <param name="major">major version</param>
		/// <param name="minor">minor version</param>
		/// <returns></returns>
		internal bool VersionIsValid(int group_tag, int major, int minor) { return State.VersionIsValid(group_tag, major, minor); }

		internal string ToVersionString() { return State.ToVersionString(versionCtorAttributeUsed); }
		#endregion

		#region Attribute
		/// <summary>
		/// Definition attribute applied to this <see cref="Definition"/> class
		/// </summary>
		/// <remarks>Uses the <see cref="Definition.State"/> member</remarks>
		public DefinitionAttribute Attribute { get { return State.Attribute; } }
		#endregion

		/// <summary>
		/// Retrieves the class members of this <see cref="Definition"/>
		/// </summary>
		/// <remarks>Returns an empty list if this object's type tree stops at <see cref="Definition"/></remarks>
		/// <returns>Returns a list of <see cref="FieldInfo"/> of all fields declared in the inheriting class</returns>
		public List<FieldInfo> GetFieldInfo() { return State.FieldInfo; }
		#endregion

		#region Implicit Versioning
		IEnumerable<Field> versionDetachedFields;
		internal bool VersionImplicitUpgradeIsActive { get { return versionDetachedFields != null; } }

		internal void VersionImplicitUpgradeBegin(int size_of, IO.ITagStream ts)
		{
			if (!State.ImplicitUpgradeDetachNewFields(out versionDetachedFields, this, size_of, ts))
				throw new Exceptions.InvalidVersion(ts,
					string.Format("implicit upgrade failed! source size: 0x{0}; in a: {1}",
					size_of.ToString("X"), State.DefinitionType));
		}

		internal void VersionImplicitUpgradeEnd()
		{
			if(versionDetachedFields != null)
				this.AddRange(versionDetachedFields);
			versionDetachedFields = null;
		}
		#endregion

		#region Util
		/// <summary>
		/// Calculate how much memory this definition consumes
		/// </summary>
		/// <param name="engine">Engine build we're calculating for</param>
		/// <param name="cache">Is this memory in a cache?</param>
		/// <returns>Size of memory in bytes</returns>
		/// <remarks>
		/// This operation will perform slower when <paramref name="cache"/> is false 
		/// due to the <see cref="FieldType.TagReference"/> case code
		/// 
		/// <b>NOTE:</b> Calculated value may not reflect the actual memory usage of the target 
		/// engine cache due optimizations in tag block and tag data fields, and in other areas
		/// </remarks>
		public int CalculateRuntimeSize(BlamVersion engine, bool cache)
		{
			int val = cache ? State.RuntimeSizes.RuntimeSize : State.Attribute.SizeOf;

			foreach (Field f in this)
			{
				switch(f.FieldType)
				{
					case FieldType.TagReference:
						Blam.DatumIndex ref_id;
						if (!cache && (ref_id = (f as TagReference).ReferenceId) != Blam.DatumIndex.Null)
							val += Program.GetTagIndex((this as IStructureOwner).OwnerId).References[ref_id].Length;
						break;

					case FieldType.Block:
						if (!cache && engine.UsesFieldSetVersionHeader())
							val += 16; // version header
						val += (f as IBlock).CalculateRuntimeSize(engine, cache);
						break;

					case FieldType.Data:
						Data df = f as Data;
						if (df.Value != null)
							val += df.Value.Length;
						break;

					case FieldType.Struct:
						if (!cache && engine.UsesFieldSetVersionHeader())
							val += 16; // version header
						val += (f as IStruct).CalculateRuntimeSize(engine, cache);
						break;
				}
			}

			return val;
		}
		#endregion

		#region Optional overrides
		/// <summary>
		/// Called whenever the <see cref="Definition"/> is about to be streamed, or when a new instance
		/// is created
		/// </summary>
		/// <param name="is_new">If the object was just recently created (due to a <see cref="Definition.NewInstance"/> invocation), then this will be true</param>
		/// <returns>True if success, false if otherwise</returns>
		/// <remarks>If the postprocessing failure is hella-serious, throw an exception in the code</remarks>
		internal virtual bool PostProcess(bool is_new) { return true; }

		/// <summary>
		/// Called whenever the <see cref="Definition"/> (as a tag group) is about to be streamed, 
		/// or when a new instance is created
		/// </summary>
		/// <param name="is_new"></param>
		/// <returns>True if success, false if otherwise</returns>
		/// <remarks>If the postprocessing failure is hella-serious, throw an exception in the code</remarks>
		internal virtual bool PostProcessGroup(bool is_new) { return true; }

		/// <summary>
		/// Called whenever the definition needs to be upgraded to the newest
		/// definition field set
		/// </summary>
		/// <returns>true if everything went ok during upgrade</returns>
		internal virtual bool Upgrade() { return true; }

		/// <summary>
		/// Called whenever the definition is being extracted from a cache file and should 
		/// be overloaded when data in the tag is postprocessed into something else for 
		/// cache builds, thus we reverse that processing
		/// </summary>
		/// <param name="c"></param>
		/// <returns>Wheather or not the reconstruction was successful or not</returns>
		internal virtual bool Reconstruct(BlamLib.Blam.CacheFile c) { return true; }
		#endregion

		#region I\O
		// NOTE: Currently, field checks with IsSource and IsCache are commented out as there are 
		// no definitions which require this kind of special handling (and probably never will)

		#region Cache
		/// <summary>
		/// Process the definition from a cache file
		/// </summary>
		/// <param name="c">Cache file</param>
		public void Read(BlamLib.Blam.CacheFile c)
		{
			Read(c, IOProcess.NoPointerData);		// first process the simple data (IE integers, real numbers, marked up atomic fields, etc)
			Read(c, IOProcess.BlockDataReflexive);	// then process the block data
		}

		/// <summary>
		/// Process the definition from a cache file
		/// </summary>
		/// <param name="c">Cache file</param>
		/// <param name="iop">Processing method</param>
		public void Read(BlamLib.Blam.CacheFile c, IOProcess iop)
		{
			bool is_xbox = c.EngineVersion.IsXbox();
			bool is_anal = Util.Flags.Test(state.RuntimeFieldCounts.Flags, DefinitionRuntimeFieldCounts.kFlagHasAnalFields);

			if (iop == IOProcess.NoPointerData) // first process the simple data (IE integers, real numbers, marked up atomic fields, etc)
			{
				foreach (Field fi in this)
				{
					if (is_anal)
					{
						if (IsXboxField(fi) && !is_xbox)continue;
						if (IsPcField(fi) && is_xbox)	continue;
						//if (IsSourceField(fi))			continue;
					}
					if		(fi.FieldType == FieldType.Struct)	(fi as IIOProcessable).Read(c, iop); // proxy what we're doing to the field
					else if (!Field.UsesRecursiveIo(fi))		fi.Read(c); // Process if this field doesn't contain any reference data (that points to the actual data)
					else										fi.ReadHeader(c); // Process the reference data for the "real" data of this field
				}
			}
			else if (iop == IOProcess.BlockDataReflexive) // then process the block data
			{
				foreach (Field fi in this)
				{
					if(is_anal)
					{
						if (IsXboxField(fi) && !is_xbox)continue;
						if (IsPcField(fi) && is_xbox)	continue;
						//if (IsSourceField(fi))			continue;
					}
					if		(Field.UsesRecursiveIo(fi))			fi.Read(c);
					else if (fi.FieldType == FieldType.Struct)	(fi as IIOProcessable).Read(c, iop); // proxy what we're doing to the field
				}

				if (!Reconstruct(c))
				{
					string name;

					var tag_index = (this as IStructureOwner).TagIndex;
					if (tag_index != Blam.DatumIndex.Null)
					{
						var tm = c.TagIndexManager[tag_index];
						name = string.Format("{0}.{1}", tm.Name, tm.GroupTag);
					}
					else
						name = string.Format("UNKNOWN ({0}.map)", c.Header.Name);

					Debug.Warn.If(false, "{1}: {0} failed to be reconstructed", this.GetType().FullName, name);
				}
			}
		}

		/// <summary>
		/// Process the definition to a cache file
		/// </summary>
		/// <param name="c">Cache file</param>
		public void Write(BlamLib.Blam.CacheFile c)
		{
			Write(c, IOProcess.NoPointerData);		// first process the simple data (IE integers, real numbers, marked up atomic fields, etc)
			Write(c, IOProcess.BlockDataReflexive);	// then process the block data
		}

		/// <summary>
		/// Process the definition to a cache file
		/// </summary>
		/// <remarks>
		/// When <paramref name="iop"/> equals <see cref="IOProcess.NoPointerData"/>, the definition's
		/// internal post-processing function is performed first, then the I\O is performed
		/// </remarks>
		/// <param name="c">Cache file</param>
		/// <param name="iop">Processing method</param>
		public void Write(BlamLib.Blam.CacheFile c, IOProcess iop)
		{
			bool is_xbox = c.EngineVersion.IsXbox();
			bool is_anal = Util.Flags.Test(state.RuntimeFieldCounts.Flags, DefinitionRuntimeFieldCounts.kFlagHasAnalFields);

			if (iop == IOProcess.NoPointerData)
			{
				// we shouldn't be doing this in a cache build
				//PostProcess(false); // NoPointerData is always the first to be called, so do the post-processing here

				foreach (Field fi in this)
				{
					if (is_anal)
					{
						if (IsXboxField(fi) && !is_xbox)continue;
						if (IsPcField(fi) && is_xbox)	continue;
						//if (IsSourceField(fi))			continue;
					}
					if		(fi.FieldType == FieldType.Struct)	(fi as IIOProcessable).Write(c, iop); // proxy what we're doing to the definition
					else if (!Field.UsesRecursiveIo(fi))		fi.Write(c); // Process if this field doesn't contain any reference data (that points to the actual data)
					else										fi.WriteHeader(c); // Process the reference data for the "real" data of this field
				}
			}
			else if (iop == IOProcess.BlockDataReflexive)
			{
				foreach (Field fi in this)
				{
					if (is_anal)
					{
						if (IsXboxField(fi) && !is_xbox)continue;
						if (IsPcField(fi) && is_xbox)	continue;
						//if (IsSourceField(fi))			continue;
					}
					if		(Field.UsesRecursiveIo(fi))			fi.Write(c);
					else if (fi.FieldType == FieldType.Struct)	(fi as IIOProcessable).Write(c, iop); // proxy what we're doing to the field
				}
			}
		}
		#endregion

		#region Tag
		/// <summary>
		/// Process the definition from a tag stream
		/// </summary>
		/// <param name="ts"></param>
		public void Read(IO.ITagStream ts)
		{
			Read(ts, IOProcess.NoPointerData);		// first process the simple data (IE integers, real numbers, marked up atomic fields, etc)
			Read(ts, IOProcess.BlockDataReflexive);	// then process the block data
		}

		/// <summary>
		/// Read the definition from a tag stream using the specified formatting option
		/// </summary>
		/// <param name="ts"></param>
		/// <param name="iop">formatting option</param>
		public void Read(IO.ITagStream ts, IOProcess iop)
		{
			bool is_anal = Util.Flags.Test(state.RuntimeFieldCounts.Flags, DefinitionRuntimeFieldCounts.kFlagHasAnalFields);
			bool is_xbox = ts.Engine.IsXbox();
			bool is_upgrading = ts.Flags.Test(IO.ITagStreamFlags.DefinitionWasUpgraded) || VersionImplicitUpgradeIsActive;

			if (iop == IOProcess.NoPointerData)
			{
				foreach (Field fi in this)
				{
					if (is_anal && !is_upgrading)
					{
						if (IsXboxField(fi) && !is_xbox)continue;
						if (IsPcField(fi) && is_xbox)	continue;
						//if (IsCacheField(fi))			continue;
					}
					if		(fi.FieldType == FieldType.Struct)	(fi as IIOProcessable).Read(ts, iop); // proxy what we're doing to the field
					else if (!Field.UsesRecursiveIo(fi))		fi.Read(ts);  // Process if this field doesn't contain any reference data (that points to the actual data)
					else										fi.ReadHeader(ts); // Process the reference data for the "real" data of this field
				}
			}
			else if (iop == IOProcess.BlockDataReflexive)
			{
				foreach (Field fi in this)
				{
					if (is_anal && !is_upgrading)
					{
						if (IsXboxField(fi) && !is_xbox)continue;
						if (IsPcField(fi) && is_xbox)	continue;
						//if (IsCacheField(fi))			continue;
					}
					if		(Field.UsesRecursiveIo(fi))	fi.Read(ts);
					else if	(fi.FieldType == FieldType.Struct)
					{
						fi.Read(ts); // read field set for halo 2 and beyond engines
						(fi as IIOProcessable).Read(ts, iop); // proxy what we're doing to the field
					}
				}
			}
		}

		/// <summary>
		/// Process the definition to a tag stream
		/// </summary>
		/// <param name="ts"></param>
		public void Write(IO.ITagStream ts)
		{
			Write(ts, IOProcess.NoPointerData);		// first process the simple data (IE integers, real numbers, marked up atomic fields, etc)
			Write(ts, IOProcess.BlockDataReflexive);	// then process the block data
		}

		/// <summary>
		/// Write data to a tag stream using the specified formatting option
		/// </summary>
		/// <param name="ts"></param>
		/// <param name="iop">formatting option</param>
		/// <remarks>
		/// When <paramref name="iop"/> equals <see cref="IOProcess.NoPointerData"/>, the definition's
		/// internal post-processing function is performed first, then the I\O is performed
		/// </remarks>
		public void Write(IO.ITagStream ts, IOProcess iop)
		{
			bool is_anal = Util.Flags.Test(state.RuntimeFieldCounts.Flags, DefinitionRuntimeFieldCounts.kFlagHasAnalFields);
			bool is_xbox = ts.Engine.IsXbox();

			if (iop == IOProcess.NoPointerData)
			{
				if (ts.Flags.Test(IO.ITagStreamFlags.DontPostprocess) && !PostProcess(false)) // NoPointerData is always the first to be called, so do the post-processing here
					Debug.Warn.If(false, "{0} failed to postprocess", this.GetType().FullName);

				foreach (Field fi in this)
				{
					if (is_anal)
					{
						if (IsXboxField(fi) && !is_xbox)continue;
						if (IsPcField(fi) && is_xbox)	continue;
						//if (IsCacheField(fi))			continue;
					}
					if		(fi.FieldType == FieldType.Struct)	(fi as IIOProcessable).Write(ts, iop); // proxy what we're doing to the field
					else if (!Field.UsesRecursiveIo(fi))		fi.Write(ts);  // Process if this field doesn't contain any reference data (that points to the actual data)
					else										fi.WriteHeader(ts); // Process the reference data for the "real" data of this field
				}
			}
			else if (iop == IOProcess.BlockDataReflexive)
			{
				foreach (Field fi in this)
				{
					if (is_anal)
					{
						if (IsXboxField(fi) && !is_xbox)continue;
						if (IsPcField(fi) && is_xbox)	continue;
						//if (IsCacheField(fi))			continue;
					}
					if		(Field.UsesRecursiveIo(fi))	fi.Write(ts);
					else if	(fi.FieldType == FieldType.Struct)
					{
						fi.Write(ts); // write field set for halo 2 and beyond engines
						(fi as IIOProcessable).Write(ts, iop); // proxy what we're doing to the field
					}
				}
			}
		}
		#endregion

		#region Generic Stream
		/// <summary>
		/// Process the definition from a stream
		/// </summary>
		/// <param name="s">Stream</param>
		public void Read(IO.EndianReader s)
		{
			Read(s, IOProcess.NoPointerData);		// first process the simple data (IE integers, real numbers, marked up atomic fields, etc)
			Read(s, IOProcess.BlockDataReflexive);	// then process the block data
		}

		/// <summary>
		/// Process the definition from a stream
		/// </summary>
		/// <param name="s">Stream</param>
		/// <param name="iop">Processing method</param>
		public void Read(IO.EndianReader s, IOProcess iop)
		{
			if (iop == IOProcess.NoPointerData)
			{
				foreach (Field fi in this)
				{
					if		(fi.FieldType == FieldType.Struct)	(fi as IIOProcessable).Read(s, iop); // proxy what we're doing to the field
					else if (!Field.UsesRecursiveIo(fi))		fi.Read(s);  // Process if this field doesn't contain any reference data (that points to the actual data)
					else										fi.ReadHeader(s); // Process the reference data for the "real" data of this field
				}
			}
			else if (iop == IOProcess.BlockDataReflexive)
			{
				foreach (Field fi in this)
				{
					if		(Field.UsesRecursiveIo(fi))			fi.Read(s);
					else if	(fi.FieldType == FieldType.Struct)	(fi as IIOProcessable).Read(s, iop); // proxy what we're doing to the field
				}
			}
		}

		/// <summary>
		/// Process the definition to a stream
		/// </summary>
		/// <param name="s">Stream</param>
		public void Write(IO.EndianWriter s)
		{
			Write(s, IOProcess.NoPointerData);		// first process the simple data (IE integers, real numbers, marked up atomic fields, etc)
			Write(s, IOProcess.BlockDataReflexive);	// then process the block data
		}

		/// <summary>
		/// Process the definition to a stream
		/// </summary>
		/// <remarks>
		/// When <paramref name="iop"/> equals <see cref="IOProcess.NoPointerData"/>, the definition's
		/// internal post-processing function is performed first, then the I\O is performed
		/// </remarks>
		/// <param name="s">Stream</param>
		/// <param name="iop">Processing method</param>
		public void Write(IO.EndianWriter s, IOProcess iop)
		{
			if (iop == IOProcess.NoPointerData)
			{
				foreach (Field fi in this)
				{
					if		(fi.FieldType == FieldType.Struct)	(fi as IIOProcessable).Write(s, iop); // proxy what we're doing to the field
					else if (!Field.UsesRecursiveIo(fi))		fi.Write(s);  // Process if this field doesn't contain any reference data (that points to the actual data)
					else										fi.WriteHeader(s); // Process the reference data for the "real" data of this field
				}
			}
			else if (iop == IOProcess.BlockDataReflexive)
			{
				foreach (Field fi in this)
				{
					if		(Field.UsesRecursiveIo(fi))			fi.Write(s);
					else if	(fi.FieldType == FieldType.Struct)	(fi as IIOProcessable).Write(s, iop); // proxy what we're doing to the field
				}
			}
		}
		#endregion
		#endregion

		#region ICacheObject Members
		/// <summary>
		/// Prepare the definition for being streamed to a cache
		/// </summary>
		/// <param name="owner">cache builder interface</param>
		public virtual void PreProcess(BlamLib.Blam.Cache.BuilderBase owner) {}
		/// <summary>
		/// Stream the definition to a cache being built
		/// </summary>
		/// <param name="owner">cache builder interface</param>
		/// <returns>true if everything was successful</returns>
		public virtual bool Build(BlamLib.Blam.Cache.BuilderBase owner)
		{
			Write(owner.CurrentStream, IOProcess.NoPointerData);
			Write(owner.CurrentStream, IOProcess.BlockDataReflexive);

			return true;
		}
		/// <summary>
		/// Post process build events for the definition when everything has been streamed to the cache
		/// </summary>
		/// <param name="owner">cache builder interface</param>
		public virtual void PostProcess(BlamLib.Blam.Cache.BuilderBase owner) { }
		#endregion
	};

	/// <summary>
	/// Utility class for blocks with only one field. Should only be used in Halo 1 and Stubbs definitions.
	/// </summary>
	/// <typeparam name="FieldType">Type of field in the block</typeparam>
	public abstract class field_block<FieldType> : Definition where FieldType : Field, new()
	{
		#region Tag Reference Hacks
		// little hackity hack hack to setup the tag ref's owner
		void Add(TagReference tagref)
		{
			if (tagref == null)
				Debug.Assert.If(false, "Definition was passed a null field. {0}", this.GetType());

			tagref.Owner = this;
			tagref.OnAddToDefinition(this);
		}

		/// <summary>
		/// Reset <see cref="Value"/>'s group tag definition to be something else
		/// </summary>
		/// <param name="group"></param>
		/// <remarks><see cref="Value"/> must be a <see cref="TagReference"/></remarks>
		public void ResetReferenceGroupTag(TagGroup group)
		{
			if (Value.FieldType == BlamLib.TagInterface.FieldType.TagReference)
				(Value as TagReference).ResetGroupTag(group);
		}
		#endregion

		/// <summary>
		/// Field data for this block
		/// </summary>
		public FieldType Value;

		public field_block() : base(1) { Add(Value = new FieldType()); }

		public override string ToString()
		{
			return Value.ToString();
		}
	};

	/// <summary>
	/// Empty tag block of doom
	/// </summary>
	[Definition(1, 0)]
	public sealed class g_null_block : Definition { public g_null_block() : base(1) { Add(new EditorField()); } };
}