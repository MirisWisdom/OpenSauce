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