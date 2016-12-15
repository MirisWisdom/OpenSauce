/*
	OpenSauceBox: SDK for Xbox User Modding

	See license\Xbox\Xbox for specific license information
*/
using System;
using System.Windows.Forms;
using Microsoft.DirectX.DirectInput;

using YeloDebug;

namespace Xbox_Controller_Emulator
{
	public partial class XCE : Form
	{
		bool running = true;
		Prompt Prompt = new Prompt();
		Xbox Xbox = new Xbox();
		Input Input;
		InputState NewInput;

		public XCE()
		{
			InitializeComponent();
		}

		public void ParseKeyboardInput(ref InputState NewInput)
		{
			//gets keyboard input
			KeyboardState Keyboard = Input.GetKeyboardState();
			if (Keyboard != null)
			{
				if (Keyboard[Key.UpArrow])		NewInput.ThumbLY = short.MaxValue;
				if (Keyboard[Key.LeftArrow])	NewInput.ThumbLX = short.MinValue;
				if (Keyboard[Key.DownArrow])	NewInput.ThumbLY = short.MinValue;
				if (Keyboard[Key.RightArrow])	NewInput.ThumbLX = short.MaxValue;
				if (Keyboard[Key.A])			NewInput.Buttons |= Buttons.Back;
				if (Keyboard[Key.S])			NewInput.Buttons |= Buttons.Start;
				if (Keyboard[Key.I])			NewInput.AnalogButtons[(int)AnalogButtons.Y] = 0xFF;
				if (Keyboard[Key.J])			NewInput.AnalogButtons[(int)AnalogButtons.X] = 0xFF;
				if (Keyboard[Key.K])			NewInput.AnalogButtons[(int)AnalogButtons.A] = 0xFF;
				if (Keyboard[Key.L])			NewInput.AnalogButtons[(int)AnalogButtons.B] = 0xFF;
				if (Keyboard[Key.U])			NewInput.AnalogButtons[(int)AnalogButtons.White] = 0xFF;
				if (Keyboard[Key.O])			NewInput.AnalogButtons[(int)AnalogButtons.Black] = 0xFF;

				if (Keyboard[Key.Escape])
				{
					Xbox.Gamepad.OverrideControllers(false);
					running = false;
				}
			}
		}

		void XCE_Shown(object sender, System.EventArgs e)
		{
			try
			{
                this.WindowState = FormWindowState.Normal;
                System.Threading.Thread.Sleep(1000);

				Prompt.Show();
				while (!Prompt.IsDisposed)  // waits for it to be disposed
				{
					System.Threading.Thread.Sleep(1);
					Application.DoEvents();
				}
				Input = new Input(this);
                Xbox.Connect(Prompt.DebugName);
				Xbox.Gamepad.InitializeControllerHook();
				Xbox.Gamepad.OverrideControllers(true);

				while (running)
				{
					NewInput = new InputState();
					//DateTime before = DateTime.Now;

					ParseKeyboardInput(ref NewInput);

					#region Mouse Input
					MouseState Mouse = Input.GetMouseState();
					if (Mouse.GetMouseButtons()[0] > 0) // left click
						NewInput.AnalogButtons[(int)AnalogButtons.RightTrigger] = 0xFF;
					if (Mouse.GetMouseButtons()[1] > 0) // right click
						NewInput.AnalogButtons[(int)AnalogButtons.LeftTrigger] = 0xFF;
					if (Mouse.GetMouseButtons()[2] > 0) // middle click
						NewInput.Buttons |= Buttons.RightThumb;

					/*
					// get mouse input with separate function...we need to capture constantly instead of 30fps

					NewInput.ThumbRX = (short)(Mouse.X * 0x7FF);
					NewInput.ThumbRY = (short)(-Mouse.Y * 0x7FF);


					//NewInput.ThumbRX = (short)(((float)short.MaxValue * (Mouse.X / 2)) / 4);
					//NewInput.ThumbRY = (short)(-((float)short.MaxValue * (Mouse.Y / 2) / 4));
					*/
					#endregion


					//int PollsPerSecond = 30;
					//System.Threading.Thread.Sleep(1000 / PollsPerSecond);
					System.Threading.Thread.Sleep(1);
					Xbox.Gamepad.SetState(0, NewInput);
					Application.DoEvents();
				}

			}
			catch
			{
				Hide();
				MessageBox.Show("You slipped one past the goalie, don't let it happen again ;P", "Unhandled Exception", MessageBoxButtons.OK, MessageBoxIcon.Error);
			}
			finally
			{
				Close();
			}
		}

		int count = 0;
		void XCE_MouseMove(object sender, System.Windows.Forms.MouseEventArgs e)
		{
			count += Input.GetMouseState().X;
		}
	};
}