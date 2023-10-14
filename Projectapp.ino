#include <avr/io.h>
#include <util/delay.h>
#include <string.h>

#define F_CPU 16000000UL
#define BAUD 9600
#define UBRR_VALUE ((F_CPU / 16 / BAUD) - 1)

volatile uint8_t switchState = 0;  // To store the switch state

int main(void) {
    // Initialize UART
    uart_init(BAUD);

    // Set switch pin as input
    DDRD &= ~(1 << DDD2);
    // Set LED pin as output
    DDRB |= (1 << DDB5);

    char button_status[19] = "Button is pressed\n";
    char button_status2[23] = "Button is not pressed\n";
    char setString[] = "LED_ON";
    char receivedString[50]; // Buffer to store received string
    unsigned char maxStringLength = sizeof(receivedString);

    while (1) {
        // Read the switch state
        if (PIND & (1 << PIND2)) {
            // Switch is not pressed
            if (switchState == 1) {
                uart_transmit_array(button_status2, 23);  // Send 'Button is not pressed' for switch released
                switchState = 0;
            }
        } else {
            // Switch is pressed
            if (switchState == 0) {
                uart_transmit_array(button_status, 19);  // Send 'Button is pressed' for switch pressed
                switchState = 1;
            }
          }    
            // Receive a string from UART
          UART_receiveString(receivedString, maxStringLength);

          int res = strcmp(receivedString, setString);

          if (res == 0) {
              PORTB |= (1 << PORTB5);  // Turn LED on
          } else {
              PORTB &= ~(1 << PORTB5);  // Turn LED off
          }
        

        _delay_ms(1000);  // Delay for debouncing and to avoid UART spam
    }

    return 0;
}
