#include "CommandLauncher.h"

//****************************************************************************//
//*** Constructoras y destructora ********************************************//
//****************************************************************************//
CommandLauncher::CommandLauncher()
	{
	}

CommandLauncher::CommandLauncher(Task t)
	{
	this->t=t;
	}

CommandLauncher::~CommandLauncher()
	{
	}

//****************************************************************************//
//*** Métodos públicos *******************************************************//
//****************************************************************************//
void CommandLauncher::launch()
	{
	time_t start=time(0);
	std::cout << t.getComment() << ":" << " '" << t.getCommand() << "'" << std::endl;
	system((t.getCommand()).c_str());
	time_t end=time(0);
	double time=difftime(end, start);// * 1000.0;
	std::cout << "Tiempo de tarea (s): " << time << std::endl;
	}

//****************************************************************************//
//*** Métodos privados *******************************************************//
//****************************************************************************//

