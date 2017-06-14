/// <copyright file="convert.h" company="Visualisierungsinstitut der Universität Stuttgart">
/// Copyright © 2017 Visualisierungsinstitut der Universität Stuttgart. Alle Rechte vorbehalten.
/// </copyright>
/// <author>Christoph Müller</author>

#pragma once

#include <cinttypes>


namespace datraw {

    template<size_t T> inline void swap_byte_order(void *data, const size_t cnt) { }

    template<> void swap_byte_order<64>(void *data, const size_t cnt);

    template<> void swap_byte_order<32>(void *data, const size_t cnt);

    template<> void swap_byte_order<16>(void *data, const size_t cnt);


} /* end namespace datraw */

#include "datraw/convert.inl"
