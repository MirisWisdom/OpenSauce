/*
    BlamLib: .NET SDK for the Blam Engine

    Copyright (C) 2005-2010  Kornner Studios (http://kornner.com)

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
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