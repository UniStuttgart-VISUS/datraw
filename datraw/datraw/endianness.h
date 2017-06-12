/// <copyright file="endianness.h" company="Visualisierungsinstitut der Universität Stuttgart">
/// Copyright © 2017 Visualisierungsinstitut der Universität Stuttgart. Alle Rechte vorbehalten.
/// </copyright>
/// <author>Christoph Müller</author>

#pragma once


namespace datraw {

    /// <summary>
    /// Represents endianness of a data set or system.
    /// </summary>
    enum class endianness {

        /// <summary>
        /// The least significant byte is stored first, like on Intel x86 or
        /// AMD64 architectures.
        /// </summary>
        little = 1,

        /// <summary>
        /// The most significant byte is stored first, like on Sun Spark.
        /// </summary>
        big
    };


    /// <summary>
    /// Answer the endianness of the system we are running on.
    /// </summary>
    /// <returns>The endianness of the system.</returns>
//    extern endianness DATRAW_API get_system_endianness(void);
}
