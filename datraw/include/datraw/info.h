/// <copyright file="info.h" company="Visualisierungsinstitut der Universität Stuttgart">
/// Copyright © 2017 Visualisierungsinstitut der Universität Stuttgart. Alle Rechte vorbehalten.
/// </copyright>
/// <author>Christoph Müller</author>

#pragma once

#include <unordered_map>

#include "datraw/api.h"
#include "datraw/types.h"
#include "datraw/variant.h"


namespace datraw {

    /// <summary>
    /// Holds the content of the dat file, which describes the raw data.
    /// </summary>
    class info {

    public:

        /// <summary>
        /// Parses the given dat file.
        /// </summary>
        /// <param name="file"></param>
        /// <returns></returns>
        //info parse(const std::string& file);

        /// <summary>
        /// Initialises an empty instance.
        /// </summary>
        inline info(void) { }



    private:

        /// <summary>
        /// A collection type storing all the properties parsed from the dat
        /// file.
        /// </summary>
        typedef std::unordered_map<std::string, variant> property_list_type;

        /// <summary>
        /// The properties stored in the dat file.
        /// </summary>
        property_list_type properties;

    };
}
