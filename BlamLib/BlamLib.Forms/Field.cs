/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Text;
using System.Windows.Forms;

namespace BlamLib.Forms
{
	/// <summary>
	/// 
	/// </summary>
	/// <remarks>
	/// Inheriting Controls must implement the following methods:
	/// Clear
	/// </remarks>
	public partial class Field : UserControl
	{
		#region Properties
		#region ControlName
		private Control controlNameControl = null;
		/// <summary>
		/// Interacts with the control that displays the name
		/// string for this control via its <c>Text</c> property
		/// </summary>
		[DesignerSerializationVisibility(DesignerSerializationVisibility.Hidden)]
		[Browsable(false)]
		public string ControlName
		{
			get { return controlNameControl.Text; }
			set { controlNameControl.Text = value; }
		}
		#endregion

		#region Help
		/// <summary>
		/// The string that appears in this field's tooltip
		/// </summary>
		[DesignerSerializationVisibility(DesignerSerializationVisibility.Hidden)]
		[Browsable(false)]
		public string Help
		{
			get { return this.attribute.Help; }
			//set
			//{
			//    this.attribute.Help = value;
			//    PropergateHelp();
			//}
		}

		protected void PropergateHelp()
		{
			toolTip.SetToolTip(this, Help);
			foreach (Control c in this.Controls)
				toolTip.SetToolTip(c, Help);
		}
		#endregion

		#region Units
		protected Control unitsControl = null;
		/// <summary>
		/// Interacts with the control that displays the units
		/// string for this control via its <c>Text</c> property
		/// </summary>
		[DesignerSerializationVisibility(DesignerSerializationVisibility.Hidden)]
		[Browsable(false)]
		public string Units
		{
			get
			{
				return this.attribute.Units;
				//try { return unitsControl.Text; }
				//catch { Debug.LogFile.WriteLine("Tried to get {0}'s 'unit' field but it doesn't have one", ControlName); return string.Empty; }
			}
			//set
			//{
			//    try { unitsControl.Text = value; }
			//    catch { Debug.LogFile.WriteLine("Tried to set {0}'s 'unit' field but it doesn't have one", ControlName); }
			//}
		}
		#endregion

		#region DefinitionIndex
		/// <summary>
		/// The index in the definition this field
		/// is associated with
		/// </summary>
		[DesignerSerializationVisibility(DesignerSerializationVisibility.Hidden)]
		[Browsable(false)]
		public int DefinitionIndex
		{
			get { return this.attribute.DefinitionIndex; }
			//set { definitionIndex = value; }
		}
		#endregion

		#region Attribute
		private void SetupForFieldAttribute()
		{
			if(controlNameControl != null)
			{
				controlNameControl.Text = attribute.Name;
			}

			if(unitsControl != null)
			{
				unitsControl.Text = attribute.Units;
			}

			if(attribute.Help != null && attribute.Help != string.Empty)
				PropergateHelp();
		}

		protected TagInterface.FieldAttribute attribute = TagInterface.FieldAttribute.Null;
		/// <summary>
		/// Description of this field
		/// </summary>
		[DesignerSerializationVisibility(DesignerSerializationVisibility.Hidden)]
		[Browsable(false)]
		public TagInterface.FieldAttribute Attribute
		{
			get { return attribute; }
			set { attribute = value; SetupForFieldAttribute(); }
		}
		#endregion
		#endregion

		#region Ctor
		public Field() { InitializeComponent(); }

		private void _Setup()
		{
			controlNameControl.MouseHover += new EventHandler(OnMouseHover);
			controlNameControl.MouseLeave += new EventHandler(OnMouseLeave);
		}

		/// <summary>
		/// Internal method to setup the needed values
		/// </summary>
		/// <param name="name_control"></param>
		protected void _Setup(Control name_control)	{ controlNameControl = name_control; }

		/// <summary>
		/// Internal method to setup the needed values
		/// </summary>
		/// <param name="name_control"></param>
		/// <param name="units_control"></param>
		protected void _Setup(Control name_control, Control units_control)
		{
			_Setup(name_control);
			unitsControl = units_control;
		}
		#endregion

		/// <summary>
		/// Emptys the field(s) of this field control to a
		/// 'null' value
		/// </summary>
		/// <remarks>Used in a empty block</remarks>
		public virtual void Clear() { }

		/// <summary>
		/// Called when the control is changed. If the parent of this
		/// control is a Block control, then we update it with its new
		/// element field data
		/// </summary>
		protected void UpdateParent()
		{
			if(Parent != null)
			{
				try
				{
					IBlockControl parent = Parent.Parent.Parent as IBlockControl;
					if (parent.CurrentIndex != -1)
						parent.UpdateFieldData(parent.CurrentIndex);
				}
				catch { Debug.LogFile.WriteLine("Block element data update failed! {0} {1}", GetType().Name, ControlName); }
			}
		}

		/// <summary>
		/// Some would call this a hack...
		/// </summary>
		/// <remarks>[0] = Text, [1] = Block, [2] = Flag</remarks>
		/// <param name="handlers"></param>
		public virtual void AddEventHandlers(params object[] handlers) {}

		protected virtual void OnMouseHover(object sender, EventArgs e) { if (HandlerOnMouseHover != null) HandlerOnMouseHover(this, e); }

		protected virtual void OnMouseLeave(object sender, EventArgs e) { if (HandlerOnMouseLeave != null) HandlerOnMouseLeave(this, e); }

		#region SetupMouseEvents
		private static EventHandler HandlerOnMouseHover = null;
		private static EventHandler HandlerOnMouseLeave = null;
		/// <summary>
		/// Call from the application that uses this library to allow
		/// debugging of fields using mouse hoving events
		/// </summary>
		/// <param name="hover"></param>
		/// <param name="leave"></param>
		public static void SetupMouseEvents(EventHandler hover, EventHandler leave)
		{
			HandlerOnMouseHover = hover;
			HandlerOnMouseLeave = leave;
		}
		#endregion
	};
}