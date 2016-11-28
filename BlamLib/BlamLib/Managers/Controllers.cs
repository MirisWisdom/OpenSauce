/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
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