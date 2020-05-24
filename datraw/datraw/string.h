/// <copyright file="string.h" company="Visualisierungsinstitut der Universität Stuttgart">
/// Copyright © 2017 - 2020 Visualisierungsinstitut der Universität Stuttgart. Alle Rechte vorbehalten.
/// </copyright>
/// <author>Christoph Müller</author>

#pragma once

#include <algorithm>
#if (!defined(__GNUC__) || (__GNUC__ >= 5))
#include <codecvt>
#endif /* (!defined(__GNUC__) || (__GNUC__ >= 5)) */
#if defined(_WIN32)
#include <conio.h>
#endif /* defined(_WIN32) */
#include <cctype>
#include <cstdio>
#include <cwchar>
#include <functional>
#include <stdexcept>
#include <string>
#include <vector>


namespace datraw {
namespace detail {

    /// <summary>
    /// Perform printf-style formatting into a string object.
    /// </summary>
    /// <param name="fmt">The format string.</param>
    /// <param name="args">The arguments to be formatted.</param>
    /// <returns>The formatted string.</returns>
    template<class... A>
    std::string format(const std::string& fmt, A... args);

    /// <summary>
    /// Perform printf-style formatting into a string object.
    /// </summary>
    /// <param name="fmt">The format string.</param>
    /// <param name="args">The arguments to be formatted.</param>
    /// <returns>The formatted string.</returns>
    template<class... A>
    std::wstring format(const std::wstring& fmt, A... args);

    /// <summary>
    /// Ensure a narrow string.
    /// </summary>
    /// <param name="str"></param>
    /// <returns></returns>
    inline const std::string& narrow_string(const std::string& str) {
        return str;
    }

    /// <summary>
    /// Ensure a narrow string.
    /// </summary>
    /// <param name="str"></param>
    /// <returns></returns>
    inline std::string narrow_string(const std::wstring& str);

    /// <summary>
    /// Answer an upper case-version of <paramref name="str" />.
    /// </summary>
    /// <tparam name="S">The type of string being processed.</tparam>
    /// <param name="str"></param>
    /// <returns></returns>
    template<class S> inline S to_upper(S str) {
        std::transform(str.begin(), str.end(), str.begin(), ::toupper);
        return str;
    }

    /// <summary>
    /// Remove all leading white-space characters from
    /// <paramref name="str" />.
    /// </summary>
    /// <tparam name="S">The type of string being processed.</tparam>
    /// <param name="str"></param>
    /// <returns></returns>
    template<class S> inline S trim_left(const S& str) {
        auto end = str.cend();
        auto begin = std::find_if(str.cbegin(), end,
            [](int ch) { return !std::isspace(ch); });
        return S(begin, str.cend());
    }

    /// <summary>
    /// Remove all trailing white-space characters from
    /// <paramref name="str" />.
    /// </summary>
    /// <tparam name="S">The type of string being processed.</tparam>
    /// <param name="str"></param>
    /// <returns></returns>
    template<class S> inline S trim_right(const S& str) {
        auto end = std::find_if(str.crbegin(), str.crend(),
            [](int ch) { return !std::isspace(ch); });
        return S(str.cbegin(), end.base());
    }

    /// <summary>
    /// Remove all leading and trailing white-space characters from
    /// <paramref name="str" />.
    /// </summary>
    /// <tparam name="S">The type of string being processed.</tparam>
    /// <param name="str"></param>
    /// <returns></returns>
    template<class S> inline S trim(const S& str) {
        return trim_left(trim_right(str));
    }

    /// <summary>
    /// Ensure a wide string.
    /// </summary>
    /// <param name="str"></param>
    /// <returns></returns>
    inline std::wstring widen_string(const std::string& str);

    /// <summary>
    /// Ensure a wide string.
    /// </summary>
    /// <param name="str"></param>
    /// <returns></returns>
    inline const std::wstring widen_string(const std::wstring& str) {
        return str;
    }

} /* end namespace detail */
} /* end namespace datraw */

#include "datraw/string.inl"
