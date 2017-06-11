/// <copyright file="scalar_type.h" company="Visualisierungsinstitut der Universität Stuttgart">
/// Copyright © 2017 Visualisierungsinstitut der Universität Stuttgart. Alle Rechte vorbehalten.
/// </copyright>
/// <author>Christoph Müller</author>

#pragma once

#include "datraw/api.h"
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
    template<> struct DATRAW_API scalar_forward_traits<scalar_type::t> {       \
        typedef datraw::t type;                                                \
    };                                                                         \
    template<> struct DATRAW_API scalar_reverse_traits<datraw::t> {            \
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

    //__DATRAW_DECL_SCALAR_TYPE_TRAITS(float16);
    __DATRAW_DECL_SCALAR_TYPE_TRAITS(float32);
    __DATRAW_DECL_SCALAR_TYPE_TRAITS(float64);

#undef __DATRAW_DECL_SCALAR_TYPE_TRAITS

}
