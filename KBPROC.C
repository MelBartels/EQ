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

void WriteFocusPosition( void)
{
	static int PrevFocusPosition = -9999;

	if( FocusPosition != PrevFocusPosition)
	{
		PrevFocusPosition = FocusPosition;
		TextAttr = DisplayText;
		VidMemXY = DisplayXY[DisplayFocusPosition];
		sprintf( StrBuf, "%5d", FocusPosition);
		WriteStrBufToScreen_f_ptr();
	}
}

void WriteHandpadStatus( void)
{
	TextAttr = DisplayText;
	VidMemXY = DisplayXY[DisplayHandpad];

	sprintf( StrBuf, HPStr[HandpadFlag]);
	WriteStrBufToScreen_f_ptr();

	if( HandpadFlag == GuideStayOn && GuideFlag ||
		 HandpadFlag == ScrollTour && ScrollUnderway ||
		 HandpadFlag == ScrollAutoTour && ScrollUnderway ||
		 HandpadFlag == HandpadAux && HandpadAuxControlOnFlag ||
		 HandpadFlag == HandpadFocus && HandpadFocusMotorControlOnFlag)

		SprintfOn;
	else
		Sprintf2Blanks;

	WriteStrBufToScreen_f_ptr();
}

void WriteTrackByRate( void)
{
	TextAttr = CurrentText;
	VidMemXY = DisplayXY[DisplayTrackByRate];
	if( TrackByRateFlag)
	{
		sprintf( StrBuf, " Microsteps/sec: %2.1f %2.1f Accel (ms/tick): %2.1f %2.1f press 't' to stop ",
		TrackMsPerSec.A, TrackMsPerSec.Z, TrackMsAccel.A, TrackMsAccel.Z);
		WriteStrBufToScreen_f_ptr();
	}
	else
		for( Ix = 0; Ix < 78; Ix++)
			WriteCharToScreen_f_ptr( ' ');
}

void WriteTrackStatus( void)
{
	VidMemXY = DisplayXY[DisplayTrack];
	if( TrackFlag)
	{
		TextAttr = DisplayText;
		sprintf( StrBuf, "On ");
	}
	else
	{
		TextAttr = DisplayText;
		sprintf( StrBuf, "Off");
	}
	WriteStrBufToScreen_f_ptr();
}

void WriteMsArcsecSec( void)
{
	TextAttr = DisplayText;
	VidMemXY = DisplayXY[DisplayMsSpeed];
	sprintf( StrBuf, "%4d", MsArcsecSec);
	WriteStrBufToScreen_f_ptr();
}

void WriteGuideArcsecSec( void)
{
	TextAttr = DisplayText;
	VidMemXY = DisplayXY[DisplayGuideSpeed];
	sprintf( StrBuf, "%3d", GuideArcsecSec);
	WriteStrBufToScreen_f_ptr();
}

void WriteFocusFastSpeed( void)
{
	TextAttr = DisplayText;
	VidMemXY = DisplayXY[DisplayFocusFast];
	sprintf( StrBuf, "%3d", FocusFastStepsSec);
	WriteStrBufToScreen_f_ptr();
}

void WriteFocusSlowSpeed( void)
{
	TextAttr = DisplayText;
	VidMemXY = DisplayXY[DisplayFocusSlow];
	sprintf( StrBuf, "%3d", FocusSlowStepsSec);
	WriteStrBufToScreen_f_ptr();
}

Flag InputEquat( struct Position* P, int X, int Y)
{
	gotoxy( X, Y++);
	printf( "RaHr   ");
	if( !GetInt( &P->RaHMSH.Hr))
		return False;
	gotoxy( X, Y++);
	printf( "RaMin  ");
	if( !GetInt( &P->RaHMSH.Min))
		return False;
	gotoxy( X, Y++);
	printf( "RaSec  "); 
	if( !GetInt( &P->RaHMSH.Sec))
		return False;
	gotoxy( X, Y++); 
	printf( "DecDeg ");
	if( !GetInt( &P->DecDMS.Deg))
		return False; 
	gotoxy( X, Y++);
	printf( "DecMin ");
	if( !GetInt( &P->DecDMS.Min))
		return False;
	gotoxy( X, Y++);
	printf( "DecSec ");
	if( !GetInt( &P->DecDMS.Sec))
		return False;

	P->RaHMSH.Sign = Plus;
	if( P->RaHMSH.Hr < 0)
	{
		P->RaHMSH.Sign = Minus;
		P->RaHMSH.Hr = -P->RaHMSH.Hr;
	}
	if( P->RaHMSH.Min < 0)
	{
		P->RaHMSH.Sign = Minus;
		P->RaHMSH.Min = -P->RaHMSH.Min;
	}
	if( P->RaHMSH.Sec < 0)
	{
		P->RaHMSH.Sign = Minus;
		P->RaHMSH.Sec = -P->RaHMSH.Sec;
	}
	P->RaHMSH.HundSec = 0;

	P->DecDMS.Sign = Plus;
	if( P->DecDMS.Deg < 0)
	{
		P->DecDMS.Sign = Minus;
		P->DecDMS.Deg = -P->DecDMS.Deg; 
	}
	if( P->DecDMS.Min < 0)
	{
		P->DecDMS.Sign = Minus;
		P->DecDMS.Min = -P->DecDMS.Min;
	}
	if( P->DecDMS.Sec < 0)
	{
		P->DecDMS.Sign = Minus;
		P->DecDMS.Sec = -P->DecDMS.Sec;
	}

	CalcRadFromHMSH( &P->Ra, P->RaHMSH);
	CalcRadFromDMS( &P->Dec, P->DecDMS);
	return True;
}

void DisplayLX200CommandsAndCharBuffer( void)
{
	for( Ix = 0; Ix < LX200_Cmd_Array_Size; Ix++)
		sprintf( StrBuf+Ix*LX200_CMD_STR_SIZE, " %s", LX200_Cmd_Str[LX200_Cmd_Array[Ix]]);
	TextAttr = DisplayText;
	VidMemXY = DisplayXY[DisplayLX200];
	WriteStrBufToScreen_f_ptr();
	sprintf( StrBuf, " %04d", LX200_Cmd_Count);
	WriteStrBufToScreen_f_ptr();
	/* highlight current command: 2 chars */
	VidMemXY.X = DisplayXY[DisplayLX200].X + 1 +	LX200_CMD_STR_SIZE * LX200_Cmd_Array_Ix;
	Screen[VidMemXY.Y][VidMemXY.X++].Attr = CurrentText;
	Screen[VidMemXY.Y][VidMemXY.X].Attr = CurrentText;
	/* display LX200 char buffer */
	for( Ix = 0; Ix < LX200QueueSize; Ix++)
		sprintf( StrBuf+Ix, "%c", (char) LX200Queue[Ix]);
	VidMemXY = DisplayXY[DisplayLX200_2];
	TextAttr = DisplayText;
	WriteStrBufToScreen_f_ptr();
	/* highlight last received char from LX200 port */
	VidMemXY.X = 2 + LX200_Ix;
	Screen[VidMemXY.Y][VidMemXY.X].Attr = CurrentText;
}

void DisplayButtonsStatus( void)
{
	sprintf( StrBuf, "%c%c%c%c%c%c%c%c",
	MsSpeed? 'm': 'H',
	Buttons & LeftKey? 'L': ' ',
	Buttons & RightKey? 'R': ' ',
	Buttons & UpKey? 'U': ' ',
	Buttons & DownKey? 'D': ' ',
	Buttons & CWKey? 'C': ' ',
	Buttons & CCWKey? 'W': ' ',
	UpDownButtonsReversedFlag? 'x': ' ');

	VidMemXY = DisplayXY[DisplayButtons];
	TextAttr = DisplayText;
	WriteStrBufToScreen_f_ptr();
}

void DisplayFRFocusAuxActionStatus( void)
{
	char AuxStr[4];
	char FocusStr[4];

	if( DisplayBiDirOut != HoldDisplayBiDirOut)
	{
		HoldDisplayBiDirOut = DisplayBiDirOut;
		if( DisplayBiDirOut & DisplayAux1Bit)
			strcpy( AuxStr, "A1 ");
		else if( DisplayBiDirOut & DisplayAux14Bit)
			strcpy( AuxStr, "A14");
		else if( DisplayBiDirOut & DisplayAux16Bit)
			strcpy( AuxStr, "A16");
		else if( DisplayBiDirOut & DisplayAux17Bit)
			strcpy( AuxStr, "A17");
		else
			strcpy( AuxStr, "   ");
		if( DisplayBiDirOut & DisplayFocusInBit || DisplayBiDirOut & DisplayFocusOutBit)
		{
			FocusStr[0] = 'F';
			if( FocusFastDisplayFlag)
				FocusStr[1] = 'f';
			else
				FocusStr[1] = 's';
			if( DisplayBiDirOut & DisplayFocusInBit)
				if( ReverseFocusMotor)
					FocusStr[2] = '+';
				else
					FocusStr[2] = '-';
			if( DisplayBiDirOut & DisplayFocusOutBit)
				if( ReverseFocusMotor)
					FocusStr[2] = '-';
				else
					FocusStr[2] = '+';
			FocusStr[3] = '\0';
		}
		else
			strcpy( FocusStr, "   ");

		sprintf( StrBuf, "%s%s", AuxStr, FocusStr);
		VidMemXY = DisplayXY[DisplayFRFocusAuxAction];
		TextAttr = DisplayText;
		WriteStrBufToScreen_f_ptr();
	}
	DisplayBiDirOut = 0;
}

void DisplayHotkeys( void)
{
	struct XY HoldXY;

	WriteWindow( HotkeyFrame);
	TextAttr = DefaultText;
	HoldXY.X = HotkeyFrame.Left+1;
	HoldXY.Y = HotkeyFrame.Top;
	VidMemXY = HoldXY;
	sprintf( StrBuf, "` (left apostrophe) or g : Guide.bat");
	WriteStrBufToScreen_f_ptr();
	HoldXY.Y++;
	VidMemXY = HoldXY;
	sprintf( StrBuf, "1 : move to equat");
	WriteStrBufToScreen_f_ptr();
	HoldXY.Y++;
	VidMemXY = HoldXY;
	sprintf( StrBuf, "2 or t : toggles tracking on/off");
	WriteStrBufToScreen_f_ptr();
	HoldXY.Y++;
	VidMemXY = HoldXY;
	sprintf( StrBuf, "3 : saves current equatorial coordinates");
	WriteStrBufToScreen_f_ptr();
	HoldXY.Y++;
	VidMemXY = HoldXY;
	sprintf( StrBuf, "4 : retrieve saved equatorial coordinates into the input fields");
	WriteStrBufToScreen_f_ptr();
	HoldXY.Y++;
	VidMemXY = HoldXY;
	sprintf( StrBuf, "5 : saves current #2 equatorial coordinates");
	WriteStrBufToScreen_f_ptr();
	HoldXY.Y++;
	VidMemXY = HoldXY;
	sprintf( StrBuf, "6 : retrieves saved #2 equatorial coordinates into the input fields");
	WriteStrBufToScreen_f_ptr();
	HoldXY.Y++;
	VidMemXY = HoldXY;
	sprintf( StrBuf, "7 : read external slew file       8 : write external slew file");
	WriteStrBufToScreen_f_ptr();
	HoldXY.Y++;
	VidMemXY = HoldXY;
	sprintf( StrBuf, "c : display current pec data      [ or { : GEM meridian flip");
	WriteStrBufToScreen_f_ptr();
	HoldXY.Y++;
	VidMemXY = HoldXY;
	sprintf( StrBuf, "e : average alt pec files         f : average az pec files");
	WriteStrBufToScreen_f_ptr();
	HoldXY.Y++;
	VidMemXY = HoldXY;
	sprintf( StrBuf, "p : toggles PEC on/off            l : reload PEC.DAT file");
	WriteStrBufToScreen_f_ptr();
	HoldXY.Y++;
	VidMemXY = HoldXY;
	sprintf( StrBuf, "u : show alt guiding -> pec       v : show az guiding -> pec");
	WriteStrBufToScreen_f_ptr();
	HoldXY.Y++;
	VidMemXY = HoldXY;
	sprintf( StrBuf, "y : zero out alt PEC              z : zero out az PEC");
	WriteStrBufToScreen_f_ptr();
	HoldXY.Y++;
	VidMemXY = HoldXY;
	sprintf( StrBuf, "d : data file                     o : restore last object");
	WriteStrBufToScreen_f_ptr();
	HoldXY.Y++;
	VidMemXY = HoldXY;
	sprintf( StrBuf, "h : toggles handpad modes         H : select a handpad mode");
	WriteStrBufToScreen_f_ptr();
	HoldXY.Y++;
	VidMemXY = HoldXY;
	sprintf( StrBuf, "i : display initializations");
	WriteStrBufToScreen_f_ptr();
	HoldXY.Y++;
	VidMemXY = HoldXY;
	sprintf( StrBuf, "m : microstepping speed           q : quits the program");
	WriteStrBufToScreen_f_ptr();
	HoldXY.Y++;
	VidMemXY = HoldXY;
	sprintf( StrBuf, "r : resets to input equat coord   ? : input equat coord");
	WriteStrBufToScreen_f_ptr();
	HoldXY.Y++;
	VidMemXY = HoldXY;
	sprintf( StrBuf, "s : selects scroll file           b : begins scrolling");
	WriteStrBufToScreen_f_ptr();
	HoldXY.Y++;
	VidMemXY = HoldXY;
	sprintf( StrBuf, "< : handpad mode switch left      > : handpad mode switch right");
	WriteStrBufToScreen_f_ptr();
	HoldXY.Y++;
	VidMemXY = HoldXY;
	sprintf( StrBuf, "microstepping : F1-up, F2-down, F3-CCW, F4-CW");
	WriteStrBufToScreen_f_ptr();
	gotoxy( HoldXY.X+20, HoldXY.Y+2);
	ContMsgRoutine();
	RemoveWindow( HotkeyFrame);
}

Flag EnterHsParms( void)
{
	int X, Y;

	X = MsgFrame.Left + 5;
	Y = MsgFrame.Top + 1;

	gotoxy( X, Y++);
	printf( "HsTimerFlag [%1d] ", HsTimerFlag);
	if( !GetFlag( &HsTimerFlag))
		return False;
	gotoxy( X, Y++);
	printf( "MaxDelay [%d] ", MaxDelay);
	if( !GetInt( &MaxDelay))
		return False;
	gotoxy( X, Y++);
	printf( "MinDelay [%d] ", MinDelay);
	if( !GetInt( &MinDelay))
		return False;
	gotoxy( X, Y++);
	printf( "HsDelayX [%d] ", HsDelayX);
	if( !GetInt( &HsDelayX))
		return False;
	gotoxy( X, Y++);
	printf( "HsRampX [%d] ", HsRampX);
	if( !GetInt( &HsRampX))
		return False;
	gotoxy( X, Y++);
	printf( "InterruptHs [%d] ", InterruptHs);
	if( !GetInt( &InterruptHs))
		return False;
	gotoxy( X, Y++);
	printf( "HoldReps [%d] ", HoldReps);
	if( !GetInt( &HoldReps))
		return False;
	return True;
}

void ProcessMenuQuit( void)
{
	if( ConfirmQuit)
	{
		WriteWindow( MsgFrame);
		VidMemXY.X = MsgFrame.Left + 5;
		VidMemXY.Y = MsgFrame.Top + 2;
		gotoxy( VidMemXY.X+=5, VidMemXY.Y);
		printf( "Really want to quit (y/n)? ");
		Response = getch();
		printf( "%c", Response);
		delay( 500);
		if( Response == 'Y' || Response == 'y' || Response == Return)
			QuitFlag = Yes;
		else
			QuitFlag = No;
		RemoveWindow( MsgFrame);
		}
	else
		QuitFlag = Yes;
}

void ProcessMenuSite( void)
{
	double HoldLat, HoldLong;
	int HoldDST;
	double HoldTz;
	long Yr;
	int M;
	int D;
	int h;
	int m;
	double s;
	struct Position Temp;

	HoldLat = LatitudeDeg;
	HoldLong = LongitudeDeg;
	HoldTz = Tz;
	HoldDST = DST;

	WriteWindow( MsgFrame);
	VidMemXY.X = MsgFrame.Left + 2;
	VidMemXY.Y = MsgFrame.Top + 2;
	gotoxy( VidMemXY.X, VidMemXY.Y);
	printf( "latitude (%3.3f)", LatitudeDeg);

	VidMemXY.X = MsgFrame.Left + 2;
	VidMemXY.Y = MsgFrame.Top + 3;
	gotoxy( VidMemXY.X, VidMemXY.Y);
	printf( "longitude (%3.3f)", LongitudeDeg);

	VidMemXY.X = MsgFrame.Left + 2;
	VidMemXY.Y = MsgFrame.Top + 4;
	gotoxy( VidMemXY.X, VidMemXY.Y);
	printf( "timezone (%3.3f)", Tz);

	VidMemXY.X = MsgFrame.Left + 2;
	VidMemXY.Y = MsgFrame.Top + 5;
	gotoxy( VidMemXY.X, VidMemXY.Y);
	printf( "daylight savings time (%d)", DST);

	VidMemXY.X = MsgFrame.Left + 2;
	VidMemXY.Y = MsgFrame.Top + 2;
	gotoxy( VidMemXY.X, VidMemXY.Y);
	printf( "latitude (%3.3f)? ", LatitudeDeg);
	if( GetDouble( &HoldLat)!=UserEscaped)
	{
		VidMemXY.X = MsgFrame.Left + 2;
		VidMemXY.Y = MsgFrame.Top + 3;
		gotoxy( VidMemXY.X, VidMemXY.Y);
		printf( "longitude (%3.3f)? ", LongitudeDeg);
		if( GetDouble( &HoldLong)!=UserEscaped)
		{
			VidMemXY.X = MsgFrame.Left + 2;
			VidMemXY.Y = MsgFrame.Top + 4;
			gotoxy( VidMemXY.X, VidMemXY.Y);
			printf( "timezone (%d)? ", Tz);
			if( GetDouble( &HoldTz)!=UserEscaped)
			{
				VidMemXY.X = MsgFrame.Left + 2;
				VidMemXY.Y = MsgFrame.Top + 5;
				gotoxy( VidMemXY.X, VidMemXY.Y);
				printf( "daylight saving time (%d)? ", DST);
				if( GetInt( &HoldDST)!=UserEscaped)
				{
					LatitudeDeg = HoldLat;
					LongitudeDeg = HoldLong;
					Tz = HoldTz;
					DST = HoldDST;
					getdate( &d);
					Yr = (long) d.da_year;
					M = (int) d.da_mon;
					D = (int) d.da_day;
					gettime( &t);
					h = (int) t.ti_hour;
					m = (int) t.ti_min;
					s = ((double) t.ti_sec) + (t.ti_hund / 100.);
					SetStartBiosClockAndSidTime( Yr, M, D, h, m, s, Tz, DST, LongitudeDeg);
					Current.SidT = SidT;
					if( One.Init)
					{
						Temp = Current;
						Current = One;
						InitMatrix( 1);
						Current = Temp;
					}
				}
			}
		}
	}
	RemoveWindow( MsgFrame);
}

void ProcessMenuDosShell( void)
{
	/* shells to DOS */
	gettext( 1, 1, 80, 25, WinBuffer);
	clrscr();
	_setcursortype( _NORMALCURSOR);
	chdir( DefaultDir);
	SpawnReturn = spawnl( P_WAIT, "c:\\command.com", "", NULL);
	if( SpawnReturn == -1)
		BadExit( "Error from spawnl");
	chdir( DefaultDir);
	_setcursortype( _NOCURSOR);
	clrscr();
	puttext( 1, 1, 80, 25, WinBuffer);
}

void ProcessMenuGuide( void)
{
	if( WriteGuideStartup())
	{
		gettext( 1, 1, 80, 25, WinBuffer);
		clrscr();
		_setcursortype( _NORMALCURSOR);
		chdir( DefaultDir);
		system( "guide.bat");
		chdir( DefaultDir);
		_setcursortype( _NOCURSOR);
		clrscr();
		puttext( 1, 1, 80, 25, WinBuffer);
		ReadGuideStartup();
		/* if tracking on, scope will slew to the new Current Ra and Dec via next call of
		ProcessHPEvents() which calls MoveToCurrentRaDec() */
		if( TrackFlag && Two.Init)
		{
			Current.Ra = In.Ra;
			Current.Dec = In.Dec;
		}
	}
}

void ClearMsgFrameLine( const int Y)
{
	VidMemXY.Y = Y;
	for( VidMemXY.X = MsgFrame.Left+1; VidMemXY.X < MsgFrame.Right-1; VidMemXY.X++)
		WriteCharToScreen( ' ');
}

void ProcessMenuHandpad( void)
{
	HandpadFlag++;
	if( HandpadFlag == MaxHandpadFlag)
		HandpadFlag = HandpadOff;
	WriteHandpadStatus();
}

void ProcessMenuSelectHandpadMode( void)
{
	int row, col;

	WriteWindow( HandpadFrame);
	TextAttr = DefaultText;
	row = col = 0;
	for( Ix = 0; Ix < MaxHandpadFlag; Ix++)
	{
		VidMemXY.X = HandpadFrame.Left + 4 + col*28;
		VidMemXY.Y = HandpadFrame.Top + row;
		sprintf( StrBuf, "%c. %s", Ix + 'a', HPStr[Ix]);
		WriteStrBufToScreen_f_ptr();
		row++;
		if( row >= MaxHandpadFlag/2 + MaxHandpadFlag%2)
		{
			col++;
			row = 0;
		}
	}
	Ix--;
	VidMemXY.X = HandpadFrame.Left + 2;
	VidMemXY.Y = HandpadFrame.Top + 1 + MaxHandpadFlag/2 + MaxHandpadFlag%2;
	sprintf( StrBuf, ">>> select a-%c or any other key to escape <<<", Ix+'a');
	WriteStrBufToScreen_f_ptr();
	Response = getch();
	if( Response >= 'a' && Response <= Ix+'a')
		HandpadFlag = Response - 'a';
	RemoveWindow( HandpadFrame);
	WriteHandpadStatus();
}

void ProcessMenuHandpadLeft( void)
{
	int HoldButtons = Buttons;

	Buttons = LeftKey;
	DisplayButtonsStatus();
	ProcessHPEventsModeSwitch();
	Buttons = HoldButtons;
	KeyboardLeftButton = True;
}

void ProcessMenuHandpadRight( void)
{
	int HoldButtons = Buttons;

	Buttons = RightKey;
	DisplayButtonsStatus();
	ProcessHPEventsModeSwitch();
	Buttons = HoldButtons;
	KeyboardRightButton = True;
}

void ProcessMenuReverseUpDownButtons( void)
{
	UpDownButtonsReversedFlag = !UpDownButtonsReversedFlag;
	if( UpDownButtonsReversedFlag)
		PressKeyToContMsg( "hand paddle up, down buttons are reversed");
	else
		PressKeyToContMsg( "hand paddle up, down buttons back to normal");
}

void ProcessMenuTrack( void)
{
	TrackFlag = !TrackFlag;
	WriteTrackStatus();
	/* if tracking being turned off, save current to input so that upon resumption of tracking, scope
	can slew to last tracking position */
	if( !TrackFlag && Two.Init)
	{
		In = Current;
		DisplayIn( "last Track", NameBlanks);
	}
}

void ProcessMenuMsSpeed( void)
{
	WriteWindow( MsgFrame);
	VidMemXY.X = MsgFrame.Left + 5;
	VidMemXY.Y = MsgFrame.Top + 2;
	gotoxy( VidMemXY.X+=5, VidMemXY.Y);
	printf( "Please enter new MsArcsecSec,");
	gotoxy( VidMemXY.X+=5, VidMemXY.Y+=2);
	printf( "(max = %5d)", MaxMsSpeed);
	gotoxy( VidMemXY.X+=17, VidMemXY.Y);
	GetInt( &MsArcsecSec);
	if( MsArcsecSec > MaxMsSpeed)
		MsArcsecSec = MaxMsSpeed;
	RemoveWindow( MsgFrame);
	InitMsTickVars( MsArcsecSec);
	WriteMsArcsecSec();
}

void ProcessMenuGuideSpeed( void)
{
	WriteWindow( MsgFrame);
	VidMemXY.X = MsgFrame.Left + 3;
	VidMemXY.Y = MsgFrame.Top + 2;
	gotoxy( VidMemXY.X, VidMemXY.Y);
	printf( "Please enter new GuideArcsecSec");
	gotoxy( VidMemXY.X += 33, VidMemXY.Y);
	GetInt( &GuideArcsecSec);
	RemoveWindow( MsgFrame);
	WriteGuideArcsecSec();
}

void ProcessMenuLX200Control( void)
{
	if( HoldLX200ComPort)
	{
		WriteWindow( MsgFrame);
		gotoxy( MsgFrame.Left + 2, MsgFrame.Top + 2);
		printf( "1. turn off LX200 commands");
		gotoxy( MsgFrame.Left + 2, MsgFrame.Top + 3);
		printf( "2. turn on LX200 commands");
		gotoxy( MsgFrame.Left + 2, MsgFrame.Top + 4);
		printf( "3. toggle LX200 display");
		gotoxy( MsgFrame.Left + 2, MsgFrame.Top + 5);
		printf( "   please select 1, 2, 3, or any other key to abort ");
		Response = getch();
		switch( Response)
		{
			case '1':
				if( LX200ComPort)
				{
					CloseSerial( LX200ComPort);
					LX200ComPort = 0;
					BlankOutLX200DisplayAreas();
					DisplayLX200CommandStatus();
				}
				else
				{
					gotoxy( MsgFrame.Left + 2, MsgFrame.Top + 6);
					printf( "LX200 already turned off.   ");
					gotoxy( MsgFrame.Left + 2, MsgFrame.Top + 7);
					ContMsgRoutine();
				}
				break;
			case '2':
				if( HoldLX200ComPort && !LX200ComPort)
				{
					LX200ComPort = HoldLX200ComPort;
					InitLX200Input();
					BlankOutLX200DisplayAreas();
					DisplayLX200CommandStatus();
				}
				else
				{
					gotoxy( MsgFrame.Left + 2, MsgFrame.Top + 6);
					printf( "LX200 already turned on.   ");
					gotoxy( MsgFrame.Left + 2, MsgFrame.Top + 7);
					ContMsgRoutine();
				}
				break;
			case '3':
				if( LX200ComPort)
				{
					LX200DisplayFlag = !LX200DisplayFlag;
					if( !LX200DisplayFlag)
						BlankOutLX200DisplayAreas();
				}
				else
				{
					gotoxy( MsgFrame.Left + 2, MsgFrame.Top + 6);
					printf( "LX200 turned off.   ");
					gotoxy( MsgFrame.Left + 2, MsgFrame.Top + 7);
					ContMsgRoutine();
				}
		}
		RemoveWindow( MsgFrame);
	}
	else
		PressKeyToContMsg( "no LX200 comm port defined");
}

void BlankOutLX200DisplayAreas( void)
{
	/* blank out display areas */
	for( Ix = 0; Ix < LX200_Cmd_Array_Size * (LX200_CMD_STR_SIZE + 1) + 3; Ix++)
		sprintf( StrBuf+Ix, "%c", Blank);
	VidMemXY = DisplayXY[DisplayLX200];
	TextAttr = DisplayText;
	WriteStrBufToScreen_f_ptr();

	for( Ix = 0; Ix < LX200QueueSize; Ix++)
		sprintf( StrBuf+Ix, "%c", Blank);
	VidMemXY = DisplayXY[DisplayLX200_2];
	TextAttr = DisplayText;
	WriteStrBufToScreen_f_ptr();
}

void DisplayLX200CommandStatus( void)
{
	if( LX200ComPort)
		sprintf( StrBuf, "LX200 commands on");
	else
		sprintf( StrBuf, "LX200 commands off");
	VidMemXY = DisplayXY[DisplayLX200];
	TextAttr = DisplayText;
	WriteStrBufToScreen_f_ptr();
}

void ProcessMenuChangeBacklashParms( void)
{
	WriteWindow( MsgFrame);
	gotoxy( MsgFrame.Left + 2, MsgFrame.Top + 1);
	printf( "AltBacklashArcmin currently is %3.2f", AltBacklashArcmin);
	gotoxy( MsgFrame.Left + 2, MsgFrame.Top + 2);
	printf( "enter new value ");
	if( GetDouble( &AltBacklashArcmin))
	{
		gotoxy( MsgFrame.Left + 2, MsgFrame.Top + 3);
		printf( "AzBacklashArcmin currently is %3.2f", AzBacklashArcmin);
		gotoxy( MsgFrame.Left + 2, MsgFrame.Top + 4);
		printf( "enter new value ");
		if( GetDouble( &AzBacklashArcmin))
		{
      	/* if backlash, then make sure that speed is > 0 */
			while( True)
			{
				gotoxy( MsgFrame.Left + 2, MsgFrame.Top + 5);
				printf( "BacklashMsArcsecSec currently is %4d", BacklashMsArcsecSec);
				gotoxy( MsgFrame.Left + 2, MsgFrame.Top + 6);
				printf( "enter new value ");
				if( GetInt( &BacklashMsArcsecSec))
				{
					if( (AltBacklashArcmin == 0 && AzBacklashArcmin == 0) || BacklashMsArcsecSec > 0)
					{
						CalcBacklashVars();
						break;
					}
				}
				else
					break;
			}
		}
	}
	RemoveWindow( MsgFrame);
}

void ProcessMenuAutoGEMFlipOnOff( void)
{
	WriteWindow( MsgFrame);

	VidMemXY.X = MsgFrame.Left + 3;
	VidMemXY.Y = MsgFrame.Top + 3;
	gotoxy( VidMemXY.X, VidMemXY.Y);
	printf( "AutoGEMFlip is %s. Turn it %s (y/n)? ", AutoGEMFlip? "on":"off", AutoGEMFlip? "off":"on");
	Response = getch();
	printf( "%c", Response);
	delay( 250);
	if( Response == 'Y' || Response == 'y' || Response == Return)
		AutoGEMFlip = !AutoGEMFlip;
	RemoveWindow( MsgFrame);
}

void ProcessMenuInputEquat( void)
{
	struct XY HoldXY;

	WriteWindow( MsgFrame);
	HoldXY.X = MsgFrame.Left + 5;
	HoldXY.Y = MsgFrame.Top + 2;
	gotoxy( HoldXY.X, HoldXY.Y);
	printf( "Please enter Input ");
	HoldXY.X = wherex();
	gotoxy( HoldXY.X, HoldXY.Y);
	InputEquat( &In, HoldXY.X, HoldXY.Y);
	RemoveWindow( MsgFrame);
	DisplayIn( "input equat", NameBlanks);
}

void ProcessMenuOffsetEquat( void)
{
	struct XY HoldXY;

	WriteWindow( MsgFrame);
	HoldXY.X = MsgFrame.Left + 3;
	HoldXY.Y = MsgFrame.Top + 2;
	gotoxy( HoldXY.X, HoldXY.Y);
	printf( "Enter Offset from Current ");
	HoldXY.X = wherex();
	gotoxy( HoldXY.X, HoldXY.Y);
	InputEquat( &Offset, HoldXY.X, HoldXY.Y);
	RemoveWindow( MsgFrame);
	In.Ra = Current.Ra + Offset.Ra;
	In.Dec = Current.Dec + Offset.Dec;
	ValidRa( &In);
	GetHMSH( In.Ra*RadToHundSec + .5, &In.RaHMSH);
	GetDMS( In.Dec*RadToArcsec + .5, &In.DecDMS);
	DisplayIn( "offset equat", NameBlanks);
}

void ProcessMenuMoveEquat( void)
{
	/* scope will slew or track to the new Current Ra and Dec next time through function via
	ProcessHPEvents() then MoveToCurrentRaDec() */
	if( Two.Init)
	{
		if( !TrackFlag)
		{
			TrackFlag = Yes;
			WriteTrackStatus();
		}
		Current.Ra = In.Ra;
		Current.Dec = In.Dec;
		SlewBeep = SlewReady;
	}
}

void ProcessMenuMoveHome( void)
{
	/* Delta = new - old */
	Delta.A = HomeAltDeg*DegToRad - Current.Alt;
	Delta.Z = HomeAzDeg*DegToRad - Current.Az;
	SetDirDistanceStepsThenMove();
	PauseUntilNewSidTime();
	HPEventGetEquat();
}

void ProcessMenuResetEquat( void)
{
	/* set current equat to input equat */
	Current.Ra = In.Ra;
	Current.Dec = In.Dec;
	/* get current altaz */
	GetAltaz();
	CheckSiderostatAltaz();
	/* set scope to current altaz */
	SetAccumMsToCurrentAltaz();
	/* get new current equat */
	HPEventGetEquat();
	/* redo encoder vars */
}

void ProcessMenuResetHome( void)
{
	Current.Alt = HomeAltDeg*DegToRad;
	Current.Az = HomeAzDeg*DegToRad;
	SetAccumMsToCurrentAltaz();
	HPEventGetEquat();
}

/* flip a german equatorial mount across the meridian:
the Ra tracking movement will continue to move in the same direction but the Dec movement will be
reversed; this is handled in low level GetAltaz() and GetEquat() routines so as to not disturb
Current.Alt and Current.Az values */

void DisplayGEMFlipStatus( void)
{
	sprintf( StrBuf, "%s", GEMflippedFlag? "On  (W side aimed E)": "Off (E side aimed W)");
	VidMemXY = DisplayXY[DisplayGEMFlip];
	TextAttr = DisplayText;
	WriteStrBufToScreen_f_ptr();
}

void ProcessMenuGEMMeridianFlip( void)
{
	struct XY HoldXY;

	if( TrackFlag)
	{
		TrackFlag = No;
		WriteTrackStatus();
	}
	ProcessGEMFlippedFlag();

	/* if called from ProcessMenuResponse() with keypress of [ or { */
	if (Response == '[' || Response == '{')
		ProcessMenuResetEquat();
	else
	{
		WriteWindow( MsgFrame);
		HoldXY.X = MsgFrame.Left + 3;
		HoldXY.Y = MsgFrame.Top + 2;
		gotoxy( HoldXY.X, HoldXY.Y++);
		printf( "Tracking has been turned off.");
		gotoxy( HoldXY.X, HoldXY.Y++);
		printf( "The scope has been flipped across the meridian.");
		gotoxy( HoldXY.X, HoldXY.Y++);
		printf( "Reset to equatorial coordinates (y/n)? ");
		Response = getch();
		printf( "%c", Response);
		if( Response == 'Y' || Response == 'y')
			ProcessMenuResetEquat();
		gotoxy( HoldXY.X, HoldXY.Y++);
		printf( "Reverse up/down handpad buttons (y/n)? ");
		Response = getch();
		printf( "%c", Response);
		if( Response == 'Y' || Response == 'y')
			UpDownButtonsReversedFlag = ! UpDownButtonsReversedFlag;
		RemoveWindow( MsgFrame);
	}
}

void ProcessMenuHomeCoord( void)
{
	struct Position HoldDeg;

	WriteWindow( MsgFrame);

	HoldDeg.Alt = HomeAltDeg;
	HoldDeg.Az = HomeAzDeg;
	gotoxy( MsgFrame.Left + 3, MsgFrame.Top + 2);
	printf( "Home Altitude Degrees ");
	if( !GetDouble( &HomeAltDeg))
		HomeAltDeg = HoldDeg.Alt;
	else
	{
		gotoxy( MsgFrame.Left + 3, MsgFrame.Top + 4);
		printf( "Home Azimuth Degrees ");
		if( !GetDouble( &HomeAzDeg))
		{
			HomeAzDeg = HoldDeg.Az;
			HomeAltDeg = HoldDeg.Alt;
		}
	}
	RemoveWindow( MsgFrame);
}

void ProcessMenuSelection( void)
{
	ConsecutiveSlews = 0;
	AlignMs();
	SetCurrentAltazToAccumMs();
	switch( Response)
	{
		case MenuQuit:
			ProcessMenuQuit();
			break;
		case MenuSite:
			ProcessMenuSite();
			break;
		case MenuDataFile:
			MenuCoordFile();
			break;
		case MenuDataFileClosest:
			if( ProcessMenuDataFileClosest( FilenameNotSet, ExactInputFieldsMatchOK))
				;
			else
				PressKeyToContMsg( "Could not find match");
			break;
		case MenuDataFileClosestNotSame:
			if( ProcessMenuDataFileClosest( FilenameNotSet, AvoidMatchInputFields))
				;
			else
				PressKeyToContMsg( "Could not find match");
			break;
		case MenuSearchDataFiles:
			ProcessMenuSearch();
			break;
		case MenuGrandTourClosest:
			if( Two.Init)
				GetGrandTourRecClosestCurrentEquat();
			else
				PressKeyToContMsg( "Must enter at least 2 inits");
			break;
		case MenuScrollTour:
			chdir( DataDir);
			if( SelectDataFilename( ScrollFile))
			{
				strcpy( ScrollFilename, Filename);
				LoadScrollFile();
			}
			GetCurDir( DataDir);
			chdir( DefaultDir);
			break;
		case MenuGuide:
			if( UseMouseFlag)
				CloseMouseControl();
			ProcessMenuGuide();
			if( UseMouseFlag)
				InitMouseControl();
			break;
		case MenuHotkeys:
			DisplayHotkeys();
			break;
		case MenuHandpad:
			ProcessMenuHandpad();
			break;
		case MenuSelectHandpadMode:
			ProcessMenuSelectHandpadMode();
			break;
		case MenuHandpadLeft:
			ProcessMenuHandpadLeft();
			break;
		case MenuHandpadRight:
			ProcessMenuHandpadRight();
			break;
		case MenuReverseUpDownButtons:
			ProcessMenuReverseUpDownButtons();
			break;
		case MenuTrack:
			if( TrackByRateFlag)
			{
				TrackByRateFlag = No;
				WriteTrackByRate();
			}
			else
				ProcessMenuTrack();
			break;
		case MenuMsSpeed:
			ProcessMenuMsSpeed();
			break;
		case MenuGuideSpeed:
			ProcessMenuGuideSpeed();
			break;
		case MenuDecMotor:
			ProcessMenuDecMotor();
			break;
		case MenuChangeBacklashParms:
			ProcessMenuChangeBacklashParms();
			break;
		case MenuAutoGEMFlipOnOff:
			ProcessMenuAutoGEMFlipOnOff();
			break;
		case MenuLX200Control:
			ProcessMenuLX200Control();
			break;
		case MenuDisplayInit:
			DisplayInit();
			break;
		case MenuInputEquat:
			ProcessMenuInputEquat();
			break;
		case MenuOffsetEquat:
			ProcessMenuOffsetEquat();
			break;
		case MenuSav1:
			ProcessSav1();
			break;
		case MenuRes1:
			ProcessRes1();
			break;
		case MenuSav2:
			ProcessSav2();
			break;
		case MenuRes2:
			ProcessRes2();
			break;
		case MenuRestoreLastObject:
			ProcessMenuRestoreLastObject();
			break;
		case MenuMoveEquat:
			ProcessMenuMoveEquat();
			break;
		case MenuMoveHome:
			ProcessMenuMoveHome();
			break;
		case MenuMoveGEMFlip:
			ProcessMenuMoveGEMFlip();
			break;
		case MenuFocusFastSpeed:
			ProcessMenuFocusFastSpeed();
			break;
		case MenuFocusSlowSpeed:
			ProcessMenuFocusSlowSpeed();
			break;
		case MenuMoveFocus:
			if( FocusMethod == FocusMethod_Pulse_1_14 || FocusMethod == FocusMethod_Pulse_16_17)
				ProcessMenuMoveFocus();
			else
				PressKeyToContMsg( BadFocusMethodMsgStrPtr);
			break;
		case MenuResetFocus:
			if( FocusMethod == FocusMethod_Pulse_1_14 || FocusMethod == FocusMethod_Pulse_16_17)
				ProcessMenuResetFocus();
			else
				PressKeyToContMsg( BadFocusMethodMsgStrPtr);
			break;
		case MenuResetEquat:
			ProcessMenuResetEquat();
			break;
		case MenuResetHome:
			ProcessMenuResetHome();
			break;
		case MenuGEMMeridianFlip:
			ProcessMenuGEMMeridianFlip();
			break;
		default:
			ProcessHPEvents();
	}
}

void ProcessMenuMoveGEMFlip( void)
{
	double Pole;
	double DecDistance, RaDistance;
	struct AZLong HoldSteps;
	struct AZFlag HoldDir;

	Steps.Z = 0;
	if( LatitudeDeg >= 0)
	{
		Pole = QtrRev;
		Dir.A = CW;
	}
	else
	{
		Pole = -QtrRev;
		Dir.A = CCW;
	}
	DecDistance = Pole - Current.Alt;
	if( DecDistance < 0)
	{
		DecDistance = -DecDistance;
		if( Dir.A == CW)
			Dir.A = CCW;
		else
			Dir.A = CW;
	}
	Steps.A = DecDistance / HsRad.A;
	HoldSteps = Steps;
	HoldDir = Dir;

	for( Ix = 0; Ix < 4; Ix++)
	{
		sound( 400);
		delay( 200);
		nosound();
		delay( 200);
	}
	TextAttr = CurrentText;
	VidMemXY = DisplayXY[DisplayGEMFlipMove];
	sprintf( StrBuf, "moving to pole in DEC: %ld hs; %3.3f deg", Steps.A, DecDistance*RadToDeg);
	WriteStrBufToScreen_f_ptr();

	KBEventMoveHs();
	PauseUntilNewSidTime();
	HPEventGetEquat();

	if( !KeyStroke)
	{
		/* can move either west or east whether flipped or not: to avoid cable wrap, traverse in
		opposite direction when moving from flipped to not flipped */
		TextAttr = CurrentText;
		VidMemXY = DisplayXY[DisplayGEMFlipMove];
		if( DisplayOpeningMsgs && !LX200ComPort)
		{
			sprintf( StrBuf, "move CW or CCW across the meridian (c/w)?" );
			WriteStrBufToScreen_f_ptr();
			Response = getch();
			if( Response == 'c' || Response == 'C')
				Dir.Z = CW;
			else
				Dir.Z = CCW;
		}
		else
			if( GEMflippedFlag)
				Dir.Z = CCW;
			else
				Dir.Z = CW;
		RaDistance = HalfRev;
		Steps.Z = RaDistance / HsRad.Z;
		Steps.A = 0;

		for( Ix = 0; Ix < 3; Ix++)
		{
			sound( 400);
			delay( 200);
			nosound();
			delay( 200);
		}
		TextAttr = CurrentText;
		VidMemXY = DisplayXY[DisplayGEMFlipMove];
		sprintf( StrBuf, "moving %s across meridian in RA: %ld hs; %3.3f deg", Dir.Z==CW? "west": "east",
		Steps.Z, RaDistance*RadToDeg);
		WriteStrBufToScreen_f_ptr();

		KBEventMoveHs();
		PauseUntilNewSidTime();
		HPEventGetEquat();

		if( !KeyStroke)
		{
			Steps = HoldSteps;
			Dir.A = HoldDir.A;

			for( Ix = 0; Ix < 2; Ix++)
			{
				sound( 400);
				delay( 200);
				nosound();
				delay( 200);
			}

			TextAttr = CurrentText;
			VidMemXY = DisplayXY[DisplayGEMFlipMove];
			sprintf( StrBuf, "moving from pole in DEC: %ld hs; %3.3f deg", Steps.A, DecDistance*RadToDeg);
			WriteStrBufToScreen_f_ptr();

			KBEventMoveHs();
			PauseUntilNewSidTime();
			HPEventGetEquat();

			for( Ix = 0; Ix < 1; Ix++)
			{
				sound( 400);
				delay( 200);
				nosound();
				delay( 200);
			}
		}
		else
			ProcessMenuMoveGEMFlipCancel();
	}
	else
		ProcessMenuMoveGEMFlipCancel();

	DrawScreenLine( MenuSubLine3Y);
	ProcessGEMFlippedFlag();
}

void ProcessMenuMoveGEMFlipCancel( void)
{
	while( KeyStroke)
		getch();
	PressKeyToContMsg( "GEM Flip move cancelled");
	/* function inverts TrackFlag, leading to tracking being turned off */
	TrackFlag = On;
	ProcessMenuTrack();
}

void ProcessMenuFocusFastSpeed( void)
{
	double focusspeed;

	WriteWindow( MsgFrame);
	VidMemXY.X = MsgFrame.Left + 2;
	VidMemXY.Y = MsgFrame.Top + 3;
	gotoxy( VidMemXY.X, VidMemXY.Y);
	printf( "Please enter fast focus steps per second ");
	if( GetDouble( &focusspeed))
		FocusFastStepsSec = focusspeed;
	RemoveWindow( MsgFrame);
	if( FocusFastStepsSec < 1)
		PressKeyToContMsg( "Fast focus speed must be > 0, setting it to 1");
	if( FocusFastStepsSec > 500)
	{
		PressKeyToContMsg( "Fast focus speed cannot exceed 500, setting it to 500");
		FocusFastStepsSec = 500;
	}
	WriteFocusFastSpeed();
}

void ProcessMenuFocusSlowSpeed( void)
{
	double focusspeed;

	WriteWindow( MsgFrame);
	VidMemXY.X = MsgFrame.Left + 2;
	VidMemXY.Y = MsgFrame.Top + 3;
	gotoxy( VidMemXY.X, VidMemXY.Y);
	printf( "Please enter slow focus steps per second ");
	if( GetDouble( &focusspeed))
		FocusSlowStepsSec = focusspeed;
	RemoveWindow( MsgFrame);
	if( FocusSlowStepsSec < 1)
		PressKeyToContMsg( "Slow focus speed must be > 0, setting it to 1");
	if( FocusSlowStepsSec > 500)
	{
		PressKeyToContMsg( "Slow focus speed cannot exceed 500, setting it to 500");
		FocusSlowStepsSec = 500;
	}
	WriteFocusSlowSpeed();
}

void ProcessMenuMoveFocus( void)
{
	int NewFocusPosition;
	int StepsToMove, StepsMoved;
	int msdelay;

	WriteWindow( MsgFrame);
	VidMemXY.X = MsgFrame.Left + 2;
	VidMemXY.Y = MsgFrame.Top + 3;
	gotoxy( VidMemXY.X, VidMemXY.Y);
	printf( "Please enter focus position to move to ");
	if( GetInt( &NewFocusPosition))
	{
		_setcursortype( _NOCURSOR);
		TextAttr = DisplayText;
		SetFocusControlLines();
		/* set focus motor direction */
		StepsToMove = NewFocusPosition - FocusPosition;
		if( StepsToMove < 0)
		{
			StepsToMove = -StepsToMove;
			ReverseFocusDir();
		}
		StepsMoved = 0;
		FocusFastDisplayFlag = True;
		msdelay = 1000 / (FocusFastStepsSec * 2);
		/* get state of port so as to not disturb unused parallel port pins */
		GetUnusedPPortLines();
		/* setup handpad vars */
		InitHandpad = Handpad;
		HandpadOKFlag = Yes;
		while( StepsMoved < StepsToMove && HandpadOKFlag && !KeyStroke)
		{
			delay( msdelay);
			/* raise focus motor pulse */
			BiDirOutNibbleValue = UnusedFocusPPortLines + FocusPulseBit + FocusDir;
			BiDirOutNibble();
			if( FocusDir)
			{
				FocusDiagPulseHighCW++;
				FocusPosition++;
			}
			else
			{
				FocusDiagPulseHighCCW++;
				FocusPosition--;
			}
			delay( msdelay);
			/* lower focus motor pulse */
			BiDirOutNibbleValue = UnusedFocusPPortLines + FocusDir;
			BiDirOutNibble();
			FocusDiagPulseLow++;
			SetHandpadOKFlag();
			StepsMoved++;
			/* display focus position */
			VidMemXY.X = MsgFrame.Left + 41;
			VidMemXY.Y = MsgFrame.Top + 3;
			gotoxy( VidMemXY.X, VidMemXY.Y);
			sprintf( StrBuf, "%04d", FocusPosition);
			WriteStrBufToScreen_f_ptr();
		}
		_setcursortype( _NORMALCURSOR);
	}
	if( KeyStroke)
		getch();
	VidMemXY.X = MsgFrame.Left + 2;
	VidMemXY.Y = MsgFrame.Top + 5;
	gotoxy( VidMemXY.X, VidMemXY.Y);
	printf( "Move finished. ");
	ContMsgRoutine();
	RemoveWindow( MsgFrame);
}

void ProcessMenuResetFocus( void)
{
	double focuspos;

	WriteWindow( MsgFrame);
	VidMemXY.X = MsgFrame.Left + 2;
	VidMemXY.Y = MsgFrame.Top + 3;
	gotoxy( VidMemXY.X, VidMemXY.Y);
	printf( "Please enter new focuser position ");
	if( GetDouble( &focuspos))
		FocusPosition = focuspos;
	RemoveWindow( MsgFrame);
	WriteFocusPosition();
}

void ProcessSav1( void)
{
	/* saves current coordinates to SavedIn */
	SavedIn = Current;
	ProcessHPEvents();
}

void ProcessRes1( void)
{
	/* retrieves SavedIn coordinates and puts them in Input fields */
	In = SavedIn;
	DisplayIn( "saved input #1", NameBlanks);
	ProcessHPEvents();
}

void ProcessSav2( void)
{
	/* saves current coordinates to SavedIn2 */
	SavedIn2 = Current;
	ProcessHPEvents();
}

void ProcessRes2( void)
{
	/* retrieves SavedIn2 coordinates and puts them in Input fields */
	In = SavedIn2;
	DisplayIn( "saved input #2", NameBlanks);
	ProcessHPEvents();
}

void ProcessMenuDecMotor( void)
{
	WriteWindow( MsgFrame);
	VidMemXY.X = MsgFrame.Left + 3;
	VidMemXY.Y = MsgFrame.Top + 2;
	gotoxy( VidMemXY.X, VidMemXY.Y++);
	if( EnableMotor.A)
		printf( "Disable ");
	else
		printf( "Enable ");
	printf( "Declination motor microstepping (y/n)? ");
	Response = getch();
	if( Response == 'Y' || Response == 'y' || Response == Return)
		EnableMotor.A = ! EnableMotor.A;
	RemoveWindow( MsgFrame);
}


