/*
	Kornner Studios: Shared Code

	See license\Shared for specific license information
*/
#include "Precompile.hpp"
#include <memory/VirtualPointer.hpp>
__CPP_CODE_START__

typedef c_virtual_pointer32<void, s_blam_virtual_pointer_xenon_traits> blam_pointer_t;

void c_pointer_interop_base::test()
{
	struct s_test
	{
		blam_pointer_t address;
	};
	boost::uint32_t data = 0xBD000000;
	void* data_ptr = &data;

	s_test* test_data = CAST_PTR(s_test*, data_ptr);
	test_data->address.get();
}

__CPP_CODE_END__