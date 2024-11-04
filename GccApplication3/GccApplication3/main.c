/*
 * GccApplication3.c
 *
 * Created: 03/11/2024 08:58:51 p. m.
 * Author : emili
 */ 

#define F_CPU 16000000UL // Define la frecuencia del reloj a 16 MHz
#include <avr/io.h>
#include <util/delay.h>

#define POT1_PIN 0 // Pin anal�gico A0
#define POT2_PIN 1 // Pin anal�gico A1

void USART_Init(unsigned int ubrr) {
	// Configura la velocidad de transmisi�n
	UBRR0H = (unsigned char)(ubrr >> 8);
	UBRR0L = (unsigned char)ubrr;
	UCSR0B = (1 << RXEN0) | (1 << TXEN0); // Habilita receptor y transmisor
	UCSR0C = (1 << UCSZ01) | (1 << UCSZ00); // 8 bits de datos, 1 bit de parada
}

void USART_Transmit(unsigned char data) {
	while (!(UCSR0A & (1 << UDRE0))); // Espera hasta que el buffer est� vac�o
	UDR0 = data; // Env�a el dato
}

unsigned int ADC_Read(unsigned char channel) {
	ADMUX = (1 << REFS0) | (channel & 0x0F); // Selecciona el canal y referencia
	ADCSRA |= (1 << ADSC); // Inicia la conversi�n
	while (ADCSRA & (1 << ADSC)); // Espera a que la conversi�n termine
	return ADC; // Devuelve el resultado
}

int main(void) {
	USART_Init(51); // Inicializa USART con 9600 bps
	ADCSRA |= (1 << ADEN); // Habilita el ADC
	while (1) {
		unsigned int pot1Value = ADC_Read(POT1_PIN); // Lee el valor de POT1
		unsigned int pot2Value = ADC_Read(POT2_PIN); // Lee el valor de POT2

		// Env�a los valores por USART
		USART_Transmit('P'); // Env�o de una letra para indicar que hay datos
		USART_Transmit(pot1Value); // Env�a el valor de POT1
		USART_Transmit(pot2Value); // Env�a el valor de POT2

		_delay_ms(1000); // Espera 1 segundo
	}
}
