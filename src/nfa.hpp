#pragma once

#include <vector>
#include <queue>
#include <map>
#include <numeric>

#include "bit_flagger.hpp"
#include "dfa.hpp"

namespace lexer0 {

    class nfa {
    private:
        size_type size;
        std::vector<status_type> curr_status;
        bool is_trapped;

        // true: empty
        std::vector<std::multimap<std::tuple<bool, input_type>, status_type>> trans;
        std::vector<std::vector<status_type>> inv_trans;
        std::vector<bool> accept_status;
        std::vector<bool> trap_status;

        void untrap(status_type acc_status);
        void trans_empty();

    public:
        explicit nfa(size_type);
        void add_trans(status_type from, status_type to, std::tuple<bool, input_type> v);
        void add_trans(status_type from, status_type to, input_type v);
        void add_trans(status_type from, status_type to);
        void add_accept(status_type acc_status);
        std::tuple<bool, bool> trans_on(input_type v);
        void reset();
        bit_flagger status_code() const;
        std::string to_string() const;

    };

}
