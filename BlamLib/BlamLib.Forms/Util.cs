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
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Text;
using System.Windows.Forms;

namespace BlamLib.Forms
{
	/// <summary>
	/// Utility class for Windows.Form related classes
	/// </summary>
	public static class Util
	{
		#region Menu Creation
		static void MenuItemParseName(ToolStripMenuItem item, ref string value)
		{
			const int kMarkupCount = 5;

			var sb = new StringBuilder(value, value.Length + (2 * kMarkupCount));
			sb = sb.Replace("!", "\n!\n").Replace("^", "\n^\n").Replace("*", "\n*\n").Replace(":", "\n:\n").Replace("#", "\n#\n");
			string[] parse = BlamLib.Util.ParseRegEx(sb.ToString(), "\n");

			item.Text = parse[0];

			for (int x = 1; x < parse.Length; x++)
			{
				switch (parse[x][0])
				{
					case '!':
						item.Visible = false;
						break;

					case '*':
						item.Enabled = false;
						break;

					case '^':
						item.Checked = Boolean.Parse(parse[x + 1]);
						break;

					case ':':
						item.ShortcutKeys = (System.Windows.Forms.Keys)System.Enum.Parse(typeof(System.Windows.Forms.Keys), parse[x + 1], true);
						break;

					case '#':
						item.ToolTipText = parse[x + 1];
						break;
				}
			}
		}

		/// <summary>
		/// ! - Makes the menu invisible
		/// * - Makes the menu not enabled
		/// ^ - Makes the menu checked (text after this gets parsed as a boolean)
		/// : - text after this gets parsed as a Keys enum string
		/// # - text after this sets the menu's tooltip text
		/// </summary>
		/// <param name="name"></param>
		/// <param name="OnClick"></param>
		/// <returns></returns>
		public static ToolStripMenuItem CreateMenuItem(string name, System.EventHandler OnClick)
		{
			ToolStripMenuItem value = new ToolStripMenuItem();
			value.Click += OnClick;
			MenuItemParseName(value, ref name);
			return value;
		}
		#endregion
	};
}