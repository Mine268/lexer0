# lexer0

`lexer0`是一个简单的正则文法识别器，通过构造有限状态自动机实现了正则文法的识别。本文介绍了`lexer0`的使用方法。

`lexer0`主要通过模板语法静态地来构造正则表达式，然后动态地生成识别该正则表达式的确定性有限状态自动机，再动态地使用这个自动机对字符串进行识别。

## 正则表达式的构造

使用模板进行构造，主要使用的语句是`using`。`lexer0`提供了四种基础的正则类模板，基于这些基础的模板可以构造复杂的正则表达式：

- `t_terminate_expr<int Termination>`：表示作为终结符存在的正则表达式
- `t_repeat_expr<typename Reg>`：表示将某个正则表达式重复零或多次的正则表达式
- `t_cat_expr<typename... Regs>`：表示将多个正则表达式拼接而成的正则表达式
- `t_or_expr<typename... Regs>`：表示多个表达式均可出现的正则表达式
- `t_exist_not_expr<typename Reg>`：表示某个正则表达式出现或不出现的正则表达式

若希望匹配终结符串`abc`，构造对应的正则表达式的方法为

```cpp
using REG = t_cat_expr<
    t_terminate_expr<'a'>,
    t_terminate_expr<'b'>,
    t_terminate_expr<'c'>>
```

如上例所示，如果希望构造匹配单个终结字符的正则表达式，可以使用`t_termiante_expr`将字符作为模板实参写入即可，这也是一切正则表达式构造的起点。

同理，如果希望构造「重复正则表达式/`*`正则」或者「存在正则表达式/`?`正则」，则可以使用`t_repeat_expr<typename Reg>`或者`t_exist_not_expr<typename Reg>`，将正则表达式对应的类型作为模板形参填入即可。

如果希望构造「连接正则表达」式或者「或正则表达式/`|`正则」，可以使用`t_cat_expr<typename... Regs>`或这`t_or_expr<typename... Regs>`，将需要连接/或的正则表达式（两个或以上）依次作为可变模板参数写入即可。

例如，正则表达式`a(b|(cd)*)k?`的构造方法为

```cpp
using REG = t_cat_expr<
    t_terminate_expr<'a'>,
    t_or_expr<
        t_terminate_expr<'b'>,
        t_repeat_expr<
            t_cat_expr<
                t_terminate_expr<'c'>,
                t_terminate_expr<'d'>
            >
        >
    >,
    t_exist_not_expr<t_terminate_expr<'k'>>
>;
```

可以通过正则表达式构造识别这个正则表达式的自动机

```cpp
auto fa = lexer0::t_get_nfa(); // 从正则构造NFA
auto bad_fa = fa.get_dfa(); // NFA确定化为DFA
auto good_fa = bad_fa.get_optimize(); // DFA最小化
```

## 识别字符串

使用提供的`t_lexer`类，可以构造针对字符串进行识别的词法分析器。`t_lexer`同时匹配多个正则表达式，并取其中匹配结果最长的正则表达式的匹配结果作为一个 token 输出，token 包含如下的信息

- `token_id`：该 token 的标识号，表示该 token 由哪一个正则表达式识别，为构造`t_lexer`时提供的正则表达式的序号
- `toekn_start`：这个 token 在字符串中的起始位置
- `token_length`：这个 token 的长度
- `token_string`：这个 token 的内容

然后从这个 token 的后一个字符继续开始匹配。

`t_lexer`的声明如下

```cpp
template<typename... Reg>
class t_lexer;
```

其中`Reg...`为这个词法分析器识别的所有正则表达式。

例如，构造一个识别

1. C 语言标准的标识符
2. 非负整数
3. 非负浮点数
4. 空格

的词法分析器的代码如下

```cpp
// 正则表达式的定义略，具体见"t_reg_expr.hpp"文件
t_lexer<t_c_identifier_reg,
    t_integer_reg,
    t_float_reg,
    t_blank_reg>
lexer;
```

调用`t_lexer::lexer(const std::string&)`识别字符串

```cpp
auto ts = lexer("flag1 28 9e4");
for (auto& t : ts)
    std::cout << ts.to_string() << std::endl;
```

识别出的 token 流为

```
[0,0,5,flag1]
[3,5,1, ]
[1,6,2,28]
[3,8,1, ]
[2,9,3,9e4]
```