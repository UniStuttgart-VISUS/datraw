/// <copyright file="info.h" company="Visualisierungsinstitut der Universität Stuttgart">
/// Copyright © 2017 Visualisierungsinstitut der Universität Stuttgart. Alle Rechte vorbehalten.
/// </copyright>
/// <author>Christoph Müller</author>

#pragma once

#include <algorithm>
#include <cctype>
#include <fstream>
#include <stdexcept>
#include <streambuf>
#include <string>
#include <unordered_map>
#include <vector>

#include "datraw/grid_type.h"
#include "datraw/types.h"
#include "datraw/variant.h"


namespace datraw {

    /// <summary>
    /// Holds the content of the dat file, which describes the raw data.
    /// </summary>
    /// <tparam name="C">The character type used for strings.</tparam>
    template<class C> class info {

    public:

        /// <summary>
        /// The type used to represent a single character in a string.
        /// </summary>
        typedef C char_type;

        /// <summary>
        /// The type used to represent a string.
        /// </summary>
        typedef std::basic_string<char_type> string_type;

        /// <summary>
        /// The type used to represent data of variable type.
        /// </summary>
        typedef basic_variant<char_type> variant_type;

        /// <summary>
        /// Parses the given dat file.
        /// </summary>
        /// <param name="file"></param>
        /// <returns>The parsed dat file.</returns>
        static info parse(const string_type& file);

        /// <summary>
        /// Skip all white space characters in the given range.
        /// </summary>
        /// <tparam name="I">The type of character iterator.</tparam>
        /// <param name="begin">The begin of the string.</param>
        /// <param name="end">The end of the string.</param>
        /// <returns>The first non-space character at or after
        /// <paramref name="it" />, but before <paramref name="end" />.
        /// </returns>
        template<class I> static I skip_spaces(I it, I end);

        /// <summmary>
        /// Split a range of characters at positions designated by
        /// <paramref name="predicate" />.
        /// </summmary>
        /// <tparam name="I">The type of character iterator.</tparam>
        /// <tparam name="P">The type of the predicate functor.</tparam>
        /// <param name="begin">The begin of the string.</param>
        /// <param name="end">The end of the string.</param>
        /// <param name="predicate">The splitting predicate.</param>
        /// <returns>Iterators representing the borders of the tokens.</returns>
        template<class I, class P>
        static std::vector<I> tokenise(I begin, I end, P predicate);

        /// <summary>
        /// Split a range of characters into tokens defined by the list of
        /// delimiters <paramref name="delims" />.
        /// </summary>
        /// <tparam name="I">The type of character iterator.</tparam>
        /// <tparam name="D">The type of the delimiter list.</tparam>
        /// <param name="begin">The begin of the string.</param>
        /// <param name="end">The end of the string.</param>
        /// <param name="delimiters">The list of delimiters.</param>
        /// <returns>Iterators representing the borders of the tokens.</returns>
        template<class I, class... D>
        static std::vector<I> tokenise(I begin, I end, D... delimiters);

        /// <summary>
        /// Initialises an empty instance.
        /// </summary>
        inline info(void) { }

    private:

        /// <summary>
        /// Answer the endianness of the system we are running on.
        /// </summary>
        datraw::endianness sys_endianness(void);

        /// <summary>
        /// A collection type storing all the properties parsed from the dat
        /// file.
        /// </summary>
        typedef std::unordered_map<string_type, variant_type> property_list_type;

        /// <summary>
        /// The properties stored in the dat file.
        /// </summary>
        property_list_type properties;

    };
}

#include "datraw/info.inl"
