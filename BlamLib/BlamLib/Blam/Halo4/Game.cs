/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
using System;
using System.Threading;
using TI = BlamLib.TagInterface;

namespace BlamLib.Blam.Halo4
{
#pragma warning disable 1591 // "Missing XML comment for publicly visible type or member"
	/// <summary>
	/// Supported languages
	/// </summary>
	public enum LanguageType
	{
		English,
		Japanese,
		German,
		French,
		Spanish,
		Italian,
		Korean,
		/// <summary>
		/// Traditional
		/// </summary>
		Chinese,
		Portuguese,
		Polish,

		Russian,
		Danish,
		Finnish,
		Dutch,
		Norwegian,

		kMax
	};
#pragma warning restore 1591 // "Missing XML comment for publicly visible type or member"

	/// <summary>
	/// Halo 4 game definition implementation
	/// </summary>
	public sealed class GameDefinition : Managers.BlamDefinitionGen3
	{
		#region Implementation
		public override TI.TagGroupCollection TagGroups						{ get { return Halo4.TagGroups.Groups; } }
		//public override TI.TagGroupCollection TagGroupsInvalidForCacheViewer{ get { return HaloReach.TagGroups.GroupsInvalidForCacheViewer; } }
		public override TI.TagGroupCollection TagGroupsInvalidForExtraction	{ get { return Halo4.TagGroups.Groups; } }

		/// <remarks>Explicit resource identifying. Yes, there are reasons for this. Ask km00 if you care that much</remarks>
		internal override void IdentifyResourceProc(Managers.BlamDefinition.Game owner, string resource_name, string resource_path)
		{
			bool add_rsrc = false;

 			switch(owner.Engine)
 			{
					case BlamVersion.Halo4_Xbox:
					switch (resource_name)
					{
						case Managers.BlamDefinition.ResourceScripts:
						case Managers.BlamDefinition.ResourceStringIds:
						case Managers.BlamDefinition.ResourceVertexBuffers: add_rsrc = true; break;
					}
					break;
// 				case BlamVersion.Halo4_PC:
// 					switch (resource_name)
// 					{
// 						case Managers.BlamDefinition.ResourceScripts:
// 						case Managers.BlamDefinition.ResourceStringIds:	add_rsrc = true;	break;
// 					}
// 					break;

				default: throw new Debug.Exceptions.UnreachableException();
 			}

			if (add_rsrc)
				owner.AddResourceLocation(resource_name, resource_path);
		}

		internal protected override IGameResource PrecacheResource(Game owner, string resource_name, string r_path, string r_name)
		{
			IGameResource gr = null;
			bool result = false;

			switch (resource_name)
			{
				case Managers.BlamDefinition.ResourceScripts:
					gr = new Scripting.XmlInterface();
					result = gr.Load(r_path, r_name);
					break;

				case Managers.BlamDefinition.ResourceStringIds:
					gr = new Managers.StringIdStaticCollection();
					result = gr.Load(r_path, r_name);
					break;

				case Managers.BlamDefinition.ResourceVertexBuffers:
					gr = new Render.VertexBufferInterface.VertexBuffersGen3();
					result = gr.Load(r_path, r_name);
					break;
			}

			if (!result && gr != null)
			{
				gr.Close();
				gr = null;
			}

			return gr;
		}

		internal protected override Blam.CacheFile LoadCacheFile(BlamVersion game, string file_path, bool is_resource)
		{
			Blam.CacheFile cf = null;

			if ((game & BlamVersion.Halo4) != 0)
			{
//				if (is_resource)
//					return null;
				/*else*/ cf = new Halo4.CacheFile(file_path);
			}

			return cf;
		}

		public override Blam.CacheFile GetCacheFileFromLocation(BlamVersion ver, string cache_name) { return Program.Halo4.FromLocation(ver, cache_name); }
		public override Blam.CacheFile GetCacheFileFromLocation(BlamVersion ver, string cache_name, out bool is_internal) { return Program.Halo4.FromLocation(ver, cache_name, out is_internal); }
		#endregion

		internal GameDefinition() { }

		#region IStringIdController Members
		// Sure, we could use a hashtable for keeping references and such, but this uses way less memory, 
		// and allows us to use the method logic below and make sure we're not trying to implement any unsupported
		// engine variants. Savvy?
		int StringIdCacheReferencesXbox = 0 
//			StringIdCacheReferencesPC = 0, 
			;

		/// <summary>
		/// <see cref="BlamLib.Managers.IStringIdController"/>
		/// </summary>
		/// <param name="game"></param>
		/// <returns></returns>
		public override bool StringIdCacheOpen(BlamVersion game)
		{
			int count = 0;

			switch (game)
			{
				case BlamVersion.Halo4_Xbox:count = Interlocked.Increment(ref StringIdCacheReferencesXbox);		break;
//				case BlamVersion.Halo4_PC:	count = Interlocked.Increment(ref StringIdCacheReferencesPC);		break;

				default: throw new Debug.Exceptions.UnreachableException();
			}

			if(count == 1)
			{
				base.PrecacheResource(game, Managers.BlamDefinition.ResourceStringIds);
				return true;
			}
			else if (count == 0) throw new Debug.Exceptions.UnreachableException();

			return false;
		}

		/// <summary>
		/// <see cref="BlamLib.Managers.IStringIdController"/>
		/// </summary>
		/// <param name="game"></param>
		/// <returns></returns>
		public override bool StringIdCacheClose(BlamVersion game)
		{
			int count = -1;

			switch (game)
			{
				case BlamVersion.Halo4_Xbox:count = Interlocked.Decrement(ref StringIdCacheReferencesXbox);		break;
//				case BlamVersion.Halo4_PC:	count = Interlocked.Decrement(ref StringIdCacheReferencesPC);		break;

				default: throw new Debug.Exceptions.UnreachableException();
			}

			if(count == 0) // since it's pre-decrement assigned, it will equal to zero when nothing is using it anymore
			{
				base.CloseResource(game, Managers.BlamDefinition.ResourceStringIds);
				return true;
			}
			else if (count == -1) throw new Debug.Exceptions.UnreachableException();

			return false;
		}
		#endregion

		#region IScriptingController Members
		int ScriptingCacheReferencesXbox = 0
//			ScriptingCacheReferencesPC = 0,
			;

		/// <summary>
		/// <see cref="BlamLib.Managers.IScriptingController"/>
		/// </summary>
		/// <param name="game"></param>
		/// <returns></returns>
		public override bool ScriptingCacheOpen(BlamVersion game)
		{
			int count = 0;

			switch (game)
			{
				case BlamVersion.Halo4_Xbox:count = Interlocked.Increment(ref ScriptingCacheReferencesXbox);	break;
//				case BlamVersion.Halo4_PC:	count = Interlocked.Increment(ref ScriptingCacheReferencesPC);		break;

				default: throw new Debug.Exceptions.UnreachableException();
			}

			if(count == 1)
			{
				base.PrecacheResource(game, Managers.BlamDefinition.ResourceScripts);
				return true;
			}
			else if (count == 0) throw new Debug.Exceptions.UnreachableException();

			return false;
		}

		/// <summary>
		/// <see cref="BlamLib.Managers.IScriptingController"/>
		/// </summary>
		/// <param name="game"></param>
		/// <returns></returns>
		public override bool ScriptingCacheClose(BlamVersion game)
		{
			int count = -1;

			switch (game)
			{
				case BlamVersion.Halo4_Xbox:count = Interlocked.Decrement(ref ScriptingCacheReferencesXbox);	break;
//				case BlamVersion.Halo4_PC:	count = Interlocked.Decrement(ref ScriptingCacheReferencesPC);		break;

				default: throw new Debug.Exceptions.UnreachableException();
			}

			if(count == 0) // since it's pre-decrement assigned, it will equal to zero when nothing is using it anymore
			{
				base.CloseResource(game, Managers.BlamDefinition.ResourceScripts);
				return true;
			}
			else if (count == -1) throw new Debug.Exceptions.UnreachableException();

			return false;
		}
		#endregion

		#region IVertexBufferController Members
		// Sure, we could use a hashtable for keeping references and such, but this uses way less memory, 
		// and allows us to use the method logic below and make sure we're not trying to implement any unsupported
		// engine variants. Savvy?
		int VertexBufferCacheReferencesXbox = 0 
//			VertexBufferCacheReferencesPC = 0, 
			;

		/// <summary>
		/// <see cref="BlamLib.Managers.IVertexBufferController"/>
		/// </summary>
		/// <param name="game"></param>
		/// <returns></returns>
		public override bool VertexBufferCacheOpen(BlamVersion game)
		{
			int count = 0;

			switch (game)
			{
				case BlamVersion.Halo4_Xbox:count = Interlocked.Increment(ref VertexBufferCacheReferencesXbox);		break;
//				case BlamVersion.Halo4_PC:	count = Interlocked.Increment(ref VertexBufferCacheReferencesPC);		break;

				default: throw new Debug.Exceptions.UnreachableException();
			}

			if(count == 1)
			{
				base.PrecacheResource(game, Managers.BlamDefinition.ResourceVertexBuffers);
				return true;
			}
			else if (count == 0) throw new Debug.Exceptions.UnreachableException();

			return false;
		}

		/// <summary>
		/// <see cref="BlamLib.Managers.IVertexBufferController"/>
		/// </summary>
		/// <param name="game"></param>
		/// <returns></returns>
		public override bool VertexBufferCacheClose(BlamVersion game)
		{
			int count = -1;

			switch (game)
			{
				case BlamVersion.Halo4_Xbox:count = Interlocked.Decrement(ref VertexBufferCacheReferencesXbox);		break;
//				case BlamVersion.Halo4_PC:	count = Interlocked.Decrement(ref VertexBufferCacheReferencesPC);		break;

				default: throw new Debug.Exceptions.UnreachableException();
			}

			if(count == 0) // since it's pre-decrement assigned, it will equal to zero when nothing is using it anymore
			{
				base.CloseResource(game, Managers.BlamDefinition.ResourceVertexBuffers);
				return true;
			}
			else if (count == -1) throw new Debug.Exceptions.UnreachableException();

			return false;
		}
		#endregion

		static readonly AesInputs[] kAesRetail = {
			new AesInputs("BungieHaloReach!"), // Localization
			new AesInputs("ILikeSafeStrings"), // Strings
			new AesInputs("LetsAllPlayNice!"), // Tag names
			new AesInputs("SneakerNetReigns"), // Networking
		};
		static void GetAesParameters(BlamVersion game, CacheSectionType type, out byte[] key, out byte[] iv)
		{
			switch (game)
			{
				case BlamVersion.Halo4_Xbox:
					if (type == CacheSectionType.Localization)
					{
						key = kAesRetail[0].Key;
						iv = kAesRetail[0].Iv;
					}
					else if (type == CacheSectionType.Debug)
					{
						key = kAesRetail[1].Key;
						iv = kAesRetail[1].Iv;
					}
					else if (type == CacheSectionType.Tag)
					{
						key = kAesRetail[2].Key;
						iv = kAesRetail[2].Iv;
					}
					else goto default;
					break;

				default: throw new Debug.Exceptions.UnreachableException(string.Format("{0}/{1}", game.ToString(), type.ToString()));
			}
		}

		internal static void SecurityAesDecrypt(BlamVersion game, CacheSectionType section_type, byte[] input, out byte[] output)
		{
			SecurityAesDecrypt(game, section_type, input, out output, GetAesParameters);
		}
	};
}