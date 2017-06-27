#include "Transcoder.h"

//****************************************************************************//
//*** Constructoras y destructora ********************************************//
//****************************************************************************//
Transcoder::Transcoder()
	{
	}

Transcoder::Transcoder(std::string options)
	{
	tf=TaskFeeder(options);
	}

Transcoder::~Transcoder()
	{
	}

//****************************************************************************//
//*** Métodos públicos *******************************************************//
//****************************************************************************//
int Transcoder::parseOptionFile()
	{
	return (tf.parseOptionFile());
	}

void Transcoder::fillTaskManager()
	{
	tm=TaskManager(tf.getTaskVector(),tf.getTaskVector2(),tf.getThreadCount());
	}

void Transcoder::launchCommands()
	{
	tm.launchCommands();
	}
