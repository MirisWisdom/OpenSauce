using OpenSauceIDE.Aether.AutoUI.Controls;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Reflection;

namespace OpenSauceIDE.Aether.AutoUI.TypeDescriptor
{
	///-------------------------------------------------------------------------------------------------
	/// <summary>	An automatic user interface type descriptor. </summary>
	///-------------------------------------------------------------------------------------------------
	public class AutoUITypeDescriptor : IAutoUITypeDescriptor
	{
		#region Fields
		private List<IAutoUIMemberInfo> mMemberInfos = new List<IAutoUIMemberInfo>();
		#endregion Fields

		///-------------------------------------------------------------------------------------------------
		/// <summary>
		/// 	Collects information for all of the members in the specified type.
		/// </summary>
		///-------------------------------------------------------------------------------------------------
		private void CollectMemberInfos()
		{
			// Get all public members
			var objectMembers = DescribedType.GetMembers(BindingFlags.Public | BindingFlags.Instance | BindingFlags.Static)
				.Where(
					member =>
					{
						// Get the member info and return it if it is not just an "object" and is not an array
						IAutoUIMemberInfo memberInfo = AutoUIFactory.GetAutoUIMemberInfo(member);

						if (memberInfo == null)
						{
							return false;
						}

						return !(typeof(object) == memberInfo.MemberValueType)
							&& !typeof(Array).IsAssignableFrom(memberInfo.MemberValueType);
					}
				);

			// Add all of the found members
			foreach (var member in objectMembers)
			{
				IAutoUIMemberInfo memberInfo = AutoUIFactory.GetAutoUIMemberInfo(member);

				mMemberInfos.Add(memberInfo);
			}
		}

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Constructor. </summary>
		/// <param name="targetType">	The type to describe. </param>
		///-------------------------------------------------------------------------------------------------
		public AutoUITypeDescriptor(Type targetType)
		{
			DescribedType = targetType;

			CollectMemberInfos();
		}

		#region IAutoUITypeDescriptor
		///-------------------------------------------------------------------------------------------------
		/// <summary>	Gets the type being described. </summary>
		/// <value>	The type being described. </value>
		///-------------------------------------------------------------------------------------------------
		public Type DescribedType { get; private set; }

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Gets the member infos for the type. </summary>
		/// <returns>	The member infos for the type. </returns>
		///-------------------------------------------------------------------------------------------------
		public List<IAutoUIMemberInfo> GetMemberInfos()
		{
			return mMemberInfos;
		}

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Gets a members attributes. </summary>
		/// <param name="memberInfo">	Information describing the member. </param>
		/// <returns>	The members attributes. </returns>
		///-------------------------------------------------------------------------------------------------
		public List<Attribute> GetMemberAttributes(IAutoUIMemberInfo memberInfo)
		{
			return memberInfo.MemberInfo.GetCustomAttributes(false)
				.OfType<Attribute>().ToList<Attribute>();
		}

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Gets a members attributes. </summary>
		/// <param name="memberName">	Name of the member. </param>
		/// <returns>	The members attributes. </returns>
		///-------------------------------------------------------------------------------------------------
		public List<Attribute> GetMemberAttributes(string memberName)
		{
			return GetMemberAttributes(mMemberInfos.Find(info => info.Name.Equals(memberName)));
		}

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Gets the attributes for the described type. </summary>
		/// <returns>	The types attributes. </returns>
		///-------------------------------------------------------------------------------------------------
		public List<Attribute> GetAttributes()
		{
			return DescribedType.GetCustomAttributes(false)
				.OfType<Attribute>().ToList<Attribute>();
		}
		#endregion IAutoUITypeDescriptor
	}
}
