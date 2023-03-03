/*
 * ask5_1.c
 *
 * Created: 11/1/2022 6:38:45 μμ
 * Author : jonny
 */ 

#include <avr/io.h>
#define SPARK_DELAY_TIME 20

#define F_CPU 8000000UL
#include "avr/io.h"
#include <util/delay.h>
//OC0 is connected to pin PB3
//OC1A is connected to pin PD5
//OC2 is connected to pin PD7

unsigned int previous_keypad_state = 0; //hold the state of the keyboard 0x0000
int ascii[16]; //Is the ascii code for each key on the keyboard

unsigned char scan_row_sim(int row)
{
	unsigned char temp;
	volatile unsigned char pressed_row;

	temp = 0x08;
	PORTC = temp << row;
	_delay_us(500);
	asm("nop");
	asm("nop");
	pressed_row = PINC & 0x0f;

	return pressed_row;
}
unsigned int scan_keypad_sim(void)
{
	volatile unsigned char pressed_row1, pressed_row2, pressed_row3, pressed_row4;
	volatile unsigned int pressed_keypad = 0x0000;

	pressed_row1 = scan_row_sim(1);
	pressed_row2 = scan_row_sim(2);
	pressed_row3 = scan_row_sim(3);
	pressed_row4 = scan_row_sim(4);

	pressed_keypad = (pressed_row1 << 12 | pressed_row2 << 8) | (pressed_row3 << 4) | (pressed_row4);
	PORTC =0x00;
	return pressed_keypad;
}
unsigned int scan_keypad_rising_edge_sim(void)
{
	unsigned int pressed_keypad1, pressed_keypad2, current_keypad_state, final_keypad_state;

	pressed_keypad1 = scan_keypad_sim();
	_delay_ms(SPARK_DELAY_TIME);
	pressed_keypad2 = scan_keypad_sim();
	current_keypad_state = pressed_keypad1 & pressed_keypad2;
	final_keypad_state = current_keypad_state & (~ previous_keypad_state);
	previous_keypad_state = current_keypad_state;

	return final_keypad_state;
}
unsigned char keypad_to_ascii_sim(unsigned int final_keypad_state)
{
	volatile int j;
	volatile unsigned int temp;

	for (j=0; j<16; j++)
	{
		temp = 0x01;
		temp = temp << j;
		if (final_keypad_state & temp) //if you find the only pressed key then return
		{
			return ascii[j];
		}
	}
	//should not reach here
	return 1;
}
void initialize_ascii(void)
{
	ascii[0] = '*';
	ascii[1] = '0';
	ascii[2] = '#';
	ascii[3] = 'D';
	ascii[4] = '7';
	ascii[5] = '8';
	ascii[6] = '9';
	ascii[7] = 'C';
	ascii[8] = '4';
	ascii[9] = '5';
	ascii[10] = '6';
	ascii[11] = 'B';
	ascii[12] = '1';
	ascii[13] = '2';
	ascii[14] = '3';
	ascii[15] = 'A';
}
unsigned char read4x4(void)
{
	unsigned int keypad_state;
	unsigned char ascii_code;

	keypad_state = scan_keypad_rising_edge_sim(); // read the state of the keyboard
	if (!keypad_state)
	{
		return 0;
	}
	ascii_code = keypad_to_ascii_sim(keypad_state); // encode it to ascii code

	return ascii_code;
}




void PWM_init()
{
	//set TMR0 in fast PWM mode with non-inverted output, prescale=8
	TCCR0 = (1<<WGM00) | (1<<WGM01) | (1<<COM01) | (1<<CS01);
	DDRB|=(1<<PB3); //set PB3 pin as output
	//set TMR1A in fast PWM 8 bit mode with non-inverted output
	//prescale=8
	TCCR1A = (1<<WGM10) | (1<<COM1A1);
	TCCR1B = (1<<WGM12) | (1<<CS11);
	DDRD|=(1<<PD5); //set PD5 pin as output
	//set TMR2 in fast PWM mode with non-inverted output, prescale=8
	TCCR2 = (1<<WGM20) | (1<<WGM21) | (1<<COM21) | (1<<CS21);
	DDRD|=(1<<PD7); //set PD7 pin as output
}

int main ()
{   
	unsigned char number, step=25;
	PWM_init();
	OCR0=50;
	OCR1AL=50;
	OCR2=50;
	while (1)
	{   
		do
		{
			number = read4x4(); // wait for the number to be pushed
		}
		while(!number);
	  if( number =='1')
	  OCR0+= step;
	   if( number =='2')
	   OCR0-= step;
	   if( number =='4')
	   OCR1AL+= step;
	    if( number =='5')
	    OCR1AL-= step;
		if( number =='7')
		OCR2+= step;
		if( number =='8')
		OCR2-= step;
		
	}
}







