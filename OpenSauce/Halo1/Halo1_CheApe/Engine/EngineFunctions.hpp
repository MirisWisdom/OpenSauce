/*
	Yelo: Open Sauce SDK
		Halo 1 (Editing Kit) Edition

	See license\OpenSauce\Halo1_CheApe for specific license information
*/
#pragma once

#include <blamlib/Halo1/cseries/debug_memory.hpp>
#include <blamlib/Halo1/cseries/errors.hpp>

namespace Yelo
{
	namespace Engine
	{
		// NOTE: command line parameters are not populated in guerilla nor tool.exe
		// So basically only use this in sapien builds
		bool GetCmdLineParameter(cstring parameter, __out_opt cstring* value_out = nullptr);
	};
};

// NOTE: it is not advisable to actually use the ARRAY variants with types which define their own destructors

#define YELO_MALLOC(size, fill_with_garbage)	Yelo::blam::debug_malloc( (size), fill_with_garbage, __FILE__, __LINE__)
#define YELO_NEW_ARRAY(type, count)				Yelo::blam::debug_new_array<type>( (count), false, __FILE__, __LINE__)
#define YELO_NEW(type)							Yelo::blam::debug_new<type>(false, __FILE__, __LINE__)
#define YELO_NEW_CTOR(type, ...)				new ( YELO_NEW(type) ) type(__VA_ARGS__)

#define YELO_MALLOC_UNIQUE(size, fill_with_garbage)			\
	std::unique_ptr<void,	Yelo::Engine::debug_memory_deleter<void>>		( YELO_MALLOC(size, fill_with_garbage) )
#define YELO_NEW_ARRAY_UNIQUE(type, count)					\
	std::unique_ptr<type[],	Yelo::Engine::debug_memory_array_deleter<type>>	( YELO_NEW_ARRAY(type, count) )
#define YELO_NEW_UNIQUE(type)								\
	std::unique_ptr<type,	Yelo::Engine::debug_memory_deleter<type>>		( YELO_NEW(type) )
#define YELO_NEW_UNIQUE_CTOR(type, ...)						\
	std::unique_ptr<type,	Yelo::Engine::debug_memory_deleter<type>>		(std::move( YELO_NEW_CTOR(type, __VA_ARGS__) ))

// Nulls [pointer] before returning
#define YELO_FREE(ptr)							Yelo::blam::debug_free_with_null( (ptr), __FILE__, __LINE__)
// Nulls [ptr] before returning
#define YELO_DELETE(ptr)						Yelo::blam::debug_delete( (ptr), __FILE__, __LINE__ )
// Nulls [ptr] before returning
#define YELO_DELETE_ARRAY(ptr)					Yelo::blam::debug_delete_array( (ptr), __FILE__, __LINE__ )

#define YELO_REALLOC(ptr, new_size)				Yelo::blam::debug_realloc( (ptr), (new_size), __FILE__, __LINE__)
#define YELO_RENEW_ARRAY(type, ptr, count)		Yelo::blam::debug_renew_array<type>( (ptr), (count), __FILE__, __LINE__)
#define YELO_RENEW(type, ptr)					Yelo::blam::debug_renew<type>( (ptr), __FILE__, __LINE__)

namespace Yelo
{
	namespace Engine
	{
		// Primarily a deleter for std::unique_ptr
		template<typename T>
		struct debug_memory_deleter
		{
			inline void operator()(T* ptr) const	{ YELO_DELETE(ptr); }
		};
		template<typename T>
		struct debug_memory_array_deleter
		{
			inline void operator()(T* ptr) const	{ YELO_DELETE_ARRAY(ptr); }
		};

		// based on http://blogs.msdn.com/b/vcblog/archive/2008/08/28/the-mallocator.aspx
		// TODO: needs to be updated to comply with C++11
		// http://en.cppreference.com/w/cpp/concept/Allocator
		template<typename T>
		struct debug_memory_stl_allocator
		{
			typedef T         value_type;
			typedef size_t    size_type;
			typedef ptrdiff_t difference_type;
			typedef T*        pointer;
			typedef const T*  const_pointer;
			typedef T&        reference;
			typedef const T&  const_reference;

			pointer address(reference r) const { return &r; };
			const_pointer address(const_reference r) const { return &r; };

			size_type max_size() const {
				// The following has been carefully written to be independent of
				// the definition of size_t and to avoid signed/unsigned warnings.
				return (static_cast<size_type>(0) - static_cast<size_type>(1)) / sizeof(T);
			}

			// The following must be the same for all allocators.
			template <typename U>
			struct rebind {
				typedef debug_memory_stl_allocator<U> other;
			};

			bool operator!=(const debug_memory_stl_allocator& other) const {
				return !(*this == other);
			}
			// Returns true if and only if storage allocated from *this
			// can be deallocated from other, and vice versa.
			// Always returns true for stateless allocators.
			bool operator==(const debug_memory_stl_allocator& other) const {
				return true;
			}


			// Default constructor, copy constructor, rebinding constructor, and destructor.
			// Empty for stateless allocators.
			debug_memory_stl_allocator() { }

			debug_memory_stl_allocator(const debug_memory_stl_allocator&) { } 

			template <typename U>
			debug_memory_stl_allocator(const debug_memory_stl_allocator<U>&) { }

			~debug_memory_stl_allocator() { }


			void construct(pointer const p, const_reference t) const {
				void* const pv = CAST(void*, p);

				new (pv) T(t);
			}

			void destroy(pointer const p) const {
				p->~T();
			}

			pointer allocate(const size_type n) const {
				// The return value of allocate(0) is unspecified.
				// We return NULL in order to avoid depending
				// on malloc(0)'s implementation-defined behavior
				// (the implementation can define malloc(0) to return NULL,
				// in which case the bad_alloc check below would fire).
				// All allocators can return NULL in this case.
				if (n == 0)
					return nullptr;

				// All allocators should contain an integer overflow check.
				// The Standardization Committee recommends that std::length_error
				// be thrown in the case of integer overflow.
				if (n > max_size()) {
					throw std::length_error(__FUNCTION__ " - Integer overflow.");
				}

				void * const pv = YELO_MALLOC(n * sizeof(T), false);

				// Allocators should throw std::bad_alloc in the case of memory allocation failure.
				if (pv == nullptr)
					throw std::bad_alloc();

				return static_cast<pointer>(pv);
			}

			void deallocate(pointer const p, const size_type n) const {
				YELO_FREE(p);
			}

			// The following will be the same for all allocators that ignore hints.
			template <typename U>
			pointer allocate(const size_type n, const U* /* const hint */) const {
				return allocate(n);
			}

			// Allocators are not required to be assignable
			debug_memory_stl_allocator& operator=(const debug_memory_stl_allocator&) = delete;
		};
	};
}