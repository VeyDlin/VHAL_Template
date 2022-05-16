#include "BSP.h"


AUART BSP::consoleSerial	= { USART1 };
AIWDG BSP::mcuWathDog 		= { IWDG, 32000 };
AGPIO BSP::ledPin 			= { GPIOC, 6, true }; // inversion logic = true (OpenDrain)
AGPIO BSP::ledErrorPin		= { GPIOC, 7, true }; // inversion logic = true (OpenDrain)



void BSP::InitSystem() {
	// Enable Clock
	// NVIC_SetPriorityGrouping
}





void BSP::InitClock() {
	// Inin system clock
}





void BSP::InitSystemTick(uint32 ms, uint32 tickPriority) {
	assert(SysTick_Config(System::GetCoreClock() / (1000U / ((1000U * ms) / 1000U))) == 0);
	assert(tickPriority < (1UL << __NVIC_PRIO_BITS));
	NVIC_SetPriority(SysTick_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), tickPriority, 0));
}





void BSP::InitAdapterPeripheryEvents() {
	consoleSerial.beforePeripheryInit = []() {
		// Enable Clock
		// AGPIO::AlternateInit<AGPIO>({ GPIOA, 9,  7, AGPIO::Pull::Up, AGPIO::Speed::VeryHigh });
		// AGPIO::AlternateInit<AGPIO>({ GPIOA, 10, 7, AGPIO::Pull::Up, AGPIO::Speed::VeryHigh });
		// NVIC_SetPriority
		// NVIC_EnableIRQ
		return Status::ok;
	};

}





void BSP::InitIO() {
	ledPin.Reset().SetParameters({ AGPIO::Mode::OpenDrain });
	ledErrorPin.Reset().SetParameters({ AGPIO::Mode::OpenDrain });
}














