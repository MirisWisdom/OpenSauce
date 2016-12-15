/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
/*
	Based on "xma_parse" code written by HCS (http://hcs64.com/contact.html)
	Used with permission.
*/
#include "Precompile.hpp"
#include "XMA/XmaParse.hpp"

#ifdef _DEBUG

__CPP_CODE_START__

#define LOWLEVEL_NO_X360_XMA_VERBOSE

const boost::uint32_t k_packet_size_in_bytes = 0x800;
const boost::uint32_t k_packet_header_size_in_bytes = 4;
const boost::uint32_t k_frame_header_size_in_bits = 15;
const boost::uint32_t k_frame_sync_size_in_bits = 15;
const boost::uint32_t k_frame_skip_size_in_bits = 10;
const boost::uint32_t k_frame_trailer_size_in_bits = 1;
const boost::uint32_t k_samples_per_frame = 512;

namespace XMA
{
	//////////////////////////////////////////////////////////////////////////
	// parse errors
	ostream& /*parse_error::*/operator<<(ostream& s, const parse_error& err) {
		s << "Parse error: ";
		err.print(s);
		return s;
	}

	bad_frame_sync_error::bad_frame_sync_error(const boost::uint32_t value) : m_value(value) {}
	void bad_frame_sync_error::print(ostream& s) const {
		s << "unexpected \"frame sync\" " << hex << m_value << dec << endl;
	}

	void early_packet_end_error::print(ostream& s) const {
		s << "packet end indicated before end of packet" << endl;
	}

	void missing_packet_end_error::print(ostream& s) const {
		s << "packet end not seen before end of packet" << endl;
	}

	void zero_frames_not_skipped_error::print(ostream& s) const {
		s << "zero frames in this packet, but not set to skip it" << endl;
	}

	skip_mismatch_error::skip_mismatch_error(boost::uint32_t skip, boost::uint32_t overflow) : 
		m_skip(skip),
		m_overflow(overflow)
	{
	}

	void skip_mismatch_error::print(ostream& s) const {
		s << "skip bits (" << m_skip << ") did not match previous packet overflow (" << m_overflow << ")" << endl;
	}

	void skip_nonzero_frames_error::print(ostream& s) const {
		s << "skipping entire packet with > 0 frames" << endl;
	}

	void bad_sequence_error::print(ostream& s) const {
		s << "packet out of sequence" << endl;
	}
	//////////////////////////////////////////////////////////////////////////


	s_xma_packet_header::s_xma_packet_header() : 
		sequence_number(0), unknown(0), skip_bits(0), packet_skip(0)
	{
	}

	c_bit_istream& /*s_xma_packet_header::*/operator>>(c_bit_istream& s, s_xma_packet_header& ph) {
		return s >> ph.sequence_number >> ph.unknown >> ph.skip_bits >> ph.packet_skip;
	}
	c_bit_ostream& /*s_xma_packet_header::*/operator<<(c_bit_ostream& s, const s_xma_packet_header& ph) {
		return s << ph.sequence_number << ph.unknown << ph.skip_bits << ph.packet_skip;
	}

	s_xma2_packet_header::s_xma2_packet_header() : 
		frame_count(0), skip_bits(0), metadata(0), packet_skip(0)
	{
	}

	c_bit_istream& /*s_xma2_packet_header::*/operator>>(c_bit_istream& s, s_xma2_packet_header& ph) {
		return s >> ph.frame_count >> ph.skip_bits >> ph.metadata >> ph.packet_skip;
	}
	//////////////////////////////////////////////////////////////////////////


	//////////////////////////////////////////////////////////////////////////
	// c_xma_interface
	boost::uint32_t c_xma_interface::parse_frames(c_bit_istream& frame_stream, s_xma_parse_frame_context& ctx)
	{
		bool packet_end_seen = false;
		boost::uint32_t sample_count = 0;
		boost::uint32_t total_bits = 0;

		if (ctx.known_frame_count && ctx.frame_count == 0)
			throw zero_frames_not_skipped_error();

		for (boost::uint32_t frame_number = 0;
			!ctx.known_frame_count || frame_number < ctx.frame_count;
			frame_number++)
		{
			c_bit_stream_integer<k_frame_header_size_in_bits> frame_bits;
			frame_stream >> frame_bits;
#ifndef LOWLEVEL_NO_X360_XMA_VERBOSE
			//cout << "   Frame #" << frame_number << ", " << total_bits << " bits read" << endl;
#endif
			total_bits += frame_bits;

			boost::uint32_t bits_left = frame_bits - k_frame_header_size_in_bits;

#ifndef LOWLEVEL_NO_X360_XMA_VERBOSE
			if (m_parse_ctx.verbose)
			{
				cout << "   Frame #" << frame_number << endl;
				cout << "   Size " << frame_bits << endl;
			}
#endif

			// sync?
			{
				c_bit_stream_integer<k_frame_sync_size_in_bits> sync;
				frame_stream >> sync;
				if (sync!= 0x7F00) throw bad_frame_sync_error(sync);
				bits_left -= k_frame_sync_size_in_bits;
			}

			if (m_parse_ctx.stereo) {
				frame_stream.get_bit();
				bits_left--;
			}

			// skip
			if (frame_stream.get_bit())
			{
				c_bit_stream_integer<k_frame_skip_size_in_bits> skip_start, skip_end;
				// skip at start
				if (frame_stream.get_bit())
				{
					frame_stream >> skip_start;
#ifndef LOWLEVEL_NO_X360_XMA_VERBOSE
					if (m_parse_ctx.verbose)
						cout << "Skip " << skip_start << " samples at start" << endl;
#endif
					bits_left -= k_frame_skip_size_in_bits;
					sample_count -= skip_start;
				}
				bits_left--;
				// skip at end
				if (frame_stream.get_bit()) {
					frame_stream >> skip_end;
#ifndef LOWLEVEL_NO_X360_XMA_VERBOSE
					if (m_parse_ctx.verbose)
						cout << "Skip " << skip_end << " samples at end" << endl;
#endif
					bits_left -= k_frame_skip_size_in_bits;
					sample_count -= skip_end;
				}
				bits_left--;
			}
			bits_left--;

#ifndef LOWLEVEL_NO_X360_XMA_VERBOSE
			if (m_parse_ctx.verbose)
				cout << hex;
#endif
			for (; bits_left >= 4 + k_frame_trailer_size_in_bits; bits_left -= 4) {
				c_bit_stream_integer<4> nybble;
				frame_stream >> nybble;

#ifndef LOWLEVEL_NO_X360_XMA_VERBOSE
				if (m_parse_ctx.verbose)
					cout << nybble;
#endif
			}

#ifndef LOWLEVEL_NO_X360_XMA_VERBOSE
			if (m_parse_ctx.verbose)
				cout << " ";
#endif
			for (; bits_left > k_frame_trailer_size_in_bits; bits_left--) {
				bool bit = frame_stream.get_bit();

#ifndef LOWLEVEL_NO_X360_XMA_VERBOSE
				if (m_parse_ctx.verbose)
					cout << (bit ? '1' : '0');
#endif
			}
#ifndef LOWLEVEL_NO_X360_XMA_VERBOSE
			if (m_parse_ctx.verbose)
				cout << dec << endl;
#endif

			// trailer
			{
				if (!frame_stream.get_bit())
				{
					if (m_parse_ctx.strict && ctx.known_frame_count &&
						frame_number != ctx.frame_count-1) throw early_packet_end_error();
					packet_end_seen = true;
				}

				sample_count += k_samples_per_frame;

				bits_left -= k_frame_trailer_size_in_bits;

				if (!ctx.known_frame_count && packet_end_seen) break;

				// FIX: detect end with bit count
				if (!m_parse_ctx.strict && !ctx.known_frame_count && total_bits >= ctx.max_bits) {
#ifndef LOWLEVEL_NO_X360_XMA_VERBOSE
					if (m_parse_ctx.verbose)
						cout << "abandon frame due to bit count (total=" << total_bits << " max=" << ctx.max_bits << ")" << endl;
#endif

					break;
				}
			}
		}

		// FIX: don't fail if packet end missing
		if (m_parse_ctx.strict && !packet_end_seen) throw missing_packet_end_error();

#ifndef LOWLEVEL_NO_X360_XMA_VERBOSE
		if (m_parse_ctx.verbose)
			cout << endl;
#endif

		if (ctx.total_bits)
			*ctx.total_bits = total_bits;

		return sample_count;
	}


	//////////////////////////////////////////////////////////////////////////
	// c_xma_parser
	c_xma_parser::c_xma_parser(std::istream& in_stream, std::ostream& out_stream, s_xma_parse_context& ctx) : 
	c_xma_interface(ctx),
		m_in_stream(in_stream),
		m_out_stream(out_stream)
	{
	}

	boost::uint32_t c_xma_parser::parse_xma_packets()
	{
		boost::int32_t offset = m_parse_ctx.offset;
		boost::int32_t last_offset = offset+m_parse_ctx.data_size;
		boost::uint32_t sample_count = 0;
		boost::uint32_t last_packet_overflow_bits = 0;
		boost::uint32_t sequence_number = 0;

		while(offset < last_offset) {
			s_xma_packet_header ph;

			{
				c_bit_istream packet_header_stream(m_in_stream);
				m_in_stream.seekg(offset);
				packet_header_stream >> ph;
			}

#ifndef LOWLEVEL_NO_X360_XMA_VERBOSE
			if(m_parse_ctx.verbose) {
				cout << "Sequence #" << ph.sequence_number << " (offset " << hex << offset << dec << ")" << endl;
				cout << "Unknown         " << ph.unknown << endl;
				cout << "Skip Bits       " << ph.skip_bits << endl;
				cout << "Packet Skip     " << ph.packet_skip << (m_parse_ctx.ignore_packet_skip?" (ignore)":"") << endl;
			}
#endif

			if(m_parse_ctx.ignore_packet_skip)
				ph.packet_skip = 0;

			if(m_parse_ctx.strict && ph.sequence_number != sequence_number)
				throw bad_sequence_error();

			c_bit_istream frame_stream(m_in_stream,
				(k_packet_size_in_bytes - k_packet_header_size_in_bytes) * k_bits_per_byte, // consecutive
				(k_packet_header_size_in_bytes + ph.packet_skip * k_packet_size_in_bytes) * k_bits_per_byte // skip
			);

			if (16384 == ph.skip_bits)
			{
				if (ph.unknown != 0)
#ifndef LOWLEVEL_NO_X360_XMA_VERBOSE
					cout << "Unknown = " << ph.unknown << ", expected 0" << endl;
#else
					__noop;
#endif

				last_packet_overflow_bits = 0;
			}
			else
			{
				boost::uint32_t total_bits;

				if (ph.unknown != 2)
#ifndef LOWLEVEL_NO_X360_XMA_VERBOSE
					cout << "Unknown = " << ph.unknown << ", expected 2" << endl;
#else
					__noop;
#endif

				// skip initial bits (overflow from a previous packet)
				for (boost::uint32_t i = 0; i < ph.skip_bits; i++) frame_stream.get_bit();

				if (ph.skip_bits != last_packet_overflow_bits)
					throw skip_mismatch_error(ph.skip_bits,last_packet_overflow_bits);

				s_xma_parse_frame_context frame_ctx = {
					0, &total_bits, (k_packet_size_in_bytes - k_packet_header_size_in_bytes)*k_bits_per_byte - ph.skip_bits, 
					false
				};
				sample_count += parse_frames(frame_stream, frame_ctx);

				int overflow_temp = last_packet_overflow_bits = (ph.skip_bits + total_bits) - 
					((k_packet_size_in_bytes - k_packet_header_size_in_bytes) * k_bits_per_byte);

				// KM00: isn't the the 'if' case redundant?
				if (overflow_temp > 0)
					last_packet_overflow_bits = overflow_temp;
				else
					last_packet_overflow_bits = 0;
			}

			// We've successfully examined this packet, dump it out if we have an output stream
			if (m_out_stream) {
				char buf[k_packet_size_in_bytes];
				m_in_stream.seekg(offset);

				m_in_stream.read(buf, k_packet_size_in_bytes);
				// FIX: fix sequence number
				if (sequence_number != ph.sequence_number && !m_parse_ctx.strict) {
					buf[0] = static_cast<char>( (buf[0] & 0xf) | (sequence_number << 4) );

#ifndef LOWLEVEL_NO_X360_XMA_VERBOSE
					if (m_parse_ctx.verbose)
						cout << "fixing sequence number (was " << ph.sequence_number << ", output " << sequence_number << ")" << endl;
#endif
				}
				buf[3] = 0; // zero packet skip, since we're packing consecutively
				m_out_stream.write(buf, k_packet_size_in_bytes);
			}

			sequence_number = (sequence_number + 1) % 16;

			offset += (ph.packet_skip + 1) * k_packet_size_in_bytes;
		}

		return sample_count;
	}

	boost::uint32_t c_xma_parser::parse_xma2_block(boost::int32_t offset, boost::int32_t block_size)
	{
		boost::int32_t last_offset = offset+block_size;
		boost::uint32_t sample_count = 0;
		boost::uint32_t last_packet_overflow_bits = 0;

		for(int packet_number = 0; offset < last_offset; packet_number++)
		{
			s_xma2_packet_header ph;

			{
				c_bit_istream packet_header_stream(m_in_stream);
				m_in_stream.seekg(offset);
				packet_header_stream >> ph;
			}

#ifndef LOWLEVEL_NO_X360_XMA_VERBOSE
			if (m_parse_ctx.verbose) {
				cout << "Packet #" << packet_number << " (offset " << hex << offset << dec << ")" << endl;
				cout << "Frame Count     " << ph.frame_count << endl;
				cout << "Skip Bits       " << ph.skip_bits << endl;
				cout << "Metadata        " << ph.metadata << endl;
				cout << "Packet Skip     " << ph.packet_skip << (m_parse_ctx.ignore_packet_skip?" (ignored)":"") << endl;
			}
#endif

			if (m_parse_ctx.ignore_packet_skip)
				ph.packet_skip = 0;

			c_bit_istream frame_stream(m_in_stream,
				(k_packet_size_in_bytes - k_packet_header_size_in_bytes) * k_bits_per_byte, // consecutive
				(k_packet_header_size_in_bytes + ph.packet_skip * k_packet_size_in_bytes) * k_bits_per_byte // skip
			);

			// At the end of a block no frame may start in a packet, signaled
			// with invalidly large skip_bits so we skip everything
			if (ph.skip_bits == 0x7FFF) {
				if (ph.frame_count != 0)
					throw skip_nonzero_frames_error();

				last_packet_overflow_bits = 0;
			} else {
				boost::uint32_t total_bits;

				// skip initial bits (overflow from a previous packet)
				for (boost::uint32_t i = 0; i < ph.skip_bits; i++) frame_stream.get_bit();

				if (ph.skip_bits != last_packet_overflow_bits)
					throw skip_mismatch_error(ph.skip_bits,last_packet_overflow_bits);

				s_xma_parse_frame_context frame_ctx = {
					ph.frame_count, &total_bits, (k_packet_size_in_bytes - k_packet_header_size_in_bytes)*k_bits_per_byte - ph.skip_bits, 
					true
				};
				sample_count += parse_frames(frame_stream, frame_ctx);

				int overflow_temp = last_packet_overflow_bits = (ph.skip_bits + total_bits) - 
					((k_packet_size_in_bytes - k_packet_header_size_in_bytes) * k_bits_per_byte);

				// KM00: isn't the the 'if' case redundant?
				if (overflow_temp > 0)
					last_packet_overflow_bits = overflow_temp;
				else
					last_packet_overflow_bits = 0;
			}

			// We've successfully examined this packet, dump it out if we have an output stream
			if (m_out_stream) {
				char buf[k_packet_size_in_bytes];
				m_in_stream.seekg(offset);

				m_in_stream.read(buf, k_packet_size_in_bytes);
				buf[3] = 0; // zero packet skip, since we're packing consecutively
				m_out_stream.write(buf, k_packet_size_in_bytes);
			}

			// advance to next packet
			offset += (ph.packet_skip + 1) * k_packet_size_in_bytes;
		}

		return sample_count;
	}


	//////////////////////////////////////////////////////////////////////////
	// c_xma_builder
	void c_xma_builder::packetize(c_bit_istream& frame_stream, boost::uint32_t frame_count, bool last)
	{
		bool packet_end_seen = false;

		if (frame_count == 0)
			throw zero_frames_not_skipped_error();

		for (boost::uint32_t frame_number = 0;
			frame_number < frame_count;
			frame_number++)
		{
			c_bit_stream_integer<k_frame_header_size_in_bits> frame_bits;
			frame_stream >> frame_bits;

#ifndef LOWLEVEL_NO_X360_XMA_VERBOSE
			//cout << "Frame #" << frame_number << ", " << bits_written << " bits written" << endl;
#endif

			if (m_bits_written + frame_bits >= k_packet_size_in_bytes * k_bits_per_byte) {
				boost::uint32_t bits_this_packet = (k_packet_size_in_bytes * k_bits_per_byte) - m_bits_written;
				boost::uint32_t overflow_bits = frame_bits - bits_this_packet;

				s_xma_packet_header ph;
				ph.sequence_number = m_sequence_number;
				m_sequence_number = (m_sequence_number + 1) % 16;
				ph.unknown = 2;
				ph.skip_bits = overflow_bits;
				ph.packet_skip = 0;

#ifndef LOWLEVEL_NO_X360_XMA_VERBOSE
				//cout << "overflow = " << overflow_bits << endl;
#endif

				// bits of frame header before packet end
				boost::uint32_t frame_header_size_bits_left = k_frame_header_size_in_bits;
				for (; bits_this_packet > 0 && frame_header_size_bits_left > 0; bits_this_packet--, frame_header_size_bits_left--) {
					m_stream.put_bit( 0 != (frame_bits & (1 << (frame_header_size_bits_left - 1))) );
				}

				if (overflow_bits == 0) {
					// frame fits packet exactly

					// payload bits before packet end
					for (boost::uint32_t i = 0; i < bits_this_packet-1; i++)
						m_stream.put_bit(frame_stream.get_bit());

					// trailer bit, no more frames in packet
					m_stream.put_bit(false);
				} else {
					// payload bits 
					for (boost::uint32_t i = 0; i < bits_this_packet; i++)
						m_stream.put_bit(frame_stream.get_bit());
				}

				m_stream << ph;

				m_bits_written = k_packet_header_size_in_bytes * k_bits_per_byte;

				if (overflow_bits != 0) {
					// bits of frame header in new packet
					for (; frame_header_size_bits_left > 0; frame_header_size_bits_left--, m_bits_written++, overflow_bits--) {
						m_stream.put_bit( 0 != (frame_bits & (1 << (frame_header_size_bits_left - 1))) );
					}

					// payload bits in new packet
					for (boost::uint32_t i = 0; i < overflow_bits - 1; i++)
						m_stream.put_bit(frame_stream.get_bit());

					m_bits_written += overflow_bits - 1;

					// trailer bit, no more frames in packet
					m_stream.put_bit(false);
					m_bits_written++;
				}
			} else {
				m_stream << frame_bits;
				for (boost::uint32_t i = 0; i < frame_bits - k_frame_header_size_in_bits - 1; i++) {
					m_stream.put_bit(frame_stream.get_bit());
				}

				// trailer bit
				if (last && frame_number == frame_count-1)
					m_stream.put_bit(false); // no more frames
				else
					m_stream.put_bit(true); // more frames in packet

				m_bits_written += frame_bits;
			}

			// trailer
			if (!frame_stream.get_bit())
			{
				if (m_parse_ctx.strict && frame_number != frame_count-1)
					throw early_packet_end_error();

				packet_end_seen = true;
			}
		}

		if (m_parse_ctx.strict && !packet_end_seen) throw missing_packet_end_error();
	}
	void c_xma_builder::finish()
	{
		// pad out with cool ones
		for(; m_bits_written < k_packet_size_in_bytes*k_bits_per_byte; m_bits_written++)
			m_stream.put_bit(true);
	}

	c_xma_builder::c_xma_builder(c_bit_ostream& bs, s_xma_parse_context& ctx) : 
	c_xma_interface(ctx),
		m_stream(bs),
		m_bits_written(k_packet_header_size_in_bytes*k_bits_per_byte),
		m_sequence_number(1)
	{
		// First packet
		s_xma_packet_header ph;
		ph.sequence_number = 0;
		ph.unknown = 2;
		ph.skip_bits = 0;
		ph.packet_skip = 0;

		m_stream << ph;
	}
	c_xma_builder::~c_xma_builder()
	{
		finish();
	}

	boost::uint32_t c_xma_builder::build_from_xma(istream& is, boost::int32_t offset)
	{
		boost::int32_t last_offset = offset + m_parse_ctx.data_size;
		boost::uint32_t sample_count = 0;
		boost::uint32_t last_packet_overflow_bits = 0;
		boost::uint32_t sequence_number = 0;

		while (offset < last_offset) {
			s_xma_packet_header ph;

			boost::uint32_t frames_this_packet = 0;

			{
				c_bit_istream packet_header_stream(is);

				is.seekg(offset);

				packet_header_stream >> ph;
			}

#ifndef LOWLEVEL_NO_X360_XMA_VERBOSE
			if (m_parse_ctx.verbose) {
				cout << "Sequence #" << ph.sequence_number << " (offset " << hex << offset << dec << ")" << endl;
				cout << "Unknown         " << ph.unknown << endl;
				cout << "Skip Bits       " << ph.skip_bits << endl;
				cout << "Packet Skip     " << ph.packet_skip << (m_parse_ctx.ignore_packet_skip?" (ignored)":"") << endl;
			}
#endif

			if (m_parse_ctx.ignore_packet_skip)
				ph.packet_skip = 0;

			if (m_parse_ctx.strict && ph.sequence_number != sequence_number)
				throw bad_sequence_error();


			c_bit_istream frame_stream(is,
				(k_packet_size_in_bytes - k_packet_header_size_in_bytes) * k_bits_per_byte, // consecutive
				(k_packet_header_size_in_bytes + ph.packet_skip * k_packet_size_in_bytes) * k_bits_per_byte // skip
			);

			if (16384 == ph.skip_bits)
			{
				last_packet_overflow_bits = 0;

				if (ph.unknown != 0)
					cout << "Unknown = " << ph.unknown << ", expected 0" << endl;
			}
			else
			{
				boost::uint32_t total_bits;

				if (ph.unknown != 2)
					cout << "Unknown = " << ph.unknown << ", expected 2" << endl;

				// skip initial bits (overflow from a previous packet)
				for (boost::uint32_t i = 0; i < ph.skip_bits; i++) frame_stream.get_bit();

				if (ph.skip_bits != last_packet_overflow_bits)
					throw skip_mismatch_error(ph.skip_bits,last_packet_overflow_bits);

				s_xma_parse_frame_context frame_ctx = {
					0, &total_bits, (k_packet_size_in_bytes - k_packet_header_size_in_bytes)*k_bits_per_byte - ph.skip_bits, 
					false
				};
				boost::uint32_t samples_this_packet = parse_frames(frame_stream, frame_ctx);
				sample_count += samples_this_packet;
				frames_this_packet = samples_this_packet / k_samples_per_frame;

				int overflow_temp = last_packet_overflow_bits = (ph.skip_bits + total_bits) - 
					((k_packet_size_in_bytes - k_packet_header_size_in_bytes) * 8);

				// KM00: isn't the the 'if' case redundant?
				if (overflow_temp > 0)
					last_packet_overflow_bits = overflow_temp;
				else
					last_packet_overflow_bits = 0;
			}

			// We've successfully examined this packet, dump it out
			{
				is.seekg(offset + k_packet_header_size_in_bytes);

				c_bit_istream dump_frame_stream(is,
					(k_packet_size_in_bytes - k_packet_header_size_in_bytes) * k_bits_per_byte, // consecutive
					(k_packet_header_size_in_bytes + ph.packet_skip * k_packet_size_in_bytes) * k_bits_per_byte // skip
				);

				// Do packet if not skipping
				if (ph.skip_bits != 16384) {
					// skip initial bits (overflow from a previous packet)
					for (boost::uint32_t i = 0; i < ph.skip_bits; i++) dump_frame_stream.get_bit();

					packetize(dump_frame_stream, frames_this_packet, 
						(CAST(boost::uint32_t,offset) + (ph.packet_skip + 1) * k_packet_size_in_bytes >= CAST(boost::uint32_t,last_offset))
					);
				}
			}

			sequence_number = (sequence_number + 1) % 16;

			offset += (ph.packet_skip + 1) * k_packet_size_in_bytes;
		}

		return sample_count;
	}
	boost::uint32_t c_xma_builder::build_from_xma2_block(istream& is, boost::int32_t offset, boost::int32_t block_size, bool last)
	{
		boost::int32_t last_offset = offset + block_size;
		boost::uint32_t sample_count = 0;
		boost::uint32_t last_packet_overflow_bits = 0;

		for (int packet_number = 0; offset < last_offset; packet_number++) {
			s_xma2_packet_header ph;

			{
				c_bit_istream packet_header_stream(is);

				is.seekg(offset);

				packet_header_stream >> ph;
			}

#ifndef LOWLEVEL_NO_X360_XMA_VERBOSE
			if (m_parse_ctx.verbose) {
				cout << "Packet #" << packet_number << " (offset " << hex << offset << dec << ")" << endl;
				cout << "Frame Count     " << ph.frame_count << endl;
				cout << "Skip Bits       " << ph.skip_bits << endl;
				cout << "Metadata        " << ph.metadata << endl;
				cout << "Packet Skip     " << ph.packet_skip << (m_parse_ctx.ignore_packet_skip?" (ignored)":"") << endl;
			}
#endif

			if (m_parse_ctx.ignore_packet_skip)
				ph.packet_skip = 0;

			c_bit_istream frame_stream(is,
				(k_packet_size_in_bytes - k_packet_header_size_in_bytes) * k_bits_per_byte, // consecutive
				(k_packet_header_size_in_bytes + ph.packet_skip * k_packet_size_in_bytes) * k_bits_per_byte // skip
			);

			// At the end of a block no frame may start in a packet, signaled
			// with invalidly large skip_bits so we skip everything
			if (ph.skip_bits == 0x7FFF) {
				if (ph.frame_count != 0)
					throw skip_nonzero_frames_error();

				last_packet_overflow_bits = 0;
			} else {
				boost::uint32_t total_bits;

				// skip initial bits (overflow from a previous packet)
				for (boost::uint32_t i = 0; i < ph.skip_bits; i++) frame_stream.get_bit();

				if (ph.skip_bits != last_packet_overflow_bits)
					throw skip_mismatch_error(ph.skip_bits,last_packet_overflow_bits);

				s_xma_parse_frame_context frame_ctx = {
					ph.frame_count, &total_bits, (k_packet_size_in_bytes - k_packet_header_size_in_bytes)*k_bits_per_byte - ph.skip_bits, 
					true
				};
				sample_count += parse_frames(frame_stream, frame_ctx);

				int overflow_temp = last_packet_overflow_bits = (ph.skip_bits + total_bits) - 
					((k_packet_size_in_bytes - k_packet_header_size_in_bytes) * 8);

				// KM00: isn't the the 'if' case redundant?
				if (overflow_temp > 0)
					last_packet_overflow_bits = overflow_temp;
				else
					last_packet_overflow_bits = 0;
			}

			// We've successfully examined this packet, dump it out
			{
				is.seekg(offset + k_packet_header_size_in_bytes);

				c_bit_istream dump_frame_stream(is,
					(k_packet_size_in_bytes - k_packet_header_size_in_bytes) * k_bits_per_byte, // consecutive
					(k_packet_header_size_in_bytes + ph.packet_skip * k_packet_size_in_bytes) * k_bits_per_byte // skip
				);

				// Do packet if not skipping
				if (ph.skip_bits != 0x7FFF) {
					// skip initial bits (overflow from a previous packet)
					for (boost::uint32_t i = 0; i < ph.skip_bits; i++) dump_frame_stream.get_bit();

					packetize(dump_frame_stream, ph.frame_count,
						last && (static_cast<boost::uint32_t>(offset) + (ph.packet_skip + 1) * k_packet_size_in_bytes >= static_cast<boost::uint32_t>(last_offset)) );
				}
			}

			// advance to next packet
			offset += (ph.packet_skip + 1) * k_packet_size_in_bytes;
		}

		return sample_count;
	}


	//////////////////////////////////////////////////////////////////////////
	// c_xma_rebuilder
	c_xma_rebuilder::c_xma_rebuilder(char* buffer, s_xma_parse_context& ctx) : 
		m_use_filestreams(false),
		m_parse_ctx(ctx),
		m_parser(cpp_null),
		m_rebuilder(cpp_null),
		m_in_stream(cpp_null),
		m_out_stream(cpp_null),
		m_rebuild_stream(cpp_null),
		m_error("")
	{
		m_in_stream = cpp_new istringstream( std::string(buffer, m_parse_ctx.data_size), ios::in | ios::binary);
		m_out_stream = cpp_new stringstream(ios::binary | ios::in | ios::out);
	}

	c_xma_rebuilder::c_xma_rebuilder(const char* in_file, s_xma_parse_context& ctx,
		const char* out_file, const char* rebuild_file) : 
		m_use_filestreams(true),
		m_parse_ctx(ctx),
		m_parser(cpp_null),
		m_rebuilder(cpp_null),
		m_in_stream(cpp_null),
		m_out_stream(cpp_null),
		m_rebuild_stream(cpp_null),
		m_error("")
	{
		m_in_stream = cpp_new ifstream(in_file, ios::in | ios::binary);

		if( !(*m_in_stream) )
			throw "Error opening file!";

		if(ctx.data_size == NONE)
		{
			m_in_stream->seekg(0, ios::end);
			ctx.data_size = (boost::int32_t)m_in_stream->tellg() - ctx.offset;
		}

		if(out_file != cpp_null)
		{
			m_out_stream = cpp_new ofstream(out_file, ios::out | ios::binary);

			if( !(*m_out_stream) )
				throw "Error opening output file!";
		}

		if(rebuild_file != cpp_null)
		{
			m_rebuild_stream = cpp_new ofstream(rebuild_file, ios::out | ios::binary);

			if( !(*m_rebuild_stream) )
				throw "Error opening rebuild output file!";
		}
	}

	c_xma_rebuilder::~c_xma_rebuilder()
	{
		dispose_xma_interfaces();

		if(m_in_stream != cpp_null)
		{
			delete m_in_stream;
			m_in_stream = cpp_null;
		}
		if(m_out_stream != cpp_null)
		{
			delete m_out_stream;
			m_out_stream = cpp_null;
		}
		if(m_rebuild_stream != cpp_null)
		{
			delete m_rebuild_stream;
			m_rebuild_stream = cpp_null;
		}
	}

	bool c_xma_rebuilder::get_output_data(char*& out_buffer, size_t& out_buffer_size)
	{
		out_buffer = cpp_null;
		out_buffer_size = 0;

		if(out_stream_valid())
		{
			auto& s = *dynamic_cast<stringstream*>(m_out_stream);
			stringstream::pos_type pos = s.tellp();

			out_buffer_size = CAST(stringstream::pos_type,pos);
			out_buffer = cpp_new char[out_buffer_size];
			s.seekg(0, ios::beg);
			s.read(out_buffer, out_buffer_size);

			s.seekg(pos);

			return true;
		}

		return false;
	}
	const char* c_xma_rebuilder::get_error_msg() const { return m_error.c_str(); }

	boost::uint32_t c_xma_rebuilder::rebuild()
	{
		boost::uint32_t total_sample_count = 0;

		if(rebuild_stream_valid())
		{
			c_bit_ostream out_bitstream(*m_rebuild_stream);
			m_rebuilder = cpp_new c_xma_builder(out_bitstream, m_parse_ctx);
		}
		else
			m_parser = cpp_new c_xma_parser(*m_in_stream, *m_out_stream, m_parse_ctx);

		if(m_parse_ctx.version == 1)
		{
			if(!use_builder() && out_stream_valid())
				total_sample_count = m_parser->parse_xma_packets();

			if(use_builder())
				total_sample_count = m_rebuilder->build_from_xma(*m_in_stream, m_parse_ctx.offset);
		}
		else for(boost::int32_t block_offset = m_parse_ctx.offset; 
				 block_offset < (m_parse_ctx.offset+m_parse_ctx.data_size);
				 block_offset += m_parse_ctx.block_size)
		{
			boost::int32_t usable_block_size = m_parse_ctx.block_size;

			if(block_offset+usable_block_size > m_parse_ctx.offset+m_parse_ctx.data_size)
				usable_block_size = m_parse_ctx.offset+m_parse_ctx.data_size - block_offset;

			boost::uint32_t sample_count = 0;

			if(!use_builder() && out_stream_valid())
				sample_count = m_parser->parse_xma2_block(block_offset, usable_block_size);

			if(use_builder())
				sample_count = m_rebuilder->build_from_xma2_block(*m_in_stream, block_offset, usable_block_size,
				(block_offset+m_parse_ctx.block_size >= m_parse_ctx.offset+m_parse_ctx.data_size) );

			total_sample_count += sample_count;
#ifndef LOWLEVEL_NO_X360_XMA_VERBOSE
			if(m_parse_ctx.verbose)
				cout << endl << sample_count << " samples (block) (" << total_sample_count << " total)" << endl << endl;
#endif
		}

		return total_sample_count;
	}

	bool c_xma_rebuilder::try_rebuild()
	{
		bool result = true;

		try { rebuild(); }
		catch(const out_of_bits_exception& /*oob*/)
		{
			result = false;
			cerr << (m_error = "Error reading bitstream") << endl;
		}
		catch(const parse_error& pe)
		{
			result = false;
			m_error = typeid(pe).name();
			cerr << pe << endl;
		}
		catch(const char* str)
		{
			result = false;
			cerr << (m_error = str) << endl;
		}

		return result;
	}
};

__CPP_CODE_END__

#endif