#include "TaskManager.h"

//****************************************************************************//
//*** Constructoras y destructora ********************************************//
//****************************************************************************//
TaskManager::TaskManager()
	{
	coreCount=(int)std::thread::hardware_concurrency();
	threadCount=0;
	}

TaskManager::TaskManager(std::vector<Task> taskVector, std::vector<Task> taskVector2, int threadCount)
	{
	this->taskVector=taskVector;
	this->taskVector2=taskVector2;
	coreCount=(int)std::thread::hardware_concurrency();
	if (threadCount<=coreCount)
		this->threadCount=threadCount;
	else
		this->threadCount=coreCount;
	}

TaskManager::~TaskManager()
	{
	}

//****************************************************************************//
//*** Métodos públicos *******************************************************//
//****************************************************************************//
void TaskManager::launchCommands()
	{
	// Tareas de TaskVector
	if (threadCount<=1)
		{
		std::cout << "********************************************" << std::endl;
		std::cout << "** PROCESAMIENTO EN SERIE (FASE 1) *********" << std::endl;
		std::cout << "********************************************" << std::endl;
		time_t start=time(0);
		for(unsigned int i=0; i!=taskVector.size(); i++)
			{
			CommandLauncher cl=/*new*/CommandLauncher(taskVector[i]);
			cl.launch();
			}
		time_t end=time(0);
		double time=difftime(end, start);// * 1000.0;
		std::cout << "Tiempo en serie FASE 1 (s): " << time << std::endl;
		std::cout << "***********************************************" << std::endl;
		std::cout << "** FIN DE PROCESAMIENTO EN SERIE (FASE 1) *****" << std::endl;
		std::cout << "***********************************************" << std::endl;
		}
	else
		{
		std::cout << "***********************************************" << std::endl;
		std::cout << "** PROCESAMIENTO EN PARALELO (FASE 1) *********" << std::endl;
		std::cout << "***********************************************" << std::endl;
		time_t start=time(0);
		ThreadPool tp {taskVector, threadCount, 1};  // Esto es crearlo directamente
		//ThreadPool tp=/*new*/ThreadPool(taskVector,threadCount,10);  // Esto es por copia, no funcionaría porque implica copiar threads
		time_t end=time(0);
		double time=difftime(end, start);// * 1000.0;
		std::cout << "Tiempo en paralelo FASE 1 (s): " << time << std::endl;
		std::cout << "***********************************************" << std::endl;
		std::cout << "** FIN DE PROCESAMIENTO EN PARALELO (FASE 1) **" << std::endl;
		std::cout << "***********************************************" << std::endl << std::endl;
		}
	// Tareas de TaskVector2
	if (threadCount<=1)
		{
		std::cout << "***********************************************" << std::endl;
		std::cout << "** PROCESAMIENTO EN SERIE (FASE 2) ************" << std::endl;
		std::cout << "***********************************************" << std::endl;
		time_t start=time(0);
		for(unsigned int i=0; i!=taskVector2.size(); i++)
			{
			CommandLauncher cl=/*new*/CommandLauncher(taskVector2[i]);
			cl.launch();
			}
		time_t end=time(0);
		double time=difftime(end, start);// * 1000.0;
		std::cout << "Tiempo en serie FASE 2 (s): " << time << std::endl;
		std::cout << "***********************************************" << std::endl;
		std::cout << "** FIN DE PROCESAMIENTO EN SERIE (FASE 2) *****" << std::endl;
		std::cout << "***********************************************" << std::endl;
		}
	else
		{
		std::cout << "***********************************************" << std::endl;
		std::cout << "** PROCESAMIENTO EN PARALELO (FASE 2) *********" << std::endl;
		std::cout << "***********************************************" << std::endl;
		time_t start=time(0);
		ThreadPool tp {taskVector2, threadCount, 1};  // Esto es crearlo directamente
		//ThreadPool tp=/*new*/ThreadPool(taskVector,threadCount,10);  // Esto es por copia, no funcionaría porque implica copiar threads
		time_t end=time(0);
		double time=difftime(end, start);// * 1000.0;
		std::cout << "Tiempo en paralelo FASE 2 (s): " << time << std::endl;
		std::cout << "***********************************************" << std::endl;
		std::cout << "** FIN DE PROCESAMIENTO EN PARALELO (FASE 2) **" << std::endl;
		std::cout << "***********************************************" << std::endl;
		}
	}

//****************************************************************************//
//*** Métodos privados *******************************************************//
//****************************************************************************//
