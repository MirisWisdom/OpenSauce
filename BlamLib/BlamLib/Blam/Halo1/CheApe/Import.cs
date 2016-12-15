/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
using System;
using System.Collections.Generic;
using System.Xml;

namespace BlamLib.Blam.Halo1.CheApe
{
	internal sealed partial class Import : BlamLib.CheApe.Import
	{
		internal const string kStringIdFieldDefinitionName = "string_id_yelo_non_resolving";
		internal const string kStringIdGroupTag = "sidy";
		internal const int kStringIdPadSize = StringId.kSizeOf - StringIdDesc.kSizeOf;

		internal TagReference StringIdFieldDefinition;
		internal Field StringIdFieldHandlePadding;

		void StringIdFieldsInitialize(BlamLib.CheApe.ProjectState state)
		{
			if (StringIdFieldDefinition != null) return;

			// Will add itself to the import state in the ctor
			StringIdFieldDefinition = new TagReference(state, kStringIdFieldDefinitionName, true, kStringIdGroupTag);
			StringIdFieldDefinition.ExplicitlyExport(state);
			StringIdFieldHandlePadding = new Field(state, state.kTypeIndexPad, "string_id", kStringIdPadSize.ToString());
		}		


		/// <summary>
		/// Tag Struct Definitions found in the XML files
		/// </summary>
		public Dictionary<string, TagStruct> Structs = new Dictionary<string, TagStruct>();
		/// <summary>
		/// Tag Block Definitions found in the XML files
		/// </summary>
		public Dictionary<string, TagBlock> Blocks = new Dictionary<string, TagBlock>();
		/// <summary>
		/// Tag Group Definitions found in the XML files
		/// </summary>
		public Dictionary<string, TagGroup> Groups = new Dictionary<string, TagGroup>();

		/// <summary>
		/// Reset the import state so there are no present definitions
		/// </summary>
		public override void Reset()
		{
			base.Reset();

			StringIdFieldDefinition = null;
			StringIdFieldHandlePadding = null;

			Structs.Clear();
			Blocks.Clear();
			Groups.Clear();
		}

		protected override int PreprocessXmlNodeComplexity(int complexity)
		{
			complexity = Math.Max(complexity, TagBlock.XmlNodeComplexity);
			complexity = Math.Max(complexity, TagGroup.XmlNodeComplexity);

			return complexity;
		}

		protected override void ProcessDefinition(XmlNode node, BlamLib.CheApe.ProjectState state, BlamLib.IO.XmlStream s)
		{
			StringIdFieldsInitialize(state);

			switch (node.Name)
			{
				#region Tag Structs
				case "structs":
					s.SaveCursor(node);
					ProcessTagStructs(state, s);
					s.RestoreCursor();
					break;
				#endregion

				#region Tag Blocks
				case "blocks":
					s.SaveCursor(node);
					ProcessTagBlocks(state, s);
					s.RestoreCursor();
					break;
				#endregion

				#region Tag Groups
				case "groups":
					s.SaveCursor(node);
					ProcessTagGroups(state, s);
					s.RestoreCursor();
					break;
				#endregion
			}
		}
	};
}