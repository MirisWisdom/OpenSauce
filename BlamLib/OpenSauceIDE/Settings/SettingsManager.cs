/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.IO;
using System.Runtime.Serialization;
using System.Runtime.Serialization.Formatters.Binary;

namespace OpenSauceIDE.Settings
{
	public class SettingsObjectBase : INotifyPropertyChanged
	{
		#region Fields
		protected int mClassVersion = 0x00000001;
		protected PropertyNode mParentNode;

		protected PropertyNode this[string name]
		{
			get
			{
				return mParentNode.Get(name);
			}
		}
		#endregion

		public int Version
		{
			get
			{
				return this["Version"].GetValue<int>();
			}
		}

		public SettingsObjectBase(PropertyNode parentNode)
		{
			mParentNode = parentNode;

			if (!mParentNode.Has("Version"))
			{
				mParentNode.Put("Version", mClassVersion);
			}
		}

		#region INotifyPropertyChanged Members
		public event PropertyChangedEventHandler PropertyChanged;

		protected void OnPropertyChanged(string name)
		{
			var eventHandler = PropertyChanged;

			if (eventHandler != null)
			{
				eventHandler(this, new PropertyChangedEventArgs(name));
			}
		}
		#endregion

		protected T GetValue<T>(string nodeName)
			where T : IConvertible
		{
			return this[nodeName].GetValue<T>();
		}

		protected void SetValue<T>(string nodeName, T value)
			where T : IConvertible
		{
			this[nodeName].SetValue<T>(value);
			OnPropertyChanged(nodeName);
		}
	}

	static public class SettingsManager
	{
		private static readonly string mSettingsFilename = "settings.xml";
		private static PropertyTree mPropertyTree = null;

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Loads the settings objects list from the. </summary>
		///-------------------------------------------------------------------------------------------------
		public static void LoadSettings()
		{
			mPropertyTree = new PropertyTree('.');

			if (File.Exists(mSettingsFilename))
			{
				mPropertyTree.ReadXml(mSettingsFilename);
			}
		}

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Saves the settings objects list to the settings file,. </summary>
		///-------------------------------------------------------------------------------------------------
		public static void SaveSettings()
		{
			if (File.Exists(mSettingsFilename))
			{
				File.Delete(mSettingsFilename);
			}

			mPropertyTree.SaveXml(mSettingsFilename);
		}

		///-------------------------------------------------------------------------------------------------
		/// <summary>
		/// 	Returns an object matching the settings type if one exists in the settings objects list.
		/// </summary>
		/// <exception cref="Exception">	Thrown when an exception error condition occurs. </exception>
		/// <param name="parentPath">   	The class type to search for. </param>
		/// <param name="settingsClass">	The settings class. </param>
		/// <returns>	Returns the requesting settings object if present, otherwise returns null. </returns>
		///-------------------------------------------------------------------------------------------------
		public static object GetSettings(string parentPath, Type settingsClass)
		{
			if (!settingsClass.IsSubclassOf(typeof(SettingsObjectBase)))
			{
				throw new Exception(String.Format("Type {0} does not derive from SettingsObjectBase", settingsClass.Name));
			}

			if (settingsClass.GetConstructor(new Type[] { typeof(PropertyNode) }) == null)
			{
				throw new Exception(String.Format("Type {0} does not have a constructor taking a PropertyNode"));
			}

			PropertyNode parentNode = mPropertyTree.Get(parentPath);

			return Activator.CreateInstance(settingsClass, new object[] { parentNode });
		}
	}
}
