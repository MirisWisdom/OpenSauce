/*
	OpenSauceBox: SDK for Xbox User Modding

	See license\Xbox\Xbox for specific license information
*/
using System;
using System.IO;
using System.Diagnostics;
using YeloDebug.Exceptions;

namespace YeloDebug
{
    public class XboxGamepad
    {
        [DebuggerBrowsable(DebuggerBrowsableState.Never)]
        Xbox Xbox;

		// keep this stuff locally to avoid repeatedly grabbing it off of the xbox...
        [DebuggerBrowsable(DebuggerBrowsableState.Never)]
        public InputState PreviousState = new InputState();  // keep track of our previous state so we can detect changes
        [DebuggerBrowsable(DebuggerBrowsableState.Never)]
        public uint PacketNumber = 0;   // if we detect any changes, update the packet number

		/// <summary>
		/// Searches xbox memory for XInputGetState(). Average execution time of 15ms.
		/// </summary>
		/// <returns></returns>
		uint GetXInputGetStateAddress()
		{
			// assumes everything we need is in header...why would they put it elsewhere???

			// buffer to store our xbe header
            byte[] xbeh = Xbox.GetMemory(0x10000, (uint)0x1000);
            BinaryReader header = new BinaryReader(new System.IO.MemoryStream(xbeh));

			header.BaseStream.Position = 0x11C;
			uint SegmentCount = header.ReadUInt32();                //gets segment count
			uint SegmentBase = header.ReadUInt32() - 0x10000;       //gets base address of segment info table

			//loops through each segment
			for (int i = 0; i < SegmentCount; i++)
			{
				header.BaseStream.Position = (uint)(SegmentBase + i * 56) + 4;
				uint SegAddress = header.ReadUInt32();
				uint SegSize = header.ReadUInt32();
				header.BaseStream.Position += 8;
				header.BaseStream.Position = header.ReadUInt32() - 0x10000;
				string SegName = System.Text.ASCIIEncoding.ASCII.GetString(header.ReadBytes(3));

				if (SegName.Equals("XPP"))
				{
					//dumps xpp segment
                    byte[] SegDump = Xbox.GetMemory(SegAddress, SegSize);

					//searches for xinputgetstate function
					for (int j = 0; j < SegSize; j++)
						if ((BitConverter.ToUInt64(SegDump, j) & 0xFFFFFFFFFFFF) == 0x000015FFDB335653)
							return (uint)(SegAddress + j);
				}
			}
			throw new Exception("Unable to find XInputGetState() in memory, you must manually specify this address instead if you wish to initialize a controller hook.");
		}

        /// <summary>
        /// Compares the differences between two states and determines if they have changed.
        /// </summary>
        /// <param name="oldState"></param>
        /// <param name="newState"></param>
        /// <returns></returns>
        bool StateChanged(InputState oldState, InputState newState)
        {
            for (int i = 0; i < 8; i++)
                if (oldState.AnalogButtons[i] != newState.AnalogButtons[i])
                    return true;

            if (oldState.Buttons != newState.Buttons ||
                oldState.ThumbRX != newState.ThumbRX ||
                oldState.ThumbRY != newState.ThumbRY ||
                oldState.ThumbLX != newState.ThumbLX ||
                oldState.ThumbLY != newState.ThumbLY)
                return true;

            return false;
        }

		/// <summary>
		/// Detects whether or not we have already hooked into XInputGetState().
		/// </summary>
		/// <returns></returns>
		public bool IsGamepadHooked()
		{
            if (Xbox.History.XInputGetStateAddress == 0) return false;
            return Xbox.GetByte(Xbox.History.XInputGetStateAddress) == 0x68; // test for "push   dword ptr ds:[address]"
		}

		/// <summary>
		/// Hooks into the XInputGetState procedure on your xbox, and hijacks controller input.
		/// </summary>
		public void InitializeControllerHook()
		{
			// dont hook again if we are already hooked
			if (IsGamepadHooked()) return;

			// get hook address
            Xbox.History.XInputGetStateAddress = GetXInputGetStateAddress();
            Xbox.SetMemory(XboxHistory.Gamepad.XInputGetState, Xbox.History.XInputGetStateAddress);

			// get first part of original code
            byte[] origCode = Xbox.GetMemory(Xbox.History.XInputGetStateAddress, 10);
            Xbox.SetMemory(XboxHistory.Gamepad.OriginalCodeBuffer, origCode);

			#region Build Script
			// store our script in memory that reads from our buffers instead...
			byte[] scriptData = new byte[71];
			using (var script = new BinaryWriter(new System.IO.MemoryStream(scriptData)))
			{
				//cmp	dword ptr ds:[EnabledAddress], 1	;IsEnabled
				//je	Intercept
				//db	10	dup (0)	;replace with orig code
				//push	XInputGetState + 10
				//ret
				//Intercept:
				script.Write((ushort)0x3D83);
				script.Write(XboxHistory.Gamepad.EnabledAddress);
				script.Write((ushort)0x7401);
				script.Write((byte)0x10);
				script.Write(origCode);
				script.Write((byte)0x68);
				script.Write(Xbox.History.XInputGetStateAddress + 10);
				script.Write((byte)0xC3);

				//pushad					; esp -= 32
				//mov	eax, [esp + 36]			; get port from handle
				//mov	eax, [eax]
				//mov	eax, [eax]
				//mov	eax, [eax + 14h]
				//add	dword ptr ds:[012345678h + eax * 4], 1	; controller in that port is plugged in
				//shl	eax, 5				; get buffer index
				//lea	esi, [012345678h + eax]		; buffer address
				//mov	edi, [esp + 40]	;pInputState	; their buffer address
				//mov	ecx, 22	;XINPUT_STATE size	; replace their data with ours
				//rep	movsb 
				//popad	
				//xor	eax, eax	;ERROR_SUCCESS
				//retn    8
				byte[] pt1 = { 0x60, 0x8B, 0x44, 0x24, 0x24, 0x8B, 0x00, 0x8B, 0x00, 0x8B, 0x40, 0x14, 0x83, 0x04, 0x85 };
				script.Write(pt1);
				script.Write(XboxHistory.Gamepad.PortStatusAddress);
				byte[] pt2 = { 0x01, 0xC1, 0xE0, 0x05, 0x8D, 0xB0 };
				script.Write(pt2);
				script.Write(XboxHistory.Gamepad.StateBufferAddress);
				byte[] pt3 = { 0x8B, 0x7C, 0x24, 0x28, 0xB9, 0x16, 0x00, 0x00, 0x00, 0xF3, 0xA4, 0x61, 0x33, 0xC0, 0xC2, 0x08, 0x00 };
				script.Write(pt3);
			}
            Xbox.SetMemory(XboxHistory.Gamepad.ScriptAddress, scriptData);
			#endregion

			// now inject our hook which jumps to our script we have just created...
			byte[] hookData = new byte[10];
			using (var hook = new BinaryWriter(new System.IO.MemoryStream(hookData)))
			{
				hook.Write((byte)0x68);
				hook.Write(XboxHistory.Gamepad.ScriptAddress);
				hook.Write((byte)0xC3);
			}
            Xbox.SetMemory(Xbox.History.XInputGetStateAddress, hookData);
		}

        /// <summary>
		/// This is to be used if YeloDebug cannot automatically detect the XInputGetState function address.
		/// Then you will need to specify your own if you wish to hook into the controllers.
        /// This function has not been tested since I have not found a game which doesn't use the standard XInputGetState function.
		/// </summary>
		/// <param name="xinputAddress"></param>
        public void ManuallyInitializeControllerHook(uint xinputAddress)
        {
            // dont hook again if we are already hooked
            if (IsGamepadHooked()) return;

            // get hook address
            Xbox.History.XInputGetStateAddress = xinputAddress;
            Xbox.SetMemory(XboxHistory.Gamepad.XInputGetState, Xbox.History.XInputGetStateAddress);

            // get first part of original code
            byte[] origCode = Xbox.GetMemory(Xbox.History.XInputGetStateAddress, 10);
            Xbox.SetMemory(XboxHistory.Gamepad.OriginalCodeBuffer, origCode);

            #region Build Script
            // store our script in memory that reads from our buffers instead...
            byte[] scriptData = new byte[71];
			using (var script = new BinaryWriter(new System.IO.MemoryStream(scriptData)))
			{
				//cmp	dword ptr ds:[EnabledAddress], 1	;IsEnabled
				//je	Intercept
				//db	10	dup (0)	;replace with orig code
				//push	XInputGetState + 10
				//ret
				//Intercept:
				script.Write((ushort)0x3D83);
				script.Write(XboxHistory.Gamepad.EnabledAddress);
				script.Write((ushort)0x7401);
				script.Write((byte)0x10);
				script.Write(origCode);
				script.Write((byte)0x68);
				script.Write(Xbox.History.XInputGetStateAddress + 10);
				script.Write((byte)0xC3);

				//pushad					; esp -= 32
				//mov	eax, [esp + 36]			; get port from handle
				//mov	eax, [eax]
				//mov	eax, [eax]
				//mov	eax, [eax + 14h]
				//add	dword ptr ds:[012345678h + eax * 4], 1	; controller in that port is plugged in
				//shl	eax, 5				; get buffer index
				//lea	esi, [012345678h + eax]		; buffer address
				//mov	edi, [esp + 40]	;pInputState	; their buffer address
				//mov	ecx, 22	;XINPUT_STATE size	; replace their data with ours
				//rep	movsb 
				//popad	
				//xor	eax, eax	;ERROR_SUCCESS
				//retn    8
				byte[] pt1 = { 0x60, 0x8B, 0x44, 0x24, 0x24, 0x8B, 0x00, 0x8B, 0x00, 0x8B, 0x40, 0x14, 0x83, 0x04, 0x85 };
				script.Write(pt1);
				script.Write(XboxHistory.Gamepad.PortStatusAddress);
				byte[] pt2 = { 0x01, 0xC1, 0xE0, 0x05, 0x8D, 0xB0 };
				script.Write(pt2);
				script.Write(XboxHistory.Gamepad.StateBufferAddress);
				byte[] pt3 = { 0x8B, 0x7C, 0x24, 0x28, 0xB9, 0x16, 0x00, 0x00, 0x00, 0xF3, 0xA4, 0x61, 0x33, 0xC0, 0xC2, 0x08, 0x00 };
				script.Write(pt3);
			}
            Xbox.SetMemory(XboxHistory.Gamepad.ScriptAddress, scriptData);
            #endregion

            // now inject our hook which jumps to our script we have just created...
            byte[] hookData = new byte[10];
			using (var hook = new BinaryWriter(new System.IO.MemoryStream(hookData)))
			{
				hook.Write((byte)0x68);
				hook.Write(XboxHistory.Gamepad.ScriptAddress);
				hook.Write((byte)0xC3);
			}
            Xbox.SetMemory(Xbox.History.XInputGetStateAddress, hookData);
        }

		/// <summary>
		/// Specifies if the xbox should override controller input and start listening for pc input.
		/// </summary>
		/// <param name="enabled"></param>
		public void OverrideControllers(bool enabled)
		{
			if (enabled)
                Xbox.SetMemory(XboxHistory.Gamepad.EnabledAddress, (int)1);
			else
			{
				// clear states so if we re-enable, it doesnt perform old actions...
                Xbox.SetMemory(XboxHistory.Gamepad.StateBufferAddress, new byte[32 * 4]);
                Xbox.SetMemory(XboxHistory.Gamepad.EnabledAddress, (int)0);
			}
		}

		/// <summary>
		/// Sends gamepad input to a specified controller port.  
		/// *** A controller must be plugged into the specified port for this to work. ***
		/// You are also responsible for the polling frequency.
		/// </summary>
		/// <param name="port">Controller port. [0-3]</param>
		/// <param name="input">Gamepad input.</param>
		public void SetState(uint port, InputState input)
		{
			if (port > 3)
				throw new Exception("Invalid controller port specified.  Valid ports are within the range of [0-3].");

			// only update if state has changed...otherwise its just a waste of bandwidth ;p
			if (StateChanged(PreviousState, input))
			{
				// get our XINPUT_STATE address
                uint inputState = XboxHistory.Gamepad.StateBufferAddress + port * 32;

				// indicate a changed gamepad state
				PacketNumber++;

				//convert pad to byte array
				byte[] gamepadData = new byte[22];
				using (var pad = new BinaryWriter(new System.IO.MemoryStream(gamepadData)))
				{
					pad.Write(PacketNumber);
					pad.Write((ushort)input.Buttons);
					pad.Write(BitConverter.ToUInt64(input.AnalogButtons, 0));
					pad.Write((short)input.ThumbLX);
					pad.Write((short)input.ThumbLY);
					pad.Write((short)input.ThumbRX);
					pad.Write((short)input.ThumbRY);
				}

				// store new state
				Xbox.SetMemory(inputState, gamepadData);

				// replace old with new
				PreviousState = input;
			}
		}

        public XboxGamepad(Xbox xbox)
        {
            this.Xbox = xbox;
        }
    };
}