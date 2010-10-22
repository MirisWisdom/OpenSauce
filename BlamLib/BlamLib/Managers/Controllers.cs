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

namespace BlamLib.Managers
{
	// I realize this isn't THE best way to interface the cached resources in games, but 
	// when it comes down to it, I want to make sure the code is touching the reference 
	// counting the right way

	/// <summary>
	/// Implemented by a game (in it's definition class) who uses the string id system
	/// </summary>
	public interface IStringIdController
	{
		/// <summary>
		/// Signal to the definition that the string id cache is needed
		/// </summary>
		/// <param name="game"></param>
		/// <returns>True if the cache was actually loaded for use, false if a reference count was just increased</returns>
		bool StringIdCacheOpen(BlamVersion game);
		/// <summary>
		/// Signal to the definition that the string id cache isn't needed
		/// </summary>
		/// <param name="game"></param>
		/// <returns>True if the cache was actually unloaded from memory, false if a reference count was just decreased</returns>
		bool StringIdCacheClose(BlamVersion game);
	};

	/// <summary>
	/// Implemented by a game (in it's definition class) who uses a scripting system
	/// </summary>
	public interface IScriptingController
	{
		/// <summary>
		/// Signal to the definition that the scripting definition cache is needed
		/// </summary>
		/// <param name="game"></param>
		/// <returns>True if the cache was actually loaded for use, false if a reference count was just increased</returns>
		bool ScriptingCacheOpen(BlamVersion game);
		/// <summary>
		/// Signal to the definition that the scripting definition cache isn't needed
		/// </summary>
		/// <param name="game"></param>
		/// <returns>True if the cache was actually unloaded from memory, false if a reference count was just decreased</returns>
		bool ScriptingCacheClose(BlamVersion game);
	};

	/// <summary>
	/// Implemented by a game (in it's definition class) who uses the Render Vertex Buffer system
	/// </summary>
	public interface IVertexBufferController
	{
		/// <summary>
		/// Signal to the definition that the vertex buffer cache is needed
		/// </summary>
		/// <param name="game"></param>
		/// <returns>True if the cache was actually loaded for use, false if a reference count was just increased</returns>
		bool VertexBufferCacheOpen(BlamVersion game);
		/// <summary>
		/// Signal to the definition that the vertex buffer cache isn't needed
		/// </summary>
		/// <param name="game"></param>
		/// <returns>True if the cache was actually unloaded from memory, false if a reference count was just decreased</returns>
		bool VertexBufferCacheClose(BlamVersion game);
	};
}