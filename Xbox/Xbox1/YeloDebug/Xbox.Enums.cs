/*
	OpenSauceBox: SDK for Xbox User Modding

	See license\Xbox\Xbox for specific license information
*/
using System;

namespace YeloDebug
{
	public enum Surface
	{
		BackBuffer = 0,
		Depth_Stencil = 1,
		FrontBuffer = 2,
		ThirdBuffer = 3,
		Texture0 = 4,
		Texture1 = 5,
		Texture2 = 6,
		Texture3 = 7,
		RenderTarget = 8,
		Overlay = 9
	}

	public enum ATACommandType
	{
		Read = 0,
		Write = 1,
		NonData = 2
	};

	public enum ATADevice
	{
		HardDrive = 0,
		DVDROM = 1
	}

	/// <summary>
	/// Ethernet link status flags.
	/// </summary>
	[Flags]
	public enum LinkStatus
	{
		/// <summary>
		/// Ethernet cable is connected and active.
		/// </summary>
		Active = 1,

		/// <summary>
		/// Ethernet link is set to 100 Mbps.
		/// </summary>
		Speed100Mbps = 2,

		/// <summary>
		/// Ethernet link is set to 10 Mbps.
		/// </summary>
		Speed10Mbps = 4,

		/// <summary>
		/// Ethernet link is in full duplex mode.
		/// </summary>
		FullDuplex = 8,

		/// <summary>
		/// Ethernet link is in half duplex mode.
		/// </summary>
		HalfDuplex = 16
	};

	public enum AVPack
	{
		SideCart,
		HDTV,
		VGA,
		RFU,
		SVideo,
		Undefined,
		StandardRGB,
		Missing,
		Unknown
	};


	public enum StopOnFlags
	{
		/// <summary>
		/// Suspend title execution when a thread is created.
		/// </summary>
		CreateThread,

		/// <summary>
		/// Suspend title execution when a first-chance exception occurs.
		/// </summary>
		FCE,

		/// <summary>
		/// Suspend title execution when a string is sent to debug out.
		/// </summary>
		DebugStr
	};

	[Flags]
	public enum BreakpointType
	{
		/// <summary>
		/// Clear this data breakpoint.
		/// </summary>
		None = 1,

		/// <summary>
		/// Break if the address is written to.
		/// </summary>
		Write = 2,

		/// <summary>
		/// Break if the address is read from or written to.
		/// </summary>
		ReadWrite = 4,

		/// <summary>
		/// Break if the address is executed.
		/// </summary>
		Execute = 8
	};

	public enum NotificationType
	{
		None,
		Breakpoint,
		DebugString,
		Execution,
		SingleStep,
		ModuleLoad,
		ModuleUnload,
		CreateThread,
		DestroyThread,
		Exception,
		ClockInt,
		Assert,
		DataBreak,
		RIP,
		ThreadSwitch,
		SectionLoad,
		SectionUnload,
		Fiber,
		NotifyMax
	};

	public enum XboxVersion
	{
		Devkit,
		DebugKit,   // Green
		V1_0,
		V1_1,
		V1_2,
		V1_3,
		V1_4,
		V1_6,
		Unknown
	};


	/// <summary>
	/// Xbox boot type.
	/// </summary>
	public enum BootFlag : uint
	{
		/// <summary>
		/// When the reboot is complete, the system software will wait 15 
		/// seconds before launching the default title. If you call DmGo 
		/// during this time, the system software will launch the title immediately.
		/// </summary>
		Wait,

		/// <summary>
		/// Perform a "warm" reboot of the console.
		/// </summary>
		Warm,

		/// <summary>
		/// Prevent debugging after reboot. The Xbox debug manager will not be 
		/// loaded on any subsequent warm reboot; a cold boot is required.
		/// </summary>
		NoDebug,

		/// <summary>
		/// When the reboot is complete, the system software will wait 
		/// (with no timeout) before launching the default title. Calling 
		/// DmGo while the system is waiting will launch the title.
		/// </summary>
		Stop,

		/// <summary>
		/// Complete shut-down of the system before rebooting.
		/// </summary>
		Cold,

		/// <summary>
		/// Warm reboot to active title.
		/// </summary>
		Current
	};

	/// <summary>
	/// Xbox video standard.
	/// </summary>
	public enum VideoStandard : byte
	{
		NTSCAmerica = 1,
		NTSCJapan = 2,
		PAL = 3
	};

	/// <summary>
	/// Xbox video flags.
	/// </summary>
	[Flags]
	public enum VideoFlags
	{
		Normal = 0,
		Widescreen = 0x1,
		HDTV_720p = 0x2,
		HDTV_1080i = 0x4,
		HDTV_480p = 0x8,
		Letterbox = 0x10,
		PAL_60Hz = 0x40
	};

	/// <summary>
	/// Xbox memory allocation type.
	/// </summary>
	public enum AllocationType : byte
	{
		Debug,
		Virtual,
		Physical,
		System
	};

	/// <summary>
	/// Xbox memory flags.
	/// </summary>
	[Flags]
	public enum MEMORY_FLAGS
	{
		PAGE_VIDEO = 0x0,
		PAGE_NOACCESS = 0x1,
		PAGE_READONLY = 0x2,
		PAGE_READWRITE = 0x4,
		PAGE_WRITECOPY = 0x8,
		PAGE_EXECUTE = 0x10,
		PAGE_EXECUTE_READ = 0x20,
		PAGE_EXECUTE_READWRITE = 0x40,
		PAGE_EXECUTE_WRITECOPY = 0x80,
		PAGE_GUARD = 0x100,
		PAGE_NOCACHE = 0x200,
		PAGE_WRITECOMBINE = 0x400,
		MEM_COMMIT = 0x1000,
		MEM_RESERVE = 0x2000,
		MEM_DECOMMIT = 0x4000,
		MEM_RELEASE = 0x8000,
		MEM_FREE = 0x10000,
		MEM_PRIVATE = 0x20000,
		MEM_RESET = 0x80000,
		MEM_TOP_DOWN = 0x100000,
		MEM_NOZERO = 0x800000
	};

	/// <summary>
	/// Represents one of the 4 possible Xbox LED states.
	/// </summary>
	public enum LEDState : byte
	{
		Off = 0,
		Red = 0x80,
		Green = 8,
		Orange = 0x88
	};

	/// <summary>
	/// Xbox gamepad buttons.
	/// </summary>
	[Flags]
	public enum Buttons : ushort
	{
		Up = 1 << 0,
		Down = 1 << 1,
		Left = 1 << 2,
		Right = 1 << 3,
		Start = 1 << 4,
		Back = 1 << 5,
		LeftThumb = 1 << 6,
		RightThumb = 1 << 7,
		LightGunOnScreen = 1 << 13,
		LightGunFrameDoubler = 1 << 14,
		LightGunLineDoubler = 1 << 15
	};

	/// <summary>
	/// Xbox analog gamepad buttons.
	/// </summary>
	[Flags]
	public enum AnalogButtons
	{
		A,
		B,
		X,
		Y,
		Black,
		White,
		LeftTrigger,
		RightTrigger
	};

	/// <summary>
	/// Xbox response type.
	/// </summary>
	public enum ResponseType
	{
		// Success
		SingleResponse = 200,  //OK
		Connected = 201,
		MultiResponse = 202,  //terminates with period
		BinaryResponse = 203,
		ReadyForBinary = 204,
		NowNotifySession = 205,  // notificaiton channel/ dedicated connection

		// Errors
		UndefinedError = 400,
		MaxConnectionsExceeded = 401,
		FileNotFound = 402,
		NoSuchModule = 403,
		MemoryNotMapped = 404,  //setzerobytes or setmem failed
		NoSuchThread = 405,
		ClockNotSet = 406,  //linetoolong or clocknotset
		UnknownCommand = 407,
		NotStopped = 408,
		FileMustBeCopied = 409,
		FileAlreadyExists = 410,
		DirectoryNotEmpty = 411,
		BadFileName = 412,
		FileCannotBeCreated = 413,
		AccessDenied = 414,
		NoRoomOnDevice = 415,
		NotDebuggable = 416,
		TypeInvalid = 417,
		DataNotAvailable = 418,
		BoxIsNotLocked = 420,
		KeyExchangeRequired = 421,
		DedicatedConnectionRequired = 422,
		InvalidArgument = 423,
		ProfileNotStarted = 424,
		ProfileAlreadyStarted = 425,
		D3DDebugCommandNotImplemented = 480,
		D3DInvalidSurface = 481,
		VxTaskPending = 496,
		VxTooManySessions = 497,
	};

	/// <summary>
	/// Receive wait type.
	/// </summary>
	public enum WaitType
	{
		/// <summary>
		/// Does not wait.
		/// </summary>
		None,

		/// <summary>
		/// Waits for data to start being received.
		/// </summary>
		Partial,

		/// <summary>
		/// Waits for data to start and then stop being received.
		/// </summary>
		Full,

		/// <summary>
		/// Waits for data to stop being received.
		/// </summary>
		Idle
	};

	/// <summary>
	/// Items to include in a memdump.
	/// </summary>
	[Flags]
	public enum DumpFlags
	{
		/// <summary>
		/// Xbe code segment.
		/// </summary>
		Code,

		/// <summary>
		/// Xbe data segment.
		/// </summary>
		Data,

		/// <summary>
		/// System pages. (kernel, stacks, pools, etc...)
		/// </summary>
		System,

		/// <summary>
		/// Debugger pages.
		/// </summary>
		Debug
	};


	/// <summary>
	/// Devices on the System Management Controller
	/// </summary>
	public enum SMCDevices
	{
		SMBus = 0x20,   // slave addresses
		VideoEncoderConnexant = 0x8a,
		VideoEncoderFocus = 0xd4,
		VideoEncoderXcalibur = 0xe0,
		TempMonitor = 0x98,
		EEPROM = 0xA8
	};


	/// <summary>
	/// Command codes available for the SMBus.
	/// </summary>
	public enum SMBusCommand
	{
		FirmwareVersion = 0x1,  // smc firmware version
		Reset = 0x2,
		TrayState = 0x3,
		VideoMode = 0x4,
		FanOverride = 0x5,
		RequestFanSpeed = 0x6,
		LedOverride = 0x7,
		LedStates = 0x8,
		CpuTemperature = 0x9,
		AirTemperature = 0xA,
		AudioClamp = 0xB,
		DvdTrayOperation = 0xC,
		OsResume = 0xD,
		WriteErrorCode = 0xE,
		ReadErrorCode = 0xF,
		ReadFanSpeed = 0x10,
		InterruptReason = 0x11,
		WriteRamTestResults = 0x12,
		WriteRamType = 0x13,
		ReadRamTestResults = 0x14,
		ReadRamType = 0x15,
		LastRegisterWritten = 0x16,
		LastByteWritten = 0x17,
		SoftwareInterrupt = 0x18,
		OverrideResetOnTrayOpen = 0x19,
		OsReady = 0x1A,
		ScratchRegister = 0x1B,
		ChallengeValue0 = 0x1C,
		ChallengeValue1 = 0x1D,
		ChallengeValue2 = 0x1E,
		ChallengeValue3 = 0x1F,
		ChallengeResponse0 = 0x20,
		ChallengeResponse1 = 0x21
	};

	/// <summary>
	/// Flags for the SMBus reset command.
	/// </summary>
	public enum ResetCommand
	{
		Reset = 0x01,
		PowerCycle = 0x40,
		ShutDown = 0x80
	};



	/// <summary>
	/// Flags for the SMBus tray state command.
	/// </summary>
	[Flags]
	public enum TrayState : byte
	{
		/// <summary>
		/// Drive is busy.
		/// </summary>
		Busy = 0x1,

		/// <summary>
		/// Tray is open.
		/// </summary>
		Open = 0x10,

		/// <summary>
		/// Tray is ejecting empty.
		/// </summary>
		Ejecting = 0x20,

		/// <summary>
		/// Tray is opening empty.
		/// </summary>
		Opening = 0x30,

		/// <summary>
		/// Tray is closed with no media.
		/// </summary>
		NoMedia = 0x40,

		/// <summary>
		/// Tray is closing
		/// </summary>
		Closing = 0x50,

		/// <summary>
		/// Tray detected media to be present.
		/// </summary>
		MediaPresent = 0x60,

		/// <summary>
		/// Tray is resetting
		/// </summary>
		Reset = 0x70,

		/// <summary>
		/// No optical drive detected.
		/// </summary>
		None = 0x5

		//Loading = 0x1,           // loading rom
		//EjectingFull = 0x21,         // ejecting with rom in tray
		//EjectingEmpty = 0x31,       // ejecting empty tray
		//Open = 0x10,
		//Closing = 0x51,             // closing tray
		//ClosedAndEmpty = 0x40,      // closed with no rom
		//ClosedAndFull = 0x60      // closed with rom
	};



	/// <summary>
	/// Commands that can be sent to the video encoder.
	/// </summary>
	public enum VideoEncoderCommand // TVEncoderSMBusID
	{
		Detect = 0x00
		//Unknown = 0x5 // subcommand > 0 spits out random numbers, its fucking weird...
	};

	/// <summary>
	/// Values for the video encoder.
	/// </summary>
	public enum VideoEncoder
	{
		Unknown = 0,
		Connexant = 1,
		Focus = 2,
		Xcalibur = 3
	};

	/// <summary>
	/// Sub-commands for the PIC led command.
	/// </summary>
	public enum LEDSubCommand
	{
		Default = 0x00,
		Custom = 0x01
	};

	/// <summary>
	/// Sub-commands for the PIC eject command.
	/// </summary>
	public enum EjectSubCommand
	{
		Eject = 0x00,
		Load = 0x01
	};

	public enum FanModeSubCommand
	{
		Default = 0x0,
		Custom = 0x1
	};


	//Reason for interrupt
	public enum InterruptReason
	{
		PowerButton = 0x01,
		AvCableRemoved = 0x10,
		EjectButton = 0x20
	};

	/// <summary>
	/// Sub-commands for reset on eject PIC command.
	/// </summary>
	public enum ResetOnEjectSubCommand
	{
		Enable = 0x00,
		Disable = 0x01
	};

	/// <summary>
	/// Scratch register values.
	/// </summary>
	public enum ScratchRegisterValue
	{
		EjectAfterBoot = 0x01,
		DisplayError = 0x02,
		NoAnimation = 0x04,
		RunDashboard = 0x08
	};

	/// <summary>
	/// Xbox device.
	/// </summary>
	public enum Drive
	{
		CDRom,
		DriveC,
		DriveE,
		DriveF,
		//DriveG, // seems to be disabled in debug bios
		//DriveH, // seems to be disabled in debug bios
		DriveX,
		DriveY,
		DriveZ
	}

	/// <summary>
	/// Xbox drive name.
	/// </summary>
	public enum DriveLabel
	{
		A,  // DVD-ROM drive
		B,  // Volume
		C,  // Main volume
		D,  // Active title media
		E,  // Game development volume
		F,  // Memory unit 1A
		G,  // Memory unit 1B
		H,  // Memory unit 2A
		I,  // Memory unit 2B
		J,  // Memory unit 3A
		K,  // Memory unit 3B
		L,  // Memory unit 4A
		M,  // Memory unit 4B
		N,  // Secondary active utility drive
		O,  // Volume
		P,  // Utility drive for unknown title
		Q,  // Utility drive for unknown title  
		R,  // Utility drive for unknown title
		S,  // Persistent data for all titles
		T,  // Persistent data for active title
		U,  // Saved games for active title
		V,  // Saved games for all titles
		W,  // Persistant data for alternate title
		X,  // Saved games for alternate title
		Y,  // Reserved/unmappable while in debug???
		Z   // Active utility drive
	};
}