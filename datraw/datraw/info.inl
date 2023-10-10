// <copyright file="info.inl" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2017 - 2023 Visualisierungsinstitut der Universität Stuttgart. Alle Rechte vorbehalten.
// </copyright>
// <author>Christoph Müller</author>


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
 * datraw::info<C>::load
 */
template<class C>
datraw::info<C> datraw::info<C>::load(const string_type& file) {
    string_type content;
    std::basic_ifstream<char_type> stream(file);

    if (!stream) {
        std::stringstream msg;
        msg << "Failed opening dat file \"" << detail::narrow_string(file)
            << "\".";
        throw std::runtime_error(msg.str());
    }

    stream.seekg(0, std::ios::end);
    content.reserve(static_cast<std::size_t>(stream.tellg()));
    stream.seekg(0, std::ios::beg);
    content.assign((std::istreambuf_iterator<char_type>(stream)),
        std::istreambuf_iterator<char_type>());
    stream.close();

    return info::parse(content, file);
}


/*
 * datraw::info<C>::parse
 */
template<class C>
datraw::info<C> datraw::info<C>::parse(const string_type& content,
        const string_type& file) {
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
        { info::property_object_file_name, detail::variant_rev_traits<C, string_type>::type },
        { info::property_origin, datraw::variant_type::vec_uint32 },
        { info::property_resolution, datraw::variant_type::vec_uint32 },    // if (!(info->resolution = (int*)malloc(info->dimensions * sizeof(int)))) {
        { info::property_slice_thickness, datraw::variant_type::vec_float32 },
        { info::property_tetrahedra, datraw::variant_type::uint64 },
        { info::property_time_steps, datraw::variant_type::uint64 },
        { info::property_vertices, datraw::variant_type::uint64 },
    };
    static const std::basic_regex<char_type> RX_RECTLNR_THICKNESS(
        DATRAW_TPL_LITERAL(C, "^SLICETHICKNESS\\[([0-9]+)\\]$"),
        std::regex::ECMAScript | std::regex::icase);

    datraw::info<C> retval;

    /* Parse the dat file. */
    auto lines = info::tokenise(content.cbegin(), content.cend(),
        DATRAW_TPL_LITERAL(C, '\r'), DATRAW_TPL_LITERAL(C, '\n'));
    assert(lines.size() <= static_cast<std::size_t>(INT_MAX));
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
            auto ucKey = detail::to_upper(key);
            auto value = info::skip_spaces(++colon, e);

            /* Check for duplicates. */
            {
                auto duplicate = retval.properties.find(key);
                if (duplicate == retval.properties.end()) {
                    duplicate = retval.properties.find(ucKey);
                }
                if (duplicate != retval.properties.end()) {
                    std::stringstream msg;
                    msg << "Duplicate property \"" << detail::narrow_string(key)
                        << "\" found at line " << (i + 1) << ".";
                    throw std::runtime_error(msg.str());
                }
            }

            /* Check whether we have a known or a custom property. */
            auto kp = detail::find_tag(KNOWN_PROPERTIES, ucKey);
            std::match_results<typename string_type::const_iterator> matches;
            if (kp != nullptr) {
                // This is one of the known properties which we might perform
                // special handling for.
                key = ucKey;
                retval.properties[key] = info::parse(parsable_scalars_t(),
                    string_type(value, e), kp->Type);

            } else if (std::regex_search(key, matches, RX_RECTLNR_THICKNESS)) {
                // This are slice distances for a rectilinear grid.
                auto axis = info::parse(parsable_scalars_t(), matches.str(1),
                    datraw::variant_type::uint32);
                key = info::format_slice_thickness(axis);
                retval.properties[key] = info::parse(parsable_scalars_t(),
                    string_type(value, e), datraw::variant_type::vec_float32);

            } else {
                // This is a user-defined property. Store it as string.
                retval.properties[key] = string_type(value, e);
            }

        } else {
            // If there is no colon, we have a syntax error.
            std::stringstream msg;
            msg << "Syntax error in line " << (i + 1) << ": \""
                << detail::narrow_string(string_type(b, e)) << "\".";
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
            msg << "The property \"" << detail::narrow_string(pn) << "\" is "
                "mandatory and must be a non-empty string.";
            throw std::runtime_error(msg.str());
        }
    }

    {
        auto& pn = info::property_format;
        if (!this->contains(pn)) {
            std::stringstream msg;
            msg << "The property \"" << detail::narrow_string(pn) << "\" is "
                "mandatory.";
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

    /* Bail out if a grid type is not supported right now. */
    switch (this->grid_type()) {
        case datraw::grid_type::cartesian:
        case datraw::grid_type::rectilinear:
            break;

        default:
            throw std::runtime_error("The specified grid type is not supported "
                "at the moment.");
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
            std::fill(v.begin(), v.end(), 1.0f);
            this->properties[pn] = v;
        }

        if (this->slice_thickness().size() != this->dimensions()) {
            std::stringstream msg;
            msg << "The property \"" << detail::narrow_string(pn) << "\" "
                "must specify the slice thickness for all of the "
                << this->dimensions() << " dimensions.";
            throw std::runtime_error(msg.str());
        }
    }

    switch (this->grid_type()) {
        case datraw::grid_type::cartesian:
        case datraw::grid_type::rectilinear:
            /* Resolution is mandatory and must be set for each dimension. */
            {
                auto& pn = info::property_resolution;
                if (!this->contains(pn)) {
                    std::stringstream msg;
                    msg << "The property \"" << detail::narrow_string(pn)
                        << "\" is mandatory for cartesian and rectilinear "
                        << "grids.";
                    throw std::runtime_error(msg.str());
                }

                if (this->resolution().size() != this->dimensions()) {
                    std::stringstream msg;
                    msg << "The property \"" << detail::narrow_string(pn)
                        << "\" must specify the resolution for all of the "
                        << this->dimensions() << " dimensions.";
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

    /*
     * Now that we know that the resolution is OK, we can check/fix the slice
     * distances of the rectilinear grid.
     */
    if (this->grid_type() == grid_type::rectilinear) {
        static const auto fmt = detail::literal_selector<C>::select("%s[%u]",
            L"%S[%u]");
        auto resolution = this->resolution();

        // Make sure that the Cartesian slice distances are not present.
        this->properties.erase(info::property_slice_thickness);

        for (std::uint32_t i = 0; i < this->dimensions(); ++i) {
            auto p = detail::format(fmt,
                info::property_slice_thickness.c_str(), i);

            try {
                auto var = (*this)[p];
                auto val = var.template get<std::vector<float>>();

                if (val.size() < resolution[i]) {
                    // Append default slice distances for missing dimensions.
                    val.reserve(resolution[i]);
                    while (val.size() < resolution[i]) {
                        val.push_back(1.0f);
                    }
                    this->properties[p] = val;

                } else if (val.size() > resolution[i]) {
                    // There are too many slices, remove excess data.
                    val.resize(resolution[i]);
                    this->properties[p] = val;
                }

            } catch (...) {
                // In case of an error (this should only happen if the slice
                // distances are not set), apply a default value of equidistant
                // slices for the axis.
                std::vector<float> value(resolution[i], 1.0f);
                this->properties[p] = value;
            }
        }
    }
}


/*
 * datraw::info<C>::element_size
 */
template<class C>
std::size_t datraw::info<C>::element_size(void) const {
    try {
        return this->scalar_size() * this->components();
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

    return detail::format(tpl, offset + stride * timeStep);
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
 * datraw::info<C>::row_pitch
 */
template<class C>
std::size_t datraw::info<C>::row_pitch(const std::size_t alignment) const {
    auto retval = this->row_size();

    if (alignment > 0) {
        if (retval < alignment) {
            retval = alignment;
        } else {
            retval += (retval % alignment);
        }
    }

    return retval;
}


/*
 * datraw::info<C>::row_size
 */
template<class C>
std::size_t datraw::info<C>::row_size(void) const {
    switch (this->grid_type()) {
        case datraw::grid_type::cartesian:
        case datraw::grid_type::rectilinear:
            return this->resolution().empty()
                ? 0
                : this->resolution().front() * this->element_size();

        default:
            throw std::runtime_error("Only Cartesian and rectilinear grids are "
                "organised in rows which the width can be computed of.");
    }
}


/*
 * datraw::info<C>::scalar_size
 */
template<class C>
std::size_t datraw::info<C>::scalar_size(void) const {
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
        msg << "Could not find property \"" << detail::narrow_string(prop)
            << "\" in datraw::info.";
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
        msg << "Could not find property \"" << detail::narrow_string(prop)
            << "\" in datraw::info.";
        throw std::out_of_range(msg.str());
    }
    return it->second;
}


/*
 * datraw::info<C>::format_slice_thickness
 */
template<class C>
typename datraw::info<C>::string_type datraw::info<C>::format_slice_thickness(
        const std::uint32_t axis) {
    static const auto FMT = detail::literal_selector<C>::select("%s[%u]",
        L"%S[%u]");
    return detail::format(FMT, info::property_slice_thickness.c_str(), axis);
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

        case T:
            // This was a hit for the types in strstrm_parsables_t.
            return datraw::parse<T>(str);

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
    msg << "\"" << detail::narrow_string(str) << "\" cannot be parsed into "
        "a variant.";
    throw std::invalid_argument(msg.str());
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
    if (std::regex_search(str, matches, RX)) {
        // This was a match, get the groups.
        auto strWidth = matches.str(2);
        width = strWidth.empty() ? 0 : datraw::parse<int>(strWidth);

        auto strSkip = matches.str(3);
        skip = strSkip.empty() ? 0 : datraw::parse<int>(strSkip);

        auto strStride = matches.str(4);
        stride = strStride.empty() ? 1 : datraw::parse<int>(strStride);

        auto retval = std::regex_replace(str, RX, matches.str(1)
            + DATRAW_TPL_LITERAL(C, "d"));
        return retval;

    } else {
        // Not a valid multi-file description.
        width = skip = stride = 0;
        return str;
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
    typedef typename detail::variant_fwd_traits<C, T>::value_type vector_type;
    typedef typename vector_type::value_type value_type;

    if (type == T) {
        // This is the right implementation.
        vector_type retval;

        auto tokens = info::tokenise(str.cbegin(), str.cend(),
            DATRAW_TPL_LITERAL(C, ' '), DATRAW_TPL_LITERAL(C, '\t'));
        retval.reserve(tokens.size());

        assert(tokens.size() <= static_cast<std::size_t>(INT_MAX));
        for (int i = 0; i < static_cast<int>(tokens.size()) - 1; ++i) {
            auto b = info::skip_spaces(tokens[i], tokens[i + 1]);
            auto e = tokens[i + 1];
            auto s = string_type(b, e);
            retval.push_back(datraw::parse<value_type>(s));
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
    msg << "\"" << detail::narrow_string(str) << "\" cannot be parsed into "
        "a vector variant.";
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
