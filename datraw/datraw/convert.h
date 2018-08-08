/// <copyright file="convert.h" company="Visualisierungsinstitut der Universität Stuttgart">
/// Copyright © 2017 Visualisierungsinstitut der Universität Stuttgart. Alle Rechte vorbehalten.
/// </copyright>
/// <author>Christoph Müller</author>

#pragma once

#include <algorithm>
#include <cassert>
#include <cinttypes>
#include <cstddef>
#include <limits>


namespace datraw {

    /// <summary>
    /// Convert the range of items designated by <paramref name="begin" /> and
    /// <paramref name="end" /> to type <tparamref name="T" /> and write the
    /// result to <paramref name="dst" />.
    /// </summary>
    /// <remarks>
    /// <para>The following conversion rules apply: If the type iterated by
    /// <typeparamref name="O" /> and <typeparamref name="T" /> are the same,
    /// a simple copy is performed. If both types are floating point types, a
    /// cast is performed, assuming that the data are within [0, 1].</para>
    /// <para>For all floating point types, it is assumed that valid values are
    /// within [0, 1]. Conversion to integral types is performed by scaling the
    /// values to the range of the type.</para>
    /// <para>Conversions from integral to other integral types are performed by
    /// rescaling to the range of <typeparamref name="T" /> via double precision
    /// floating point arithmetics.</para>
    /// </remarks>
    /// <tparam name="T">The target type.</tparam>
    /// <tparam name="I">The type of the input iterator.</tparam>
    /// <tparam name="O">The type of the output iterator.</tparam>
    /// <param name="begin">The begin of the range to be converted.</param>
    /// <param name="end">The end of the range to be converted.</param>
    /// <param name="dst">The beginning of the destination range, which must be
    /// able to hold the same number of elements as the source range.</param>
    template<class T, class I, class O> void convert(I begin, I end, O dst);

    /// <summary>
    /// Convert the range of items designated by <paramref name="begin" /> and
    /// <paramref name="end" /> to the value type of <tparamref name="O" /> and
    /// write the result to <paramref name="dst" />.
    /// </summary>
    /// <remarks>
    /// This version of the method tries to derive the desired target type from
    /// the output iterator. Note that this will not work for some kinds of
    /// output iterators like back inserters.
    /// </remarks>
    /// <tparam name="I">The type of the input iterator.</tparam>
    /// <tparam name="O">The type of the output iterator.</tparam>
    /// <param name="begin">The begin of the range to be converted.</param>
    /// <param name="end">The end of the range to be converted.</param>
    /// <param name="dst">The beginning of the destination range, which must be
    /// able to hold the same number of elements as the source range.</param>
    template<class I, class O> void convert(I begin, I end, O dst) {
        convert<typename O::value_type>(begin, end, dst);
    }

    /// <summary>
    /// Convert the byte order of <paramref name="cnt" /> numbers with a width
    /// of <tparamref name="T" /> bytes.
    /// </summary>
    /// <remarks>
    /// <para>This is the default case, which <b>does nothing</b>.</para>
    /// <para>There are template specialisation for the data type sizes that are
    /// supported for conversion.</para>
    /// <para>The conversion is performed in-place.</para>
    /// </remarks>
    /// <tparam name="T">The size of an number in bytes, which must be
    /// 1, 2, 4 or 8.</tparam>
    /// <param name="data">A pointer to the data to be converted.</param>
    /// <param name="cnt">The number of numbers designated by
    /// <paramref name="data" />, which of each is <tparamref name="T" /> bytes
    /// wide.</param>
    template<size_t T>
    inline void swap_byte_order(void *data, const size_t cnt) { }

    /// <summary>
    /// Convert the byte order of <paramref name="cnt" /> numbers with a width
    /// of 8 bytes each.
    /// </summary>
    /// <param name="data">A pointer to the data to be converted.</param>
    /// <param name="cnt">The number of numbers designated by
    /// <paramref name="data" />.</param>
    template<> inline void swap_byte_order<8>(void *data, const size_t cnt);

    /// <summary>
    /// Convert the byte order of <paramref name="cnt" /> numbers with a width
    /// of 4 bytes each.
    /// </summary>
    /// <param name="data">A pointer to the data to be converted.</param>
    /// <param name="cnt">The number of numbers designated by
    /// <paramref name="data" />.</param>
    template<> inline void swap_byte_order<4>(void *data, const size_t cnt);

    /// <summary>
    /// Convert the byte order of <paramref name="cnt" /> numbers with a width
    /// of 2 bytes each.
    /// </summary>
    /// <param name="data">A pointer to the data to be converted.</param>
    /// <param name="cnt">The number of numbers designated by
    /// <paramref name="data" />.</param>
    template<> inline void swap_byte_order<2>(void *data, const size_t cnt);

    /// <summary>
    /// Convert the byte order of <paramref name="cnt" /> numbers with a
    /// <paramref name="width" /> bytes each.
    /// </summary>
    /// <param name="width">The width of a number in bytes, which must be
    /// 2, 4 or 8 in order for the implementation to do anything.</param>
    /// <param name="data">A pointer to the data to be converted.</param>
    /// <param name="cnt">The number of numbers designated by
    /// <paramref name="data" />.</param>
    inline void swap_byte_order(const size_t width, void *data,
        const size_t cnt);


} /* end namespace datraw */

#include "datraw/convert.inl"
