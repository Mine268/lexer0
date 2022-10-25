#include "reg_expr.hpp"
#include "nfa.hpp"
#include <string>
#include <iostream>
#include <map>

using namespace lexer0;

template<typename>
void test_nfa();

template<typename>
void test_reg();

template<typename>
void test_nfa2dfa();

int main() {
    test_nfa2dfa<void>();
//    test_reg<void>();
//    test_nfa<void>();
    return 0;
}

template<typename>
void test_nfa2dfa() {
    // ab(xy)*(w|zt)*k
    reg_expr* r =
            cat_expr::get_cat(
                    new terminate_expr{'a'},
                    new terminate_expr{'b'},
                    new repeat_expr{
                            new cat_expr{
                                    new terminate_expr{'x'},
                                    new terminate_expr{'y'}
                            }
                    },
                    new repeat_expr{
                            or_expr::get_or(
                                    new terminate_expr{'w'},
                                    new cat_expr{
                                            new terminate_expr{'z'},
                                            new terminate_expr{'t'}
                                    }
                            )
                    },
                    new terminate_expr{'k'}
            );
    auto fa = reg_expr::get_nfa(r).get_dfa();
    std::cout << fa.to_string() << std::endl;
    delete r;

    std::string str;
    std::cin >> str;
    for (auto c : str) {
        auto res = fa.trans_on(c);
        std::cout << "after [" << c << "], ";
        if (std::get<0>(res)) {
            std::cout << "accept: ";
        } else if (std::get<1>(res)) {
            std::cout << "trap: ";
        } else {
            std::cout << "read: ";
        }
        std::cout << fa.status_code() << std::endl;
    }
}

template<typename = void>
void test_reg() {
    // ab(xy)*(w|zt)*k
    reg_expr* r =
            cat_expr::get_cat(
                    new terminate_expr{'a'},
                    new terminate_expr{'b'},
                    new repeat_expr{
                            new cat_expr{
                                    new terminate_expr{'x'},
                                    new terminate_expr{'y'}
                            }
                    },
                    new repeat_expr{
                            or_expr::get_or(
                                    new terminate_expr{'w'},
                                    new cat_expr{
                                            new terminate_expr{'z'},
                                            new terminate_expr{'t'}
                                    }
                            )
                    },
                    new terminate_expr{'k'}
            );
    auto fa = reg_expr::get_nfa(r);
    std::cout << fa.to_string() << std::endl;
    delete r;

    std::string str;
    std::cin >> str;
    for (auto c : str) {
        auto res = fa.trans_on(c);
        std::cout << "after [" << c << "], ";
        if (std::get<0>(res)) {
            std::cout << "accept: ";
        } else if (std::get<1>(res)) {
            std::cout << "trap: ";
        } else {
            std::cout << "read: ";
        }
        std::cout << fa.status_code().to_string() << std::endl;
    }
}

template<typename = void>
void test_nfa() {
    // a(xy)*(z|w)*b
    lexer0::nfa nfa0{6};
    nfa0.add_trans(0, 1, 'a');
    nfa0.add_trans(1, 2, 'x');
    nfa0.add_trans(2, 1, 'y');
    nfa0.add_trans(1, 3);
    nfa0.add_trans(3, 4, 'z');
    nfa0.add_trans(3, 4, 'w');
    nfa0.add_trans(4, 3);
    nfa0.add_trans(3, 5, 'b');
    nfa0.add_accept(5);

    std::string str;
    std::cin >> str;
    for (auto c : str) {
        auto res = nfa0.trans_on(c);
        std::cout << "after [" << c << "], ";
        if (std::get<0>(res)) {
            std::cout << "accept: ";
        } else if (std::get<1>(res)) {
            std::cout << "trap: ";
        } else {
            std::cout << "read: ";
        }
        std::cout << nfa0.status_code().to_string() << std::endl;
    }
}