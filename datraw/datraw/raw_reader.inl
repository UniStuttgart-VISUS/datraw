/// <copyright file="raw_reader.inl" company="Visualisierungsinstitut der Universität Stuttgart">
/// Copyright © 2017 Visualisierungsinstitut der Universität Stuttgart. Alle Rechte vorbehalten.
/// </copyright>
/// <author>Christoph Müller</author>


/*
 * datraw::raw_reader<C>::read_current
 */
template<class C>
typename datraw::raw_reader<C>::size_type datraw::raw_reader<C>::read_current(
        void *dst, const size_type cntDst) const {
    if (this->curTimeStep >= this->datInfo.time_steps()) {
        throw std::range_error("All time steps have been consumed already.");
    }

    // Compute the path to the current time step.
    auto path = this->datInfo.multi_file_name(this->curTimeStep);
    path = this->datInfo.evaluate_path(path);

    // Get the required buffer size.
    ifstream_type stream(path, ifstream_type::ate | ifstream_type::binary);
    if (!stream.good()) {
        std::stringstream msg;
        msg << "The raw file \"" << detail::narrow_string(path) 
            << "\" could not be opened." << std::ends;
        throw std::invalid_argument(msg.str());
    }
    auto retval = static_cast<size_t>(stream.tellg());

    // Read the data if possible.
    if ((dst != nullptr) && (cntDst >= retval)) {
        stream.seekg(0, ifstream_type::beg);
        stream.read(static_cast<char *>(dst), cntDst);

        if (this->datInfo.requires_byte_swap()) {
            assert(this->datInfo.format() != scalar_type::raw);
            auto ss = this->datInfo.scalar_size();
            if ((retval % ss) != 0) {
                std::stringstream msg;
                msg << "The raw file \"" << detail::narrow_string(path)
                    << "\" contains " << retval << " bytes, which is not "
                    << "divisible by the size of a scalar (" << ss << ")."
                    << std::ends;
                throw std::invalid_argument(msg.str());
            }

            datraw::swap_byte_order(ss, dst, retval / ss);
        }
    }

    stream.close();
    return retval;
}


/*
 * datraw::raw_reader<C>::read_next
 */
template<class C>
std::vector<datraw::uint8> datraw::raw_reader<C>::read_next(void) const {
    static const std::vector<datraw::uint8> EMPTY;
    if (this->move_next()) {
        return this->read_current();
    } else {
        return EMPTY;
    }
}
