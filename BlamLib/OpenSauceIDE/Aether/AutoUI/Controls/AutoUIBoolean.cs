using System;
using System.Windows.Forms;
using System.Reflection;

namespace OpenSauceIDE.Aether.AutoUI.Controls
{
	/// <summary>	An automatic user interface checkbox for booleans. </summary>
	public class AutoUIBoolean : CheckBox, IAutoUIControl
	{
		#region IAutoUIControl Members
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets the member info for the control. </summary>
		///
		/// <value>	Information describing the control's target member. </value>
		public IAutoUIMemberInfo MemberInfo { get; private set; }

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Sets the target member info for the control. </summary>
		///
		/// <param name="memberInfo">	Information describing the control's target member. </param>
		public void SetMemberInfo(IAutoUIMemberInfo memberInfo)
		{
			MemberInfo = memberInfo;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Sets the source object to bind the control to. </summary>
		///
		/// <param name="sourceObject">	The source object to bind to. </param>
		public void SetSource(object sourceObject)
		{
			// Get the initial state of the checkbox
			bool initialValue = (bool)MemberInfo.GetValue(sourceObject);
			Checked = initialValue;

			// Bind the control to the source object
			DataBindings.Add(new Binding("Checked", sourceObject, MemberInfo.Name));

			// Disable the control if the member is read only
			Enabled = MemberInfo.CanWrite;
		}
		#endregion IAutoUIControl Members
	}
}
