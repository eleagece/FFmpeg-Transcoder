#ifndef SRC_THREADPOOL_H_
#define SRC_THREADPOOL_H_

#include <unistd.h>
#include <vector>
#include <string>
#include <thread>
#include <cstdlib>
#include "Task.h"
#include "CommandLauncher.h"

class ThreadPool
	{
	private:
		// Atributos
		int timer;
		int tasksInProgressOrCompleted;
		std::vector<Task> taskVector;
		int threadCount;
		std::vector<std::thread> pool;
		std::vector<bool> freeThreads;
		// Métodos privados
		void waitForEveryone();
		void check(int);
		void launchThread(int,int);
		int getFirstFreeThread();

	public:
		// Constructores y destructora
		ThreadPool();
		// Los compiladores generan un constructor por defecto, uno por copia, y uno de asignación.
		// Si pones este delete estás haciendo explicito que no quieres que se genere. Otra opción
		// habría sido crear un constructor por copia vacío en private, pero ahora se hace con =delete.
		// También tienes default, que es decirle al compilador lo que iba a hacer pero se lo digo yo
		ThreadPool(const ThreadPool&) = delete;
		ThreadPool(std::vector<Task>,int,int);
		virtual ~ThreadPool();
		// Métodos públicos
		void begin();
	};

#endif /* SRC_THREADPOOL_H_ */
