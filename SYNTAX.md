# Syntax and nomenclature #

Numerus nomenclature
===============================================================================

- **Roman char** or 
  **Roman character**:
      a character from the latin alphabet that represents a numeric value, for 
      instance `C` means `100`.
- **Numeral** or
  **Roman numeral**:
      a sequence of roman characters that have to be summed to represent a more
      complex value.
- **Value**:
      usually this word indicates the value a roman numeral represents
- **Integer part**:
      the integer part of a value is the part before the decimal separator 
      (usually the dot or comma) or a value without considerating the twelfths.
- **Long numeral**:
      a Numerus syntax to indicate numerals that hold integer values outside 
      `[-3999, 3999]` that are written with underscores representing the 
      overlined notation - where overlined characters have their values 
      multiplied by 1000.
- **Float numeral**:
      a Numerus syntax to indicate numerals that hold a non-zero number of 
      twelfths as well, making them approximate a floating point value.
- **CLI**:
    stands for _Command Line Interface_, a Numerus shell where any roman 
    numeral can be typed to be converted to a value or the other way around.


Roman numerals syntax (briefly)
===============================================================================

This is a short explanation on how roman numerals work. For a different
one, check the 
[Wikipedia article](https://en.wikipedia.org/wiki/Roman_numerals) on them.


Character values
----------------------------------------

Roman numerals are sequences of characters from the latin alphabet and each
represents a different value:

| Char | Value |
|------|-------|
|   M  |  1000 |
|   D  |   500 |
|   C  |   100 |
|   L  |    50 |
|   X  |    10 |
|   V  |     5 |
|   I  |     1 |
|   S  |   1/2 |
|   .  |  1/12 |

When a character with a smaller value is placed before a character with
bigger value (except for `S` and `.`), they have to be interpreted together 
as one and their value have to be computed by subtraction _bigger - smaller_,
resulting in:

| Char | Value |
|------|-------|
|  CM  |   900 |
|  CD  |   400 |
|  XC  |    90 |
|  XL  |    40 |
|  IX  |     9 |
|  IV  |     4 |

The rules to read and write roman numerals are:

1. When a character with smaller value stand before a character with bigger
   value, subtract the smaller from the bigger, as shown above.
2. For everything else, sum from the left to the right.

An example:

```
CDLXXXVIIS...  =     CD      +  L   +    XXX     +  V  +  II   +  S   +  ...
                    /  \        |       / | \       |     |\      |      /|\
               =  500 - 100  +  50  +  10+10+10  +  5  +  1+1  +  1/2 +  3/12
               
               =  487 + 3/4  =  487.75
```


Standard roman numeral syntax
----------------------------------------

The syntax of a standard roman numeral requires a specific sequence of 
characters as in the following points:

- 0-3 M
- 0-1 CM or 0-1 CD or ( 0-1 D and 0-3 C )
- 0-1 XC or 0-1 XL or ( 0-1 L and 0-3 X )
- 0-1 IX or 0-1 IV or ( 0-1 V and 0-3 I )

Please notice that there are no `S` or `.` in it. This allows writing integer 
values in `[1, 3999]`. In a regex form it would be:

```
M{0,3}(CM|CD|D?C{0,3})(XC|XL|L?X{0,3})(IX|IV|V?I{0,3})
```



Numerus syntax extensions
===============================================================================

Numerus add more syntax rules, while keeping compatibility with the standard
roman numeral syntax, with the goal of writing numerals with more values.


Zero
----------------------------------------

Numerus uses the string `NULLA` (as stored in the global constant variable 
`NUMERUS_ZERO`) for the numeral of value 0 (zero). The word _nulla_ means 
_nothing_ in latin.

This allows writing integer values in `[0, 3999]`.


Negative values
----------------------------------------

Numerus uses the minus `-` sign at the beginning of a roman numeral to indicate
that its value is negative. This allows writing integer values in
`[-3999, 3999]`.

An example:

```
 II       =     2
-II       =    -2
-MMCCCXLV = -2345
-NULLA    =     0

```


Long numerals
----------------------------------------

In ancient Rome, positive values bigger than 3999 were indicated with an 
overlining, called _vinculum_. The overlined characters have their value 
multiplied by 1000.

An example:

```
___
MIIXVI = 1002016
```

Numerus adopted this syntax with a difference: instead of actually overlining
the characters, making them impossible to parse, encloses the characters that
_should_ be overlined with _underscores_ `_`. This allows a simple record of
characters with values that have to be multiplied by 1000. Creating an actually
overlined numeral (a two-line string with underscores in the first line) is
possible with the numeral prettifying function.

These are called _long numerals_ in the Numerus nomenclature, resembling the
name of a `long int` value they represent. This allows writing integer values in
`[-3999999, 3999999]`.

Long numerals have have the following syntax:

- the overlined part (between underscores) has standard roman numeral syntax 
- the non-overlined part (after the underscores) has standard roman numeral 
  syntax without `M`

An example:

```
 _MII_XVI = 1002016
-_IV_I    =   -4001
 _MCMLII_ = 1952000
```

Float numerals
----------------------------------------

Roman numerals provide fractional values in _twelfths (1/12)_ using the 
character central dot `•` to indicate a _twelfth_. Numerus uses the normal dot 
(period) `.` instead for simplicity. The character `S` is used to indicate one 
_half (6/12)_ - `S` stands for _semis_, which means _half_ in latin.

Numerus implements that feature allowing a fractional part in all roman numerals
(except `NULLA`) just by adding `S` or `.` at the end of the numeral.  These are
called _float numerals_ in the Numerus nomenclature, resembling the name of a
`float` value they approximate. This allows writing all twelfths values in
`[-3999999.91666, 3999999.91666]`.

Float numerals have have the following syntax:

- at the end of any roman numeral append `S` to indicate a fractional part of
  one half (6/12) or more, if that is the case;
- append up to five `.` to indicate additional 1 to 5 twelfths.
- instead of indicating 12/12, switch to the next roman numeral instead of
  appending another twelfth.

Please note that a float numeral can be at the same time a long numeral and
vice-versa. The nomenclature is a pure formality to indicate the capability of
the numeral.


Regex matching them all
---------------------------------------

Just for the documenation a regex matching all long, float, negative and 
standard roman numerals but not (`-`)`NULLA` is:

```
-?((_M{0,3}(CM|CD|D?C{0,3})(XC|XL|L?X{0,3})(IX|IV|V?I{0,3})_)|M{0,3})(CM|CD|D?C{0,3})(XC|XL|L?X{0,3})(IX|IV|V?I{0,3})S?\\.{0,5}
```

