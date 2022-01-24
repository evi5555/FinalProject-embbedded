#include "led.h"
#include "stm32f303xe.h"
#include "uart.h"
#include "wifi.h"




// Dummy byte, for transmission on the SPI, see the function SPI1_send().
// We declare it as static rather than a local variable,
// so that we don't get a warning for reading into it without using it.
static uint8_t Rx_Dummy_Byte;




void SPI1_init(void)
{
	// Enable GPIOA and GPIOB clocks (p. 148 in the reference manual).
    RCC->AHBENR |= 0x00020000;
    RCC->AHBENR |= 0x00040000;
    // Enable SPI1 clock.
    RCC->APB2ENR |= 0x00001000;
    // Configure GPIOA pin 15, and GPIOB pins 3, 4, and 5 as alternate function 5,
    // which is SPI1 (p. 53 in the datasheet).
    GPIOA->MODER |= 0x80000000;
    GPIOB->MODER |= 0x00000A80;
    // AFR[0] is the same as AFRL in the reference manual (p. 241),
    // and AFR[1] is the same as AFRH in the reference manual (p. 242).
    GPIOA->AFR[1] |= 0x50000000;
    GPIOB->AFR[0] |= 0x00555000;
    // Enable SPI1, configure it as the master side,
    // and set the communication rate to the lowest value possible.
    // Also enable hardware NSS management and set it as an output,
    // because we only have one slave.
    SPI1->CR1 = 0x0000007C;
    // Note that the reset value of the CR2 register is 0x00000700,
    // so we don't change the bits which are already set in the register.
    SPI1->CR2 |= 0x00000004;
}




uint8_t SPI1_receive(void)
{
	uint8_t rx_byte;
    // Write a dummy byte to the SPI1 data register, to initiate a transfer.
    SPI1->DR = 0x55;
    // Wait for the TXE and RXNE flags to be set.
    while((SPI1->SR & 0x00000003) != 0x00000003);
    // Get the data from the data register.
    rx_byte = SPI1->DR;
    return rx_byte;
}




void SPI1_send(uint8_t data)
{
    // Write the data to the SPI1 data register.
    SPI1->DR = data;
    // Wait for the TXE and RXNE flags to be set.
    while((SPI1->SR & 0x00000003) != 0x00000003);
    // Read the RX byte and discard it.
    Rx_Dummy_Byte = SPI1->DR;
}
