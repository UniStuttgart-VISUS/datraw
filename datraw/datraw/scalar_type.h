// <copyright file="scalar_type.h" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2017 - 2025 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#if !defined(_DATRAW_SCALAR_TYPE_H)
#define _DATRAW_SCALAR_TYPE_H
#pragma once

#include "datraw/types.h"


DATRAW_NAMESPACE_BEGIN

/// <summary>
/// Representation of scalars used in raw files.
/// </summary>
enum class scalar_type {

    /// <summary>
    /// The data are raw bytes that cannot be interpreted by the library.
    /// </summary>
    /// <remarks>
    /// Raw data cannot be reflected using our traits classes, most
    /// importantly, the library cannot compute the size of a frame from
    /// this. This value is only as fallback as files in this format are
    /// basically useless.
    /// </remarks>
    raw = 0,

    int8,
    int16,
    int32,
    int64,

    uint8,
    uint16,
    uint32,
    uint64,

    float16,
    float32,
    float64

    // Add new members here. Do not forget to add them is the dispatch list
    // below, too.
};


/// <summary>
/// A traits type allowing for deriving the C++ type from a
/// <see cref="scalar_type" />.
/// </summary>
template<scalar_type T> struct scalar_forward_traits {
    typedef void *type;
};

/// <summary>
/// A traits type allowing for deriving the <see cref="scalar_type" /> from a
/// C++ type.
/// </summary>
template<class T> struct scalar_reverse_traits {
    static constexpr scalar_type type = scalar_type::raw;
};

#define __DATRAW_DECL_SCALAR_TYPE_TRAITS(t)                                    \
template<> struct scalar_forward_traits<scalar_type::t> {                      \
    typedef t type;                                                            \
};                                                                             \
template<> struct scalar_reverse_traits<datraw::t> {                           \
    static constexpr scalar_type type = scalar_type::t;                        \
}

__DATRAW_DECL_SCALAR_TYPE_TRAITS(int8);
__DATRAW_DECL_SCALAR_TYPE_TRAITS(int16);
__DATRAW_DECL_SCALAR_TYPE_TRAITS(int32);
__DATRAW_DECL_SCALAR_TYPE_TRAITS(int64);

__DATRAW_DECL_SCALAR_TYPE_TRAITS(uint8);
__DATRAW_DECL_SCALAR_TYPE_TRAITS(uint16);
__DATRAW_DECL_SCALAR_TYPE_TRAITS(uint32);
__DATRAW_DECL_SCALAR_TYPE_TRAITS(uint64);

__DATRAW_DECL_SCALAR_TYPE_TRAITS(float16);
__DATRAW_DECL_SCALAR_TYPE_TRAITS(float32);
__DATRAW_DECL_SCALAR_TYPE_TRAITS(float64);

#undef __DATRAW_DECL_SCALAR_TYPE_TRAITS


/// <summary>
/// Resolves the C++ type for the given <typeparamref name="Scalar" /> type.
/// </summary>
/// <typeparam name="Scalar">The scalar type to resolve the C++ type of.
/// </typeparam>
template<scalar_type Scalar>
using scalar_t = typename scalar_forward_traits<Scalar>::type;


/// <summary>
/// Resolves the <see cref="scalar_type" /> for the given C++ type.
/// </summary>
/// <typeparam name="TType">A C++ type to determine the scalar type for.
/// </typeparam>
template<class TType>
constexpr scalar_type scalar_type_v = scalar_reverse_traits<TType>::type;

DATRAW_NAMESPACE_END


DATRAW_DETAIL_NAMESPACE_BEGIN

/// <summary>
/// A list of scalar types to expand.
/// </summary>
template<scalar_type...> struct scalar_type_list_t { };

/// <summary>
/// The actual type list we use in 
/// <see cref="variant::conditional_invoke" />.
/// </summary>
/// <remarks>
/// New types in <see cref="variant_type" /> must be added here.
/// </remarks>
typedef scalar_type_list_t<scalar_type::int8, scalar_type::int16,
    scalar_type::int32, scalar_type::int64, scalar_type::uint8,
    scalar_type::uint16, scalar_type::uint32, scalar_type::uint64,
    scalar_type::float16, scalar_type::float32, scalar_type::float64
    /* Add new members here. */>
    scalar_type_list;

/// <summary>
/// Resolve the size of the given scalar type using the
/// <see cref="scalar_type_list" /> above.
/// </summary>
template<scalar_type T, scalar_type... Ts>
inline constexpr std::size_t get_scalar_size(scalar_type_list_t<T, Ts...>,
        const scalar_type type) noexcept {
    return (type == T)
        ? sizeof(typename scalar_forward_traits<T>::type)
        : get_scalar_size(scalar_type_list_t<Ts...>(), type);
}

/// <summary>
/// Recursion stop for <see cref="get_scalar_size" />, which will
/// return 0.
/// </summary>
inline constexpr std::size_t get_scalar_size(
        detail::scalar_type_list_t<>,
        const scalar_type) noexcept {
    return 0;
}

DATRAW_DETAIL_NAMESPACE_END


DATRAW_NAMESPACE_BEGIN

/// <summary>
/// Answer the size of a scalar of the given type in bytes.
/// </summary>
/// <param name="type">The scalar type to get the size for.</param>
/// <returns>The size of a scalar in bytes or 0 if the size could not be
/// determined, eg because the scalar is typeless.</returns>
inline constexpr std::size_t get_scalar_size(const scalar_type type) noexcept {
    return detail::get_scalar_size(detail::scalar_type_list(), type);
}

DATRAW_NAMESPACE_END

#endif /* !defined(_DATRAW_SCALAR_TYPE_H) */
