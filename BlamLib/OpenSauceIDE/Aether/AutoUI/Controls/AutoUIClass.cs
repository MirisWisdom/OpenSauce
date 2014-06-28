using System;
using System.Collections.Generic;

namespace OpenSauceIDE.Aether.AutoUI.Controls
{
	/// <summary>	An automatic user interface control for a class. </summary>
	public class AutoUIClass : System.Windows.Forms.FlowLayoutPanel, IAutoUIControl
	{
		/// <summary>	Initializes a new instance of the AutoUIClass class. </summary>
		public AutoUIClass()
		{
			FlowDirection = System.Windows.Forms.FlowDirection.TopDown;
			WrapContents = false;
		}

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
		/// <summary>	Sets the source object to bind the control to, creating child controls to bind to it. </summary>
		///
		/// <param name="sourceObject">	The object to bind to. </param>
		public void SetSource(object sourceObject)
		{
			// Get the member value from the source object
			object memberObject = MemberInfo.GetValue(sourceObject);

			// Create an auto ui control set for the member, wrapped in field containers
			var autoUIControls = AutoUIFactory.Instance.BuildAutoUI(memberObject);
			List<AutoUIFieldContainer> fieldContainers = new List<AutoUIFieldContainer>();

			autoUIControls.ForEach(
				control =>
				{
					// Set the name for each field container and add them to the class control
					AutoUIFieldContainer container = new AutoUIFieldContainer();
					IAutoUIControl autoUIControl = control as IAutoUIControl;

					string name = AutoUIFactory.GetAutoUIName(autoUIControl.MemberInfo);
					name = (name == null ? name = MemberInfo.Name : name);

					container.SetTitle(name);
					container.AddControl(control);

					fieldContainers.Add(container);

					Controls.Add(container);
				}
			);
		}
		#endregion
	}
}
