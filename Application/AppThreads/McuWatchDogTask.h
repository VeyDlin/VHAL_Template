#pragma once
#include <Application.h>



class McuWatchDogTask: public ThreadStatic<128> {
public:
	virtual void Execute() override {
		BSP::mcuWathDog.SetParameters({ 10000 });
		auto deadline = BSP::mcuWathDog.GetDeadlineMs() / 2;
		BSP::mcuWathDog.Start();
		while(true) {
			BSP::mcuWathDog.Reset();
			Sleep(1ms * deadline);
		}
	}
};
