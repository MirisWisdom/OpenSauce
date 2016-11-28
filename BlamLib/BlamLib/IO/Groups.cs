/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
using System;
using System.Collections.Generic;
using System.Text;
using BlamLib.TagInterface;

namespace BlamLib.IO
{
#pragma warning disable 1591 // "Missing XML comment for publicly visible type or member"
	public static class TagGroups
	{
		#region Tag Groups
		/// <summary>
		/// Generic BlamLib file manager signature
		/// </summary>
		public static readonly TagGroup BlamLib = new TagGroup("blib", "blamlib");

		/// <summary>
		/// <see cref="IO.DirectoryListingByExtension"/> signature
		/// </summary>
		public static readonly TagGroup DirectoryListingByExtension = new TagGroup("dlbe", "directory_listing");

		/// <summary>
		/// <see cref="CheApe.Project"/> signature
		/// </summary>
		public static readonly TagInterface.TagGroup CheApeProject = new TagInterface.TagGroup("CHE1", "cheape_project");

		/// <summary>
		/// DefinitionFile signature
		/// </summary>
		public static readonly TagGroup DefinitionFile = new TagGroup("deff", "definition");

		/// <summary>
		/// DefinitionConversionFile signature
		/// </summary>
		public static readonly TagGroup DefinitionConversionFile = new TagGroup("decf", "definition_conversion");

		/// <summary>
		/// <see cref="Blam.Halo1.DelimitorFile"/> signature
		/// </summary>
		public static readonly TagGroup DelimitorFile = new TagGroup("delf", "delimitor");

		/// <summary>
		/// <see cref="Managers.ReferenceManager"/> signature
		/// </summary>
		public static readonly TagGroup ReferenceManagerFile = new TagGroup("refm", "reference_manager_data");

		/// <summary>
		/// <see cref="Managers.ReferenceActionsManager"/> signature
		/// </summary>
		public static readonly TagGroup ReferenceActionsFile = new TagGroup("refa", "reference_actions_data");
		#endregion

		#region Groups
		/// <summary>
		/// All file-manageable tag groups
		/// </summary>
		public static TagGroupCollection Groups = new TagGroupCollection(false,
			BlamLib,
			DirectoryListingByExtension,
			CheApeProject,
			DefinitionFile,
			DefinitionConversionFile,
			DelimitorFile,
			ReferenceManagerFile,
			ReferenceActionsFile
			);

		/// <summary>
		/// Constant indices to each tag group in <see cref="Groups"/>
		/// </summary>
		public enum Enumerated : int
		{
			BlamLib,
			DirectoryListingByExtension,
			CheApeProject,
			DefinitionFile,
			DefinitionConversionFile,
			DelimitorFile,
			ReferenceManagerFile,
			ReferenceActionsFile,
		};
		#endregion
	};
#pragma warning restore 1591 // "Missing XML comment for publicly visible type or member"

	#region Class File Management
	/// <summary>
	/// Attribute applied to a class to define its required file information
	/// </summary>
	[AttributeUsage(AttributeTargets.Class, AllowMultiple = false, Inherited = false)]
	public sealed class ClassAttribute : DefinitionAttribute
	{
		#region GroupIndex
		/// <summary>
		/// Index of this class's <see cref="TagInterface.TagGroup"/> definition
		/// </summary>
		/// <remarks>Index is relative to the <see cref="IO.TagGroups.Groups"/> collection</remarks>
		public readonly int GroupIndex;
		#endregion

		/// <summary>
		/// Constructs a class description
		/// </summary>
		/// <param name="group_tag"></param>
		public ClassAttribute(int group_tag) : base() { GroupIndex = group_tag; }

		/// <summary>
		/// Constructs a class description with a specific version identifier
		/// </summary>
		/// <param name="group_tag"></param>
		/// <param name="version">Version of this structure definition</param>
		public ClassAttribute(int group_tag, short version) : base(version) { GroupIndex = group_tag; }

		public static new ClassAttribute FromType(Type t)
		{
			object[] attrs = t.GetCustomAttributes(typeof(ClassAttribute), false);
			if (attrs.Length == 0)
				throw new Debug.ExceptionLog("Type {0} doesn't have this attribute applied to it", t.FullName);
			else
				return (ClassAttribute)attrs[0]; // attrs will have ClassAttribute only objects, so the 'as' operator would be redundant here
		}
	};

	/// <summary>
	/// Describes the serialization data of a single IO manageable class
	/// </summary>
	public sealed class FileManageableState
	{
		#region DefinitionType
		private Type defType;
		/// <summary>
		/// Type information of the <see cref="FileManageable"/> this state data is for
		/// </summary>
		public Type DefinitionType { get { return defType; } }
		#endregion

		/// <summary>
		/// Create state data from a <see cref="FileManageable"/> instance
		/// </summary>
		/// <param name="definition"></param>
		/// <param name="inst"></param>
		public FileManageableState(Type definition, FileManageable inst)
		{
			defType = definition;
			ProcessAtrributes(inst);
		}

		#region GroupTag
		TagGroup groupTag;
		/// <summary>
		/// Group tag for the data of this object's file signature
		/// </summary>
		public TagGroup GroupTag { get { return groupTag; } }
		#endregion

		#region Reflection
		#region Attribute
		private ClassAttribute attribute;
		/// <summary>
		/// 
		/// </summary>
		public ClassAttribute Attribute { get { return attribute; } }
		#endregion

		/// <summary>
		/// Processes attributes applied to one of the inheriting classes of this class
		/// </summary>
		private void ProcessAtrributes(FileManageable inst)
		{
			#region Get definition attribute
			Type t = inst.GetType();
			object[] attr = t.GetCustomAttributes(typeof(ClassAttribute), false);
			if (attr.Length == 0) return;

			ClassAttribute ca = attr[0] as ClassAttribute;
			attribute = ca;
			#endregion

			groupTag = TagGroups.Groups[ca.GroupIndex];
		}
		#endregion

		#region State Pool
		static Dictionary<Type, FileManageableState> Pool = new Dictionary<Type, FileManageableState>(32);

		/// <summary>
		/// Builds or finds the state information for a <see cref="FileManageable"/> type
		/// </summary>
		/// <param name="inst">Target definition</param>
		/// <returns>State data for <paramref name="inst"/></returns>
		internal static FileManageableState Add(FileManageable inst)
		{
			Type t = inst.GetType();
			FileManageableState state = null;

			lock(Pool)
				if (!Pool.TryGetValue(t, out state))
				{
					state = new FileManageableState(t, inst);
					Pool.Add(t, state);
				}

			return state;
		}
		#endregion
	};

	/// <summary>
	/// Object state to stream state management base class
	/// </summary>
	/// <remarks>
	/// Basically the <seealso cref="BlamLib.TagInterface.Definition"/> solution for internal 
	/// library classes that may or may not be related to something blam
	/// </remarks>
	public abstract class FileManageable : IStreamable
	{
		#region State
		protected FileManageableState state;
		/// <summary>
		/// Definition data for this file manageable object
		/// </summary>
		public FileManageableState State { get { return state; } }
		#endregion

		#region GroupTag
		/// <summary>
		/// Group tag for the data of this object's file signature
		/// </summary>
		/// <remarks>Uses the <see cref="State"/> member</remarks>
		public TagGroup GroupTag { get { return state.GroupTag; } }
		#endregion

		#region Attribute
		/// <summary>
		/// Class attribute applied to this <see cref="FileManageable"/> class
		/// </summary>
		/// <remarks>Uses the <see cref="State"/> member</remarks>
		public ClassAttribute Attribute { get { return state.Attribute; } }
		#endregion

		/// <remarks>
		/// Automatically finds and assigns runtime meta data needed
		/// to do advance stuff with this class file definition (like versioning)
		/// </remarks>
		protected FileManageable() { state = FileManageableState.Add(this); }

		#region IStreamable Members
		/// <summary>
		/// Stream the contents of the file from a buffer
		/// </summary>
		/// <param name="s"></param>
		public abstract void Read(EndianReader s);
		/// <summary>
		/// Stream the contents of the file to a buffer
		/// </summary>
		/// <param name="s"></param>
		public abstract void Write(EndianWriter s);
		#endregion
	};
	#endregion
}