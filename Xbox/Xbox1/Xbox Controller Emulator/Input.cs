/*
    OpenSauceBox: SDK for Xbox User Modding

    Copyright (C)  Kornner Studios (http://kornner.com)

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
using System;
using System.Windows.Forms;
using Microsoft.DirectX.DirectInput;

namespace Xbox_Controller_Emulator
{
    public class Input
    {
        Device Keyboard, Mouse, Joystick/*, Gamepad*/;

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
                {}
            }

            return null;
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
                {}
            }

            return new MouseState();
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
                {}
            }

            return new JoystickState();
        }
    };
}