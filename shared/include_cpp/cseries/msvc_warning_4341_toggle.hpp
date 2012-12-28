#ifndef __MSVC_WARNING_4341_TOGGLE
	#define __MSVC_WARNING_4341_TOGGLE
	
	// TODO: remove this pragma warning off in any later migrations newer (VS2010 and later) VS versions
	// http://connect.microsoft.com/VisualStudio/feedback/details/674442/enumeration-with-negative-values-are-popping-c4341-warnings
	#pragma warning( push )
	#pragma warning( disable : 4341 ) // signed value is out of range for enum constant
#else
	#undef __MSVC_WARNING_4341_TOGGLE

	#pragma warning( pop )
#endif