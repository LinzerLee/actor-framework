/******************************************************************************
 *                       ____    _    _____                                   *
 *                      / ___|  / \  |  ___|    C++                           *
 *                     | |     / _ \ | |_       Actor                         *
 *                     | |___ / ___ \|  _|      Framework                     *
 *                      \____/_/   \_|_|                                      *
 *                                                                            *
 * Copyright (C) 2011 - 2016                                                  *
 * Dominik Charousset <dominik.charousset (at) haw-hamburg.de>                *
 *                                                                            *
 * Distributed under the terms and conditions of the BSD 3-Clause License or  *
 * (at your option) under the terms and conditions of the Boost Software      *
 * License 1.0. See accompanying files LICENSE and LICENSE_ALTERNATIVE.       *
 *                                                                            *
 * If you did not receive a copy of the license files, see                    *
 * http://opensource.org/licenses/BSD-3-Clause and                            *
 * http://www.boost.org/LICENSE_1_0.txt.                                      *
 ******************************************************************************/

#ifndef CAF_STREAM_MSG_HPP
#define CAF_STREAM_MSG_HPP

#include <utility>
#include <vector>
#include <cstdint>

#include "caf/atom.hpp"
#include "caf/message.hpp"
#include "caf/variant.hpp"
#include "caf/stream_id.hpp"
#include "caf/stream_priority.hpp"
#include "caf/actor_control_block.hpp"

#include "caf/tag/boxing_type.hpp"

#include "caf/detail/type_list.hpp"

namespace caf {

/// Stream communication messages for handshaking, ACKing, data transmission,
/// etc.
struct stream_msg : tag::boxing_type {
  /// Initiates a stream handshake.
  struct open {
    /// Allows the testing DSL to unbox this type automagically.
    using outer_type = stream_msg;
    /// A type-erased stream<T> object for picking the correct message
    /// handler of the receiving actor.
    message token;
    /// A pointer to the previous stage in the pipeline.
    strong_actor_ptr prev_stage;
    /// Configures the priority for stream elements.
    stream_priority priority;
    /// Available topics for this stream. An empty vector indicates that the
    /// upstream does provide only a single channel for this stream.
    std::vector<atom_value> topics;
    /// Tells the downstream whether rebindings can occur on this path.
    bool redeployable;
  };

  /// Acknowledges a previous `open` message and finalizes a stream handshake.
  /// Also signalizes initial demand.
  struct ack_open {
    /// Allows the testing DSL to unbox this type automagically.
    using outer_type = stream_msg;
    /// Grants credit to the source.
    int32_t initial_demand;
    /// Subscribes to a subset of the stream if non-empty. Otherwise, the
    /// upstream sends all data of the stream.
    std::vector<atom_value> filter;
    /// Tells the upstream whether rebindings can occur on this path.
    bool redeployable;
  };

  /// Transmits stream data.
  struct batch {
    /// Allows the testing DSL to unbox this type automagically.
    using outer_type = stream_msg;
    /// Size of the type-erased vector<T> (used credit).
    int32_t xs_size;
    /// A type-erased vector<T> containing the elements of the batch.
    message xs;
    /// ID of this batch (ascending numbering).
    int64_t id;
  };

  /// Cumulatively acknowledges received batches and signalizes new demand from
  /// a sink to its source.
  struct ack_batch {
    /// Allows the testing DSL to unbox this type automagically.
    using outer_type = stream_msg;
    /// Newly available credit.
    int32_t new_capacity;
    /// Cumulative ack ID.
    int64_t acknowledged_id;
  };

  /// Closes a stream after receiving an ACK for the last batch.
  struct close {
    /// Allows the testing DSL to unbox this type automagically.
    using outer_type = stream_msg;
  };

  /// Propagates fatal errors.
  struct abort {
    /// Allows the testing DSL to unbox this type automagically.
    using outer_type = stream_msg;
    /// Reason for shutting down the stream.
    error reason;
  };

  /// Send by the runtime if a downstream path failed. The receiving actor
  /// awaits a `resume` message afterwards if the downstream path was
  /// redeployable. Otherwise, this results in a fatal error.
  struct downstream_failed {
    /// Allows the testing DSL to unbox this type automagically.
    using outer_type = stream_msg;
    /// Exit reason of the failing downstream path.
    error reason;
  };

  /// Send by the runtime if an upstream path failed. The receiving actor
  /// awaits a `resume` message afterwards if the upstream path was
  /// redeployable. Otherwise, this results in a fatal error.
  struct upstream_failed {
    /// Allows the testing DSL to unbox this type automagically.
    using outer_type = stream_msg;
    /// Exit reason of the failing upstream path.
    error reason;
  };

  /// Lists all possible options for the payload.
  using content_alternatives = detail::type_list<open, ack_open, batch,
                                                 ack_batch, close, abort,
                                                 downstream_failed,
                                                 upstream_failed>;

  /// Stores one of `content_alternatives`.
  using content_type = variant<open, ack_open, batch, ack_batch, close,
                               abort, downstream_failed, upstream_failed>;

  /// ID of the affected stream.
  stream_id sid;

  /// Palyoad of the message.
  content_type content;

  template <class T>
  stream_msg(stream_id  id, T&& x)
      : sid(std::move(id)),
        content(std::forward<T>(x)) {
    // nop
  }

  stream_msg() = default;
  stream_msg(stream_msg&&) = default;
  stream_msg(const stream_msg&) = default;
  stream_msg& operator=(stream_msg&&) = default;
  stream_msg& operator=(const stream_msg&) = default;
};

/// Allows the testing DSL to unbox `stream_msg` automagically.
template <class T>
const T& get(const stream_msg& x) {
  return get<T>(x.content);
}

template <class T, class... Ts>
typename std::enable_if<
  detail::tl_contains<
    stream_msg::content_alternatives,
    T
  >::value,
  stream_msg
>::type
make(const stream_id& sid, Ts&&... xs) {
  return {sid, T{std::forward<Ts>(xs)...}};
}

template <class Inspector>
typename Inspector::result_type inspect(Inspector& f, stream_msg::open& x) {
  return f(meta::type_name("open"), x.token, x.prev_stage, x.priority,
           x.topics, x.redeployable);
}

template <class Inspector>
typename Inspector::result_type inspect(Inspector& f, stream_msg::ack_open& x) {
  return f(meta::type_name("ok"), x.initial_demand, x.filter, x.redeployable);
}

template <class Inspector>
typename Inspector::result_type inspect(Inspector& f, stream_msg::batch& x) {
  return f(meta::type_name("batch"), meta::omittable(), x.xs_size, x.xs, x.id);
}

template <class Inspector>
typename Inspector::result_type inspect(Inspector& f,
                                        stream_msg::ack_batch& x) {
  return f(meta::type_name("demand"), x.new_capacity, x.acknowledged_id);
}

template <class Inspector>
typename Inspector::result_type inspect(Inspector& f, stream_msg::close&) {
  return f(meta::type_name("close"));
}

template <class Inspector>
typename Inspector::result_type inspect(Inspector& f, stream_msg::abort& x) {
  return f(meta::type_name("abort"), x.reason);
}

template <class Inspector>
typename Inspector::result_type inspect(Inspector& f,
                                        stream_msg::downstream_failed& x) {
  return f(meta::type_name("downstream_failed"), x.reason);
}

template <class Inspector>
typename Inspector::result_type inspect(Inspector& f,
                                        stream_msg::upstream_failed& x) {
  return f(meta::type_name("upstream_failed"), x.reason);
}

template <class Inspector>
typename Inspector::result_type inspect(Inspector& f, stream_msg& x) {
  return f(meta::type_name("stream_msg"), x.sid, x.content);
}

} // namespace caf

#endif // CAF_STREAM_MSG_HPP
