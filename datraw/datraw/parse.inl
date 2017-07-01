/// <copyright file="parse.inl" company="Visualisierungsinstitut der Universität Stuttgart">
/// Copyright © 2017 Visualisierungsinstitut der Universität Stuttgart. Alle Rechte vorbehalten.
/// </copyright>
/// <author>Christoph Müller</author>


/*
 * datraw::detail::find_tag
 */
template<class T, size_t N, class S>
const T *datraw::detail::find_tag(const T(&tags)[N], const S& tag) {
    for (size_t i = 0; i < N; ++i) {
        if (tags[i].Tag == tag) {
            return &tags[i];
        }
    }

    return nullptr;
}


/*
 * datraw::detail::parser<S, T>::parse
 */
template<class S, class T>
typename datraw::detail::parser<S, T>::value_type
datraw::detail::parser<S, T>::parse(const string_type& str) {
    std::basic_stringstream<char_type> input(str);
    value_type retval;

    if (!(input >> retval)) {
        std::stringstream msg;
        msg << "\"" << narrow_string(str) << "\" cannot be parsed as "
            << typeid(value_type).name() << std::ends;
        throw std::invalid_argument(msg.str());
    }

    return retval;
}


/*
 * datraw::detail::parser<S, datraw::endianness>::parse
 */
template<class S>
typename datraw::detail::parser<S, datraw::endianness>::value_type
datraw::detail::parser<S, datraw::endianness>::parse(const string_type& str) {
    static const struct {
        string_type Tag;
        endianness Value;
    } ENDIANNESSES[] = {
        { DATRAW_TPL_LITERAL(char_type, "BIG_ENDIAN"), endianness::big },
        { DATRAW_TPL_LITERAL(char_type, "LITTLE_ENDIAN"), endianness::little },
    };

    auto s = to_upper(trim(str));
    auto it = find_tag(ENDIANNESSES, s);
    if (it != nullptr) {
        return it->Value;
    } else {
        std::stringstream msg;
        msg << "\"" << narrow_string(s) << "\" does not designate a "
            "valid endianness." << std::ends;
        throw std::invalid_argument(msg.str());
    }
}


/*
 * datraw::detail::parser<S, datraw::grid_type>::parse
 */
template<class S>
typename datraw::detail::parser<S, datraw::grid_type>::value_type
datraw::detail::parser<S, datraw::grid_type>::parse(const string_type& str) {
    static const struct {
        string_type Tag;
        datraw::grid_type Value;
    } GRID_TYPES[] = {
        { DATRAW_TPL_LITERAL(char_type, "EQUIDISTANT"), grid_type::cartesian },
        { DATRAW_TPL_LITERAL(char_type, "CARTESIAN"), grid_type::cartesian },
        { DATRAW_TPL_LITERAL(char_type, "UNIFORM"), grid_type::cartesian },
        { DATRAW_TPL_LITERAL(char_type, "RECTILINEAR"), grid_type::rectilinear },
        { DATRAW_TPL_LITERAL(char_type, "TETRAHEDRA"), grid_type::tetrahedral }
    };

    auto s = to_upper(trim(str));
    auto it = find_tag(GRID_TYPES, s);
    if (it != nullptr) {
        return it->Value;
    } else {
        std::stringstream msg;
        msg << "\"" << narrow_string(s) << "\" is not a valid grid type."
            << std::ends;
        throw std::invalid_argument(msg.str());
    }
}


/*
 * datraw::detail::parser<S, datraw::scalar_type>::parse
 */
template<class S>
typename datraw::detail::parser<S, datraw::scalar_type>::value_type
datraw::detail::parser<S, datraw::scalar_type>::parse(const string_type& str) {
    static const struct {
        string_type Tag;
        datraw::scalar_type Value;
    } SCALAR_TYPES[] = {
        { DATRAW_TPL_LITERAL(char_type, "CHAR"), scalar_type::int8 },
        { DATRAW_TPL_LITERAL(char_type, "UCHAR"), scalar_type::uint8 },
        { DATRAW_TPL_LITERAL(char_type, "SHORT"), scalar_type::int16 },
        { DATRAW_TPL_LITERAL(char_type, "USHORT"), scalar_type::uint16 },
        { DATRAW_TPL_LITERAL(char_type, "INT"), scalar_type::int32 },
        { DATRAW_TPL_LITERAL(char_type, "UINT"), scalar_type::uint32 },
        { DATRAW_TPL_LITERAL(char_type, "LONG"), scalar_type::int64 },
        { DATRAW_TPL_LITERAL(char_type, "ULONG"), scalar_type::uint64 },
        { DATRAW_TPL_LITERAL(char_type, "HALF"), scalar_type::float16 },
        { DATRAW_TPL_LITERAL(char_type, "FLOAT"), scalar_type::float32 },
        { DATRAW_TPL_LITERAL(char_type, "DOUBLE"), scalar_type::float64 }
    };

    auto s = to_upper(trim(str));
    auto it = find_tag(SCALAR_TYPES, s);
    if (it != nullptr) {
        return it->Value;
    } else {
        std::stringstream msg;
        msg << "\"" << narrow_string(s) << "\" is not a valid scalar type."
            << std::ends;
        throw std::invalid_argument(msg.str());
    }
}
