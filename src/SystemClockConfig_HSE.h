void Error_Handler(void)
{
    while (1)
    {
        // Stay here
    }
}
// System Clock Configuration
void SystemClockConfig_HSE(void)
{
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    // // Configure the main internal regulator output voltage
    // HAL_RCC_DeInit();
    // HAL_PWR_EnableBkUpAccess();
    // __HAL_RCC_PWR_CLK_ENABLE();
    // __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

    // Configure the PLL clock source and parameters
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState = RCC_HSE_ON;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLM = 8;      // Divide HSE frequency by 8
    RCC_OscInitStruct.PLL.PLLN = 216;    // Multiply by 216
    RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2; // Divide by 2 to get 216 MHz
    RCC_OscInitStruct.PLL.PLLQ = 9;      // For USB clock, etc.
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
        Error_Handler();
    }

    volatile uint32_t pllrdy = (__HAL_RCC_GET_FLAG(RCC_FLAG_PLLRDY) != RESET);
    if (__HAL_RCC_GET_FLAG(RCC_FLAG_PLLRDY) == RESET) {
        printf("PLL is not ready!\n");
        Error_Handler();
    }

    __HAL_FLASH_SET_LATENCY(FLASH_LATENCY_7);
    if (__HAL_FLASH_GET_LATENCY() != FLASH_LATENCY_7) {
        printf("Flash latency configuration failed!\n");
        Error_Handler();
    }

    // Select PLL as the system clock source and configure the HCLK, PCLK1, and PCLK2 clocks
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK |
                                  RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;   // HCLK = SYSCLK / 1
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;    // PCLK1 = HCLK / 4
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;    // PCLK2 = HCLK / 2

    // volatile uint32_t in_here = 0;
    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_7) != HAL_OK) {
        // in_here = 1;
        Error_Handler();
    }

    // Configure the SysTick to generate an interrupt every 1 ms
    HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq() / 1000);

    // Set SysTick clock source
    HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

    // Set SysTick interrupt priority
    HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}