using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using OpenSauceIDE.Aether.AutoUI.Controls;

namespace OpenSauceIDE.Aether.AutoUI.BlamControls
{
	public partial class AutoUIBlamRealBounds : UserControl, IAutoUIControl
	{
		public AutoUIBlamRealBounds()
		{
			InitializeComponent();
		}

		#region IAutoUIControl Members
		public Controls.IAutoUIMemberInfo MemberInfo
		{
			get;
			private set;
		}

		public void SetMemberInfo(Controls.IAutoUIMemberInfo memberInfo)
		{
			MemberInfo = memberInfo;
		}

		public void SetSource(object sourceObject)
		{
			object fieldValue = MemberInfo.GetValue(sourceObject);

			if (fieldValue == null)
			{
				Enabled = false;
				return;
			}
			
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
