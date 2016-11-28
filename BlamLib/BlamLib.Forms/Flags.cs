/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
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