#pragma once
#include <BSP.h>

#include <Adapter/OSAdapter/RTOS.h>
#include <Adapter/OSAdapter/Thread.h>
#include <Adapter/OSAdapter/Mutex.h>
#include <Adapter/OSAdapter/MailBox.h>



using namespace OSAdapter;





class Application {
public:
	static class McuWatchDogTask mcuWatchDog;


public:
	static void Init();
};

