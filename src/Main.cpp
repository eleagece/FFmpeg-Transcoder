#include <iostream>
#include "Transcoder.h"

int main(int argc, char* argv[])
	{
	// Se crea un 'Transcoder' pasando el archivo de opciones en el primer argumento
	Transcoder t=Transcoder(argv[1]);
    // Se hace el parsing del archivo de opciones para crear el vector de tareas
    if (-1==t.parseOptionFile())
    	return -1;
    // Se rellena el 'TaskManager' con el vector de tareas y con el número de threads que queremos
    t.fillTaskManager();
    // Se lanzan comandos (en serie con 'CommandLauncher' y en paralelo con 'ThreadPool')
    t.launchCommands();
    return 0;
	}
