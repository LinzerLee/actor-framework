/******************************************************************************\
 *           ___        __                                                    *
 *          /\_ \    __/\ \                                                   *
 *          \//\ \  /\_\ \ \____    ___   _____   _____      __               *
 *            \ \ \ \/\ \ \ '__`\  /'___\/\ '__`\/\ '__`\  /'__`\             *
 *             \_\ \_\ \ \ \ \L\ \/\ \__/\ \ \L\ \ \ \L\ \/\ \L\.\_           *
 *             /\____\\ \_\ \_,__/\ \____\\ \ ,__/\ \ ,__/\ \__/.\_\          *
 *             \/____/ \/_/\/___/  \/____/ \ \ \/  \ \ \/  \/__/\/_/          *
 *                                          \ \_\   \ \_\                     *
 *                                           \/_/    \/_/                     *
 *                                                                            *
 * Copyright (C) 2011 - 2014                                                  *
 * Dominik Charousset <dominik.charousset (at) haw-hamburg.de>                *
 *                                                                            *
 * Distributed under the Boost Software License, Version 1.0. See             *
 * accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt  *
\******************************************************************************/

#include "caf/group.hpp"
#include "caf/channel.hpp"
#include "caf/message.hpp"

#include "caf/detail/singletons.hpp"
#include "caf/detail/group_manager.hpp"

namespace caf {

group::group(const invalid_group_t&) : m_ptr(nullptr) {}

group::group(abstract_group_ptr ptr) : m_ptr(std::move(ptr)) {}

group& group::operator=(const invalid_group_t&) {
    m_ptr.reset();
    return *this;
}

intptr_t group::compare(const group& other) const {
    return channel::compare(m_ptr.get(), other.m_ptr.get());
}

group group::get(const std::string& arg0, const std::string& arg1) {
    return detail::singletons::get_group_manager()->get(arg0, arg1);
}

group group::anonymous() {
    return detail::singletons::get_group_manager()->anonymous();
}

void group::add_module(abstract_group::unique_module_ptr ptr) {
    detail::singletons::get_group_manager()->add_module(std::move(ptr));
}

abstract_group::module_ptr group::get_module(const std::string& module_name) {
    return detail::singletons::get_group_manager()->get_module(module_name);
}

} // namespace caf
