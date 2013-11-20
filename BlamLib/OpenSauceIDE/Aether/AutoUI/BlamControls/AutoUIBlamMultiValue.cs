using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace OpenSauceIDE.Aether.AutoUI.BlamControls
{
	public partial class AutoUIBlamMultiValue<T> : System.Windows.Forms.FlowLayoutPanel, AutoUI.Controls.IAutoUIControl
		where T : Control, AutoUI.Controls.IAutoUIControl, new()
	{
		private struct ControlInstance
		{
			public string BindToMember;
			public T BlamControl;
		}

		private List<ControlInstance> mControlInstances = new List<ControlInstance>();

		public virtual void SetControls(string[] members)
		{
			mControlInstances.Clear();
			Controls.Clear();

			if (members == null)
			{
				return;
			}

			foreach (var member in members)
			{
				Label label = new Label();

				label.AutoSize = false;
				label.Margin = new Padding(0);
				label.Padding = new Padding(0);
				label.Size = new Size(18, 16);
				label.Text = member;
				label.TextAlign = ContentAlignment.MiddleRight;

				Controls.Add(label);

				T blamControl = new T();

				blamControl.Margin = new Padding(0);
				blamControl.Size = new Size(70, 20);

				Controls.Add(blamControl);

				mControlInstances.Add(new ControlInstance { BindToMember = member, BlamControl = blamControl });
			}
		}

		public AutoUIBlamMultiValue(params string[] members)
		{
			InitializeComponent();

			FlowDirection = System.Windows.Forms.FlowDirection.LeftToRight;
			WrapContents = true;

			//LayoutHandler = new AutoUI.Controls.AutoUILayoutFlow(this)
			//{
			//	Direction = AutoUI.Controls.AutoUILayoutFlow.DirectionEnum.X,
			//	Fill = false
			//};

			SetControls(members);
		}

		public AutoUIBlamMultiValue()
			: this(null)
		{ }

		#region IAutoUIControl Members
		///-------------------------------------------------------------------------------------------------
		/// <summary>	Gets or sets information describing the target member. </summary>
		/// <value>	Information describing the target member. </value>
		///-------------------------------------------------------------------------------------------------
		public Controls.IAutoUIMemberInfo MemberInfo { get; private set; }

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Sets the target member info for the control. </summary>
		/// <param name="memberInfo">	Information describing the target member. </param>
		///-------------------------------------------------------------------------------------------------
		public void SetMemberInfo(Controls.IAutoUIMemberInfo memberInfo)
		{
			MemberInfo = memberInfo;
		}

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Sets the source object to bind the control to. </summary>
		/// <param name="sourceObject">	The source object to bind to. </param>
		///-------------------------------------------------------------------------------------------------
		public void SetSource(object sourceObject)
		{
			object fieldValue = MemberInfo.GetValue(sourceObject);

			if (fieldValue == null)
			{
				Enabled = false;
				return;
			}

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
