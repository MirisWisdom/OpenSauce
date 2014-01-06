using System;

namespace OpenSauceIDE.Aether.AutoUI.Controls
{
	///-------------------------------------------------------------------------------------------------
	/// <summary>	Interface for automatic user interface controls. </summary>
	///-------------------------------------------------------------------------------------------------
	public interface IAutoUIControl
	{
		///-------------------------------------------------------------------------------------------------
		/// <summary>	Gets information describing the target member. </summary>
		/// <value>	Information describing the target member. </value>
		///-------------------------------------------------------------------------------------------------
		IAutoUIMemberInfo MemberInfo { get; }

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Sets the target member info for the control. </summary>
		/// <param name="memberInfo">	Information describing the control's target member. </param>
		///-------------------------------------------------------------------------------------------------
		void SetMemberInfo(IAutoUIMemberInfo memberInfo);

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Sets the source object to bind the control to. </summary>
		/// <param name="sourceObject">	The source object to bind to. </param>
		///-------------------------------------------------------------------------------------------------
		void SetSource(object sourceObject);
	}
}
