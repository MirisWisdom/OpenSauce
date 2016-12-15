/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
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
			bool x64 = IntPtr.Size == 64;

			string k_hce = @"C:\Program Files" + (x64 ? " (x86)" : "") + @"\Microsoft Games\Halo Custom Edition\tags\";
			string k_h2v = @"C:\Program Files" + (x64 ? " (x86)" : "") + @"\Microsoft Games\Halo 2 Map Editor\tags\";

			Assert.IsTrue(System.IO.Directory.Exists(k_hce));
			Assert.IsTrue(System.IO.Directory.Exists(k_h2v));

			var ti = new Managers.TagIndex(BlamVersion.Halo1_CE, k_hce, false);
			ti.Dispose();
			ti = new Managers.TagIndex(BlamVersion.Halo2_PC, k_h2v, false);
			ti.Dispose();
		}

		static void TestNewStringIdManagerResources(string r_path, string r_name)
		{
			var sid_sc = new Managers.StringIdStaticCollection();
			sid_sc.Load(r_path, r_name);

			var sm2 = new Managers.StringIdManager(sid_sc);
			sm2.Definition.ToString();

			sm2 = null;
		}
		[TestMethod]
		public void BlamTestNewStringIdManagerResources()
		{
			string r_path, r_name;

			#region Halo 2
			{ // Halo2_PC
				r_path = Managers.GameManager.GetRelativePath(Managers.GameManager.Namespace.Halo2, Managers.GameManager.Platform.PC, Managers.GameManager.PlatformFolder.Definitions);
				r_name = "Halo2_PC_StringId.xml";

				TestNewStringIdManagerResources(r_path, r_name);
			}

			{ // Halo2_Alpha
				r_path = Managers.GameManager.GetRelativePath(Managers.GameManager.Namespace.Halo2, Managers.GameManager.Platform.Xbox, Managers.GameManager.PlatformFolder.Definitions);
				r_name = "Halo2_Alpha_StringId.xml";

				TestNewStringIdManagerResources(r_path, r_name);
			}
			{ // Halo2_Xbox
				r_path = Managers.GameManager.GetRelativePath(Managers.GameManager.Namespace.Halo2, Managers.GameManager.Platform.Xbox, Managers.GameManager.PlatformFolder.Definitions);
				r_name = "Halo2_Xbox_StringId.xml";

				TestNewStringIdManagerResources(r_path, r_name);
			}
			#endregion

			#region Halo 3
			r_path = Managers.GameManager.GetRelativePath(Managers.GameManager.Namespace.Halo3, Managers.GameManager.Platform.Xbox, Managers.GameManager.PlatformFolder.Definitions);

			{ // Halo3_Beta
				r_name = "Halo3_Beta_StringId.xml";

				TestNewStringIdManagerResources(r_path, r_name);
			}
			{ // Halo3_Xbox
				r_name = "Halo3_Xbox_StringId.xml";

				TestNewStringIdManagerResources(r_path, r_name);
			}

			{ // HaloOdst_Xbox
				r_path = Managers.GameManager.GetRelativePath(Managers.GameManager.Namespace.HaloOdst, Managers.GameManager.Platform.Xbox, Managers.GameManager.PlatformFolder.Definitions);
				r_name = "HaloOdst_Xbox_StringId.xml";

				TestNewStringIdManagerResources(r_path, r_name);
			}
			#endregion

			#region Halo Reach
			r_path = Managers.GameManager.GetRelativePath(Managers.GameManager.Namespace.HaloReach, Managers.GameManager.Platform.Xbox, Managers.GameManager.PlatformFolder.Definitions);

			{ // HaloReach_Beta
				r_name = "HaloReach_Beta_StringId.xml";

				TestNewStringIdManagerResources(r_path, r_name);
			}
			{ // HaloReach_Xbox. HAVEN'T FINISHED RETAIL'S STRING IDS YET!
//				r_name = "HaloReach_Xbox_StringId.xml";

//				TestNewStringIdManagerResources(r_path, r_name);
			}
			#endregion
		}
	};
}