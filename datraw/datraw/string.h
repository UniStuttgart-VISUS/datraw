/// <copyright file="string.h" company="Visualisierungsinstitut der Universität Stuttgart">
/// Copyright © 2017 Visualisierungsinstitut der Universität Stuttgart. Alle Rechte vorbehalten.
/// </copyright>
/// <author>Christoph Müller</author>

#pragma once

#include <algorithm>
#if (!defined(__GNUC__) || (__GNUC__ >= 5))
#include <codecvt>
#endif /* (!defined(__GNUC__) || (__GNUC__ >= 5)) */
#include <cctype>
#include <functional>
#include <stdexcept>
#include <string>


namespace datraw {
namespace detail {

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
    inline std::string narrow_string(const std::wstring& str) {
#if (!defined(__GNUC__) || (__GNUC__ >= 5))
        static std::wstring_convert<std::codecvt_utf8<wchar_t>> cvt;
        return cvt.to_bytes(str);
#else /* (!defined(__GNUC__) || (__GNUC__ >= 5)) */
        std::string retval;
        retval.reserve(str.size());
        for (auto c : str) {
            retval.push_back(c);
        }
        return retval;
#endif /* (!defined(__GNUC__) || (__GNUC__ >= 5)) */
    }

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
            std::not1(std::ptr_fun<int, int>(std::isspace)));
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
            std::not1(std::ptr_fun<int, int>(std::isspace)));
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

} /* end namespace detail */
} /* end namespace datraw */
