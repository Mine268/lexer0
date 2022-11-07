#pragma once

#include <functional>

#include "nfa.hpp"

namespace lexer0 {

    template<int Termination>
    class t_terminate_expr {
    public:
        static constexpr std::size_t get_size();
        static void create_nfa(nfa &, status_type zero_status);
    };
    template<int Termination>
    constexpr std::size_t t_terminate_expr<Termination>::get_size() {
        return 2;
    }
    template<int Termination>
    void t_terminate_expr<Termination>::create_nfa(lexer0::nfa &fa, lexer0::status_type zero_status) {
        fa.add_trans(zero_status, zero_status + 1, Termination);
    }

    template<typename Regex>
    class t_repeat_expr {
        using Check = decltype((Regex::get_size, Regex::create_nfa, int{}));
    public:
        static constexpr std::size_t get_size();
        static void create_nfa(nfa &, status_type zero_status);
    };
    template<typename Regex>
    constexpr std::size_t t_repeat_expr<Regex>::get_size() {
        return Regex::get_size() + 2;
    }
    template<typename Regex>
    void t_repeat_expr<Regex>::create_nfa(lexer0::nfa &fa, lexer0::status_type zero_status) {
        std::size_t zero_r{zero_status + 1}, acc_r{zero_r + Regex::get_size() - 1}, acc{acc_r + 1};
        Regex::create_nfa(fa, zero_r);
        fa.add_trans(zero_status, zero_r);
        fa.add_trans(zero_r, acc);
        fa.add_trans(acc_r, acc);
        fa.add_trans(acc, zero_r);
    }

    template<typename... Regex>
    class t_cat_expr {
        static_assert(sizeof...(Regex) >= 2, "Provide more than 1 regex.");
    public:
        static constexpr std::size_t get_size();
        static void create_nfa(nfa &, status_type zero_status);
    private:
        template<typename FReg, typename... Regs>
        static void create_nfa_recur(nfa &, status_type zero_status);
    };
    template<typename... Regex>
    constexpr std::size_t t_cat_expr<Regex...>::get_size() {
        return (Regex::get_size() + ... + 1) - sizeof...(Regex);
    }
    template<typename... Regex>
    void t_cat_expr<Regex...>::create_nfa(nfa &fa, status_type zero_status) {
        create_nfa_recur<Regex...>(fa, zero_status);
    }
    template<typename... Regex>
    template<typename FReg, typename... Regs>
    void t_cat_expr<Regex...>::create_nfa_recur(nfa &fa, status_type zero_status) {
        status_type acc_status = zero_status + FReg::get_size() - 1;
        FReg::create_nfa(fa, zero_status);
        if constexpr (sizeof...(Regs)) {
            create_nfa_recur<Regs...>(fa, acc_status);
        }
    }

    template<typename... Regex>
    class t_or_expr {
        static_assert(sizeof...(Regex) >= 2, "Provide more than 1 regex.");
    public:
        static constexpr std::size_t get_size();
        static void create_nfa(nfa &, status_type zero_status);
    private:
        template<typename FReg, typename... Regs>
        static void create_nfa_recur(nfa &,
                                     status_type zero_status,
                                     status_type or_zero_status,
                                     status_type or_acc_status);
    };
    template<typename... Regex>
    constexpr std::size_t t_or_expr<Regex...>::get_size() {
        return (Regex::get_size() + ... + 2);
    }
    template<typename... Regex>
    void t_or_expr<Regex...>::create_nfa(nfa &fa, status_type zero_status) {
        status_type or_zero_status, or_acc_status;
        or_zero_status = zero_status;
        or_acc_status = get_size() + or_zero_status - 1;
        create_nfa_recur<Regex...>(fa,
                                   zero_status + 1,
                                   or_zero_status,
                                   or_acc_status);
    }

    template<typename... Regex>
    template<typename FReg, typename... Regs>
    void t_or_expr<Regex...>::create_nfa_recur(nfa &fa,
                                               status_type zero_status,
                                               status_type or_zero_status,
                                               status_type or_acc_status) {
        status_type acc_status = zero_status + FReg::get_size() - 1;
        FReg::create_nfa(fa, zero_status);
        fa.add_trans(or_zero_status, zero_status);
        fa.add_trans(acc_status, or_acc_status);
        if constexpr (sizeof...(Regs)) {
            create_nfa_recur<Regs...>(fa,
                                      acc_status + 1,
                                      or_zero_status,
                                      or_acc_status);
        }
    }

}