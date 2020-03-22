# Ranger-Lang
A programming language designed while studying compilers and taking heavy inspiration from RPGLE and C grammars.
Compiles RangerLang source to assembly for a barebones VM called RangerVM.


## About
This language was my introduction to the world of compilers. 


The goal of writing this was to learn, so there's not a lot of interesting stuff implemented. 
Additionally, I wimped out and didn't dive into functions. 
So, I did miss out on a lot of good stuff about activation records, heap management, etc.
In the future, I will make a full blown object oriented language to fill in the missing knowledge and more.

I started out making a dumber version of C's grammar, but decided for ease of parsing I would take RPGLE's method of code blocks (if -> endif, dow -> enddo).
I've been studying RPGLE lately and the simple grammar just makes sense as a good starting point for writing an introductory language.

This compiler takes RangerLang source and compiles it to assembly for a barebones VM called RangerVM.

RangerLang is named after my cat that always hangs out in my room while I code.


## Features
The full context free grammar can be found in **RangerLang.ebnf**.
But, I will briefly list the sparse features I implemented:
- [x] Integer variables
- [x] Base operators - ```+ - / * < > = :=```
- [x] If/else 
- [x] Do while
- [x] Read
- [x] Write
- [x] Basic virtual machine


## Stretch Features
Every side project starts out fun, but later becomes stale; We'll see if I get here.
If not, I'm sure the next language I make will have these and more.

- [ ] Additional relational operators - ```!=, >=, <=```
- [ ] Additional binary operators - ```&&, ||```
- [ ] Unary operators - ```!, +, -```
- [ ] Strict type definition - new keywords ```int, float, string```
- [ ] Additional data types - float, string
- [ ] Integer arrays


## Compilation Example
More examples can be found in **tests/**.

Source program (RangerLang)
TODO:
```php
...
```

Target program (RangerVM Assembly Language)
TODO:
```
...
```

Compile Listing
TODO:
```
...
```


## References
* EBNF
  * https://en.wikipedia.org/wiki/Extended_Backus%E2%80%93Naur_form
  * https://tomassetti.me/ebnf/
* Books
  * Compilers: Principles, Techniques, and Tools (The Dragon Book)
  * Engineering A Compiler
  * SubC Compiler - http://www.t3x.org/subc/
  * Crafting Interpreters - http://craftinginterpreters.com/
* Misc
  * Dangling else - https://en.wikipedia.org/wiki/Dangling_else
  * Pratt Parsing (operator precedence) - https://journal.stuffwithstuff.com/2011/03/19/pratt-parsers-expression-parsing-made-easy/
  * x86 64-bit assembly reference - https://www.felixcloutier.com/x86/

  