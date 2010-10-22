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
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace BlamLib.Forms
{
	public partial class BlockIndex : BlamLib.Forms.Field
	{
		/// <summary>
		/// Returns the index its pointing to. WARNING: Fixes index internally
		/// </summary>
		public int Field
		{
			get { return field.SelectedIndex - 1; }
			set
			{
				try { field.SelectedIndex = value + 1; }
				catch (ArgumentOutOfRangeException) { throw new Debug.ExceptionLog("The \'{0}\' block index field was given a bad index #{1} ({2})", ControlName, value, field.Items.Count); }
			}
		}

		public BlockIndex()
		{
			InitializeComponent();
			_Setup(name);
		}

		public override void Clear() { field.SelectedIndex = -1; }

		public override void AddEventHandlers(params object[] handlers) { field.SelectedIndexChanged += (handlers[1] as EventHandler); }
	};
}