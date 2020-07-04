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
[assembly: AssemblyTitle("BlamLib")]
[assembly: AssemblyDescription("Blam Engine Library")]
[assembly: AssemblyConfiguration(
	"Blam Library (" +
#if DEBUG
	"DEBUG"
#else
	"RELEASE"
#endif
 + ")")]
[assembly: AssemblyCompany("Kornner Studios ©  2005 - 2011")]
[assembly: AssemblyProduct("BlamLib")]
[assembly: AssemblyCopyright("Copyright ©  2006 - 2011")]
[assembly: AssemblyTrademark("")]
[assembly: AssemblyCulture("")]

// Setting ComVisible to false makes the types in this assembly not visible 
// to COM components.  If you need to access a type in this assembly from 
// COM, set the ComVisible attribute to true on that type.
[assembly: ComVisible(false)]

// The following GUID is for the ID of the typelib if this project is exposed to COM
[assembly: Guid("1805628b-f401-489f-b66a-edc0ffe6a8e5")]

// Version information for an assembly consists of the following four values:
//
//      Major Version
//      Minor Version 
//      Build Number
//      Revision
//
// You can specify all the values or you can default the Revision and Build Numbers 
// by using the '*' as shown below:
[assembly: AssemblyVersion("1.0.*")]
[assembly: AssemblyFileVersion("1.0.0.0")]


[assembly: InternalsVisibleTo("Tool")] // So tool can access the Render.XmlInterface code
[assembly: InternalsVisibleTo("BlamLib.Test")]
//TODO:
//[assembly: InternalsVisibleTo("BlamLib.Editor, PublicKey=")]
//[assembly: InternalsVisibleTo("BlamLib.Forms, PublicKey=")]