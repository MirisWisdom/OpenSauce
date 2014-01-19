using System;
using System.Collections.Generic;
using System.Drawing;
using System.Windows.Forms;

namespace OpenSauceIDE.Aether.AutoUI.BlamControls
{
	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>
	/// 	An automatic user interface multi value control for creating a list of controls bound to known type members.
	/// </summary>
	///
	/// <typeparam name="T">	Generic type parameter. </typeparam>
	public partial class AutoUIBlamMultiValue<T> : System.Windows.Forms.FlowLayoutPanel, AutoUI.Controls.IAutoUIControl
		where T : Control, AutoUI.Controls.IAutoUIControl, new()
	{
		/// <summary>	Struct defining a member and control binding. </summary>
		private struct ControlInstance
		{
			public string BindToMember;
			public T BlamControl;
		}

		private List<ControlInstance> mControlInstances = new List<ControlInstance>();

		#region Constructor
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Initializes a new instance of the AutoUIBlamMultiValue class. </summary>
		///
		/// <param name="members">	A variable-length parameters list containing the members to add controls for. </param>
		public AutoUIBlamMultiValue(params string[] members)
		{
			InitializeComponent();

			FlowDirection = System.Windows.Forms.FlowDirection.LeftToRight;
			WrapContents = true;

			SetControls(members);
		}

		/// <summary>	Initializes a new instance of the AutoUIBlamMultiValue class. </summary>
		public AutoUIBlamMultiValue()
			: this(null)
		{ }
		#endregion Constructor

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Sets the controls to have in the control, according to the member names. </summary>
		///
		/// <param name="members">	A variable-length parameters list containing the members to add controls for. </param>
		public virtual void SetControls(string[] members)
		{
			// Clear the current controls
			mControlInstances.Clear();
			Controls.Clear();

			if (members == null)
			{
				return;
			}

			foreach (var member in members)
			{
				// Create a new label for each of the members
				Label label = new Label();

				label.AutoSize = false;
				label.Margin = new Padding(0);
				label.Padding = new Padding(0);
				label.Size = new Size(18, 16);
				label.Text = member;
				label.TextAlign = ContentAlignment.MiddleRight;

				Controls.Add(label);

				// Add the control type instance
				T blamControl = new T();

				blamControl.Margin = new Padding(0);
				blamControl.Size = new Size(70, 20);

				Controls.Add(blamControl);

				mControlInstances.Add(new ControlInstance { BindToMember = member, BlamControl = blamControl });
			}
		}

		#region IAutoUIControl Members
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets information describing the target member. </summary>
		///
		/// <value>	Information describing the target member. </value>
		public Controls.IAutoUIMemberInfo MemberInfo { get; private set; }

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Sets the target member info for the control. </summary>
		///
		/// <param name="memberInfo">	Information describing the target member. </param>
		public void SetMemberInfo(Controls.IAutoUIMemberInfo memberInfo)
		{
			MemberInfo = memberInfo;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Sets the source object to bind the control to. </summary>
		///
		/// <param name="sourceObject">	The source object to bind to. </param>
		public void SetSource(object sourceObject)
		{
			object fieldValue = MemberInfo.GetValue(sourceObject);

			if (fieldValue == null)
			{
				Enabled = false;
				return;
			}

			// Bind the control instances to the source object
			foreach (var controlInstance in mControlInstances)
			{
				Controls.IAutoUIMemberInfo memberInfo = AutoUIFactory.GetAutoUIMemberInfo(fieldValue.GetType(), controlInstance.BindToMember);

				controlInstance.BlamControl.SetMemberInfo(memberInfo);
				controlInstance.BlamControl.SetSource(fieldValue);
			}
		}
		#endregion
	}
}
