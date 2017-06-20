/// <copyright file="raw_reader.h" company="Visualisierungsinstitut der Universität Stuttgart">
/// Copyright © 2017 Visualisierungsinstitut der Universität Stuttgart. Alle Rechte vorbehalten.
/// </copyright>
/// <author>Christoph Müller</author>

#pragma once

#include <fstream>
#include <sstream>

#include "datraw/convert.h"
#include "datraw/info.h"


namespace datraw {

    /// <summary>
    /// Implements a reader for the raw files.
    /// </summary>
    /// <remarks>
    /// The main task of the reader is evaluating the file names from a
    /// <see cref="info" /> and to perform conversions as necessary.
    /// </remarks>
    template<class C> class raw_reader {

    public:

        /// <summary>
        /// The type used to represent a single character in a string.
        /// </summary>
        typedef C char_type;

        /// <summary>
        /// The type of the <see cref="info" /> object representing the dat
        /// file.
        /// </summary>
        typedef info<char_type> info_type;

        /// <summary>
        /// The type to express file sizes.
        /// </summary>
        typedef std::size_t size_type;

        /// <summary>
        /// The type of input stream.
        /// </summary>
        typedef std::basic_ifstream<char_type> ifstream_type;

        /// <summary>
        /// The type used to represent a string.
        /// </summary>
        typedef std::basic_string<char_type> string_type;

        /// <summary>
        /// The type to express a time step.
        /// </summary>
        typedef typename std::decay<decltype(
            std::declval<info_type>().time_steps())>::type time_step_type;

        /// <summary>
        /// Open a new reader for the specifie dat file.
        /// </summary>
        /// <param name="datPath">The path to the dat file.</param>
        /// <returns>A reader for the raw data described in the dat file.
        /// </returns>
        static inline raw_reader open(const string_type& datPath) {
            raw_reader retval;
            retval.datInfo = info_type::parse(datPath);
            return std::move(retval);
        }

        /// <summary>
        /// Initialises a new instance.
        /// </summary>
        inline raw_reader(void) : curTimeStep(0) { }

        /// <summary>
        /// Initialises a new instance for the given dat file content.
        /// </summary>
        /// <param name="info">The content of a dat file.</param>
        inline raw_reader(const info_type& info)
            : datInfo(info), curTimeStep(0) { }

        /// <summary>
        /// Initialises a new instance for the given dat file content.
        /// </summary>
        /// <param name="info">The content of a dat file.</param>
        inline raw_reader(info_type&& info)
            : datInfo(std::move(info)), curTimeStep(0) { }

        /// <summary>
        /// Answer the content of the dat file.
        /// </summary>
        /// <returns>The content of the dat file.</returns>
        inline const info_type& info(void) const {
            return this->datInfo;
        }

        /// <summary>
        /// Read the content of the current time step and store it to
        /// <paramref name="dst" /> provided the buffer size
        /// (<paramref name="cntDst" />) is large enough.
        /// </summary>
        /// <param name="dst">Pointer to <paramref name="cntDst" /> bytes of
        /// memory where the raw data can be stored. Nothing will be written if
        /// this is <c>nullptr</c>.</param>
        /// <param name="cntDst">The size of the buffer <paramref name="dst" />
        /// in bytes. Nothing will be written if this is less than the required
        /// number of bytes to hold the whole time step.</param>
        /// <returns>The size of the time step in bytes. The return value is
        /// independent from whether data have actually been written.</returns>
        /// <exception cref="std::range_error">If the time series has been
        /// completely read, ie the current time step is invalid.</exception>
        /// <exception cref="std::invalid_argument">If the path of the current
        /// time step was invalid, ie the raw file could not be opened.
        /// </exception>
        size_type read_current(void *dst, const size_type cntDst) const;

        /// <summary>
        /// Resets the current time step to the begin of the sequence.
        /// </summary>
        inline void reset(void) {
            this->curTimeStep = 0;
        }

    private:

        /// <summary>
        /// Stores the current time step.
        /// </summary>
        time_step_type curTimeStep;

        /// <summary>
        /// The content from a dat file.
        /// </summary>
        info_type datInfo;
    };

} /* end namespace datraw */

#include "datraw/raw_reader.inl"
