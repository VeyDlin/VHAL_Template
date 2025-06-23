#include <Application.h>
#include <AppThreads/McuWatchDogTask.h>
#include <AppThreads/MainTask.h>



McuWatchDogTask Application::mcuWatchDog;
MainTask Application::main;




void Application::Init() {
	InitSystemHandle();

	System::console << Console::debug << "Start Main task" << Console::endl;
	RTOS::CreateThread(main);

	if(useWatchDog) {
		System::console << Console::debug << "Start WatchDog task" << Console::endl;
		RTOS::CreateThread(mcuWatchDog, ThreadPriority::realtime);
	}

	RTOS::Start();
}





void Application::InitSystemHandle() {

	// For use System::DelayMs() in RTOS
	System::rtosDelayMsHandle = [](auto delay) {
		if(RTOS::IsSchedulerRun()) {
			RTOS::Sleep(std::chrono::milliseconds(delay));
			return true;
		}
		return false;
	};


	// For use std::cout and printf and System::log
	System::console.SetWriteHandler([](auto string, auto size) {
		BSP::consoleSerial.WriteArray<char>(string, size);
	});

	System::console.SetReadHandler([]() {
		auto read = BSP::consoleSerial.Read<char>(1);
		return static_cast<int>(read.IsOk() ? read.data : EOF);
	});


	// For log System::CriticalError() and SystemAbort()
	System::criticalErrorHandle = [](auto message, auto file, auto line) {
		BSP::ledErrorPin = true;
		// Save to FLASH or/and log
		// Error in (string) file on (line) line code
	};
}


