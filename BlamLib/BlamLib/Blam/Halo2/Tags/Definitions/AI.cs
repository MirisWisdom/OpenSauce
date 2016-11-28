/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
﻿#pragma warning disable 1591 // "Missing XML comment for publicly visible type or member"
using System;
using TI = BlamLib.TagInterface;

namespace BlamLib.Blam.Halo2.Tags
{
	#region ai_dialogue_globals
	[TI.TagGroup((int)TagGroups.Enumerated.adlg, 1, 60)]
	public sealed partial class ai_dialogue_globals_group : TI.Definition
	{
		#region vocalization_definitions_block_0
		public abstract partial class vocalization_definitions_block : TI.Definition
		{
			#region response_block
			[TI.Definition(1, 12)]
			public sealed partial class response_block : TI.Definition
			{
			};
			#endregion
		};

		[TI.Definition(2, 104)]
		public sealed partial class vocalization_definitions_block_0 : vocalization_definitions_block
		{
			#region vocalization_definitions_block_1
			[TI.Definition(2, 104)]
			public sealed partial class vocalization_definitions_block_1 : vocalization_definitions_block
			{
				#region vocalization_definitions_block_2
				[TI.Definition(2, 104)]
				public sealed partial class vocalization_definitions_block_2 : vocalization_definitions_block
				{
					#region vocalization_definitions_block_3
					[TI.Definition(2, 104)]
					public sealed partial class vocalization_definitions_block_3 : vocalization_definitions_block
					{
						#region vocalization_definitions_block_4
						[TI.Definition(2, 104)]
						public sealed partial class vocalization_definitions_block_4 : vocalization_definitions_block
						{
							#region vocalization_definitions_block_5
							[TI.Definition(2, 104)]
							public sealed partial class vocalization_definitions_block_5 : vocalization_definitions_block
							{
							};
							#endregion
						};
						#endregion
					};
					#endregion
				};
				#endregion
			};
			#endregion
		};
		#endregion

		#region vocalization_patterns_block
		[TI.Definition(1, 64)]
		public sealed partial class vocalization_patterns_block : TI.Definition
		{
			#region Fields
			#endregion
		}
		#endregion

		#region dialogue_data_block
		[TI.Definition(1, 4)]
		public sealed partial class dialogue_data_block : TI.Definition
		{
			public TI.ShortInteger StartIndex;
			public TI.ShortInteger Length;
		}
		#endregion

		#region involuntary_data_block
		[TI.Definition(1, 4)]
		public sealed partial class involuntary_data_block : TI.Definition
		{
			public TI.ShortInteger InvoluntaryVocalizationIndex;
		}
		#endregion

		#region Fields
		public TI.Block<vocalization_definitions_block_0> Vocalizations;
		public TI.Block<vocalization_patterns_block> Patterns;
		public TI.Block<dialogue_data_block> DialogueData;
		public TI.Block<involuntary_data_block> InvoluntaryData;
		#endregion
	};
	#endregion

	#region ai_mission_dialogue
	[TI.TagGroup((int)TagGroups.Enumerated.mdlg, 1, 12)]
	public sealed partial class ai_mission_dialogue_group : TI.Definition
	{
		#region mission_dialogue_lines_block
		[TI.Definition(1, 20)]
		public sealed partial class mission_dialogue_lines_block : TI.Definition
		{
			#region mission_dialogue_variants_block
			[TI.Definition(1, 24)]
			public sealed partial class mission_dialogue_variants_block : TI.Definition
			{
				#region Fields
				#endregion
			}
			#endregion

			#region Fields
			#endregion
		}
		#endregion

		public TI.Block<mission_dialogue_lines_block> Lines;
	};
	#endregion

	#region cellular_automata
	[TI.TagGroup((int)TagGroups.Enumerated.devo, 2, 564)]
	public sealed partial class cellular_automata_group : TI.Definition
	{
		#region Fields
		#endregion
	};
	#endregion

	#region cellular_automata2d
	[TI.TagGroup((int)TagGroups.Enumerated.whip, 1, 556)]
	public sealed partial class cellular_automata2d_group : TI.Definition
	{
		#region rules_block
		[TI.Definition(1, 88)]
		public sealed partial class rules_block : TI.Definition
		{
			#region states_block
			[TI.Definition(1, 96)]
			public sealed partial class states_block : TI.Definition
			{
				#region Fields
				#endregion
			}
			#endregion

			#region Fields
			#endregion
		}
		#endregion

		#region Fields
		#endregion
	};
	#endregion

	#region character
	[TI.TagGroup((int)TagGroups.Enumerated.char_, 1, 3, 372)]
	public sealed partial class character_group : TI.Definition
	{
		#region character_variants_block
		[TI.Definition(1, 12)]
		public sealed partial class character_variants_block : TI.Definition
		{
			#region Fields
			#endregion
		}
		#endregion

		#region character_general_block
		[TI.Definition(1, 12)]
		public sealed partial class character_general_block : TI.Definition
		{
			#region Fields
			#endregion
		}
		#endregion

		#region character_vitality_block
		[TI.Definition(1, 112)]
		public sealed partial class character_vitality_block : TI.Definition
		{
			#region Fields
			#endregion
		}
		#endregion

		#region character_placement_block
		[TI.Definition(1, 52)]
		public sealed partial class character_placement_block : TI.Definition
		{
			#region Fields
			#endregion
		}
		#endregion

		#region character_perception_block
		[TI.Definition(1, 52)]
		public sealed partial class character_perception_block : TI.Definition
		{
			#region Fields
			#endregion
		}
		#endregion

		#region character_look_block
		[TI.Definition(1, 80)]
		public sealed partial class character_look_block : TI.Definition
		{
			#region Fields
			#endregion
		}
		#endregion

		#region character_movement_block
		[TI.Definition(1, 36)]
		public sealed partial class character_movement_block : TI.Definition
		{
			#region Fields
			#endregion
		}
		#endregion

		#region character_swarm_block
		[TI.Definition(1, 40)]
		public sealed partial class character_swarm_block : TI.Definition
		{
			#region Fields
			#endregion
		}
		#endregion

		#region character_ready_block
		[TI.Definition(1, 8)]
		public sealed partial class character_ready_block : TI.Definition
		{
		}
		#endregion

		#region character_engage_block
		[TI.Definition(1, 16)]
		public sealed partial class character_engage_block : TI.Definition
		{
			#region Fields
			#endregion
		}
		#endregion

		#region character_charge_block
		[TI.Definition(4, 72)]
		public sealed partial class character_charge_block : TI.Definition
		{
			#region Fields
			#endregion
		}
		#endregion

		#region character_evasion_block
		[TI.Definition(1, 20)]
		public sealed partial class character_evasion_block : TI.Definition
		{
			#region Fields
			#endregion
		}
		#endregion

		#region character_cover_block
		[TI.Definition(1, 64)]
		public sealed partial class character_cover_block : TI.Definition
		{
			#region Fields
			#endregion
		}
		#endregion

		#region character_retreat_block
		[TI.Definition(1, 84)]
		public sealed partial class character_retreat_block : TI.Definition
		{
			#region Fields
			#endregion
		}
		#endregion

		#region character_search_block
		[TI.Definition(1, 20)]
		public sealed partial class character_search_block : TI.Definition
		{
			#region Fields
			#endregion
		}
		#endregion

		#region character_presearch_block
		[TI.Definition(2, 36)]
		public sealed partial class character_presearch_block : TI.Definition
		{
			#region Fields
			#endregion
		}
		#endregion

		#region character_idle_block
		[TI.Definition(1, 12)]
		public sealed partial class character_idle_block : TI.Definition
		{
			#region Fields
			#endregion
		}
		#endregion

		#region character_vocalization_block
		[TI.Definition(1, 8)]
		public sealed partial class character_vocalization_block : TI.Definition
		{
			#region Fields
			#endregion
		}
		#endregion

		#region character_boarding_block
		[TI.Definition(1, 16)]
		public sealed partial class character_boarding_block : TI.Definition
		{
			#region Fields
			#endregion
		}
		#endregion

		#region character_boss_block
		[TI.Definition(1, 12)]
		public sealed partial class character_boss_block : TI.Definition
		{
			#region Fields
			#endregion
		}
		#endregion

		#region character_firing_pattern_block
		[TI.Definition(1, 64)]
		public sealed partial class character_firing_pattern_block : TI.Definition
		{
			#region Fields
			#endregion
		}
		#endregion

		#region character_weapons_block
		[TI.Definition(2, 224)]
		public sealed partial class character_weapons_block : TI.Definition
		{
			#region Fields
			#endregion
		}
		#endregion

		#region character_firing_pattern_properties_block
		[TI.Definition(1, 28)]
		public sealed partial class character_firing_pattern_properties_block : TI.Definition
		{
			#region Fields
			#endregion
		}
		#endregion

		#region character_grenades_block
		[TI.Definition(1, 60)]
		public sealed partial class character_grenades_block : TI.Definition
		{
			#region Fields
			#endregion
		}
		#endregion

		#region character_vehicle_block
		[TI.Definition(2, 196)]
		public sealed partial class character_vehicle_block : TI.Definition
		{
			#region Fields
			#endregion
		}
		#endregion

		#region Fields
		public TI.Flags Flags;
		public TI.TagReference ParentCharacter;
		public TI.TagReference Unit;
		public TI.TagReference Creature;
		public TI.TagReference Style;
		public TI.TagReference MajorCharacter;
		public TI.Block<character_variants_block> Variants;
		public TI.Block<character_general_block> GeneralProperties;
		public TI.Block<character_vitality_block> VitalityProperties;
		public TI.Block<character_placement_block> PlacementProperties;
		public TI.Block<character_perception_block> PerceptionProperties;
		public TI.Block<character_look_block> LookProperties;
		public TI.Block<character_movement_block> MovementProperties;
		public TI.Block<character_swarm_block> SwarmProperties;
		public TI.Block<character_ready_block> ReadyProperties;
		public TI.Block<character_engage_block> EngageProperties;
		#endregion
	};
	#endregion

	#region dialogue
	[TI.TagGroup((int)TagGroups.Enumerated.udlg, 1, 2, 36)]
	public sealed partial class dialogue_group : TI.Definition
	{
		#region sound_references_block
		[TI.Definition(2, 24)]
		public sealed partial class sound_references_block : TI.Definition
		{
			#region Fields
			#endregion
		}
		#endregion

		#region Fields
		#endregion
	};
	#endregion

	#region style
	[TI.TagGroup((int)TagGroups.Enumerated.styl, 1, 100)]
	public sealed partial class style_group : TI.Definition
	{
		#region special_movement_block
		[TI.Definition(1, 4)]
		public sealed partial class special_movement_block : TI.Definition
		{
		}
		#endregion

		#region behavior_names_block
		[TI.Definition(1, 32)]
		public sealed partial class behavior_names_block : TI.Definition
		{
		}
		#endregion

		#region Fields
		#endregion
	};
	#endregion
};