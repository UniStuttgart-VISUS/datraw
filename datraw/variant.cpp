/// <copyright file="variant.cpp" company="Visualisierungsinstitut der Universität Stuttgart">
/// Copyright © 2017 Visualisierungsinstitut der Universität Stuttgart. Alle Rechte vorbehalten.
/// </copyright>
/// <author>Christoph Müller</author>


#include "datraw/variant.h"

#include <cassert>
#include <cstring>
#include <memory>

#include <wchar.h>


/*
 * datraw::variant::operator =
 */
datraw::variant& datraw::variant::operator =(const variant& rhs) {
    if (this != std::addressof(rhs)) {
        rhs.conditional_invoke<detail::copy_to>(*this);
    }

    return *this;
}


/*
 * datraw::variant::operator =
 */
datraw::variant& datraw::variant::operator =(variant&& rhs) {
    if (this != std::addressof(rhs)) {
        rhs.conditional_invoke<detail::move_to>(*this);
        rhs.clear();
        assert(rhs.empty());
    }

    return *this;
}


/*
 * datraw::variant::operator ==
 */
bool datraw::variant::operator ==(const variant& rhs) const {
    bool retval = (this == std::addressof(rhs));

    if (!retval && this->is(rhs.cur_type)) {
        this->conditional_invoke<detail::is_same>(rhs, retval);
    }

    return retval;
}
