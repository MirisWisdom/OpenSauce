using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Collections;

namespace OpenSauceIDE.Aether.AutoUI.Controls
{
	//public partial class AutoUIArray : UserControl, IAutoUIControl
	//{
	//	private object mSourceObject = null;
	//	private Array mArrayMember = null;
	//	private Type mElementType = null;
	//	private bool mShowDisplay = false;

	//	public AutoUIArray()
	//	{
	//		InitializeComponent();

	//		ButtonShowHide.Click += ButtonShowHide_Click;
	//		ButtonAdd.Click += ButtonAdd_Click;
	//		ButtonInsert.Click += ButtonInsert_Click;
	//		ButtonDuplicate.Click += ButtonDuplicate_Click;
	//		ButtonDelete.Click += ButtonDelete_Click;
	//		ButtonDeleteAll.Click += ButtonDeleteAll_Click;
	//	}


	//	void ButtonShowHide_Click(object sender, EventArgs e)
	//	{
	//		mShowDisplay = !mShowDisplay;

	//		ButtonShowHide.Text = mShowDisplay ? "-" : "+";
	//	}

	//	void ButtonAdd_Click(object sender, EventArgs e)
	//	{
	//		if (mArrayMember == null)
	//		{
	//			mArrayMember = Activator.CreateInstance(MemberInfo.MemberValueType) as Array;
	//			MemberInfo.SetValue(mSourceObject, mArrayMember);
	//		}
			
	//	}

	//	void ButtonInsert_Click(object sender, EventArgs e)
	//	{
	//		throw new NotImplementedException();
	//	}

	//	void ButtonDuplicate_Click(object sender, EventArgs e)
	//	{
	//		throw new NotImplementedException();
	//	}

	//	void ButtonDelete_Click(object sender, EventArgs e)
	//	{
	//		throw new NotImplementedException();
	//	}

	//	void ButtonDeleteAll_Click(object sender, EventArgs e)
	//	{
	//		throw new NotImplementedException();
	//	}

	//	#region IAutoUIControl Members

	//	public IAutoUIMemberInfo MemberInfo
	//	{
	//		get;
	//		private set;
	//	}

	//	public void SetMemberInfo(IAutoUIMemberInfo memberInfo)
	//	{
	//		MemberInfo = memberInfo;
	//	}

	//	public void SetSource(object sourceObject)
	//	{
	//		mEnumerableMember = MemberInfo.GetValue(sourceObject) as IEnumerable;

	//		if (mEnumerableMember == null)
	//		{
	//			Enabled = false;
	//			return;
	//		}

	//		mEnumeratorType = MemberInfo.MemberValueType.GetElementType();
	//	}

	//	#endregion
	//}
}
