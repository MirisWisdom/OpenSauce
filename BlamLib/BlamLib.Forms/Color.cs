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