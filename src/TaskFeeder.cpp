#include "TaskFeeder.h"

//****************************************************************************//
//*** Constructoras y destructora ********************************************//
//****************************************************************************//
TaskFeeder::TaskFeeder()
	{
	threadCount=0;
	times=0;
	}

TaskFeeder::TaskFeeder(std::string options)
	{
	times=0;  // 0 por defecto, al leer 'options' veremos si hay split
	threadCount=0;  // 0 por defecto, al leer 'options' veremos si hay multithreading
	this->options=options;  // Copia local de la ruta del archivo de opciones
	}

TaskFeeder::~TaskFeeder()
	{
	}

//****************************************************************************//
//*** Métodos públicos *******************************************************//
//****************************************************************************//
int TaskFeeder::parseOptionFile()
	{
	int returnValue;
	int next;
	bool end=false;
	std::ifstream ifs(options);
	if (!ifs.good())
		{
		returnValue=-1;
		std::cout << "Error al leer de archivo" << std::endl;
		}
	else
		{
		std::cout << "*******************************************" << std::endl;
		std::cout << "**** PARSING ******************************" << std::endl;
		std::cout << "*******************************************" << std::endl;
		while(!end)
			{
			std::string line;  // Línea de datos
			std::getline(ifs,line);  // Se obtiene la línea
			if(!ifs) break;  // Si no hay línea es que se acabó el fichero de forma incorrecta
			// Se parsea la linea de cabecera y se actúa en consecuencia
			next=parseLabelLine(line);
			switch (next)
				{
				case 0:  // **CONFIG=begin
					parseConfigLines(ifs);
					break;
				case 1:  // **OUTPUT=begin
					parseOutputLines(ifs);
					break;
				case 2:  // **SPLIT=begin
					parseSplitLines(ifs,line);
					break;
				case 3:  // **ENDFILE
					end=true;
					returnValue=0;
					break;
				default:  // Etiqueta desconocida (archivo mal formateado)
					end=true;
					returnValue=-1;
					std::cout << "Archivo mal formateado" << std::endl;
					break;
				}
			}
		std::cout << "*******************************************" << std::endl;
		std::cout << "**** END PARSING **************************" << std::endl;
		std::cout << "*******************************************" << std::endl << std::endl;
		}
	return returnValue;
	}

std::vector<Task> TaskFeeder::getTaskVector()
	{
	return taskVector;
	}

std::vector<Task> TaskFeeder::getTaskVector2()
	{
	return taskVector2;
	}

int TaskFeeder::getThreadCount()
	{
	return threadCount;
	}

//****************************************************************************//
//*** Métodos privados *******************************************************//
//****************************************************************************//
int TaskFeeder::parseLabelLine(std::string& line)
	{
	int returnValue;
	if (line=="**CONFIG=begin")
		returnValue=0;
	else if (line=="**OUTPUT=begin")
		returnValue=1;
	else if (line=="**SPLIT=begin")
		returnValue=2;
	else if (line=="**ENDFILE")
		returnValue=3;
	else  // Etiqueta desconocida (archivo mal formateado)
		returnValue=4;
	return returnValue;
	}

void TaskFeeder::parseConfigLines(std::istream& ifs)
	{
	std::cout << "Parseando CONFIG" << std::endl;
	std::string key;
	std::getline(ifs,key,'=');
	std::getline(ifs,inputFile);
	std::getline(ifs,key,'=');
	std::getline(ifs,inputFolder);
	std::getline(ifs,key,'=');
	std::getline(ifs,outputFolder);
	std::getline(ifs,key,'=');
	std::string threadCountStr;
	std::getline(ifs,threadCountStr);
	threadCount=std::stoi(threadCountStr);
	std::cout << "  inputfile=" << inputFile << std::endl;
	std::cout << "  inputfolder=" << inputFolder << std::endl;
	std::cout << "  outputfolder=" << outputFolder << std::endl;
	std::cout << "  threads=" << threadCount << std::endl;
	std::string configEnd;
	std::getline(ifs,configEnd);
	if (configEnd=="**CONFIG=end")
		std::cout << "Fin del parseo de CONFIG" << std::endl;
	}

void TaskFeeder::parseOutputLines(std::istream& ifs)
	{
	std::cout << "Parseando OUTPUT" << std::endl;
	bool end=false;
	currentTask=Task();
	currentTask.setInputFolder(this->inputFolder);
	currentTask.setOutputFolder(this->outputFolder);
	currentTask.setInputFile(this->inputFile);
	while (!end)
		end=parseLine(ifs,"**OUTPUT");
	currentTask.makeCommand(false);  // (noSplit,noJoin)
	std::cout << "  " << currentTask.getCommand() << std::endl;
	taskVector.push_back(currentTask);
	std::cout << "Fin del parseo de OUTPUT" << std::endl;
	}

void TaskFeeder::parseSplitLines(std::istream& ifs,std::string& line)
	{
	std::cout << "Parseando SPLIT" << std::endl;
	std::string key;
	std::string times;
	std::getline(ifs,key,'=');
	std::getline(ifs,times);
	if (key!="times")
		std::cout << "Bloque SPLIT debe tener 'times=valor' en la primera línea" << std::endl;
	else
		{
		bool end=false;
		currentTask=Task();
		currentTask.setInputFolder(this->inputFolder);
		currentTask.setOutputFolder(this->outputFolder);
		currentTask.setInputFile(this->inputFile);
		while (!end)
			end=parseLine(ifs,"**SPLIT");
		currentTask.makeCommand(false);  // Tenemos la tarea con su comando sin splitear
		std::cout << "  To split: " << currentTask.getCommand() << std::endl;
		int partsInSplit=std::stoi(times);
		splitTask(partsInSplit);  // Spliteamos la tarea y el comando 'times' veces
		std::cout << "Fin del parseo de SPLIT" << std::endl;
		}
	}

void TaskFeeder::splitTask(int times)
	// Splitea la tarea para el vídeo en subtareas teniendo cuidado con los casos límite. Es de-
	// cir: el primer trozo sólo usa -ss si no empieza en el principio. Y el último trozo no usa
	// -t si es el último trozo de un vídeo completo. Al final une los archivos
	{
	// Comprueba si el split se hace de una parte del vídeo (partialDuration=true), no del vídeo completo
	bool partialDuration;
	int duration=getFileDuration(partialDuration);
	std::string offset;
	int intOffset=0;
	if (partialDuration)
		{
		std::string orderTimestart=currentTask.getOutputHash()["timestart"];
		std::string realTimestart=orderTimestart.substr(4,orderTimestart.length());
		intOffset=std::stoi(realTimestart);
		}
	// Recopila datos para crear los comandos nuevos, los crea y los añade como tareas
	int partDuration=duration/times;
	for (int i=0; i<times; i++)
		{
		Task currentSubTask=currentTask;
		currentSubTask.setPart(i);
		std::string stringStartTime=std::to_string((partDuration*i)+intOffset);
		// Decide cuando poner -ss
		if (i!=0 || (i==0 && (!startFromZero())))
			currentSubTask.setInInputHash("timestart","-ss "+stringStartTime);
		else
			currentSubTask.getInputHash().erase("timestart");
		// Decide cuando poner -t
		if (i<times-1 || (i==times-1 && partialDuration))
			currentSubTask.setInOutputHash("duration","-t "+std::to_string(partDuration));
		else
			currentSubTask.getOutputHash().erase("duration");
		// Crea la tarea indicando que es parte de un split y la mete en el taskVector como tarea
		currentSubTask.makeCommand(true);
		std::cout << "  " << currentSubTask.getCommand() << std::endl;
		taskVector.push_back(currentSubTask);
		}
	// Juntando los archivos
	Task joinTask=Task();
	// Poner a 'inputFileTxt' el valor de inputFile pero con extension .txt y meterlo en la 'joinTask'
	std::string inputFileTxt=inputFile.substr(0,inputFile.length()-4)+".txt";
	joinTask.setInputFile(inputFileTxt);
	// Crear y llenar el archivo txt con las rutas de los archivos de salida
	std::ofstream outputFile;
	outputFile.open(inputFolder+inputFileTxt);
	std::string fileLine;
	for (int i=0; i<times; i++)
		{
		outputFile << "file "
				   << outputFolder
				   << currentTask.getOutputFile().substr(0,currentTask.getOutputFile().length()-4)
				   << i
				   << inputFile.substr(inputFile.length()-4,inputFile.length())
				   << std::endl;
		}
	outputFile.close();
	// Poner nombre al archivo de salida unido (outputFileName)
	std::string outputFileName=inputFile;
	joinTask.setOutputFile(outputFileName);
	// Rellenar resto de datos para construir el comando
	joinTask.setComment("joining chunks");
	joinTask.setInputFolder(this->inputFolder);
	joinTask.setOutputFolder(this->outputFolder);
	joinTask.setInInputHash("concat","-f concat -safe 0");
	joinTask.setInOutputHash("justcopy","-c copy");
	joinTask.makeCommand(false);
	taskVector2.push_back(joinTask);
	}

bool TaskFeeder::startFromZero()
	{
	// Este método dice si el trozo de vídeo que vamos a procesar empieza en el segundo cero
	std::string zero=currentTask.getInputHash()["timestart"];
	if (zero=="0" || zero=="00" || zero=="00:00" || zero=="00:00:00" || zero=="")
		return true;
	else
		return false;
	}

int TaskFeeder::getFileDuration(bool& partialDuration)
	// Este método devuelve lo que dura el trozo de vídeo que queremos splitear. Si el trozo
	// de vídeo es el vídeo completo 'partialDuration' es 'false'. Si sólo es un subtrozo del vídeo
	// completo es 'true'
	{
	int duration;
	std::string orderDuration=currentTask.getOutputHash()["duration"];
	std::string realDuration=orderDuration.substr(3,orderDuration.length());
	duration=std::stoi(realDuration);
	// Estamos spliteando un trozo del vídeo (no un vídeo completo)
	if (!currentTask.getIsPartOfAFullVideo())
		partialDuration=true;
	// Estamos spliteando el vídeo completo
	else
		partialDuration=false;
	return duration;
	}

bool TaskFeeder::parseLine(std::istream& ifs, std::string endlabel)
	{
	std::string key;
	std::string value;
	std::string commandValue;
	bool end=false;
	std::getline(ifs,key,'=');
	////////////////////////////
	// OPCIÓN DE FINALIZACIÓN //
	////////////////////////////
	if (key==endlabel)
		{
		std::getline(ifs,value);
		end=true;
		}
	////////////////////////
	// OPCIONES GENERALES //
	////////////////////////
	// Comentarios de lo que hará 'currentTask'
	else if (key=="comment")
		{
		std::getline(ifs,value);
		currentTask.setComment(value);
		}
	// Nombre del archivo de salida
	else if (key=="name")
		{
		std::getline(ifs,value);
		currentTask.setOutputFile(value);
		}
	///////////////////////
	// OPCIONES DE SPLIT //
	///////////////////////
	// Para indicar que se hace split del vídeo completo. Junto con eso hay que incluír
	// duration=segundos_del_vídeo
	else if (key=="fullvideo")
		{
		std::getline(ifs,value);
		if (value=="yes")
			currentTask.setIsPartOfAFullVideo(true);
		else
			std::cout << "Usar 'fullvideo=yes' para split de vídeo completo" << std::endl;
		}
	////////////////////////
	// OPCIONES DE TIEMPO //
	////////////////////////
	// Segundo desde el que se va a analizar el vídeo
	else if (key=="timestart" || key=="ss")
		{
		std::getline(ifs,value);  // hh:mm:ss, mm:ss, segundos
		commandValue="-ss "+value;
		currentTask.setInInputHash(key,commandValue);
		}
	// Duración del vídeo que se va a analizar en segundos
	else if (key=="duration" || key=="t")
		{
		std::getline(ifs,value);  // hh:mm:ss, mm:ss, segundos
		commandValue="-t "+value;
		currentTask.setInOutputHash(key,commandValue);
		}
	// Frame desde el que se va a analizar el vídeo
	else if (key=="framestart")
		{/*
		std::getline(ifs,value);  // hh:mm:ss, mm:ss, segundos
		commandValue="-ss "+value;
		currentTask.setInOutputHash(key,commandValue);*/
		}
	// Duración del vídeo que se va a analizar en frames
	else if (key=="frames")
		{/*
		std::getline(ifs,value);  // hh:mm:ss, mm:ss, segundos
		commandValue="-t "+value;
		currentTask.setInOutputHash(key,commandValue);*/
		}
	// PARA QUÉ SIRVE EL BUFFER???
	else if (key=="buffersize")
		{
		std::getline(ifs,value);  // hh:mm:ss, mm:ss, segundos
		commandValue="-bufsize "+value+"k ";
		currentTask.setInOutputHash(key,commandValue);
		}
	// Hay 'gop' frames entre keyframes
	else if (key=="gop")
		{
		std::getline(ifs,value);
		commandValue="-g "+value;
		currentTask.setInOutputHash(key,commandValue);
		}
	/////////////////////////////////
	// OPCIONES DE VÍDEO GENERALES //
	/////////////////////////////////
	// Bitrate constante en kbps
	else if (key=="videobitrate" || key=="b:v")
		{
		std::getline(ifs,value);
		commandValue="-b:v "+value+"k";
		currentTask.setInOutputHash(key,commandValue);
		}
	// Bitrate variable (leer 'http://slhck.info/video-encoding')
	else if (key=="varvideobitrate" || key=="q:v")
		{
		std::getline(ifs,value);
		commandValue="-q:v "+value;
		currentTask.setInOutputHash(key,commandValue);
		}
	// Codec del vídeo (mirar los que hay en 'ffmpeg -encoders')
	else if (key=="videocodec" || key=="c:v")
		{
		std::getline(ifs,value);  // copy, libx264...
		commandValue="-c:v "+value;
		currentTask.setInOutputHash(key,commandValue);
		}
	// Resolución en formato 'ancho x alto'
	else if (key=="resolution" || key=="s")
		{
		std::getline(ifs,value);  // 640x480, 1920x1080...
		commandValue="-s "+value;
		currentTask.setInOutputHash(key,commandValue);
		}
	// Framerate en fps
	else if (key=="framerate" || key=="r")
		{
		std::getline(ifs,value);  // 24, 60...
		commandValue="-r "+value;
		currentTask.setInOutputHash(key,commandValue);
		}
	// Aspect ratio en formato 'a:b' o '1.xxxx'
	else if (key=="aspectratio" || key=="aspect")
		{
		std::getline(ifs,value);  // 4:3, 16:9, 1.3333, 1.7777...
		commandValue="-aspect "+value;
		currentTask.setInOutputHash(key,commandValue);
		}
	//////////////////////////////////
	// OPCIONES DE VÍDEO DE libx264 //
	//////////////////////////////////
	// Bitrate variable fijado según calidad (valor de 18-23 está bien. Leer 'http://slhck.info/video-encoding')
	else if (key=="libx264.crf")
		{
		std::getline(ifs,value);  // 51,50,...,1,0
		commandValue="-crf "+value;
		currentTask.setInOutputHash(key,commandValue);
		}
	// Velocidad de procesado. A más lento mejor aprovechamiento del espacio
	else if (key=="libx264.preset")
		{
		std::getline(ifs,value);  // ultrafast, superfast, veryfast, faster, fast, medium, slow, slower, veryslow
		commandValue="-preset "+value;
		currentTask.setInOutputHash(key,commandValue);
		}
	/////////////////////////////////
	// OPCIONES DE VÍDEO DE VA-API //
	/////////////////////////////////
	else if (key=="vaapih264.slot1")
		{
		/*
		std::getline(ifs,value);  // 51,50,...,1,0
		commandValue="-crf "+value;
		currentTask.setInOutputHash(key,commandValue);
		*/
		}
	// Velocidad de procesado. A más lento mejor aprovechamiento del espacio
	else if (key=="vaapih264.slot2")
		{
		/*
		std::getline(ifs,value);  // ultrafast, superfast, veryfast, faster, fast, medium, slow, slower, veryslow
		commandValue="-preset "+value;
		currentTask.setInOutputHash(key,commandValue);
		*/
		}
	///////////////////////
	// OPCIONES DE AUDIO //
	///////////////////////
	// Bitrate constante en kbps
	else if (key=="audiobitrate" || key=="b:a")
		{
		std::getline(ifs,value);
		commandValue="-b:a "+value+"k";
		currentTask.setInOutputHash(key,commandValue);
		}
	// Bitrate variable (leer 'http://slhck.info/video-encoding')
	else if (key=="varaudiobitrate" || key=="q:a")
		{
		std::getline(ifs,value);
		commandValue="-q:a "+value;
		currentTask.setInOutputHash(key,commandValue);
		}
	// Codec del audio (mirar los que hay en 'ffmpeg -encoders')
	else if (key=="audiocodec" || key=="c:a")
		{
		std::getline(ifs,value);  // copy, ac3...
		commandValue="-c:a "+value;
		currentTask.setInOutputHash(key,commandValue);
		}
	/*else if (key=="")
		{

		}*/
	//////////////////////////
	// OPCIONES INCORRECTAS //
	//////////////////////////
	else
		{
		std::cout << "Bloque mal formateado: opción no reconocida" << std::endl;
		}
	return end;
	}

