/// <copyright file="grid_type.h" company="Visualisierungsinstitut der Universit�t Stuttgart">
/// Copyright � 2017 Visualisierungsinstitut der Universit�t Stuttgart. Alle Rechte vorbehalten.
/// </copyright>
/// <author>Christoph M�ller</author>

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
