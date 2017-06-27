#include "Task.h"

//****************************************************************************//
//*** Constructoras y destructora ********************************************//
//****************************************************************************//
Task::Task()
	{
	part=0;
	partOfAFullVideo=false;
	comment="???";
	globalOptions={};
	inputOptions={};
	outputOptions={};
	}

Task::Task(std::string inputFile, std::string inputFolder, std::string outputFolder)
	{
	part=0;
	partOfAFullVideo=false;
	this->inputFile=inputFile;
	this->inputFolder=inputFolder;
	this->outputFolder=outputFolder;
	}

Task::Task(std::string command)
	{
	part=0;
	partOfAFullVideo=false;
	this->command=command;
	}

Task::~Task()
	{
	}

//****************************************************************************//
//*** Métodos públicos *******************************************************//
//****************************************************************************//
std::string Task::getOutputFile()
	{
	return outputFile;
	}

std::string Task::getComment()
	{
	return comment;
	}

std::string Task::getCommand()
	{
	return command;
	}

bool Task::getIsPartOfAFullVideo()
	{
	return partOfAFullVideo;
	}

std::string Task::getTxtListFile()
	{
	return txtListFile;
	}

std::unordered_map<std::string,std::string>& Task::getGlobalHash()
	{
	return globalOptions;
	}

std::unordered_map<std::string,std::string>& Task::getInputHash()
	{
	return inputOptions;
	}

std::unordered_map<std::string,std::string>& Task::getOutputHash()
	{
	return outputOptions;
	}

void Task::setInputFolder(std::string inputFolder)
	{
	this->inputFolder=inputFolder;
	}

void Task::setInputFile(std::string inputFile)
	{
	this->inputFile=inputFile;
	}

void Task::setOutputFolder(std::string outputFolder)
	{
	this->outputFolder=outputFolder;
	}

void Task::setOutputFile(std::string outputFile)
	{
	this->outputFile=outputFile;
	}

void Task::setComment(std::string comment)
	{
	this->comment=comment;
	}

void Task::setPart(int part)
	{
	this->part=part;
	}

void Task::setIsPartOfAFullVideo(bool partOfAFullVideo)
	{
	this->partOfAFullVideo=partOfAFullVideo;
	}

void Task::setInGlobalHash(std::string key, std::string value)
	{
	globalOptions[key]=value;
	}

void Task::setInInputHash(std::string key, std::string value)
	{
	inputOptions[key]=value;
	}

void Task::setInOutputHash(std::string key, std::string value)
	{
	outputOptions[key]=value;
	//std::cout << outputOptions[key] << std::endl;
	}

void Task::setTxtListFile(std::string txtListFile)
	{
	this->txtListFile=txtListFile;
	}

void Task::makeCommand(bool isSplitted)
	{
	// Lee de las hashes para construir el comando
	// ffmpeg [global_options] {[input_file_options] -i input_url} ... {[output_file_options] output_url} ...
	std::string header="ffmpeg";//"~/bin/ffmpeg";
	std::string gOptStr="";
	std::string iOptStr="";
	std::string oOptStr="";
	if (!globalOptions.empty())
		{
		for (auto itg: globalOptions)
			{
			//std::cout << itg.first << "," << itg.second << std::endl;
			gOptStr=gOptStr+" "+itg.second;
			}
		}
	if (!inputOptions.empty())
		{
		for (auto iti: inputOptions)
			{
			//std::cout << iti.first << "," << iti.second << std::endl;
			iOptStr=iOptStr+" "+iti.second;
			}
		}
	if (!outputOptions.empty())
		{
		for (auto ito: outputOptions)
			{
			//std::cout << ito.first << "," << ito.second << std::endl;
			oOptStr=oOptStr+" "+ito.second;
			}
		}
	if (isSplitted)
		modifyOutputFile(part);
	command=header+gOptStr+iOptStr+" -i "+inputFolder+inputFile+oOptStr+" "+outputFolder+outputFile;
	}

//****************************************************************************//
//*** Métodos privados *******************************************************//
//****************************************************************************//
void Task::modifyOutputFile(int index)
	{
	std::string outputFileName=outputFile.substr(0,outputFile.length()-4);
	std::string outputFileFormat=outputFile.substr(outputFile.length()-4,outputFile.length());
	std::string indexString=std::to_string(index);
	outputFile=outputFileName+indexString+outputFileFormat;
	}
