using OpenSauceIDE.Aether.AutoUI.Controls;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Reflection;

namespace OpenSauceIDE.Aether.AutoUI.TypeDescriptor
{
	///-------------------------------------------------------------------------------------------------
	/// <summary>
	/// 	Automatic user interface type decorator. Used to provide AutoUI attributes for the
	/// 	members of an external type.
	/// </summary>
	/// <typeparam name="T">	Generic type parameter. </typeparam>
	///-------------------------------------------------------------------------------------------------
	public class AutoUITypeDecorator<T> : IAutoUITypeDescriptor
		where T : INotifyPropertyChanged
	{
		///-------------------------------------------------------------------------------------------------
		/// <summary>	Defines the attribute decoration for a type member. </summary>
		///-------------------------------------------------------------------------------------------------
		protected class DecoratedMember
		{
			#region Fields
			public IAutoUIMemberInfo MemberInfo;
			public List<Attribute> Attributes;
			#endregion

			///-------------------------------------------------------------------------------------------------
			/// <summary>	Gets the name of the decorated member. </summary>
			/// <value>	The name of the decorated member. </value>
			///-------------------------------------------------------------------------------------------------
			public string MemberName
			{
				get
				{
					return MemberInfo.Name;
				}
			}

			///-------------------------------------------------------------------------------------------------
			/// <summary>	Constructor. </summary>
			/// <param name="memberInfo">	Information describing the member. </param>
			///-------------------------------------------------------------------------------------------------
			public DecoratedMember(IAutoUIMemberInfo memberInfo)
			{
				MemberInfo = memberInfo;
				Attributes = new List<Attribute>();
			}

			///-------------------------------------------------------------------------------------------------
			/// <summary>	Adds an attribute to the member. </summary>
			/// <param name="attribute">	The attribute to add. </param>
			///-------------------------------------------------------------------------------------------------
			public void AddAttribute(Attribute attribute)
			{
				Attributes.Add(attribute);
			}

			///-------------------------------------------------------------------------------------------------
			/// <summary>	Adds a number of attributes to the member. </summary>
			/// <param name="attributes">	The attributes to add. </param>
			///-------------------------------------------------------------------------------------------------
			public void AddAttributes(Attribute[] attributes)
			{
				Attributes.AddRange(attributes);
			}
		}

		#region Fields
		private List<Attribute> mAttributes;
		private Dictionary<string, DecoratedMember> mDecoratedMembers;
		#endregion Fields

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Default constructor. </summary>
		///-------------------------------------------------------------------------------------------------
		public AutoUITypeDecorator()
		{
			mAttributes = new List<Attribute>();
			mDecoratedMembers = new Dictionary<string, DecoratedMember>();
		}

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Adds a decorated member.</summary>
		/// <param name="name">		 	The name of the member. </param>
		/// <param name="attributes">	(Optional) The attributes to add to the member. </param>
		///-------------------------------------------------------------------------------------------------
		protected void AddMember(string name, Attribute[] attributes = null)
		{
			// Get the named member from the type
			MemberInfo[] members = DescribedType.GetMember(name);

			if (members.Length != 1)
			{
				throw new Exception(String.Format("Invalid member count when getting info for member {0} in {1}, expected a single field or property member info", name, DescribedType.Name));
			}

			// Add an entry to the descorated members with the provided attributes if preset
			IAutoUIMemberInfo memberInfo = AutoUIFactory.GetAutoUIMemberInfo(members[0]);
			mDecoratedMembers.Add(name, new DecoratedMember(memberInfo));

			if (attributes != null)
			{
				mDecoratedMembers[name].AddAttributes(attributes);
			}
		}

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Adds attributes to the type. </summary>
		/// <param name="attributes">	The attributes to add to the type. </param>
		///-------------------------------------------------------------------------------------------------
		protected void AddAttributes(Attribute[] attributes)
		{
			mAttributes.AddRange(attributes);
		}

		#region IAutoUITypeDescriptor
		///-------------------------------------------------------------------------------------------------
		/// <summary>	Gets the type that is being described. </summary>
		/// <value>		Type that is being described. </value>
		///-------------------------------------------------------------------------------------------------
		public Type DescribedType
		{
			get
			{
				return typeof(T);
			}
		}

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Gets the member infos for the type. </summary>
		/// <returns>	The member infos for the type. </returns>
		///-------------------------------------------------------------------------------------------------
		public List<IAutoUIMemberInfo> GetMemberInfos()
		{
			List<IAutoUIMemberInfo> memberInfos = new List<IAutoUIMemberInfo>();

			foreach(var member in mDecoratedMembers)
			{
				memberInfos.Add(member.Value.MemberInfo);
			}

			return memberInfos;
		}

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Gets a members attributes. </summary>
		/// <param name="memberInfo">	Information describing the member. </param>
		/// <returns>	The members attributes. </returns>
		///-------------------------------------------------------------------------------------------------
		public List<Attribute> GetMemberAttributes(IAutoUIMemberInfo memberInfo)
		{
			return GetMemberAttributes(memberInfo.Name);
		}

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Gets a members attributes. </summary>
		/// <param name="memberName">	Name of the member. </param>
		/// <returns>	The members attributes. </returns>
		///-------------------------------------------------------------------------------------------------
		public List<Attribute> GetMemberAttributes(string memberName)
		{
			return mDecoratedMembers[memberName].Attributes;
		}

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Gets the attributes for the described type. </summary>
		/// <returns>	The types attributes. </returns>
		///-------------------------------------------------------------------------------------------------
		public List<Attribute> GetAttributes()
		{
			return mAttributes;
		}
		#endregion IAutoUITypeDescriptor
	}
}
