#ifndef HDR_TRANSCODER_H_
#define HDR_TRANSCODER_H_

#include <string>
#include <iostream>
#include "TaskFeeder.h"
#include "TaskManager.h"

class Transcoder
	{
	private:
		// Atributos
		TaskFeeder tf;
		TaskManager tm;

	public:
		// Constructoras y destructora
		Transcoder();
		Transcoder(std::string);
		virtual ~Transcoder();
		// Métodos públicos
		int parseOptionFile();
		void fillTaskManager();
		void launchCommands();
		//void start(string,string);
		//void setTV();  // Enciende o apaga la tele
		//void setLamp();  // Enciende o apaga la lámpara
		//void setPuerta();  // Abre o cierra la puerta
		//void escala(int selec,float x,float y,float z);  // Escala la escena, la lámpara o los muebles
		//void traslada(int selec,float x,float y,float z);  // Traslada la escena, la lámpara o los muebles
		//void rota(int selec,float a,float x,float y,float z);  // Rota la escena, la lámpara o los muebl
	};

#endif /* HDR_TRANSCODER_H_ */
