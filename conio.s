.export   _cputc, _cgetc, gotoxy

.segment	"CODE"

_cputc: BIT $D012
        BMI _cputc
		STA $D012
		RTS

_cgetc: LDA $D011
		BPL _cgetc
		LDA $D010
		LDX #0
		RTS

; Not implemented, but referred to by cputs.s
gotoxy:	RTS
