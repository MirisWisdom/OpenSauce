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

		public static string GetObjectTypeString(ObjectType type)
		{
			Dictionary<ObjectType, string> typeNames = new Dictionary<ObjectType,string>
			{
				{ ObjectType.Object, "Object" },
				{ ObjectType.Device, "Device" },
				{ ObjectType.Item, "Item" },
				{ ObjectType.Unit, "Unit" },
				{ ObjectType.Placeholder, "Placeholder" },
				{ ObjectType.Projectile, "Projectile" },
				{ ObjectType.Scenery, "Scenery" },
				{ ObjectType.SoundScenery, "Sound Scenery" },
				{ ObjectType.DeviceControl, "Device Control" },
				{ ObjectType.DeviceLightFixture, "Device Light Fixture" },
				{ ObjectType.DeviceMachine, "Device Machine" },
				{ ObjectType.Equipment, "Equipment" },
				{ ObjectType.Garbage, "Garbage" },
				{ ObjectType.Weapon, "Weapon" },
				{ ObjectType.Biped, "Biped" },
				{ ObjectType.Vehicle, "Vehicle" }
			};

			return typeNames[type];
		}
	}
}
