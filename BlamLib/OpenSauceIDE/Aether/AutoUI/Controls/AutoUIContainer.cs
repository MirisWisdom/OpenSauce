using System;
using System.Collections.Generic;

namespace OpenSauceIDE.Aether.AutoUI.Controls
{
	//public class AutoUIContainer : System.Windows.Forms.Panel
	//{
	//	private AutoUIContainerLayout mLayoutHandler = null;

	//	public AutoUIContainerLayout LayoutHandler { get; set; }
	//}

	//public abstract class AutoUIContainerLayout
	//{
	//	protected System.Windows.Forms.Control mTargetControl;

	//	public AutoUIContainerLayout(System.Windows.Forms.Control targetControl)
	//	{
	//		mTargetControl = targetControl;

	//		mTargetControl.Layout += OnTargetControlLayout;
	//	}

	//	~AutoUIContainerLayout()
	//	{
	//		mTargetControl.Layout -= OnTargetControlLayout;
	//	}

	//	private void OnTargetControlLayout(object sender, System.Windows.Forms.LayoutEventArgs e)
	//	{
	//		mTargetControl.SuspendLayout();
	//		UpdateLayout();
	//		mTargetControl.ResumeLayout();
	//	}

	//	protected abstract void UpdateLayout();
	//};

	//public class AutoUILayoutFlow : AutoUIContainerLayout
	//{
	//	public enum DirectionEnum
	//	{
	//		X,
	//		Y
	//	}

	//	public DirectionEnum Direction { get; set; }
	//	public bool Fill { get; set; }
	//	public bool ExpandTarget { get; set; }

	//	public AutoUILayoutFlow(System.Windows.Forms.Control targetControl)
	//		: base(targetControl)
	//	{ }

	//	protected override void UpdateLayout()
	//	{
	//		int x = 0, y = 0;

	//		for (int i = 0; i < mTargetControl.Controls.Count; i++)
	//		{
	//			var control = mTargetControl.Controls[i];

	//			//if (!singleList)
	//			//{
	//			//    switch (direction)
	//			//    {
	//			//        case ListDirectionEnum.ListX:
	//			//            if ((x + control.Width) > parentControl.Width)
	//			//            {
	//			//                y = nextY;
	//			//                x = 0;
	//			//            }
	//			//            break;
	//			//        case ListDirectionEnum.ListY:
	//			//            if ((y + control.Height) > parentControl.Height)
	//			//            {
	//			//                x = nextX;
	//			//                y = 0;
	//			//            }
	//			//            break;
	//			//    }
	//			//}

	//			switch (Direction)
	//			{
	//				case DirectionEnum.X:
	//					AutoUIFactory.SetControlBounds(control, x, y, control.Width, Fill ? mTargetControl.Height : control.Height);
	//					x += control.Width;
	//					break;
	//				case DirectionEnum.Y:
	//					AutoUIFactory.SetControlBounds(control, x, y, Fill ? mTargetControl.Width : control.Width, control.Height);
	//					y += control.Height;
	//					break;
	//			}
	//		}

	//		if (ExpandTarget)
	//		{
	//			switch (Direction)
	//			{
	//				case DirectionEnum.X:
	//					mTargetControl.SetBounds(mTargetControl.Location.X, mTargetControl.Location.Y, x, mTargetControl.Size.Height);
	//					break;
	//				case DirectionEnum.Y:
	//					mTargetControl.SetBounds(mTargetControl.Location.X, mTargetControl.Location.Y, mTargetControl.Size.Width, y);
	//					break;
	//			}
	//		}
	//	}
	//}

	//public class AutoUILayoutFill : AutoUIContainerLayout
	//{
	//	public enum DirectionEnum
	//	{
	//		X,
	//		Y
	//	}

	//	public DirectionEnum Direction { get; set; }

	//	public AutoUILayoutFill(System.Windows.Forms.Control targetControl)
	//		: base(targetControl)
	//	{ }

	//	protected override void UpdateLayout()
	//	{
	//		int controlWidth = mTargetControl.Width, controlHeight = mTargetControl.Height;

	//		switch (Direction)
	//		{
	//			case DirectionEnum.X:
	//				controlWidth /= mTargetControl.Controls.Count;
	//				break;
	//			case DirectionEnum.Y:
	//				controlHeight /= mTargetControl.Controls.Count;
	//				break;
	//		}

	//		int x = 0, y = 0;
	//		for (int i = 0; i < mTargetControl.Controls.Count; i++)
	//		{
	//			var control = mTargetControl.Controls[i];

	//			switch (Direction)
	//			{
	//				case DirectionEnum.X:
	//					AutoUIFactory.SetControlBounds(control, x, y, controlWidth, mTargetControl.Height);
	//					x += controlWidth;
	//					break;
	//				case DirectionEnum.Y:
	//					AutoUIFactory.SetControlBounds(control, x, y, mTargetControl.Width, controlHeight);
	//					y += controlHeight;
	//					break;
	//			}
	//		}
	//	}
	//}
}
