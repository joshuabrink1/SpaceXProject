#ifndef INTERRUPT_H
#define INTERRUPT_H

#include "Command.h"
#include "Rocket.h"

class Interrupt :public Command {
private:
	Rocket* myRocket;

public:
	Interrupt(Rocket* R) { myRocket = R; };
	void execute() { myRocket->Interrupt(); };
};

#endif
