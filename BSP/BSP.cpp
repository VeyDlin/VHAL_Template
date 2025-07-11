#include "BSP.h"


AUART BSP::consoleSerial	= { USART1 };
AIWDG BSP::mcuWathDog		= { IWDG, 32000 };
AGPIO BSP::ledPin			= { GPIOC, 6, true }; // inversion logic = true (OpenDrain)
AGPIO BSP::ledErrorPin		= { GPIOC, 7, true }; // inversion logic = true (OpenDrain)



void BSP::InitSystem() {
	/* Reset of all peripherals, Initializes the Flash interface and the Systick. */
	LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_SYSCFG);
	LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);

	/* System interrupt init*/
	NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);

	/* PendSV_IRQn interrupt configuration */
	NVIC_SetPriority(PendSV_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 15, 0));

	/* SysTick_IRQn interrupt configuration */
	NVIC_SetPriority(SysTick_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 15, 0));
}





void BSP::InitClock() {
	LL_FLASH_SetLatency(LL_FLASH_LATENCY_3);
	while (LL_FLASH_GetLatency() != LL_FLASH_LATENCY_3) {}
	LL_PWR_SetRegulVoltageScaling(LL_PWR_REGU_VOLTAGE_SCALE1);
	LL_RCC_HSI_SetCalibTrimming(16);
	LL_RCC_HSI_Enable();

	/* Wait till HSI is ready */
	while (LL_RCC_HSI_IsReady() != 1) {}
	LL_RCC_LSI_Enable();

	/* Wait till LSI is ready */
	while (LL_RCC_LSI_IsReady() != 1) {}
	LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSI, LL_RCC_PLLM_DIV_8, 100, LL_RCC_PLLP_DIV_2);
	LL_RCC_PLL_Enable();

	/* Wait till PLL is ready */
	while (LL_RCC_PLL_IsReady() != 1) {}
	LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
	LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_2);
	LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_1);
	LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);

	/* Wait till System clock is ready */
	while (LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL) {}
	// Use BSP::InitSystemTick()
	// LL_Init1msTick(100000000);  
	LL_SetSystemCoreClock(100000000);
	LL_RCC_SetTIMPrescaler(LL_RCC_TIM_PRESCALER_TWICE);
}





void BSP::InitSystemTick(uint32 ms, uint32 tickPriority) {
	assert(SysTick_Config(System::GetCoreClock() / (1000U / ((1000U * ms) / 1000U))) == 0);
	assert(tickPriority < (1UL << __NVIC_PRIO_BITS));
	NVIC_SetPriority(SysTick_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), tickPriority, 0));
}





void BSP::InitAdapterPeripheryEvents() {
	consoleSerial.beforePeripheryInit = []() {
		LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_USART1);

		AGPIO::AlternateInit({ GPIOA, 9,  7, AGPIO::Pull::Up, AGPIO::Speed::VeryHigh });
		AGPIO::AlternateInit({ GPIOA, 10, 7, AGPIO::Pull::Up, AGPIO::Speed::VeryHigh });

		NVIC_SetPriority(USART1_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 0, 0));
		NVIC_EnableIRQ(USART1_IRQn);

		return Status::ok;
	};

}





void BSP::InitIO() {
	ledPin.Reset().SetParameters({ AGPIO::Mode::OpenDrain });
	ledErrorPin.Reset().SetParameters({ AGPIO::Mode::OpenDrain });
}














