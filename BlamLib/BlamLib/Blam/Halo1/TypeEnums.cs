/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace BlamLib.Blam.Halo1
{
	public static class TypeEnums
	{
		public enum ObjectType
		{
			Object,
			Device,
			Item,
			Unit,
			Placeholder,
			Projectile,
			Scenery,
			SoundScenery,
			DeviceControl,
			DeviceLightFixture,
			DeviceMachine,
			Equipment,
			Garbage,
			Weapon,
			Biped,
			Vehicle
		}

		public enum LevelOfDetailEnum
		{
			SuperHigh,
			High,
			Medium,
			Low,
			SuperLow
		}
	}
}
