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