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
	public enum VersioningFailureReason
	{
		None,

		VersionIdNotSupported,
		VersionIdIsNewer,
		VersionIdMismatch,

		GroupTagNotFound,
		GroupTagMismatch,

		CountMismatch,
	};

	internal interface IFieldSetVersioning
	{
		VersioningFailureReason FailureReason { get; }

		bool NeedsUpgrading { get; }

		bool UseImplicitUpgrading { get; }

		void GetUpgradeParameters(out int index, out int size);
	};

	partial class DefinitionState
	{
		#region Implicit upgrades
		// TODO: Implicit versioning upgrades doesn't work for cases where a tag_struct field was inserted into a 
		// field-set. This can be seen in Halo'2 'unit' tag, under unit_lipsync_scales_struct. That struct can either 
		// not exist or exist. I haven't RE'd the tool code yet to figure out how exactly this process is done by Bungie
		// Example tag: objects\weapons\fixed\h_turret_mp\h_turret_mp.vehicle (no unit_lipsync_scales_struct fields)

		int VersioningGetRealSizeOf(uint flags)
		{
			bool useless = Util.Flags.Test(flags, IO.ITagStreamFlags.Halo2OldFormat_UselessPadding);
			bool old_string_ids = Util.Flags.Test(flags, IO.ITagStreamFlags.Halo2OldFormat_StringId);

			return RuntimeSizes.SizeOf +
				(!useless ? 0 : RuntimeSizes.TotalUselessPad) +
				(!old_string_ids ? 0 : RuntimeSizes.TotalOldStringIdSize);
		}
		/// <summary>
		/// Can the field-set be implicitly upgraded by just not reading some (supposedly) newer fields?
		/// </summary>
		/// <param name="header"></param>
		/// <param name="io_flags"></param>
		/// <returns></returns>
		bool VersioningCanBeImplicitlyPerformed(tag_fieldset_header header, Util.Flags io_flags)
		{
			int size_of = VersioningGetRealSizeOf(io_flags);

			return header.Index == attribute.Version && size_of > header.Size;
		}

		// <SizeOf, FieldIndex>
		Dictionary<int, int> implicitUpgradeFieldIndexes = new Dictionary<int, int>();
		int ImplicitUpgradeCalculateFieldStartIndex(Definition def, int size_of, BlamVersion engine, uint flags)
		{
			int index;
			if (!implicitUpgradeFieldIndexes.TryGetValue(size_of, out index))
			{
				int current_size_of = VersioningGetRealSizeOf(flags);
				for (int x = def.Count - 1; x >= 0; x--)
				{
					var f = def[x];

					//if(f.FieldType != FieldType.UselessPad)
						current_size_of -= FieldUtil.Sizeof(f, engine, false, flags);

					if (current_size_of == size_of)
					{
						implicitUpgradeFieldIndexes.Add(size_of, index = x);
						break;
					}
					else if (current_size_of < size_of) // field layouts don't match up
						return -1;
				}
			}

			return index;
		}

		internal bool ImplicitUpgradeDetachNewFields(out IEnumerable<Field> detached_fields, Definition def, int size_of, IO.ITagStream ts)
		{
			detached_fields = null;
			int start_index = ImplicitUpgradeCalculateFieldStartIndex(def, size_of, ts.Engine, ts.Flags);

			if (start_index == -1) return false;

			int count = def.Count - start_index;
			detached_fields = def.GetRange(start_index, count);
			def.RemoveRange(start_index, count);

			return true;
		}
		#endregion

		/// <summary>
		/// Is the field-set size (ignores version indices) equal to the latest definition's, taking 
		/// into account things like
		/// </summary>
		/// <param name="header"></param>
		/// <param name="io_flags"></param>
		/// <param name="expected_size"></param>
		/// <returns></returns>
		bool VersioningSizeIsValid(tag_fieldset_header header, Util.Flags io_flags, out int expected_size)
		{
			bool useless = io_flags.Test(IO.ITagStreamFlags.Halo2OldFormat_UselessPadding);
			bool old_string_ids = io_flags.Test(IO.ITagStreamFlags.Halo2OldFormat_StringId);

			int size_of = VersioningGetRealSizeOf(io_flags);
			expected_size = size_of;

			return header.Size == size_of;
		}

		struct tag_fieldset_header
		{
			public uint GroupTag;
			public int Index;
			public int Count;
			public int Size;

			#region IStreamable Members
			public void Read(BlamLib.IO.EndianReader s, IO.ITagStream ts)
			{
				bool upgrade = ts.Flags.Test(IO.ITagStreamFlags.Halo2OldFormat_Fieldset);

				GroupTag = s.ReadTagUInt();
				if(!upgrade)
				{
					Index = s.ReadInt32();
					Count = s.ReadInt32();
				}
				else
				{
					Index = s.ReadInt16();
					Count = s.ReadInt16();
				}
				Size = s.ReadInt32();

				++Index;
			}
			#endregion
		};

		struct FieldSetVersioning : IFieldSetVersioning
		{
			FieldType type;
			DefinitionState state;
			TagGroup headerGroupTag;

			VersioningFailureReason failureReason;
			public VersioningFailureReason FailureReason { get { return failureReason; } }

			#region Ctor
			public FieldSetVersioning(BlamVersion engine, FieldType container_type, DefinitionState state)
			{
				failureReason = VersioningFailureReason.None;
				relativeOffset = 0;
				expectedCount = 1;
				needsUpgrading = false;
				useImplicitUpgrading = false;
				header = new tag_fieldset_header();

				this.type = container_type;
				this.state = state;

				if(container_type != FieldType.Struct)
					this.headerGroupTag = Blam.MiscGroups.tbfd;
				else
				{
					var groups = engine.VersionToStructCollection();
					var sa = state.Attribute as StructAttribute;

					headerGroupTag = groups[sa.GroupIndex];
				}
			}
			public FieldSetVersioning(BlamVersion engine, FieldType container_type, DefinitionState state, TagGroup group_tag) 
				: this(engine, container_type, state)
			{
				this.headerGroupTag = group_tag;
			}

			uint relativeOffset;
			int expectedCount;
			public void Initialize(uint relative_offset, int expected_count)
			{
				relativeOffset = relative_offset;
				expectedCount = expected_count;
			}
			#endregion

			string GetTypeString()
			{
				switch(type)
				{
					case FieldType.Tag: return "Tag group";
					case FieldType.Block: return "Tag block";
					case FieldType.Struct: return "Inline struct";
					case FieldType.StructReference: return "Data reference";

					default: throw new Debug.Exceptions.UnreachableException(type);
				}
			}

			#region Upgrade util
			bool needsUpgrading;
			public bool NeedsUpgrading { get { return needsUpgrading; } }

			bool useImplicitUpgrading;
			public bool UseImplicitUpgrading { get { return useImplicitUpgrading; } }

			tag_fieldset_header header;
			public void GetUpgradeParameters(out int index, out int size)
			{
				index = header.Index;
				size = header.Size;
			}
			#endregion

			#region Verification
			void VerifyHeaderGroupTag(IO.ITagStream ts, Type type_of)
			{
				if (header.GroupTag != headerGroupTag.ID)
				{
					if(type == FieldType.Struct)
					{
						var groups = ts.Engine.VersionToStructCollection();
						int index = groups.FindGroupIndex(header.GroupTag);

						if(index == -1)
						{
							failureReason = VersioningFailureReason.GroupTagNotFound;

							Debug.LogFile.WriteLine("{5} field set signature failed (signature not found) \t@{0:X8} [{1} !{2}] in {3}\t {4}",
								relativeOffset, headerGroupTag.TagToString(), new string(TagGroup.FromUInt(header.GroupTag)),
								type_of, ts.GetExceptionDescription(),
								GetTypeString());
							throw new Exceptions.InvalidVersion(ts);
						}
					}

					failureReason = VersioningFailureReason.GroupTagMismatch;

					Debug.LogFile.WriteLine("{5} field set signature failed\t@{0:X8} [{1} !{2}] in {3}\t {4}",
						relativeOffset,
						headerGroupTag.TagToString(), new string(TagGroup.FromUInt(header.GroupTag)),
						type_of, ts.GetExceptionDescription(),
						GetTypeString());
					throw new Exceptions.InvalidVersion(ts);
				}
			}

			void VerifyHeaderCount(IO.ITagStream ts, Type type_of)
			{
				if (header.Count != expectedCount)
				{
					failureReason = VersioningFailureReason.CountMismatch;

					throw new Debug.ExceptionLog("{5} field set count mismatch \t@{0:X8} [{1} !{2}] in {3}\t {4}",
						relativeOffset, expectedCount, header.Count,
						type_of, ts.GetExceptionDescription(),
						GetTypeString());
				}
			}

			void VerifyHeaderVersionIds(IO.ITagStream ts, Type type_of)
			{
				if ((header.Index != state.Attribute.Version || header.Size != state.Attribute.SizeOf) &&
					 state.VersionIsValid(header.Index, header.Size))
				{
					needsUpgrading = true;
				}
				else
				{
					if (header.Index != state.Attribute.Version)
					{
						if (header.Index > state.Attribute.Version)
							failureReason = VersioningFailureReason.VersionIdIsNewer;
						else
							failureReason = VersioningFailureReason.VersionIdNotSupported;

						Debug.LogFile.WriteLine("{5} field set version mismatch\t@{0:X8} [{1} !{2}] in {3}\t {4}",
							relativeOffset, state.Attribute.Version, header.Index,
							type_of, ts.GetExceptionDescription(),
							GetTypeString());
						throw new Exceptions.InvalidVersion(ts, state.Attribute.Version, header.Index);
					}

					int expected_size;
					if (!state.VersioningSizeIsValid(header, ts.Flags, out expected_size))
					{
						if (state.VersioningCanBeImplicitlyPerformed(header, ts.Flags))
						{
							needsUpgrading = true;
							useImplicitUpgrading = true;
						}
						else
						{
							failureReason = VersioningFailureReason.VersionIdMismatch;

							Debug.LogFile.WriteLine("{5} field set sizeof mismatch \t@{0:X8} [{1} !{2}] in {3}\t {4}",
								relativeOffset, expected_size, header.Size,
								type_of, ts.GetExceptionDescription(),
								GetTypeString());
							throw new Exceptions.InvalidVersion(ts, state.Attribute.Version, expected_size, header.Index, header.Size);
						}
					}
				}
			}
			#endregion

			#region IO
			void ReadHeader(IO.EndianReader s, IO.ITagStream ts)
			{
				header = new tag_fieldset_header();
				header.Read(s, ts);
				Type type_of = state.DefinitionType;

				VerifyHeaderGroupTag(ts, type_of);

				VerifyHeaderCount(ts, type_of);

				VerifyHeaderVersionIds(ts, type_of);
			}

			public void Read(IO.ITagStream ts)
			{
				IO.EndianReader s = ts.GetInputStream();

				ReadHeader(s, ts);
			}

			void WriteHeader(IO.EndianWriter s, IO.ITagStream ts)
			{
				headerGroupTag.Write(s);
				s.Write((int)state.Attribute.Version - 1);
				s.Write(expectedCount);
				s.Write(state.RuntimeSizes.SizeOf); // sizeof a single block would go here but its not actually required
			}

			public void Write(IO.ITagStream ts)
			{
				IO.EndianWriter s = ts.GetOutputStream();

				WriteHeader(s, ts);
			}
			#endregion
		};

		#region FieldSet interface
		internal static bool FieldSetRequiresVersioningHeader(IO.ITagStream ts)
		{
			return ts.Engine.UsesFieldSetVersionHeader() && !ts.Flags.Test(IO.ITagStreamFlags.DontStreamFieldSetHeader);
		}

		internal IFieldSetVersioning FieldSetReadVersion(IO.ITagStream ts, FieldType container_type, uint relative_offset)
		{
			var fs = new FieldSetVersioning(ts.Engine, container_type, this);
			fs.Initialize(relative_offset, 1);

			fs.Read(ts);

			return fs;
		}

		internal IFieldSetVersioning FieldSetReadVersion(IO.ITagStream ts, FieldType container_type, uint relative_offset, int expected_count)
		{
			var fs = new FieldSetVersioning(ts.Engine, container_type, this);
			fs.Initialize(relative_offset, expected_count);

			fs.Read(ts);

			return fs;
		}

		internal void FieldSetWriteVersion(IO.ITagStream ts, FieldType container_type)
		{
			var fs = new FieldSetVersioning(ts.Engine, container_type, this);

			fs.Write(ts);
		}

		internal void FieldSetWriteVersion(IO.ITagStream ts, FieldType container_type, int count)
		{
			var fs = new FieldSetVersioning(ts.Engine, container_type, this);
			fs.Initialize(uint.MaxValue, count);

			fs.Write(ts);
		}
		#endregion
	};

	/// <summary>
	/// Versioning attribute that is applied to a <see cref="Definition"/>'s
	/// versioning constructor to denote what version upgrades are
	/// possible
	/// </summary>
	[AttributeUsage(AttributeTargets.Constructor, AllowMultiple=true)]
	public abstract class VersionCtorAttribute : Attribute
	{
		/// <summary>
		/// Index to a tag group collection for the struct's group tag
		/// </summary>
		public int GroupTag = -1;
		/// <summary>
		/// With Bungie tag files, this is the actual programmer specified version
		/// </summary>
		public int Major = -1;
		/// <summary>
		/// With Bungie tag files, this is the size of the data block
		/// </summary>
		public int Minor = -1;
		/// <summary>
		/// Which game engine this version is explicitly for
		/// </summary>
		/// <remarks>
		/// Useful for when adding support for halo 2 alpha or echo maps that were
		/// different from the halo 2 xbox version, or the xbox version was different from
		/// the pc version (or any other engine version for that matter)
		/// </remarks>
		public BlamVersion Engine = BlamVersion.Unknown;

		/// <summary>
		/// Construct version info
		/// </summary>
		/// <param name="major">Major version</param>
		/// <param name="minor">Minor version</param>
		public VersionCtorAttribute(int major, int minor) { Major = major; Minor = minor; }

		/// <summary>
		/// Construct version info
		/// </summary>
		/// <param name="major">Major version</param>
		/// <param name="minor">Minor version</param>
		/// <param name="engine"></param>
		public VersionCtorAttribute(int major, int minor, BlamVersion engine) : this(major, minor) { Engine = engine; }

		/// <summary>
		/// Construct struct version info
		/// </summary>
		/// <param name="group_tag">Version's group tag</param>
		/// <param name="major">Major version</param>
		/// <param name="minor">Minor version</param>
		public VersionCtorAttribute(int group_tag, int major, int minor) : this(major, minor) { GroupTag = group_tag; }

		/// <summary>
		/// Construct struct version info
		/// </summary>
		/// <param name="group_tag">Version's group tag</param>
		/// <param name="major">Major version</param>
		/// <param name="minor">Minor version</param>
		/// <param name="engine"></param>
		public VersionCtorAttribute(int group_tag, int major, int minor, BlamVersion engine) : this(major, minor, engine) { GroupTag = group_tag; }
	};

	/// <summary>
	/// Version attribute applied to halo 2 definitions
	/// </summary>
	/// <see cref="Version"/>
	[AttributeUsage(AttributeTargets.Constructor, AllowMultiple=true)]
	public sealed class VersionCtorHalo2Attribute : VersionCtorAttribute
	{
		/// <summary>
		/// Construct version info for halo2 (default engine: <see cref="BlamVersion.Halo2"/>)
		/// </summary>
		/// <param name="major">Major version</param>
		/// <param name="minor">Minor version</param>
		public VersionCtorHalo2Attribute(int major, int minor) : base(major, minor, BlamVersion.Halo2) { }

		/// <summary>
		/// Construct version info for halo2
		/// </summary>
		/// <param name="major">Major version</param>
		/// <param name="minor">Minor version</param>
		/// <param name="engine"></param>
		public VersionCtorHalo2Attribute(int major, int minor, BlamVersion engine) : base(major, minor, engine) { }

		/// <summary>
		/// Construct struct version info for halo2 (default engine: <see cref="BlamVersion.Halo2"/>)
		/// </summary>
		/// <param name="group_tag">Version's group tag</param>
		/// <param name="major">Major version</param>
		/// <param name="minor">Minor version</param>
		public VersionCtorHalo2Attribute(int group_tag, int major, int minor) : base(group_tag, major, minor, BlamVersion.Halo2) { }

		/// <summary>
		/// Construct struct version info for halo2
		/// </summary>
		/// <param name="group_tag">Version's group tag</param>
		/// <param name="major">Major version</param>
		/// <param name="minor">Minor version</param>
		/// <param name="engine"></param>
		public VersionCtorHalo2Attribute(int group_tag, int major, int minor, BlamVersion engine) : base(group_tag, major, minor, engine) { }
	};

	/// <summary>
	/// Version attribute applied to halo 3 definitions
	/// </summary>
	/// <see cref="Version"/>
	[AttributeUsage(AttributeTargets.Constructor, AllowMultiple = true)]
	public sealed class VersionCtorHalo3Attribute : VersionCtorAttribute
	{
		/// <summary>
		/// Construct version info for halo3 (default engine: <see cref="BlamVersion.Halo3"/>)
		/// </summary>
		/// <param name="major">Major version</param>
		/// <param name="minor">Minor version</param>
		public VersionCtorHalo3Attribute(int major, int minor) : base(major, minor, BlamVersion.Halo3) { }

		/// <summary>
		/// Construct version info for halo3
		/// </summary>
		/// <param name="major">Major version</param>
		/// <param name="minor">Minor version</param>
		/// <param name="engine"></param>
		public VersionCtorHalo3Attribute(int major, int minor, BlamVersion engine) : base(major, minor, engine) { }

		/// <summary>
		/// Construct struct version info for halo3 (default engine: AnyHalo3)
		/// </summary>
		/// <param name="group_tag">Version's group tag</param>
		/// <param name="major">Major version</param>
		/// <param name="minor">Minor version</param>
		public VersionCtorHalo3Attribute(int group_tag, int major, int minor) : base(group_tag, major, minor, BlamVersion.Halo3) { }

		/// <summary>
		/// Construct struct version info for halo3
		/// </summary>
		/// <param name="group_tag">Version's group tag</param>
		/// <param name="major">Major version</param>
		/// <param name="minor">Minor version</param>
		/// <param name="engine"></param>
		public VersionCtorHalo3Attribute(int group_tag, int major, int minor, BlamVersion engine) : base(group_tag, major, minor, engine) { }
	};

	/// <summary>
	/// Version attribute applied to Halo ODST definitions
	/// </summary>
	/// <see cref="Version"/>
	[AttributeUsage(AttributeTargets.Constructor, AllowMultiple = true)]
	public sealed class VersionCtorHaloOdstAttribute : VersionCtorAttribute
	{
		/// <summary>
		/// Construct version info for HaloOdst (default engine: <see cref="BlamVersion.HaloOdst"/>)
		/// </summary>
		/// <param name="major">Major version</param>
		/// <param name="minor">Minor version</param>
		public VersionCtorHaloOdstAttribute(int major, int minor) : base(major, minor, BlamVersion.HaloOdst) { }

		/// <summary>
		/// Construct version info for HaloOdst
		/// </summary>
		/// <param name="major">Major version</param>
		/// <param name="minor">Minor version</param>
		/// <param name="engine"></param>
		public VersionCtorHaloOdstAttribute(int major, int minor, BlamVersion engine) : base(major, minor, engine) { }

		/// <summary>
		/// Construct struct version info for HaloOdst (default engine: <see cref="BlamVersion.HaloOdst"/>)
		/// </summary>
		/// <param name="group_tag">Version's group tag</param>
		/// <param name="major">Major version</param>
		/// <param name="minor">Minor version</param>
		public VersionCtorHaloOdstAttribute(int group_tag, int major, int minor) : base(group_tag, major, minor, BlamVersion.HaloOdst) { }

		/// <summary>
		/// Construct struct version info for HaloOdst
		/// </summary>
		/// <param name="group_tag">Version's group tag</param>
		/// <param name="major">Major version</param>
		/// <param name="minor">Minor version</param>
		/// <param name="engine"></param>
		public VersionCtorHaloOdstAttribute(int group_tag, int major, int minor, BlamVersion engine) : base(group_tag, major, minor, engine) { }
	};

	/// <summary>
	/// Version attribute applied to Halo Reach definitions
	/// </summary>
	/// <see cref="Version"/>
	[AttributeUsage(AttributeTargets.Constructor, AllowMultiple = true)]
	public sealed class VersionCtorHaloReachAttribute : VersionCtorAttribute
	{
		/// <summary>
		/// Construct version info for HaloReach (default engine: <see cref="BlamVersion.HaloReach"/>)
		/// </summary>
		/// <param name="major">Major version</param>
		/// <param name="minor">Minor version</param>
		public VersionCtorHaloReachAttribute(int major, int minor) : base(major, minor, BlamVersion.HaloReach) { }

		/// <summary>
		/// Construct version info for HaloReach
		/// </summary>
		/// <param name="major">Major version</param>
		/// <param name="minor">Minor version</param>
		/// <param name="engine"></param>
		public VersionCtorHaloReachAttribute(int major, int minor, BlamVersion engine) : base(major, minor, engine) { }

		/// <summary>
		/// Construct struct version info for HaloReach (default engine: <see cref="BlamVersion.HaloReach"/>)
		/// </summary>
		/// <param name="group_tag">Version's group tag</param>
		/// <param name="major">Major version</param>
		/// <param name="minor">Minor version</param>
		public VersionCtorHaloReachAttribute(int group_tag, int major, int minor) : base(group_tag, major, minor, BlamVersion.HaloReach) { }

		/// <summary>
		/// Construct struct version info for HaloReach
		/// </summary>
		/// <param name="group_tag">Version's group tag</param>
		/// <param name="major">Major version</param>
		/// <param name="minor">Minor version</param>
		/// <param name="engine"></param>
		public VersionCtorHaloReachAttribute(int group_tag, int major, int minor, BlamVersion engine) : base(group_tag, major, minor, engine) { }
	};
}