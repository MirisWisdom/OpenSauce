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
using System;
using System.Threading;

namespace BlamLib.Blam.HaloOdst
{
	/// <summary>
	/// Halo Odst game definition implementation
	/// </summary>
	public sealed class GameDefinition : Managers.BlamDefinition, Managers.IStringIdController, Managers.IScriptingController, Managers.IVertexBufferController
	{
		#region Implementation
		public override TagInterface.TagGroupCollection TagGroups	{ get { return HaloOdst.TagGroups.Groups; } }

		/// <remarks>Explicit resource identifying. Yes, there are reasons for this. Ask km00 if you care that much</remarks>
		internal override void IdentifyResourceProc(Managers.BlamDefinition.Game owner, string resource_name, string resource_path)
		{
			bool add_rsrc = false;

 			switch(owner.Engine)
 			{
					case BlamVersion.HaloOdst_Xbox:
					switch (resource_name)
					{
						case Managers.BlamDefinition.ResourceScripts:
						case Managers.BlamDefinition.ResourceStringIds:
						case Managers.BlamDefinition.ResourceVertexBuffers: add_rsrc = true; break;
					}
					break;
// 				case BlamVersion.HaloOdst_PC:
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
					gr = new Managers.StringIdManager();
					result = gr.Load(r_path, r_name);
					break;

				case Managers.BlamDefinition.ResourceVertexBuffers:
					gr = new Render.VertexBufferInterface.VertexBuffersHalo3();
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

		internal protected override Blam.Cache.BuilderBase ConstructCacheBuilder(BlamVersion game)
		{
			Blam.Cache.BuilderBase cb = null;

			if ((game & BlamVersion.Halo3) != 0) // TODO: odst
			{
				cb = new Halo3.Builder();
			}

			return cb;
		}

		internal protected override Blam.CacheFile LoadCacheFile(BlamVersion game, string file_path, bool is_resource)
		{
			Blam.CacheFile cf = null;

			if ((game & BlamVersion.HaloOdst) != 0)
			{
//				if (is_resource)
//					return null;
				/*else*/ cf = new HaloOdst.CacheFile(file_path);
			}

			return cf;
		}

		public override BlamLib.Managers.TagDatabase CreateTagDatabase() { throw new NotSupportedException();/*return new HaloOdst.Tags.TagDatabase()*/; }

		protected override BlamLib.Managers.CacheTagDatabase CreateCacheTagDatabaseInternal(DatumIndex cache_id) { throw new NotSupportedException();/*return new HaloOdst.Tags.CacheTagDatabase((HaloOdst.CacheFile)Program.GetCacheFile(cache_id))*/; }

		public override BlamLib.Managers.ErrorTagDatabase CreateErrorTagDatabase() { throw new NotSupportedException();/*return new HaloOdst.Tags.ErrorTagDatabase()*/; }

		public override BlamLib.TagInterface.TagGroup TagDatabaseGroup { get { throw new NotSupportedException();/*return Halo3.TagGroups.tag_*/; } }
		#endregion

		internal GameDefinition() {}

		#region IStringIdController Members
		// Sure, we could use a hashtable for keeping references and such, but this uses way less memory, 
		// and allows us to use the method logic below and make sure we're not trying to implement any unsupported
		// engine variants. Savvy?
		int StringIdCacheReferencesXbox = 0
//			, StringIdCacheReferencesPC = 0 
			;

		/// <summary>
		/// <see cref="BlamLib.Managers.IStringIdController"/>
		/// </summary>
		/// <param name="game"></param>
		/// <returns></returns>
		public bool StringIdCacheOpen(BlamVersion game)
		{
			int count = 0;

			switch (game)
			{
				case BlamVersion.HaloOdst_Xbox:	count = Interlocked.Increment(ref StringIdCacheReferencesXbox);		break;
//				case BlamVersion.HaloOdst_PC:	count = Interlocked.Increment(ref StringIdCacheReferencesPC);		break;

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
		public bool StringIdCacheClose(BlamVersion game)
		{
			int count = -1;

			switch (game)
			{
				case BlamVersion.HaloOdst_Xbox:	count = Interlocked.Decrement(ref StringIdCacheReferencesXbox);		break;
//				case BlamVersion.HaloOdst_PC:	count = Interlocked.Decrement(ref StringIdCacheReferencesPC);		break;

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
//			, ScriptingCacheReferencesPC = 0
			;

		/// <summary>
		/// <see cref="BlamLib.Managers.IScriptingController"/>
		/// </summary>
		/// <param name="game"></param>
		/// <returns></returns>
		public bool ScriptingCacheOpen(BlamVersion game)
		{
			int count = 0;

			switch (game)
			{
				case BlamVersion.HaloOdst_Xbox:	count = Interlocked.Increment(ref ScriptingCacheReferencesXbox);	break;
//				case BlamVersion.HaloOdst_PC:	count = Interlocked.Increment(ref ScriptingCacheReferencesPC);		break;

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
		public bool ScriptingCacheClose(BlamVersion game)
		{
			int count = -1;

			switch (game)
			{
				case BlamVersion.HaloOdst_Xbox:	count = Interlocked.Decrement(ref ScriptingCacheReferencesXbox);	break;
//				case BlamVersion.HaloOdst_PC:	count = Interlocked.Decrement(ref ScriptingCacheReferencesPC);		break;

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
//			, VertexBufferCacheReferencesPC = 0
			;

		/// <summary>
		/// <see cref="BlamLib.Managers.IVertexBufferController"/>
		/// </summary>
		/// <param name="game"></param>
		/// <returns></returns>
		public bool VertexBufferCacheOpen(BlamVersion game)
		{
			int count = 0;

			switch (game)
			{
				case BlamVersion.HaloOdst_Xbox:	count = Interlocked.Increment(ref VertexBufferCacheReferencesXbox);	break;
//				case BlamVersion.HaloOdst_PC:	count = Interlocked.Increment(ref VertexBufferCacheReferencesPC);	break;

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
		public bool VertexBufferCacheClose(BlamVersion game)
		{
			int count = -1;

			switch (game)
			{
				case BlamVersion.HaloOdst_Xbox:	count = Interlocked.Decrement(ref VertexBufferCacheReferencesXbox);	break;
//				case BlamVersion.HaloOdst_PC:	count = Interlocked.Decrement(ref VertexBufferCacheReferencesPC);		break;

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
	};
}