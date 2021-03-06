#pragma once
#include <algorithm>
#include <limits>
#include <memory>
#include <optional>
#include <vector>

#include "core/edge.hpp"
#include "core/vertex.hpp"

namespace georhiau {
namespace algo {

// rsi prevention
template <typename T>
using edge = georhiau::core::edge<T, 2>;

template <typename T>
using vertex = georhiau::core::vertex<T, 2>;

// mate version 1 (my version)
// if the return value is cloud.end() then we have failed
template <typename T>
auto mate(const edge<T>& e, const std::vector<vertex<T>>& cloud) {
    auto best_p = cloud.end();
    T t, best_t = std::numeric_limits<T>::max();
    auto e_rot = georhiau::core::rotate(e);
    for (auto pnt = cloud.begin(); pnt != cloud.end(); ++pnt) {
        // obtain midpoint
        auto m = e.midpoint();
        // change of origin
        auto coo = *pnt - m;
        auto bisec_vec = e_rot.dest() - m;
        // project coo (change of origin) onto e_rot(ten) (90's punk rock apple
        // product)
        t = coo * bisec_vec;
        if (t <= 0) continue;  // not rhs
        if (t < best_t) {
            best_t = t;
            best_p = pnt;
        }
    }
    return best_p;
}

// mate version 2 (temp for benchmarking and testing)
// "I stole this from a book" version
template <typename T>
auto mate2(const edge<T>& e, const std::vector<vertex<T>>& cloud) {
    auto best_p = cloud.end();
    T t, best_t = std::numeric_limits<T>::max();
    auto e_rot = georhiau::core::rotate(e);
    for (auto pnt = cloud.begin(); pnt != cloud.end(); ++pnt) {
        // escape early if we are not on the right.
        if (georhiau::core::classify(e.orig(), e.dest(), *pnt) !=
            vertex<T>::orientation::Right) {
            continue;
        }
        edge<T> g = rotate(edge<T>{e.dest(), *pnt});

        // we don't want a mate that is parallel or colinear
        auto [type, t] = georhiau::core::intersect(e_rot, g);
        if (type == edge<T>::intersection::Parallel ||
            type == edge<T>::intersection::Colinear) {
            continue;
        }
        //
        if (t < best_t) {
            best_p = pnt;
            best_t = t;
        }
    }
    return best_p;
}

}  // namespace algo
}  // namespace georhiau

