/// <copyright file="info.h" company="Visualisierungsinstitut der Universität Stuttgart">
/// Copyright © 2017 Visualisierungsinstitut der Universität Stuttgart. Alle Rechte vorbehalten.
/// </copyright>
/// <author>Christoph Müller</author>

#pragma once

#include <algorithm>
#include <cassert>
#include <cctype>
#if (!defined(__GNUC__) || (__GNUC__ >= 5))
#include <codecvt>
#endif /* (!defined(__GNUC__) || (__GNUC__ >= 5)) */
#include <fstream>
#include <stdexcept>
#include <streambuf>
#include <sstream>
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
        /// Ensure a narrow string.
        /// </summary>
        inline static const std::string& narrow_string(
                const std::string& str) {
            return str;
        }

        /// <summary>
        /// Ensure a narrow string.
        /// </summary>
        static std::string narrow_string(const std::wstring& str);

        /// <summary>
        /// Parses the given dat file.
        /// </summary>
        /// <param name="file"></param>
        /// <returns>The parsed dat file.</returns>
        static info parse(const string_type& file);

        /// <summary>
        /// Constant for the byte order of the raw data.
        /// </summary>
        /// <remarks>
        /// <para>Original documentation: Byte order of the raw-file is
        /// stored in; either LITTLE_ENDIAN (default) or BIG_ENDIAN.</para>
        /// </remarks>
        static const string_type property_byte_order;

        /// <summary>
        /// Constant for number of components each element in the data set has.
        /// </summary>
        /// <remarks>
        /// <para>Original documentation: Number N of components per tuple
        /// (int value).</para>
        /// </remarks>
        static const string_type property_components;

        /// <summary>
        /// Constant for the offset (in bytes) of the data in the raw file.
        /// </summary>
        /// <remarks>
        /// <para>Original documentation: Byte offset in the raw-file(s) where
        /// the actual data starts.</para>
        /// </remarks>
        static const string_type property_data_offset;

        /// <summary>
        /// Constant for the dimensionality of the elements in the volume.
        /// </summary>
        /// <remarks>
        /// <para>Original documentation: Dimensionality M of the grid
        /// (int value).</para>
        /// </remarks>
        static const string_type property_dimensions;

        /// <summary>
        /// Constant for the scalar data format in the raw data.
        /// </summary>
        /// <remarks>
        /// <para>Original documentation: The format (or data type) of a 
        /// single element. Currently supported are:
        /// <list type="bullet">
        /// <listheader>
        ///     <term>Constant</term>
        ///     <description>Format</description>
        /// </listheader>
        /// <item>
        ///     <term>CHAR</term>
        ///     <description>8bit signed int</description>
        /// </item>
        /// <item>
        ///     <term>UCHAR</term>
        ///     <description>8bit unsigned int</description>
        /// </item>
        /// <item>
        ///     <term>SHORT</term>
        ///     <description>16bit signed int</description>
        /// </item>
        /// <item>
        ///     <term>USHORT</term>
        ///     <description>16bit unsigned int</description>
        /// </item>
        /// <item>
        ///     <term>INT</term>
        ///     <description>32bit signed int</description>
        /// </item>
        /// <item>
        ///     <term>UINT</term>
        ///     <description>32bit unsigned int</description>
        /// </item>
        /// <item>
        ///     <term>LONG</term>
        ///     <description>64bit signed int</description>
        /// </item>
        /// <item>
        ///     <term>ULONG</term>
        ///     <description>64bit unsigned int</description>
        /// </item>
        /// <item>
        ///     <term>HALF</term>
        ///     <description>16bit float format (1 sign bit 
        /// + 5 bit exponent + 10 bit mantissa)</description>
        /// </item>
        /// <item>
        ///     <term>FLOAT</term>
        ///     <description>32bit IEEE single float format</description>
        /// </item>
        /// <item>
        ///     <term>DOUBLE</term>
        ///     <description>64bit IEEE double float format</description>
        /// </item>
        /// </list>
        /// </para>
        /// </remarks>
        static const string_type property_format;

        /// <summary>
        /// Constant for the type of the grid the data is located on.
        /// </summary>
        /// <remarks>
        /// <para>Original documentation: The type of grid the data is
        /// organized in. Currently, only the UNIFORM type is supported.
        /// <para>
        /// </remarks>
        static const string_type property_grid_type;

        /// <summary>
        /// Constant for the raw file(s).
        /// </summary>
        /// <remarks>
        /// <para>Original documentation: The name(s) of the raw-file(s).
        /// For a single raw-file this is just the file name, for multiple
        /// raw-files, forming for example a time-series, a the numbering
        /// is controlled by a format string.</para>
        /// <para>The raw data for a data set consisting of multiple timesteps
        /// can be either stored consecutively in a single file or in a separate
        /// file per time step. In the second case, the ObjectFilename must
        /// contain a conversion specification similar to the printf format
        /// string. It starts with the '%' character followed by an optional
        /// padding flag, field width, skip, and stride modifier and has to end
        /// with the conversion specifier 'd'. The padding flags('0', '-', ' ')
        /// and the minimum field width have the same meaning as in the *printf
        /// specification. The skip flag (a '+' followed by a positive decimal
        /// value) gives the enumeration of the first data file. The default
        /// skip offset is 0, thus the first time step is assumed to be stored
        /// in the file enumerated with 0. The stride (a '*' followed by a 
        /// positive decimal value) specifies the offset between two consecutive
        /// file enumerations. Example: data%03+1*2d.raw specifies the data
        /// files data001.raw, data003.raw, data005.raw, data007.raw, ...
        /// </para>
        /// </remarks>
        static const string_type property_object_file_name;

        /// <summary>
        /// TODO: no original documentation
        /// </summary>
        static const string_type property_origin;

        /// <summary>
        /// Constant for the resolution of the raw data.
        /// </summary>
        /// <remarks>
        /// <para>Original documentation: Resolution of the grid, ie number
        /// of elements in each dimension (M int values, X Y Z ...).</para>
        /// </remarks>
        static const string_type property_resolution;

        /// <summary>
        /// Constant for the thickness of the slices.
        /// </summary>
        /// <remarks>
        /// <para>Original documentation: Size of the grid cells in each 
        /// direction/dimension (M float values, dX dY dZ ...).</para>
        /// </remarks>
        static const string_type property_slice_thickness;

        /// <summary>
        /// TODO: no original documentation
        /// </summary>
        static const string_type property_tetrahedra;

        /// <summary>
        /// Constant for the number of time steps.
        /// </summary>
        /// <remarks>
        /// Number of time steps/number of raw files.
        /// </remarks>
        static const string_type property_time_steps;

        /// <summary>
        /// TODO: no original documentation
        /// </summary>
        static const string_type property_vertices;

        /// <summary>
        /// Initialises an empty instance.
        /// </summary>
        inline info(void) { }

        /// <summary>
        /// Gets the value of the well-known property named
        /// <see cref="property_byte_order" />.
        /// </summary>
        /// <returns>The byte order of the data.</returns>
        /// <exception cref="std::out_of_range">If no such property was stored
        /// in the object.</exception>
        inline datraw::endianness byte_order(void) const {
            auto v = (*this)[info::property_byte_order];
            return v.get<datraw::endianness>();
        }

        /// <summary>
        /// Performs a sanity check of the objects content.
        /// </summary>
        /// <remarks>
        /// This method will also fill in defaults as possible.
        /// </remarks>
        /// <exception cref="std::runtime_error">In case something is wrong with
        /// the content of the object.</exception>
        void check(void);

        /// <summary>
        /// Gets the value of the well-known property named
        /// <see cref="property_components" />.
        /// </summary>
        /// <returns>The number of components per voxel.</returns>
        /// <exception cref="std::out_of_range">If no such property was stored
        /// in the object.</exception>
        inline std::uint32_t components(void) const {
            auto v = (*this)[info::property_components];
            return v.get<std::uint32_t>();
        }

        /// <summary>
        /// Answer whether the object holds a property with the given name.
        /// </summary>
        /// <param name="prop">The name of the propery to search.</param>
        /// <returns><c>true</c> in case the property exists, <c>false</c>
        /// otherwise.</returns>
        inline bool contains(const string_type& prop) const {
            return (this->properties.find(prop) != this->properties.end());
        }

        /// <summary>
        /// Gets the value of the well-known property named
        /// <see cref="property_data_offset" />.
        /// </summary>
        /// <returns>The offset in bytes into the raw file where the actual
        /// data starts.</returns>
        /// <exception cref="std::out_of_range">If no such property was stored
        /// in the object.</exception>
        inline std::uint64_t data_offset(void) const {
            auto v = (*this)[info::property_data_offset];
            return v.get<std::uint64_t>();
        }

        /// <summary>
        /// Gets the value of the well-known property named
        /// <see cref="property_dimensions" />.
        /// </summary>
        /// <returns>The dimensions of the data, usually 3.</returns>
        /// <exception cref="std::out_of_range">If no such property was stored
        /// in the object.</exception>
        inline std::uint32_t dimensions(void) const {
            auto v = (*this)[info::property_dimensions];
            return v.get<std::uint32_t>();
        }

        /// <summary>
        /// Answer the size of a single element of the scalar or vector field.
        /// </summary>
        /// <remarks>
        /// This is equivalent to <see cref="record_size" />.
        /// </remarks>
        /// <returns>The size of an element or 0 in case of an error.</returns>
        size_t element_size(void) const;

        /// <summary>
        /// Gets the value of the well-known property named
        /// <see cref="property_format" />.
        /// </summary>
        /// <returns>The format of a scalar.</returns>
        /// <exception cref="std::out_of_range">If no such property was stored
        /// in the object.</exception>
        inline datraw::scalar_type format(void) const {
            auto v = (*this)[info::property_format];
            return v.get<datraw::scalar_type>();
        }


        /// <summary>
        /// Gets the value of the well-known property named
        /// <see cref="property_grid_type" />.
        /// </summary>
        /// <returns>The type of grid used for the data.</returns>
        /// <exception cref="std::out_of_range">If no such property was stored
        /// in the object.</exception>
        inline datraw::grid_type grid_type(void) const {
            auto v = (*this)[info::property_grid_type];
            return v.get<datraw::grid_type>();
        }

        /// <summary>
        /// Gets the value of the well-known property named
        /// <see cref="property_object_file_name" />.
        /// </summary>
        /// <returns>The path to the raw file.</returns>
        /// <exception cref="std::out_of_range">If no such property was stored
        /// in the object.</exception>
        inline string_type object_file_name(void) const {
            auto v = (*this)[info::property_object_file_name];
            return v.get<string_type>();
        }

        /// <summary>
        /// Gets the value of the well-known property named
        /// <see cref="property_origin" />.
        /// </summary>
        /// <returns>TODO.</returns>
        /// <exception cref="std::out_of_range">If no such property was stored
        /// in the object.</exception>
        inline const std::vector<std::uint32_t>& origin(void) const {
            auto& v = (*this)[info::property_origin];
            return v.get<std::vector<std::uint32_t>>();
        }

        /// <summary>
        /// Answer the names of all properties stored in the object.
        /// </summary>
        /// <remarks>
        /// An exemplary use of this method would be:
        /// <code>
        /// std::vector<std::string> props;
        /// props.reserve(info.size());
        /// info.property_names(std::back_inserter(props));
        /// </code>
        /// </remarks>
        /// <tparam name="I">An output iterator type for
        /// <see cref="string_type" />.</tparam>
        /// <param name="oit">An output iterator, which is an inserter or can at
        /// least be incremented <see cref="size" /> times.</param>
        template<class I> void property_names(I oit) const;

        /// <summary>
        /// Answer the size (in byte) of one data record of the data described
        /// in this object.
        /// </summary>
        /// <remarks>
        /// This is the legacy-named version of <see cref="element_size" />.
        /// </remarks>
        /// <returns>The size of an element or 0 in case of an error.</returns>
        inline size_t record_size(void) const {
            return this->element_size();
        }

        /// <summary>
        /// Gets the value of the well-known property named
        /// <see cref="property_resolution" />.
        /// </summary>
        /// <returns>The number of voxels on each axis.</returns>
        /// <exception cref="std::out_of_range">If no such property was stored
        /// in the object.</exception>
        inline std::vector<std::uint32_t> resolution(void) const {
            auto v = (*this)[info::property_resolution];
            return v.get<std::vector<std::uint32_t>>();
        }

        /// <summary>
        /// Answer the size of a single scalar in bytes.
        /// </summary>
        /// <returns>The size of a single scalar or 0 in case of an error.
        /// </returns>
        size_t scalar_size(void) const;

        /// <summary>
        /// Answer the number of properties stored in the object.
        /// </summary>
        /// <returns>The number of properties.</returns>
        inline size_t size(void) const {
            return this->properties.size();
        }

        /// <summary>
        /// Gets the value of the well-known property named
        /// <see cref="property_slice_thickness" />.
        /// </summary>
        /// <returns>The distance between the voxels in each axis.</returns>
        /// <exception cref="std::out_of_range">If no such property was stored
        /// in the object.</exception>
        inline std::vector<float> slice_thickness(void) const {
            auto v = (*this)[info::property_slice_thickness];
            return v.get<std::vector<float>>();
        }

        /// <summary>
        /// Gets the value of the well-known property named
        /// <see cref="property_tetrahedra" />.
        /// </summary>
        /// <returns>TODO.</returns>
        /// <exception cref="std::out_of_range">If no such property was stored
        /// in the object.</exception>
        inline std::uint64_t tetrahedra(void) const {
            auto v = (*this)[info::property_tetrahedra];
            return v.get<std::uint64_t>();
        }

        /// <summary>
        /// Gets the value of the well-known property named
        /// <see cref="property_time_steps" />.
        /// </summary>
        /// <returns>The number of time steps (individual raw files) the data
        /// set comprises.</returns>
        /// <exception cref="std::out_of_range">If no such property was stored
        /// in the object.</exception>
        inline std::uint64_t time_steps(void) const {
            auto v = (*this)[info::property_time_steps];
            return v.get<std::uint64_t>();
        }

        /// <summary>
        /// Gets the value of the well-known property named
        /// <see cref="property_vertices" />.
        /// </summary>
        /// <returns>TODO.</returns>
        /// <exception cref="std::out_of_range">If no such property was stored
        /// in the object.</exception>
        inline std::uint64_t vertices(void) const {
            auto v = (*this)[info::property_vertices];
            return v.get<std::uint64_t>();
        }

        /// <summary>
        /// Retrieves, if available, the property with the given name.
        /// </summary>
        /// <param name="prop">The name of the property to retrieve, preferably
        /// one of the constants in this class</param>
        /// <returns>The value of the given property</returns>
        /// <exception cref="std::out_of_range">If the property was not found.
        /// </exception>
        variant_type& operator [](const string_type& prop);

        /// <summary>
        /// Retrieves, if available, the property with the given name.
        /// </summary>
        /// <param name="prop">The name of the property to retrieve, preferably
        /// one of the constants in this class</param>
        /// <returns>The value of the given property</returns>
        /// <exception cref="std::out_of_range">If the property was not found.
        /// </exception>
        const variant_type& operator [](const string_type& prop) const;

    private:

        typedef detail::variant_type_list_t</*datraw::variant_type::int8,*/
            datraw::variant_type::int16, datraw::variant_type::int32,
            datraw::variant_type::int64, /*datraw::variant_type::uint8,*/
            datraw::variant_type::uint16, datraw::variant_type::uint32,
            datraw::variant_type::uint64, datraw::variant_type::float32,
            datraw::variant_type::float64> parsable_scalars_t;
        // Note: 1 byte cannot be parsed from wchar_t string.

        typedef detail::variant_type_list_t</*datraw::variant_type::vec_int8,*/
            datraw::variant_type::vec_int16, datraw::variant_type::vec_int32,
            datraw::variant_type::vec_int64, /*datraw::variant_type::vec_uint8,*/
            datraw::variant_type::vec_uint16, datraw::variant_type::vec_uint32,
            datraw::variant_type::vec_uint64, datraw::variant_type::vec_float32,
            datraw::variant_type::vec_float64> parsable_vectors_t;

        /// <summary>
        /// Find, if any, the array entry for the given tag.
        /// </summary>
        template<class T, size_t N>
        static const T *find_tag(const T (&tags)[N], const string_type& tag);

        /// <summary>
        /// Parse the given string into a variant of the given type.
        /// </summary>
        /// <remarks>
        /// This is the general parsing method for property types. It will
        /// called specialised ones as needed.
        /// </remarks>
        template<datraw::variant_type T, datraw::variant_type... Ts>
        static variant_type parse(detail::variant_type_list_t<T, Ts...>,
            const string_type& str, const datraw::variant_type type);

        /// <summary>
        /// Called if parsing a string into a variant failed.
        /// </summary>
        static variant_type parse(detail::variant_type_list_t<>,
            const string_type& str, const datraw::variant_type type);

        /// <summary>
        /// Parse one of <tparamref name="T" /> from <paramref name="str" />.
        /// </summary>
        template<class T> static T parse(const string_type& str);

        /// <summary>
        /// Parse one of <see cref="strstrm_parsables_t" /> from
        /// <paramref name="str" />.
        /// </summary>
        template<datraw::variant_type T>
        static inline typename variant_type::forward_traits<T>::value_type parse(
                const string_type& str) {
            return info::parse<typename variant_type::forward_traits<T>::value_type>(str);
        }

        /// <summary>
        /// Parses <see cref="endianness" /> from <paramref name="str" />.
        /// </summary>
        static variant_type parse_endianness(const string_type& str);

        /// <summary>
        /// Parses <see cref="grid_type" /> from <paramref name="str" />.
        /// </summary>
        static variant_type parse_grid_type(const string_type& str);

        /// <summary>
        /// Parses <see cref="scalar_type" /> from <paramref name="str" />.
        /// </summary>
        static variant_type parse_scalar_type(const string_type& str);

        /// <summary>
        /// Parse the given string into a vector of the given type.
        /// </summary>
        template<datraw::variant_type T, datraw::variant_type... Ts>
        static variant_type parse_vec(detail::variant_type_list_t<T, Ts...>,
            const string_type& str, const datraw::variant_type type);

        /// <summary>
        /// Called if parsing a string into vector variant failed.
        /// </summary>
        static variant_type parse_vec(detail::variant_type_list_t<>,
            const string_type& str, const datraw::variant_type type);

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

        /// <summary>
        /// Answer the endianness of the system we are running on.
        /// </summary>
        static datraw::endianness sys_endianness(void);

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
        /// Answer an upper case-version of <paramref name="str" />.
        /// </summary>
        static inline string_type to_upper(string_type str) {
            std::transform(str.begin(), str.end(), str.begin(), ::toupper);
            return str;
        }

        /// <summary>
        /// Remove all leading white-space characters from
        /// <paramref name="str" />.
        /// </summary>
        /// <param name="str"></param>
        /// <returns></returns>
        static inline string_type trim_left(const string_type& str) {
            auto end = str.cend();
            auto begin = std::find_if(str.cbegin(), end,
                std::not1(std::ptr_fun<int, int>(std::isspace)));
            return string_type(begin, str.cend());
        }

        /// <summary>
        /// Remove all trailing white-space characters from
        /// <paramref name="str" />.
        /// </summary>
        /// <param name="str"></param>
        /// <returns></returns>
        static inline string_type trim_right(const string_type& str) {
            auto end = std::find_if(str.crbegin(), str.crend(),
                std::not1(std::ptr_fun<int, int>(std::isspace)));
            return string_type(str.cbegin(), end.base());
        }

        /// <summary>
        /// Remove all leading and trailing white-space characters from
        /// <paramref name="str" />.
        /// </summary>
        /// <param name="str"></param>
        /// <returns></returns>
        static inline string_type trim(const string_type& str) {
            return trim_left(trim_right(str));
        }

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
