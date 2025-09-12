// <copyright file="literal.h" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2017 - 2025 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#if !defined(_DATRAW_LITERAL_H)
#define _DATRAW_LITERAL_H
#pragma once

#include "datraw/abi.h"


DATRAW_DETAIL_NAMESPACE_BEGIN

template<class T> struct literal_selector;

template<> struct literal_selector<char> final {
    typedef char char_type;
    static inline constexpr char_type select(
            const char n,
            const wchar_t) noexcept {
        return n;
    }
    static inline constexpr const char_type *select(
            const char *n,
            const wchar_t *) noexcept {
        return n;
    }
};

template<> struct literal_selector<wchar_t> final {
    typedef wchar_t char_type;
    static inline constexpr char_type select(
            const char,
            const wchar_t w) noexcept {
        return w;
    }
    static inline constexpr const char_type *select(
            const char *,
            const wchar_t *w) noexcept {
        return w;
    }
};

DATRAW_DETAIL_NAMESPACE_END

/// <summary>
/// Yields a literal based on the template parameter <paramref name="T" />.
/// </summary>
#define DATRAW_TPL_LITERAL(T, l)\
    DATRAW_DETAIL_NAMESPACE::literal_selector<T>::select(l, L##l)

#endif /* !defined(_DATRAW_LITERAL_H) */
