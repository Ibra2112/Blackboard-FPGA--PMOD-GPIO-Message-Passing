
#include <stdint.h>  // Integer types
#include <stdio.h>   // sprintf
#include "sleep.h"   // Delay functions

#define UART1_CR *((uint32_t*) 0xE0001000)
#define UART1_MR *((uint32_t*)0xE0001004)
#define UART1_BAUDGEN *((uint32_t*)0xE0001018)
#define UART1_BAUDDIV *((uint32_t*)0xE0001034)

#define UART1_SR *((uint32_t*)0xE000102C)
#define UART1_DATA *((uint32_t*)0xE0001030)
#define BaudGen115200 0x7c
#define BaudDiv115200 6

// Function to see if status register is full
int uart1_tx_full()
{
    return (UART1_SR & 0x10) != 0;
}

int UART1_RXEmpty()
{
    uint32_t UARTData; 
    UARTData = UART1_SR;
    return (UARTData & 0x02) == 0x2;
}

void ResetUART()
{
    UART1_CR = 0x3;
    while ((UART1_CR & 0x2) == 0x2) {}
    while ((UART1_CR & 0x1) == 0x1) {}
}

void SetBaudrate1()
{
    UART1_BAUDGEN = BaudGen115200;
    UART1_BAUDDIV = BaudDiv115200;
}

void Configuure_UART1()
{
    uint32_t UARTDATA;
    UART1_MR = 0x20;
    UARTDATA = UART1_CR;
    UARTDATA |= 0x14;
    UART1_CR = UARTDATA;
}

void intiUart1()
{
    ResetUART();
    Configuure_UART1();
    SetBaudrate1();
}

void Uart1_send_char(char c)
{
    while (uart1_tx_full()) {}
    UART1_DATA = c;
}

void Uart1_send_string(const char *str)
{
    while (*str)
    {
        Uart1_send_char(*str);
        str++;
    }
}

char Uart1_receive_char()
{
    while (UART1_RXEmpty()) {}
    return (char)(UART1_DATA & 0xFF);
}

void Uart1_receive_string(char *buffer, int max_length)
{
    int i = 0;
    char c;
    while (i < max_length - 1)
    {
        c = Uart1_receive_char();
        if (c == '\n' || c == '\r')
        {
            break;
        }
        buffer[i++] = c;
    }
    buffer[i] = '\0';
}

int is_prime(int num)
{
    if (num < 2) return 0;
    for (int i = 2; i * i <= num; i++)
    {
        if (num % i == 0)
            return 0;
    }
    return 1;
}

int main(void)
{
    intiUart1();
    
    while (1) {
        Uart1_send_string("Enter a positive integer greater than or equal to 2: \n");
        
        char input_buffer[10];
        Uart1_receive_string(input_buffer, sizeof(input_buffer));
        
        int num = atoi(input_buffer);
        
        if (num < 2)
        {
            Uart1_send_string("Invalid number. Must be >= 2.\n");
            continue;
        }
        
        char output_buffer[100];
        sprintf(output_buffer, "You entered: %d\n", num);
        Uart1_send_string(output_buffer);
        
        if (is_prime(num)) {
            sprintf(output_buffer, "%d is a prime number.\n", num);
        } else {
            int nearest_prime = num;
            while (!is_prime(nearest_prime)) {
                nearest_prime--;
            }
            sprintf(output_buffer, "%d is not a prime number. The closest prime number is: %d\n", num, nearest_prime);
        }
        Uart1_send_string(output_buffer);
    }
    
    return 0;
}
