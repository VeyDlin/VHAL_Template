#pragma once
#include <Application.h>



class MainTask: public ThreadStatic<128> {
public:
	virtual void Execute() override {
		while(true) {
			BSP::ledPin.Toggle();
			Sleep(1s);
		}
	}
};
