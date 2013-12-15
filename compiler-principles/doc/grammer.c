/* notes:
 * <cond> ::= <bool> && <bool> | <bool> || <bool> | '!' <bool> | <bool>
 * <bool> ::= <expr>
 * 如何检测语法可以正确归约?
 * bool表达式是否应该和算数表达式分开?
 * 算术表达式可以转换为bool表达式，反之则不行。
 * 下面哪种是正确的?
 * <prog> ::= <prog><func> | <prog><stmt> | <func>
 * <prog> ::= <func><prog> | <stmt><func> | <func>
 */

G[<microc>]:
<microc> ::= <prog> // accpet sign
<prog> ::= <func><prog>
<prog> ::= <stmt><prog>
<prog> ::= <func>
<if> ::= "if" <cond> <stmt> "fi"
<if> ::= "if" <cond> <stmt> "else" <stmt> "fi"
<for> ::= "for" '('<decl>';' <cond>';' <stmt>')' <stmt>
<for> ::= "for" '('<assign> ')';' <cond>';' <stmt>')' <stmt>
<while> ::= "while" (<cond>) <stmt>
<block> ::= '{' <stmts> '}'
<do> ::= "do" <stmt> "while" '('<cond>')'';'
<cond> ::= <expr>
<cond> ::= <cond> "&&" <cond>
<cond> ::= <cond> "||" <cond>
<cond> ::= '(' <cond> ')'
<cond> ::= <expr> '>' <expr>
<cond> ::= <expr> '>=' <expr>
<cond> ::= <expr> '<' <expr>
<cond> ::= <expr> '<=' <expr>
<cond> ::= <expr> '!=' <expr>
<expr> ::= <expr>+<term>
<expr> ::= <expr>-<term>
<expr> ::= -<term>
<expr> ::= <term>
<term> ::= <term>*<factor>
<term> ::= <term>/<factor>
<term> ::= <term>%<factor>
<term> ::= <factor>
<factor> ::= '(' <expr> ')'
<factor> ::= <id>
<factor> ::= <num>
<stmt> ::= <decl>';'
<stmt> ::= <assign>';'
<stmt> ::= <do>
<stmt> ::= <for>
<stmt> ::= <while>
<stmt> ::= <if>
<stmt> ::= <block>
<decl> ::= <type><assign>
<decl> ::= <type><id>
<decl> ::= <decl> ',' <id>
<assign> ::= <id> '=' <expr>
<assign> ::= <id> '=' <id>
<assign> ::= <id> '=' <num>
<stmts> ::= <stmts><stmt>
<stmts> ::= <stmt>
<func> ::= <type> <id> '(' <args> ')' <block>
<type> ::= int | double | bool
<id> ::= [a-zA-Z][a-zA-Z0-9_]*
<arg> ::= <type><id>
<args> ::= <args><arg>
<args> ::= <arg>


I0:[<microc> ::= _<prog>]:
<microc> ::= _<prog>
_<prog> ::= _<func><prog>
_<prog> ::= _<stmt><prog>
_<prog> ::= _<func>
_<func> ::= _<type><id> '(' <args> ')' <block>
_<type> ::= int | double | bool
_<stmt> ::= _<decl> ';'
_<stmt> ::= _<assign> ';'
_<stmt> ::= _<do>
_<stmt> ::= _<for>
_<stmt> ::= _<while>
_<stmt> ::= _<if>
_<stmt> ::= _<block>
_<decl> ::= _<type><assign>
_<decl> ::= _<type><id>
_<decl> ::= _<decl> ',' <id>
_<assign> ::= _<id> '=' <expr>
_<assign> ::= _<id> '=' <id>
_<assign> ::= _<id> '=' <num>
_<do> ::= "do" <stmt> "while" '(' <cond> ')' ';'
_<for> ::= "for" '(' <decl>';' <cond>';' <stmt>')' <stmt>
_<while> ::= "while" '(' <cond> ')' <stmt>
_<if> ::= "if" <cond> <stmt> "fi"
_<if> ::= "if" <cond> <stmt> "else" <stmt> "fi"
_<block> ::= '{' <stmts> '}'

I1:[I0 + <prog>]:
<microc> ::= <prog>_

I2:[I0 + <func>]:
_<prog> ::= <func>_<prog>
_<prog> ::= <func>_
_<prog> ::= _<func><prog>
_<prog> ::= _<stmt><prog>
_<prog> ::= _<func>
_<func> ::= _<type><id> '(' <args> ')' <block>
_<type> ::= int | double | bool
_<stmt> ::= _<decl> ';'
_<stmt> ::= _<assign> ';'
_<stmt> ::= _<do>
_<stmt> ::= _<for>
_<stmt> ::= _<while>
_<stmt> ::= _<if>
_<stmt> ::= _<block>
_<decl> ::= _<type><assign>
_<decl> ::= _<type><id>
_<decl> ::= _<decl> ',' <id>
_<assign> ::= _<id> '=' <expr>
_<assign> ::= _<id> '=' <id>
_<assign> ::= _<id> '=' <num>
_<do> ::= "do" <stmt> "while" '(' <cond> ')' ';'
_<for> ::= "for" '(' <decl>';' <cond>';' <stmt>')' <stmt>
_<while> ::= "while" '(' <cond> ')' <stmt>
_<if> ::= "if" <cond> <stmt> "fi"
_<if> ::= "if" <cond> <stmt> "else" <stmt> "fi"
_<block> ::= '{' <stmts> '}'

I3:[I0 + <stmt>]:
_<prog> ::= <stmt>_<prog>
_<prog> ::= _<func><prog>
_<prog> ::= _<stmt><prog>
_<prog> ::= _<func>
_<func> ::= _<type><id> '(' <args> ')' <block>
_<type> ::= int | double | bool
_<stmt> ::= _<decl> ';'
_<stmt> ::= _<assign> ';'
_<stmt> ::= _<do>
_<stmt> ::= _<for>
_<stmt> ::= _<while>
_<stmt> ::= _<if>
_<stmt> ::= _<block>
_<decl> ::= _<type><assign>
_<decl> ::= _<type><id>
_<decl> ::= _<decl> ',' <id>
_<assign> ::= _<id> '=' <expr>
_<assign> ::= _<id> '=' <id>
_<assign> ::= _<id> '=' <num>
_<do> ::= "do" <stmt> "while" '(' <cond> ')' ';'
_<for> ::= "for" '(' <decl>';' <cond>';' <stmt>')' <stmt>
_<while> ::= "while" '(' <cond> ')' <stmt>
_<if> ::= "if" <cond> <stmt> "fi"
_<if> ::= "if" <cond> <stmt> "else" <stmt> "fi"
_<block> ::= '{' <stmts> '}'

I4:[I0 + <func>]:
_<prog> ::= <func>_

I5:[I0 + <type>]:
_<func> ::= <type>_<id> '(' <args> ')' <block>
_<decl> ::= <type>_<assign>
_<assign> ::= _<id> '=' <epxr>
_<assign> ::= _<id> '=' <id>
_<assign> ::= _<id> '=' <num>
_<decl> ::= <type>_<id>

I6:[I0 + int]:
_<type> ::= int_

I7:[I0 + double]:
_<type> ::= double_

I8:[I0 + bool]:
_<type> ::= bool_

I9:[I0 + <decl>]:
_<stmt> ::= <decl>_

I10:[I0 + <assign>]:
_<stmt> ::= <assign>_

I11:[I0 + <do>]:
_<stmt> ::= <do>_

I12:[I0 + <for>]:
_<stmt> ::= <for>_

I13:[I0 + <while>]:
_<stmt> ::= <while>_

I14:[I0 + <if>]:
_<stmt> ::= <if>_

I15:[I0 + <block>]:
_<stmt> ::= <block>_

I16:[I0 + <id>]:
_<assign> ::= <id>_ '=' <expr>
_<assign> ::= <id>_ '=' <id>
_<assign> ::= <id>_ '=' <num>

I17:[I0 + "do"]:
_<do> ::= "do"_<stmt> "while" '(' <cond> ')' ';'
_<stmt> ::= _<decl> ';'
_<stmt> ::= _<assign> ';'
_<stmt> ::= _<do>
_<stmt> ::= _<for>
_<stmt> ::= _<while>
_<stmt> ::= _<if>
_<stmt> ::= _<block>
_<decl> ::= _<type><assign>
_<decl> ::= _<type><id>
_<decl> ::= _<decl> ',' <id>
_<assign> ::= _<id> '=' <expr>
_<assign> ::= _<id> '=' <id>
_<assign> ::= _<id> '=' <num>
_<do> ::= "do" <stmt> "while" '(' <cond> ')' ';'
_<for> ::= "for" '(' <decl>';' <cond>';' <stmt>')' <stmt>
_<while> ::= "while" '(' <cond> ')' <stmt>
_<if> ::= "if" <cond> <stmt> "fi"
_<if> ::= "if" <cond> <stmt> "else" <stmt> "fi"
_<block> ::= '{' <stmts> '}'
_<type> ::= _int
_<type> ::= _double
_<type> ::= _bool

I18:[I0 + "for"]:
_<for> ::= "for"_ '(' <decl> ';' <cond> ';' <stmt> ')' <stmt>

I19:[I0 + "while"]:
_<while> ::= "while" '(' <cond> ')' <stmt>

I20:[I0 + "if"]:
_<if> ::= "if" _<cond> <stmt> "fi"
_<if> ::= "if" _<cond> <stmt> "else" <stmt> "fi"
_<cond> ::= _<expr>
_<expr> ::= _<expr> '+' <term>
_<expr> ::= _<expr> '-' <term>
_<expr> ::= _ '-' <term>
_<cond> ::= _<expr> '>' <expr>
_<cond> ::= _<expr> ">=" <expr>
_<cond> ::= _<expr> "<" <expr>
_<cond> ::= _<expr> "<=" <expr>
_<cond> ::= _<expr> "!=" <expr>

I21:[I0 + '{']:
_<block> ::= '{' _<stmts> '}'
_<stmts> ::= _<stmts><stmt>
_<stmts> ::= _<stmt>
_<stmt> ::= _<decl> ';'
_<stmt> ::= _<assign> ';'
_<stmt> ::= _<do>
_<stmt> ::= _<for>
_<stmt> ::= _<while>
_<stmt> ::= _<if>
_<stmt> ::= _<block>
_<decl> ::= _<type><assign>
_<decl> ::= _<type><id>
_<decl> ::= _<decl> ',' <id>
_<assign> ::= _<id> '=' <expr>
_<assign> ::= _<id> '=' <id>
_<assign> ::= _<id> '=' <num>
_<do> ::= "do" <stmt> "while" '(' <cond> ')' ';'
_<for> ::= "for" '(' <decl>';' <cond>';' <stmt>')' <stmt>
_<while> ::= "while" '(' <cond> ')' <stmt>
_<if> ::= "if" <cond> <stmt> "fi"
_<if> ::= "if" <cond> <stmt> "else" <stmt> "fi"
_<block> ::= '{' <stmts> '}'
_<type> ::= _int | _double | _bool

I22:[I2 + <prog>]:
_<prog> ::= <func><prog>_

[I2 + <func>]::= I2
[I2 + <stmt>]::= I3
[I2 + <type>]::= I5
[I2 + <assign>]::= I10
[I2 + <do>]::= I11
[I2 + <for>]::= I12
[I2 + <while>]::= I13
[I2 + <if>]::= I14
[I2 + <block>]::= I15
[I2 + <type>]::= I5
[I2 + "do"]::= I17
[I2 + "for"]::= I18
[I2 + "while"]::= I19
[I2 + "if"]::= I20
[I2 + '{']::= I21

I23:[I2 + <decl>]:
_<decl> ::= <decl>_ ',' <id>

[I3 + <stmt>]::= I3
[I3 + <func>]::= I2
[I3 + <type>]::= I5

I24:[I3 + <decl>]:
_<stmt> ::= <decl>_ ';'

I25:[I3 + <assign>]:
_<stmt> ::= <assign>_ ';'

[I3 + <do>]::= I11
[I3 + <for>]::= I12
[I3 + <while>]::= I13
[I3 + <if>]::= I14
[I3 + <block>]::= I15
[I3 + <id>]::= I16
[I3 + "do"]::= I17
[I3 + "for"]::= I18
[I3 + "while"]::= I19
[I3 + "if"]::= I20
[I3 + '{']::= I21

I26:[I5 + <id>]:
_<func> ::= <type><id>_ '(' <args> ')' <block>
_<assign> ::= <id>_ '=' <expr>
_<assign> ::= <id>_ '=' <id>
_<assign> ::= <id>_ '=' <num>
_<decl> ::= <type><id>_

I27:[I5 + <assign>]:
_<decl> ::= <type><assign>_

I28:[I16 + '=']:
_<assign> ::= <id> '=' _<expr>
_<expr> ::= _<expr> '+' <term>
_<expr> ::= _<expr> '-' <term>
_<expr> ::= _ '-' <term>
_<assign> ::= <id> '=' _<id>
_<assign> ::= <id> '=' _<num>

I29:[I17 + <stmt>]:
_<do> ::= "do" <stmt>_ "while" '(' <cond> ')' ';'

[I17 + <decl>]::= I24
[I17 + <assign>]::= I25
[I17 + <do>]::= I11
[I17 + <for>]::= I12
[I17 + <while>]::= I13
[I17 + <if>]::= I14
[I17 + <block>]::= I15
[I17 + "do"]::= I17
[I17 + "for"]::= I18
[I17 + "while"]::= I19
[I17 + "if"]::= I20
[I17 + '{']::= I21

I30:[I18 + '(']:
_<for> ::= "for" '('_<decl> ';' <cond> ';' <stmt> ')' <stmt>
_<decl> ::= _<type><assign>
_<decl> ::= _<type><id>
_<decl> ::= _<decl> ',' <id>

I31:[I19 + '(']:
_<while> ::= "while" '('_<cond> ')' <stmt>
_<cond> ::= _<expr>
_<expr> ::= _<expr> '+' <term>
_<expr> ::= _<expr> '-' <term>
_<expr> ::= _ '-' <term>
_<cond> ::= _<expr> '>' <expr>
_<cond> ::= _<expr> ">=" <expr>
_<cond> ::= _<expr> '<' <expr>
_<cond> ::= _<expr> "<=" <expr>
_<cond> ::= _<expr> "!=" <expr>

I32:[I20 + <cond>]:
_<if> ::= "if" <cond> _<stmt> "fi"
_<if> ::= "fi" <cond> _<stmt> "else" <stmt> "fi"
_<stmt> ::= _<decl> ';'
_<stmt> ::= _<assign> ';'
_<stmt> ::= _<do>
_<stmt> ::= _<for>
_<stmt> ::= _<while>
_<stmt> ::= _<if>
_<stmt> ::= _<block>
_<decl> ::= _<type><assign>
_<decl> ::= _<type><id>
_<decl> ::= _<decl> ',' <id>
_<assign> ::= _<id> '=' <expr>
_<assign> ::= _<id> '=' <id>
_<assign> ::= _<id> '=' <num>
_<do> ::= _"do" <stmt> "while" '(' <cond> ')' ';'
_<for> ::= "for" '(' <decl> ';' <cond> ';' <stmt> ')' <stmt>
_<while> ::= "while" '(' <cond> ')' <stmt>
_<if> ::= "if" <cond> <stmt> "fi"
_<if> ::= "if" <cond> <stmt> "else" <stmt> "fi"
_<block> ::= '{' <stmts> '}'

I33:[I20 + <expr>]:
_<cond> ::= <expr>_
_<cond> ::= <expr>_ '+' <term>
_<cond> ::= <expr>_ '-' <term>
_<cond> ::= <expr>_ '>' <expr>
_<cond> ::= <expr>_ ">=" <expr>
_<cond> ::= <expr>_ '<' <expr>
_<cond> ::= <expr>_ "<=" <expr>
_<cond> ::= <expr>_ "!=" <expr>

I34:[I20 + '-']:
_<expr> ::= '-' _<term>
_<term> ::= _<term> '*' <factor>
_<term> ::= _<term> '/' <factor>
_<term> ::= _<term> '%' <factor>
_<term> ::= _<factor>
_<factor> ::= _'(' <expr> ')'
_<factor> ::= _<id>
_<factor> ::= _<num>
_<factor> ::= _<bool>
_<bool> ::= "true" | "false"

I35:[I21 + <stmts>]:
_<block> ::= '{' <stmts>_ '}'
_<stmts> ::= _<stmts><stmt>
_<stmts> ::= _<stmt>
_<stmt> ::= _<decl> ';'
_<stmt> ::= _<assign> ';'
_<stmt> ::= _<do>
_<stmt> ::= _<for>
_<stmt> ::= _<while>
_<stmt> ::= _<if>
_<stmt> ::= _<block>
_<decl> ::= _<type><assign>
_<decl> ::= _<type><id>
_<decl> ::= _<decl> ',' <id>
_<assign> ::= _<id> '=' <expr>
_<assign> ::= _<id> '=' <id>
_<assign> ::= _<id> '=' <num>
_<do> ::= "do" <stmt> "while" '(' <cond> ')' ';'
_<for> ::= "for" '(' <decl>';' <cond>';' <stmt>')' <stmt>
_<while> ::= "while" '(' <cond> ')' <stmt>
_<if> ::= "if" <cond> <stmt> "fi"
_<if> ::= "if" <cond> <stmt> "else" <stmt> "fi"
_<block> ::= '{' <stmts> '}'
_<type> ::= _int | _double | _bool

I36:[I21 + <stmt>]:
_<stmts> ::= <stmt>_

I37:[I21 + <decl>]:
_<stmt> ::= <decl>_ ';'

I38:[I21 + <assign>]:
_<stmt> ::= <assign>_ ';'

[I21 + <do>]::= I11
[I21 + <for>]::= I12
[I21 + <while>]::= I13
[I21 + <if>]::= I14
[I21 + <block>]::= I15

I39:[I21 + <type>]:
_<decl> ::= <type>_<assign>
_<decl> ::= <type>_<id>
_<assign> ::= _<id> '=' <expr>
_<assign> ::= _<id> '=' <id>
_<assign> ::= _<id> '=' <num>

[I21 + <id>]::= I16
[I21 + "do"]::= I17
[I21 + "for"]::= I18
[I21 + "while"]::= I19
[I21 + "if"]::= I20

I40:[I23 + ',']:
_<decl> ::= <decl> ',' _<id>

I41:[I24 + ';']:
_<stmt> ::= <decl> ';'_

I42:[I25 + ';']:
_<stmt> ::= <assign> ';'_

I43:[I26 + '(']:
_<func> ::= <type><id> '(' _<args> ')' <block>
_<args> ::= _<args><arg>
_<args> ::= _<arg>
_<arg> ::= _<type><id>

[I26 + '=']::= I28

I44:[I28 + <expr>]:
_<assign> ::= <id> '=' <expr>_
_<expr> ::= <expr>_'+' <term>
_<expr> ::= <expr>_ '-' <term>

[I28 + '-']::= I34

I45:[I28 + <id>]:
_<assign> ::= <id> '=' <id>_

I46:[I28 + <num>]:
_<assign> ::= <id> '=' <num>_

I47:[I29 + "while"]:
_<do> ::= "do" <stmt> "while" _ '(' <cond> ')' ';'

I48:[I30 + <decl>]:
_<for> ::= "for" '(' <decl> _ ';' <cond> ';' <stmt> ')' <stmt>

[I30 + <type>]::= I39
I49:[I31 + <cond>]:
_<while> ::= "while" '(' <cond> _ ')' <stmt>

[I31 + <expr>]::= I33
[I31 + '-']::= I34
I50:[I32 + <stmt>]:
_<if> ::= "if" <cond> <stmt>_ "fi"
_<fi> ::= "if" <cond> <stmt> _ "else" <stmt> "fi"

[I32 + <decl>]::= I24
[I32 + <assign>]::= I25
[I32 + <do>]::= I11
[I32 + <for>]::= I12
[I32 + <while>]::= I13
[I32 + <fi>]::= I14
[I32 + <block>]::= I15
[I32 + <type>]::= I39
[I32 + <decl>]::= I23
[I32 + <id>]::= I16
[I32 + "do"]::= I17
[I32 + "for"]::= I18
[I32 + "while"]::= I19
[I32 + "if"]::= I20
[I32 + '{']::= I21

I51:[I33 + '+']:
_<cond> ::= <expr> '+' _<term>
_<term> ::= _<term> '*' <factor>
_<term> ::= _<term> '/' <factor>
_<term> ::= _<term> '%' <factor>
_<term> ::= _<factor>
_<factor> ::= _ '(' <expr> ')'
_<factor> ::= _<id>
_<factor> ::= _<num>

I52:[I33 + '-']:
_<cond> ::= <expr> '-' _<term>
_<term> ::= _<term> '*' <factor>
_<term> ::= _<term> '/' <factor>
_<term> ::= _<term> '%' <factor>
_<term> ::= _<factor>
_<factor> ::= _ '(' <expr> ')'
_<factor> ::= _<id>
_<factor> ::= _<num>

I53:[I33 + '>']:
_<cond> ::= <expr> '>' _<expr>
_<expr> ::= _<expr> '+' <term>
_<expr> ::= _<expr> '-' <term>
_<expr> ::= _ '-' <term>

I54:[I33 + ">="]:
_<cond> ::= <expr> ">=" _<expr>
_<expr> ::= _<expr> '+' <term>
_<expr> ::= _<expr> '-' <term>
_<expr> ::= _ '-' <term>

I55:[I33 + '<']:
_<cond> ::= <expr> '<' _<expr>
_<expr> ::= _<expr> '+' <term>
_<expr> ::= _<expr> '-' <term>
_<expr> ::= _ '-' <term>

I56:[I33 + "<="]:
_<cond> ::= <expr> "<=" _<expr>
_<expr> ::= _<expr> '+' <term>
_<expr> ::= _<expr> '-' <term>
_<expr> ::= _ '-' <term>

I57:[I33 + "!="]:
_<cond> ::= <expr> "!=" _<expr>
_<expr> ::= _<expr> '+' <term>
_<expr> ::= _<expr> '-' <term>
_<expr> ::= _ '-' <term>

I58:[I34 + <term>]:
_<expr> ::= '-' <term>_
_<term> ::= <term>_ '*' <factor>
_<term> ::= <term>_ '/' <factor>
_<term> ::= <term>_ '%' <factor>

I59:[I34 + <factor>]:
_<term> ::= <factor>_

I60:[I34 + '(']:
_<factor> ::= '(' _<expr> ')'
_<expr> ::= _<expr> '+' <term>
_<expr> ::= _<expr> '-' <term>
_<expr> ::= _ '-' <term>

I61:[I34 + <id>]:
_<factor> ::= <id>_

I62:[I34 + <num>]:
_<factor> ::= <num>_

I63:[I35 + '}']:
_<block> ::= '{' <stmts> '}' _

I64:[I35 + <stmts>]:
_<stmts> ::= <stmts>_<stmt>
_<stmt> ::= _<decl> ';'
_<stmt> ::= _<assign> ';'
_<stmt> ::= _<do>
_<stmt> ::= _<for>
_<stmt> ::= _<while>
_<stmt> ::= _<if>
_<stmt> ::= _<block>
_<decl> ::= _<type><assign>
_<decl> ::= _<type><id>
_<decl> ::= _<decl> ',' <id>
_<assign> ::= _<id> '=' <expr>
_<assign> ::= _<id> '=' <id>
_<assign> ::= _<id> '=' <num>
_<do> ::= "do" <stmt> "while" '(' <cond> ')' ';'
_<for> ::= "for" '(' <decl>';' <cond>';' <stmt>')' <stmt>
_<while> ::= "while" '(' <cond> ')' <stmt>
_<if> ::= "if" <cond> <stmt> "fi"
_<if> ::= "if" <cond> <stmt> "else" <stmt> "fi"
_<block> ::= '{' <stmts> '}'
_<type> ::= _int | _double | _bool

[I35 + <stmt>]::= I36

[I35 + <decl>]::= I24
[I35 + <assign>]::= I25
[I35 + <do>]::= I11
[I35 + <for>]::= I12
[I35 + <while>]::= I13
[I35 + <if>]::= I14
[I35 + <block>]::= I15

I65:[I37 + ';']:
_<stmts> ::= <decl> ';' _

I66:[I38 + ';']:
_<stmt> ::= <assign> ';' _

[I39 + <assign>]::= I27
I67:[I39 + <id>]:
_<decl> ::= <type><id>_
_<assign> ::= <id> _ '=' <expr>
_<assign> ::= <id> _ '=' <id>
_<assign> ::= <id> _ '=' <num>

I68:[I40 + <id>]:
_<decl> ::= <decl> ',' <id>_

I69:[I43 + <args>]:
_<func> ::= <type><id> '(' <args> _ ')' <block>
_<args> ::= <args>_<arg>
_<arg> ::= _<type><id>
_<args> ::= _<args><arg>

I70:[I43 + <args>]:
_<args> ::= <args>_<arg>
_<arg> ::= _<type><id>

I71:[I43 + <arg>]:
_<args> ::= <arg>_

[I44 + '+']::= I33
[I44 + '-']::= I34

I73:[I47 + '(']:
_<do> ::= "do" <stmt> "while" '(' _<cond> ')' ';'
_<cond> ::= ...

I74:[I48 + ';']:
_<for> ::= "for" '(' <decl> ';' _<cond> ';' <stmt> ')' <stmt>
_<cond> ::= ...

I75:[I49 + ')']:
_<while> ::= "while" '(' <cond> ')' _<stmt>
_<stmt> ::= ...

I76:[I50 + "fi"]:
_<if> ::= "if" <cond> <stmt> "fi" _

I77:[I50 + "else"]:
_<fi> ::= "if" <cond> <stmt> "else" _<stmt> "fi"
_<stmt> ::= ...

I78:[I51 + <term>]:
_<cond> ::= <expr> '+' <term> _
_<term> ::= <term> _ '*' <factor>
_<term> ::= <term> _ '/' <factor>
_<term> ::= <term> _ '%' <factor>

[I51 + <factor>]::= I59
I79:[I51 + '(']:
_<factor> ::= '(' _<expr> ')'
_<expr> ::= ...

[I51 + <id>]::= I61
[I51 + <num>]::= I62

I80:[I52 + <term>]:
_<cond> ::= <expr> '-' <term> _
_<term> ::= <term> _ '*' <factor>
_<term> ::= <term> _ '/' <factor>
_<term> ::= <term> _ '%' <factor>

[I52 + <factor>]::= I59
[I52 + '(']::= I60
[I52 + <id>]::= I61
[I52 + <num>]::= I62
[I53 + <expr>]::= I44
[I53 + '-']::= I34
[I54 + <expr>]::= I44
[I54 + '-']::= I34
[I55 + <expr>]::= I44
[I55 + '-']::= I34
[I56 + <expr>]::= I44
[I56 + '-']::= I34
[I57 + <expr>]::= I44
[I57 + '-']::= I34

I81:[I58 + '*']:
_<term> ::= <term> '*' _<factor>
_<factor> ::= ...

I82:[I58 + '/']:
_<term> ::= <term> '/' _<factor>
_<factor> ::= ...

I82:[I58 + '%']:
_<term> ::= <term> '%' _<factor>
_<factor> ::= ...

I83:[I60 + <expr>]:
_<factor> ::= '(' <expr> _')'
_<expr> ::= <expr> _'+' <term>
_<expr> ::= <expr> _'-' <term>

[I60 + '-']::= I34

I84:[I64 + <stmt>]:
_<stmts> ::= <stmts><stmt>_

I85:[I64 + <decl>]:
_<stmt> ::= <decl> _
_<decl> ::= <decl> _ ',' <id>

[I64 + <assign>]::= I25
[I64 + <do>]::= I11
[I64 + <for>]::= I12
[I64 + <while>]::= I13
[I64 + <if>]::= I14
[I64 + <block>]::= I15
[I64 + <type>]::= I39
[I64 + <id>]::= I16
[I64 + "do"]::= I17
[I64 + "for"]::= I18
[I64 + "while"]::= I19
[I64 + "if"]::= I20
[I64 + '{']::= I21

[I67 + '=']::= I28

I86:[I69 + ')']:
_<func> ::= <type><id> '(' <args> ')' _<block>
_<block> ::= ...

I87:[I69 + <arg>]:
_<args> ::= <args><arg>_

[I69 + <args>]::= I70

I88:[I69 + <type>]:
_<arg> ::= <type>_<id>

[I70 + <arg>]::= I87
[I70 + <type>]::= I88

I89:[I73 + <cond>]:
_<do> ::= "do" <stmt> "whlie" '(' <cond> _ ')' ';'

I90:[I74 + <cond>]:
_<for> ::= "for" '(' <decl> ';' <cond> _ ';' <stmt> ')' <stmt>

I91:[I75 + <stmt>]:
_<while> ::= "while" '(' <cond> ')' <stmt> _

I92:[I77 + <stmt>]:
_<if> ::= "if" <cond> <stmt> "else" <stmt> _ "fi"

[I78 + '*']::= I81
[I78 + '/']::= I82
[I78 + '%']::= I83

I93:[I79 + <expr>]:
_<factor> ::= '(' <expr> _ ')'

[I80 + '*']::= I81
[I80 + '/']::= I82
[I80 + '%']::= I83

I94:[I81 + <factor>]:
_<term> ::= <term> '*' <factor> _
I95:[I82 + <factor>]:
_<term> ::= <term> '/' <factor> _
I95:[I83 + <factor>]:
_<term> ::= <term> '%' <factor> _

I96:[I83 + ')']:
_<factor> ::= '(' <expr> ')' _

[I83 + '+']::= I51
[I83 + '-']::= I52

I97:[I85 + ',']:
_<decl> ::= <decl> ',' _<id>

I98:[I86 + <block>]:
_<func> ::= <type><id> '(' <args> ')' <block>_

I99:[I88 + <id>]:
_<arg> ::= <type><id> _

I100:[I89 + <')']:
_<do> ::= "do" <stmt> "while" '(' <cond> ')' _ ';'

I101:[I90 + ';']:
_<for> ::= "for" '(' <decl> ';' <cond> ';' _<stmt> ')' <stmt>
_<stmt> ::= ...

I102:[I92 + "fi"]:
_<if> ::= "if" <cond> <stmt> "else" <stmt> "fi" _

I103:[I93 + ')']:
_<factor> ::= '(' <expr> ')' _

I104:[I97 + <id>]:
_<decl> ::= <decl> ',' <id> _

I105:[I100 + ';']:
_<do> ::= "do" <stmt> "while" '(' <cond> ')' ';' _

I106:[I101 + <stmt>]:
_<for> ::= "for" '(' <decl> ';' <cond> ';' <stmt> _ ')' <stmt>

I107:[I106 + ')']:
_<for> ::= "for" '(' <decl> ';' <cond> ';' <stmt>  ')' _<stmt>
_<stmt> ::= ...

I108:[I107 + <stmt>]:
_<for> ::= "for" '(' <decl> ';' <cond> ';' <stmt>  ')' <stmt> _
