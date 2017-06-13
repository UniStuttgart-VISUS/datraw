/// <copyright file="grid_type.h" company="Visualisierungsinstitut der Universität Stuttgart">
/// Copyright © 2017 Visualisierungsinstitut der Universität Stuttgart. Alle Rechte vorbehalten.
/// </copyright>
/// <author>Christoph Müller</author>

#pragma once


namespace datraw {

    /// <summary>
    /// Possible grid types supported by the library.
    /// </summary>
    enum class grid_type {
        cartesian = 1,
        rectilinear,
        tetrahedral
    };

}
