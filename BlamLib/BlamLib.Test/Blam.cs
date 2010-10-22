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
﻿using System;
using Microsoft.VisualStudio.TestTools.UnitTesting;

namespace BlamLib.Test
{
	[TestClass]
	public class BlamTest : BaseTestClass
	{
		[ClassInitialize]
		public static void Initialize(TestContext context)
		{
		}
		[ClassCleanup]
		public static void Dispose()
		{
		}

		[TestMethod]
		public void BlamTestSystemStringIdCache()
		{
			Program.Halo3.Manager.StringIdCacheOpen(BlamVersion.Halo3_Xbox);
			Program.Halo3.Manager.StringIdCacheClose(BlamVersion.Halo3_Xbox);
		}

		[TestMethod]
		public void BlamTestSystemVertexBufferCache()
		{
			Program.Halo3.Manager.VertexBufferCacheOpen(BlamVersion.Halo3_Xbox);
			var gr = Program.Halo3.Manager[BlamVersion.Halo3_Xbox].
				GetResource<Render.VertexBufferInterface.VertexBuffers>("VertexBuffer");
			Program.Halo3.Manager.VertexBufferCacheClose(BlamVersion.Halo3_Xbox);
		}

		[TestMethod]
		public void BlamTestSystemTagIndex()
		{
			var ti = new Managers.TagIndex(BlamVersion.Halo1_CE, @"C:\Program Files\Microsoft Games\Halo Custom Edition\tags\", false);
			ti.Dispose();
			ti = new Managers.TagIndex(BlamVersion.Halo2_PC, @"C:\Program Files\Microsoft Games\Halo 2 Map Editor\tags\", false);
			ti.Dispose();
		}
	};
}