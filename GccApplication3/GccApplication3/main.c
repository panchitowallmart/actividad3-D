/*
 * GccApplication3.c
 *
 * Created: 03/11/2024 08:58:51 p. m.
 * Author : emili
 */ 

#define F_CPU 16000000UL // Define la frecuencia del reloj a 16 MHz
#include <avr/io.h>
#include <util/delay.h>

#define POT1_PIN 0 // Pin analógico A0
#define POT2_PIN 1 // Pin analógico A1

void USART_Init(unsigned int ubrr) {
	// Configura la velocidad de transmisión
	UBRR0H = (unsigned char)(ubrr >> 8);
	UBRR0L = (unsigned char)ubrr;
	UCSR0B = (1 << RXEN0) | (1 << TXEN0); // Habilita receptor y transmisor
	UCSR0C = (1 << UCSZ01) | (1 << UCSZ00); // 8 bits de datos, 1 bit de parada
}

void USART_Transmit(unsigned char data) {
	while (!(UCSR0A & (1 << UDRE0))); // Espera hasta que el buffer esté vacío
	UDR0 = data; // Envía el dato
}

unsigned int ADC_Read(unsigned char channel) {
	ADMUX = (1 << REFS0) | (channel & 0x0F); // Selecciona el canal y referencia
	ADCSRA |= (1 << ADSC); // Inicia la conversión
	while (ADCSRA & (1 << ADSC)); // Espera a que la conversión termine
	return ADC; // Devuelve el resultado
}

int main(void) {
	USART_Init(51); // Inicializa USART con 9600 bps
	ADCSRA |= (1 << ADEN); // Habilita el ADC
	while (1) {
		unsigned int pot1Value = ADC_Read(POT1_PIN); // Lee el valor de POT1
		unsigned int pot2Value = ADC_Read(POT2_PIN); // Lee el valor de POT2

		// Envía los valores por USART
		USART_Transmit('P'); // Envío de una letra para indicar que hay datos
		USART_Transmit(pot1Value); // Envía el valor de POT1
		USART_Transmit(pot2Value); // Envía el valor de POT2

		_delay_ms(1000); // Espera 1 segundo
	}
}
