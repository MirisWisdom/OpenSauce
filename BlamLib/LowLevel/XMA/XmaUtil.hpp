/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
/*
	Based on "xma_parse" code written by HCS (http://hcs64.com/contact.html)
	Used with permission.
*/
#pragma once

#ifdef _DEBUG

__CPP_CODE_START__

extern const boost::uint32_t k_bits_per_byte;

namespace XMA
{
	class invalid_byte_size_exception {};
	class out_of_bits_exception {};
	class too_many_bits_exception {};
	class integer_too_big_exception {};

	class c_bit_istream
	{
		const boost::uint32_t k_consecutive_bits;
		const boost::uint32_t k_skip_bits;
		std::istream& m_stream;
		boost::uint32_t m_consecutive_bits_left;
		boost::uint32_t m_bits_left;
		boost::uint8_t m_bit_buffer;

	public:
		c_bit_istream(std::istream& stream, boost::uint32_t consecutive_bits = 0, boost::uint32_t skip_bits = 0);

		bool get_bit();
	};

	class c_bit_ostream
	{
		std::ostream& m_stream;
		boost::uint32_t m_bits_stored;
		boost::uint8_t m_bit_buffer;

	public:
		c_bit_ostream(std::ostream& stream);
		~c_bit_ostream();

		void put_bit(bool bit);
		void flush();
	};

	template<const boost::uint8_t k_bit_count>
	class c_bit_stream_integer
	{
		boost::uint32_t m_value;

	public:
		c_bit_stream_integer() : 
			m_value(0)
		{
			if(k_bit_count > std::numeric_limits<boost::uint32_t>::digits)
				throw too_many_bits_exception();
		}

		explicit c_bit_stream_integer(boost::uint32_t value) : 
			m_value(value)
		{
			if(value >= (1<<k_bit_count))
				throw integer_too_big_exception();
		}

		c_bit_stream_integer& operator=(boost::uint32_t value)
		{
			if(value >= (1<<k_bit_count))
				throw integer_too_big_exception();

			m_value = value;
			return *this;
		}

		operator boost::uint32_t() const { return m_value; }

		friend c_bit_istream& operator>>(c_bit_istream& bstream, c_bit_stream_integer& obj) {
			obj.m_value = 0;
			for ( int i = 0; i < k_bit_count; i++) {
				obj.m_value *= 2;
				if ( bstream.get_bit() ) obj.m_value++;
			}
			return bstream;
		}

		friend c_bit_ostream& operator<<(c_bit_ostream& bstream, const c_bit_stream_integer& obj) {
			for ( int i = k_bit_count-1; i >= 0; i--)
				bstream.put_bit((obj.m_value & (1 << i)) != 0);

			return bstream;
		}
	};
};
__CPP_CODE_END__

#endif