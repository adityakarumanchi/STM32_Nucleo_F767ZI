void Error_Handler(void)
{
    while (1)
    {
        // Stay here
    }
}
void SystemClockConfig_HSI(void) {
    // Configure the main internal regulator output voltage
    // HAL_RCC_DeInit();
    // HAL_PWR_EnableBkUpAccess();
    // __HAL_RCC_PWR_CLK_ENABLE();
    // __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    // Enable HSI oscillator and configure PLL
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI; // Use HSI
    RCC_OscInitStruct.HSIState = RCC_HSI_ON;                  // Turn on HSI
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;              // Enable PLL
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;      // Use HSI as PLL source
    RCC_OscInitStruct.PLL.PLLM = 8;                          // Divide HSI by 16
    RCC_OscInitStruct.PLL.PLLN = 216;                         // Multiply by 216
    RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;               // Divide by 2
    RCC_OscInitStruct.PLL.PLLQ = 9;                           // Divide by 9 for USB clock

    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
        Error_Handler();
    }

    // Configure AHB and APB bus clocks
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK |
                                  RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK; // Set PLL as system clock
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

    __HAL_FLASH_SET_LATENCY(FLASH_LATENCY_7);
    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_7) != HAL_OK) {
        Error_Handler();
    }
        // Configure the SysTick to generate an interrupt every 1 ms
    HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq() / 1000);

    // Set SysTick clock source
    HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

    // Set SysTick interrupt priority
    HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}