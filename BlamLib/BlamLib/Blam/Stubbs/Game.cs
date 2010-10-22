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

namespace BlamLib.Blam.Stubbs
{
	/// <summary>
	/// Stubbs game definition implementation
	/// </summary>
	public sealed class GameDefinition : Managers.BlamDefinition, Managers.IScriptingController
	{
		#region Implementation
		public override TagInterface.TagGroupCollection TagGroups	{ get { return Stubbs.TagGroups.Groups; } }

		/// <remarks>Explicit resource identifying. Yes, there are reasons for this. Ask km00 if you care that much</remarks>
		internal override void IdentifyResourceProc(Managers.BlamDefinition.Game owner, string resource_name, string resource_path)
		{
			bool add_rsrc = false;

 			switch(owner.Engine)
 			{
				case BlamVersion.Stubbs_Xbox:
					switch (resource_name)
					{
						case Managers.BlamDefinition.ResourceScripts: add_rsrc = true; break;
					}
					break;
				case BlamVersion.Stubbs_PC:
					switch (resource_name)
					{
						case Managers.BlamDefinition.ResourceScripts: add_rsrc = true; break;
					}
					break;
				case BlamVersion.Stubbs_Mac:
					switch (resource_name)
					{
						case Managers.BlamDefinition.ResourceScripts: add_rsrc = true; break;
					}
					break;

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

			if ((game & BlamVersion.Stubbs) != 0)
			{
				cb = new Stubbs.Builder();
			}

			return cb;
		}

		internal protected override Blam.CacheFile LoadCacheFile(BlamVersion game, string file_path, bool is_resource)
		{
			Blam.CacheFile cf = null;

			if ((game & BlamVersion.Stubbs) != 0)
			{
				cf = new Stubbs.CacheFile(file_path);
			}

			return cf;
		}

		public override BlamLib.Managers.TagDatabase CreateTagDatabase() { return new Halo1.Tags.TagDatabase(BlamVersion.Stubbs); }

		protected override BlamLib.Managers.CacheTagDatabase CreateCacheTagDatabaseInternal(DatumIndex cache_id) { return new Stubbs.Tags.CacheTagDatabase((Stubbs.CacheFile)Program.GetCacheFile(cache_id)); }

		public override BlamLib.Managers.ErrorTagDatabase CreateErrorTagDatabase() { return new Halo1.Tags.ErrorTagDatabase(BlamVersion.Stubbs); }

		public override BlamLib.TagInterface.TagGroup TagDatabaseGroup { get { return Halo1.TagGroups.tag_; } }
		#endregion

		internal GameDefinition() {}

		#region IScriptingController Members
		int ScriptingCacheReferencesXbox = 0,
			ScriptingCacheReferencesPC = 0,
			ScriptingCacheReferencesMac = 0;

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
				case BlamVersion.Stubbs_Xbox:	count = Interlocked.Increment(ref ScriptingCacheReferencesXbox);	break;
				case BlamVersion.Stubbs_PC:		count = Interlocked.Increment(ref ScriptingCacheReferencesPC);	break;
				case BlamVersion.Stubbs_Mac:	count = Interlocked.Increment(ref ScriptingCacheReferencesMac);	break;

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
				case BlamVersion.Stubbs_Xbox:	count = Interlocked.Decrement(ref ScriptingCacheReferencesXbox);	break;
				case BlamVersion.Stubbs_PC:		count = Interlocked.Decrement(ref ScriptingCacheReferencesPC);	break;
				case BlamVersion.Stubbs_Mac:	count = Interlocked.Decrement(ref ScriptingCacheReferencesMac);	break;

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
	};
}