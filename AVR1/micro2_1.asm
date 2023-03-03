;
; microlab2_1.asm
;
; 
; 
;


; Replace with your application code
.INCLUDE "m16def.inc"

.def input = r18
.def temp = r19
.def temp2=r17
.def A = r20
.def B = r21
.def C = r22
.def D = r23
.def F0 = r24
.def F1 = r25


main:
	clr temp 
	out DDRC ,temp
	ser temp
	out DDRB,temp

	in input , PINC ; take the input from the PINC
	mov A,input 
	andi A,0x01 ; mask the lsb

	mov B,input 
	andi B,0x02 ; mask the 2nd lsb
	ror B ; move it to the 1 position

	mov C,input
	andi C,0x04 ; mask the 2nd lsb
	ror C ; move it to the 1 position
	ror	C

	mov D,input
	andi D,0x08 ; mask the 4th lsb
	ror D ; move it to the 1 position
	ror D
	ror D
	
	mov temp2,B
	com temp2 ; create the B'
	andi temp2,0x01 ;temp2=B'
	
	mov temp,A
	com temp ; create the A'
	andi temp,0x01; temp=A'
	
	
	and temp,B ; temp=A'B
	and temp2,C ; temp2=B'C
	and temp2,D ; temp2= B'CD
	or temp,temp2 ; temp=A'B+B'CD
	mov F0,temp

	mov temp,A
	and temp,C ; temp =AC

	mov temp2,B
	or temp2,D ; temp2 = B+D
	and temp,temp2 ; temp =(AC)(B+D)
	mov F1,temp
	clc
	rol F1 ; move it to the 2nd position
	com F0 ; create the F0'
	andi F0,0x01 ; mask the first position
	add F0,F1
	out PORTB,F0 ; show it to the PORTB
	rjmp main
end:




