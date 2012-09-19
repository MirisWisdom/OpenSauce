using System;
using System.Collections.Generic;
using System.IO;
using System.Runtime.Serialization;
using System.Runtime.Serialization.Formatters.Binary;

namespace OpenSauceIDE.Settings
{
	static public class SettingsManager
	{
		public class SettingsVersionAttribute : Attribute
		{
			public int Version { get; set; }

			public SettingsVersionAttribute() { }
		};

		[Serializable()]
		class SettingsObject
		{
			public int SettingsVersion;
			public string ObjectTypeName;
			public object ObjectInstance;
		};

		static List<SettingsObject> SettingsObjects = null;

		/// <summary>
		/// Loads the settings objects list from the
		/// </summary>
		public static void LoadSettings()
		{
			if (File.Exists("settings.bin"))
			{
				using (FileStream settings_file = File.Open("settings.bin", FileMode.Open))
				{
					if (settings_file.Length != 0)
					{
						BinaryFormatter formatter = new BinaryFormatter();
						SettingsObjects = formatter.Deserialize(settings_file) as List<SettingsObject>;
					}
				}
			}

			if (SettingsObjects == null)
				SettingsObjects = new List<SettingsObject>();
		}

		/// <summary>
		/// Saves the settings objects list to the settings file,
		/// </summary>
		public static void SaveSettings()
		{
			if (SettingsObjects.Count == 0)
				return;

			if (File.Exists("settings.bin"))
				File.Delete("settings.bin");

			// serialize the settings objects list using a binary formatter
			using (FileStream settings_file = File.Open("settings.bin", FileMode.CreateNew))
			{
				BinaryFormatter formatter = new BinaryFormatter();
				formatter.Serialize(settings_file, SettingsObjects);
			}
		}

		/// <summary>
		/// Returns the settings version of a class
		/// </summary>
		/// <param name="settings_type">The class type to retrieve the version from</param>
		/// <returns>The settings version of a class</returns>
		private static int GetSettingsVersion(Type settings_type)
		{
			object[] attributes = settings_type.GetCustomAttributes(typeof(SettingsVersionAttribute), true);

			if ((attributes == null) || (attributes.Length == 0)) throw new Exception("The settings object has no settings version attribute");

			SettingsVersionAttribute version_attribute = attributes[0] as SettingsVersionAttribute;

			if (version_attribute == null) throw new Exception("The settings object has no settings version attribute");

			return version_attribute.Version;
		}
		
		/// <summary>
		/// Returns the settings version of a class
		/// </summary>
		/// <param name="settings_object">The class type instance to retrieve the version from</param>
		/// <returns>The settings version of a class</returns>
		private static int GetSettingsVersion(object settings_object)
		{
			return GetSettingsVersion(settings_object.GetType());
		}

		/// <summary>
		/// Adds or updates a settings entry with a new settings object
		/// </summary>
		/// <param name="settings_object">The object to add to the settings objects lsit</param>
		public static void AddSettings(object settings_object)
		{
			//is the object serializable
			if (!settings_object.GetType().IsSerializable)
				throw new Exception("The provided settings object is not serializable");

			// look for an existing settings object instance matching the object type name
			SettingsObject object_entry = SettingsObjects.Find(
				delegate(SettingsObject obj)
				{
					return obj.ObjectTypeName == settings_object.GetType().Name;
				}
			);

			int version = GetSettingsVersion(settings_object);

			// if the version does not match, delete it
			if ((object_entry != null) && (object_entry.SettingsVersion != version))
			{
				SettingsObjects.Remove(object_entry);
				object_entry = null;
			}

			// if no matching entry was found, create a new one and add it to the list
			if (object_entry == null)
			{
				object_entry = new SettingsObject();

				object_entry.ObjectTypeName = settings_object.GetType().Name;
				SettingsObjects.Add(object_entry);
			}

			// set the object instance of the settings entry
			object_entry.ObjectInstance = settings_object;
			object_entry.SettingsVersion = GetSettingsVersion(settings_object);
		}

		/// <summary>
		/// Returns an object matching the settings type if one exists in the settings objects list.
		/// </summary>
		/// <param name="settings_type">The class type to search for.</param>
		/// <returns>Returns the requesting settings object if present, otherwise returns null.</returns>
		public static object GetSettings(Type settings_type)
		{
			SettingsObject value = SettingsObjects.Find(
				delegate(SettingsObject obj)
				{
					return obj.ObjectTypeName == settings_type.Name;
				}
			);
			
			int version = GetSettingsVersion(settings_type);

			if (value == null)
				return null;
			else if (value.SettingsVersion != version)
				return null;

			return value.ObjectInstance;
		}
	}
}
