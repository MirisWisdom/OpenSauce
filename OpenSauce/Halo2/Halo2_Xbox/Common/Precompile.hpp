/*
	Yelo: Open Sauce SDK
		Halo 2 (Xbox) Edition

	See license\OpenSauce\Halo2_Xbox for specific license information
*/
#pragma once

#include <XboxLib.hpp>
#include <XboxApi.hpp>
#include <XboxInterface.hpp>
#include <Std/Math.hpp>

#include <boost/preprocessor.hpp>
#include <boost/static_assert.hpp>
#include <boost/cstdint.hpp>

#include "Common/Platform.hpp"

// Library's function convention
#define API_FUNC __stdcall

#include <blamlib/Halo2/cseries/cseries.hpp>
#include <blamlib/scenario/scenario_location.hpp>
#include <blamlib/Halo2/tag_files/tag_groups.hpp>
#include "Common/BlamTypes.hpp"

#include "Cache/StringsIds.hpp"

#include "Memory/MemoryInterface.hpp"

#if defined(__cplusplus)
extern "C" {
#endif



#if defined(__cplusplus)
};
#endif


#pragma region new/delete hacks
XBOX_STDPROC(void*, new, (
	__in size_t _Size
));

XBOX_STDPROC(void, delete, (
	void* _Ptr
));

XBOX_STDPROC(void*, new_vector, (
	void* _Where,
	size_t _Size,
	size_t _Count
));

XBOX_STDPROC(void, delete_vector, (
	void*,
	void* _Ptr
));

typedef struct _LIB_API_VTABLE {
	_new operator_new;
	_delete operator_delete;
	_new_vector operator_new__;
	_delete_vector operator_delete__;
}LIB_API_VTABLE;
extern LIB_API_VTABLE* gLIB_API_VTABLE;

inline void *__CRTDECL operator new(size_t _Size) { return gLIB_API_VTABLE->operator_new(_Size); }

inline void *__CRTDECL operator new[](size_t _Size, size_t _Count) { return gLIB_API_VTABLE->operator_new__(NULL, _Size, _Count); }

inline void __CRTDECL operator delete(void* Ptr) { return gLIB_API_VTABLE->operator_delete(Ptr); }

inline void __CRTDECL operator delete[](void* Ptr) { return gLIB_API_VTABLE->operator_delete__(0, Ptr); }
#pragma endregion