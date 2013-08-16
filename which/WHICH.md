# DESCRIPTION

**`WHICH.PY`** scans through all directories specified in the system **`%PATH%`**
environment variable, looking for the specified **`COMMAND`**(s). It tries
to follow the sometimes bizarre rules for Windows command lookup.

# USAGE

    Write the full path of COMMAND(s) to standard output.
    
    Usage: WHICH [-e] [-f] [-v] [-h] COMMAND [...]
    
      -e, --exact   Print exact matches only.
      -f, --first   Print just the first match.
      -v, --version Print version and exit successfully.
      -h, --help    Print help message and exit successfully.
    
    WHICH scans through all directories specified in the system %PATH%
    environment variable, looking for the specified COMMAND(s). It tries
    to follow the sometimes bizarre rules for Windows command lookup.
    
# EASTER EGGS

I provide two shortcuts to restrict the search:

-   In general, we don't want to have a pathname specified as part of
    the **`COMMAND`**. However, I allow the syntax **`'.\COMMAND'`** to restrict
    search to current directory.
-   I also allow a shortcut to specify exactMatchOnly by appending a dot.
    For example, **`"foo.exe."`** looks for **`"foo.exe"`** and nothing else.

# DISCUSSION

I have often wanted a Windows-specific version of the Unix **`which`** command.
**GNU**-derived versions of **`which`** are useful, but incomplete. The rules
for command lookup under Windows are sometimes bizarre. This is what I have
determined through experimentation:

-   If the command name exactly matches a built-in command, the built-in takes 
    precedence. 

-   Otherwise **`CMD.EXE`** will search for the command starting in
    the current directory, then in each directory specified by the **`%PATH%`**
    environment table. At each directory in this search:
    -   If the command name has an extension:
        -   If the command extension matches an extension from the **`%PATHEXT%`**
            environment variable, then **`CMD.EXE`** will attempt to execute the file.
        -   If the command name extension is associated with a particular program,
            then that program is invoked, with this file as input.
            -   **TODO:** My program does not handle this situation.
        -   Don't give up yet. This is Windows.
    -   Finally, each extension from the **`%PATHEXT%`** environment variable will
        be appended in turn to the supplied command name. If the constructed 
        command matches one of the above rules, we have a winner!

Consider this scenario:

-   There is a program named **`verify.exe`**, somewhere on your path.
-   Unknown to you, there is also a program named **`verify.exe.py`**, 
    at another location on your path.
-   At the command prompt, you type **`verify`**. Much to your surprise, 
    **`CMD.EXE`** executes the built-in program **`VERIFY`**, which 
    is not what you want.
-   You try again: **`verify.exe`**. Again to your surprise, **`CMD.EXE`** 
    finds and executes **`verify.exe.py`**, which is also not what you want.

Granted that's not very likely, but it could happen. This is why **`WHICH.PY`**
defaults to listing every possible match it finds in the system path.

# ALTERNATIVES

The **`GnuWin32`** version of **`which.exe`** is useful, but it does not know
about **`CMD.EXE's`** built-in commands. It also does not know all of the
rules for command lookup

**`WHERE.EXE`**, available in Windows since Windows Server 2003, does not know 
about built-in commands. 
**`WHERE.EXE`** also provides too much information, because it does not 
distinguish between files that are executable and those that are not.

# TODO

-   Account for file associations.
-   Use **`Py2Exe`** to build an executable binary.
-   Create a GitHub repository for this project.

# REQUIRED MODULES

    sys os os.path argparse traceback

# TAGS

    Python Windows commandline utility which

# LICENSE AND COPYRIGHT

Copyright &copy; 2013 by Robert L. Pyron 
[(mailto:rpyron+which@gmail.com)](mailto:rpyron+which@gmail.com)

Distributed under terms of the MIT License: 
[http://opensource.org/licenses/MIT](http://opensource.org/licenses/MIT)




