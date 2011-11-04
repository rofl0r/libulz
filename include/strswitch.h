#ifndef STRSWITCH_H
#define STRSWITCH_H

#define STRSWITCH_LETTERS_UC 'A': case 'B': case 'C': case 'D': case 'E': case 'F': case 'G': case 'H': case 'I': \
			     case 'J': case 'K': case 'L': case 'M': case 'N': case 'O': case 'P': case 'Q': case 'R': \
			     case 'S': case 'T': case 'U': case 'V': case 'W': case 'X': case 'Y': case 'Z'

#define STRSWITCH_LETTERS_LC 'a': case 'b': case 'c': case 'd': case 'e': case 'f': case 'g': case 'h': case 'i': \
			     case 'j': case 'k': case 'l': case 'm': case 'n': case 'o': case 'p': case 'q': case 'r': \
			     case 's': case 't': case 'u': case 'v': case 'w': case 'x': case 'y': case 'z'

#define STRSWITCH_LETTERS STRSWITCH_LETTERS_UC: case STRSWITCH_LETTERS_LC

#define STRSWITCH_NUMBERS '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9'

#define STRSWITCH_ALPHA STRSWITCH_LETTERS_UC: case STRSWITCH_LETTERS_LC: case STRSWITCH_NUMBERS

#define STRSWITCH_MATH_OPS '*': case '/': case '+': case '-': case '%': case '='

#define STRSWITCH_NON_ALPHA STRSWITCH_MATH_OPS: case ' ': case '@': case ',': case ';': case '.': case ':': case '_':  case '!': \
			     case '?': case '"': case '&': case '(': case ')': case '[': case ']': case '{': case '}': \
			     case '<': case '>': case '\\'
				     
#define STRSWITCH_NON_ALPHA_C STRSWITCH_MATH_OPS: case ' ': case '@': case ',': case ';': case '.': case ':': case '_':  case '!': \
			     case '&': case '(': case ')': case '[': case ']': case '{': case '}': \
			     case '<': case '>'
				     
#define STRSWITCH_PRINTABLE STRSWITCH_ALPHA: case STRSWITCH_NON_ALPHA
#define STRSWITCH_C_PRINTABLE STRSWITCH_ALPHA: case STRSWITCH_NON_ALPHA_C

#endif
