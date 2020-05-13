/// <copyright file="variant.inl" company="Visualisierungsinstitut der Universität Stuttgart">
/// Copyright © 2017 Visualisierungsinstitut der Universität Stuttgart. Alle Rechte vorbehalten.
/// </copyright>
/// <author>Christoph Müller</author>


/*
 * trrojan::detail::copy_to::invoke
 */
template<class C, datraw::variant_type T>
void datraw::detail::copy_to<C, T>::invoke(const type& v,
        variant_type& target) {
    target.set(v);
}


/*
* trrojan::detail::is_same::invoke
*/
template<class C, datraw::variant_type T>
void datraw::detail::is_same<C, T>::invoke(type& v, const variant_type& rhs,
        bool& retval) {
    typedef detail::variant_fwd_traits<C, T> traits;
    assert(rhs.type() == T);
    // Cf. https://stackoverflow.com/questions/3505713/c-template-compilation-error-expected-primary-expression-before-token
    //retval = (v == rhs.template get<T>());
    // New version of VS2017 cannot resolve the above any more ...
    retval = (v == rhs.template get<traits::value_type>());
}


/*
 * trrojan::detail::move_to::invoke
 */
template<class C, datraw::variant_type T>
void datraw::detail::move_to<C, T>::invoke(type& v, variant_type& target) {
    target.set(std::move(v));
}



/*
 * datraw::basic_variant<C>::operator =
 */
template<class C>
datraw::basic_variant<C>& datraw::basic_variant<C>::operator =(
        const basic_variant& rhs) {
    if (this != std::addressof(rhs)) {
        rhs.conditional_invoke<detail::copy_to>(*this);
    }

    return *this;
}


/*
 * datraw::basic_variant<C>::operator =
 */
template<class C>
datraw::basic_variant<C>& datraw::basic_variant<C>::operator =(
        basic_variant&& rhs) {
    if (this != std::addressof(rhs)) {
        rhs.conditional_invoke<detail::move_to>(*this);
        rhs.clear();
        assert(rhs.empty());
    }

    return *this;
}


/*
 * datraw::basic_variant<C>::operator ==
 */
template<class C>
bool datraw::basic_variant<C>::operator ==(const basic_variant& rhs) const {
    bool retval = (this == std::addressof(rhs));

    if (!retval && this->is(rhs.cur_type)) {
        this->conditional_invoke<detail::is_same>(rhs, retval);
    }

    return retval;
}


/*
 * datraw::basic_variant<C>::conditional_invoke0
 */
template<class C>
template<template<class, datraw::variant_type> class F, datraw::variant_type T,
        datraw::variant_type... U, class... P>
void datraw::basic_variant<C>::conditional_invoke0(
        detail::variant_type_list_t<T, U...>, P&&... params) {
    typedef detail::variant_fwd_traits<C, T> traits;
    if (this->cur_type == T) {
        F<C, T>::invoke(*traits::get(this->data), std::forward<P>(params)...);
    }
    this->conditional_invoke0<F>(detail::variant_type_list_t<U...>(),
        std::forward<P>(params)...);
}
