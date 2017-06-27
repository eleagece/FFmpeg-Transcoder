#include "ThreadPool.h"

//****************************************************************************//
//*** Constructoras y destructora ********************************************//
//****************************************************************************//
ThreadPool::ThreadPool()
	{
	tasksInProgressOrCompleted=0;
	threadCount=0;
	timer=10;
	}

ThreadPool::ThreadPool(std::vector<Task> taskVector, int threadCount, int timer)
	{
	this->taskVector=taskVector;
	tasksInProgressOrCompleted=0;
	// Cálculo del número de threads que se ejecutarán
	if (threadCount>(int)taskVector.size())
		this->threadCount=taskVector.size();
	else
		this->threadCount=threadCount;
	// Al principio todos los threads están libres
	for (int i=0; i<threadCount; i++)
		freeThreads.push_back(true);
	// Lanzamiento de las 'threadCount' primeras tareas llenando todos los threads
	begin();
	// Chequeo de los threads cada 'timer' segundos para ver si entra siguiente tarea
	this->timer=timer;
	std::thread checker(&ThreadPool::check,this,timer);
	// Al crear un thread al que la tarea que le pasamos es una función miembro
	// de la clase hay que hacerlo como está arriba y no como se haría si no fue-
	// se miembro de la clase:
	//  std::thread checker(check,timer);
	// Más info en:
	//  http://stackoverflow.com/questions/11057800/passing-member-functions-to-stdthread
	//  https://www.tutorialspoint.com/cplusplus/cpp_class_member_functions.htm
	// Se espera a checker (checker termina cuando no hay más tareas que lanzar)
	checker.join();
	std::cout << "JOIN CHECKER " << std::endl;
	// Se espera a las últimas tareas (checker lanzó las últimas pero no esperó por ellas)
	waitForEveryone();
	std::cout << "Ejecución terminada" << std::endl;
	}

ThreadPool::~ThreadPool()
	{
	}

//****************************************************************************//
//*** Métodos públicos *******************************************************//
//****************************************************************************//

void ThreadPool::begin()
	{
	// Se lanzan los 'threadCount' primeros threads que corresponden a las
	// 'threadCount' primeras tareas
	for (int i=0; i<threadCount; i++)
		{
		freeThreads[i]=false;
		//pool.emplace_back(std::thread(&ThreadPool::launchThread,this,i,i).detach());
		std::cout << "INIT_LAUNCH(task="<< i <<",thread=" << i << ")" << std::endl;
		pool.emplace_back(std::thread(&ThreadPool::launchThread,this,i,i));
		//pool[i]=std::thread(&ThreadPool::launchThread,this,i,i);
		// Creamos el thread de manera anónima y lo asignamos en un vector
		// de threads, por tanto hay que usar asignación '='. No se puede
		// hacer igual que como se crea 'checker' en el constructor. Estas
		// cosas están mal:
		//  pool[i](launchThread,i,i);
		//  pool[i](&ThreadPool::launchThread,this,i,i);
		}
	}

//****************************************************************************//
//*** Métodos privados *******************************************************//
//****************************************************************************//
void ThreadPool::waitForEveryone()
	{
	// Espera a los últimos threads
	for (int i=0; i<(int)pool.size(); i++)
		{
		pool[i].join();
		std::cout << "END_JOIN(thread=" << i << ")" << std::endl;
		}
	}

void ThreadPool::check(int time)
	{
	// Este método se usa como thread especial. Cada 'time' segundos chequea si
	// quedan tareas por hacer y en caso afirmativo intenta lanzarlas usando el
	// primer thread libre. Lo consiga o no, vuelve a dormir 'time' segundos
	bool tasksToDo=true;
	while (tasksToDo)
		{
		if (tasksInProgressOrCompleted==(int)taskVector.size())
			tasksToDo=false;
		else
			{
			std::cout << "CHECKER(tasksLaunched=" << tasksInProgressOrCompleted << ")" << std::endl;
			int first=getFirstFreeThread();
			if (first>-1)
				{
				pool[first].join();  // Espera (en realidad lo sabemos acabado) al thread de la posición 'i'
				std::cout << "CHECK_JOIN(thread=" << first << ")" << std::endl;
				std::cout << "CHECK_LAUNCH(task="<< tasksInProgressOrCompleted <<",thread=" << first << ")" << std::endl;
				pool[first]=std::thread(&ThreadPool::launchThread,this,tasksInProgressOrCompleted,first);
				// Creamos el thread de manera anónima y lo asignamos en un vector
				// de threads, por tanto hay que usar asignación '='. No se puede
				// hacer igual que como se crea 'checker' en el constructor. Estas
				// cosas están mal:
				//  pool[first](launchThread,tasksInProgressOrCompleted,first);
				//  pool[first](&ThreadPool::launchThread,this,tasksInProgressOrCompleted,first);
				}
			sleep(time);
			}
		}
	}

void ThreadPool::launchThread(int taskNum, int threadNum)
	{
	// Se indica que avanzamos una tarea con 'tasksInProgressOrCompleted++'. La lanzamos
	// con 'cl.launch()' y una vez termina decimos que el thread que se ocupó de ella que-
	// da libre para que ejecute otra tarea con 'freeThreads[threadNum]=true'
	/*
	// TESTING: los procesos duermen segundos aleatoriamente entre 5 y 10
	tasksInProgressOrCompleted++;
	freeThreads[threadNum]=false;
	int thisTask=tasksInProgressOrCompleted-1;
	int timeSleeping=5+(rand()%5);
	std::cout << "SLEEP_" << timeSleeping <<"(task=" << thisTask << ")" << std::endl;
	sleep(timeSleeping);
	std::cout << "AWAKE_" << timeSleeping << "(task=" << thisTask << ")" << std::endl;
	freeThreads[threadNum]=true;
	*/
	///*
	// FINAL: los procesos son las tareas
	tasksInProgressOrCompleted++;
	freeThreads[threadNum]=false;
	CommandLauncher cl=CommandLauncher(taskVector[taskNum]);
	cl.launch();
	freeThreads[threadNum]=true;
	//*/
	}

int ThreadPool::getFirstFreeThread()
	{
	// Se busca el primer thread libre de la thread pool. Si se encuentra devuelve
	// su posición, si están todos ocupados devuelve -1
	int first=-1;
	for (int i=0; i<threadCount; i++)
		{
		if (freeThreads[i]==true)
			{
			first=i;
			break;
			}
		}
	return first;
	}
