/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
using System;
using Crypt = System.Security.Cryptography;
using TI = BlamLib.TagInterface;

namespace BlamLib.Managers
{
	public abstract class BlamDefinitionGen3 : Managers.BlamDefinition, Managers.IStringIdController, Managers.IScriptingController, Managers.IVertexBufferController
	{
		protected class AesInputs
		{
			public byte[] Root;
			public byte[] Key;
			public byte[] Iv;

			static void CalculateInputs(byte[] root, out byte[] key, out byte[] iv)
			{
				key = new byte[root.Length];
				iv = new byte[root.Length];

				for (int x = 0; x < root.Length; x++)
				{
					key[x] = (byte)(root[x] ^ 0xFFA5);
					iv[x] = (byte)(key[x] ^ 0x3C);
				}
			}
			public AesInputs(string root)
			{
				Root = System.Text.Encoding.ASCII.GetBytes(root);

				CalculateInputs(Root, out Key, out Iv);
			}
		};

		#region Implementation
		protected internal override Blam.Cache.BuilderBase ConstructCacheBuilder(BlamVersion game)	{ throw new NotSupportedException(); }
		public override TagDatabase CreateTagDatabase()					{ throw new NotSupportedException(); }
		protected override CacheTagDatabase CreateCacheTagDatabaseInternal(Blam.DatumIndex cache_id) { throw new NotSupportedException(); }
		public override ErrorTagDatabase CreateErrorTagDatabase()		{ throw new NotSupportedException(); }
		public override BlamLib.TagInterface.TagGroup TagDatabaseGroup	{ get { throw new NotSupportedException(); } }
		#endregion

		#region IStringIdController Members
		public abstract bool StringIdCacheOpen(BlamVersion game);
		public abstract bool StringIdCacheClose(BlamVersion game);
		#endregion

		#region IScriptingController Members
		public abstract bool ScriptingCacheOpen(BlamVersion game);
		public abstract bool ScriptingCacheClose(BlamVersion game);
		#endregion

		#region IVertexBufferController Members
		public abstract bool VertexBufferCacheOpen(BlamVersion game);
		public abstract bool VertexBufferCacheClose(BlamVersion game);
		#endregion

		protected delegate void GetAesParametersProc(BlamVersion game, Blam.CacheSectionType type, out byte[] key, out byte[] iv);
		protected static void SecurityAesDecrypt(BlamVersion game, Blam.CacheSectionType section_type, byte[] input, out byte[] output,
			GetAesParametersProc GetAesParameters)
		{
			output = null;

			using (var aesm = new Crypt.AesManaged())
			{
				aesm.KeySize = 128;
				aesm.Padding = Crypt.PaddingMode.Zeros;
				aesm.Mode = Crypt.CipherMode.CBC;

				byte[] key, iv;
				GetAesParameters(game, section_type, out key, out iv);

				if (key != null && iv != null)
					using (var ctx = aesm.CreateDecryptor(key, iv))
					{
						output = ctx.TransformFinalBlock(input, 0, input.Length);
					}
			}
		}
	};
}