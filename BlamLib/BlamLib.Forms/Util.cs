/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
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