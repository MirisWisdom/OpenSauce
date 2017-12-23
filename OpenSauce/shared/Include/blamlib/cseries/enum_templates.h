#pragma once

template <
	typename t_flag,
	typename t_flag_base = typename std::underlying_type<t_flag>::type>
struct is_flags : std::enable_if<(std::is_enum<t_flag>::value &&
		(
			std::is_same<Yelo::byte_flags, t_flag_base>::value ||
			std::is_same<Yelo::word_flags, t_flag_base>::value ||
			std::is_same<Yelo::long_flags, t_flag_base>::value ||
			std::is_same<Yelo::longlong_flags, t_flag_base>::value
		)), t_flag> {};

template <
	typename t_flag,
	t_flag... t_bits>
struct compute_flags;

template <
	typename t_flag>
struct compute_flags<t_flag>
{
	enum
	{
		k_flags = 0
	};
};

template <
	typename t_flag,
	t_flag t_bit>
struct compute_flags<t_flag, t_bit>
{
	enum
	{
		k_flags = 1U << t_bit
	};
};

template <
	typename t_flag,
	t_flag t_bit,
	t_flag... t_bits>
struct compute_flags<t_flag, t_bit, t_bits...>
{
	enum
	{
		k_first_flag = compute_flags<t_flag, t_bit>::k_flags,
		k_other_flags = compute_flags<t_flag, t_bits...>::k_flags,
		k_flags = k_first_flag | k_other_flags
	};
};

template <
	typename t_flag,
	t_flag t_first_bit,
	t_flag t_last_bit>
struct compute_flags_range_mask
{
	enum
	{
		k_first_flag = static_cast<t_flag>(1) << t_first_bit,
		k_last_flag = static_cast<t_flag>(1) << t_last_bit,
		k_range_bits = (k_last_flag + 1) - k_first_flag,
		k_range_mask = ((static_cast<t_flag>(1) << k_range_bits) - 1) << t_first_bit
	};
};

template <
	typename t_flag,
	typename t_flag_base = typename std::underlying_type<t_flag>::type>
struct flags_type
{
	typedef flags_type<t_flag, t_flag_base> this_type_t;

	static_assert(
		std::is_same<t_flag, typename is_flags<t_flag, t_flag_base>::type>::value,
		"Enum is not a flags type");

	t_flag_base value;

	explicit operator t_flag_base() const
	{
		return this->value;
	}

	this_type_t operator ~() const
	{
		this_type_t result { ~this->value };
		return result;
	}

	template <t_flag t_bit>
	bool test() const
	{
		return (this->value & (static_cast<t_flag_base>(1) << t_bit)) > 0;
	}

	bool test(
		const t_flag bit) const
	{
		return (this->value & (static_cast<t_flag_base>(1) << bit)) > 0;
	}

	template <
		t_flag... t_bits>
	bool test_any() const
	{
		return (this->value & this_type_t::create<t_bits...>().value) > 0;
	}

	template <
		t_flag... t_bits>
	bool test_all() const
	{
		const auto test_flags = this_type_t::create<t_bits...>().value;
		return (this->value & test_flags) == test_flags;
	}

	template <
		t_flag... t_bits>
	bool equal_to() const
	{
		return this->value == this_type_t::create<t_bits...>().value;
	}

	template <
		t_flag... t_bits>
	bool not_equal_to() const
	{
		return this->value != this_type_t::create<t_bits...>().value;
	}

	template <
		t_flag t_bit,
		bool t_bit_value = true>
	this_type_t& set()
	{
		if (t_bit_value)
		{
			this->value |= static_cast<t_flag_base>(1) << t_bit;
		}
		else
		{
			this->value &= ~(static_cast<t_flag_base>(1) << t_bit);
		}

		return *this;
	}

	template <
		bool t_bit_value = true,
		t_flag... t_bits>
	this_type_t& set_bits()
	{
		const this_type_t flags_to_change = this_type_t::create<t_bits...>();

		if (t_bit_value)
		{
			this->value |= flags_to_change.value;
		}
		else
		{
			this->value &= flags_to_change.value;
		}

		return *this;
	}

	this_type_t& set(
		const t_flag bit,
		const bool bit_value)
	{
		if (bit_value)
		{
			this->value |= static_cast<t_flag_base>(1) << bit;
		}
		else
		{
			this->value &= ~(static_cast<t_flag_base>(1) << bit);
		}

		return *this;
	}

	template <
		t_flag t_bit>
	this_type_t& toggle()
	{
		this->value ^= static_cast<t_flag_base>(1) << t_bit;
		return *this;
	}

	template <
		t_flag... t_bits>
	this_type_t& toggle_bits()
	{
		const auto flags_to_toggle = this_type_t::create<t_bits...>();

		this->value ^= flags_to_toggle.value;
		return *this;
	}

	this_type_t& toggle(
		const t_flag bit)
	{
		this->value ^= static_cast<t_flag_base>(1) << bit;
		return *this;
	}

	template <
		t_flag t_bit>
	bool toggle_and_test_bit()
	{
		this->toggle<t_bit>();
		return this->test<t_bit>();
	}

	bool toggle_and_test_bit(
		const t_flag bit)
	{
		this->toggle(bit);
		return this->test(bit);
	}

	this_type_t& operator &=(
		const this_type_t& rhs)
	{
		this->value &= rhs.value;
		return *this;
	}

	this_type_t& operator |=(
		const this_type_t& rhs)
	{
		this->value |= rhs.value;
		return *this;
	}

	bool operator ==(
		const this_type_t& rhs) const
	{
		return this->value == rhs.value;
	}

	bool operator !=(
		const this_type_t& rhs) const
	{
		return this->value != rhs.value;
	}

	template <
		t_flag_base t_member_count = t_flag::k_count>
	bool is_valid() const
	{
		const t_flag_base k_invalid_flags_mask = ~((static_cast<t_flag_base>(1) << t_member_count) - 1);
		return (this->value & k_invalid_flags_mask) == 0;
	}

	bool is_zero() const
	{
		return this->value == 0;
	}

	bool is_not_zero() const
	{
		return !is_zero();
	}

	void clear()
	{
		this->value = 0;
	}

	/*constexpr*/
	static this_type_t zero()
	{
		this_type_t flags { 0 };
		return flags;
	}

	bool is_none() const
	{
		return this->value == ~static_cast<t_flag_base>(0);
	}

	bool is_not_none() const
	{
		return !is_none();
	}

	void clear_with_none()
	{
		this->value = ~static_cast<t_flag_base>(0);
	}

	/*constexpr*/
	static this_type_t none()
	{
		this_type_t flags { static_cast<t_flag_base>(~0) };
		return flags;
	}

	template <
		t_flag... t_bits>
	/*constexpr*/
	static this_type_t create()
	{
		static_assert(
			compute_flags<t_flag, t_bits...>::k_flags <= static_cast<t_flag_base>(~static_cast<t_flag_base>(0)),
			"Bits cannot be represented in the given underlying_type");

		this_type_t flags { static_cast<t_flag_base>(compute_flags<t_flag, t_bits...>::k_flags) };
		return flags;
	}

	template <
		t_flag t_first_bit,
		t_flag t_last_bit>
	/*constexpr*/
	static this_type_t create_range_mask()
	{
		// inline BIT_COUNT here
		static_assert(
			static_cast<t_flag_base>(t_first_bit) < (sizeof(t_flag_base) * 8),
			"Bit cannot be represented in the given underlying_type");
		// inline BIT_COUNT here
		static_assert(
			static_cast<t_flag_base>(t_last_bit) < (sizeof(t_flag_base) * 8),
			"Bit cannot be represented in the given underlying_type");
		static_assert(
			t_first_bit <= t_last_bit,
			"first_bit doesn't come before last_bit");

		this_type_t flags {
		static_cast<t_flag_base>(compute_flags_range_mask<t_flag, t_first_bit, t_last_bit>::k_range_mask)
		};
		return flags;
	}
};

#define FLAGS_T_ZERO(ns) (ns::flags_t::zero())
#define FLAGS_T_NONE(ns) (ns::flags_t::none())
#define FLAG_T(ns, b) (ns::flags_t::create<ns::b>())
#define FLAGS_T(ns, ...) (ns::flags_t::create<__VA_ARGS__>())
#define FLAGS_T_RANGE(ns, first_bit, last_bit) (ns::flags_t::create_range_mask<ns::first_bit, ns::last_bit>())
