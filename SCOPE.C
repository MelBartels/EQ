#include <math.h>
#include <time.h>
#include <dos.h>
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <values.h>
#include <string.h>
#include <ctype.h>
#include <dir.h>
#include <process.h>
#include <graphics.h>
#include "header.h"

void BadExit( const char* Msg)
{
	CloseSteppers();
	CloseSerial( LX200ComPort);
	BadMsgExit( Msg);
}

void main( int argc, char** argv)
{
	int Ix;

	strcpy( ConfigFile, "config.dat");

	Parity = NoneParity;
	DataBits = 8;
	StopBits = 1;

	StartScrollFlag = No;

	/*	if -c, then use following string as configuration file name, ie scope.exe -c config.dat will
	result in config.dat being used

	if -x, then use following string as scroll file name, and execute scroll file upon program
	startup, ie scope.exe -x nan.scr	will cause nan.scr to be loaded and started */

	/* argv[0] is name of executing program */
	for( Ix = 1; Ix < argc; Ix++)
		if( argv[Ix][0] == '-')
			if( (argv[Ix][1] == 'c' || argv[Ix][1] == 'C') && Ix < argc-1)
				strcpy( ConfigFile, argv[Ix+1]);
			else if( (argv[Ix][1] == 'x' || argv[Ix][1] == 'X') && Ix < argc-1)
			{
				strcpy( ScrollFilename, argv[Ix+1]);
				StartScrollFlag = Yes;
			}

	InitCommonVars();
	ReadConfig();

	clrscr();
	if( DisplayOpeningMsgs)
		printf( "\nThis software is licensed and copyrighted.  DO NOT make illegal copies.\n");

	InitMouseControl();
	InitTimes( DisplayOpeningMsgs, Tz, DST, LongitudeDeg);
	InitVideo( DisplayOpeningMsgs);
	InitPPort();
	InitializeHandpad();
	InitSteppers();
	InitConvert();
	InitGuide();
	InitIACA();
	InitLX200Input();
	InitHPEvent();
	InitKBEvent();
	while( !QuitFlag)
	{
		SequentialTaskController();
		/* GrandTourFlag used to flag next object: set in ProcessHPEventsModeSwitch() */
		if( GrandTourLoaded && GrandTourFlag)
			ProcessGrandTour();
		else
			if( ScrollLoaded && ScrollFlag)
				ProcessScroll();
			else
			{
				if( UseMouseFlag && ProcessMouseEvent())
					;
				else
					if( KeyStroke)
						ProcessKBEvents();
					else
						ProcessHPEvents();
			}
	}
	CloseKBEvent();
	WriteConfig();
	CloseSteppers();
	ClosePPort();
	CloseSerial( LX200ComPort);
	CloseMouseControl();
}

