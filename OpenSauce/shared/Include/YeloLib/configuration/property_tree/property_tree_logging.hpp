/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

//#define PRINT_PROPERTY_TREES

#ifdef PRINT_PROPERTY_TREES

namespace Yelo
{
	namespace Configuration { namespace PropertyTree
	{
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Print a property tree to the debug output. </summary>
		///
		/// <param name="tree"> 	The tree to print. </param>
		/// <param name="depth">	The current recursion depth. Do not use. </param>
		void PrintPropertyTree(const boost::property_tree::ptree& tree, const int depth = 0);
	};};
};
#endif