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
        typedef datraw::info<char_type> info_type;

        /// <summary>
        /// The type to express file sizes.
        /// </summary>
        typedef std::size_t size_type;

        /// <summary>
        /// The type of input stream.
        /// </summary>
        /// <remarks>
        /// Note that the input stream is alway working on chars (bytes) as we
        /// are processing the raw files in binary mode.
        /// </remarks>
        typedef std::basic_ifstream<char> ifstream_type;

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
            retval.datInfo = info_type::load(datPath);
            return retval;
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
            : curTimeStep(0), datInfo(info){ }

        /// <summary>
        /// Initialises a new instance for the given dat file content.
        /// </summary>
        /// <param name="info">The content of a dat file.</param>
        inline raw_reader(info_type&& info)
            : curTimeStep(0), datInfo(std::move(info)) { }

        /// <summary>
        /// Answer the content of the dat file.
        /// </summary>
        /// <returns>The content of the dat file.</returns>
        inline const info_type& info(void) const {
            return this->datInfo;
        }

        /// <summary>
        /// Answer the current time step.
        /// </summary>
        /// <returns>The current time step.</returns>
        inline time_step_type current(void) const {
            return this->curTimeStep;
        }

        /// <summary>
        /// Answer whether, according to the <see cref="info" />, there is
        /// another time step with another raw file exists.
        /// </summary>
        /// <returns><c>true</c> if the last time step has not yet been reached,
        /// <c>false</c> otherwise.</returns>
        inline bool has_next(void) const {
            return (this->curTimeStep + 1 < this->datInfo.time_steps());
        }

        /// <summary>
        /// Move to the next time step and answer whether this is valid according
        /// to the <see cref="info" />.
        /// </summary>
        /// <returns><c>true</c> if the new time step is valid, <c>false</c>
        /// otherwise.</returns>
        inline bool move_next(void) {
            ++this->curTimeStep;
            return static_cast<bool>(*this);
        }

        /// <summary>
        /// Move to the specified time step and answer whether said time step is
        /// valid according to the <see cref="info" />.
        /// </summary>
        /// <param name="timeStep">The target time step.</param>
        /// <returns><c>true</c> if the new time step is valid, <c>false</c>
        /// otherwise.</returns>
        inline bool move_to(const time_step_type timeStep) {
            this->curTimeStep = timeStep;
            return static_cast<bool>(*this);
        }

        /// <summary>
        /// Read the content of the current time step and store it to
        /// <paramref name="dst" /> provided the buffer size
        /// (<paramref name="cntDst" />) is large enough.
        /// </summary>
        /// <remarks>
        /// <para>The method will swap the byte order as necessary, ie it is
        /// guaranteed that the data returned match the byte order of the
        /// system.</para>
        /// </remarks>
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
        /// Read the content of the current time step and store it in a new
        /// <see cref="std::vector" />.
        /// </summary>
        /// <remarks>
        /// <para>The method will swap the byte order as necessary, ie it is
        /// guaranteed that the data returned match the byte order of the
        /// system.</para>
        /// </remarks>
        /// <returns>The content of the current time step.</returns>
        /// <exception cref="std::range_error">If the time series has been
        /// completely read, ie the current time step is invalid.</exception>
        /// <exception cref="std::invalid_argument">If the path of the current
        /// time step was invalid, ie the raw file could not be opened.
        /// </exception>
        inline std::vector<datraw::uint8> read_current(void) const {
            std::vector<datraw::uint8> retval;
            retval.resize(this->read_current(nullptr, 0));
            this->read_current(retval.data(), retval.size());
            return retval;
        }

        /// <summary>
        /// Advance to the next time step and store the raw file in a new
        /// <see cref="std::vector" />.
        /// </summary>
        /// <remarks>
        /// <para>The method will swap the byte order as necessary, ie it is
        /// guaranteed that the data returned match the byte order of the
        /// system.</para>
        /// </remarks>
        /// <returns>The content of the current time step or an empty array
        /// if the new time step is invalid.</returns>
        /// <exception cref="std::invalid_argument">If the path of the current
        /// time step was invalid, ie the raw file could not be opened.
        /// </exception>
        std::vector<datraw::uint8> read_next(void) const;

        /// <summary>
        /// Resets the current time step to the begin of the sequence.
        /// </summary>
        inline void reset(void) {
            this->curTimeStep = 0;
        }

        /// <summary>
        /// Conversion to <c>bool</c>, which expresses whether the current
        /// time stamp is valid according to the <see cref="info" />.
        /// </summary>
        /// <returns><c>true</c> if the current time step is valid, <c>false</c>
        /// otherwise.</returns>
        inline operator bool(void) const {
            return (this->curTimeStep < this->datInfo.time_steps());
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
