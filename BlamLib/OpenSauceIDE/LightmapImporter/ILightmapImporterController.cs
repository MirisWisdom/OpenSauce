/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
using System;
using BlamLib.Messaging;
using OpenSauceIDE.LightmapImporter.Settings;

namespace OpenSauceIDE.LightmapImporter
{
	/// <summary>   Enum for the current state of the lightmap importer controller. </summary>
	public enum LightmapImporterStateEnum
	{
		ImporterReady,
		ImporterImporting
	}

	/// <summary>   Arguments for the lightmap importer controller state change. </summary>
	public class LightmapImporterStateChangeArgs : EventArgs
	{
		public LightmapImporterStateEnum State { get; private set; }

		public LightmapImporterStateChangeArgs(LightmapImporterStateEnum state)
		{
			State = state;
		}
	}

	/// <summary>	Interface for a lightmap importer controller. </summary>
	public interface ILightmapImporterController
		: IMessageSource
	{
		event EventHandler<LightmapImporterStateChangeArgs> StateChanged;

		ILightmapImporterSettings GetImporterSettings();

		void ImportLightmap();
	}
}
