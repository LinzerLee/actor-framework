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

#include "caf/exception.hpp"
#include "caf/scheduler.hpp"
#include "caf/blocking_actor.hpp"

#include "caf/detail/logging.hpp"
#include "caf/detail/singletons.hpp"
#include "caf/detail/actor_registry.hpp"

namespace caf {

void blocking_actor::await_all_other_actors_done() {
    detail::singletons::get_actor_registry()->await_running_count_equal(1);
}

void blocking_actor::quit(uint32_t reason) {
    planned_exit_reason(reason);
    throw actor_exited(reason);
}

void blocking_actor::functor_based::create(blocking_actor*, act_fun fun) {
    m_act = fun;
}

void blocking_actor::functor_based::act() {
    CAF_LOG_TRACE("");
    m_act(this);
}

} // namespace caf
