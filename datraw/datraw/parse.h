/// <copyright file="parse.h" company="Visualisierungsinstitut der Universität Stuttgart">
/// Copyright © 2017 Visualisierungsinstitut der Universität Stuttgart. Alle Rechte vorbehalten.
/// </copyright>
/// <author>Christoph Müller</author>

#pragma once

#include <cinttypes>
#include <streambuf>
#include <sstream>
#include <string>
#include <type_traits>

#include "datraw/string.h"
#include "datraw/variant.h"


namespace datraw {
namespace detail {

    /// <summary>
    /// Find, if any, the array entry for the given tag.
    /// </summary>
    /// <tparam name="T"></tparam>
    /// <tparam name="N"></tparam>
    /// <tparam name="S">The type of string being processed.</tparam>
    template<class T, size_t N, class S>
    static const T *find_tag(const T(&tags)[N], const S& tag);

    /// <summary>
    /// Generic functor parsing a variable of type <tparamref name="T" /> from
    /// a string <paramref name="str" />.
    /// </summary>
    /// <remarks>
    /// We need to use a <c>struct</c> here, because functions and methods would
    /// not allow us to provide partial specialisations for certain types.
    /// </remarks>
    /// <tparam name="S">The type of string being processed.</tparam>
    /// <tparam name="T">The type to be parsed from a string.</tparam>
    template<class S, class T> struct parser {
        typedef typename S::value_type char_type;
        typedef S string_type;
        typedef T value_type;

        static value_type parse(const string_type& str);
    };

    /// <summary>
    /// Specialisation for bytes, which is required because otherwise,
    /// the implementation would try to extract a character rather than
    /// parsing a number from the string.
    /// </summary>
    /// <tparam name="S">The type of string being processed.</tparam>
    template<class S> struct parser<S, std::int8_t> {
        typedef typename S::value_type char_type;
        typedef S string_type;
        typedef std::int8_t value_type;

        static inline value_type parse(const string_type& str) {
            auto retval = parser<string_type, std::int32_t>::parse(str);
            return static_cast<value_type>(retval);
        }
    };

    /// <summary>
    /// Specialisation for bytes, which is required because otherwise,
    /// the implementation would try to extract a character rather than
    /// parsing a number from the string.
    /// </summary>
    /// <tparam name="S">The type of string being processed.</tparam>
    template<class S> struct parser<S, std::uint8_t> {
        typedef typename S::value_type char_type;
        typedef S string_type;
        typedef std::uint8_t value_type;

        static inline value_type parse(const string_type& str) {
            auto retval = parser<string_type, std::int32_t>::parse(str);
            return static_cast<value_type>(retval);
        }
    };

    /// <summary>
    /// Parses <see cref="endianness" /> from <paramref name="str" />.
    /// </summary>
    /// <tparam name="S">The type of string being processed.</tparam>
    template<class S> struct parser<S, datraw::endianness> {
        typedef typename S::value_type char_type;
        typedef S string_type;
        typedef datraw::endianness value_type;

        static value_type parse(const string_type& str);
    };

    /// <summary>
    /// Parses <see cref="grid_type" /> from <paramref name="str" />.
    /// </summary>
    /// <tparam name="S">The type of string being processed.</tparam>
    template<class S> struct parser<S, datraw::grid_type> {
        typedef typename S::value_type char_type;
        typedef S string_type;
        typedef datraw::grid_type value_type;

        static value_type parse(const string_type& str);
    };

    /// <summary>
    /// Parses <see cref="scalar_type" /> from <paramref name="str" />.
    /// </summary>
    /// <tparam name="S">The type of string being processed.</tparam>
    template<class S> struct parser<S, datraw::scalar_type> {
        typedef typename S::value_type char_type;
        typedef S string_type;
        typedef datraw::scalar_type value_type;

        static value_type parse(const string_type& str);
    };

} /* end namespace detail */

    /// <summary>
    /// Tries parsing a value of the given type <tparamref name="T" /> from 
    /// the given string <paramref name="str" />.
    /// <summary>
    /// <tparam name="T">The type to be parsed from a string.</tparam>
    /// <tparam name="S">The type of string being processed.</tparam>
    /// <param name="str">The string to be parsed.</param>
    /// <returns>The value represented by the given string.</returns>
    /// <exception cref="std::invalid_argument">In case the string did
    /// not represent a valid value of type <tparamref name="T" />
    /// </exception>
    template<class T, class S> inline T parse(const S& str) {
        return detail::parser<S, T>::parse(str);
    }

    /// <summary>
    /// Parse one of <see cref="strstrm_parsables_t" /> from
    /// <paramref name="str" />.
    /// </summary>
    /// <tparam name="T">The type to be parsed from a string.</tparam>
    /// <tparam name="S">The type of string being processed.</tparam>
    /// <param name="str">The string to be parsed.</param>
    /// <returns>The value represented by the given string.</returns>
    /// <exception cref="std::invalid_argument">In case the string did
    /// not represent a valid value of type <tparamref name="T" />
    /// </exception>
    template<datraw::variant_type T, class S>
    inline typename detail::variant_fwd_traits<typename S::value_type, T>::value_type parse(const S& str) {
        return parse<typename detail::variant_fwd_traits<typename S::value_type, T>::value_type>(str);
    }

} /* end namespace datraw */

#include "datraw/parse.inl"
