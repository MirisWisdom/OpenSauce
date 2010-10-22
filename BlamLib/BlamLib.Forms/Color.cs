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
	public partial class Color : BlamLib.Forms.Field
	{
		public static Point[] ARGB = new Point[] 
		{
			new Point(168, 0),// A Label
			new Point(184, 0),// A
			new Point(256, 0),// R Label
			new Point(272, 0),// R
			new Point(344, 0),// G Label
			new Point(360, 0),// G
			new Point(432, 0),// B Label
			new Point(448, 0) // B
		};


		public byte Field_A
		{
			get { try { return Convert.ToByte(field_A.Text); } catch { return 0; } }
			set { field_A.Text = value.ToString(); }
		}

		public byte Field_R
		{
			get { try { return Convert.ToByte(field_R.Text); } catch { return 0; } }
			set { field_R.Text = value.ToString(); }
		}

		public byte Field_G
		{
			get { try { return Convert.ToByte(field_G.Text); } catch { return 0; } }
			set { field_G.Text = value.ToString(); }
		}

		public byte Field_B
		{
			get { try { return Convert.ToByte(field_B.Text); } catch { return 0; } }
			set { field_B.Text = value.ToString(); }
		}

		public Color()
		{
			InitializeComponent();

			_Setup(name);
		}

		public override void Clear()
		{
			this.field_R.Text = "";
			this.field_G.Text = "";
			this.field_B.Text = "";
			this.field_A.Text = "";
			this.colorButton.ForeColor = SystemColors.Control;
			this.colorButton.BackColor = colorButton.ForeColor;
		}

		private void OnChangeColor(object sender, EventArgs e)
		{
			colorDialog.Color = System.Drawing.Color.FromArgb(Field_A, Field_R, Field_G, Field_B);

			colorButton.BackColor = colorDialog.Color;
			colorButton.ForeColor = colorDialog.Color;

			// Update the color button if the user clicks OK 
			if (colorDialog.ShowDialog() == DialogResult.OK)
			{
				Field_A = colorDialog.Color.A;
				Field_R = colorDialog.Color.R;
				Field_G = colorDialog.Color.G;
				Field_B = colorDialog.Color.B;
				colorButton.BackColor = colorDialog.Color;
				colorButton.ForeColor = colorDialog.Color;
			}
		}

		private void OnChanged(object sender, EventArgs e) { try { colorDialog.Color = System.Drawing.Color.FromArgb(Field_A, Field_R, Field_G, Field_B); } catch (Exception) { } }

		public override void AddEventHandlers(params object[] handlers)
		{
			EventHandler eh = (handlers[0] as EventHandler);
			field_A.TextChanged += eh;
			field_R.TextChanged += eh;
			field_G.TextChanged += eh;
			field_B.TextChanged += eh;
		}
	};
}