/// <copyright file="info.inl" company="Visualisierungsinstitut der Universität Stuttgart">
/// Copyright © 2017 Visualisierungsinstitut der Universität Stuttgart. Alle Rechte vorbehalten.
/// </copyright>
/// <author>Christoph Müller</author>


/*
 * datraw::info<C>::parse
 */
template<class C>
datraw::info<C> datraw::info<C>::parse(const string_type& file) {
    static const string_type EMPTY_STRING(DATRAW_TPL_LITERAL(C, ""));
    static const struct {
        string_type Tag;
        endianness Value;
    } ENDIANNESSES[] = {
        { DATRAW_TPL_LITERAL(C, "BIG_ENDIAN"), endianness::big },
        { DATRAW_TPL_LITERAL(C, "LITTLE_ENDIAN"), endianness::little },
    };
    static const struct {
        string_type Tag;
        grid_type Value;
    } GRID_TYPES[] = {
        { DATRAW_TPL_LITERAL(C, "EQUIDISTANT"), grid_type::cartesian },
        { DATRAW_TPL_LITERAL(C, "CARTESIAN"), grid_type::cartesian },
        { DATRAW_TPL_LITERAL(C, "UNIFORM"), grid_type::cartesian },
        { DATRAW_TPL_LITERAL(C, "RECTILINEAR"), grid_type::rectilinear },
        { DATRAW_TPL_LITERAL(C, "TETRAHEDRA"), grid_type::tetrahedral }
    };
    static const struct {
        string_type Name;
        variant_type DefaultValue;
    } KNOWN_PROPERTIES[] = {
        { DATRAW_TPL_LITERAL(C, "OBJECTFILENAME"), variant_type() },
        { DATRAW_TPL_LITERAL(C, "FORMAT"), variant_type(scalar_type::raw) },
        { DATRAW_TPL_LITERAL(C, "GRIDTYPE"), variant_type(grid_type::cartesian) },
        { DATRAW_TPL_LITERAL(C, "COMPONENTS"), variant_type(1) },
        { DATRAW_TPL_LITERAL(C, "DIMENSIONS"), variant_type(3) },
        { DATRAW_TPL_LITERAL(C, "TIMESTEPS"), variant_type(1) },
        { DATRAW_TPL_LITERAL(C, "BYTEORDER"), variant_type(endianness::little) },
        { DATRAW_TPL_LITERAL(C, "DATAOFFSET"), variant_type(0) },
        { DATRAW_TPL_LITERAL(C, "RESOLUTION"), variant_type(std::vector<std::uint32_t>()) },    // if (!(info->resolution = (int*)malloc(info->dimensions * sizeof(int)))) {
        { DATRAW_TPL_LITERAL(C, "SLICETHICKNESS"), variant_type(std::vector<std::uint32_t>()) },
        { DATRAW_TPL_LITERAL(C, "ORIGIN"), variant_type(std::vector<std::uint32_t>()) },
        { DATRAW_TPL_LITERAL(C, "VERTICES"), variant_type(0) },
        { DATRAW_TPL_LITERAL(C, "TETRAHEDRA"), variant_type(0) },
    };
    static const struct {
        string_type Tag;
        scalar_type Value;
    } SCALAR_TYPES[] = {
        { DATRAW_TPL_LITERAL(C, "CHAR"), scalar_type::int8 },
        { DATRAW_TPL_LITERAL(C, "UCHAR"), scalar_type::uint8 },
        { DATRAW_TPL_LITERAL(C, "SHORT"), scalar_type::int16 },
        { DATRAW_TPL_LITERAL(C, "USHORT"), scalar_type::uint16 },
        { DATRAW_TPL_LITERAL(C, "INT"), scalar_type::int32 },
        { DATRAW_TPL_LITERAL(C, "UINT"), scalar_type::uint32 },
        { DATRAW_TPL_LITERAL(C, "LONG"), scalar_type::int64 },
        { DATRAW_TPL_LITERAL(C, "ULONG"), scalar_type::uint64 },
        { DATRAW_TPL_LITERAL(C, "HALF"), scalar_type::float16 },
        { DATRAW_TPL_LITERAL(C, "FLOAT"), scalar_type::float32 },
        { DATRAW_TPL_LITERAL(C, "DOUBLE"), scalar_type::float64 }
    };

    string_type content;
    datraw::info<C> retval;

    /* Read the whole dat file specified by the user. */
    {
        std::basic_ifstream<char_type> stream(file);

        stream.seekg(0, std::ios::end);
        content.reserve(stream.tellg());
        stream.seekg(0, std::ios::beg);
        content.assign((std::istreambuf_iterator<char_type>(stream)),
            std::istreambuf_iterator<char_type>());
    }

    /* Parse the dat file. */
    auto lines = info::tokenise(content.cbegin(), content.cend(),
        DATRAW_TPL_LITERAL(C, '\r'), DATRAW_TPL_LITERAL(C, '\n'));
    assert(lines.size() <= static_cast<size_t>(INT_MAX));
    for (int i = 0; i < static_cast<int>(lines.size()) - 1; ++i) {
        auto b = info::skip_spaces(lines[i], lines[i + 1]);
        auto e = lines[i + 1];

        auto colon = std::find(b, e, DATRAW_TPL_LITERAL(C, ':'));
        if (colon != e) {
            auto key = string_type(b, colon);
            auto value = info::skip_spaces(++colon, e);

            // TODO: process known content
            /*
             OBJECTFILENAME  - the name(s) of the raw-file(s)                     *
 *                       For a single raw-file this is just the file name,  *
 *                       for multiple raw-files, forming for example a      *
 *                       time-series, a the numbering is controlled by a    *
 *                       format string. More details below.                 *
 *     FORMAT          - The format (or data type) of a single element.     *
 *                       Currently supported are:                           *
 *                        - CHAR (8bit signed int)                          *
 *                        - UCHAR (8bit unsigned int)                       *
 *                        - SHORT (16bit signed int)                        *
 *                        - USHORT (16bit unsigned int)                     *
 *                        - INT (32bit signed int)                          *
 *                        - UINT (32bit unsigned int)                       *
 *                        - LONG (64bit signed int)                         *
 *                        - ULONG (64bit unsigned int)                      *
 *                        - HALF (16bit float format)                       *
 *                          (1 sign bit + 5bit exponent + 10b mantissa)     *
 *                        - FLOAT (32bit IEEE single float format)          *
 *                        - LONG (64bit IEEE double float format)           *
 *     GRIDTYPE        - The type of grid the data is organized in.         *
 *                       Currently only the UNIFORM type is supported.      *
 *     COMPONENTS      - number N of components per tupel (int value)       *
 *     DIMENSIONS      - dimensionality M of the grid (int value)           *
 *     TIMESTEPS       - number of time steps/ number of raw files          *
 *     BYTEORDER       - byte order of the raw-file is stored in; either    *
 *                       LITTLE_ENDIAN (default) or BIG_ENDIAN              *
 *     DATAOFFSET      - byte offset in the ra-file(s) where the actual     *
 *                       data starts                                        *
 *     RESOLUTION      - resolution of the grid, i.e. number of elements in *
 *                       each dimension. (M int values, X Y Z ...)          *
 *     SLICETHICKNESS  - size of the grid cells in each direction/dimension *
 *                       (M float values, (dX dY dZ ...)         
            */

            retval.properties[key] = string_type(value, e);
        }
    }




    return retval;
}


/*
 * datraw::info<C>::skip_spaces
 */
template<class C>
template<class I> I datraw::info<C>::skip_spaces(I it, I end) {
    while ((it != end) && std::isspace(*it)) {
        ++it;
    }
    return it;
}


/*
 * datraw::info<C>::tokenise
 */
template<class C>
template<class I, class P>
std::vector<I> datraw::info<C>::tokenise(I begin, I end, P predicate) {
    auto cur = begin;
    std::vector<I> retval;

    while (cur != end) {
        retval.push_back(cur);
        cur = std::find_if(++cur, end, predicate);
    }

    retval.push_back(end);

    return retval;
}


/*
 * datraw::info<C>::tokenise
 */
template<class C>
template<class I, class... D>
std::vector<I> datraw::info<C>::tokenise(I begin, I end, D... delimiters) {
    typedef typename I::value_type char_type;

    std::vector<char_type> delims = { delimiters... };

    return info::tokenise(begin, end, [&delims](char_type c) {
        auto e = delims.end();
        return (std::find(delims.begin(), e, c) != e);
    });
}


/*
 * datraw::info<C>::sys_endianness
 */
template<class C>
datraw::endianness datraw::info<C>::sys_endianness(void) {
    static const union {
        std::uint32_t u;
        std::uint8_t b[4];
    } BYTES = { 0x01020304 };
    
    switch (BYTES.b[0]) {
        case 0x01: return endianness::big;
        case 0x04: return endianness::little;
        default: throw std::runtime_error("Unknown endianness. This should "
            "never happen!");
    }
}
