#include "nfa.hpp"

namespace lexer0 {

    void nfa::reset() {
        curr_status.clear();
        curr_status.push_back(0);
        trans_empty();
        is_trapped = std::reduce(curr_status.begin(),
                                 curr_status.end(),
                                 true,
                                 [&](auto res, auto ix) {
                                     return res && trap_status.at(ix);
                                 });
    }

    nfa::nfa(size_type n) : size{n}, curr_status{0},
                            is_trapped{true}, trans(n),
                            inv_trans(n), accept_status(n, false),
                            trap_status(n, true)
    {
        trans_empty();
    }

    void nfa::add_accept(status_type acc_status) {
        accept_status.at(acc_status) = true;
        untrap(acc_status);
        is_trapped = std::reduce(curr_status.begin(),
                                 curr_status.end(),
                                 true,
                                 [&](auto res, auto ix) {
                                     return res && trap_status.at(ix);
                                 });
    }

    void nfa::add_trans(status_type from, status_type to, std::tuple<bool, input_type> v) {
        if (std::get<0>(v)) {
            std::get<1>(v) = input_type{};
        }
        auto& trans_it = trans.at(from);
        auto tmp_edge_it = trans_it.find(v);
        if (tmp_edge_it == trans_it.end() || tmp_edge_it->second != to) {
            trans_it.insert(std::make_pair(v, to));
            std::reduce(inv_trans.at(to).begin(),
                        inv_trans.at(to).end(),
                        false,
                        [&](auto res, auto inv_ix) {
                return res || (inv_ix == from);
            }) ? 0 : (inv_trans.at(to).push_back(from), 0);
            trans_empty();
        }
    }

    void nfa::untrap(status_type acc_status) {
        std::queue<status_type> q;
        q.push(acc_status);
        while (!q.empty()) {
            auto top = q.front();
            q.pop();
            for (auto prev : inv_trans.at(top)) {
                if (trap_status.at(prev)) {
                    trap_status.at(prev) = false;
                    q.push(prev);
                }
            }
        }
    }

    void nfa::trans_empty() {
        std::queue<status_type> q;
        bit_flagger flagger {size, false};
        for (auto c : curr_status) {
            q.push(c);
        }
        while (!q.empty()) {
            auto top = q.front();
            q.pop();
            flagger.set(top, true);
            for (auto &[key, val] : trans.at(top)) {
                if (std::get<0>(key) && !flagger.get(val)) {
                    q.push(val);
                    curr_status.push_back(val);
                }
            }
        }
    }

    bit_flagger nfa::status_code() const {
        bit_flagger ret{this->size, false};
        for (auto ix : curr_status) {
            ret.set(ix, true);
        }
        return ret;
    }

    std::tuple<bool, bool> nfa::trans_on(input_type v) {
        if (is_trapped) {
            curr_status.clear();
            return std::make_tuple(false, is_trapped);
        }

        std::vector<status_type> next_status;
        for (auto curr : curr_status) {
            auto& trans_map = trans.at(curr);
            for (auto &[input_tup, ns] : trans_map) {
                if (!std::get<0>(input_tup) && std::get<1>(input_tup) == v) {
                    next_status.push_back(ns);
                }
            }
        }
        curr_status = std::move(next_status);
        trans_empty();
        is_trapped = curr_status.empty() ||
                std::reduce(curr_status.begin(),
                            curr_status.end(),
                            true,
                            [&](auto res, auto ix) {
                    return res & trap_status.at(ix);
                });
        return std::make_tuple(
                std::reduce(curr_status.begin(),
                            curr_status.end(),
                            false,
                            [&](auto res, auto ix) {
                    return res | accept_status.at(ix);
                }),
                is_trapped
                );
    }

    void nfa::add_trans(status_type from, status_type to, input_type v) {
        this->add_trans(from, to, std::make_tuple(false, v));
    }

    void nfa::add_trans(status_type from, status_type to) {
        this->add_trans(from, to, std::make_tuple(true, input_type{}));
    }
}