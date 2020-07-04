/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
﻿using System.Reflection;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;

// General Information about an assembly is controlled through the following 
// set of attributes. Change these attribute values to modify the information
// associated with an assembly.
[assembly: AssemblyTitle("OpenSauceIDE")]
[assembly: AssemblyDescription("OpenSauce IDE Beta")]
[assembly: AssemblyConfiguration(
	"OpenSauce IDE (" +
#if DEBUG
	"DEBUG"
#else
	"RELEASE"
#endif
 + ")")]
[assembly: AssemblyCompany("Kornner Studios ©  2005 - 2011")]
[assembly: AssemblyProduct("OpenSauceIDE")]
[assembly: AssemblyCopyright("Copyright ©  2008 - 2011")]
[assembly: AssemblyTrademark("")]
[assembly: AssemblyCulture("")]

// Setting ComVisible to false makes the types in this assembly not visible 
// to COM components.  If you need to access a type in this assembly from 
// COM, set the ComVisible attribute to true on that type.
[assembly: ComVisible(false)]

// The following GUID is for the ID of the typelib if this project is exposed to COM
[assembly: Guid("00d65114-1acd-42c5-92f3-221ac2114546")]

// Version information for an assembly consists of the following four values:
//
//      Major Version
//      Minor Version 
//      Build Number
//      Revision
//
[assembly: AssemblyVersion("1.0.*")]
[assembly: AssemblyFileVersion("1.0.0.0")]
