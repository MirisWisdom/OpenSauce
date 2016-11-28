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

#include "XMA/XmaUtil.hpp"

namespace XMA
{
	cpp_using(std);

	//////////////////////////////////////////////////////////////////////////
	// parse errors
	class parse_error
	{
	public:
		virtual ~parse_error() {}

		virtual void print(ostream& s) const = NULL;

		friend ostream& operator<<(ostream& s, const parse_error& err);
	};
	class bad_frame_sync_error : public parse_error
	{
		const boost::uint32_t m_value;
	public:
		explicit bad_frame_sync_error(const boost::uint32_t value);

		virtual void print(ostream& s) const;
	};
	class early_packet_end_error : public parse_error
	{
	public:
		virtual void print(ostream& s) const;
	};
	class missing_packet_end_error : public parse_error
	{
	public:
		virtual void print(ostream& s) const;
	};
	class zero_frames_not_skipped_error : public parse_error
	{
	public:
		virtual void print(ostream& s) const;
	};
	class skip_mismatch_error : public parse_error
	{
		const boost::uint32_t m_skip, m_overflow;
	public:
		skip_mismatch_error(boost::uint32_t skip, boost::uint32_t overflow);

		virtual void print(ostream& s) const;
	};
	class skip_nonzero_frames_error : public parse_error
	{
	public:
		virtual void print(ostream& s) const;
	};
	class bad_sequence_error : public parse_error
	{
	public:
		virtual void print(ostream& s) const;
	};
	//////////////////////////////////////////////////////////////////////////


	struct s_xma_packet_header
	{
		c_bit_stream_integer<4> sequence_number;
		c_bit_stream_integer<2> unknown;
		c_bit_stream_integer<15> skip_bits;
		c_bit_stream_integer<11> packet_skip;

		s_xma_packet_header();

		friend c_bit_istream& operator>>(c_bit_istream& s, s_xma_packet_header& ph);
		friend c_bit_ostream& operator<<(c_bit_ostream& s, const s_xma_packet_header& ph);
	};

	struct s_xma2_packet_header				// XMA2PACKET
	{
		c_bit_stream_integer<6> frame_count;//						GetXmaPacketFrameCount
		c_bit_stream_integer<15> skip_bits;	// FrameOffsetInBits	GetXmaPacketFirstFrameOffsetInBits
		c_bit_stream_integer<3> metadata;	//						GetXmaPacketMetadata
		c_bit_stream_integer<8> packet_skip;// PacketSkipCount		GetXmaPacketSkipCount

		s_xma2_packet_header();

		friend c_bit_istream& operator>>(c_bit_istream& s, s_xma2_packet_header& ph);
	};

	struct s_xma_parse_context
	{
		bool stereo;
		bool strict;
		bool verbose;
		bool ignore_packet_skip;
		boost::int32_t version;

		boost::int32_t offset;
		boost::int32_t data_size;
		boost::int32_t block_size;
	};

	class c_xma_interface
	{
	protected:
		s_xma_parse_context& m_parse_ctx;

		struct s_xma_parse_frame_context
		{
			boost::uint32_t frame_count;
			boost::uint32_t* total_bits;
			boost::uint32_t max_bits;
			bool known_frame_count;
			PAD24;
		};
		boost::uint32_t parse_frames(c_bit_istream& frame_stream, s_xma_parse_frame_context& ctx);

	public:
		c_xma_interface(s_xma_parse_context& ctx) : 
			m_parse_ctx(ctx)
		{
		}
	};

	class c_xma_parser : public c_xma_interface
	{
		std::istream& m_in_stream;
		std::ostream& m_out_stream;

	public:
		c_xma_parser(std::istream& in_stream, std::ostream& out_stream, s_xma_parse_context& ctx);

		boost::uint32_t parse_xma_packets();
		boost::uint32_t parse_xma2_block(boost::int32_t offset, boost::int32_t block_size);
	};

	class c_xma_builder : public c_xma_interface
	{
		c_bit_ostream& m_stream;
		boost::uint32_t m_bits_written;
		boost::uint32_t m_sequence_number;

		void packetize(c_bit_istream& frame_stream, boost::uint32_t frame_count, bool last);
		void finish();
	public:
		c_xma_builder(c_bit_ostream& bs, s_xma_parse_context& ctx);
		~c_xma_builder();

		boost::uint32_t build_from_xma(istream& is, boost::int32_t offset);
		boost::uint32_t build_from_xma2_block(istream& is, boost::int32_t offset, boost::int32_t block_size, bool last);
	};

	class c_xma_rebuilder
	{
		bool m_use_filestreams;
		PAD24;
		s_xma_parse_context& m_parse_ctx;

		c_xma_parser* m_parser;
		c_xma_builder* m_rebuilder;

		std::istream* m_in_stream;
		std::ostream* m_out_stream;
		std::ostream* m_rebuild_stream;

		std::string m_error;

		bool use_parser() { return m_parser != cpp_null; }
		bool use_builder() { return m_rebuilder != cpp_null; }

		bool out_stream_valid()
		{
			bool result = m_out_stream != cpp_null;

			if(result && m_use_filestreams)
				result |= dynamic_cast<ofstream*>(m_out_stream)->is_open();

			return result;
		}
		bool rebuild_stream_valid()
		{
			bool result = m_rebuild_stream != cpp_null;

			if(result && m_use_filestreams)
				result |= dynamic_cast<ofstream*>(m_rebuild_stream)->is_open();

			return result;
		}

		void dispose_xma_interfaces()
		{
			if(m_parser != cpp_null)
			{
				delete m_parser;
				m_parser = cpp_null;
			}
			if(m_rebuilder != cpp_null)
			{
				delete m_rebuilder;
				m_rebuilder = cpp_null;
			}
		}
	public:
		c_xma_rebuilder(char* buffer, s_xma_parse_context& ctx);
		c_xma_rebuilder(const char* in_file, s_xma_parse_context& ctx,
			const char* out_file, const char* rebuild_file);
		~c_xma_rebuilder();

		// If successful, allocates a byte buffer that is a copy of the output stream
		// Don't forget to delete [out_buffer]
		bool get_output_data(char*& out_buffer, size_t& out_buffer_size);
		const char* get_error_msg() const;

		boost::uint32_t rebuild();
		bool try_rebuild();
	};
};

__CPP_CODE_END__

#endif