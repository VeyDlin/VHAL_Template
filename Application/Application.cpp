#include <Application.h>
#include <AppThreads/McuWatchDogTask.h>
#include <AppThreads/MainTask.h>



McuWatchDogTask Application::mcuWatchDog;
MainTask Application::main;




void Application::Init() {
	InitSystemHandle();

	RTOS::CreateThread(main);

	if(useWatchDog) {
		RTOS::CreateThread(mcuWatchDog, ThreadPriority::realtime);
	}

	RTOS::Start();
}





void Application::InitSystemHandle() {

	// For use System::DelayMs() in RTOS
	System::rtosDelayMsHandle = [](uint32 delay) {
		if(RTOS::IsSchedulerRun()) {
			RTOS::Sleep(std::chrono::milliseconds(delay));
			return true;
		}
		return false;
	};


	// For use std::cout and printf and System::log
	System::writeHandle = [](char* string, size_t size) {
		BSP::consoleSerial.WriteArray<char>(string, size);
	};

	System::readHandle = [](char* string, size_t size) {
		BSP::consoleSerial.ReadArray<char>(string, size);
	};


	// For log System::CriticalError() and SystemAbort()
	System::criticalErrorHandle = [](char* string, uint32 line) {
		BSP::ledErrorPin = true;
		// Save to FLASH or/and log
		// Error in (string) file on (line) line code
	};
}


