#include "main.h"
#include <sys/types.h>
#include "SystemClockConfig_HSI.h"
// #include "SystemClockConfig_HSE.h"

int main(void)
{
    // Initialize the HAL Library
    HAL_Init();
    __enable_irq(); // Enable global interrupts
    // HAL_InitTick(TICK_INT_PRIORITY);

    // Configure the system clock
    SystemClockConfig_HSI();
    // SystemClockConfig_HSE();

    // Enable the GPIO clock for port B
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOG_CLK_ENABLE();

    // Configure the GPIO pin for the LED
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = GPIO_PIN_7;       // PB7 (LD2 LED)
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP; // Push-pull output mode
    GPIO_InitStruct.Pull = GPIO_NOPULL;    // No pull-up or pull-down resistor
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    // Configure PC12 as output
    GPIO_InitStruct.Pin = GPIO_PIN_12;          // Pin PC12
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_2;          // Pin PG2
    HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);
    HAL_GPIO_WritePin(GPIOG, GPIO_PIN_2, GPIO_PIN_RESET); // Set PG2 LOW (GND)

    // volatile uint32_t previous_tick = 0;
    while (1) {
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_SET);
        HAL_GPIO_WritePin(GPIOC, GPIO_PIN_12, GPIO_PIN_SET); // Set PC12 HIGH (5V)
        HAL_Delay(5000);
        HAL_GPIO_WritePin(GPIOC, GPIO_PIN_12, GPIO_PIN_RESET); // Set PC12 HIGH (5V)
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_RESET);
        HAL_Delay(5000);
    }
    return 0;
}


/* ***THIS IS EXTREMELY IMPORTANT FOR THE TICKING TO WORK!!!***
I believe this should've been implemented in the HAL or some such lib.
But it's not. Without this, the LED doesn't blink - it just stays on.
*/
void SysTick_Handler(void) {
    HAL_IncTick();  // Increment tick counter
}

// void Simple_Delay(uint32_t delay_ms) {
//     for (volatile uint32_t i = 0; i < delay_ms * 1000; i++) {
//         __NOP(); // No operation
//     }
// }
