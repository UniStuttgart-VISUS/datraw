/// <copyright file="info.cpp" company="Visualisierungsinstitut der Universität Stuttgart">
/// Copyright © 2017 Visualisierungsinstitut der Universität Stuttgart. Alle Rechte vorbehalten.
/// </copyright>
/// <author>Christoph Müller</author>


#include "datraw/info.h"

#include <array>
#include <fstream>
#include <streambuf>

//static const std::array<std::


/*
 * datraw::info::parse
 */
datraw::info datraw::info::parse(const string_type& file) {
    std::string content;
    datraw::info retval;

    /* Read the whole dat file specified by the user. */
    {
        std::ifstream stream(file);

        stream.seekg(0, std::ios::end);
        content.reserve(stream.tellg());
        stream.seekg(0, std::ios::beg);
        content.assign((std::istreambuf_iterator<char_type>(stream)),
            std::istreambuf_iterator<char_type>());
    }

    /* Parse the dat file. */
    auto lines = info::tokenise(content, true, '\r', '\n');





    return retval;
}
