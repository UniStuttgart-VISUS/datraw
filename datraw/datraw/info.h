/// <copyright file="info.h" company="Visualisierungsinstitut der Universität Stuttgart">
/// Copyright © 2017 Visualisierungsinstitut der Universität Stuttgart. Alle Rechte vorbehalten.
/// </copyright>
/// <author>Christoph Müller</author>

#pragma once

#include <cassert>
#include <cinttypes>
#include <climits>
#include <cstring>
#include <fstream>
#include <regex>
#include <streambuf>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

#include "datraw/grid_type.h"
#include "datraw/parse.h"
#include "datraw/string.h"
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
        /// Determines whether the given string is a multi-file description for
        /// a time series.
        /// </summary>
        /// <param name="str">The string to be tested.</param>
        /// <returns><c>true</c> if the string is a multi-file description,
        /// <c>false</c> otherwise.</returns>
        static bool is_multi_file_description(const string_type& str);

        /// <summary>
        /// Parses the given dat file and checks its content.
        /// </summary>
        /// <param name="file">Path to the dat file.</param>
        /// <returns>The parsed dat file.</returns>
        static info load(const string_type& file);

        /// <summary>
        /// Parses the given dat file content and checks it.
        /// </summary>
        /// <param name="content">The content of a dat file.</param>
        /// <param name="file">The path to the dat file if relative paths
        /// to raw files should be resolved.</param>
        /// <returns>The parsed dat file.</returns>
        static info parse(const string_type& content,
            const string_type& file = DATRAW_TPL_LITERAL(C, ""));

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
        /// TODO: no original documentation; I guess this was never implemented.
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
            return v.template get<datraw::endianness>();
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
            return v.template get<std::uint32_t>();
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
            return v.template get<std::uint64_t>();
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
            return v.template get<std::uint32_t>();
        }

        /// <summary>
        /// Answer the size of a single element of the scalar or vector field.
        /// </summary>
        /// <remarks>
        /// This is equivalent to <see cref="record_size" />.
        /// </remarks>
        /// <returns>The size of an element or 0 in case of an error.</returns>
        std::size_t element_size(void) const;

        /// <summary>
        /// Evaluate a path of a raw file as if it came from the dat file
        /// represented by this object.
        /// </summary>
        /// <remarks>
        /// A dat file can contain relative paths to a raw file, which are
        /// relative wrt the location of the dat file. This method checks
        /// whether the path is absolute or relative, and appends the path to
        /// the dat file such that the raw file can be opened (given the working
        /// directory has not been changed between parsing the dat file and
        /// calling this method.
        /// </remarks>
        /// <param name="path">The path to a raw file.</param>
        /// <returns>The path which should be opened to access the raw data.
        /// </returns>
        string_type evaluate_path(const string_type& path) const;

        /// <summary>
        /// Gets the value of the well-known property named
        /// <see cref="property_format" />.
        /// </summary>
        /// <returns>The format of a scalar.</returns>
        /// <exception cref="std::out_of_range">If no such property was stored
        /// in the object.</exception>
        inline datraw::scalar_type format(void) const {
            auto v = (*this)[info::property_format];
            return v.template get<datraw::scalar_type>();
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
            return v.template get<datraw::grid_type>();
        }

        /// <summary>
        /// Given that the dat file holds a pattern for a time series, evaluate
        /// this expression and return the file name of the
        /// <paramref name="timeStep" />th raw file.
        /// </summary>
        /// <param name="timeStep">The time step to retrieve the name of the
        /// raw file for.</param>
        /// <returns>The name of the raw file.</returns>
        string_type multi_file_name(const std::uint64_t timeStep) const;

        /// <summary>
        /// Gets the value of the well-known property named
        /// <see cref="property_object_file_name" />.
        /// </summary>
        /// <returns>The path to the raw file.</returns>
        /// <exception cref="std::out_of_range">If no such property was stored
        /// in the object.</exception>
        inline string_type object_file_name(void) const {
            auto v = (*this)[info::property_object_file_name];
            return v.template get<string_type>();
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
            return v.template get<std::vector<std::uint32_t>>();
        }

        /// <summary>
        /// Answer the path of the dat file this <see cref="info" /> was parsed
        /// from.
        /// </summary>
        /// <remarks>
        /// This might be an empty string if the information was parsed from an
        /// in-memory string. In this case, relative paths to raw files cannot
        /// be resolved correctly unless the working directory matches the
        /// location of the raw files.
        /// </remarks>
        /// <returns>The path to the dat file.</returns>
        inline const string_type& path(void) const {
            return this->datPath;
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
        /// Answer the size (in byte) of one data record of the data (voxel)
        /// described in this object.
        /// </summary>
        /// <remarks>
        /// This is the legacy-named version of <see cref="element_size" />.
        /// </remarks>
        /// <returns>The size of an element or 0 in case of an error.</returns>
        inline std::size_t record_size(void) const {
            return this->element_size();
        }

        /// <summary>
        /// Answer whether the bytes in the raw data need to be swapped, ie
        /// whether the endianness of the data is not the same as the endianness
        /// of the platform.
        /// </summary>
        /// <returns><c>true</c> if the byte order needs to be swapped,
        /// <c>false</c> otherwise.</returns>
        inline bool requires_byte_swap(void) const {
            return (this->byte_order() != info::sys_endianness());
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
            return v.template get<std::vector<std::uint32_t>>();
        }

        /// <summary>
        /// Answer the size of a single scalar in bytes.
        /// </summary>
        /// <returns>The size of a single scalar or 0 in case of an error.
        /// </returns>
        std::size_t scalar_size(void) const;

        /// <summary>
        /// Answer the number of properties stored in the object.
        /// </summary>
        /// <returns>The number of properties.</returns>
        inline std::size_t size(void) const {
            return this->properties.size();
        }

        /// <summary>
        /// Gets the value of the well-known property named
        /// <see cref="property_slice_thickness" />.
        /// </summary>
        /// <remarks>
        /// <para>This property is only valid for Cartesian grids.</para>
        /// </remarks>
        /// <returns>The distance between the voxels in each axis.</returns>
        /// <exception cref="std::out_of_range">If no such property was stored
        /// in the object.</exception>
        inline std::vector<float> slice_thickness(void) const {
            auto v = (*this)[info::property_slice_thickness];
            return v.template get<std::vector<float>>();
        }

        /// <summary>
        /// In case of <see cref="grid_type" /> being
        /// <see cref="datraw::grid_type::rectilinear" />, answer the distances
        /// between the slices on the specified axis, which is indexed starting
        /// at 0.
        /// </summary>
        /// <remarks>
        /// <para>This property is only valid for rectilinear grids.</para>
        /// </remarks>
        /// <param name="axis">The (zero-indexed) number of the axis to retrieve
        /// the grid positions of.</param>
        /// <returns>The distance between the slices in the specified axis.
        /// </returns>
        /// <exception cref="std::out_of_range">If no such property was stored
        /// in the object.</exception>
        inline std::vector<float> slice_thickness(
                const std::uint32_t axis) const {
            auto v = (*this)[info::format_slice_thickness(axis)];
            return v.template get<std::vector<float>>();
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
            return v.template get<std::uint64_t>();
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
            return v.template get<std::uint64_t>();
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
            return v.template get<std::uint64_t>();
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

        typedef detail::variant_type_list_t<datraw::variant_type::int8,
            datraw::variant_type::int16, datraw::variant_type::int32,
            datraw::variant_type::int64, datraw::variant_type::uint8,
            datraw::variant_type::uint16, datraw::variant_type::uint32,
            datraw::variant_type::uint64, datraw::variant_type::float32,
            datraw::variant_type::float64, datraw::variant_type::endianness,
            datraw::variant_type::grid_type, datraw::variant_type::scalar_type>
            parsable_scalars_t;

        typedef detail::variant_type_list_t<datraw::variant_type::vec_int8,
            datraw::variant_type::vec_int16, datraw::variant_type::vec_int32,
            datraw::variant_type::vec_int64, datraw::variant_type::vec_uint8,
            datraw::variant_type::vec_uint16, datraw::variant_type::vec_uint32,
            datraw::variant_type::vec_uint64, datraw::variant_type::vec_float32,
            datraw::variant_type::vec_float64> parsable_vectors_t;

        /// <summary>
        /// Format the slice thickness property for rectilinear grids.
        /// </summary>
        static string_type format_slice_thickness(const std::uint32_t axis);

        /// <summary>
        /// Answer whether <paramref name="c" /> is a valid directory separator.
        /// </summary>
        static inline bool is_dir_sep(const char_type c) {
#ifdef _WIN32
            return ((c == DATRAW_TPL_LITERAL(C, '\\'))
                || (c == DATRAW_TPL_LITERAL(C, '/')));
#else /* _WIN32 */
            return (c == DATRAW_TPL_LITERAL(C, '/'));
#endif /* _WIN32 */
        }

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
        /// Parses the given multi-file description, returns a template for the
        /// actual file names and writes the parameters to the given
        /// out-parameters.
        /// </summary>
        static string_type parse_multi_file_description(const string_type& str,
            int& width, int& skip, int& stride);

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
        /// A collection type storing all the properties parsed from the dat
        /// file.
        /// </summary>
        typedef std::unordered_map<string_type, variant_type> property_list_type;

        /// <summary>
        /// The path to the dat file itself.
        /// </summary>
        string_type datPath;

        /// <summary>
        /// The properties stored in the dat file.
        /// </summary>
        property_list_type properties;

    };
}

#include "datraw/info.inl"
