#ifndef SRC_TASKMANAGER_H_
#define SRC_TASKMANAGER_H_

#include <vector>
#include "ThreadPool.h"
#include "CommandLauncher.h"
#include "Task.h"

class TaskManager
	{
	private:
		// Atributos
		int threadCount;
		int coreCount;
		std::vector<Task> taskVector;
		std::vector<Task> taskVector2;
	public:
		// Constructoras y destructora
		TaskManager();
		TaskManager(std::vector<Task>,std::vector<Task>,int);
		virtual ~TaskManager();
		// Métodos públicos
		void launchCommands();
	};

#endif /* SRC_TASKMANAGER_H_ */
