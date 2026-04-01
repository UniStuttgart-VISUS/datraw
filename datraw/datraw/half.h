// <copyright file="half.h" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2026 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#if !defined(_DATRAW_HALF_H)
#define _DATRAW_HALF_H
#pragma once

#include <cinttypes>
#include <cstring>
#include <cstdint>
#include <limits>
#include <type_traits>

#if defined(_MSC_VER)
#include <intrin.h>
#endif /* defined(_MSC_VER) */

#if (defined(_M_IX86) || defined(_M_X64))
#include <immintrin.h>
#endif /* (defined(_M_IX86) || defined(_M_X64)) */

#if (defined(_M_ARM) || defined(_M_ARM64) || defined(__aarch64__))
#include <arm_neon.h>
#endif /* (defined(_M_ARM) || defined(_M_ARM64) || defined(__aarch64__)) */

#include "datraw/abi.h"


DATRAW_DETAIL_NAMESPACE_BEGIN

/// <summary>
/// Allows for defining constants that can be interpreted either as
/// floating-point values or as bit patterns. In absence of the C++ 20
/// bit_cast, this is the only way to achieve the reinterpretation in
/// a constexpr context.
/// </summary>
union float_bits final {
    float as_float;
    std::uint32_t as_uint;

    inline constexpr float_bits(const float value,
        std::true_type) noexcept : as_float(value) { }

    inline constexpr float_bits(const std::uint32_t value,
        std::false_type) noexcept : as_uint(value) { }

    inline constexpr operator float(void) const noexcept {
        return this->as_float;
    }

    inline constexpr operator std::uint32_t(void) const noexcept {
        return this->as_uint;
    }
};


/// <summary>
/// Constructs <see cref="float_bits"/> from a bit representation.
/// </summary>
inline constexpr float_bits make_float_from_bits(
    const std::uint32_t value) noexcept {
    return float_bits(value, std::false_type());
}


/// <summary>
/// Counts the number of leading zero bits in the given
/// <paramref name="value" />.
/// </summary>
inline std::uint32_t leading_zeros(const std::uint32_t value) noexcept {
#if defined(_MSC_VER)
    unsigned long i;
    _BitScanReverse(&i, value);
    return (31 - i);
#else /* defined(_MSC_VER) */
    return static_cast<std::uint32_t>(__builtin_clz(value));
#endif /* defined(_MSC_VER) */
}


/// <summary>
/// Converts between bit patterns for 16-bit float and 32-bit float values.
/// </summary>
/// <remarks>
/// <para>The default implementation uses the IEEE 754-2008 format.</para>
/// <para>The converter uses the implementation from 
/// https://github.com/Maratyszcza/FP16/blob/master/include/fp16/fp16.h rather
/// than the one from the old C implementation because the former requires far
/// less branching.</para>
/// </remarks>
/// <typeparam name="Arm"></typeparam>
/// <typeparam name="Intrinsic"></typeparam>
template<bool Arm, bool Intrinsic> struct half_converter final {

    typedef std::uint16_t half_type;

    static constexpr auto sign_mask = UINT32_C(0x80000000);

    static inline float to_float(const half_type half) noexcept {
        const auto bits = static_cast<std::uint32_t>(half) << 16;
        const auto sign = bits & sign_mask;
        const auto value = bits & ~sign_mask;

        auto shift = leading_zeros(value);
        shift = (shift > 5) ? shift - 5 : 0;

        const auto infnan_mask = (static_cast<std::int32_t>(value
            + UINT32_C(0x04000000)) >> 8) & 0x7F800000;
        const auto zero_mask = ~(static_cast<std::int32_t>(value - 1) >> 31);

        const auto retval = make_float_from_bits(sign
            | (((((value << shift) >> 3) + ((0x70 - shift) << 23))
            | infnan_mask)) & zero_mask);
        return retval.as_float;
    }

    static inline half_type to_half(const float flt) noexcept {
        constexpr auto bias_mask = UINT32_C(0xFF000000);
        constexpr auto min_bias = UINT32_C(0x71000000);
        constexpr auto scalei = make_float_from_bits(0x77800000);
        constexpr auto scalez = make_float_from_bits(0x08800000);

        float_bits bits(flt, std::true_type());
        const auto sign = bits.as_uint & sign_mask;
        const auto two = bits.as_uint + bits.as_uint;

        auto bias = two & bias_mask;
        if (bias < min_bias) {
            bias = min_bias;
        }

        bits.as_uint = ((bias >> 1) + UINT32_C(0x07800000));
        bits.as_float += (std::fabs(flt) * scalei.as_float) * scalez.as_float;
        const auto exp = (bits.as_uint >> 13) & UINT32_C(0x00007C00);
        const auto mantissa = bits.as_uint & UINT32_C(0x00000FFF);
        const auto value = exp + mantissa;

        return (sign >> 16) | ((two > bias_mask) ? UINT16_C(0x7E00) : value);
    }
};


#if (defined(_MSC_VER) && (defined(_M_IX86) || defined(_M_X64)))
/// <summary>
/// Specialisation using AVX2 intrinsics for the conversion.
/// </summary>
template<> struct half_converter<false, true> final {
    typedef std::uint16_t half_type;

    static inline float to_float(const half_type half) noexcept {
        const auto h = static_cast<int>(static_cast<std::uint32_t>(half));
        return _mm_cvtss_f32(_mm_cvtph_ps(_mm_cvtsi32_si128(h)));
    }

    static inline half_type to_half(const float flt) noexcept {
        const auto h = _mm_cvtsi128_si32(_mm_cvtps_ph(_mm_set_ss(flt),
            _MM_FROUND_CUR_DIRECTION));
        return static_cast<half_type>(h);
    }
};
#endif /* (defined(_MSC_VER) && (defined(_M_IX86) || defined(_M_X64))) */


#if (defined(_M_ARM) || defined(_M_ARM64) || defined(__aarch64__))
/// <summary>
/// Specialisation using ARM NEON intrinsics for the conversion.
/// </summary>
template<> struct half_converter<false, true> final {
    typedef std::uint16_t half_type;

    static inline float to_float(const half_type half) noexcept {
        return vgetq_lane_f32(vcvt_f32_f16(vreinterpret_f16_u16(
            vdup_n_u16(half))), 0);
    }

    static inline half_type to_half(const float flt) noexcept {
        return vget_lane_u16(vcvt_f16_f32(vdupq_n_f32(flt)), 0);
    }
};
#endif /* (defined(_M_ARM) || defined(_M_ARM64) || defined(__aarch64__)) */

DATRAW_DETAIL_NAMESPACE_END


DATRAW_NAMESPACE_BEGIN

/// <summary>
/// Implements a 16-bit floating point type.
/// </summary>
/// <typeparam name="Arm">Controls whether the ARM variant without special
/// handling of infinity and NaN is used or not.</typeparam>
template<bool Arm> class basic_half final {

    typedef detail::half_converter<Arm, false> _converter;

public:

    /// <summary>
    /// Constructs a <see cref="basic_half"/> from a 16-bit pattern.
    /// </summary>
    /// <param name="bits">The bit pattern representing the number.</param>
    /// <returns>The half-precision floating-point number.</returns>
    inline static constexpr basic_half from_bits(
            const std::uint16_t bits) noexcept {
        basic_half result;
        result._value = bits;
        return result;
    }

    /// <summary>
    /// Initialises a new instance.
    /// </summary>
    inline constexpr basic_half(void) noexcept : _value(0) { }

    /// <summary>
    /// Initialises a new instance.
    /// </summary>
    /// <param name="value">A 32-bit floating-point value to be
    /// converted.</param>
    inline constexpr basic_half(const float value) noexcept
        : _value(_converter::to_half(value)) { }

    /// <summary>
    /// Test for equality.
    /// </summary>
    /// <param name="rhs">The right-hand-side operand.</param>
    /// <returns><see langword="true"/> if this object and
    /// <paramref name="rhs "/> represent the same value,
    /// <see langword="false" /> otherwise.</returns>
    inline bool operator ==(const basic_half rhs) const noexcept {
        return (this->_value == rhs._value);
    }

    /// <summary>
    /// Test for inequality.
    /// </summary>
    /// <param name="rhs">The right-hand-side operand.</param>
    /// <returns><see langword="true"/> if this object and
    /// <paramref name="rhs "/> represent different values,
    /// <see langword="false" /> otherwise.</returns>
    inline bool operator !=(const basic_half rhs) const noexcept {
        return (this->_value != rhs._value);
    }

    /// <summary>
    /// Converts the value to a 32-bit floating-point representation.
    /// </summary>
    /// <returns>The 32-bit representation of this value.</returns>
    inline constexpr operator float(void) const noexcept {
        return _converter::to_float(this->_value);
    }

private:

    typename _converter::half_type _value;
};


/// <summary>
/// An IEEE 754-2008 half-precision floating point type.
/// </summary>
typedef basic_half<false> half;
static_assert(sizeof(half) == 2, "The size of half must be 2 bytes.");

DATRAW_NAMESPACE_END


/// <summary>
/// Specialisation of STL numeric limits for 16-bit floating-point numbers.
/// </summary>
/// <typeparam name="Arm"></typeparam>
template<bool Arm>
class std::numeric_limits<DATRAW_NAMESPACE::basic_half<Arm>> final {

    typedef DATRAW_NAMESPACE::basic_half<Arm> _type;

public:

    static constexpr int digits = 11;
    static constexpr int digits10 = 3;
    static constexpr float_denorm_style has_denorm = std::denorm_present;
    static constexpr bool has_infinity = true;
    static constexpr bool has_quiet_NaN = true;
    static constexpr bool is_exact = false;
    static constexpr bool is_iec559 = true;
    static constexpr bool is_modulo = false;
    static constexpr bool is_signed = true;
    static constexpr int max_digits10 = 5;
    static constexpr int max_exponent = 16;
    static constexpr int max_exponent10 = 4;
    static constexpr int min_exponent = -13;
    static constexpr int min_exponent10 = -4;
    static constexpr int radix = 2;
    static constexpr float_round_style round_style = std::round_indeterminate;

    //static constexpr _type min() noexcept { return 0x0400; }
    //static constexpr _type lowest() noexcept { return 0xFBFF; }
    //static constexpr _type max() noexcept { return 0x7BFF; }
    //static constexpr _type epsilon() noexcept { return 0x1400; }
    //static constexpr _type round_error() noexcept {
    //    return (round_style == std::round_to_nearest) ? 0x3800 : 0x3C00;
    //}
    //static constexpr _type infinity() noexcept { return 0x7C00; }
    //static constexpr _type quiet_NaN() noexcept { return 0x7FFF; }
    //static constexpr _type signaling_NaN() noexcept { return 0x7DFF; }
    //static constexpr _type denorm_min() noexcept { return 0x0001; }
};

#endif /* !defined(_DATRAW_HALF_H) */
