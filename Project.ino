#include <avr/io.h>

void uart_SetBaudRate(int baud)
{
  int ubrr = ((F_CPU)/(baud*16UL)-1);
  UBRR0H = (ubrr>>8);                      // shift the register right by 8 bits
  UBRR0L = ubrr;                           // set baud rate bits [7 : 0]
}

void uart_init (int baud)
{
  uart_SetBaudRate(baud);
  UCSR0B|= (1<<TXEN0)|(1<<RXEN0);        // enable receiver and transmitter
  UCSR0C|= (1<<UCSZ00)|(1<<UCSZ01);      // 8bit data format
}

// function to send data
void uart_transmit (unsigned char data)
{ 
  int udr_empty;
  do 
  {
    udr_empty = (UCSR0A & (1<<UDRE0)) != 0;
  }while(!udr_empty);                            // wait while register is free

  UDR0 = data;                                  // load data in the register
}


void uart_transmit_array(char* data, int size){
  for(int i = 0; i < size; i++){
    while(!(UCSR0A & (1<<UDRE0)));
    UDR0 = (int) data[i];
  }
}

// function to receive data
unsigned char uart_recieve (void)
{
  while(!uart_recieve_ready ());                   // wait while data is being received
  return UDR0;                                   // return 8-bit data
}

void UART_receiveString(char *str, unsigned char maxLength) {
	unsigned char i = 0;
	while (i < (maxLength - 1)) {
		while (!(UCSR0A & (1 << RXC0))); // Wait for data to be received
		unsigned char receivedByte = UDR0; // Read the received byte
		
		// Check for end of string
		if (receivedByte == '\0' || receivedByte == '\n' || receivedByte == '\r') {
			break;
		}
		
		str[i++] = receivedByte;
	}
	str[i] = '\0'; // Null-terminate the string
}


int uart_recieve_ready (void)
{
  int flag = ((UCSR0A) & (1<<RXC0)) != 0;     // check if data is receiving is complete
  return flag;
}


