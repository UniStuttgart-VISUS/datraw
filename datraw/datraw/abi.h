// <copyright file="abi.h" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2025 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#if !defined(_DATRAW_ABI_H)
#define _DATRAW_ABI_H
#pragma once


#define DATRAW_ABI_MAJOR 1
#define DATRAW_ABI_MINOR 2


#define _DATRAW_CONCAT0(l, r) l ## r
#define _DATRAW_CONCAT(l, r) _DATRAW_CONCAT0(l, r)


/// <summary>
/// Creates the full ABI versison string.
/// </summary>
/// <param name="mj">The major version of the ABI.</param>
/// <param name="mn">The minor version of the ABI.</param>
#define DATRAW_ABI_VERSION(mj, mn) _DATRAW_CONCAT(mj, _DATRAW_CONCAT(_, mn))


/// <summary>
/// Creates the name of the namespace for the specified ABI version.
/// </summary>
/// <param name="mj">The major version of the ABI.</param>
/// <param name="mn">The minor version of the ABI.</param>
#define DATRAW_ABI_NAMESPACE(mj, mn)\
    _DATRAW_CONCAT(v, DATRAW_ABI_VERSION(mj, mn))


/// <summary>
/// Creates the name of the current ABI namespace.
/// </summary>
#define DATRAW_NAMESPACE datraw::DATRAW_ABI_NAMESPACE(DATRAW_ABI_MAJOR,\
    DATRAW_ABI_MINOR)


/// <summary>
/// Marks the begin of the current ABI namespace.
/// </summary>
#define DATRAW_NAMESPACE_BEGIN namespace datraw {\
    inline namespace DATRAW_ABI_NAMESPACE(DATRAW_ABI_MAJOR, DATRAW_ABI_MINOR) {


/// <summary>
/// Marks the end of the current ABI namespace.
/// </summary>
#define DATRAW_NAMESPACE_END } }


/// <summary>
/// Create the name of the current namespace for implementation details
/// </summary>
#define DATRAW_DETAIL_NAMESPACE DATRAW_NAMESPACE::detail


/// <summary>
/// Marks the begin of the current namespace for implementation details.
/// </summary>
#define DATRAW_DETAIL_NAMESPACE_BEGIN DATRAW_NAMESPACE_BEGIN\
    namespace detail {


/// <summary>
/// Marks the end of the current namespace for implementation details.
/// </summary>
#define DATRAW_DETAIL_NAMESPACE_END } DATRAW_NAMESPACE_END


#if defined(__cpp_if_constexpr)
#define DATRAW_CONSTEXPR_IF constexpr
#else /* defined(__cpp_if_constexpr) */
#define DATRAW_CONSTEXPR_IF
#endif /* defined(__cpp_if_constexpr) */

#endif /* !defined(_DATRAW_ABI_H) */
