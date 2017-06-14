/// <copyright file="scalar_type.h" company="Visualisierungsinstitut der Universität Stuttgart">
/// Copyright © 2017 Visualisierungsinstitut der Universität Stuttgart. Alle Rechte vorbehalten.
/// </copyright>
/// <author>Christoph Müller</author>

#pragma once

#include "datraw/types.h"


namespace datraw {

    /// <summary>
    /// Representation of scalars used in raw files.
    /// </summary>
    enum class scalar_type {
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
    };


    /// <summary>
    /// A traits type allowing for deriving the C++ type from a
    /// <see cref="scalar_type" />.
    /// </summary>
    template<scalar_type T> struct scalar_forward_traits {
        typedef void *type;
    };

    /// <summary>
    /// A traits type allowing for deriving the <see cref="scalar_type" />
    /// from a C++ type.
    /// </summary>
    template<class T> struct scalar_reverse_traits {
        static const scalar_type type = scalar_type::raw;
    };

#define __DATRAW_DECL_SCALAR_TYPE_TRAITS(t)                                    \
    template<> struct scalar_forward_traits<scalar_type::t> {                  \
        typedef datraw::t type;                                                \
    };                                                                         \
    template<> struct scalar_reverse_traits<datraw::t> {                       \
        static const scalar_type type = scalar_type::t;                        \
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


namespace detail {

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
    template<datraw::scalar_type T, datraw::scalar_type... Ts>
    inline size_t get_scalar_size(scalar_type_list_t<T, Ts...>,
            const scalar_type type) {
        return (type == T)
            ? sizeof(typename scalar_forward_traits<T>::type)
            : get_scalar_size(scalar_type_list_t<Ts...>(), type);
    }

    /// <summary>
    /// Recursion stop for <see cref="get_scalar_size" />, which will
    /// return 0.
    /// </summary>
    inline static size_t get_scalar_size(detail::scalar_type_list_t<>,
            const scalar_type type) {
        return 0;
    }

} /* end namespace detail */


    /// <summary>
    /// Answer the size of a scalar of the given type in bytes.
    /// </summary>
    /// <param name="type">The scalar type to get the size for.</param>
    /// <returns>The size of a scalar in bytes or 0 if the size could not
    /// be determined, eg because the scalar is typeless.</returns>
    inline size_t get_scalar_size(const scalar_type type) {
        return detail::get_scalar_size(detail::scalar_type_list(), type);
    }

} /* end namespace datraw */
