#pragma once

#include <type_traits>
#include <utility>
#include <vector>
#include <queue>
#include <map>

namespace lexer0 {

    using index_type = std::size_t;
    using size_type = index_type;
    using status_type = std::size_t;
    using input_type = int;

    class dfa {
        friend class nfa;
    private:
        // status size
        size_type size;
        // current status of dfa
        status_type curr_status;
        /* flag, indicating whether there is possible path from
            current state to accepting status */
        bool is_trapped;

        // transition edge for every status on designated input
        std::vector<std::map<input_type, status_type>> trans;
        /* the possible status that might transimit to current
            status in 1 step */
        std::vector<std::vector<status_type>> inv_trans;
        // accepting status
        std::vector<bool> accept_status;
        /* indicating whether there is possible path from the 
            current status to accepting status*/
        std::vector<bool> trap_status;

        // updating the *trap_status* given the accepting status
        void untrap(status_type);

    public:
        explicit dfa(size_type);
        /**
         * @brief Create a transition edge from status *from*
         *  to status *to* on input *v*.
        */
        void add_trans(status_type, status_type, input_type);
        /**
         * @brief Make status *acc_status* the accepting status.
        */
        void add_accept(status_type);
        /**
         * @brief Feed a input character to the dfa, the return-
         * ing value is 
         * <
         *  Is the dfa at the accepting status, 
         *  Is there no possible path to the accepting status
         * >.
        */
        std::tuple<bool, bool> trans_on(input_type);
        /**
         * @brief Reset the dfa.
        */
        void reset();
        /**
         * @brief Get the current status code
        */
        [[nodiscard]] status_type status_code() const;
        /**
         * Get the description
         * @return description
         */
        [[nodiscard]]std::string to_string() const;
    };

}

