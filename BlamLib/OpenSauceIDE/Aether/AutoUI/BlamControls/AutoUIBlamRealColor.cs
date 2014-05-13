using System;
using OpenSauceIDE.Aether.AutoUI.Controls;
using System.Windows.Forms;
using System.ComponentModel;
using System.Drawing;

namespace OpenSauceIDE.Aether.AutoUI.BlamControls
{
	/// <summary>	An automatic user interface blam real color control. </summary>
	public partial class AutoUIBlamRealColor : UserControl, IAutoUIControl, INotifyPropertyChanged
	{
		private BlamLib.TagInterface.RealColor mSourceColor = null;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets or sets the current color. </summary>
		///
		/// <value>	The color of the current. </value>
		public Color CurrentColor
		{
			get
			{
				if(mSourceColor == null)
				{
					return Color.White;
				}
				else
				{
					return mSourceColor.ToColor();
				}
			}
			set
			{
				if (mSourceColor == null)
				{
					return;
				}
				else
				{
					mSourceColor.ToArgbColor(value.A, value.R, value.G, value.B);
				}
			}
		}

		/// <summary>	Initializes a new instance of the AutoUIBlamRealColor class. </summary>
		public AutoUIBlamRealColor()
		{
			InitializeComponent();

			ColorFields.SetControls(new string[] { "Alpha", "Red", "Green", "Blue" });

			ColorButton.DataBindings.Add("BackColor", this, "CurrentColor", false, DataSourceUpdateMode.OnPropertyChanged);
		}

		#region IAutoUIControl Members
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets information describing the target member. </summary>
		///
		/// <value>	Information describing the target member. </value>
		public IAutoUIMemberInfo MemberInfo { get; private set; }

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Sets the target member info for the control. </summary>
		///
		/// <exception cref="Exception">	Thrown when an exception error condition occurs. </exception>
		///
		/// <param name="memberInfo">	Information describing the target member. </param>
		public void SetMemberInfo(IAutoUIMemberInfo memberInfo)
		{
			if (memberInfo.MemberValueType != typeof(BlamLib.TagInterface.RealColor))
			{
				throw new Exception("AutoUIBlamRealColor was not passed a real color member info");
			}

			MemberInfo = memberInfo;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Sets the source object to bind the control to. </summary>
		///
		/// <exception cref="Exception">	Thrown if the expected field value is invalid. </exception>
		///
		/// <param name="sourceObject">	The source object to bind to. </param>
		public void SetSource(object sourceObject)
		{
			// Get the color values
			var value = MemberInfo.GetValue(sourceObject);

			if (mSourceColor != null)
			{
				mSourceColor.PropertyChanged -= OnSourceColorChanged;
			}

			if (value == null)
			{
				mSourceColor = null;

				ColorFields.SetControls(null);
				Enabled = false;
				return;
			}
			else
			{
				Enabled = MemberInfo.CanWrite;
			}

			if (!(value is BlamLib.TagInterface.RealColor))
			{
				throw new Exception("The value returned by a AutoUIBlamRealColor member was not a BlamLib.TagInterface.RealColor");
			}
			
			// Attatch to the colour field changed event
			mSourceColor = value as BlamLib.TagInterface.RealColor;
			mSourceColor.PropertyChanged += OnSourceColorChanged;

			// Add a different control set depending on what type of colour the field is
			switch (mSourceColor.FieldType)
			{
				case BlamLib.TagInterface.FieldType.RealArgbColor:
					ColorFields.SetControls(new string[] { "A", "R", "G", "B" });
					break;
				case BlamLib.TagInterface.FieldType.RealRgbColor:
					ColorFields.SetControls(new string[] { "R", "G", "B" });
					break;
				default:
					throw new Exception("A RealColor field has an invalid field type");
			}

			ColorFields.SetMemberInfo(MemberInfo);
			ColorFields.SetSource(sourceObject);
			OnPropertyChanged("CurrentColor");
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Raises the property changed event for CurrentColor. </summary>
		///
		/// <param name="sender">	Source of the event. </param>
		/// <param name="e">	 	Event information to send to registered event handlers. </param>
		void OnSourceColorChanged(object sender, PropertyChangedEventArgs e)
		{
			if ((e.PropertyName == "Alpha") ||
				(e.PropertyName == "Red") ||
				(e.PropertyName == "Green") ||
				(e.PropertyName == "Blue"))
			{
				OnPropertyChanged("CurrentColor");
			}
		}
		#endregion

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Event handler. Called by ColorButton for click events. </summary>
		///
		/// <param name="sender">	Source of the event. </param>
		/// <param name="e">	 	Event information. </param>
		private void ColorButton_Click(object sender, EventArgs e)
		{
			System.Windows.Forms.ColorDialog dialog = new ColorDialog();

			dialog.AllowFullOpen = false;
			dialog.Color = CurrentColor;
			dialog.ShowHelp = false;
			dialog.SolidColorOnly = false;

			if(dialog.ShowDialog() == DialogResult.OK)
			{
				CurrentColor = dialog.Color;
			}
		}

		#region INotifyPropertyChanged Members
		public event PropertyChangedEventHandler PropertyChanged;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Executes the property changed action. </summary>
		///
		/// <param name="name">	The name of the changed property. </param>
		private void OnPropertyChanged(string name)
		{
			var eventHandler = PropertyChanged;

			if (eventHandler != null)
			{
				eventHandler(this, new PropertyChangedEventArgs(name));
			}
		}
		#endregion
	}
}
