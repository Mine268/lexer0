#include "dfa.hpp"

namespace lexer0 {
    dfa::dfa(size_type st) : trans(st), inv_trans(st),
                             accept_status(st, false), trap_status(st, true),
                             curr_status{0}, is_trapped{true}, size{st} {}

    void dfa::add_trans(status_type from, status_type to, input_type v) {
        auto trans_it = trans.at(from).find(v);
        if (trans_it == trans.at(from).end()) {
            trans.at(from).insert(std::make_pair(v, to));
            inv_trans.at(to).push_back(from);
        }
    }

    void dfa::add_accept(status_type acc_status) {
        accept_status.at(acc_status) = true;
        untrap(acc_status);
        is_trapped = trap_status[curr_status];
    }

    std::tuple<bool, bool> dfa::trans_on(input_type input) {
        if (is_trapped) {
            return std::make_tuple(false, true);
        }
        auto& trans_map = trans.at(curr_status);
        auto next_trans_it = trans_map.find(input);
        bool is_accept = false;
        if (next_trans_it == trans_map.end()) {
            is_trapped = true;
        } else {
            curr_status = next_trans_it->second;
            is_accept = accept_status.at(curr_status);
            is_trapped = trap_status.at(curr_status);
        }
        return std::make_tuple(is_accept, is_trapped);
    }

    void dfa::reset() {
        curr_status = 0;
        is_trapped = false;
    }

    void dfa::untrap(status_type acc_status) {
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

    status_type dfa::status_code() const {
        return curr_status;
    }
}