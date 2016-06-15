#include "p18f2525.inc"

code
START org 0x6000
		movlb	0x0f		; Set memory bank to 0x0f
		return
global START

END
