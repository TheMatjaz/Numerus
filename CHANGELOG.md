# Changelog #

v2.0.0
===============================================================================


Added
----------------------------------------

1. Support for roman numerals with integer values outside of `[-3999, 3999]`
   emulating the overlined notation of characters with values multiplied
   by 1000.  Numerus uses underscores to indicate overlined parts of a numeral,
   which in this case is called _long roman numeral_.
2. Support for roman numerals with twelfths as an approximation of fractional
   parts of the value using characters `S` for `1/2` and dot `.` for `1/12`.
   The numerals in this case are called _float roman numerals_.
3. More constants indicating the extremes of the value range for different 
   types of roman numerals.
4. Numeral analysis functions:
    1. check if a numeral has value zero;
    2. check if a numeral is a _long numeral_;
    3. check if a numeral is a _float numeral_;
    4. count the number of roman character - and find any illegal ones.
5. Twelfth managment functions:
    1. convert a double from and to an integer value plus a number of twelfths;
    2. shorten an integer value and a number of twelfts.
6. Human readability functions:
    1. prettify a long roman numerals with actual overlining;
    2. prettify an integer value and a number of twelfths with shortening;
    3. error code explanation generation.
7. _Command Line Interface_ (CLI) to quickly convert any value; also interpretes
   any command line arguments (main arguments) as a CLI command, allowing easy
   pipelining of the executable in the system shell.


Changed
----------------------------------------

1. Roman numerals' syntax check is performed without regexes, making it ~75%
   faster, while making the conversion function support the new roman numeral
   formats as well.
2. Because of the previous point, it's possible to catch a more precise error
   when the syntax is wrong.
3. Error codes are used much much more, they have different (clearer) names
   and better values overall. They are returned as a passed parameter pretty
   much everywhere, making the user able to understand the exact problem.
4. Function names have changed to a more understandable version.
5. All code is completely rewritten, carefully optimized and (trying to be)
   foolproof.


Fixed
----------------------------------------

1. All roman numerals are now case insensitive.
2. Roman numerals are initially checked if NULL or empty. Any leading 
   whitespace is ignored.
3. `malloc()` failures are controlled and returned in the error code.


Removed
----------------------------------------

1. SQLite3 and CSV export functions
2. In-memory allocation of all roman numerals



v1.0.0
===============================================================================

Added
----------------------------------------

1. Bidirectional conversion short integer <-> roman numerals (as string)
2. Syntax check of roman numerals
3. Comparison of two roman numerals
4. Allocation of array with all roman numerals
5. Creation of SQLite3 database with all roman numerals and their values
6. Custom error codes
7. Documentation of all functions and variables with doxygen
8. BSD 3-clause license, Changelog and Readme files
