# Numerus changelog

*******************************************************************************

v2.0.0
===============================================================================


Added
-------------------------------------------------------------------------------

1. Support for roman numerals with integer values outside of `[-3999, 3999]`
   emulating the overlined notation of characters with values multiplied
   by 1000.  Numerus uses underscores to indicate overlined parts of a numeral,
   which in this case is called _long roman numeral_.
2. Support for roman numerals with twelfths as an approximation of fractional
   parts of the value using characters `S` for `1/2` and dot `.` for `1/12`.
   The numerals in this case are called _float roman numerals_.
3. Numeral analysis functions:
    1. check if a numeral has value zero;
    2. check if a numeral is a _long numeral_;
    3. check if a numeral is a _float numeral_;
    4. count the number of roman character - and find any illegal ones.
4. Twelfth managment functions:
    1. convert a double from and to an integer value and a number of twelfths;
    2. shorten an integer value and a number of twelfts.
5. Human readability functions:
    1. prettify long roman numerals with actual overlining;
    2. prettify an integer value and a number of twelfths with shortening;
    3. error code explanation generation.
6. _Command Line Interface_ (CLI) to quickly convert any value; also interpretes
   any command line arguments (main arguments) as a CLI command, allowing easy
   pipelining of the executable in the system shell.


Changed
-------------------------------------------------------------------------------
no regex
faster 75%
all function names
error codes: more codes, more codes usage, different values, more clear names, more used, passed as parameter everywhere
more optimizations


Fixed
-------------------------------------------------------------------------------

now case insensitive
heading whitespace is ignored
malloc() fails are controlled


Removed
-------------------------------------------------------------------------------
sqlite and csv
in-memory allocation of roman numerals



v1.0.0
===============================================================================

Added
-------------------------------------------------------------------------------

- Bidirectional conversion short integer <-> roman numerals (as string)
- Syntax check of roman numerals
- Comparison of two roman numerals
- Allocation of array with all roman numerals
- Creation of SQLite3 database with all roman numerals and their values
- Custom error codes
- Documentation of all functions and variables with doxygen
- BSD 3-clause license, Changelog and Readme files
