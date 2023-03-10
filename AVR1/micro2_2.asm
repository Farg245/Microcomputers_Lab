.include "m16def.inc"

		.org 0x0
		rjmp RESET
		.org 0x4
		rjmp ISR1
		
		


RESET:
	.def temp = r20
	.def input =r21
	.def interupt_counter=r22
	.def timer_counter=r26
	
	clr interupt_counter
	clr timer_counter

	ldi temp,LOW(RAMEND)
	out SPL,temp
	ldi temp,HIGH(RAMEND)
	out SPH,temp

	 

    ldi r24,(1<<ISC11)|(1<<ISC10) ; mask the MCUCR for the positive edge of the interupt1
	out MCUCR,r24
	ldi r24 ,(1<<INT1)
	out GICR,r24 ; mask the GICR of the interupt1
	sei ; enable interupts

	ser temp
	out DDRC,temp ; DDRC output

loop:
	out PORTC,timer_counter ; show the timer_counter
	;ldi r24,low(100)
	;ldi r25,high(100)
	;rcall wait_msec
	inc timer_counter ; increace the timer_counter
	rjmp loop ; go again

ISR1:
	cli
	in temp, SREG ; save the System Register
	push temp
	clr temp
	out DDRA,temp ; A= input
	ser temp
	out DDRB,temp ; B = output
	
	inc interupt_counter ; increace the interupt_counter
	
	in input,PINA ; check A if you have to show it or not
	cpi input,0xC0
	brne dont_display_the_interupt_counter

	out PORTB,interupt_counter 
	
dont_display_the_interupt_counter:
	;ldi r24,low(100)
	;ldi r25,high(100)
	;rcall wait_msec
	pop temp
	out SREG,temp
	reti
