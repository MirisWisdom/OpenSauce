using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Runtime.InteropServices;
using System.Collections;
using System.Reflection;

namespace OpenSauceIDE.Controls
{
	// Largely taken from "Building a Better ComboBox" http://msdn.microsoft.com/en-us/library/ms996411 by George Politis
	public partial class OSIDEComboBox : ComboBox
	{
		[DllImport("user32.dll")]
		static extern bool SetWindowPos(IntPtr hWnd, IntPtr hWndInsertAfter, int X, int Y, int cx, int cy, uint uFlags);

		private const int SWP_NOSIZE = 0x1;
		private const uint WM_CTLCOLORLISTBOX = 0x0134;


		[Category("OSIDE"), DefaultValue(0)]
		public uint MinimumDropDownWidth { get; set; }

		[Category("OSIDE"), DefaultValue(0)]
		public uint MaximumDropDownWidth { get; set; }

		[Category("OSIDE"), DefaultValue(0)]
		public uint DropDownWidthPadding { get; set; }

		public OSIDEComboBox()
		{
			InitializeComponent();

			this.HandleCreated += new EventHandler(OSIDEComboBox_HandleCreated);
		}

		void OSIDEComboBox_HandleCreated(object sender, EventArgs e)
		{
			UpdateDropDownWidth();
		}

		private int GetMaxItemLength()
		{
			// create a GDI+ drawing surface to measure string widths
			System.Drawing.Graphics drawing_surface = this.CreateGraphics();

			int max_length = 0;
			if ((this.DataSource != null) && (this.DisplayMember != ""))
			{
				IEnumerable enumerable_list = null;

				// get the list objects enumerable interface
				IListSource ilist_source = this.DataSource as IListSource;
				if (ilist_source != null)
					enumerable_list = ilist_source.GetList();
				else
					enumerable_list = this.DataSource as IEnumerable;

				// get an enumerator for the list
				IEnumerator enumerator = enumerable_list.GetEnumerator();

				// if the lsit is empty, return 0
				if (!enumerator.MoveNext())
				{
					drawing_surface.Dispose();
					return 0;
				}

				// get the type and property infor for the lists element type
				Type element_type = enumerator.Current.GetType();
				PropertyInfo display_property = element_type.GetProperty(this.DisplayMember);

				// get the longest value in the list
				do
				{
					if (enumerator.Current == null)
						continue;

					string value_string = display_property.GetValue(enumerator.Current, null).ToString();
					max_length = Math.Max(max_length, (int)drawing_surface.MeasureString(value_string, this.Font).Width);
				} while (enumerator.MoveNext());
			}
			else
			{
				foreach (var item in Items)
					max_length = Math.Max(max_length, (int)drawing_surface.MeasureString(item.ToString(), this.Font).Width);
			}
			drawing_surface.Dispose();
			return max_length;
		}

		public void UpdateDropDownWidth()
		{
			// float to hold largest single item width
			float max_item_width = 0;
			max_item_width = GetMaxItemLength();

			//Add a buffer for some white space
			//around the text
			if (DropDownWidthPadding == 0)
				max_item_width += 30;
			else
				max_item_width += DropDownWidthPadding;

			//round max_item_width and cast to an int
			int new_dropdown_width = (int)Decimal.Round((decimal)max_item_width, 0);
			
			if (MinimumDropDownWidth != 0)
				new_dropdown_width = (int)Math.Max(new_dropdown_width, MinimumDropDownWidth);
			if (MaximumDropDownWidth != 0)
				new_dropdown_width = (int)Math.Min(new_dropdown_width, MaximumDropDownWidth);

			//If the width is bigger than the screen, ensure
			//we stay within the bounds of the screen
			new_dropdown_width = (int)Math.Min(new_dropdown_width, Screen.GetWorkingArea(this).Width);

			this.DropDownWidth = new_dropdown_width;
		}

		//TODO: add an anchor option
		protected override void WndProc(ref Message m)
		{
			if (m.Msg == WM_CTLCOLORLISTBOX)
			{
				// Make sure we are inbounds of the screen
				int left = this.PointToScreen(new Point(0, 0)).X;

				//Only do this if the dropdown is going off right edge of screen
				if (this.DropDownWidth > Screen.PrimaryScreen.WorkingArea.Width - left)
				{
					// Get the current combo position and size
					Rectangle comboRect = this.RectangleToScreen(this.ClientRectangle);

					int dropHeight = 0;
					int topOfDropDown = 0;
					int leftOfDropDown = 0;

					//Calculate dropped list height
					for (int i = 0; (i < this.Items.Count && i < this.MaxDropDownItems); i++)
					{
						dropHeight += this.ItemHeight;
					}

					//Set top position of the dropped list if 
					//it goes off the bottom of the screen
					if (dropHeight > Screen.PrimaryScreen.WorkingArea.Height -
					   this.PointToScreen(new Point(0, 0)).Y)
					{
						topOfDropDown = comboRect.Top - dropHeight - 2;
					}
					else
					{
						topOfDropDown = comboRect.Bottom;
					}

					//Calculate shifted left position
					leftOfDropDown = comboRect.Left - (this.DropDownWidth -
					   (Screen.PrimaryScreen.WorkingArea.Width - left));

					// Postioning/sizing the drop-down
					//SetWindowPos(HWND hWnd,
					//      HWND hWndInsertAfter,
					//      int X,
					//      int Y,
					//      int cx,
					//      int cy,
					//      UINT uFlags);
					//when using the SWP_NOSIZE flag, cx and cy params are ignored
					SetWindowPos(m.LParam,
					   IntPtr.Zero,
					   leftOfDropDown,
					   topOfDropDown,
					   0,
					   0,
					   SWP_NOSIZE);
				}
			}

			base.WndProc(ref m);
		}
	}
}
