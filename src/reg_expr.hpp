#pragma once

#include <type_traits>

namespace lexer0 {

    class reg_expr {
    public:
        virtual ~reg_expr() = default;

        virtual std::size_t get_size() const = 0;
    };

    class terminate_expr : public virtual reg_expr {
    public:
        std::size_t get_size() const override;
    };

    class repeat_expr : public virtual reg_expr {
    public:
        explicit repeat_expr(reg_expr* item);
        ~repeat_expr() override;

        reg_expr* rep_item{nullptr};
        std::size_t get_size() const override;
    };

    class cat_expr : public virtual reg_expr {
    public:
        cat_expr(reg_expr* i1, reg_expr* i2);
        ~cat_expr() override;

        reg_expr* cat_item1{nullptr}, *cat_item2{nullptr};
        std::size_t get_size() const override;

        template<typename... T>
        static reg_expr* get_cat(reg_expr* ft, T...);
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
        std::size_t get_size() const override;

        template<typename... T>
        static reg_expr* get_or(reg_expr* ft, T...);
    };

    template<typename... T>
    lexer0::reg_expr *lexer0::or_expr::get_or(lexer0::reg_expr *ft, T... ts) {
        if constexpr (sizeof...(ts) == 0) {
            return ft;
        } else {
            return new cat_expr{ft, or_expr::get_or(ts...)};
        }
    }

}
