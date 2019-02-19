Lispcal
======
A calculator using a lisp-like syntax
------

`Lispcal` basically does what you think it should do when handling arithmatic expressions in the syntax of lisp.

All input constant should be positive.

Usage
------

To end the program, call `exit` or `quit`.

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

Also, `ans` and `ditto` cites the answer from the last calculation.

If the input expression have more than one value, then they will be shown as a list and `ans` or `ditto` will be set to the last value.

Errors
------

Every run-time error shall be reported (the reporting mechanism should be improved, though). However, this excludes the wrong placement of parentheses since lisp syntax is unambiguous without parentheses.

Unknown identifiers will return errors. However, this will not terminate the program. By contrast, a RAM error (unsuccessful allocation of memory) terminates the program immediately with exit code `1`.
