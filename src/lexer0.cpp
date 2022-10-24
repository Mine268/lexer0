#include "reg_expr.hpp"
#include <string>
#include <iostream>
#include <map>

int main() {

    lexer0::reg_expr* r = lexer0::cat_expr::get_cat(
            new lexer0::terminate_expr{},
            lexer0::or_expr::get_or(
                    new lexer0::terminate_expr{},
                    new lexer0::repeat_expr{new lexer0::terminate_expr{}}
                    ),
            new lexer0::terminate_expr{}
            );
    std::cout << r->get_size() << std::endl;
    delete r;

//    lexer0::nfa nfa0{6};
//    nfa0.add_trans(0, 1, 'a');
//    nfa0.add_trans(1, 2, 'x');
//    nfa0.add_trans(2, 1, 'y');
//    nfa0.add_trans(1, 3);
//    nfa0.add_trans(3, 4, 'z');
//    nfa0.add_trans(3, 4, 'w');
//    nfa0.add_trans(4, 3);
//    nfa0.add_trans(3, 5, 'b');
//    nfa0.add_accept(5);
//
//    std::string str;
//    std::cin >> str;
//    for (auto c : str) {
//        auto res = nfa0.trans_on(c);
//        std::cout << "after [" << c << "], ";
//        if (std::get<0>(res)) {
//            std::cout << "accept: ";
//        } else if (std::get<1>(res)) {
//            std::cout << "trap: ";
//        } else {
//            std::cout << "read: ";
//        }
//        std::cout << nfa0.status_code().to_string() << std::endl;
//    }

    return 0;
}
