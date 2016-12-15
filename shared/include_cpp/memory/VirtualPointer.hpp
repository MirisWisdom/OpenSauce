/*
	Kornner Studios: Shared Code

	See license\Shared for specific license information
*/
#pragma once

#include <boost/preprocessor.hpp>
#include <boost/static_assert.hpp>

#include <boost/cstdint.hpp>
#include <boost/assert.hpp>

#include <cseries/MacrosCpp.hpp>
#include <cseries/MacrosClr.hpp>

__CPP_CODE_START__

class c_pointer_interop_base
{
private:
	void operator==( c_pointer_interop_base const& ) const;
	void operator!=( c_pointer_interop_base const& ) const;

protected:
	static const boost::uint32_t k_max_32bit_address = 
		#if defined(_WIN64)
				// 64-bit kernels allow us to allocate anywhere in the 32-bit range (as long as it's after the first 64KB)
				std::numeric_limits<boost::uint32_t>::max();
		#else
				0x7FFEFFFF;
		#endif

public:
	// Default address used for high-memory emulation
	static const boost::uint32_t k_emulated_memory_base_address = 0x40000000;

	static void test();
};

// Generic traits for Blam engine virtual pointers
struct s_blam_virtual_pointer_base_traits : protected c_pointer_interop_base
{
	static const boost::uint32_t k_emulated_memory_base_address =	0x40000000;

protected:
	// Generate mask used to get the pointer without the base address bits. Enables easier rebasing code
	template<class pointer_traits>
	struct generate_address_mask
	{
		static const boost::uint32_t value = ~pointer_traits::k_base_address;
	};

	// If this is true, we're emulating a memory range at a different base address.
	// If this is false, nothing is emulated and the pointer value is treated normally 
	// however, the value is always contained in 32-bit space but when accessed in 64-bit 
	// environments, gets extended to 64-bit space
	template<class pointer_traits>
	struct emulate_high_memory
	{
		static const bool value = pointer_traits::k_base_address > k_max_32bit_address;
	};
};
// Virtual pointer traits for Xbox 1 pointers
struct s_blam_virtual_pointer_xbox_traits : public s_blam_virtual_pointer_base_traits
{
	// Address of game-state data on the Xbox
	// Note: We adjust this value to work with [k_emulated_memory_base_address]
	static const boost::uint32_t k_base_address =	0x80061000 - 0x61000;
	static const boost::uint32_t k_address_mask =	generate_address_mask<s_blam_virtual_pointer_xbox_traits>::value;

	static const bool k_emulate_high_memory =		emulate_high_memory<s_blam_virtual_pointer_xbox_traits>::value;
};
// Virtual pointer traits for Xbox 360 pointers
struct s_blam_virtual_pointer_xenon_traits : public s_blam_virtual_pointer_base_traits
{
	// Address of game-state data on the Xbox 360
	static const boost::uint32_t k_base_address =	0xA6000000;
	static const boost::uint32_t k_address_mask =	generate_address_mask<s_blam_virtual_pointer_xenon_traits>::value;

	static const bool k_emulate_high_memory =		emulate_high_memory<s_blam_virtual_pointer_xenon_traits>::value;
};

// Provides a guaranteed 32-bit storage interface for pointer interop, even when producing 64-bit code.
template<typename T, class pointer_traits>
class c_virtual_pointer32 : public c_pointer_interop_base
{
private:
	typedef c_virtual_pointer32<T, pointer_traits> this_type;

public:
	typedef T element_type;

	union {
		boost::uint32_t px;

		boost::uint32_t m_value;
	};

public:
	explicit c_virtual_pointer32( T* p = NULL ) // never throws
	{
		this->set(p)
	}

	void reset(T* p = NULL) // never throws
	{
		BOOST_ASSERT( p == 0 || p != px ); // catch self-reset errors
		this_type(p).swap(*this);
	}

#if 0
	T& operator*() const // never throws
	{
		BOOST_ASSERT( m_value != NULL );
		return *this->get();
	}
#endif

	T* operator->() const // never throws
	{
		BOOST_ASSERT( m_value != NULL );
		return this->get();
	}

	T* get() const // never throws
	{
		if(pointer_traits::k_emulate_high_memory)
			return CAST_PTR(T*, (m_value & pointer_traits::k_address_mask) + pointer_traits::k_emulated_memory_base_address);
		
		return CAST_PTR(T*, m_value);
	}
	void set_raw(const void* p)
	{
		m_value = pointer_traits::k_emulate_high_memory ?
			(CAST_PTR(boost::uint32_t, p) & pointer_traits::k_address_mask) + pointer_traits::k_base_address
		:
			CAST_PTR(boost::uint32_t, p);
	}
	inline void set(const T* p)
	{
		set_raw(p);
	}

// implicit conversion to "bool"
#include <boost/smart_ptr/detail/operator_bool.hpp>

	void swap(c_virtual_pointer32& b) // never throws
	{
		boost::uint32_t tmp = b.m_value;
		b.m_value = m_value;
		m_value = tmp;
	}

	// If for some reason we want to set a pointer via '=', we want to make sure it stays in
	// the format we're expecting
	inline const this_type& operator=(const T* p)
	{
		set(p);

		return *this;
	}
};

template<typename T, class pointer_traits>
inline void swap(c_virtual_pointer32<T, pointer_traits>& a, c_virtual_pointer32<T, pointer_traits>& b) // never throws
{
	a.swap(b);
}

// get_pointer(p) is a generic way to say p.get()
template<typename T, class pointer_traits>
inline T* get_pointer(c_virtual_pointer32<T, pointer_traits> const& p)
{
	return p.get();
}


#if 0
template<typename T, const boost::uint32_t k_base_address>
class c_virtual_pointer : public c_pointer_interop_base
{
private:
	typedef c_virtual_pointer<T, k_address_mask, k_base_address> this_type;

public:
	static const boost::uint32_t k_address_mask =	~k_base_address;

	union {
		T* px;

		boost::uint32_t m_value;
	};

	void reset(T* p = NULL) // never throws
	{
		BOOST_ASSERT( p == 0 || p != px ); // catch self-reset errors
		this_type(p).swap(*this);
	}

	T& operator*() const // never throws
	{
		BOOST_ASSERT( px != NULL );
		return *px;
	}

	T* operator->() const // never throws
	{
		BOOST_ASSERT( px != NULL );
		return px;
	}

	T* get() const // never throws
	{
		return px;
	}

// implicit conversion to "bool"
#include <boost/smart_ptr/detail/operator_bool.hpp>

	void swap(c_virtual_pointer& b) // never throws
	{
		T* tmp = b.px;
		b.px = px;
		px = tmp;
	}
};

template<typename T, const boost::uint32_t k_base_address>
inline void swap(c_virtual_pointer<T, k_base_address>& a, c_virtual_pointer<T, k_base_address>& b) // never throws
{
	a.swap(b);
}

// get_pointer(p) is a generic way to say p.get()
template<typename T, const boost::uint32_t k_base_address>
inline T* get_pointer(c_virtual_pointer<T, k_base_address> const& p)
{
	return p.get();
}
#endif

__CPP_CODE_END__