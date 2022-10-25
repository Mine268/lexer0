#pragma once

#include <type_traits>

#include "nfa.hpp"

namespace lexer0 {

    class reg_expr {
    protected:
        std::size_t fa_size{0};
    public:
        /**
         * Create the NFA recognized the regular expression
         * @param fa The NFA to be created
         * @param zero_status The initial status code current NFA
         */
        virtual void create_nfa(nfa& fa, std::size_t zero_status) const = 0;
        virtual ~reg_expr() = default;
        /**
         * Return the size of the FA for this regular expression
         * @param force Force-ly update the size
         * @return
         */
        [[nodiscard]] virtual std::size_t get_size(bool force) = 0;
        /**
         * Get NFA for the regex
         * @param r Regex
         * @return NFA
         */
        static nfa get_nfa(reg_expr *r);
    };


    class terminate_expr : public virtual reg_expr {
    public:
        int terminate_char {0};
        explicit terminate_expr(int tc);
        [[nodiscard]] std::size_t get_size(bool force) override;
    protected:
        void create_nfa(nfa& fa, std::size_t zero_status) const override;
    };


    class repeat_expr : public virtual reg_expr {
    public:
        explicit repeat_expr(reg_expr* item);
        ~repeat_expr() override;
        reg_expr* rep_item{nullptr};
        [[nodiscard]] std::size_t get_size(bool force) override;
    protected:
        void create_nfa(nfa& fa, std::size_t zero_status) const override;
    };


    class cat_expr : public virtual reg_expr {
    public:
        cat_expr(reg_expr* i1, reg_expr* i2);
        ~cat_expr() override;
        reg_expr* cat_item1{nullptr}, *cat_item2{nullptr};
        [[nodiscard]] std::size_t get_size(bool force) override;
        template<typename... T>
        static reg_expr* get_cat(reg_expr* ft, T...);
    protected:
        void create_nfa(nfa& fa, std::size_t zero_status) const override;
    };

    template<typename... T>
    lexer0::reg_expr *lexer0::cat_expr::get_cat(reg_expr* ft, T... ts) {
        if constexpr (sizeof...(ts) == 0) {
            return ft;
        } else {
            return new cat_expr{ft, cat_expr::get_cat(ts...)};
        }
    }


    class or_expr : public virtual reg_expr {
    public:
        or_expr(reg_expr* i1, reg_expr* i2);
        ~or_expr() override;
        reg_expr* or_item1{nullptr}, *or_item2{nullptr};
        [[nodiscard]] std::size_t get_size(bool force) override;
        template<typename... T>
        static reg_expr* get_or(reg_expr* ft, T...);
    protected:
        void create_nfa(nfa& fa, std::size_t zero_status) const override;
    };

    template<typename... T>
    lexer0::reg_expr *lexer0::or_expr::get_or(lexer0::reg_expr *ft, T... ts) {
        if constexpr (sizeof...(ts) == 0) {
            return ft;
        } else {
            return new or_expr{ft, or_expr::get_or(ts...)};
        }
    }

}
