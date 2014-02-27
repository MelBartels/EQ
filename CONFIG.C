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

/* config... */

void ReadConfig( void)
{
	int I;

	/* defaults */
	DefaultBackground = 0;
	DefaultColor = 7;
	TitleColor = 9;
	BorderColor = 1;
	MenuColor = 12;
	DisplayColor = 12;
	SelectColor = 0;
	CurrentColor = 14;
	SelectBackground = 4;
	ConfirmQuit = 0;
	DisplayOpeningMsgs = 1;
	MoveHsMsgDeg = 10;
	InterfacePath = "C:\\GUIDE\\";
	UseMouseFlag = 0;
	IACA_Flag = 0;
   AutoGEMFlip = 0;
   AutoGEMFlipFuzzDeg = 7.5;
	Siderostat = 0;
	HomeAltDeg = 0;
	HomeAzDeg = 0;
	MsArcsecSec = 300;
	AltFullStepSizeArcsec = 3;
	AzFullStepSizeArcsec = 3;
	HandpadPresentFlag = 1;
	StartingHandpadMode = 0;
	HandpadDesign = StandardHandpad;
	HandpadFlipUpDownWithGEMFlip = Off;
	BacklashMsArcsecSec = 500;
	AltBacklashArcmin = 0;
	AzBacklashArcmin = 0;
	AltLowLimitDeg = 0;
	AltHighLimitDeg = 0;
	AzLowLimitDeg = 0;
	AzHighLimitDeg = 0;
	GuideArcsecSec = 5;
	FocusMethod = 0;
	ReverseFocusMotor = 0;
	FocusFastStepsSec = 2;
	FocusSlowStepsSec = 1;
	FocusPosition = 0;
	MotorControlMethod = 0;
	MotorWindings = 4;
	InvertOutput = 1;
	ReverseAMotor = 0;
	ReverseZMotor = 0;
	HsRampStyle = 1;
	HsTimerFlag = 1;
	MaxDelay = 1000;
	MinDelay = 300;
	HsDelayX = 2;
	HsRampX = 5;
	InterruptHs = 100;
	HoldReps = 20;
	HsOverVoltageControl = 0;
	MaxConsecutiveSlews = 5;
	MsPowerDownSec = 5;
	PWMRepsTick = 35;
	AvgPWMRepsTickOnFlag = 1;
	MsDelayX = 6;
	MsPause = 500;
	Ms = 10;
	MaxIncrMsPerPWMWasRead = False;
	MsHsToggleIncrMsPerPWMWasRead = False;
	MaxPWM = 100;
	UseComplexPWMFlag = UseComplexPWMZFlag = No;
	/* zero out PWM[] arrays */
	for( Ix = 0; Ix < MaxMs; Ix++)
	{
		PWM[Ix].A = 0;
		PWM[Ix].Z = 0;
		PWMZ[Ix].A = 0;
		PWMZ[Ix].Z = 0;
	}
	PWM[0].A = 100;
	PWM[1].A = 100;
	PWM[2].A = 100;
	PWM[3].A = 100;
	PWM[4].A = 100;
	PWM[5].A = 100;
	PWM[6].A = 85;
	PWM[7].A = 70;
	PWM[8].A = 55;
	PWM[9].A = 35;
	UsePWMZFlag = No;
	SavePWMComp = No;
	/* 5 is max # of motor windings */
	for( Ix = 0; Ix < 5; Ix++)
		PWM_A_Comp[Ix] = PWM_Z_Comp[Ix] = 1.;
	PPortAddr = 1;
	LX200ComPort = 0;
	LX200BaudRate = 9600;
	LX200MotionTimeoutSec = 5;
	LX200SlewHs = 10000L;
	LX200_LongFormat = 0;
	Current.Alt = 0;
	Current.Az = 0;
	AccumMs.A = 0;
	AccumMs.Z = 0;
	StartInitState = 1;
	One.Init = No;
	Two.Init = No;
	Three.Init = No;
	Z1Deg = 0;
	Z2Deg = 0;
	Z3Deg = 0;
	LatitudeDeg = 44;
	CMOS_RTC_Access = 0;
	LongitudeDeg = 123;
	Tz = 8;
	DST = 1;
	TestString = "NoTest";

	MaxPWMFoundFlag = No;

	Input = fopen( ConfigFile, "r");
	if( Input == NULL)
		BadExit( strcat( "Could not open ", ConfigFile));

	fscanf( Input, "%s", Name);
	while( !feof( Input))
	{
		/* if 1st char is ';' or '[' then goto next line */
		if( Name[0] == ';' || Name[0] == '[')
			FReadToNewLine( Input);
		if( (strncmpi( Name, "DefaultBackground", sizeof( Name))) == 0)
			FReadInt( Input, &DefaultBackground);
		if( (strncmpi( Name, "DefaultColor", sizeof( Name))) == 0)
			FReadInt( Input, &DefaultColor);
		if( (strncmpi( Name, "TitleColor", sizeof( Name))) == 0)
			FReadInt( Input, &TitleColor);
		if( (strncmpi( Name, "BorderColor", sizeof( Name))) == 0)
			FReadInt( Input, &BorderColor);
		if( (strncmpi( Name, "MenuColor", sizeof( Name))) == 0)
			FReadInt( Input, &MenuColor);
		if( (strncmpi( Name, "DisplayColor", sizeof( Name))) == 0)
			FReadInt( Input, &DisplayColor);
		if( (strncmpi( Name, "SelectColor", sizeof( Name))) == 0)
			FReadInt( Input, &SelectColor);
		if( (strncmpi( Name, "CurrentColor", sizeof( Name))) == 0)
			FReadInt( Input, &CurrentColor);
		if( (strncmpi( Name, "SelectBackground", sizeof( Name))) == 0)
			FReadInt( Input, &SelectBackground);
		if( (strncmpi( Name, "ConfirmQuit", sizeof( Name))) == 0)
			FReadFlag( Input, &ConfirmQuit);
		if( (strncmpi( Name, "MoveHsMsgDeg", sizeof( Name))) == 0)
			FReadDouble( Input, &MoveHsMsgDeg);
		if( (strncmpi( Name, "InterfacePath", sizeof( Name))) == 0)
			FReadStringToCharCountOrNewLine( Input, InterfacePath, sizeof( Name));
		if( (strncmpi( Name, "UseMouseFlag", sizeof( Name))) == 0)
			FReadFlag( Input, &UseMouseFlag);
		if( (strncmpi( Name, "IACA_Flag", sizeof( Name))) == 0)
			FReadFlag( Input, &IACA_Flag);
		if( (strncmpi( Name, "AutoGEMFlip", sizeof( Name))) == 0)
			FReadFlag( Input, &AutoGEMFlip);
		if( (strncmpi( Name, "AutoGEMFlipFuzzDeg", sizeof( Name))) == 0)
			FReadDouble( Input, &AutoGEMFlipFuzzDeg);
		if( (strncmpi( Name, "Siderostat", sizeof( Name))) == 0)
			FReadFlag( Input, &Siderostat);
		if( (strncmpi( Name, "HomeAltDeg", sizeof( Name))) == 0)
			FReadDouble( Input, &HomeAltDeg);
		if( (strncmpi( Name, "HomeAzDeg", sizeof( Name))) == 0)
			FReadDouble( Input, &HomeAzDeg);
		if( (strncmpi( Name, "MsArcsecSec", sizeof( Name))) == 0)
			FReadInt( Input, &MsArcsecSec);
		if( (strncmpi( Name, "AltFullStepSizeArcsec", sizeof( Name))) == 0)
			FReadDouble( Input, &AltFullStepSizeArcsec);
		if( (strncmpi( Name, "AzFullStepSizeArcsec", sizeof( Name))) == 0)
			FReadDouble( Input, &AzFullStepSizeArcsec);
		if( (strncmpi( Name, "HandpadPresentFlag", sizeof( Name))) == 0)
			FReadFlag( Input, &HandpadPresentFlag);
		if( (strncmpi( Name, "StartingHandpadMode", sizeof( Name))) == 0)
			FReadInt( Input, &StartingHandpadMode);
		if( (strncmpi( Name, "HandpadDesign", sizeof( Name))) == 0)
			FReadInt( Input, &HandpadDesign);
		if( (strncmpi( Name, "HandpadFlipUpDownWithGEMFlip", sizeof( Name))) == 0)
			FReadFlag( Input, &HandpadFlipUpDownWithGEMFlip);
		if( (strncmpi( Name, "BacklashMsArcsecSec", sizeof( Name))) == 0)
			FReadInt( Input, &BacklashMsArcsecSec);
		if( (strncmpi( Name, "AltBacklashArcmin", sizeof( Name))) == 0)
			FReadDouble( Input, &AltBacklashArcmin);
		if( (strncmpi( Name, "AzBacklashArcmin", sizeof( Name))) == 0)
			FReadDouble( Input, &AzBacklashArcmin);
		if( (strncmpi( Name, "AltLowLimitDeg", sizeof( Name))) == 0)
			FReadDouble( Input, &AltLowLimitDeg);
		if( (strncmpi( Name, "AltHighLimitDeg", sizeof( Name))) == 0)
			FReadDouble( Input, &AltHighLimitDeg);
		if( (strncmpi( Name, "AzLowLimitDeg", sizeof( Name))) == 0)
			FReadDouble( Input, &AzLowLimitDeg);
		if( (strncmpi( Name, "AzHighLimitDeg", sizeof( Name))) == 0)
			FReadDouble( Input, &AzHighLimitDeg);
		if( (strncmpi( Name, "GuideArcsecSec", sizeof( Name))) == 0)
			FReadInt( Input, &GuideArcsecSec);
		/* FocusMethod and MotorControlMethods are defined as enum */
		if( (strncmpi( Name, "FocusMethod", sizeof( Name))) == 0)
		{
			FReadInt( Input, &I);
			FocusMethod = I;
		}
		if( (strncmpi( Name, "ReverseFocusMotor", sizeof( Name))) == 0)
			FReadFlag( Input, &ReverseFocusMotor);
		if( (strncmpi( Name, "FocusFastStepsSec", sizeof( Name))) == 0)
			FReadInt( Input, &FocusFastStepsSec);
		if( (strncmpi( Name, "FocusSlowStepsSec", sizeof( Name))) == 0)
			FReadInt( Input, &FocusSlowStepsSec);
		if( (strncmpi( Name, "FocusPosition", sizeof( Name))) == 0)
			FReadInt( Input, &FocusPosition);
		if( (strncmpi( Name, "MotorControlMethod", sizeof( Name))) == 0)
		{
			FReadInt( Input, &I);
			MotorControlMethod = I;
		}
		if( (strncmpi( Name, "InvertOutput", sizeof( Name))) == 0)
			FReadFlag( Input, &InvertOutput);
		if( (strncmpi( Name, "ReverseAMotor", sizeof( Name))) == 0)
			FReadFlag( Input, &ReverseAMotor);
		if( (strncmpi( Name, "ReverseZMotor", sizeof( Name))) == 0)
			FReadFlag( Input, &ReverseZMotor);
		if( (strncmpi( Name, "HsRampStyle", sizeof( Name))) == 0)
			FReadFlag( Input, &HsRampStyle);
		if( (strncmpi( Name, "HsTimerFlag", sizeof( Name))) == 0)
			FReadFlag( Input, &HsTimerFlag);
		if( (strncmpi( Name, "MaxDelay", sizeof( Name))) == 0)
			FReadInt( Input, &MaxDelay);
		if( (strncmpi( Name, "MinDelay", sizeof( Name))) == 0)
			FReadInt( Input, &MinDelay);
		if( (strncmpi( Name, "HsDelayX", sizeof( Name))) == 0)
			FReadInt( Input, &HsDelayX);
		if( (strncmpi( Name, "HsRampX", sizeof( Name))) == 0)
			FReadInt( Input, &HsRampX);
		if( (strncmpi( Name, "InterruptHs", sizeof( Name))) == 0)
			FReadInt( Input, &InterruptHs);
		if( (strncmpi( Name, "HoldReps", sizeof( Name))) == 0)
			FReadInt( Input, &HoldReps);
		if( (strncmpi( Name, "HsOverVoltageControl", sizeof( Name))) == 0)
			FReadInt( Input, &HsOverVoltageControl);
		if( (strncmpi( Name, "MaxConsecutiveSlews", sizeof( Name))) == 0)
			FReadInt( Input, &MaxConsecutiveSlews);
		if( (strncmpi( Name, "MsPowerDownSec", sizeof( Name))) == 0)
			FReadInt( Input, &MsPowerDownSec);
		if( (strncmpi( Name, "PWMRepsTick", sizeof( Name))) == 0)
			FReadInt( Input, &PWMRepsTick);
		if( (strncmpi( Name, "AvgPWMRepsTickOnFlag", sizeof( Name))) == 0)
			FReadFlag( Input, &AvgPWMRepsTickOnFlag);
		if( (strncmpi( Name, "MsDelayX", sizeof( Name))) == 0)
			FReadInt( Input, &MsDelayX);
		if( (strncmpi( Name, "MsPause", sizeof( Name))) == 0)
			FReadInt( Input, &MsPause);
		if( (strncmpi( Name, "Ms", sizeof( Name))) == 0)
			FReadInt( Input, &Ms);
		if( (strncmpi( Name, "MaxIncrMsPerPWM", sizeof( Name))) == 0)
		{
			FReadInt( Input, &MaxIncrMsPerPWM);
			MaxIncrMsPerPWMWasRead = True;
		}
		if( (strncmpi( Name, "MsHsToggleIncrMsPerPWM", sizeof( Name))) == 0)
		{
			FReadInt( Input, &MsHsToggleIncrMsPerPWM);
			MsHsToggleIncrMsPerPWMWasRead = True;
		}
		if( (strncmpi( Name, "MaxPWM", sizeof( Name))) == 0)
		{
			MaxPWMFoundFlag = Yes;
			FReadInt( Input, &MaxPWM);
		}
		if( (strncmpi( Name, "PWM[", 4)) == 0)
			ReadPWMValue();
		if( (strncmpi( Name, "PWMZ[", 5)) == 0)
		{
			UsePWMZFlag = Yes;
			ReadPWMValue();
		}
		if( (strncmpi( Name, "PWM_A", 5)) == 0)
		{
			Ix = Name[6] - 'a';
			if( Ix >= 0 && Ix < 4)
			{
				FReadDouble( Input, &PWM_A_Comp[Ix]);
				SavePWMComp = Yes;
			}
			else
				BadExit( strcat( "Bad PWM_A_? char in ", ConfigFile));
		}
		if( (strncmpi( Name, "PWM_Z", 5)) == 0)
		{
			Ix = Name[6] - 'a';
			if( Ix >= 0 && Ix < 4)
			{
				FReadDouble( Input, &PWM_Z_Comp[Ix]);
				SavePWMComp = Yes;
			}
			else
				BadExit( strcat( "Bad PWM_Z_? char in ", ConfigFile));
		}
		if( (strncmpi( Name, "PPortAddr", sizeof( Name))) == 0)
		{
			FReadUnsigned( Input, &PPortAddr);
			StartPPortAddr = PPortAddr;
		}
		if( (strncmpi( Name, "LX200ComPort", sizeof( Name))) == 0)
			FReadInt( Input, &LX200ComPort);
		if( (strncmpi( Name, "LX200BaudRate", sizeof( Name))) == 0)
			FReadLong( Input, &LX200BaudRate);
		if( (strncmpi( Name, "LX200MotionTimeoutSec", sizeof( Name))) == 0)
			FReadInt( Input, &LX200MotionTimeoutSec);
		if( (strncmpi( Name, "LX200SlewHs", sizeof( Name))) == 0)
			FReadLong( Input, &LX200SlewHs);
		if( (strncmpi( Name, "LX200_LongFormat", sizeof( Name))) == 0)
			FReadFlag( Input, &LX200_LongFormat);
		if( (strncmpi( Name, "Current.Alt", sizeof( Name))) == 0)
		{
			FReadDouble( Input, &Current.Alt);
			Current.Alt *= DegToRad;
		}
		if( (strncmpi( Name, "Current.Az", sizeof( Name))) == 0)
		{
			FReadDouble( Input, &Current.Az);
			Current.Az *= DegToRad;
		}
		if( (strncmpi( Name, "AccumMs.A", sizeof( Name))) == 0)
			FReadLong( Input, &AccumMs.A);
		if( (strncmpi( Name, "AccumMs.Z", sizeof( Name))) == 0)
			FReadLong( Input, &AccumMs.Z);
		if( (strncmpi( Name, "InitOne", sizeof( Name))) == 0)
			FReadPositionToNewLine( Input, &One);
		if( (strncmpi( Name, "InitTwo", sizeof( Name))) == 0)
			FReadPositionToNewLine( Input, &Two);
		if( (strncmpi( Name, "InitThree", sizeof( Name))) == 0)
			FReadPositionToNewLine( Input, &Three);
		if( (strncmpi( Name, "Z1Deg", sizeof( Name))) == 0)
			FReadDouble( Input, &Z1Deg);
		if( (strncmpi( Name, "Z2Deg", sizeof( Name))) == 0)
			FReadDouble( Input, &Z2Deg);
		if( (strncmpi( Name, "Z3Deg", sizeof( Name))) == 0)
			FReadDouble( Input, &Z3Deg);
		if( (strncmpi( Name, "LatitudeDeg", sizeof( Name))) == 0)
			FReadDouble( Input, &LatitudeDeg);
		if( (strncmpi( Name, "CMOS_RTC_Access", sizeof( Name))) == 0)
			FReadFlag( Input, &CMOS_RTC_Access);
		if( (strncmpi( Name, "LongitudeDeg", sizeof( Name))) == 0)
			FReadDouble( Input, &LongitudeDeg);
		if( (strncmpi( Name, "Tz", sizeof( Name))) == 0)
			FReadDouble( Input, &Tz);
		if( (strncmpi( Name, "DST", sizeof( Name))) == 0)
			FReadInt( Input, &DST);
		if( (strncmpi( Name, "TestString", sizeof( Name))) == 0)
			FReadStringToCharCountOrNewLine( Input, TestString, sizeof( Name));
		fscanf( Input, "%s", Name);
	}
	fclose( Input);

	if( !MaxIncrMsPerPWMWasRead)
		MaxIncrMsPerPWM = Ms/2;

	if( !MsHsToggleIncrMsPerPWMWasRead)
		MsHsToggleIncrMsPerPWM = Ms/4;

	if( LongitudeDeg < 0)
		LongitudeDeg = 360. + LongitudeDeg;
}

void ReadPWMValue( void)
{
	char C;
	int Index;

	Ix = 0;
	/* read through first left bracket */
	while( Ix < NameSize)
		if( Name[Ix++] == '[')
			break;
	if( Ix == NameSize)
		if( UsePWMZFlag)
			BadExit( "couldn't find '[' in \"PWMZ[...\"");
		else
			BadExit( "couldn't find '[' in \"PWM[...\"");
	if( Name[Ix] >= '0' && Name[Ix] <= '9')
		Index = Name[Ix] - '0';
	else
		if( UsePWMZFlag)
			BadExit( "couldn't find first Index digit in \"PWMZ[...\"");
		else
			BadExit( "couldn't find first Index digit in \"PWM[...\"");
	Ix++;
	if( Name[Ix] >= '0' && Name[Ix] <= '9')
		Index = Index*10 + Name[Ix] - '0';
	else
		if( Name[Ix] != ']')
			if( UsePWMZFlag)
				BadExit( "couldn't find ']' in \"PWMZ[...]\"");
			else
				BadExit( "couldn't find ']' in \"PWM[...]\"");
	if( UsePWMZFlag)
		FReadInt( Input, &PWMZ[Index].A);
	else
		FReadInt( Input, &PWM[Index].A);
	do
	{
		C = Blank;
		FReadChar( Input, &C);
	}
	while( C != '\n' && C != ':');
	if( C == ':')
	{
		if( UsePWMZFlag)
		{
			UseComplexPWMZFlag = Yes;
			FReadInt( Input, &PWMZ[Index].Z);
		}
		else
		{
			UseComplexPWMFlag = Yes;
			FReadInt( Input, &PWM[Index].Z);
		}
	}
}

void WriteConfig( void)
{
	time_t t;

	Output = fopen( ConfigFile, "w");
	if( Output == NULL)
		BadExit( strcat( "Could not create ", ConfigFile));

	time( &t);
	/* ctime() ends with \n\0 */
	fprintf( Output, ";created on %s", ctime( &t));

	fprintf( Output, "\n[*** display section ***]\n");
	fprintf( Output, "DefaultBackground %d\n", DefaultBackground);
	fprintf( Output, "DefaultColor %d\n", DefaultColor);
	fprintf( Output, "TitleColor %d\n", TitleColor);
	fprintf( Output, "BorderColor %d\n", BorderColor);
	fprintf( Output, "MenuColor %d\n", MenuColor);
	fprintf( Output, "DisplayColor %d\n", DisplayColor);
	fprintf( Output, "SelectColor %d\n", SelectColor);
	fprintf( Output, "CurrentColor %d\n", CurrentColor);
	fprintf( Output, "SelectBackground %d\n", SelectBackground);
	fprintf( Output, "ConfirmQuit %d\n", ConfirmQuit);
	fprintf( Output, "MoveHsMsgDeg %f\n", MoveHsMsgDeg);

	fprintf( Output, "\n[*** interface section ***]\n");
	fprintf( Output, "InterfacePath %s\n", InterfacePath);

	fprintf( Output, "\n[*** control section ***]\n");
	fprintf( Output, "UseMouseFlag %d\n", UseMouseFlag);
	fprintf( Output, "IACA_Flag %d\n", IACA_Flag);

	fprintf( Output, "\n[*** mount section ***]\n");
	fprintf( Output, "AutoGEMFlip %d\n", AutoGEMFlip);
	fprintf( Output, "AutoGEMFlipFuzzDeg %f\n", AutoGEMFlipFuzzDeg);
	fprintf( Output, "Siderostat %d\n", Siderostat);
	fprintf( Output, "HomeAltDeg %f\n", HomeAltDeg);
	fprintf( Output, "HomeAzDeg %f\n", HomeAzDeg);
	fprintf( Output, "MsArcsecSec %d\n", MsArcsecSec);
	fprintf( Output, "AltFullStepSizeArcsec %f\n", AltFullStepSizeArcsec);
	fprintf( Output, "AzFullStepSizeArcsec %f\n", AzFullStepSizeArcsec);

	fprintf( Output, "\n[*** Handpad section ***]\n");
	fprintf( Output, "HandpadPresentFlag %d\n", HandpadPresentFlag);
	fprintf( Output, "StartingHandpadMode %d\n", HandpadFlag);
	fprintf( Output, "HandpadDesign %d\n", HandpadDesign);
	fprintf( Output, "HandpadFlipUpDownWithGEMFlip %d\n", HandpadFlipUpDownWithGEMFlip);

	fprintf( Output, "\n[*** backlash section ***]\n");
	fprintf( Output, "BacklashMsArcsecSec %d\n", BacklashMsArcsecSec);
	fprintf( Output, "AltBacklashArcmin %f\n", NegBacklash.A? -AltBacklashArcmin: AltBacklashArcmin);
	fprintf( Output, "AzBacklashArcmin %f\n", NegBacklash.Z? -AzBacklashArcmin: AzBacklashArcmin);

	fprintf( Output, "\n[*** limit move section ***]\n");
	fprintf( Output, "AltLowLimitDeg %f\n", AltLowLimitDeg);
	fprintf( Output, "AltHighLimitDeg %f\n", AltHighLimitDeg);
	fprintf( Output, "AzLowLimitDeg %f\n", AzLowLimitDeg);
	fprintf( Output, "AzHighLimitDeg %f\n", AzHighLimitDeg);

	fprintf( Output, "\n[*** guide section ***]\n");
	fprintf( Output, "GuideArcsecSec %d\n", GuideArcsecSec);

	fprintf( Output, "\n[*** focus section ***]\n");
	fprintf( Output, "FocusMethod %d\n", FocusMethod);
	fprintf( Output, "ReverseFocusMotor %d\n", ReverseFocusMotor);
	fprintf( Output, "FocusFastStepsSec %d\n", FocusFastStepsSec);
	fprintf( Output, "FocusSlowStepsSec %d\n", FocusSlowStepsSec);
	fprintf( Output, "FocusPosition %d\n", FocusPosition);

	fprintf( Output, "\n[*** motor setup section ***]\n");
	fprintf( Output, "MotorControlMethod %d\n", MotorControlMethod);
	fprintf( Output, "InvertOutput %d\n", InvertOutput);
	fprintf( Output, "ReverseAMotor %d\n", ReverseAMotor);
	fprintf( Output, "ReverseZMotor %d\n", ReverseZMotor);

	fprintf( Output, "\n[*** halfstep section ***]\n");
	fprintf( Output, "HsRampStyle %d\n", HsRampStyle);
	fprintf( Output, "HsTimerFlag %d\n", HsTimerFlag);
	fprintf( Output, "MaxDelay %d\n", MaxDelay);
	fprintf( Output, "MinDelay %d\n", MinDelay);
	fprintf( Output, "HsDelayX %d\n", HsDelayX);
	fprintf( Output, "HsRampX %d\n", HsRampX);
	fprintf( Output, "InterruptHs %d\n", InterruptHs);
	fprintf( Output, "HoldReps %d\n", HoldReps);
	fprintf( Output, "HsOverVoltageControl %d\n", HsOverVoltageControl);
	fprintf( Output, "MaxConsecutiveSlews %d\n", MaxConsecutiveSlews);

	fprintf( Output, "\n[*** microstep section ***]\n");
	fprintf( Output, "MsPowerDownSec %d\n", MsPowerDownSec);
	fprintf( Output, "PWMRepsTick %d\n", PWMRepsTick);
	fprintf( Output, "AvgPWMRepsTickOnFlag %d\n", AvgPWMRepsTickOnFlag);
	fprintf( Output, "MsDelayX %d\n", MsDelayX);
	fprintf( Output, "MsPause %d\n", MsPause);
	fprintf( Output, "Ms %d\n", Ms);
	fprintf( Output, "MaxIncrMsPerPWM %d\n", MaxIncrMsPerPWM);
	fprintf( Output, "MsHsToggleIncrMsPerPWM %d\n", MsHsToggleIncrMsPerPWM);
	fprintf( Output, "MaxPWM %d\n", MaxPWM);
	for( Ix = 0; Ix < Ms; Ix++)
	{
		fprintf( Output, "PWM[%*d] %d", Ix<10?1:2, Ix, PWM[Ix].A);
		if( UseComplexPWMFlag)
			fprintf( Output, " : %d\n", PWM[Ix].Z);
		else
			if( Ix)
				fprintf( Output, " : %d\n", PWM[Ms-Ix].A);
			else
				fprintf( Output, " : 0\n");
	}
	if( UsePWMZFlag)
		for( Ix = 0; Ix < Ms; Ix++)
		{
			fprintf( Output, "PWMZ[%*d] %d", Ix<10?1:2, Ix, PWMZ[Ix].A);
			if( UseComplexPWMZFlag)
				fprintf( Output, " : %d\n", PWMZ[Ix].Z);
			else
				if( Ix)
					fprintf( Output, " : %d\n", PWMZ[Ms-Ix].A);
				else
					fprintf( Output, " : 0\n");
		}
	for( Ix = 0; Ix < MotorWindings; Ix++)
	{
		if( SavePWMComp)
		{
			for( Ix = 0; Ix < MotorWindings; Ix++)
				fprintf( Output, "PWM_A_%c_Comp %1.3f\n", 'a'+Ix, PWM_A_Comp[Ix]);
			for( Ix = 0; Ix < MotorWindings; Ix++)
				fprintf( Output, "PWM_Z_%c_Comp %1.3f\n", 'a'+Ix, PWM_Z_Comp[Ix]);
			break;
		}
	}

	fprintf( Output, "\n[*** parallel port section ***]\n");
	fprintf( Output, "PPortAddr %d\n", StartPPortAddr);

	fprintf( Output, "\n[*** LX200 protocol input section ***]\n");
	fprintf( Output, "LX200ComPort %d\n", LX200ComPort);
	fprintf( Output, "LX200BaudRate %ld\n", LX200BaudRate);
	fprintf( Output, "LX200MotionTimeoutSec %d\n", LX200MotionTimeoutSec);
	fprintf( Output, "LX200SlewHs %ld\n", LX200SlewHs);
	fprintf( Output, "LX200_LongFormat %d\n", LX200_LongFormat);

	fprintf( Output, "\n[*** coordinate conversion section ***]\n");
	fprintf( Output, "Current.Alt %f\n", Current.Alt*RadToDeg);
	fprintf( Output, "Current.Az %f\n", Current.Az*RadToDeg);
	fprintf( Output, "AccumMs.A %ld\n", AccumMs.A);
	fprintf( Output, "AccumMs.Z %ld\n", AccumMs.Z);
	if( One.Init)
	{
		fprintf( Output, "InitOne ");
		FWritePosition( Output, &One, Yes);
	}
	if( Two.Init)
	{
		fprintf( Output, "InitTwo ");
		FWritePosition( Output, &Two, Yes);
	}
	if( Three.Init)
	{
		fprintf( Output, "InitThree ");
		FWritePosition( Output, &Three, Yes);
	}
	fprintf( Output, "Z1Deg %f\n", Z1Deg);
	fprintf( Output, "Z2Deg %f\n", Z2Deg);
	fprintf( Output, "Z3Deg %f\n", Z3Deg);

	fprintf( Output, "\n[*** astronomical times section ***]\n");
	fprintf( Output, "LatitudeDeg %f\n", LatitudeDeg);
	fprintf( Output, "CMOS_RTC_Access %d\n", CMOS_RTC_Access);
	fprintf( Output, "LongitudeDeg %f\n", LongitudeDeg);
	fprintf( Output, "Tz %f\n", Tz);
	fprintf( Output, "DST %d\n", DST);

	fprintf( Output, "\n[*** test section ***]\n");
	fprintf( Output, ";TestString NoTest\n");
	fprintf( Output, "TestString %s\n", TestString);

	fclose( Output);
}

