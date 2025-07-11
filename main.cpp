#define F_CPU 8000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include <string.h>
#include <util/delay.h>

#define HBRIDGE_DDR DDRB
#define HBRIDGE_PORT PORTB
#define HBRIDGE_IN1 PB5
#define HBRIDGE_IN2 PB7

#define RX_BUFFER_SIZE 32

//uart0 global variables
volatile char rx_buffer[RX_BUFFER_SIZE];
volatile uint8_t rx_index = 0;
volatile uint8_t new_message_flag = 0;

//motor global variables
bool curtain_is_open = false;
bool curtain_is_closed = false;

//Function Prototypes
void uart0_initialization(uint32_t baud_rate);
void uart0_transmit(char data);
void uart0_send_string(const char *str);

void open_curtain();
void close_curtain();

ISR(USART0_RX_vect){
	char received_char = UDR0;
	
	if (received_char == '\r' || received_char == '\n')
	{
		if (rx_index > 0)
		{
			rx_buffer[rx_index] = '\0';
			new_message_flag = 1;
			rx_index = 0;
		}
	} else if (rx_index < (RX_BUFFER_SIZE - 1))
	{
		rx_buffer[rx_index++] = received_char;
	}
}

int main(void)
{
	HBRIDGE_DDR |= (1<<HBRIDGE_IN1) | (1<<HBRIDGE_IN2);
	
	
	uart0_initialization(9600);
	
	sei();
	
	uart0_send_string("MCU Ready!\r\n");
    while (1) 
    {
		if (new_message_flag)
		{
			new_message_flag = 0;
			
			if (strstr((const char*)rx_buffer, "open curtain") != NULL)
			{
				open_curtain();
				uart0_send_string("Opening curtain......\r\n");
			} else if (strstr((const char*)rx_buffer, "close curtain") != NULL)
			{
				close_curtain();
				uart0_send_string("Closing curtain......\r\n");
			} else
			{
				uart0_send_string("Unknown command.\r\n");
			}
			memset((void*)rx_buffer, 0, RX_BUFFER_SIZE);
		}
    }
}

void uart0_initialization(uint32_t baud_rate){
	uint16_t ubrr_value = (F_CPU / (8 * baud_rate)) - 1;
	UBRR0H = (uint8_t) (ubrr_value>>8);
	UBRR0L = (uint8_t) ubrr_value;
	
	UCSR0B |= (1<<RXEN0) | (1<<TXEN0) | (1<<RXCIE0);
	UCSR0B &= ~(1<<UCSZ02);
	
	UCSR0C &= ~((1<<UMSEL01) | (1<<UMSEL00) | (1<<UPM01) | (1<<UPM00) | (1<<USBS0));
	UCSR0C |= (1<<UCSZ01) | (1<<UCSZ00);
	
	UCSR0A |= (1<<U2X0);
}

void uart0_transmit(char data){
	while (!(UCSR0A & (1<<UDRE0)));
	UDR0 = data;
}

void uart0_send_string(const char *str){
	while (*str){
		uart0_transmit(*str++);
	}
}

void open_curtain() {
	if (!curtain_is_open)
	{
		HBRIDGE_PORT |= (1<<HBRIDGE_IN1);
		HBRIDGE_PORT &= ~(1<<HBRIDGE_IN2);
		_delay_ms(50);
		curtain_is_open = true;
		curtain_is_closed = false;
	}
}

void close_curtain() {
	if(!curtain_is_closed){
		HBRIDGE_PORT &= ~(1<<HBRIDGE_IN1);
		HBRIDGE_PORT |= (1<<HBRIDGE_IN2);
		_delay_ms(50);
		curtain_is_open = false;
		curtain_is_closed = true;
	}
}