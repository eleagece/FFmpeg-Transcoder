#ifndef SRC_TASK_H_
#define SRC_TASK_H_

#include <string>
#include <iostream>
#include <unordered_map>

class Task
	{
	private:
		// Atributos
		std::string inputFile;
		std::string inputFolder;
		std::string outputFile;
		std::string outputFolder;
		std::string command;
		std::string comment;
		int part;
		bool partOfAFullVideo;
		std::string txtListFile;
		std::unordered_map<std::string,std::string> globalOptions;  // <id,orden>
		std::unordered_map<std::string,std::string> inputOptions;  // <id,orden>
		std::unordered_map<std::string,std::string> outputOptions;  // <id,orden>
		// Métodos privados
		void modifyOutputFile(int);

	public:
		// Constructoras y destructora
		Task();
		Task(std::string,std::string,std::string);
		Task(std::string);
		virtual ~Task();
		// Métodos públicos
		std::string getOutputFile();
		std::string getComment();
		std::string getCommand();
		bool getIsPartOfAFullVideo();
		std::string getTxtListFile();
		std::unordered_map<std::string,std::string>& getGlobalHash();
		std::unordered_map<std::string,std::string>& getInputHash();
		std::unordered_map<std::string,std::string>& getOutputHash();
		void setInputFolder(std::string);
		void setInputFile(std::string);
		void setOutputFolder(std::string);
		void setOutputFile(std::string);
		void setComment(std::string);
		void setPart(int);
		void setIsPartOfAFullVideo(bool);
		void setInGlobalHash(std::string,std::string);
		void setInInputHash(std::string,std::string);
		void setInOutputHash(std::string,std::string);
		void setTxtListFile(std::string);
		void makeCommand(bool);
	};

#endif /* SRC_TASK_H_ */
