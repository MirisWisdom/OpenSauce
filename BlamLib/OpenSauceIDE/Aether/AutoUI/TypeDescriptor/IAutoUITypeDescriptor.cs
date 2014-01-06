using OpenSauceIDE.Aether.AutoUI.Controls;
using System;
using System.Collections.Generic;

namespace OpenSauceIDE.Aether.AutoUI.TypeDescriptor
{
	///-------------------------------------------------------------------------------------------------
	/// <summary>	Interface for an automatic user interface type descriptor. </summary>
	///-------------------------------------------------------------------------------------------------
	public interface IAutoUITypeDescriptor
	{
		///-------------------------------------------------------------------------------------------------
		/// <summary>	Gets the type that is being described. </summary>
		/// <value>		Type that is being described. </value>
		///-------------------------------------------------------------------------------------------------
		Type DescribedType { get; }

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Gets the member infos for the type. </summary>
		/// <returns>	The member infos for the type. </returns>
		///-------------------------------------------------------------------------------------------------
		List<IAutoUIMemberInfo> GetMemberInfos();

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Gets a members attributes. </summary>
		/// <param name="memberInfo">	Information describing the member. </param>
		/// <returns>	The members attributes. </returns>
		///-------------------------------------------------------------------------------------------------
		List<Attribute> GetMemberAttributes(IAutoUIMemberInfo memberInfo);

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Gets a members attributes. </summary>
		/// <param name="memberName">	Name of the member. </param>
		/// <returns>	The members attributes. </returns>
		///-------------------------------------------------------------------------------------------------
		List<Attribute> GetMemberAttributes(string memberName);

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Gets the attributes for the described type. </summary>
		/// <returns>	The types attributes. </returns>
		///-------------------------------------------------------------------------------------------------
		List<Attribute> GetAttributes();
	}
}
