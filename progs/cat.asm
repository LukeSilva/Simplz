:start
	NOR @_ff
	ADD @IN
	STR @OUT
	JPC @start
	JPC @start

:_ff
	LIT $ff
