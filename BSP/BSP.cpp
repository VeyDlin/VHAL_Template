#include "BSP.h"


AUART BSP::consoleSerial	= { USART1 };
AIWDG BSP::mcuWathDog 		= { IWDG, 32000 };
AGPIO BSP::ledPin 			= { GPIOC, 6, true }; // inversion logic = true (OpenDrain)
AGPIO BSP::ledErrorPin		= { GPIOC, 7, true }; // inversion logic = true (OpenDrain)



void BSP::InitSystem() {
	LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_SYSCFG);
	LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);
	NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);
}





void BSP::InitClock() {
	LL_FLASH_SetLatency(LL_FLASH_LATENCY_2);
	while (LL_FLASH_GetLatency() != LL_FLASH_LATENCY_2) {
	}
	LL_PWR_SetRegulVoltageScaling(LL_PWR_REGU_VOLTAGE_SCALE1);
	LL_RCC_HSE_Enable();

	/* Wait till HSE is ready */
	while (LL_RCC_HSE_IsReady() != 1) {
	}
	LL_RCC_LSI_Enable();

	/* Wait till LSI is ready */
	while (LL_RCC_LSI_IsReady() != 1) {

	}
	LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSE, LL_RCC_PLLM_DIV_8, 72, LL_RCC_PLLP_DIV_2);
	LL_RCC_PLL_ConfigDomain_48M(LL_RCC_PLLSOURCE_HSE, LL_RCC_PLLM_DIV_8, 72, LL_RCC_PLLQ_DIV_3);
	LL_RCC_PLL_Enable();

	/* Wait till PLL is ready */
	while (LL_RCC_PLL_IsReady() != 1) {
	}
	LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
	LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_2);
	LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_1);
	LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);

	/* Wait till System clock is ready */
	while (LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL) {
	}
	LL_SetSystemCoreClock(72000000);

	LL_RCC_SetUSBClockSource(LL_RCC_USB_CLKSOURCE_PLL);
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

		AGPIO::AlternateInit<AGPIO>({ GPIOA, 9,  7, AGPIO::Pull::Up, AGPIO::Speed::VeryHigh });
		AGPIO::AlternateInit<AGPIO>({ GPIOA, 10, 7, AGPIO::Pull::Up, AGPIO::Speed::VeryHigh });

		NVIC_SetPriority(USART1_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 0, 0));
		NVIC_EnableIRQ(USART1_IRQn);

		return Status::ok;
	};

}





void BSP::InitIO() {
	ledPin.Reset().SetParameters({ AGPIO::Mode::OpenDrain });
	ledErrorPin.Reset().SetParameters({ AGPIO::Mode::OpenDrain });
}














