#include <Application.h>
#include <AppThreads/McuWatchDogTask/McuWatchDogTask.h>


McuWatchDogTask Application::mcuWatchDog;





void Application::Init() {
	//RTOS::CreateThread(mcuWatchDog, ThreadPriority::realtime);

	RTOS::Start();
}



