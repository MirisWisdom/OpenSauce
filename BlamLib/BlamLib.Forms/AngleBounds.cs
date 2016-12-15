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
	public partial class AngleBounds : BlamLib.Forms.Field
	{
		public float FieldLower
		{
			get { try { return Convert.ToSingle(field_Lower.Text); } catch { return 0; } }
			set { field_Lower.Text = value.ToString(); }
		}

		public float FieldUpper
		{
			get { try { return Convert.ToSingle(field_Upper.Text); } catch { return 0; } }
			set { field_Upper.Text = value.ToString(); }
		}

		public AngleBounds()
		{
			InitializeComponent();

			_Setup(name, units);
		}

		public override void Clear()
		{
			field_Lower.Text = "";
			field_Upper.Text = "";
		}

		public override void AddEventHandlers(params object[] handlers)
		{
			field_Lower.TextChanged += (handlers[0] as EventHandler);
			field_Upper.TextChanged += (handlers[0] as EventHandler);
		}
	};
}