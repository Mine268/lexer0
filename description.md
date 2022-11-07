 
# lexer0

`lexer0`是一个简单的正则文法识别器，通过构造有限状态自动机实现了正则文法的识别。

主要实现的三个类
- `nfa`：非确定性有限状态自动机类
- `dfa`：确定性有限状态自动机类
- `reg_expr`：正则表达式构造类

## reg_expr

正则表达式构造类，提供类四种正则表达式的构造入口，通过构造正则表达式，`reg_expr`生成对应的`nfa`对象，从而实现类识别正则表达式的目的。四种正则表达式分别是：
1. 终结字符
2. 正则表达式连接
3. 正则表达式重复
4. 正则表达式选择

如下是一个正则表达式`abw*(p|s(k|zt)*)m`的一个构造示例
```cpp
using REG = t_cat_expr<
        t_terminate_expr<'a'>,
        t_terminate_expr<'b'>,
        t_repeat_expr<t_terminate_expr<'w'>>,
        t_or_expr<
                t_terminate_expr<'p'>,
                t_cat_expr<
                        t_terminate_expr<'s'>,
                        t_repeat_expr<
                                t_or_expr<
                                        t_terminate_expr<'k'>,
                                        t_cat_expr<
                                                t_terminate_expr<'z'>,
                                                t_terminate_expr<'t'>
                                        >
                                >
                        >
                >
        >,
        t_terminate_expr<'m'>
>;
```

构造完成之后，调用`lexer0::t_get_nfa`获得生成的非确定性有限状态自动机对象（`nfa`对象）。
```cpp
auto the_nfa = lexer0::t_get_nfa<REG>();
```

`nfa`提供了转换为等价的确定性有限状态自动机（`dfa`对象）的方法`nfa::get_dfa`，调用之获得确定花的非确定性有限状态自动机。
```cpp
auto the_dfa = r.get_dfa();
```

在获得类确定性有限状态自动机后可以调用`dfa::get_optimize`进行最小化。
```cpp
auto the_optim_dfa = the_dfa.get_optimize();
```

## dfa / nfa

`dfa`和`nfa`的使用只需要注意两点：
1. 接受某一个输入之后的返回值
2. 重置`dfa`/`nfa`的状态
3. 获取`dfa`/`nfa`的状态

两者的使用方式类似，后文不做区分，这里设`fa`为一个`dfa`/`nfa`对象。

### 接受输入后的返回值

通过`dfa::trans_on`/`nfa::trans_on`方法接受输入，例如输入`23`并转移的方法为
```cpp
auto res_tup = fa.trans_on(23);
```

返回一个`std::tuple<bool, bool>`对象，其中第一个元素表示接受输入之后`fa`是否转移到类接受状态，第二个元素表示接受输入之后`fa`接下来的所有可能转移到的状态中是否存在接受状态。

前者的含义不必说明，后者用来表示继续向下转移是否有意义，用于进行**贪婪搜索**。

### 重置 dfa/nfa 状态

调用`dfa::reset`/`nfa::reset`重置状态及的当前状态为起始状态，以重新开始新一轮的匹配，如
```cpp
fa.reset();
```

### 获取 dfa/nfa 状态

调用`dfa::status_code`/`nfa::status_code`获取当前状态及所处的状态，这个状态本身没有什么含义，但是可以和这个状态机中的其他状态进行比较判别。