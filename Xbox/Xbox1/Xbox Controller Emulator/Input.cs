using System;
using System.Windows.Forms;
using System.Collections.Generic;
using System.Text;
using Microsoft.DirectX.DirectInput;

namespace Xbox_Controller_Emulator
{
    public class Input
    {
        private Device Keyboard, Mouse, Joystick, Gamepad;

        public Input(Control parentWindow)
        {
            // detect all available devices
            foreach (DeviceInstance di in Manager.GetDevices(DeviceClass.All, EnumDevicesFlags.AllDevices))
            {
                if (di.DeviceType == DeviceType.Keyboard)
                {
                    Keyboard = new Device(di.InstanceGuid);
                    Keyboard.SetDataFormat(DeviceDataFormat.Keyboard);
                    Keyboard.SetCooperativeLevel(parentWindow, CooperativeLevelFlags.Foreground | CooperativeLevelFlags.NonExclusive);
                }
                else if (di.DeviceType == DeviceType.Mouse)
                {
                    Mouse = new Device(di.InstanceGuid);
                    Mouse.SetDataFormat(DeviceDataFormat.Mouse);
                    Mouse.SetCooperativeLevel(parentWindow, CooperativeLevelFlags.Foreground | CooperativeLevelFlags.NonExclusive);
                }
                else if (di.DeviceType == DeviceType.Joystick)
                {
                    Joystick = new Device(di.InstanceGuid);
                    Joystick.SetDataFormat(DeviceDataFormat.Joystick);
                    Joystick.SetCooperativeLevel(parentWindow, CooperativeLevelFlags.Foreground | CooperativeLevelFlags.NonExclusive);
                }
                //else if (di.DeviceType == DeviceType.Gamepad)
                //{
                //    Gamepad = new Device(di.InstanceGuid);

                //    // must manually specify a format when working with different gamepads...i think ;x
                //    DataFormat customFormat;

                //    Gamepad.SetDataFormat(customFormat);
                //    Gamepad.SetCooperativeLevel(parentWindow, CooperativeLevelFlags.Foreground | CooperativeLevelFlags.NonExclusive);
                //}

            }
        }

        public KeyboardState GetKeyboardState()
        {
            if (Keyboard != null)
            {
                try
                {
                    Keyboard.Acquire();
                    return Keyboard.GetCurrentKeyboardState();
                }
                catch (Exception)
                {
                    return null;
                }
            }
            else return null;
        }

        public MouseState GetMouseState()
        {
            if (Mouse != null)
            {
                try
                {
                    Mouse.Acquire();
                    return Mouse.CurrentMouseState;
                }
                catch (Exception)
                {
                    return new MouseState();
                }
            }
            else return new MouseState();
        }

        public JoystickState GetJoystickState()
        {
            if (Joystick != null)
            {
                try
                {
                    Joystick.Acquire();
                    return Joystick.CurrentJoystickState;
                }
                catch (Exception)
                {
                    return new JoystickState();
                }
            }
            else return new JoystickState();
        }
    };
}