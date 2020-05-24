/// <copyright file="string.inl" company="Visualisierungsinstitut der Universität Stuttgart">
/// Copyright © 2020 Visualisierungsinstitut der Universität Stuttgart. Alle Rechte vorbehalten.
/// </copyright>
/// <author>Christoph Müller</author>


/*
 * datraw::detail::format
 */
template<class... A>
std::string datraw::detail::format(const std::string& fmt, A... args) {
    auto f = fmt.c_str();

#if (defined(_MSC_VER) && (_MSC_VER >= 1400))
    auto length = ::_scprintf(f, args...) + 1;
#elif defined(_WIN32)
    auto length = ::_snprintf(nullptr, 0, f, args...) + 1;
#else
    auto length = ::snprintf(nullptr, 0, f, args...) + 1;
#endif

    std::vector<char> retval(length, '\0');

#if (defined(_MSC_VER) && (_MSC_VER >= 1400))
    ::_snprintf_s(retval.data(), length, length, f, args...);
#elif defined(_WIN32)
    ::_snprintf(retval.data(), length, f, args...);
#else
    ::snprintf(retval.data(), length, f, args...);
#endif

    return retval.data();
}


/*
 * datraw::detail::format
 */
template<class... A>
std::wstring datraw::detail::format(const std::wstring& fmt, A... args) {
    auto f = fmt.c_str();

#if defined(_WIN32)
#if (defined(_MSC_VER) && (_MSC_VER >= 1400))
    auto length = ::_scwprintf(f, args...) + 1;
#else /* (defined(_MSC_VER) && (_MSC_VER >= 1400)) */
    auto length = ::_snwprintf(nullptr, 0, f, args...) + 1;
#endif /* (defined(_MSC_VER) && (_MSC_VER >= 1400)) */

    std::vector<wchar_t> retval(length, '\0');

#if (defined(_MSC_VER) && (_MSC_VER >= 1400))
    ::_snwprintf_s(retval.data(), length, length, f, args...);
#else /* (defined(_MSC_VER) && (_MSC_VER >= 1400)) */
    ::_snwprintf(retval.data(), length, f, args...);
#endif /* (defined(_MSC_VER) && (_MSC_VER >= 1400)) */

    return retval.data();

#else /* defined(_WIN32) */
    auto length = static_cast<std::size_t>(1.5f * fmt.length());
    std::vector<wchar_t> retval(length, L'\0');

    while (::swprintf(retval.data(), retval.size(), fmt.c_str(), args...) < 0) {
        length = static_cast<std::size_t>(1.5f * retval.size());
        retval.resize(length);
    }

    return retval.data();
#endif /* defined(_WIN32) */
}


/*
 * datraw::detail::narrow_string
 */
std::string datraw::detail::narrow_string(const std::wstring& str) {
#if (!defined(__GNUC__) || (__GNUC__ >= 5))
    static std::wstring_convert<std::codecvt_utf8<wchar_t>> cvt;
    return cvt.to_bytes(str);
#else /* (!defined(__GNUC__) || (__GNUC__ >= 5)) */
    std::string retval;
    retval.reserve(str.size());
    for (auto c : str) {
        retval.push_back(static_cast<char>(c));
    }
    return retval;
#endif /* (!defined(__GNUC__) || (__GNUC__ >= 5)) */
}


/*
 * datraw::detail::widen_string
 */
std::wstring datraw::detail::widen_string(const std::string& str) {
#if (!defined(__GNUC__) || (__GNUC__ >= 5))
    static std::wstring_convert<std::codecvt_utf8<wchar_t>> cvt;
    return cvt.from_bytes(str);
#else /* (!defined(__GNUC__) || (__GNUC__ >= 5)) */
    std::wstring retval;
    retval.reserve(str.size());
    for (auto c : str) {
        retval.push_back(c);
    }
    return retval;
#endif /* (!defined(__GNUC__) || (__GNUC__ >= 5)) */
}
