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
        msg << "The raw file \"" << info_type::narrow_string(path)
            << "\" could not be opened." << std::ends;
        throw std::invalid_argument(msg.str());
    }
    auto retval = stream.tellg();

    // Read the data if possible.
    if ((dst != nullptr) && (cntDst >= retval)) {
        stream.seekg(0, ifstream_type::beg);
        stream.read(static_cast<char *>(dst), cntDst);
    }

    stream.close();
    return retval;
}
