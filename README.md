![The awesomnest roman numerals library](design/Numerus_Logo_75dpi.png)


Numerus is an open-source C library that allows conversion and manipulation of 
roman numerals.

- [GitHub repository](https://github.com/TheMatjaz/Numerus/)
- [Web documentation](http://thematjaz.github.io/Numerus/)

Roman numerals?
---------------

Roman numerals are the numeric system used in ancient Rome based on combinations
of letters from the Latin alphabet to signify
values. [Wikipedia](https://en.wikipedia.org/wiki/Roman_numerals) has an
excellent article on them. For a shorter explanation on how they work, check
the [Numerus syntax page](http://thematjaz.github.io/Numerus/md_SYNTAX.html).


Usage
-----
 
### 1. Download
 
[Download](https://github.com/TheMatjaz/Numerus/releases) a zip/tarball of
the library and unpack it. On a Unix-like OS, you can also use a command line
downloader and extract them directly with:

```sh
wget -qO - https://github.com/TheMatjaz/Numerus/archive/v2.0.0.tar.gz | tar -zx ; cd Numerus-*
```

```sh
curl -fsSL https://github.com/TheMatjaz/Numerus/archive/v2.0.0.tar.gz | tar -zx ; cd Numerus-*
```

Alternatively clone the whole git repository:

```sh
git clone https://github.com/TheMatjaz/Numerus ; cd Numerus
```


### 2. Using Numerus' CLI

Then, if you desire to use Numerus directly with its build-in command line
interface (CLI), just compile it with [`cmake`](http://cmake.org/), `make` and
run it. On a Unix-like OS, do it with the following command in the Numerus root
folder:

```sh
mkdir -p bin ; cd bin ; cmake .. ; make ; ./numerus ; cd -
```

_Hint:_ type the `help` command in the CLI for assistance

_Hint 2:_ check the
[Numerus syntax page](http://thematjaz.github.io/Numerus/md_SYNTAX.html) for
rules on how to write roman numerals.


### 3. Using Numerus in your project

Otherwise to use it as a library, add it to your project, include the
[Numerus header file](http://thematjaz.github.io/Numerus/numerus_8h.html) and
you're good to go!

```C
#include "numerus.h"
```

You'll probably need the **conversion functions**: those are the first 6 functions
in the
[Numerus header file](http://thematjaz.github.io/Numerus/numerus_8h.html). There
are some [Numerus library usage examples](http://thematjaz.github.io/Numerus/md_USAGE_EXAMPLES.html)
too! Check the
[Numerus syntax page](http://thematjaz.github.io/Numerus/md_SYNTAX.html) for
rules on how to write roman numerals and why Numerus uses some strange names
such as _long numeral_.

Be sure to check out the
[web documentation](http://thematjaz.github.io/Numerus/), where every function,
parameter, constant and
[error code](http://thematjaz.github.io/Numerus/numerus__error__codes_8h.html)
is explained. The same documentation is written within the code (thanks
[Doxygen](http://www.doxygen.org)!).


License
-------

Numerus is released under the [BSD 3-clause license](LICENSE.md) which
basically allows you to do anything with it as long as you credit the original
author (more or less).


Credits
-------

Matjaž Guštin <dev@matjaz.it> [matjaz.it](http://matjaz.it). For **any**
suggestions please contact me.

Numerus is a port of my (not-really-finished but functioning-ish) project
[with the same name](https://github.com/TheMatjaz/jNumerus) in Java. The goal of
that project was mainly learning good programming, source control, clean code,
optimizations and documentation - and the same goal applied here as well.

The inspirations for the conversion functions were:

 - http://stackoverflow.com/a/26723344/5292928
 - http://stackoverflow.com/a/30816418/5292928
 
The [SQLite3 library](http://sqlite.org/) offered many ideas on keeping the
code clean and creating a nice API. 
