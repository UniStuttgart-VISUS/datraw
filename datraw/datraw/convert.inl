/// <copyright file="convert.inl" company="Visualisierungsinstitut der Universität Stuttgart">
/// Copyright © 2017 Visualisierungsinstitut der Universität Stuttgart. Alle Rechte vorbehalten.
/// </copyright>
/// <author>Christoph Müller</author>


/*
 * datraw::convert
 */
template<class T, class I, class O>
void datraw::convert(I begin, I end , O dst) {
    while (begin != end) {
        *dst++ = static_cast<T>(*begin++);
    }
}


/*
 * datraw::swap_byte_order
 */
template<> void datraw::swap_byte_order<8>(void *data, const size_t cnt) {
    auto idata = static_cast<std::uint64_t *>(data);
    for (size_t i = 0; i < cnt; i++) {
        auto v = idata[i];
        auto sv = (v & 0x00000000000000FFULL);
        sv = ((v & 0x000000000000FF00ULL) >> 0x08) | (sv << 0x08);
        sv = ((v & 0x0000000000FF0000ULL) >> 0x10) | (sv << 0x08);
        sv = ((v & 0x00000000FF000000ULL) >> 0x18) | (sv << 0x08);
        sv = ((v & 0x000000FF00000000ULL) >> 0x20) | (sv << 0x08);
        sv = ((v & 0x0000FF0000000000ULL) >> 0x28) | (sv << 0x08);
        sv = ((v & 0x00FF000000000000ULL) >> 0x30) | (sv << 0x08);
        sv = ((v & 0xFF00000000000000ULL) >> 0x38) | (sv << 0x08);
        idata[i] = sv;
    }
}


/*
 * datraw::swap_byte_order
 */
template<> void datraw::swap_byte_order<4>(void *data, const size_t cnt) {
    auto idata = static_cast<std::uint32_t *>(data);
    for (size_t i = 0; i < cnt; i++) {
        auto v = idata[i];
        auto sv = (v & 0x000000FF);
        sv = ((v & 0x0000FF00) >> 0x08) | (sv << 0x08);
        sv = ((v & 0x00FF0000) >> 0x10) | (sv << 0x08);
        sv = ((v & 0xFF000000) >> 0x18) | (sv << 0x08);
        idata[i] = sv;
        idata[i] = sv;
    }
}


/*
 * datraw::swap_byte_order
 */
template<> void datraw::swap_byte_order<2>(void *data, const size_t cnt) {
    auto idata = static_cast<std::uint16_t *>(data);
    for (size_t i = 0; i < cnt; i++) {
        auto v = idata[i];
        auto sv = (v & 0x00FF);
        sv = ((v & 0xFF00) >> 0x08) | (sv << 0x08);
        idata[i] = sv;
    }
}


/*
 * datraw::swap_byte_order
 */
void datraw::swap_byte_order(const size_t width, void *data,
        const size_t cnt) {
    switch (width) {
        case 8: swap_byte_order<8>(data, cnt); break;
        case 4: swap_byte_order<4>(data, cnt); break;
        case 2: swap_byte_order<2>(data, cnt); break;
        default: swap_byte_order<0>(data, cnt); break;
    }
}
