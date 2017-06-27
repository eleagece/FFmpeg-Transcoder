#ifndef SRC_COMMANDLAUNCHER_H_
#define SRC_COMMANDLAUNCHER_H_

#include <stdlib.h>
#include <cstring>
#include <iostream>
#include "Task.h"

class CommandLauncher
	{
private:
	// Atributos
	Task t;
public:
	// Constructoras y destructora
	CommandLauncher();
	CommandLauncher(Task);
	virtual ~CommandLauncher();
	// Métodos públicos
	void launch();
	};

#endif /* SRC_COMMANDLAUNCHER_H_ */
