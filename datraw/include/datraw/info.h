/// <copyright file="info.h" company="Visualisierungsinstitut der Universität Stuttgart">
/// Copyright © 2017 Visualisierungsinstitut der Universität Stuttgart. Alle Rechte vorbehalten.
/// </copyright>
/// <author>Christoph Müller</author>

#pragma once

#include <string>
#include <unordered_map>
#include <vector>

#include "datraw/api.h"
#include "datraw/types.h"
#include "datraw/variant.h"


namespace datraw {

    /// <summary>
    /// Holds the content of the dat file, which describes the raw data.
    /// </summary>
    class info {

    public:

        /// <summary>
        /// The type used to represent a single character in a string.
        /// </summary>
        typedef std::string::value_type char_type;

        /// <summary>
        /// The type used to represent a string.
        /// </summary>
        typedef std::string string_type;

        /// <summary>
        /// Split a range of characters into tokens defined by the list of
        /// delimiters <paramref name="delims" />.
        /// </summary>
        /// <tparam name="D">The type of the delimiter list.</tparam>
        /// <param name="begin">The begin of the string.</param>
        /// <param name="end">The end of the string.</param>
        /// <param name="removeEmpty">If <c>true</c>, empty tokens will be
        /// ignored in the result.</param>
        /// <param name="delims">The list of delimiters.</param>
        /// <returns>Iterators representing the borders of the tokens.</returns>
        template<class... D> std::vector<string_type::const_iterator> tokenise(
            string_type::const_iterator begin, string_type::const_iterator end,
            const bool removeEmpty, D... delims);

        /// <summary>
        /// Split <paramref name="str" /> into tokens defined by the list of
        /// delimiters <paramref name="delims" />.
        /// </summary>
        /// <tparam name="D">The type of the delimiter list.</tparam>
        /// <param name="str">The string to be split.</param>
        /// <param name="removeEmpty">If <c>true</c>, empty tokens will be
        /// ignored in the result.</param>
        /// <param name="delims">The list of delimiters.</param>
        /// <returns>Iterators representing the borders of the tokens.</returns>
        template<class... D>
        inline std::vector<string_type::const_iterator> tokenise(
                const string_type& str, const bool removeEmpty, D... delims) {
            return info::tokenise(str.begin(), str.end(), removeEmpty,
                std::forward<D>(delims)...);
        }

        /// <summary>
        /// Parses the given dat file.
        /// </summary>
        /// <param name="file"></param>
        /// <returns></returns>
        info parse(const string_type& file);

        /// <summary>
        /// Initialises an empty instance.
        /// </summary>
        inline info(void) { }



    private:

        /// <summary>
        /// A collection type storing all the properties parsed from the dat
        /// file.
        /// </summary>
        typedef std::unordered_map<std::string, variant> property_list_type;

        /// <summary>
        /// The properties stored in the dat file.
        /// </summary>
        property_list_type properties;

    };
}

#include "datraw/info.inl"
