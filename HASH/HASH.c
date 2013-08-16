/*
 * The executable does nothing, which makes it a viable candidate to replace REM
 * in Windows batch files (CMD and BAT). This is not recommended for batch files
 * that will be distributed to others.
 * 
 * NOTE: my original name for this was #.c, but Github does not like that name.
 * 
 * I build this with TinyCC ((http://bellard.org/tcc/):
 * 		tcc HASH.c
 * 		rename HASH.exe #.exe
 *
 * Author: Robert L. Pyron
 * License: [WTFPL - Do What the F*** You Want to Public License](http://www.wtfpl.net/)
 * 
 * Copyright © 2013 Robert L. Pyron <rpyron@alum.mit.edu
 * 
 * This program is free software. It comes without any warranty, to	 the extent
 * permitted by applicable law. You can redistribute it and/or modify it under 
 * the terms of the Do What The Fuck You Want To Public License, Version 2, 
 * as published by Sam Hocevar. See http://www.wtfpl.net/ for more details. 
 */

void main(){}
