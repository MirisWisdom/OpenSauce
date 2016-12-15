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
	partial class ai_dialogue_globals_group
	{
		#region vocalization_definitions_block_0
		partial class vocalization_definitions_block
		{
			#region response_block
			partial class response_block
			{
				#region Ctor
				public response_block() : base(5)
				{
					Add(/*vocalization name = */ new TI.StringId());
					Add(/*flags = */ new TI.Flags(TI.FieldType.WordFlags));
					Add(/*vocalization index (post process) = */ new TI.ShortInteger());
					Add(/*response type = */ new TI.Enum());
					Add(/*dialogue index (import) = */ new TI.ShortInteger());
				}
				#endregion
			};
			#endregion

			#region Ctor
			protected vocalization_definitions_block(int field_count) : base(26 + field_count)
			{
				Add(/*vocalization = */ new TI.StringId());
				Add(/*parent vocalization = */ new TI.StringId());
				Add(/*parent index = */ new TI.ShortInteger());
				Add(/*priority = */ new TI.Enum());
				Add(/*flags = */ new TI.Flags());
				Add(/*glance behavior = */ new TI.Enum());
				Add(/*glance recipient behavior = */ new TI.Enum());
				Add(/*perception type = */ new TI.Enum());
				Add(/*max combat status = */ new TI.Enum());
				Add(/*animation impulse = */ new TI.Enum());
				Add(/*overlap priority = */ new TI.Enum());
				Add(/*sound repetition delay = */ new TI.Real());
				Add(/*allowable queue delay = */ new TI.Real());
				Add(/*pre voc. delay = */ new TI.Real());
				Add(/*notification delay = */ new TI.Real());
				Add(/*post voc. delay = */ new TI.Real());
				Add(/*repeat delay = */ new TI.Real());
				Add(/*weight = */ new TI.Real());
				Add(/*speaker freeze time = */ new TI.Real());
				Add(/*listener freeze time = */ new TI.Real());
				Add(/*speaker emotion = */ new TI.Enum());
				Add(/*listener emotion = */ new TI.Enum());
				Add(/*player skip fraction = */ new TI.Real());
				Add(/*skip fraction = */ new TI.Real());
				Add(/*Sample line = */ new TI.StringId());
				Add(/*reponses = */ new TI.Block<response_block>(this, 20));
			}
			#endregion
		};

		partial class vocalization_definitions_block_0
		{
			#region vocalization_definitions_block_1
			partial class vocalization_definitions_block_1
			{
				#region vocalization_definitions_block_2
				partial class vocalization_definitions_block_2
				{
					#region vocalization_definitions_block_3
					partial class vocalization_definitions_block_3
					{
						#region vocalization_definitions_block_4
						partial class vocalization_definitions_block_4
						{
							#region vocalization_definitions_block_5
							partial class vocalization_definitions_block_5
							{
								public vocalization_definitions_block_5() : base(1)
								{
									Add(/* = */ new TI.Block<TI.g_null_block>(this, 0));
								}
							};
							#endregion

							public vocalization_definitions_block_4() : base(1)
							{
								Add(/*children = */ new TI.Block<vocalization_definitions_block_5>(this, 500));
							}
						};
						#endregion

						public vocalization_definitions_block_3() : base(1)
						{
							Add(/*children = */ new TI.Block<vocalization_definitions_block_4>(this, 500));
						}
					};
					#endregion

					public vocalization_definitions_block_2() : base(1)
					{
						Add(/*children = */ new TI.Block<vocalization_definitions_block_3>(this, 500));
					}
				};
				#endregion

				public vocalization_definitions_block_1() : base(1)
				{
					Add(/*children = */ new TI.Block<vocalization_definitions_block_2>(this, 500));
				}
			};
			#endregion

			public vocalization_definitions_block_0() : base(1)
			{
				Add(/*children = */ new TI.Block<vocalization_definitions_block_1>(this, 500));
			}
		};
		#endregion

		#region vocalization_patterns_block
		partial class vocalization_patterns_block
		{
			#region Ctor
			public vocalization_patterns_block() : base(22)
			{
				Add(/*dialogue type = */ new TI.Enum());
				Add(/*vocalization index = */ new TI.ShortInteger());
				Add(/*vocalization name = */ new TI.StringId());
				Add(/*speaker type = */ new TI.Enum());
				Add(/*flags = */ new TI.Flags(TI.FieldType.WordFlags));
				Add(/*listener/target = */ new TI.Enum());
				Add(new TI.Pad(2 + 4));
				Add(/*hostility = */ new TI.Enum());
				Add(/*damage type = */ new TI.Enum());
				Add(/*danger level = */ new TI.Enum());
				Add(/*attitude = */ new TI.Enum());
				Add(new TI.Pad(4));
				Add(/*subject actor type = */ new TI.Enum());
				Add(/*cause actor type = */ new TI.Enum());
				Add(/*cause type = */ new TI.Enum());
				Add(/*subject type = */ new TI.Enum());
				Add(/*cause ai type name = */ new TI.StringId());
				Add(/*spatial relation = */ new TI.Enum());
				Add(new TI.Pad(2));
				Add(/*subject ai type name = */ new TI.StringId());
				Add(new TI.Pad(8));
				Add(/*Conditions = */ new TI.Flags());
			}
			#endregion
		};
		#endregion

		#region dialogue_data_block
		partial class dialogue_data_block
		{
			public dialogue_data_block() : base(2)
			{
				Add(StartIndex = new TI.ShortInteger());
				Add(Length = new TI.ShortInteger());
			}
		};
		#endregion

		#region involuntary_data_block
		partial class involuntary_data_block
		{
			public involuntary_data_block() : base(2)
			{
				Add(InvoluntaryVocalizationIndex = new TI.ShortInteger());
				Add(new TI.Pad(2));
			}
		};
		#endregion

		#region Ctor
		public ai_dialogue_globals_group() : base(5)
		{
			Add(Vocalizations = new TI.Block<vocalization_definitions_block_0>(this, 500));
			Add(Patterns = new TI.Block<vocalization_patterns_block>(this, 1000));
			Add(new TI.Pad(12));
			Add(DialogueData = new TI.Block<dialogue_data_block>(this, 200));
			Add(InvoluntaryData = new TI.Block<involuntary_data_block>(this, 100));
		}
		#endregion
	};
	#endregion

	#region ai_mission_dialogue
	partial class ai_mission_dialogue_group
	{
		#region mission_dialogue_lines_block
		partial class mission_dialogue_lines_block
		{
			#region mission_dialogue_variants_block
			partial class mission_dialogue_variants_block
			{
				#region Ctor
				public mission_dialogue_variants_block() : base(3)
				{
					Add(/*variant designation = */ new TI.StringId());
					Add(/*sound = */ new TI.TagReference(this, TagGroups.snd_));
					Add(/*sound effect = */ new TI.StringId());
				}
				#endregion
			};
			#endregion

			#region Ctor
			public mission_dialogue_lines_block() : base(3)
			{
				Add(/*name = */ new TI.StringId());
				Add(/*variants = */ new TI.Block<mission_dialogue_variants_block>(this, 10));
				Add(/*default sound effect = */ new TI.StringId());
			}
			#endregion
		};
		#endregion

		#region Ctor
		public ai_mission_dialogue_group() : base(1)
		{
			Add(Lines = new TI.Block<mission_dialogue_lines_block>(this, 500));
		}
		#endregion
	};
	#endregion

	#region cellular_automata
	partial class cellular_automata_group
	{
		#region Ctor
		public cellular_automata_group() : base(31)
		{
			Add(/*updates per second = */ new TI.ShortInteger());
			Add(/*x (width) = */ new TI.ShortInteger());
			Add(/*y (depth) = */ new TI.ShortInteger());
			Add(/*z (height) = */ new TI.ShortInteger());
			Add(/*x (width) = */ new TI.Real());
			Add(/*y (depth) = */ new TI.Real());
			Add(/*z (height) = */ new TI.Real());
			Add(new TI.Pad(32));
			Add(/*marker = */ new TI.StringId(true));
			Add(/*cell birth chance = */ new TI.Real(TI.FieldType.RealFraction));
			Add(new TI.Pad(32));
			Add(/*cell gene mutates 1 in = */ new TI.LongInteger());
			Add(/*virus gene mutations 1 in = */ new TI.LongInteger());
			Add(new TI.Pad(32));
			Add(/*infected cell lifespan = */ new TI.ShortIntegerBounds());
			Add(/*minimum infection age = */ new TI.ShortInteger());
			Add(new TI.Pad(2));
			Add(/*cell infection chance = */ new TI.Real(TI.FieldType.RealFraction));
			Add(/*infection threshold = */ new TI.Real(TI.FieldType.RealFraction));
			Add(new TI.Pad(32));
			Add(/*new cell filled chance = */ new TI.Real(TI.FieldType.RealFraction));
			Add(/*new cell infected chance = */ new TI.Real(TI.FieldType.RealFraction));
			Add(new TI.Pad(32));
			Add(/*detail texture change chance = */ new TI.Real(TI.FieldType.RealFraction));
			Add(new TI.Pad(32));
			Add(/*detail texture width = */ new TI.ShortInteger());
			Add(new TI.Pad(2));
			Add(/*detail texture = */ new TI.TagReference(this, TagGroups.bitm));
			Add(new TI.Pad(32));
			Add(/*mask bitmap = */ new TI.TagReference(this, TagGroups.bitm));
			Add(new TI.Pad(240));
		}
		#endregion
	};
	#endregion

	#region cellular_automata2d
	partial class cellular_automata2d_group
	{
		#region rules_block
		partial class rules_block
		{
			#region states_block
			partial class states_block
			{
				#region Ctor
				public states_block() : base(16)
				{
					Add(/*name = */ new TI.String());
					Add(/*color = */ new TI.RealColor());
					Add(/*counts as = */ new TI.ShortInteger());
					Add(new TI.Pad(2));
					Add(/*initial placement weight = */ new TI.Real());
					Add(new TI.Pad(24));
					Add(/*zero = */ new TI.BlockIndex()); // 1 states_block
					Add(/*one = */ new TI.BlockIndex()); // 1 states_block
					Add(/*two = */ new TI.BlockIndex()); // 1 states_block
					Add(/*three = */ new TI.BlockIndex()); // 1 states_block
					Add(/*four = */ new TI.BlockIndex()); // 1 states_block
					Add(/*five = */ new TI.BlockIndex()); // 1 states_block
					Add(/*six = */ new TI.BlockIndex()); // 1 states_block
					Add(/*seven = */ new TI.BlockIndex()); // 1 states_block
					Add(/*eight = */ new TI.BlockIndex()); // 1 states_block
					Add(new TI.Pad(2));
				}
				#endregion
			};
			#endregion

			#region Ctor
			public rules_block() : base(4)
			{
				Add(/*name = */ new TI.String());
				Add(/*tint color = */ new TI.RealColor());
				Add(new TI.Pad(32));
				Add(/*states = */ new TI.Block<states_block>(this, 16));
			}
			#endregion
		};
		#endregion

		#region Ctor
		public cellular_automata2d_group() : base(27)
		{
			Add(/*updates per second = */ new TI.ShortInteger());
			Add(new TI.Pad(2));
			Add(/*dead cell penalty = */ new TI.Real());
			Add(/*live cell bonus = */ new TI.Real());
			Add(new TI.Pad(80));
			Add(/*width = */ new TI.ShortInteger());
			Add(/*height = */ new TI.ShortInteger());
			Add(/*cell width = */ new TI.Real());
			Add(/*height = */ new TI.Real());
			Add(/*velocity = */ new TI.RealVector2D());
			Add(new TI.Pad(28));
			Add(/*marker = */ new TI.StringId());
			Add(/*interpolation flags = */ new TI.Flags());
			Add(/*base color = */ new TI.RealColor());
			Add(/*peak color = */ new TI.RealColor());
			Add(new TI.Pad(76));
			Add(/*width = */ new TI.ShortInteger());
			Add(/*height = */ new TI.ShortInteger());
			Add(/*cell width = */ new TI.Real());
			Add(/*velocity = */ new TI.RealVector2D());
			Add(new TI.Pad(48));
			Add(/*marker = */ new TI.StringId());
			Add(/*texture width = */ new TI.ShortInteger());
			Add(new TI.Pad(2 + 48));
			Add(/*texture = */ new TI.TagReference(this, TagGroups.bitm));
			Add(new TI.Pad(160));
			Add(/*rules = */ new TI.Block<rules_block>(this, 16));
		}
		#endregion
	};
	#endregion

	#region character
	partial class character_group
	{
		#region character_variants_block
		partial class character_variants_block
		{
			#region Ctor
			public character_variants_block() : base(4)
			{
				Add(/*variant name = */ new TI.StringId());
				Add(/*variant index = */ new TI.ShortInteger());
				Add(new TI.Pad(2));
				Add(/*variant designator = */ new TI.StringId());
			}
			#endregion
		};
		#endregion

		#region character_general_block
		partial class character_general_block
		{
			#region Ctor
			public character_general_block() : base(5)
			{
				Add(/*general flags = */ new TI.Flags());
				Add(/*type = */ new TI.Enum());
				Add(new TI.Pad(2));
				Add(new TI.UselessPad(100));
				Add(/*scariness = */ new TI.Real());
			}
			#endregion
		};
		#endregion

		#region character_vitality_block
		partial class character_vitality_block
		{
			#region Ctor
			public character_vitality_block() : base(31)
			{
				Add(/*vitality flags = */ new TI.Flags());
				Add(/*normal body vitality = */ new TI.Real());
				Add(/*normal shield vitality = */ new TI.Real());
				Add(/*legendary body vitality = */ new TI.Real());
				Add(/*legendary shield vitality = */ new TI.Real());
				Add(/*body recharge fraction = */ new TI.Real());
				Add(/*soft ping threshold (with shields) = */ new TI.Real());
				Add(/*soft ping threshold (no shields) = */ new TI.Real());
				Add(/*soft ping min interrupt time = */ new TI.Real());
				Add(new TI.UselessPad(4));
				Add(/*hard ping threshold (with shields) = */ new TI.Real());
				Add(/*hard ping threshold (no shields) = */ new TI.Real());
				Add(/*hard ping min interrupt time = */ new TI.Real());
				Add(new TI.UselessPad(4));
				Add(/*current damage decay delay = */ new TI.Real());
				Add(/*current damage decay time = */ new TI.Real());
				Add(new TI.UselessPad(8));
				Add(/*recent damage decay delay = */ new TI.Real());
				Add(/*recent damage decay time = */ new TI.Real());
				Add(/*body recharge delay time = */ new TI.Real());
				Add(/*body recharge time = */ new TI.Real());
				Add(/*shield recharge delay time = */ new TI.Real());
				Add(/*shield recharge time = */ new TI.Real());
				Add(/*stun threshold = */ new TI.Real());
				Add(/*stun time bounds = */ new TI.RealBounds());
				Add(new TI.UselessPad(16));
				Add(/*extended shield damage threshold = */ new TI.Real());
				Add(/*extended body damage threshold = */ new TI.Real());
				Add(new TI.UselessPad(16));
				Add(/*suicide radius = */ new TI.Real());
				Add(new TI.Skip(8));
			}
			#endregion
		};
		#endregion

		#region character_placement_block
		partial class character_placement_block
		{
			#region Ctor
			public character_placement_block() : base(13)
			{
				Add(new TI.Pad(4));
				Add(/*few upgrade chance (easy) = */ new TI.Real());
				Add(/*few upgrade chance (normal) = */ new TI.Real());
				Add(/*few upgrade chance (heroic) = */ new TI.Real());
				Add(/*few upgrade chance (legendary) = */ new TI.Real());
				Add(/*normal upgrade chance (easy) = */ new TI.Real());
				Add(/*normal upgrade chance (normal) = */ new TI.Real());
				Add(/*normal upgrade chance (heroic) = */ new TI.Real());
				Add(/*normal upgrade chance (legendary) = */ new TI.Real());
				Add(/*many upgrade chance (easy) = */ new TI.Real());
				Add(/*many upgrade chance (normal) = */ new TI.Real());
				Add(/*many upgrade chance (heroic) = */ new TI.Real());
				Add(/*many upgrade chance (legendary) = */ new TI.Real());
			}
			#endregion
		};
		#endregion

		#region character_perception_block
		partial class character_perception_block
		{
			#region Ctor
			public character_perception_block() : base(17)
			{
				Add(/*perception flags = */ new TI.Flags());
				Add(/*max vision distance = */ new TI.Real());
				Add(/*central vision angle = */ new TI.Real(TI.FieldType.Angle));
				Add(/*max vision angle = */ new TI.Real(TI.FieldType.Angle));
				Add(new TI.UselessPad(4));
				Add(/*peripheral vision angle = */ new TI.Real(TI.FieldType.Angle));
				Add(/*peripheral distance = */ new TI.Real());
				Add(new TI.UselessPad(4 + 24));
				Add(/*hearing distance = */ new TI.Real());
				Add(/*notice projectile chance = */ new TI.Real());
				Add(/*notice vehicle chance = */ new TI.Real());
				Add(new TI.UselessPad(8));
				Add(/*combat perception time = */ new TI.Real());
				Add(/*guard perception time = */ new TI.Real());
				Add(/*non-combat perception time = */ new TI.Real());
				Add(new TI.UselessPad(24));
				Add(/*first ack. surprise distance = */ new TI.Real());
			}
			#endregion
		};
		#endregion

		#region character_look_block
		partial class character_look_block
		{
			#region Ctor
			public character_look_block() : base(11)
			{
				Add(/*maximum aiming deviation = */ new TI.RealEulerAngles2D());
				Add(/*maximum looking deviation = */ new TI.RealEulerAngles2D());
				Add(new TI.Pad(16));
				Add(/*noncombat look delta L = */ new TI.Real(TI.FieldType.Angle));
				Add(/*noncombat look delta R = */ new TI.Real(TI.FieldType.Angle));
				Add(/*combat look delta L = */ new TI.Real(TI.FieldType.Angle));
				Add(/*combat look delta R = */ new TI.Real(TI.FieldType.Angle));
				Add(/*noncombat idle looking = */ new TI.RealBounds());
				Add(/*noncombat idle aiming = */ new TI.RealBounds());
				Add(/*combat idle looking = */ new TI.RealBounds());
				Add(/*combat idle aiming = */ new TI.RealBounds());
			}
			#endregion
		};
		#endregion

		#region character_movement_block
		partial class character_movement_block
		{
			#region Ctor
			public character_movement_block() : base(14)
			{
				Add(/*movement flags = */ new TI.Flags());
				Add(/*pathfinding radius = */ new TI.Real());
				Add(/*destination radius = */ new TI.Real());
				Add(new TI.UselessPad(20));
				Add(/*dive grenade chance = */ new TI.Real());
				Add(new TI.UselessPad(8));
				Add(/*obstacle leap min size = */ new TI.Enum());
				Add(/*obstacle leap max size = */ new TI.Enum());
				Add(/*obstacle ignore size = */ new TI.Enum());
				Add(/*obstacle smashable size = */ new TI.Enum());
				Add(new TI.Pad(2));
				Add(/*jump height = */ new TI.Enum());
				Add(/*movement hints = */ new TI.Flags());
				Add(/*throttle scale = */ new TI.Real());
			}
			#endregion
		};
		#endregion

		#region character_swarm_block
		partial class character_swarm_block
		{
			#region Ctor
			public character_swarm_block() : base(13)
			{
				Add(new TI.UselessPad(48));
				Add(/*scatter killed count = */ new TI.ShortInteger());
				Add(new TI.Pad(2));
				Add(/*scatter radius = */ new TI.Real());
				Add(/*scatter time = */ new TI.Real());
				Add(new TI.UselessPad(16));
				Add(/*hound min distance = */ new TI.Real());
				Add(/*hound max distance = */ new TI.Real());
				Add(new TI.UselessPad(16));
				Add(/*perlin offset scale = */ new TI.Real());
				Add(/*offset period = */ new TI.RealBounds());
				Add(/*perlin idle movement threshold = */ new TI.Real());
				Add(/*perlin combat movement threshold = */ new TI.Real());
			}
			#endregion
		};
		#endregion

		#region character_ready_block
		partial class character_ready_block
		{
			public character_ready_block() : base(1)
			{
				Add(/*ready time bounds = */ new TI.RealBounds());
			}
		};
		#endregion

		#region character_engage_block
		partial class character_engage_block
		{
			#region Ctor
			public character_engage_block() : base(5)
			{
				Add(/*flags = */ new TI.Flags());
				Add(new TI.UselessPad(16));
				Add(/*Crouch danger threshold = */ new TI.Real());
				Add(/*Stand danger threshold = */ new TI.Real());
				Add(/*Fight danger move threshold = */ new TI.Real());
			}
			#endregion
		};
		#endregion

		#region character_charge_block
		partial class character_charge_block
		{
			#region Ctor
			public character_charge_block() : base(15)
			{
				Add(/*Charge flags = */ new TI.Flags());
				Add(/*melee consider range = */ new TI.Real());
				Add(/*melee chance = */ new TI.Real());
				Add(/*melee attack range = */ new TI.Real());
				Add(/*melee abort range = */ new TI.Real());
				Add(/*melee attack timeout = */ new TI.Real());
				Add(/*melee attack delay timer = */ new TI.Real());
				Add(/*melee leap range = */ new TI.RealBounds());
				Add(/*melee leap chance = */ new TI.Real());
				Add(/*ideal leap velocity = */ new TI.Real());
				Add(/*max leap velocity = */ new TI.Real());
				Add(/*melee leap ballistic = */ new TI.Real());
				Add(/*melee delay timer = */ new TI.Real());
				Add(new TI.UselessPad(12));
				Add(/*berserk weapon = */ new TI.TagReference(this, TagGroups.weap));
			}
			#endregion
		};
		#endregion

		#region character_evasion_block
		partial class character_evasion_block
		{
			#region Ctor
			public character_evasion_block() : base(7)
			{
				Add(new TI.UselessPad(4));
				Add(/*Evasion danger threshold = */ new TI.Real());
				Add(/*Evasion delay timer = */ new TI.Real());
				Add(/*Evasion chance = */ new TI.Real());
				Add(/*Evasion proximity threshold = */ new TI.Real());
				Add(new TI.UselessPad(12));
				Add(/*dive retreat chance = */ new TI.Real());
			}
			#endregion
		};
		#endregion

		#region character_cover_block
		partial class character_cover_block
		{
			#region Ctor
			public character_cover_block() : base(17)
			{
				Add(/*cover flags = */ new TI.Flags());
				Add(/*hide behind cover time = */ new TI.RealBounds());
				Add(new TI.UselessPad(4));
				Add(/*Cover vitality threshold = */ new TI.Real());
				Add(/*Cover shield fraction = */ new TI.Real());
				Add(/*Cover check delay = */ new TI.Real());
				Add(/*Emerge from cover when shield fraction reaches threshold = */ new TI.Real());
				Add(/*Cover danger threshold = */ new TI.Real());
				Add(/*Danger upper threshold = */ new TI.Real());
				// Explanation here
				Add(/*Cover chance = */ new TI.RealBounds());
				Add(/*Proximity self-preserve = */ new TI.Real());
				Add(/*Disallow cover distance = */ new TI.Real());
				Add(new TI.UselessPad(16));
				Add(/*proximity melee distance = */ new TI.Real());
				Add(new TI.UselessPad(8));
				Add(/*unreachable enemy danger threshold = */ new TI.Real());
				Add(/*scary target threshold = */ new TI.Real());
			}
			#endregion
		};
		#endregion

		#region character_retreat_block
		partial class character_retreat_block
		{
			#region Ctor
			public character_retreat_block() : base(21)
			{
				Add(/*Retreat flags = */ new TI.Flags());
				Add(/*Shield threshold = */ new TI.Real());
				Add(/*Scary target threshold = */ new TI.Real());
				Add(/*Danger threshold = */ new TI.Real());
				Add(/*Proximity threshold = */ new TI.Real());
				Add(new TI.UselessPad(16));
				Add(/*min/max forced cower time bounds = */ new TI.RealBounds());
				Add(/*min/max cower timeout bounds = */ new TI.RealBounds());
				Add(new TI.UselessPad(12));
				Add(/*proximity ambush threshold = */ new TI.Real());
				Add(/*awareness ambush threshold = */ new TI.Real());
				Add(new TI.UselessPad(24));
				Add(/*leader dead retreat chance = */ new TI.Real());
				Add(/*peer dead retreat chance = */ new TI.Real());
				Add(/*second peer dead retreat chance = */ new TI.Real());
				Add(new TI.UselessPad(12));
				Add(/*zig-zag angle = */ new TI.Real(TI.FieldType.Angle));
				Add(/*zig-zag period = */ new TI.Real());
				Add(new TI.UselessPad(8));
				Add(/*retreat grenade chance = */ new TI.Real());
				Add(/*backup weapon = */ new TI.TagReference(this, TagGroups.weap));
			}
			#endregion
		};
		#endregion

		#region character_search_block
		partial class character_search_block
		{
			#region Ctor
			public character_search_block() : base(5)
			{
				Add(/*Search flags = */ new TI.Flags());
				Add(new TI.UselessPad(24));
				Add(/*search time = */ new TI.RealBounds());
				Add(new TI.UselessPad(24));
				Add(/*Uncover distance bounds = */ new TI.RealBounds());
			}
			#endregion
		};
		#endregion

		#region character_presearch_block
		partial class character_presearch_block
		{
			#region Ctor
			public character_presearch_block() : base(6)
			{
				Add(/*Pre-search flags = */ new TI.Flags());
				Add(/*min presearch time = */ new TI.RealBounds());
				Add(/*max presearch time = */ new TI.RealBounds());
				Add(/*Min certainty radius = */ new TI.Real());
				Add(/*DEPRECATED = */ new TI.Real());
				Add(/*min suppressing time = */ new TI.RealBounds());
			}
			#endregion
		};
		#endregion

		#region character_idle_block
		partial class character_idle_block
		{
			#region Ctor
			public character_idle_block() : base(3)
			{
				Add(new TI.Pad(4));
				Add(new TI.UselessPad(24));
				Add(/*idle pose delay time = */ new TI.RealBounds());
			}
			#endregion
		};
		#endregion

		#region character_vocalization_block
		partial class character_vocalization_block
		{
			#region Ctor
			public character_vocalization_block() : base(2)
			{
				Add(/*look comment time = */ new TI.Real());
				Add(/*look long comment time = */ new TI.Real());
			}
			#endregion
		};
		#endregion

		#region character_boarding_block
		partial class character_boarding_block
		{
			#region Ctor
			public character_boarding_block() : base(5)
			{
				Add(/*flags = */ new TI.Flags());
				Add(/*max distance = */ new TI.Real());
				Add(/*abort distance = */ new TI.Real());
				Add(new TI.UselessPad(12));
				Add(/*max speed = */ new TI.Real());
			}
			#endregion
		};
		#endregion

		#region character_boss_block
		partial class character_boss_block
		{
			#region Ctor
			public character_boss_block() : base(4)
			{
				Add(new TI.Pad(4));
				Add(new TI.UselessPad(36));
				Add(/*flurry damage threshold = */ new TI.Real());
				Add(/*flurry time = */ new TI.Real());
			}
			#endregion
		};
		#endregion

		#region character_firing_pattern_block
		partial class character_firing_pattern_block
		{
			#region Ctor
			public character_firing_pattern_block() : base(14)
			{
				Add(/*rate of fire = */ new TI.Real());
				Add(/*target tracking = */ new TI.Real());
				Add(/*target leading = */ new TI.Real());
				Add(/*burst origin radius = */ new TI.Real());
				Add(/*burst origin angle = */ new TI.Real(TI.FieldType.Angle));
				Add(/*burst return length = */ new TI.RealBounds());
				Add(/*burst return angle = */ new TI.Real(TI.FieldType.Angle));
				Add(/*burst duration = */ new TI.RealBounds());
				Add(/*burst separation = */ new TI.RealBounds());
				Add(/*weapon damage modifier = */ new TI.Real());
				Add(/*projectile error = */ new TI.Real(TI.FieldType.Angle));
				Add(new TI.UselessPad(12));
				Add(/*burst angular velocity = */ new TI.Real(TI.FieldType.Angle));
				Add(/*maximum error angle = */ new TI.Real(TI.FieldType.Angle));
			}
			#endregion
		};
		#endregion

		#region character_weapons_block
		partial class character_weapons_block
		{
			#region Ctor
			public character_weapons_block() : base(44)
			{
				Add(/*weapons flags = */ new TI.Flags());
				Add(/*weapon = */ new TI.TagReference(this, TagGroups.weap));
				Add(new TI.UselessPad(24));
				Add(/*maximum firing range = */ new TI.Real());
				Add(/*minimum firing range = */ new TI.Real());
				Add(/*normal combat range = */ new TI.RealBounds());
				Add(/*bombardment range = */ new TI.Real());
				Add(/*Max special target distance = */ new TI.Real());
				Add(/*timid combat range = */ new TI.RealBounds());
				Add(/*aggressive combat range = */ new TI.RealBounds());
				Add(/*super-ballistic range = */ new TI.Real());
				Add(/*Ballistic firing bounds = */ new TI.RealBounds());
				Add(/*Ballistic fraction bounds = */ new TI.RealBounds());
				Add(new TI.UselessPad(24));
				Add(/*first burst delay time = */ new TI.RealBounds());
				Add(/*surprise delay time = */ new TI.Real());
				Add(/*surprise fire-wildly time = */ new TI.Real());
				Add(/*death fire-wildly chance = */ new TI.Real());
				Add(/*death fire-wildly time = */ new TI.Real());
				Add(new TI.UselessPad(12));
				Add(/*custom stand gun offset = */ new TI.RealVector3D());
				Add(/*custom crouch gun offset = */ new TI.RealVector3D());
				Add(new TI.UselessPad(12));
				Add(/*special-fire mode = */ new TI.Enum());
				Add(/*special-fire situation = */ new TI.Enum());
				Add(/*special-fire chance = */ new TI.Real());
				Add(/*special-fire delay = */ new TI.Real());
				Add(/*special damage modifier = */ new TI.Real());
				Add(/*special projectile error = */ new TI.Real(TI.FieldType.Angle));
				Add(new TI.UselessPad(24));
				Add(/*drop weapon loaded = */ new TI.RealBounds());
				Add(/*drop weapon ammo = */ new TI.ShortIntegerBounds());
				Add(new TI.UselessPad(24));
				Add(/*normal accuracy bounds = */ new TI.RealBounds());
				Add(/*normal accuracy time = */ new TI.Real());
				Add(new TI.UselessPad(4));
				Add(/*heroic accuracy bounds = */ new TI.RealBounds());
				Add(/*heroic accuracy time = */ new TI.Real());
				Add(new TI.UselessPad(4));
				Add(/*legendary accuracy bounds = */ new TI.RealBounds());
				Add(/*legendary accuracy time = */ new TI.Real());
				Add(new TI.UselessPad(4 + 48));
				Add(/*firing patterns = */ new TI.Block<character_firing_pattern_block>(this, 2));
				Add(/*weapon melee damage = */ new TI.TagReference(this, TagGroups.jpt_));
			}
			#endregion
		};
		#endregion

		#region character_firing_pattern_properties_block
		partial class character_firing_pattern_properties_block
		{
			#region Ctor
			public character_firing_pattern_properties_block() : base(3)
			{
				Add(/*weapon = */ new TI.TagReference(this, TagGroups.weap));
				Add(new TI.UselessPad(24));
				Add(/*firing patterns = */ new TI.Block<character_firing_pattern_block>(this, 2));
			}
			#endregion
		};
		#endregion

		#region character_grenades_block
		partial class character_grenades_block
		{
			#region Ctor
			public character_grenades_block() : base(19)
			{
				Add(/*grenades flags = */ new TI.Flags());
				Add(/*grenade type = */ new TI.Enum());
				Add(/*trajectory type = */ new TI.Enum());
				Add(new TI.Pad(2));
				Add(/*minimum enemy count = */ new TI.ShortInteger());
				Add(/*enemy radius = */ new TI.Real());
				Add(/*grenade ideal velocity = */ new TI.Real());
				Add(/*grenade velocity = */ new TI.Real());
				Add(/*grenade ranges = */ new TI.RealBounds());
				Add(/*collateral damage radius = */ new TI.Real());
				Add(/*grenade chance = */ new TI.Real(TI.FieldType.RealFraction));
				Add(/*grenade throw delay = */ new TI.Real());
				Add(new TI.UselessPad(4));
				Add(/*grenade uncover chance = */ new TI.Real(TI.FieldType.RealFraction));
				Add(new TI.UselessPad(4));
				Add(/*anti-vehicle grenade chance = */ new TI.Real(TI.FieldType.RealFraction));
				Add(new TI.UselessPad(4));
				// Explanation here
				Add(/*grenade count = */ new TI.ShortIntegerBounds());
				Add(/*dont drop grenades chance = */ new TI.Real());
			}
			#endregion
		};
		#endregion

		#region character_vehicle_block
		partial class character_vehicle_block
		{
			#region Ctor
			public character_vehicle_block() : base(53)
			{
				Add(/*unit = */ new TI.TagReference(this, TagGroups.unit));
				Add(/*style = */ new TI.TagReference(this, TagGroups.styl));
				Add(new TI.UselessPad(32));
				Add(/*vehicle flags = */ new TI.Flags());
				Add(new TI.UselessPad(8));
				Add(/*ai pathfinding radius = */ new TI.Real());
				Add(/*ai destination radius = */ new TI.Real());
				Add(/*ai deceleration distanceworld units = */ new TI.Real());
				Add(new TI.UselessPad(8));
				Add(/*ai turning radius = */ new TI.Real());
				Add(/*ai inner turning radius (< tr) = */ new TI.Real());
				Add(/*ai ideal turning radius (> tr) = */ new TI.Real());
				Add(new TI.UselessPad(8));
				Add(/*ai banshee steering maximum = */ new TI.Real(TI.FieldType.Angle));
				Add(/*ai max steering angle = */ new TI.Real());
				Add(/*ai max steering delta = */ new TI.Real());
				Add(/*ai oversteering scale = */ new TI.Real());
				Add(/*ai oversteering bounds = */ new TI.RealBounds(TI.FieldType.AngleBounds));
				Add(/*ai sideslip distance = */ new TI.Real());
				Add(/*ai avoidance distance = */ new TI.Real());
				Add(/*ai min urgency = */ new TI.Real());
				Add(new TI.UselessPad(4));
				Add(/*ai throttle maximum = */ new TI.Real());
				Add(/*ai goal min throttle scale = */ new TI.Real());
				Add(/*ai turn min throttle scale = */ new TI.Real());
				Add(/*ai direction min throttle scale = */ new TI.Real());
				Add(/*ai acceleration scale = */ new TI.Real());
				Add(/*ai throttle blend = */ new TI.Real());
				Add(/*theoretical max speed = */ new TI.Real());
				Add(/*error scale = */ new TI.Real());
				Add(new TI.UselessPad(8));
				Add(/*ai allowable aim deviation angle = */ new TI.Real(TI.FieldType.Angle));
				Add(new TI.UselessPad(12));
				Add(/*ai charge tight angle distance = */ new TI.Real());
				Add(/*ai charge tight angle = */ new TI.Real());
				Add(/*ai charge repeat timeout = */ new TI.Real());
				Add(/*ai charge look-ahead time = */ new TI.Real());
				Add(/*ai charge consider distance = */ new TI.Real());
				Add(/*ai charge abort distance = */ new TI.Real());
				Add(new TI.UselessPad(4));
				Add(/*vehicle ram timeout = */ new TI.Real());
				Add(/*ram paralysis time = */ new TI.Real());
				Add(new TI.UselessPad(4));
				Add(/*ai cover damage threshold = */ new TI.Real());
				Add(/*ai cover min distance = */ new TI.Real());
				Add(/*ai cover time = */ new TI.Real());
				Add(/*ai cover min boost distance = */ new TI.Real());
				Add(/*turtling recent damage threshold = */ new TI.Real());
				Add(/*turtling min time = */ new TI.Real());
				Add(/*turtling timeout = */ new TI.Real());
				Add(new TI.UselessPad(8));
				Add(/*obstacle ignore size = */ new TI.Enum());
				Add(new TI.Pad(2));
			}
			#endregion
		};
		#endregion

		#region Ctor
		public character_group() : base(38)
		{
			Add(Flags = new TI.Flags());
			Add(new TI.UselessPad(24));
			Add(ParentCharacter = new TI.TagReference(this, TagGroups.char_));
			Add(Unit = new TI.TagReference(this, TagGroups.unit));
			Add(Creature = new TI.TagReference(this, TagGroups.crea));
			Add(Style = new TI.TagReference(this, TagGroups.styl));
			Add(new TI.UselessPad(16));
			Add(MajorCharacter = new TI.TagReference(this, TagGroups.char_));
			Add(new TI.UselessPad(12));
			Add(Variants = new TI.Block<character_variants_block>(this, 64));
			Add(new TI.UselessPad(36));
			Add(GeneralProperties = new TI.Block<character_general_block>(this, 1));
			Add(VitalityProperties = new TI.Block<character_vitality_block>(this, 1));
			Add(PlacementProperties = new TI.Block<character_placement_block>(this, 1));
			Add(PerceptionProperties = new TI.Block<character_perception_block>(this, 4));
			Add(LookProperties = new TI.Block<character_look_block>(this, 1));
			Add(MovementProperties = new TI.Block<character_movement_block>(this, 1));
			Add(SwarmProperties = new TI.Block<character_swarm_block>(this, 3));
			Add(new TI.UselessPad(36));
			Add(ReadyProperties = new TI.Block<character_ready_block>(this, 3));
			Add(EngageProperties = new TI.Block<character_engage_block>(this, 3));
			Add(/*charge properties = */ new TI.Block<character_charge_block>(this, 3));
			Add(/*evasion properties = */ new TI.Block<character_evasion_block>(this, 3));
			Add(/*cover properties = */ new TI.Block<character_cover_block>(this, 3));
			Add(/*retreat properties = */ new TI.Block<character_retreat_block>(this, 3));
			Add(/*search properties = */ new TI.Block<character_search_block>(this, 3));
			Add(/*pre-search properties = */ new TI.Block<character_presearch_block>(this, 3));
			Add(/*idle properties = */ new TI.Block<character_idle_block>(this, 3));
			Add(/*vocalization properties = */ new TI.Block<character_vocalization_block>(this, 1));
			Add(/*boarding properties = */ new TI.Block<character_boarding_block>(this, 1));
			Add(new TI.UselessPad(12));
			Add(/*boss properties = */ new TI.Block<character_boss_block>(this, 1));
			Add(/*weapons properties = */ new TI.Block<character_weapons_block>(this, 100));
			Add(/*firing pattern properties = */ new TI.Block<character_firing_pattern_properties_block>(this, 100));
			Add(new TI.UselessPad(24));
			Add(/*grenades properties = */ new TI.Block<character_grenades_block>(this, 10));
			Add(new TI.UselessPad(24));
			Add(/*vehicle properties = */ new TI.Block<character_vehicle_block>(this, 100));
		}
		#endregion
	};
	#endregion

	#region dialogue
	partial class dialogue_group
	{
		#region sound_references_block
		partial class sound_references_block
		{
			#region Ctor
			public sound_references_block() : base(4)
			{
				Add(/*flags = */ new TI.Flags(TI.FieldType.WordFlags));
				Add(new TI.Pad(2));
				Add(/*vocalization = */ new TI.StringId());
				Add(/*sound = */ new TI.TagReference(this, TagGroups.snd_));
			}
			#endregion
		};
		#endregion

		#region Ctor
		public dialogue_group() : base(4)
		{
			Add(/*global dialogue info = */ new TI.TagReference(this, TagGroups.adlg));
			Add(/*flags = */ new TI.Flags());
			Add(/*vocalizations = */ new TI.Block<sound_references_block>(this, 500));
			Add(/*mission dialogue designator = */ new TI.StringId());
		}
		#endregion
	};
	#endregion

	#region style
	partial class style_group
	{
		#region special_movement_block
		partial class special_movement_block
		{
			public special_movement_block() : base(1)
			{
				Add(/*Special movement 1 = */ new TI.Flags());
			}
		};
		#endregion

		#region behavior_names_block
		partial class behavior_names_block
		{
			public behavior_names_block() : base(1)
			{
				Add(/*behavior name = */ new TI.String());
			}
		};
		#endregion

		#region Ctor
		public style_group() : base(28)
		{
			Add(/*name = */ new TI.String());
			Add(/*Combat status decay options = */ new TI.Enum());
			Add(new TI.Pad(2));
			Add(/*Attitude = */ new TI.Enum());
			Add(new TI.Pad(2));
			Add(/*engage attitude = */ new TI.Enum(TI.FieldType.ByteEnum));
			Add(/*evasion attitude = */ new TI.Enum(TI.FieldType.ByteEnum));
			Add(/*cover attitude = */ new TI.Enum(TI.FieldType.ByteEnum));
			Add(/*search attitude = */ new TI.Enum(TI.FieldType.ByteEnum));
			Add(/*presearch attitude = */ new TI.Enum(TI.FieldType.ByteEnum));
			Add(/*retreat attitude = */ new TI.Enum(TI.FieldType.ByteEnum));
			Add(/*charge attitude = */ new TI.Enum(TI.FieldType.ByteEnum));
			Add(/*ready attitude = */ new TI.Enum(TI.FieldType.ByteEnum));
			Add(/*idle attitude = */ new TI.Enum(TI.FieldType.ByteEnum));
			Add(/*weapon attitude = */ new TI.Enum(TI.FieldType.ByteEnum));
			Add(/*swarm attitude = */ new TI.Enum(TI.FieldType.ByteEnum));
			Add(new TI.Pad(1));
			Add(new TI.UselessPad(24));
			Add(/*Style control = */ new TI.Flags());
			Add(/*Behaviors1 = */ new TI.Flags());
			Add(/*Behaviors2 = */ new TI.Flags());
			Add(/*Behaviors3 = */ new TI.Flags());
			Add(/*Behaviors4 = */ new TI.Flags());
			Add(/*Behaviors5 = */ new TI.Flags());
			Add(new TI.UselessPad(12));
			Add(/*Special movement = */ new TI.Block<special_movement_block>(this, 1));
			Add(new TI.UselessPad(60));
			Add(/*Behavior list = */ new TI.Block<behavior_names_block>(this, 160));
		}
		#endregion
	};
	#endregion
};