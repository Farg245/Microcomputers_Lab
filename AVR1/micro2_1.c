#include <avr/io.h>



unsigned char A,B,C,D,NOTA,NOTB,F0,F1;

int main(void){
	DDRC=0x00;  // C= input
	DDRB=0xff;  // B= output
	while(1){
		A = PINC &0x01; // mask the  lsb
		B = PINC &0x02; // mask the 2 position
		B = B >> 1; // move it to the 1 position
		C = PINC & 0x04; // mask the 3 position
		C = C >> 2; // move it to the 1 position
		D = PINC &0x08; //mask the 4 position
		D = D >> 3; // move it to the 1 position
		NOTA = A ^ 0x01; // xor with 1 so we came out with A' and B'
		NOTB = B ^ 0x01;
		F0 = (NOTA & B)|(NOTB & C & D);
		F0 = F0 ^ 0x01; // F0'
		F1 = (A & C) & (B | D);
		F1 = F1 << 1; // move it to the second position
		F0 = F0 + F1; 
		PORTB = F0; // output to the port B
		
		
	}
}
