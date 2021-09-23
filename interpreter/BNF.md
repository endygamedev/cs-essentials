# Backus–Naur form

###### Short description
**Backus-Naur form (BNF)** or **Backus normal form (BNF)** is a metasyntax notation for context-free grammars, often used to describe the syntax of languages used in computing, such as computer programming languages, document formats, instruction sets and communication protocols.
###### View full in [Wiki](https://en.wikipedia.org/wiki/Backus%E2%80%93Naur_form)
<br>

There are many ways how we can describe Backus-Naur form, but we will use such grammar rules. Each rule is a name, followed by an arrow (→), followed by a sequence of symbols, and finally ending with a semicolon (;). Terminals are quoterd strings, and nonterminals are lowercase words.

**Example**
```
breakfast       →   protein "with" breakfast "on the side" ;
breakfast       →   protein ;
breakfast       →   bread ;

protein         →   crispiness "crispy" "bacon" ;
protein         →   "sausage" ;
protein         →   cooked "eggs" ;

crispiness      →   "really" ;
crispiness      →   "really" crispiness ;

cooked          →   "scrambled" ;
cooked          →   "poached" ;
cooked          →   "fried" ;

bread           →   "toast" ;
bread           →   "biscuits" ;
bread           →   "English muffin" ;
```

We can use this grammar to generate random breakfast.<br>
Let's create more suitable rules.
- Instead of repeating the rule name each time we want to add another production for it, we'll allow a series of productions separated by a pipe (`|`):<br>
`bread  →   "toast" | "biscuits" | "English muffin" ;`
- Further, we'll allow parentheses for grouping and then allow `|` within that to select one from a series of options within meddle of a production:<br>
`protein    →   ( "scrambled" | "poached" | "fried" ) "eggs" ;`
- Using recursion to support repeated sequences of symbols has a certain appealing purily, but it's kind of a chore to make a seprate named sub-rule each time we want to loop. So, we also use a postfix `*` to allow the previous symbol or group to be repeated zero or more times:<br>
`crispiness →   "really" "really"* ;`
- A postfix `+` is similar, but requires the preceding production to appear at least once:<br>
`crispiness →   "really"+ ;`
- A postfix `?` is for an optional production. The thing before it can appear zero or one time, but not more:<br>
`breakfast  →   protein ( "with" breakfast "on the side" )? ;`

With all of those syntactic niceties, our breakfast grammar condenses down to:
```
breakfast   →   protein ( "with" breakfast "on the side")?
                | bread ;

protein     →   "really" + "crispy" "bacon"
                | sausage
                | ( "scrambled" | "poached" | "fried" ) "eggs" ;

bread       →   "toast" | "biscuits" | "English muffin" ;
```
<br>

How can we implement this expression in such notation?
```
1 - (2 * 3) < 4 == false
```
We have rules:
- **Literals.** Numbers, Strings, Booleans, and `nil`.
- **Unary expressions.** A prefix `!` to perform a logic not, and - to negate a number.
- **Binary expressions.** The infix arithmetic (`+`, `-`, `*`, `/`) and logic operators (`==`, `!=`, `<`, `<=`, `>`, `>=`) we know and love.
- **Parentheses.** A pair of `(` and `)` wrapped around an expression.

Now we can use all our experience to write expression notation:
```
expression      →   literal
                    | unary
                    | binary
                    | grouping ;

literal         →   NUMBER | STRING | "true" | "false" | "nil" ;

grouping        →   "(" expression ")" ;

unary           →   ( "-" | "+" ) expression ;

binary          →   expression operator expression ;

operator        →   "==" | "!=" | "<" | "<=" | ">" | ">="
                    | "+" | "-" | "*" | "/" ;
```
