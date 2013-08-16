Bob Pyron's Command-Line Utilities
==================================

A miscellaneous collection of command-line tools, mostly written (or "improved")
by myself.

These are mostly written in `Python`. Some of these programs are written 
in `C`; these are generally small enough to compile with 
[`TinyCC`](http://bellard.org/tcc/), so I don't bother with makefiles, 
Visual Studio project files, or any of that other unnecessary folderol. 

## `HASH` (should be `#`)

NOTE: My original name for this was `#.exe`, but Github does not like that 
name. The source file is checked in to Github as `HASH.c`.

`#.exe` lets me start comment lines with `#` instead of `REM` in Windows `CMD` 
or `BAT` files. This is not recommended for files that will be distributed to 
others. 

-   Author: [Robert L. Pyron](mailto:rpyron@alum.mit.edu)
-   License: [WTFPL - Do What the F* You Want to Public License](http://www.wtfpl.net/)

## `which`

`WHICH.PY` scans through all directories specified in the system `%PATH%` 
environment variable, looking for the specified `COMMAND`. It tries to follow 
the sometimes bizarre rules for Windows command lookup. 

-   Author: [Robert L. Pyron](mailto:rpyron+which@gmail.com)
-   License: [MIT License](http://opensource.org/licenses/MIT)

## `CEcho`

`CECHO` is a replacement for the built-in `echo` command; 
it adds color support.

I have made no modifications to this program. It's here because I like it.

-   Author: Thomas Polaert
-   CodeProject source: [Add Colors to Batch Files](http://www.codeproject.com/Articles/17033/Add-Colors-to-Batch-Files)
-   License: [The Code Project Open License (CPOL)](http://www.codeproject.com/info/cpol10.aspx)


