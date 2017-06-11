/// <copyright file="info.inl" company="Visualisierungsinstitut der Universität Stuttgart">
/// Copyright © 2017 Visualisierungsinstitut der Universität Stuttgart. Alle Rechte vorbehalten.
/// </copyright>
/// <author>Christoph Müller</author>


/*
 * datraw::info::tokenise
 */
template<class... D>
std::vector<datraw::info::string_type::const_iterator> datraw::info::tokenise(
        string_type::const_iterator begin, string_type::const_iterator end,
        const bool removeEmpty, D... delims) {
    char_type d[] = { delims... };
    std::vector<string_type::const_iterator> retval;

    retval.push_back(begin);


    return retval;
}
