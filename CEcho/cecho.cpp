// cecho.cpp by thomas_polaert@yahoo.fr
//
// This source code is provided "as-is" under The Code Project Open License (CPOL)
// http://www.codeproject.com/info/cpol10.aspx
// 
#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
#include <windows.h>
#include <stdlib.h>
#include <stdio.h>

// action declarations
typedef void (*Fct)();

void printStack();
void resetStack();
void setColor();
void printEscChar();
void printHexChar();
void parseColor();

#define MAX_TRANS 5
#define MAX_STACK 0x200

// expected tokens list
wchar_t* Grammar[][MAX_TRANS] = {
	{L"{", L""},
	{L"{}", L"\\", L"0123456789abcdefABCDEF", L" \t", L""},
	{L"tn", L"u"},
	{L"0123456789abcdefABCDEF"},
	{L"0123456789abcdefABCDEF", L" \t", L"}", L"\\"},
	{L"0123456789abcdefABCDEF", L""},
	{L"}", L""},
	{L"}", L""}
};

// action executed on state changed
Fct Action[][MAX_TRANS] = {
	{&printStack,0},
	{&resetStack,0,0,0,0},
	{&printEscChar,0},
	{0},
	{0,&printHexChar,&printHexChar,&printHexChar},
	{0,0},
	{&setColor,0},
	{&parseColor,0}
};

// next state Ids
char Successor[][MAX_TRANS] = {
	{1,0},
	{0,2,5,1,7},
	{1,3},
	{4},
	{4,1,0,2},
	{6,7},
	{0,7},
	{0,7}
};

// available colors
wchar_t* Color[18] = {
	L"black",
	L"navy",
	L"green",
	L"teal",
	L"maroon",
	L"purple",
	L"olive",
	L"silver",
	L"gray",
	L"blue",
	L"lime",
	L"aqua",
	L"red",
	L"fuschia",
	L"yellow",
	L"white",
	L"default",
	L"#"
};

// token stack
wchar_t tokStack[MAX_STACK];
// token stack cursor
int tokCurs = 0;
// standard output handle
HANDLE hConsole;
// default colors
WORD defaultTxtAttr;

// color info can be provided as a sentence, e.g. "Red on gray"
// keywords 'default' or '#' refers to initial colors.
// if not specified, background is black
void parseColor()
{
	short clr, shift = 0;
	WORD txtAttr = 0;
	wchar_t* token;
	wchar_t* sep = L" \t";

	tokStack[tokCurs-1] = 0;
	token = wcstok(tokStack, sep);
	while (token != 0)
	{
		for (clr = 0; clr < 18; clr++)
		{
			if (!_wcsicmp(token, Color[clr]))
			{
				if (clr > 15)
					txtAttr += defaultTxtAttr & (15 << shift);
				else
					txtAttr += clr << shift;
				shift += 4;
				break;
			}
		}
		token = wcstok(0, sep);
	}
	if (shift == 0)
		wprintf(L"<Invalid color: '%s'>", tokStack);
	else
		SetConsoleTextAttribute(hConsole, (txtAttr & 0x00ff));
}

// print the token stack content
void printStack()
{
	DWORD numCharsWritten;
	tokStack[tokCurs-1] = 0;
	WriteConsoleW(hConsole, tokStack, (DWORD)wcslen(tokStack), &numCharsWritten, NULL);
}

void resetStack()
{
	// {{ is the escape sequence for '{'
	if (tokStack[tokCurs-1] == L'{')
		printf("{"); 
}

void setColor()
{
	tokStack[tokCurs-1] = 0;
	SetConsoleTextAttribute(hConsole, (WORD)wcstol(tokStack, 0, 16));
}

void printEscChar()
{
	switch (tokStack[tokCurs-1])
	{
	case L't': printf("\t"); break;
	case L'n': printf("\n"); break;
	}
}

void printHexChar()
{
	DWORD numCharsWritten;
	long chr;
	tokStack[tokCurs] = 0;
	chr = wcstol(tokStack + 2, 0, 16);
	WriteConsoleW(hConsole, &chr, 1, &numCharsWritten, NULL);
}

int wmain(int argc, wchar_t* argv[])
{
	short trans, clr, fired;
	wchar_t* token;
	char state = 0;
	CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
	OSVERSIONINFO osvi;

	// save initial colors
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	if (hConsole == INVALID_HANDLE_VALUE || hConsole == NULL)
		return -1;

	if (GetConsoleScreenBufferInfo(hConsole, &consoleInfo) != FALSE)
		defaultTxtAttr = consoleInfo.wAttributes;

	// Print help text
	if (argc < 2 || !wcscmp(argv[1], L"/?"))
	{
		SetConsoleTextAttribute(hConsole, 10);

		printf(
			"\tcecho v2.0\n"
			"\n"
			"echo command with colors support\n" 
			"by thomas_polaert@yahoo.fr\n"
			);
		SetConsoleTextAttribute(hConsole, defaultTxtAttr);

		printf(
			"\n"
			"Usage: cecho <text_to_output>\n"
			"	{{		= Escape character '{'\n"
			"	{\\n\\t}		= New line - tab\n"
			"	{\\u0007}	= Unicode character code\n"
			"	{0A}		= Color code\n"
			"	{red on gray}	= Color name\n"
			"	{#}		= Restore initial color\n"
			"\n"
			"Available colors:\n"
			);
		for (clr = 0; clr < 8; clr++)
			printf("\t%X = %-7ls\t%X = %ls\n", clr, Color[clr], clr+8, Color[clr+8]);

		return -1;
	}

	// retrieve command argument
	LPWSTR input = GetCommandLineW();

	if (*input == L'"')
		input += 2;

	input += wcslen(argv[0]) + 1;

	// starting with Windows Vista, there's an additional space between exe name and 1st argument
	osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	if (GetVersionEx(&osvi) && osvi.dwMajorVersion >= 6)
		input++;

	// parse input string
	for (fired = 0, token = input; *token != 0; token++)
	{
		// does token trigger a transition ?
		for (trans = 0; trans < MAX_TRANS && Grammar[state][trans] != 0; trans++)
		{
			// check if token belongs to the expected tokens list ? 
			// a empty list acts as a wildchar token
			if (wcschr(Grammar[state][trans], *token) || *Grammar[state][trans] == 0)
			{
				// push token into the stack
				if (tokCurs < MAX_STACK)
					tokStack[tokCurs++] = *token;

				// execute the action associated to the transition (if any)
				if (Action[state][trans] != 0)
				{
					(*Action[state][trans])();
					tokCurs = 0; // reset token stack
				}
				// update parser state
				state = Successor[state][trans];
				fired = 1;
				break;
			}
		}
		if (fired == 0)
		{
			wprintf(L"Syntax error: '%c' col %i", *token, input - token);
			return -1;
		}
	}
	// print remaing token stack
	tokCurs++;
	printStack();
	return 0;
}
