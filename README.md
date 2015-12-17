![The awesomnest roman numerals library](design/Numerus_Logo_75dpi.png)

Numerus is an open-source C library that allows conversion and manipulation of 
roman numerals.

Usage
-----

The real core of Numerus are two functions:
 
 - `numerus_short_to_roman(char *roman)` to convert a roman numeral (a string)
   to an integer with the value of the roman numeral
 - `numerus_roman_to_short(short arabic)` to convert an arabic integer to a roman numeral
 
All Numerus functions and variables start with the `numerus_` prefix, while the 
constants have `NUMERUS_` to be easily identifiable (thanks to the SQLite3 C 
API for the idea), while the static functions and variables meant just for 
internal operations in the library are prefixed `_num`.


License
-------

Numerus is released under the [BSD 3-clause license](LICENSE.md).


Credits
-------

Matjaž Guštin <dev@matjaz.it> [matjaz.it](http://matjaz.it)

Numerus is a port of my (not-really-finished but functioning) project with the
same name in Java. The goal of the project is mainly learning good programming,
source control, clean code, optimizations and documentation. For any suggestions
please contact me.

The inspirations for the conversion functions were:

 - http://stackoverflow.com/a/26723344/5292928
 - http://stackoverflow.com/a/30816418/5292928
 
