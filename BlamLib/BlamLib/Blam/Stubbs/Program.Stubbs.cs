/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
﻿
namespace BlamLib
{
	partial class Program
	{
		/// <summary>
		/// Stubbs the Zombie global wide values
		/// </summary>
		public static class Stubbs
		{
			#region Manager
			static Blam.Stubbs.GameDefinition manager = new Blam.Stubbs.GameDefinition();
			/// <summary>
			/// Stubbs's specific manager instance
			/// </summary>
			public static Blam.Stubbs.GameDefinition Manager	{ get { return manager; } }
			#endregion

			/// <summary>
			/// Initialize the resources used by the Stubbs systems
			/// </summary>
			public static void Initialize()
			{
				manager.Read(Managers.GameManager.GetRelativePath(BlamLib.Managers.GameManager.Namespace.Stubbs), "Stubbs.xml");
			}

			/// <summary>
			/// Close the resources used by the Stubbs' systems
			/// </summary>
			public static void Close()
			{
				manager.Close();
			}
		};
	};
}