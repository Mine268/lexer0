#include <cassert>

#include "reg_expr.hpp"

std::size_t lexer0::terminate_expr::get_size() const {
    return 2;
}

std::size_t lexer0::repeat_expr::get_size() const {
    assert(rep_item != nullptr);
    return rep_item->get_size() + 1;
}

lexer0::repeat_expr::~repeat_expr() {
    delete rep_item;
}

lexer0::repeat_expr::repeat_expr(lexer0::reg_expr *item) : rep_item{item} {}

lexer0::cat_expr::~cat_expr() {
    delete cat_item1;
    delete cat_item2;
}

std::size_t lexer0::cat_expr::get_size() const {
    assert(cat_item1 != nullptr && cat_item2 != nullptr);
    return cat_item1->get_size() + cat_item2->get_size() + 1;
}

lexer0::cat_expr::cat_expr(lexer0::reg_expr *i1, lexer0::reg_expr *i2) : cat_item1{i1}, cat_item2{i2} {}

lexer0::or_expr::or_expr(lexer0::reg_expr *i1, lexer0::reg_expr *i2) : or_item1{i1}, or_item2{i2} {}

lexer0::or_expr::~or_expr() {
    delete or_item1;
    delete or_item2;
}

std::size_t lexer0::or_expr::get_size() const {
    assert(or_item1 != nullptr && or_item2 != nullptr);
    return or_item1->get_size() + or_item2->get_size() + 2;
}
