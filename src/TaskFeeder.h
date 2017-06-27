#ifndef SRC_TASKFEEDER_H_
#define SRC_TASKFEEDER_H_

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include "Task.h"

class TaskFeeder
	{
	private:
		// Atributos
		std::string options;
		std::string inputFile;
		std::string inputFolder;
		std::string outputFolder;
		int threadCount;
		Task currentTask;
		int times;
		std::vector<Task> taskVector;  // tareas sobre archivo de entrada
		std::vector<Task> taskVector2;  // tareas sobre los archivos resultado del de entrada
		// Métodos privados
		int parseLabelLine(std::string&);
		void parseConfigLines(std::istream&);
		void parseOutputLines(std::istream&);
		void parseSplitLines(std::istream&,std::string&);
		bool parseLine(std::istream&,std::string);
		void splitTask(int);
		bool startFromZero();
		int getFileDuration(bool&);
	public:
		// Constructoras y destructora
		TaskFeeder();
		TaskFeeder(std::string);
		virtual ~TaskFeeder();
		// Métodos públicos
		int parseOptionFile();
		void createTask(std::string);
		std::vector<Task> getTaskVector();
		std::vector<Task> getTaskVector2();
		int getThreadCount();
	};

#endif /* SRC_TASKFEEDER_H_ */
