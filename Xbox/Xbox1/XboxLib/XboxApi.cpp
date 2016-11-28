/*
	OpenSauceBox: SDK for Xbox User Modding

	See license\Xbox\Xbox for specific license information
*/
#include "XboxLib.hpp"
#include "XboxApi.hpp"

#if defined(__cplusplus)
extern "C" {
#endif

const XBOX_API_VTABLE* gXBOX_API_VTABLE = &XboxApi;

const XBOX_STD_VTABLE* gXBOX_STD_VTABLE = &XboxStd;


static bool XboxApiObjectContainsNullFunctions(const void* _this, size_t size)
{
	if(_this == NULL) return true;

	const void*const* obj = CAST_PTR(const void*const*, _this);

	for(; size > 0; size -= sizeof(PVOID), obj++)
		if(*obj == NULL) return true;

	return false;
}

bool XboxApiContainsNullFunctions()
{
	return XboxApiObjectContainsNullFunctions(gXBOX_API_VTABLE, sizeof(*gXBOX_API_VTABLE));
}

bool XboxStdContainsNullFunctions()
{
	return XboxApiObjectContainsNullFunctions(gXBOX_STD_VTABLE, sizeof(*gXBOX_STD_VTABLE));
}

#if defined(__cplusplus)
};
#endif