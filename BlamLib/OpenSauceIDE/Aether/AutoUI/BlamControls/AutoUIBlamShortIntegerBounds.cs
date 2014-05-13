using System;
using OpenSauceIDE.Aether.AutoUI.Controls;

namespace OpenSauceIDE.Aether.AutoUI.BlamControls
{
	/// <summary>	An automatic user interface blam short integer bounds control. </summary>
	public partial class AutoUIBlamShortIntegerBounds : System.Windows.Forms.UserControl, IAutoUIControl
	{
		/// <summary>	Initializes a new instance of the AutoUIBlamShortIntegerBounds class. </summary>
		public AutoUIBlamShortIntegerBounds()
		{
			InitializeComponent();
		}

		#region IAutoUIControl Members
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets information describing the target member. </summary>
		///
		/// <value>	Information describing the target member. </value>
		public Controls.IAutoUIMemberInfo MemberInfo
		{
			get;
			private set;
		}

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
		/// <param name="sourceObject">	Source object. </param>
		public void SetSource(object sourceObject)
		{
			object fieldValue = MemberInfo.GetValue(sourceObject);

			if (fieldValue == null)
			{
				Enabled = false;
				return;
			}

			// Bind the upper and lower controls to the source object
			Controls.IAutoUIMemberInfo lowerMemberInfo = AutoUIFactory.GetAutoUIMemberInfo(fieldValue.GetType(), "Lower");
			Controls.IAutoUIMemberInfo upperMemberInfo = AutoUIFactory.GetAutoUIMemberInfo(fieldValue.GetType(), "Upper");

			ValueLower.SetMemberInfo(lowerMemberInfo);
			ValueLower.SetSource(fieldValue);
			ValueUpper.SetMemberInfo(upperMemberInfo);
			ValueUpper.SetSource(fieldValue);
		}
		#endregion
	}
}
