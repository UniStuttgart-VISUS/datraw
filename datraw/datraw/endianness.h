// <copyright file="endianness.h" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2017 - 2024 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#pragma once

#include <cinttypes>


namespace datraw {

    /// <summary>
    /// Represents endianness of a data set or system.
    /// </summary>
    enum class endianness : std::uint32_t {

        /// <summary>
        /// The least significant byte is stored first, like on Intel x86 or
        /// AMD64 architectures.
        /// </summary>
        little = 0x03020100,

        /// <summary>
        /// The most significant byte is stored first, like on Sun Spark.
        /// </summary>
        big = 0x00010203,

        /// <summary>
        /// The native endian of the system the code is running on.
        /// </summary>
        system = '\0\1\2\3'
    };

}
