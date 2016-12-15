/*
	OpenSauceBox: SDK for Xbox User Modding

	See license\Xbox\Xbox for specific license information
*/
using System;
using System.IO;
using System.Drawing;
using System.Windows.Forms;
using YeloDebug;
using Microsoft.Win32;
//using Microsoft.Xna.Framework;
//using Microsoft.Xna.Framework.Input;
//using Input = Microsoft.Xna.Framework.Input;

namespace LaymansXboxEmulator
{
    public partial class Form1 : Form
    {
        Xbox Xbox = new Xbox();
        XboxVideoStream Video;
        Form blocker = new Form();

		public System.Drawing.Image GetDefaultImage()
        {
			return System.Drawing.Image.FromFile("xboxlogo.bmp");
        }


        public void Main()
        {
            try
            {
                int counter = 0;
                DateTime before = DateTime.Now;
                while (Xbox.Ping(500))
                {
                    try
                    {
                        if (!mnuPause.Checked && Video.IsActive)
                        {
                            //ParseUserInput();

                            // dont bother updating at full speed if minimized, just poll so we don't lose the stream...
                            if (this.WindowState == FormWindowState.Minimized)
                                System.Threading.Thread.Sleep(1000);

                            renderWindow.Image = Video.NextFrame();
                            status.Text = "Frame: " + Video.FrameNumber;    // Xbox.GetUInt32(Xbox.GetUInt32(0xB0033DA0)); (presentation frame number)
                            counter++;
                        }

                        // attempt to recreate videostream before breaking out of the loop
                        // the main reason for it becoming inactive is if its paused for more than 5 seconds or so...
                        if (!Video.IsActive && !Video.Restart())
							break;
                    }
                    catch (Exception)
                    {
                        Xbox.Connect();
                        Video.Restart();
                    }
                    Application.DoEvents();
                }
                TimeSpan elapse = DateTime.Now - before;
                double fps = counter / elapse.TotalSeconds;

                Xbox.Disconnect();
                renderWindow.Image = null;
                mnuPause.Checked = false;
                mnuStep.Enabled = false;
                status.Text = "Disconnected";
            }
            catch (Exception)
            {
            }

            renderWindow.Image = GetDefaultImage();
        }

        public Form1()
        {
            InitializeComponent();
            renderWindow.Image = GetDefaultImage();
            //blocker.WindowState = FormWindowState.Maximized;
            //blocker.Opacity = 0.01f;
            //blocker.Visible = false;
            //blocker.ShowIcon = false;
            //blocker.TopMost = false;
            //blocker.FormBorderStyle = FormBorderStyle.FixedSingle;
            //blocker.ControlBox = false;
            //blocker.Show();
            
            // autodetect if we can run in fullscreen or just half screen...might support lower resolutions later...

            // half screen
            Size = new Size(328, 317);
            renderWindow.Size = new Size(320, 245);
           
            // full screen
            //Size = new Size(648, 560);
            //renderWindow.Size = new Size(640, 480);

        }

        // MCPX - Media Control Processor for Xbox

        // hook DSOUND:00382D22 ; public: long __thiscall DirectSound::CMcpxBuffer::Play(unsigned long)
        // DSOUND:003826E4 ; protected: void __thiscall DirectSound::CMcpxBuffer::PlayFromPosition(unsigned long,unsigned long)
        // DSOUND:003825A3 ; protected: void __thiscall DirectSound::CMcpxBuffer::PlayFromCurrent(unsigned long)


        //pFormat->wFormatTag          = 0x69;		// WAVE_FORMAT_XBOX_ADPCM
        //pFormat->nSamplesPerSec      = 44100
        //pFormat->nChannels           = 1;
        //pFormat->wBitsPerSample      = 4;
        //pFormat->nBlockAlign         = 36;
        //pFormat->nAvgBytesPerSec     = 24804		// 24kbs
        //pFormat->cbSize              = 2;
        //				                64 samples per block


        //pFormat->wFormatTag          = 1;		// WAVE_FORMAT_PCM
        //pFormat->nSamplesPerSec      = 44100;
        //pFormat->nChannels           = 2;
        //pFormat->wBitsPerSample      = 16;
        //pFormat->nBlockAlign         = 4;
        //pFormat->nAvgBytesPerSec     = 176400;
        //pFormat->cbSize              = 0;



        // possibly hook dsound direct media object instead, seems to be a much lower sample rate
        // todo: hook CAc97Channel::AttachPacket(DWORD dwBufferAddress, DWORD dwBufferSize)
        //const WAVEFORMATEX CAc97Device::m_wfxFormat = 
        //{ 
        //    WAVE_FORMAT_PCM,    // wFormatTag
        //    2,                  // nChannels
        //    48000,              // nSamplesPerSec
        //    192000,             // nAvgBytesPerSec
        //    4,                  // nBlockAlign
        //    16,                 // wBitsPerSample
        //    0                   // cbSize
        //};

        // 768 kilobits (93 kilobytes) per second (averaging 187.5 kilobytes a second)

        //const DWORD CAc97Channel::m_dwSilenceSize = 0x2000;
        //ASSERT((dwBufferSize / (m_pDevice->m_wfxFormat.wBitsPerSample / 8)) <= 0xFFFF);

        // so.....
        // ASSERT(dwBufferSize <= 64kb)
        // ASSERT(!(dwBufferSize % m_pDevice->m_wfxFormat.nBlockAlign));

        // **** max packet size of 64kb and requires 4-byte alignment ****

        // xmoreg
        //pFormat->wFormatTag          = WAVE_FORMAT_PCM;
        //pFormat->nSamplesPerSec      = 16000;
        //pFormat->nChannels           = 1;
        //pFormat->wBitsPerSample      = (WORD) 16;
        //pFormat->nBlockAlign         = 2;     //pFormat->wBitsPerSample * pFormat->nChannels / 8;
        //pFormat->nAvgBytesPerSec     = 32000  //pFormat->nSamplesPerSec * pFormat->nBlockAlign;
        //pFormat->cbSize              = 0;

        // dsound
        //wfx.wFormatTag = WAVE_FORMAT_PCM;
        //wfx.nChannels = 2;
        //wfx.nSamplesPerSec = 44100;
        //wfx.wBitsPerSample = 16;
        //wfx.nBlockAlign = 4;          //wfx.wBitsPerSample / 8 * wfx.nChannels;
        //wfx.nAvgBytesPerSec = 176400  //wfx.nSamplesPerSec * wfx.nBlockAlign;


        // have caved code write to our own audio queue where we can grab from there at our own leisure...

        // possibly downsample to a still acceptable quality

        // disable screensaver when this is running!!!

        // +DSobpool
        void mnuStart_Click(object sender, EventArgs e)
        {
            try
            {
                if (true)//!Xbox.Ping(500))
                {
                    // connect to xbox

                    //System.Diagnostics.Stopwatch sw = System.Diagnostics.Stopwatch.StartNew();
                    Xbox.Connect();
                    SurfaceInformation si = Xbox.DisplayBufferInformation;
                    //Xbox.Reboot(BootFlag.Current);
                    //Xbox.EnableNotificationSession = true;
                    //Xbox.RegisterNotificationSession();
                    //sw.Stop();

                    //Xbox.ClearAllBreakpoints();
                    //Xbox.ContinueAllThreads();
                    //Xbox.Continue();

                    //Xbox.ReceiveNotifications();


                    //Xbox.SetBreakPoint(0x37F3A8);

                    //Xbox.ReceiveNotifications();



                    //int blahasdf = 0;


                    //string test = Xbox.StreamTest();

                    //System.Diagnostics.Stopwatch sw2 = System.Diagnostics.Stopwatch.StartNew();
                    //for (int i = 0; i < 100000; i++)
                    //{
                    //    if (sw2.ElapsedMilliseconds > 1000)
                    //        break;
                    //    Xbox.Ping();
                    //}

                    //Xbox.SetFileCacheSize(1);

                    //Xbox.SendCommand("screenshot");
                    //string line = Xbox.ReceiveSocketLine();
                    //Xbox.FlushSocketBuffer();



                    //Xbox.Reboot();      // have special reconnect command for a reboot, instead of waiting x amount of seconds, just poll x amount of seconds with a max poll count
                    //uint ptr = Xbox.AllocateMemory(0x10000);


                    //uint greatsuccess = Xbox.FreeAllMemory();




                    //while (true)
                    //{
                    //    System.Diagnostics.Stopwatch sw = System.Diagnostics.Stopwatch.StartNew();

                    //    Xbox.FastScreenshot();
                    //    sw.Stop();

                    //    //System.Threading.Thread.Sleep(10);
                    //}

                    //bool valid  = Xbox.IsValidAddress(0x3145000 - 0x1000);




                    //Xbox.Reboot(BootFlag.Wait);
                    //Xbox.LaunchTitle(
                    //using (System.IO.FileStream fs = new System.IO.FileStream("ambient_sound_dump4.bin", System.IO.FileMode.Create, System.IO.FileAccess.Write, System.IO.FileShare.Read))
                    //{
                    //    while (false)
                    //    {

                    //        uint packetBuffer = Xbox.GetUInt32(0xD00614D4);
                    //        uint packetSize = Xbox.GetUInt32(0xD00614D8);
                    //        fs.Write(Xbox.GetMemory(packetBuffer, packetSize), 0, (int)packetSize);
                    //    }


                    //}






                    //Xbox.SetFileCacheSize(1);
                   // Xbox.ClearAllBreakpoints();
                   // Xbox.ContinueAllThreads();
                   //Xbox.Continue();


                   // Xbox.Pause();
                   // // kill irq level change requests so we can actually step through the code in a debugger....
                   // byte[] nopsled = new byte[] { 0x90, 0x90, 0x90, 0x90, 0x90 };
                   // byte[] nopsled2 = new byte[] { 0x90, 0x90, 0x90, 0x90, 0x90 };


                   // // 0x37EBEB DirectSound::CMcpxAPU::DoWork(void)
                   // Xbox.SetMemory(0x37EC05, nopsled);
                   // Xbox.SetMemory(0x37EC46, nopsled);
                   // Xbox.SetMemory(0x37EC5C, nopsled);
                   // Xbox.SetMemory(0x37EC75, nopsled);




                   // Xbox.Continue();


                    //// DirectSound::CMcpxVoiceClient::ActivateVoice(void)
                    //Xbox.SetMemory(0x38236F, nopsled);
                    //Xbox.SetMemory(0x3824C3, nopsled);

                    //// DirectSound::CMcpxBuffer::PlayFromPosition(unsigned long,unsigned long)
                    //Xbox.SetMemory(0x382702, nopsled);
                    //Xbox.SetMemory(0x38279D, nopsled);
                    //Xbox.SetMemory(0x3827D4, nopsled);
                    //Xbox.SetMemory(0x382857, nopsled);

                    //// DirectSound::CMcpxBuffer::PlayFromCurrent(unsigned long)
                    //Xbox.SetMemory(0x3825B5, nopsled);
                    //Xbox.SetMemory(0x3826C8, nopsled);

                    //// DirectSoundEnterCriticalSection(void)
                    //Xbox.SetMemory(0x37A169, nopsled2);
                    //Xbox.SetMemory(0x37A18A, nopsled2);
                    //Xbox.SetMemory(0x37A7FA, nopsled2);
                    //Xbox.SetMemory(0x37A823, nopsled2);

                    //Xbox.SetMemory(0x379E9E, (byte)0xC3);

 


                    //Xbox.SetMemory(0x382D22, (byte)0x55);
                    //uint addr = Xbox.GetDirectSoundPlayAddress();



                    //Xbox.ContinueAllThreads();
                    //Xbox.Continue();
                    //Xbox.SetMemory(0x382D22, 0xcccccccc);


                    //Xbox.Reboot(BootFlag.Cold);

                    //uint ac97RegisterBase = Xbox.GetUInt32(0xFEC00198);
                    //uint frontVolume = Xbox.GetUInt32(0xFEC00100 + 4);
                   // Xbox.SetMemory(0xFEC00100, new byte[128]); // kills sound

                    //byte[] data = Xbox.GetMemory(0xFEC00100, 1024);


                    //uint p = Xbox.GetUInt32(0xFE8020C8);
                    //byte[] data = Xbox.GetMemory(p, 1024);
                    //Xbox.Continue();
                    //Xbox.ClearAllBreakpoints();
                    //Xbox.ResumeThread(48);
                    //Xbox.ContinueAllThreads();

                    //// ac97 register ds:0FEC00000h[edx*2], cx
                    //uint ptr6 = Xbox.GetUInt32(0xFE8020D4);
                    //uint ptr1 = Xbox.GetUInt32(0xFE820010);
                    //uint ptr2 = Xbox.GetUInt32(0xFE8202F8);
                    //uint ptr3 = Xbox.GetUInt32(0xFE8202FC);
                    //uint ptr4 = Xbox.GetUInt32(0xFE820010);

                    //uint ptr5 = Xbox.GetUInt32(0xFEC00000);
                    //uint ptr7 = Xbox.GetUInt32(0xFEC0012C);
                    //uint ptr8 = Xbox.GetUInt32(0xFEC00130);
                    //uint ptr9 = Xbox.GetUInt32(0xFEC0017C); // ac97 channel mode
                    //Xbox.SetMemory(0xFEC0017C, 0);
                    //Xbox.SetMemory(0xFEC0017C, 0x02000000);
                   
                    //DSOUND:003847E9 ; protected: long __thiscall DirectSound::CMcpxCore::SetupFrontEndProcessor(void)
                    //Xbox.LockGPU();

                    //DateTime before = DateTime.Now;
                    //for (int i = 0; i < 1; i++)
                    //{
                    //    Xbox.FastScreenshot();
                    //}
                    //TimeSpan elapse = DateTime.Now - before;

                    // setup controller hook
                    //Xbox.Gamepad.InitializeControllerHook();
                    //Xbox.Gamepad.OverrideControllers(mnuKeyboardInput.Checked || mnuX360Input.Checked);
                    

                    // initialize video stream
                    if (mnuFullResolution.Checked)
                    {
                        // ~16fps regular immediate presentation
                        Video = new XboxVideoStream(Xbox, XboxVideoStream.VideoSize.Full, XboxVideoStream.VideoQuality.Regular, XboxVideoStream.VideoPresentationInterval.Immediate);
                        Video.Begin();
                    }
                    else
                    {   // ~56fps with regular immediate presentation, 30fps with regular presentation interval two
                        Video = new XboxVideoStream(Xbox, XboxVideoStream.VideoSize.Medium, XboxVideoStream.VideoQuality.Regular, XboxVideoStream.VideoPresentationInterval.Two);
                        Video.Begin();
                    }
                    status.Text = "Connected";

                    // ~63fps with a small video size using an immediate presentation window

                    // main program loop
                    Main();
                }
            }
            catch (Exception)
            {
                Xbox.Disconnect();
                mnuPause.Checked = false;
                mnuStep.Enabled = false;
                status.Text = "Disconnected";
            }
        }

        void mnuStop_Click(object sender, EventArgs e)
        {
            mnuPause.Checked = false;
            mnuStep.Enabled = false;
            status.Text = "Disconnected";
            try
            {
                Video.End();
                Xbox.Disconnect();
            }
            catch (Exception) { }
        }

        void mnuScreenshot_Click(object sender, EventArgs e)
        {
            try
            {
                if (Xbox.Connected && Video.IsActive)
					Xbox.Screenshot().Save(DateTime.Now.Ticks + ".png", System.Drawing.Imaging.ImageFormat.Png);
            }
            catch (Exception) { }
        }

        void mnuPause_Click(object sender, EventArgs e)
        {
            if (Video != null && Video.IsActive)
            {
                mnuPause.Checked ^= true;
                mnuStep.Enabled ^= true;
            }
        }

        // needs work
        void mnuReset_Click(object sender, EventArgs e)
        {
            //if (Video != null && Video.IsActive)
            //{
            //    Video.End();
            //    mnuPause.Checked = false;
            //    mnuStep.Enabled = false;
            //    Xbox.Reboot(BootFlag.Current);
            //    System.Threading.Thread.Sleep(250);
            //    Video = new Xbox.VideoStream(Xbox, VideoSize.Medium, VideoPresentationInterval.NoOverride);
            //    Video.Begin();

            //    // setup controller hook
            //    Xbox.InitializeControllerHook();
            //    Xbox.OverrideControllers(true);
            //}
        }

        void mnuExit_Click(object sender, EventArgs e)
        {
            Dispose();
        }

        void mnuStep_Click(object sender, EventArgs e)
        {
            try
            {
                renderWindow.Image = Video.NextFrame();
                status.Text = "Frame: " + Video.FrameNumber;
            }
            catch
            {
                Video.End();
            }
        }

        void mnuFullResolution_Click(object sender, EventArgs e)
        {
            mnuFullResolution.Checked = true;
            mnuMediumResolution.Checked = false;
            Size = new Size(648, 560);
            renderWindow.Size = new Size(640, 480);

            Video = new XboxVideoStream(Xbox, XboxVideoStream.VideoSize.Full, XboxVideoStream.VideoQuality.Regular, XboxVideoStream.VideoPresentationInterval.NoOverride);
            Video.Begin();
        }

        void mnuMediumResolution_Click(object sender, EventArgs e)
        {
            mnuMediumResolution.Checked = true;
            mnuFullResolution.Checked = false;
            Size = new Size(328, 317);
            renderWindow.Size = new Size(320, 245);

            Video = new XboxVideoStream(Xbox, XboxVideoStream.VideoSize.Medium, XboxVideoStream.VideoQuality.Regular, XboxVideoStream.VideoPresentationInterval.Two);
            Video.Begin();

        }

        void mnuCustomWindow_Click(object sender, EventArgs e)
        {
            MaximizeBox ^= true;
            MinimizeBox ^= true;
            if (mnuCustomWindow.Checked)
                FormBorderStyle = FormBorderStyle.Sizable;
            else FormBorderStyle = FormBorderStyle.FixedSingle;

            // be sure to reset current resolution settings when deactivating custom size...

        }

        //int oldX, oldY;
        //public void ParseUserInput()
        //{
        //    InputState input = new InputState();
        //    Microsoft.Xna.Framework.Input.ButtonState pressed = Microsoft.Xna.Framework.Input.ButtonState.Pressed;
        //    if (mnuKeyboardInput.Checked)
        //    {
        //        Microsoft.Xna.Framework.Input.KeyboardState ks = Microsoft.Xna.Framework.Input.Keyboard.GetState();
        //        if (ks.IsKeyDown(Microsoft.Xna.Framework.Input.Keys.Up)) input.Buttons |= YeloDebug.Buttons.Up;
        //        if (ks.IsKeyDown(Microsoft.Xna.Framework.Input.Keys.Down)) input.Buttons |= YeloDebug.Buttons.Down;
        //        if (ks.IsKeyDown(Microsoft.Xna.Framework.Input.Keys.Left)) input.Buttons |= YeloDebug.Buttons.Left;
        //        if (ks.IsKeyDown(Microsoft.Xna.Framework.Input.Keys.Right)) input.Buttons |= YeloDebug.Buttons.Right;
        //        if (ks.IsKeyDown(Microsoft.Xna.Framework.Input.Keys.Z)) input.Buttons |= YeloDebug.Buttons.Back;
        //        if (ks.IsKeyDown(Microsoft.Xna.Framework.Input.Keys.X)) input.Buttons |= YeloDebug.Buttons.Start;
        //        if (ks.IsKeyDown(Microsoft.Xna.Framework.Input.Keys.Q)) input.Buttons |= YeloDebug.Buttons.LeftThumb;
        //        if (ks.IsKeyDown(Microsoft.Xna.Framework.Input.Keys.E)) input.Buttons |= YeloDebug.Buttons.RightThumb;

        //        if (ks.IsKeyDown(Microsoft.Xna.Framework.Input.Keys.G)) input.AnalogButtons[(int)YeloDebug.AnalogButtons.A] = byte.MaxValue;
        //        if (ks.IsKeyDown(Microsoft.Xna.Framework.Input.Keys.Y)) input.AnalogButtons[(int)YeloDebug.AnalogButtons.B] = byte.MaxValue;
        //        if (ks.IsKeyDown(Microsoft.Xna.Framework.Input.Keys.F)) input.AnalogButtons[(int)YeloDebug.AnalogButtons.X] = byte.MaxValue;
        //        if (ks.IsKeyDown(Microsoft.Xna.Framework.Input.Keys.T)) input.AnalogButtons[(int)YeloDebug.AnalogButtons.Y] = byte.MaxValue;
        //        if (ks.IsKeyDown(Microsoft.Xna.Framework.Input.Keys.C)) input.AnalogButtons[(int)YeloDebug.AnalogButtons.White] = byte.MaxValue;
        //        if (ks.IsKeyDown(Microsoft.Xna.Framework.Input.Keys.V)) input.AnalogButtons[(int)YeloDebug.AnalogButtons.Black] = byte.MaxValue;
        //        if (ks.IsKeyDown(Microsoft.Xna.Framework.Input.Keys.Space)) input.AnalogButtons[(int)YeloDebug.AnalogButtons.LeftTrigger] = byte.MaxValue;

        //        if (ks.IsKeyDown(Microsoft.Xna.Framework.Input.Keys.W)) input.ThumbLY = short.MaxValue;
        //        if (ks.IsKeyDown(Microsoft.Xna.Framework.Input.Keys.S)) input.ThumbLY = short.MinValue;
        //        if (ks.IsKeyDown(Microsoft.Xna.Framework.Input.Keys.D)) input.ThumbLX = short.MaxValue;
        //        if (ks.IsKeyDown(Microsoft.Xna.Framework.Input.Keys.A)) input.ThumbLX = short.MinValue;
        //        if (ks.IsKeyDown(Microsoft.Xna.Framework.Input.Keys.I)) input.ThumbRY = short.MaxValue;
        //        if (ks.IsKeyDown(Microsoft.Xna.Framework.Input.Keys.K)) input.ThumbRY = short.MinValue;
        //        if (ks.IsKeyDown(Microsoft.Xna.Framework.Input.Keys.L)) input.ThumbRX = short.MaxValue;
        //        if (ks.IsKeyDown(Microsoft.Xna.Framework.Input.Keys.J)) input.ThumbRX = short.MinValue;

        //        Microsoft.Xna.Framework.Input.MouseState ms = Microsoft.Xna.Framework.Input.Mouse.GetState();
        //        if (ms.LeftButton == pressed) input.AnalogButtons[(int)YeloDebug.AnalogButtons.RightTrigger] = byte.MaxValue;
        //        if (ms.RightButton == Microsoft.Xna.Framework.Input.ButtonState.Pressed)
        //        {
        //            if (oldX != ms.X)
        //            {
        //                input.ThumbRX = (short)((1.0f / (ms.X - oldX)) * short.MaxValue);
        //                oldX = ms.X;
        //            }

        //            if (oldY != ms.Y)
        //            {
        //                input.ThumbRY = (short)((ms.Y - oldY) * short.MaxValue);
        //                oldY = ms.Y;
        //            }
        //        }
        //    }
        //    else if (mnuX360Input.Checked)
        //    {
        //        // get controller state
        //        Microsoft.Xna.Framework.Input.GamePadState x360Input = Microsoft.Xna.Framework.Input.GamePad.GetState(Microsoft.Xna.Framework.PlayerIndex.One);
        //        if (x360Input.IsConnected)
        //        {
        //            // convert to xbox input state
        //            if (x360Input.DPad.Up == pressed) input.Buttons |= YeloDebug.Buttons.Up;
        //            if (x360Input.DPad.Down == pressed) input.Buttons |= YeloDebug.Buttons.Down;
        //            if (x360Input.DPad.Left == pressed) input.Buttons |= YeloDebug.Buttons.Left;
        //            if (x360Input.DPad.Right == pressed) input.Buttons |= YeloDebug.Buttons.Right;
        //            if (x360Input.Buttons.Back == pressed) input.Buttons |= YeloDebug.Buttons.Back;
        //            if (x360Input.Buttons.Start == pressed) input.Buttons |= YeloDebug.Buttons.Start;
        //            if (x360Input.Buttons.LeftStick == pressed) input.Buttons |= YeloDebug.Buttons.LeftThumb;
        //            if (x360Input.Buttons.RightStick == pressed) input.Buttons |= YeloDebug.Buttons.RightThumb;

        //            if (x360Input.Buttons.A == pressed) input.AnalogButtons[(int)YeloDebug.AnalogButtons.A] = byte.MaxValue;
        //            if (x360Input.Buttons.B == pressed) input.AnalogButtons[(int)YeloDebug.AnalogButtons.B] = byte.MaxValue;
        //            if (x360Input.Buttons.X == pressed) input.AnalogButtons[(int)YeloDebug.AnalogButtons.X] = byte.MaxValue;
        //            if (x360Input.Buttons.Y == pressed) input.AnalogButtons[(int)YeloDebug.AnalogButtons.Y] = byte.MaxValue;
        //            if (x360Input.Buttons.LeftShoulder == pressed) input.AnalogButtons[(int)YeloDebug.AnalogButtons.White] = byte.MaxValue;
        //            if (x360Input.Buttons.LeftShoulder == pressed) input.AnalogButtons[(int)YeloDebug.AnalogButtons.Black] = byte.MaxValue;

        //            input.AnalogButtons[(int)YeloDebug.AnalogButtons.LeftTrigger] = (byte)(x360Input.Triggers.Left * byte.MaxValue);
        //            input.AnalogButtons[(int)YeloDebug.AnalogButtons.RightTrigger] = (byte)(x360Input.Triggers.Right * byte.MaxValue);
        //            input.ThumbLX = (short)(x360Input.ThumbSticks.Left.X * short.MaxValue);
        //            input.ThumbLY = (short)(x360Input.ThumbSticks.Left.Y * short.MaxValue);
        //            input.ThumbRX = (short)(x360Input.ThumbSticks.Right.X * short.MaxValue);
        //            input.ThumbRY = (short)(x360Input.ThumbSticks.Right.Y * short.MaxValue);
        //        }
        //    }
        //    Xbox.Gamepad.SetState(0, input);
        //}

        void mnuKeyboardInput_Click(object sender, EventArgs e)
        {
            //mnuKeyboardInput.Checked = true;
            //mnuX360Input.Checked = false;
            //mnuNone.Checked = false;
            //Xbox.Gamepad.OverrideControllers(true);
            //blocker.Visible = true;
        }

        void mnuX360Input_Click(object sender, EventArgs e)
        {
            //mnuX360Input.Checked = true;
            //mnuKeyboardInput.Checked = false;
            //mnuNone.Checked = false;
            //Xbox.Gamepad.OverrideControllers(true);
            //blocker.Visible = false;
        }

        void mnuNone_Click(object sender, EventArgs e)
        {
            //mnuNone.Checked = true;
            //mnuKeyboardInput.Checked = false;
            //mnuX360Input.Checked = false;
            //Xbox.Gamepad.OverrideControllers(false);
            //blocker.Visible = false;
        }

        void mnuReboot_Click(object sender, EventArgs e)
        {
            Xbox.Reboot(BootFlag.Warm);
            if (mnuMediumResolution.Checked)
            {
                Size = new Size(328, 317);
                renderWindow.Size = new Size(320, 245);

                Video = new XboxVideoStream(Xbox, XboxVideoStream.VideoSize.Medium, XboxVideoStream.VideoQuality.Regular, XboxVideoStream.VideoPresentationInterval.Two);
                Video.Begin();
            }
            else if (mnuFullResolution.Checked)
            {
                Size = new Size(648, 560);
                renderWindow.Size = new Size(640, 480);

                Video = new XboxVideoStream(Xbox, XboxVideoStream.VideoSize.Full, XboxVideoStream.VideoQuality.Regular, XboxVideoStream.VideoPresentationInterval.NoOverride);
                Video.Begin();
            }
        }


        //RegistryKey regkeyScreenSaver;
        //object origScreensaverSetting;
        //void DisableScreenSaver()
        //{
        //    regkeyScreenSaver = Registry.CurrentUser.OpenSubKey("ControlPanel").OpenSubKey("Desktop", true);
        //    origScreensaverSetting = regkeyScreenSaver.GetValue("ScreenSaveActive");
        //    regkeyScreenSaver.SetValue("ScreenSaveActive", "0");
        //}
        //void RestoreScreenSaver()
        //{
        //    regkeyScreenSaver.SetValue("ScreenSaveActive", origScreensaverSetting);
        //}
    };
}