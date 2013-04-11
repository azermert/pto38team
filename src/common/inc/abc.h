#ifndef _UART_ABC_
#define _UART_ABC_

#define _A_ 'A'
#define _B_ 'B'
#define _C_ 'C'
#define _D_ 'D'
#define _E_ 'E'
#define _F_ 'F'
#define _G_ 'G'
#define _H_ 'H'
#define _I_ 'I'
#define _J_ 'J'
#define _K_ 'K'
#define _L_ 'L'
#define _M_ 'M'
#define _N_ 'N'
#define _O_ 'O'
#define _P_ 'P'
#define _Q_ 'Q'
#define _R_ 'R'
#define _S_ 'S'
#define _T_ 'T'
#define _U_ 'U'
#define _V_ 'V'
#define _W_ 'W'
#define _X_ 'X'
#define _Y_ 'Y'
#define _Z_ 'Z'

#define _QM_ '?'
#define _US_ '_'

#define _0_ '0'
#define _1_ '1'
#define _2_ '2'
#define _3_ '3'
#define _4_ '4'
#define _5_ '5'
#define _6_ '6'
#define _7_ '7'
#define _8_ '8'
#define _9_ '9'


#define TO_CHAR(a)	_##a##_
#define BUILD_WORD(a,b,c,d)	(TO_CHAR(d) << 24)|(TO_CHAR(c) << 16)|(TO_CHAR(b) << 8)|(TO_CHAR(a))
#define REGISTER_WID(a,b,c,d)	WID_##a##b##c##d##=BUILD_WORD(a,b,c,d)


#endif
