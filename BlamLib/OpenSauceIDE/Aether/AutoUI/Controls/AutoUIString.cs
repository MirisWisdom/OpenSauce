using System;

namespace OpenSauceIDE.Aether.AutoUI.Controls
{
	/// <summary>	An automatic user interface text box for a string. </summary>
	public class AutoUIString : System.Windows.Forms.TextBox, IAutoUIControl
	{
		#region Constructor
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Constructor. </summary>
		///
		/// <param name="maxLength">	The maximum length of the string. </param>
		public AutoUIString(int maxLength)
		{
			MaxLength = maxLength;
		}

		/// <summary>	Default constructor. </summary>
		public AutoUIString()
			: this(0)
		{ }
		#endregion Constructor

		#region IAutoUIControl Members
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets information describing the target member. </summary>
		///
		/// <value>	Information describing the target member. </value>
		public IAutoUIMemberInfo MemberInfo { get; private set; }

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Sets the target member info for the control. </summary>
		///
		/// <param name="memberInfo">	Information describing the target member. </param>
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
			DataBindings.Clear();

			DataBindings.Add("Text"
				, sourceObject
				, MemberInfo.Name
				, false
				, System.Windows.Forms.DataSourceUpdateMode.OnPropertyChanged
				, "");

			Enabled = MemberInfo.CanWrite;
		}
		#endregion IAutoUIControl Members
	}
}
