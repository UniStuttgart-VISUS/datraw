/// <copyright file="variant.inl" company="Visualisierungsinstitut der Universität Stuttgart">
/// Copyright © 2017 Visualisierungsinstitut der Universität Stuttgart. Alle Rechte vorbehalten.
/// </copyright>
/// <author>Christoph Müller</author>


/*
 * datraw::variant::conditional_invoke0
 */
template<template<datraw::variant_type> class F, datraw::variant_type T,
    datraw::variant_type... U, class... P>
void datraw::variant::conditional_invoke0(detail::variant_type_list_t<T, U...>,
        P&&... params) {
    if (this->cur_type == T) {
        F<T>::invoke(*detail::variant_fwd_traits<T>::get(this->data),
            std::forward<P>(params)...);
    }
    this->conditional_invoke0<F>(detail::variant_type_list_t<U...>(),
        std::forward<P>(params)...);
}
