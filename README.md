# Ranger-Lang
A programming language designed while studying compilers and taking heavy inspiration from RPGLE and C grammars.
Compiles RangerLang source to assembly for a barebones VM called RangerVM.


## About
This language was my introduction to the world of compilers. 
The goal of writing this was to learn, so there's not a lot of interesting stuff implemented. 
In the future, I do want to make a full object oriented language to further my knowledge.

I started out making a dumber version of C's grammar, but decided for ease of parsing I would take RPGLE's method of code blocks (if -> endif, dow -> enddo).
I've been studying RPGLE lately and the simple grammar just makes sense as a good starting point for writing a language.

This compiler takes RangerLang source and compiles it to assembly for a barebones VM called RangerVM.

RangerLang is named after my cat that always hangs out in my room while I code.


## Features
The full context free grammar can be found in **RangerLang.ebnf**.
But, I will briefly list the sparse features I implemented:
TODO:


## Stretch Features
Every side project starts out fun, but later becomes stale, we'll see if I get here.
If not, I'm sure the next language I make will have these.

- [ ] Additional relational operators - NE, GE, LE
- [ ] Additional binary operators - &&, ||
- [ ] Unary operators - !, +, -
- [ ] Additional data types - float, string
- [ ] Arrays
- [ ] Pointers
- [ ] Functions - Definitely not going to have the mental bandwidth at the moment



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

  