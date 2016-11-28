/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
/*
	Based on "xma_parse" code written by HCS (http://hcs64.com/contact.html)
	Used with permission.
*/
#include "Precompile.hpp"
#include "XMA/XmaUtil.hpp"

#ifdef _DEBUG
__CPP_CODE_START__

const boost::uint32_t k_bits_per_byte = 8;

namespace XMA
{
	c_bit_istream::c_bit_istream(std::istream& stream, boost::uint32_t consecutive_bits, boost::uint32_t skip_bits) :
		m_stream(stream),
		k_consecutive_bits(consecutive_bits),
		k_skip_bits(skip_bits),
		m_consecutive_bits_left(consecutive_bits),
		m_bits_left(0), m_bit_buffer(0)
	{
		if(std::numeric_limits<boost::uint8_t>::digits != k_bits_per_byte)
			throw invalid_byte_size_exception();
	}

	bool c_bit_istream::get_bit()
	{
		if(k_consecutive_bits && m_consecutive_bits_left == 0)
		{
			m_consecutive_bits_left = k_skip_bits;
			// recursive call to get, yet throw away, bits
			while(m_consecutive_bits_left > 0)
				get_bit();
		}

		if(m_bits_left == 0)
		{
			std::istream::int_type c = m_stream.get();
			if(c == EOF)
				throw out_of_bits_exception();
			m_bit_buffer = c;
			m_bits_left = k_bits_per_byte;
		}

		m_bits_left--;
		if(k_consecutive_bits > 0) m_consecutive_bits_left--;

		return (m_bit_buffer & (1<<m_bits_left)) != 0;
	}


	c_bit_ostream::c_bit_ostream(std::ostream& stream) : 
		m_stream(stream),
		m_bits_stored(0), m_bit_buffer(0)
	{
		if(std::numeric_limits<boost::uint8_t>::digits != k_bits_per_byte)
			throw invalid_byte_size_exception();
	}

	c_bit_ostream::~c_bit_ostream()
	{
		flush();
	}

	void c_bit_ostream::put_bit(bool bit)
	{
		m_bit_buffer <<= 1;
		if(bit)
			m_bit_buffer |= 1;

		m_bits_stored++;
		if(m_bits_stored == k_bits_per_byte)
			flush();
	}

	void c_bit_ostream::flush()
	{
		if(m_bits_stored != 0)
		{
			m_stream << m_bit_buffer;
			m_bits_stored = 0;
			m_bit_buffer = 0;
		}
	}
};

__CPP_CODE_END__

#endif