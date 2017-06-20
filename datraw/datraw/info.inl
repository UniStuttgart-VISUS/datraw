/// <copyright file="info.inl" company="Visualisierungsinstitut der Universit�t Stuttgart">
/// Copyright � 2017 Visualisierungsinstitut der Universit�t Stuttgart. Alle Rechte vorbehalten.
/// </copyright>
/// <author>Christoph M�ller</author>


/*
 * datraw::info<C>::is_multi_file_description
 */
template<class C>
bool datraw::info<C>::is_multi_file_description(const string_type& str) {
    // Direct adaption of Thomas Klein's code.
    static const auto MARKER = DATRAW_TPL_LITERAL(C, '%');

    const char_type *s = str.data();
    while (*s) {
        if ((s[0] == MARKER) && (s[1] != MARKER)) {
            return true;
        }
        ++s;
    }

    return false;
}


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
        { info::property_slice_thickness, datraw::variant_type::vec_float32  },
        { info::property_tetrahedra, datraw::variant_type::uint64 },
        { info::property_time_steps, datraw::variant_type::uint64 },
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

    retval.check();
    retval.datPath = file;
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
 * datraw::info<C>::check
 */
template<class C>
void datraw::info<C>::check(void) {
    /* Check fatal errors. */
    {
        auto& pn = info::property_object_file_name;
        if (!this->contains(pn) || this->object_file_name().empty()) {
            std::stringstream msg;
            msg << "The property \"" << info::narrow_string(pn) << "\" is "
                "mandatory and must be a non-empty string." << std::ends;
            throw std::runtime_error(msg.str());
        }
    }

    {
        auto& pn = info::property_format;
        if (!this->contains(pn)) {
            std::stringstream msg;
            msg << "The property \"" << info::narrow_string(pn) << "\" is "
                "mandatory." << std::ends;
            throw std::runtime_error(msg.str());
        }
    }

    /* Fix soft errors. */
    {
        typedef typename std::decay<decltype(this->components())>::type v_t;
        auto& pn = info::property_components;
        if (!this->contains(pn)) {
            // If number of components is not given, assume scalars.
            this->properties[pn] = static_cast<v_t>(1);
        }
    }

    {
        auto& pn = info::property_byte_order;
        if (!this->contains(pn)) {
            // If byte order is not given, assume Intel (little endian).
            this->properties[pn] = datraw::endianness::little;
        }
    }

    {
        auto& pn = info::property_grid_type;
        if (!this->contains(pn)) {
            // If no grid is given, assume a Cartesian one.
            this->properties[pn] = datraw::grid_type::cartesian;
        }
    }

    {
        typedef typename std::decay<decltype(this->time_steps())>::type v_t;
        auto& pn = info::property_time_steps;
        if (!this->contains(pn)) {
            // Assume a single time step.
            this->properties[pn] = static_cast<v_t>(1);
        }
    }

    {
        typedef typename std::decay<decltype(this->dimensions())>::type v_t;
        auto& pn = info::property_dimensions;
        if (!this->contains(pn)) {
            // Assume 3D volumetric data.
            this->properties[pn] = static_cast<v_t>(3);
        }
    }

    {
        typedef typename std::decay<decltype(this->data_offset())>::type v_t;
        auto& pn = info::property_data_offset;
        if (!this->contains(pn)) {
            // Assume that the raw file has no header.
            this->properties[pn] = static_cast<v_t>(0);
        }
    }

    if (this->grid_type() != datraw::grid_type::cartesian) {
        throw std::runtime_error("Only Cartesian grids are supported at the "
            "moment.");
        // TODO: implement rectilinear grids.
    }

#if 0
     case DR_GRID_RECTILINEAR:
         if (strstr(inputLine, "SLICETHICKNESS")) {
             if (sscanf(inputLine, "SLICETHICKNESS[%d]", &axis) != 1) {
                 datRaw_logError("SLICKETHICKNESS must be subscripted with the axis in case of a RECTILINEAR grid!\n");
                 return 0;
             }

             size = 0;
             for (i = 0; i < axis; ++i) {
                 size += info->resolution[i];
             }

             if (info->sliceDist[size] != -1.0f) {
                 datRaw_logError("DatRaw Error: Multiple slice distance lines given\n");
                 error = 1;
             } else {
                 i = 0;
                 p = s1 = dupstr(sep + 1);
                 if (!p) {
                     datRaw_logError("DatRaw: Error parsing dat-file - Failed to allocate temp. storage!\n");
                     return 0;
                 }
                 /*while ((s2 = strsep(&s1," \t\n"))) {*/ /* not ansi :-( */
                 while ((s2 = strtok(s1, " ,\t\n"))) {
                     if (*s2 == '\0') {
                         continue;
                     }
                     if (i >= info->resolution[axis]) {
                         error = 1;
                         break;
                     }
                     if (sscanf(s2, "%f", &info->sliceDist[size + i]) != 1) {
                         error = 1;
                         break;
                     }
                     i++;
                     s1 = NULL;
                 }
                 free(p);
             }
         }
         break;

#endif

    /* Check/fix errors depending on the grid type. */
    if (this->grid_type() == datraw::grid_type::cartesian) {
        auto& pn = info::property_slice_thickness;
        if (!this->contains(pn)) {
            typename std::decay<decltype(this->slice_thickness())>::type v;
            v.resize(this->dimensions());
            std::fill(v.begin(), v.end(), 1);
            this->properties[pn] = v;
        }

        if (this->slice_thickness().size() != this->dimensions()) {
            std::stringstream msg;
            msg << "The property \"" << info::narrow_string(pn) << "\" "
                "must specify the slice thickness for all of the "
                << this->dimensions() << " dimensions." << std::ends;
            throw std::runtime_error(msg.str());
        }
    }

    switch (this->grid_type()) {
        case datraw::grid_type::cartesian:
        case datraw::grid_type::rectilinear:
            /* Resolution is mandatory and must be set for each dimesnion. */
            {
                auto& pn = info::property_resolution;
                if (!this->contains(pn)) {
                    std::stringstream msg;
                    msg << "The property \"" << info::narrow_string(pn) << "\" "
                        "is mandatory for cartesian and rectilinear grids."
                        << std::ends;
                    throw std::runtime_error(msg.str());
                }

                if (this->resolution().size() != this->dimensions()) {
                    std::stringstream msg;
                    msg << "The property \"" << info::narrow_string(pn) << "\" "
                        "must specify the resolution for all of the "
                        << this->dimensions() << " dimensions." << std::ends;
                    throw std::runtime_error(msg.str());
                }
            }
            break;

        case datraw::grid_type::tetrahedral:
            {
            typedef typename std::decay<decltype(this->vertices())>::type v_t;
                auto& pn = info::property_vertices;
                if (!this->contains(pn)) {
                    this->properties[pn] = static_cast<v_t>(0);
                }
            }
            {
                typedef typename std::decay<decltype(this->tetrahedra())>::type v_t;
                auto& pn = info::property_tetrahedra;
                if (!this->contains(pn)) {
                    this->properties[pn] = static_cast<v_t>(0);
                }
            }
            break;
    }
}


/*
 * datraw::info<C>::element_size
 */
template<class C>
size_t datraw::info<C>::element_size(void) const {
    try {
        return this->scalar_size() * this->dimensions();
    } catch (...) {
        return 0;
    }
}


/*
 * datraw::info<C>::evaluate_path
 */
template<class C>
typename datraw::info<C>::string_type datraw::info<C>::evaluate_path(
        const string_type& path) const {
    auto len = path.length();
    auto isAbsolute = false;

    if (len > 0) {
        if (info::is_dir_sep(path[0])) {
            isAbsolute = true;
        }
#ifdef _WIN32
        if (((len > 2) && (path[1] == DATRAW_TPL_LITERAL(C, ':')))
                && info::is_dir_sep(path[2])) {
            // Note: A path with disk designator is only absolute if followed
            // by a directory separator as described on
            // https://msdn.microsoft.com/en-us/library/windows/desktop/aa365247(v=vs.85).aspx
            isAbsolute = true;
        }
#endif /* _WIN32 */
    }

    if (isAbsolute) {
        // An absolute path does not need to be modified.
        return path;

    } else {
        auto it = std::find_if(this->datPath.rbegin(), this->datPath.rend(),
            info::is_dir_sep);
        if (it == this->datPath.rend()) {
            // The dat file is in the current working directory, so any relative
            // path in it is valid, too.
            return path;
        }

        string_type retval(this->datPath.begin(), it.base());
        retval += path;
        return retval;
    }
}


/*
 * datraw::info<C>::multi_file_name
 */
template<class C>
typename datraw::info<C>::string_type datraw::info<C>::multi_file_name(
        const std::uint64_t timeStep) const {
    // Adapted directly from Thomas Klein's code.
    int minWidth, offset, stride;
    auto tpl = info::parse_multi_file_description(this->object_file_name(),
        minWidth, offset, stride);
    if (tpl.empty()) {
        return tpl;
    }

    std::vector<char_type> retval;
    retval.resize(tpl.length()
        + (std::max)(static_cast<int>(log10(this->time_steps())), minWidth)
        + 2);

    ::sprintf(retval.data(), tpl.c_str(), offset + stride * timeStep);

    return retval.data();
}


/*
 * datraw::info<C>::property_names
 */
template<class C>
template<class I> void datraw::info<C>::property_names(I oit) const {
    for (auto& it : this->properties) {
        *oit++ = it.first;
    }
}


/*
 * datraw::info<C>::scalar_size
 */
template<class C>
size_t datraw::info<C>::scalar_size(void) const {
    try {
        return datraw::get_scalar_size(this->format());
    } catch (...) {
        return 0;
    }
}


/*
 * datraw::info<C>::operator []
 */
template<class C>
typename datraw::info<C>::variant_type& datraw::info<C>::operator [](
        const string_type& prop) {
    auto it = this->properties.find(prop);
    if (it == this->properties.end()) {
        std::stringstream msg;
        msg << "Could not find property \"" << info::narrow_string(prop)
            << "\" in datraw::info." << std::ends;
        throw std::out_of_range(msg.str());
    }
    return it->second;
}


/*
 * datraw::info<C>::operator []
 */
template<class C>
const typename datraw::info<C>::variant_type& datraw::info<C>::operator [](
        const string_type& prop) const {
    auto it = this->properties.find(prop);
    if (it == this->properties.end()) {
        std::stringstream msg;
        msg << "Could not find property \"" << info::narrow_string(prop)
            << "\" in datraw::info." << std::ends;
        throw std::out_of_range(msg.str());
    }
    return it->second;
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
        datraw::grid_type Value;
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
 * datraw::info<C>::parse_multi_file_description
 */
template<class C>
typename datraw::info<C>::string_type
datraw::info<C>::parse_multi_file_description(const string_type& str,
        int& width, int& skip, int& stride) {
    static const std::basic_regex<char_type> RX(DATRAW_TPL_LITERAL(C,
        "(%(?:[0\\- ]([0-9]+))?)(?:\\+([0-9]+))?(?:\\*([0-9]+))?d"),
        std::regex::ECMAScript | std::regex::icase);

    std::match_results<typename string_type::const_iterator> matches;
    string_type strx = DATRAW_TPL_LITERAL(C, "data%03+1*2d.raw");
    if (std::regex_search(strx, matches, RX)) {
        // This was a match, get the groups.
        auto strWidth = matches.str(2);
        width = strWidth.empty() ? 0 : info::parse<int>(strWidth);

        auto strSkip = matches.str(3);
        skip = strWidth.empty() ? 0 : info::parse<int>(strWidth);

        auto strStride = matches.str(4);
        stride = strWidth.empty() ? 1 : info::parse<int>(strWidth);

        auto retval = std::regex_replace(str, RX, matches.str(1)
            + DATRAW_TPL_LITERAL(C, "d"));
        return retval;

    } else {
        // Not a valid multi-file description.
        width = skip = stride = 0;
        return str;
    }

#if 0
    // TODO: replace with regex
    // Direct adaption of Thomas Klein's code.
    static const auto ASTERISK = DATRAW_TPL_LITERAL(C, '*');
    static const auto D = DATRAW_TPL_LITERAL(C, 'd');
    static const auto MARKER = DATRAW_TPL_LITERAL(C, '%');
    static const auto MINUS = DATRAW_TPL_LITERAL(C, '-');
    static const auto PLUS = DATRAW_TPL_LITERAL(C, '+');
    static const auto SPACE = DATRAW_TPL_LITERAL(C, ' ');
    static const auto ZERO = DATRAW_TPL_LITERAL(C, '0');

    std::vector<char_type> retval(str.begin(), str.end());
    char_type *s = nullptr;
    auto p = retval.data();
    auto q = str.data();

    while (*q) {
        if ((q[0] == MARKER) && (q[1] != MARKER)) {
            if (s) {
                std::stringstream msg;
                msg << "The multi-file description \""
                    << info::narrow_string(str) << "\" contains more than one "
                    "varying part." << std::ends;
                throw std::invalid_argument(msg.str());
            } else {
                s = q;
            }
        }
        ++q;
    }
    ++s;

    if (!s || (*s == 0)) {
        std::stringstream msg;
        msg << "The multi-file description \"" << info::narrow_string(str)
            << "\" is invalid." << std::ends;
        throw std::invalid_argument(msg.str());
    }

    if ((*s == ZERO) || (*s == MINUS) || (*s == SPACE)) {
        ++s;
    }

    width = 0;
    while (isdigit(*s)) {
        width = width * 10 + (*s - ZERO);
        s++;
    }

    skip = 0;
    if (*s == PLUS) {
        q = s;
        s++;
        if (!isdigit(*s)) {
            std::stringstream msg;
            msg << "The multi-file description \"" << info::narrow_string(str)
                << "\" contains an invalid skip specification." << std::ends;
            throw std::invalid_argument(msg.str());
        }
        while (isdigit(*s)) {
            skip = skip * 10 + (*s - ZERO);
            s++;
        }
        std::memmove(q, s, strlen(s) + 1);
        s = q;
    }

    stride = 0;
    if (*s == ASTERISK) {
        q = s;
        s++;
        if (!isdigit(*s)) {
            std::stringstream msg;
            msg << "The multi-file description \"" << info::narrow_string(str)
                << "\" contains an invalid stride specification." << std::ends;
            throw std::invalid_argument(msg.str());
        }
        while (isdigit(*s)) {
            stride = stride * 10 + (*s - ZERO);
            s++;
        }
        std::memmove(q, s, strlen(s) + 1);
        s = q;
    } else {
        stride = 1;
    }

    if (*s != D) {
        std::stringstream msg;
        msg << "The multi-file description \"" << info::narrow_string(str)
            << "\" is invalid." << std::ends;
        throw std::invalid_argument(msg.str());
    }

    return p;
#endif
}


/*
 * datraw::info<C>::parse_scalar_type
 */
template<class C>
typename datraw::info<C>::variant_type datraw::info<C>::parse_scalar_type(
        const string_type& str) {
    static const struct {
        string_type Tag;
        datraw::scalar_type Value;
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
