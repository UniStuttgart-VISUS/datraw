/// <copyright file="info.inl" company="Visualisierungsinstitut der Universität Stuttgart">
/// Copyright © 2017 Visualisierungsinstitut der Universität Stuttgart. Alle Rechte vorbehalten.
/// </copyright>
/// <author>Christoph Müller</author>



/*
 * datraw::info<C>::narrow_string
 */
template<class C>
std::string datraw::info<C>::narrow_string(const std::wstring& str) {
#if (!defined(__GNUC__) || (__GNUC__ >= 5))
    static std::wstring_convert<std::codecvt_utf8<wchar_t>> cvt;
    return cvt.to_bytes(str);
#else /* (!defined(__GNUC__) || (__GNUC__ >= 5)) */
    std::string retval;
    retval.reserve(str.size());
    for (auto c : str) {
        retval.push_back(c);
    }
    return retval;
#endif /* (!defined(__GNUC__) || (__GNUC__ >= 5)) */
}


/*
 * datraw::info<C>::parse
 */
template<class C>
datraw::info<C> datraw::info<C>::parse(const string_type& file) {
    static const string_type EMPTY_STRING(DATRAW_TPL_LITERAL(C, ""));
    static const struct {
        string_type Tag;
        datraw::variant_type Type;
    } KNOWN_PROPERTIES[] = {
        { info::property_byte_order, datraw::variant_type::endianness },
        { info::property_components, datraw::variant_type::uint32 },
        { info::property_data_offset, datraw::variant_type::uint64 },
        { info::property_dimensions, datraw::variant_type::uint32 },
        { info::property_format,  datraw::variant_type::scalar_type },
        { info::property_grid_type, datraw::variant_type::grid_type },
        { info::property_object_file_name, variant_type::reverse_traits<string_type>::type },
        { info::property_origin, datraw::variant_type::vec_uint32 },
        { info::property_resolution, datraw::variant_type::vec_uint32  },    // if (!(info->resolution = (int*)malloc(info->dimensions * sizeof(int)))) {
        { info::property_slice_thickness, datraw::variant_type::vec_uint32  },
        { info::property_tetrahedra, datraw::variant_type::uint64 },
        { info::property_time_steps, datraw::variant_type::uint32 },
        { info::property_vertices, datraw::variant_type::uint64 },
    };


    string_type content;
    datraw::info<C> retval;

    /* Read the whole dat file specified by the user. */
    {
        std::basic_ifstream<char_type> stream(file);

        stream.seekg(0, std::ios::end);
        content.reserve(static_cast<size_t>(stream.tellg()));
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

        if ((b == e) || (*b == DATRAW_TPL_LITERAL(C, '#'))) {
            // This is an empty or comment line, skip it.
            continue;
        }

        auto colon = std::find(b, e, DATRAW_TPL_LITERAL(C, ':'));
        if (colon != e) {
            // Line is not empty, not a comment line and contains a valid
            // property name, which ends with a colon.
            auto key = string_type(b, colon);
            auto ucKey = info::to_upper(key);
            auto value = info::skip_spaces(++colon, e);

            /* Check for duplicates. */
            {
                auto duplicate = retval.properties.find(key);
                if (duplicate == retval.properties.end()) {
                    duplicate = retval.properties.find(ucKey);
                }
                if (duplicate != retval.properties.end()) {
                    std::stringstream msg;
                    msg << "Duplicate property \"" << info::narrow_string(key)
                        << "\" found at line " << (i + 1) << "." << std::ends;
                    throw std::runtime_error(msg.str());
                }
            }

            /* Check whether we have a known or a custom property. */
            auto kp = info::find_tag(KNOWN_PROPERTIES, ucKey);
            if (kp != nullptr) {
                // This is one of the known properties which we might perform
                // special handling for.
                key = ucKey;
                retval.properties[key] = info::parse(parsable_scalars_t(),
                    string_type(value, e), kp->Type);

            } else {
                // This is a user-defined property. Store it as string.
                retval.properties[key] = string_type(value, e);
            }

        } else {
            // If there is no colon, we have a syntax error.
            std::stringstream msg;
            msg << "Syntax error in line " << (i + 1) << ": \""
                << info::narrow_string(string_type(b, e)) << "\"."
                << std::ends;
            throw std::runtime_error(msg.str());
        } /* end if (colon != e) */
    }

    return retval;
}


/*
 * datraw::info<C>::property_byte_order
 */
template<class C>
const typename datraw::info<C>::string_type datraw::info<C>::property_byte_order(
    DATRAW_TPL_LITERAL(C, "BYTEORDER"));


/*
 * datraw::info<C>::property_components
 */
template<class C>
const typename datraw::info<C>::string_type datraw::info<C>::property_components(
    DATRAW_TPL_LITERAL(C, "COMPONENTS"));


/*
 * datraw::info<C>::property_data_offset
 */
template<class C>
const typename datraw::info<C>::string_type datraw::info<C>::property_data_offset(
    DATRAW_TPL_LITERAL(C, "DATAOFFSET"));


/*
 * datraw::info<C>::property_dimensions
 */
template<class C>
const typename datraw::info<C>::string_type datraw::info<C>::property_dimensions(
    DATRAW_TPL_LITERAL(C, "DIMENSIONS"));


/*
 * datraw::info<C>::property_format
 */
template<class C>
const typename datraw::info<C>::string_type datraw::info<C>::property_format(
    DATRAW_TPL_LITERAL(C, "FORMAT"));


/*
 * datraw::info<C>::property_grid_type
 */
template<class C>
const typename datraw::info<C>::string_type datraw::info<C>::property_grid_type(
    DATRAW_TPL_LITERAL(C, "GRIDTYPE"));


/*
 * datraw::info<C>::property_object_file_name
 */
template<class C>
const typename datraw::info<C>::string_type datraw::info<C>::property_object_file_name(
    DATRAW_TPL_LITERAL(C, "OBJECTFILENAME"));


/*
 * datraw::info<C>::property_origin
 */
template<class C>
const typename datraw::info<C>::string_type datraw::info<C>::property_origin(
    DATRAW_TPL_LITERAL(C, "ORIGIN"));


/*
 * datraw::info<C>::property_resolution
 */
template<class C>
const typename datraw::info<C>::string_type datraw::info<C>::property_resolution(
    DATRAW_TPL_LITERAL(C, "RESOLUTION"));


/*
 * datraw::info<C>::property_slice_thickness
 */
template<class C>
const typename datraw::info<C>::string_type datraw::info<C>::property_slice_thickness(
    DATRAW_TPL_LITERAL(C, "SLICETHICKNESS"));


/*
 * datraw::info<C>::property_tetrahedra
 */
template<class C>
const typename datraw::info<C>::string_type datraw::info<C>::property_tetrahedra(
    DATRAW_TPL_LITERAL(C, "TETRAHEDRA"));


/*
 * datraw::info<C>::property_time_steps
 */
template<class C>
const typename datraw::info<C>::string_type datraw::info<C>::property_time_steps(
    DATRAW_TPL_LITERAL(C, "TIMESTEPS"));


/*
 * datraw::info<C>::property_vertices
 */
template<class C>
const typename datraw::info<C>::string_type datraw::info<C>::property_vertices(
    DATRAW_TPL_LITERAL(C, "VERTICES"));


/*
 * datraw::info<C>::operator []
 */
template<class C>
typename datraw::info<C>::variant_type& datraw::info<C>::operator [](
        const string_type& prop) {
    auto it = this->properties.find(prop);
    if (it == this->properties.end()) {
        throw std::out_of_range("Could not find property in datraw info.");
    }
    return *it;
}


/*
 * datraw::info<C>::operator []
 */
template<class C>
const typename datraw::info<C>::variant_type& datraw::info<C>::operator [](
        const string_type& prop) const {
    auto it = this->properties.find(prop);
    if (it == this->properties.end()) {
        throw std::out_of_range("Could not find property in datraw info.");
    }
    return *it;
}


/*
 * datraw::info<C>::find_tag
 */
template<class C>
template<class T, size_t N>
const T *datraw::info<C>::find_tag(const T(&tags)[N], const string_type& tag) {
    for (size_t i = 0; i < N; ++i) {
        if (tags[i].Tag == tag) {
            return &tags[i];
        }
    }

    return nullptr;
}


/*
 * datraw::info<C>::parse
 */
template<class C>
template<datraw::variant_type T, datraw::variant_type... Ts>
typename datraw::info<C>::variant_type datraw::info<C>::parse(
        detail::variant_type_list_t<T, Ts...>, const string_type& str,
        const datraw::variant_type type) {
    typedef typename variant_type::forward_traits<T>::value_type value_type;

    switch (type) {
        case datraw::variant_type::string:
        case datraw::variant_type::wstring:
            return str;

        case datraw::variant_type::vec_int8:
        case datraw::variant_type::vec_int16:
        case datraw::variant_type::vec_int32:
        case datraw::variant_type::vec_int64:
        case datraw::variant_type::vec_uint8:
        case datraw::variant_type::vec_uint16:
        case datraw::variant_type::vec_uint32:
        case datraw::variant_type::vec_uint64:
        case datraw::variant_type::vec_float32:
        case datraw::variant_type::vec_float64:
            // This is an array of space-separated items.
            return info::parse_vec(parsable_vectors_t(), str, type);

        case datraw::variant_type::scalar_type:
            // Special handling using lookup table.
            return info::parse_scalar_type(str);

        case datraw::variant_type::grid_type:
            // Special handling using lookup table.
            return info::parse_grid_type(str);

        case datraw::variant_type::endianness:
            // Special handling using lookup table.
            return info::parse_endianness(str);

        case T:
            // This was a hit for the types in strstrm_parsables_t.
            return info::parse<T>(str);

        default:
            // No hit, continue searching.
            return info::parse(detail::variant_type_list_t<Ts...>(),
                str, type);
    }
}


/*
 * datraw::info<C>::parse
 */
template<class C>
typename datraw::info<C>::variant_type datraw::info<C>::parse(
        detail::variant_type_list_t<>, const string_type& str,
        const datraw::variant_type type) {
    std::stringstream msg;
    msg << "\"" << info::narrow_string(str) << "\" cannot be parsed into "
        "a variant." << std::ends;
    throw std::invalid_argument(msg.str());
}


/*
 * datraw::info<C>::parse
 */
template<class C>
template<class T> T datraw::info<C>::parse(const string_type& str) {
    std::basic_stringstream<char_type> input(str);
    T retval;

    if (!(input >> retval)) {
        std::stringstream msg;
        msg << "\"" << info::narrow_string(str) << "\" cannot be "
            "parsed as " << typeid(T).name() << std::ends;
        throw std::invalid_argument(msg.str());
    }

    return retval;
}


/*
 * datraw::info<C>::parse_endianness
 */
template<class C>
typename datraw::info<C>::variant_type datraw::info<C>::parse_endianness(
        const string_type& str) {
    static const struct {
        string_type Tag;
        endianness Value;
    } ENDIANNESSES[] = {
        { DATRAW_TPL_LITERAL(C, "BIG_ENDIAN"), endianness::big },
        { DATRAW_TPL_LITERAL(C, "LITTLE_ENDIAN"), endianness::little },
    };

    auto s = info::to_upper(info::trim(str));
    auto it = info::find_tag(ENDIANNESSES, s);
    if (it != nullptr) {
        return it->Value;
    } else {
        std::stringstream msg;
        msg << "\"" << info::narrow_string(s) << "\" does not designate a "
            "valid endianness." << std::ends;
        throw std::invalid_argument(msg.str());
    }
}


/*
 * datraw::info<C>::parse_grid_type
 */
template<class C>
typename datraw::info<C>::variant_type datraw::info<C>::parse_grid_type(
        const string_type& str) {
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

    auto s = info::to_upper(info::trim(str));
    auto it = info::find_tag(GRID_TYPES, s);
    if (it != nullptr) {
        return it->Value;
    } else {
        std::stringstream msg;
        msg << "\"" << info::narrow_string(s) << "\" is not a valid grid "
            "type." << std::ends;
        throw std::invalid_argument(msg.str());
    }
}


/*
 * datraw::info<C>::parse_scalar_type
 */
template<class C>
typename datraw::info<C>::variant_type datraw::info<C>::parse_scalar_type(
        const string_type& str) {
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

    auto s = info::to_upper(info::trim(str));
    auto it = info::find_tag(SCALAR_TYPES, s);
    if (it != nullptr) {
        return it->Value;
    } else {
        std::stringstream msg;
        msg << "\"" << info::narrow_string(s) << "\" is not a valid scalar "
            "type." << std::ends;
        throw std::invalid_argument(msg.str());
    }
}


/*
 * datraw::info<C>::parse_vec
 */
template<class C>
template<datraw::variant_type T, datraw::variant_type... Ts>
typename datraw::info<C>::variant_type datraw::info<C>::parse_vec(
        detail::variant_type_list_t<T, Ts...>, const string_type& str,
        const datraw::variant_type type) {
    typedef typename variant_type::forward_traits<T>::value_type vector_type;
    typedef typename vector_type::value_type value_type;


    if (type == T) {
        // This is the right implementation.
        vector_type retval;

        auto tokens = info::tokenise(str.cbegin(), str.cend(),
            DATRAW_TPL_LITERAL(C, ' '), DATRAW_TPL_LITERAL(C, '\t'));
        retval.reserve(tokens.size());

        assert(tokens.size() <= static_cast<size_t>(INT_MAX));
        for (int i = 0; i < static_cast<int>(tokens.size()) - 1; ++i) {
            auto b = info::skip_spaces(tokens[i], tokens[i + 1]);
            auto e = tokens[i + 1];
            auto s = string_type(b, e);
            retval.push_back(info::parse<value_type>(s));
        }

        return retval;

    } else {
        // Continue searching.
        return info::parse_vec(detail::variant_type_list_t<Ts...>(), str, type);
    }
}


/*
 * datraw::info<C>::parse_vec
 */
template<class C>
typename datraw::info<C>::variant_type datraw::info<C>::parse_vec(
        detail::variant_type_list_t<>, const string_type& str,
        const datraw::variant_type type) {
    std::stringstream msg;
    msg << "\"" << info::narrow_string(str) << "\" cannot be parsed into "
        "a vector variant." << std::ends;
    throw std::invalid_argument(msg.str());
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
