/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
﻿using System;
using System.Collections.Generic;
using System.IO;
using System.Xml;
using Microsoft.VisualStudio.TestTools.UnitTesting;

namespace BlamLib.Test
{
	[TestClass]
	public partial class Halo4 : BaseTestClass
	{
		[ClassInitialize]
		public static void Initialize(TestContext context)
		{
			(Program.GetManager(BlamVersion.Halo4_Xbox) as Managers.IStringIdController)
				.StringIdCacheOpen(BlamVersion.Halo4_Xbox);

			Directory.CreateDirectory(kTestResultsPath);
		}
		[ClassCleanup]
		public static void Dispose()
		{
			(Program.GetManager(BlamVersion.Halo4_Xbox) as Managers.IStringIdController)
				.StringIdCacheClose(BlamVersion.Halo4_Xbox);
		}

		static readonly string[] kMapNames_Retail = {
		};
	};
}