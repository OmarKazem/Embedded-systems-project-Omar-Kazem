// Function prototypes defined in header file for UART library
// .h files should be included in main app in order to use the library

void uart_SetBaudRate(int baud);
void uart_init (int baud);
void uart_transmit (unsigned char data);
void uart_transmit_array(char* data, int size);
unsigned char uart_recieve (void);
void UART_receiveString(char *str, unsigned char maxLength);
int uart_recieve_ready (void);