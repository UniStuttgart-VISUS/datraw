// <copyright file="grid_type.h" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2017 - 2024 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#pragma once


namespace datraw {

    /// <summary>
    /// Possible grid types supported by the library.
    /// </summary>
    enum class grid_type {

        /// <summary>
        /// A three-dimensional Cartesian grid with equally spaced dimensions.
        /// </summary>
        cartesian = 1,

        /// <summary>
        /// A rectilinear grid for which the distances between the subdivisions
        /// is explicitly given.
        /// </summary>
        rectilinear,

        /// <summary>
        /// This is currently not supported.
        /// </summary>
        /// <remarks>
        /// Who has added this in the first place in the original C library. The
        /// implementation there seems to be incomplete such that it is not
        /// clear how the format is actually defined.
        /// </remarks>
        tetrahedral
    };

}
