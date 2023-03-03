
#include <avr/io.h>
#include <avr/interrupt.h>


unsigned char A,B,temp;
char x,c;
volatile int flag=1;



 ISR(INT0_vect){
	
	cli();

	x=0x00;
	if((PINB &0x01) == 0x01) x++;
	if((PINB &0x02) == 0x02) x++;
	if((PINB &0x04) == 0x04) x++;
	if((PINB &0x08) == 0x08) x++;
	if((PINB &0x10) == 0x10) x++;
	if((PINB &0x20) == 0x20) x++;
	if((PINB &0x40) == 0x40) x++;
	if((PINB &0x80) == 0x80) x++; //count the pins of B

	if((PINA & 0x04) == 0x04){
		c=0x00;
		while(x>0){
			c=c<<1;
			c=c+1;
			x=x-1;
		}
		PORTC=c; // open so many leds of the C starting from the lsb as the count of B
		
	}
	else{
		PORTC=x; // output them on binary form
	}
	sei();
}

int main(){
	
	
	GICR=(1<<INT0); // be ready for the interrupt0
	MCUCR = (1<<ISC01|1<<ISC00); // wait on the positive edge of the interrupt0
	sei();
	
	DDRA = 0x00; // A input
	DDRB = 0x00; // B input
	DDRC = 0xff; // C output
	while(1){
		PORTC=0x00; // wait for the interrupt0
		}
	}
