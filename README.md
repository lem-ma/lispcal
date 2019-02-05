lispcal
======
A calculator using a lisp-like syntax
------

So, basically `lispcal` does what you think it should do when handling arithmatic expressions in the syntax of lisp.

Usage
------

To end the program, use `exit` or `quit`.

The following functions are available:

`+ add plus`

`- minus`

`* times mult`

`/ div`

`^ pow`

`% mod`

`sin`

`cos`

`tan`

`asin`

`acos`

`atan`

`sinh`

`cosh`

`tanh`

`exp`

`sqrt`

`ln`

`log`

`ceil`

`floor`

`abs`

They are used in whatever ways you expect them to be used.

Predefined constant:

`pi`

`e`

`gamma`

with the maximum precision that `double` type floating-point numbers allows.

Also, `ans` cites the answer from the last calculation.

Errors
------

Every incurred error shall be reported (the reporting mechanism should be improved, though). However, this excludes the wrong placement of parentheses since lisp syntax is unambiguous without parentheses.
