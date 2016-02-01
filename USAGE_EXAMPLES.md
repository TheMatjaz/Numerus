# Numerus usage example #

First of all, include the library header file:

```C
#include "numerus.h"
```

Convert a double to roman numeral with error checking.
-------------------------------------------------------------------------------

Create a value and an error code variable. Pass both to the conversion function
`numerus_double_to_roman()` (pass a pointer to the error code). If the error 
code is NUMERUS_OK, the conversion was successfull, otherwise something bad 
happened, like your value was outside the allowed range or `malloc()` failed.

`numerus_explain_error()` can help to understand the error, since it prints
an explanation of the error for humans.

Remember to `free()` the generated roman numeral one you don't need it anymore.

```C
double value = 12.25;
int errcode;
char *roman = numerus_double_to_roman(value, &errcode);
if (errcode != NUMERUS_OK) {
    printf("Conversion failed: %s\n", numerus_explain_error(errcode));
} else {
    printf("%f -> %s\n", value, roman);
    free(roman); /* Always free() unnecessary allocated strings!!! */
}
```


Convert an integer value to roman numeral without error checking
-------------------------------------------------------------------------------
**Please** be a good programmer and **always check your errors!**. If you are
lazy and your program explodes, don't blame me.

Anyways, error codes can be `NULL` to ignore any error. Just pass the value to a
conversion function, `numerus_int_to_roman()` in this case, and get the result.
In case of error, the generated roman numeral will be `NULL`. If you dislike 
error codes, you may always check this.

```C
char *roman = numerus_int_to_roman(-100, NULL);
   /* At this point roman is NULL if anything failed in the conversion */
printf("%s\n", roman);
free(roman); /* Freeing a null pointer breaks a lot of stuff! */
```


Convert roman numeral to an integer with some twelfths with error checking and pretty printing
-------------------------------------------------------------------------------

In case of error, the generated int_part will be bigger than the maximum 
 
Create a roman numeral, a variable to hold a number of twelfths and an error 
code variable. Pass all of them to the conversion function
`numerus_roman_to_int_part_and_twelfths()`.

If the error code is NUMERUS_OK, the conversion was successfull, otherwise 
something bad happened, like your roman numeral had incorrect syntax (an illegal
character, illegal character order, missing underscore, ...). In that case the 
returned long integer_part will be bigger than the maximum value a roman numeral
may have, NUMERUS_MAX_VALUE. If you dislike error codes, you may always check 
this. `numerus_explain_error()` can help to understand the error, since it 
prints an explanation of the error for humans.

Remember to `free()` the generated roman numeral one you don't need it anymore.
 
```C
int errcode;
short twelfths;
char *roman = "-_MMVI_XCIIS...";
long integer_part = numerus_roman_to_int_part_and_twelfths(roman, &twelfths, 
                                                                  &errcode);
if (errcode != NUMERUS_OK) {
    printf("Conversion failed: %s\n", numerus_explain_error(errcode));
} else {
    char *pretty_value = numerus_create_pretty_value_as_parts(integer_part, 
                                                              twelfths);
    if (pretty_value == NULL) {
        printf("%s\n", numerus_explain_error(NUMERUS_ERROR_MALLOC_FAIL));
    } else {
        printf("%s -> %s\n", roman, pretty_value);
        free(roman); /* Always free() unnecessary allocated strings!!! */
        free(pretty_value);
    }
}
```
For **any** other explanation, please contact me - my contact is in the 
[readme file](http://thematjaz.github.io/Numerus/).
