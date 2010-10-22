/*
    BlamLib: .NET SDK for the Blam Engine

    Copyright (C) 2005-2010  Kornner Studios (http://kornner.com)

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
[assembly: AssemblyCompany("Kornner Software ©  2005 - 2010")]
[assembly: AssemblyProduct("BlamLib")]
[assembly: AssemblyCopyright("Copyright ©  2006 - 2010")]
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