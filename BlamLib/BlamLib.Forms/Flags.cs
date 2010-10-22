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
	public partial class Flags : BlamLib.Forms.Field
	{
		#region Field
		private int flags = 0;
		private bool HasFlag(int flag) { return (flags & flag) != 0; }
		private void ModifyFlags(int flags, bool add)
		{
			if (add) this.flags |= flags;
			else this.flags &= ~flags;
		}

		[DesignerSerializationVisibility(DesignerSerializationVisibility.Hidden)]
		public int Field
		{
			get { return flags; }
			set { flags = value; UpdateFlags(); }
		}

		private void UpdateFlags()
		{
			field.SuspendLayout();
			for(int x = 0; x < field.Items.Count; x++)
				field.Items[x].Checked = HasFlag(1 << x);
			field.ResumeLayout();
		}
		#endregion

		public Flags()
		{
			InitializeComponent();
			_Setup(name);
		}

		public override void Clear()
		{
			for (int x = 0; x < field.Items.Count; x++)
				field.Items[x].Checked = false;
		}

		public override void AddEventHandlers(params object[] handlers) { field.ItemChecked += (handlers[1] as ItemCheckedEventHandler); }

		private void OnItemChecked(object sender, ItemCheckedEventArgs e) { ModifyFlags(1 << e.Item.Index, e.Item.Checked); }
	};
}