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

void InitKBEvent( void)
{
	int IxA;

	SlewBeep = SlewOff;
	SlewBeepSound = 0;
	QuitFlag = No;
	LX200DisplayFlag = No;
	HoldLX200ComPort = LX200ComPort;
	FirstLinkFname = NULL;
	RaDecInit = NULL;
	FirstRaDecInit = NULL;
	FirstScroll = NULL;
	LastScroll = NULL;
	CurrentScroll = NULL;
	ScrollLoaded = No;
	ScrollUnderway = No;
	AutoGEMFlipFuzzRad = AutoGEMFlipFuzzDeg*DegToRad;
	ETask = HTask = LTask = 0;
	KeyboardLeftButton = KeyboardRightButton = False;
	DataFileNameSet = No;
	FocusFastDisplayFlag = True;

	GetCurDir( DefaultDir);
	GetCurDir( DataDir);

	strcpy( 	BadFocusMethodMsgStrPtr, "Not appropriate FocusMethod");

	strcpy( Filename, "bstars.dat");

	strcpy( GuideStartupMarFilePtr, InterfacePath);
	strcat( GuideStartupMarFilePtr, GuideStartupMarFile);
	strcpy( ScopeStartupMarFilePtr, InterfacePath);
	strcat( ScopeStartupMarFilePtr, ScopeStartupMarFile);

	strcpy( FullyQualifiedSlewFile, InterfacePath);
	strcat( FullyQualifiedSlewFile, SlewFile);
	strcpy( FullyQualifiedSlewOutFile, InterfacePath);
	strcat( FullyQualifiedSlewOutFile, SlewOutFile);

	OutGuideFilePtr = fopen( OutGuideFile, "w");
	if( OutGuideFilePtr == NULL)
		BadExit( strcat( "Could not open ", OutGuideFile));

	CalcHsMsgSteps();

	InitDisplayXY();

	MsgFrame.Left = 12;
	MsgFrame.Top = 9;
	MsgFrame.Right = 68;
	MsgFrame.Bottom = 17;
	MsgFrame.Title = " Message ";

	CoordFrame.Left = 1;
	CoordFrame.Top = 5;
	CoordFrame.Right = 80;
	CoordFrame.Bottom = 19;
	CoordFrame.Title = " Coordinates ";

	DataFileFrame.Left = 10;
	DataFileFrame.Top = 5;
	DataFileFrame.Right = 70;
	DataFileFrame.Bottom = 20;
	DataFileFrame.Title = " Data Files ";

	InitFrame.Left = 10;
	InitFrame.Top = 9;
	InitFrame.Right = 70;
	InitFrame.Bottom = 23;
	InitFrame.Title = " Initialization ";

	HotkeyFrame.Left = 4;
	HotkeyFrame.Top = 1;
	HotkeyFrame.Right = 76;
	HotkeyFrame.Bottom = 25;
	HotkeyFrame.Title = " Hotkeys ";

	HandpadFrame.Left = 10;
	HandpadFrame.Top = 6;
	HandpadFrame.Right = 70;
	HandpadFrame.Bottom = 20;
	HandpadFrame.Title = " Handpad Modes ";

	MainFrame.Left = 1;
	MainFrame.Top = 1;
	MainFrame.Right = MaxX;
	MainFrame.Bottom = MaxY;
	MainFrame.Title = " EQUATORIAL SCOPE, compiled Feb 23, 2003, by Mel Bartels ";
	MainStr_14 = "Input Ra:           Dec:                    Focus";
	MainStr_16 = "File                          Track         MeridianFlip";
	MainStr_17 = "Object                        Handpad";
	MainStr_18 = "Sidereal Time                 Mouse         Microstep    \"/sec  Guide   \"/sec";
	MainStr_19 = "Date/Time                     FastFocus    /sec   SlowFocus    /sec";
	MainStr_20 = "                              PWM         Ms";
	MainStr_22 = "Slew";
	MainStr_23 = "LX200";

	Ix = 0;
	SlewText[Ix++] =	"RampU";
	SlewText[Ix++] =	"MaxU ";
	SlewText[Ix++] =	"MaxD ";
	SlewText[Ix++] =	"RampD";
	SlewText[Ix++] =	"Done ";

	Ix = 0;
	AbortText[Ix++] = "           ";
	AbortText[Ix++] =	"HPad-Abort ";
	AbortText[Ix++] =	"Mouse-Abort";
	AbortText[Ix++] =	"KeyB-Abort ";
	AbortText[Ix++] =	"LX200-Abort";
	AbortText[Ix++] =	"AltLo-Abort";
	AbortText[Ix++] =	"AltHi-Abort";
	AbortText[Ix++] =	"AzLo-Abort ";
	AbortText[Ix++] =	"AzHi-Abort ";
	AbortText[Ix++] =	"GEMfp-Abort";

	Ix = 0;
	HPStr[Ix++] = "Off               ";
	HPStr[Ix++] = "Guide+Stay        ";
	HPStr[Ix++] = "Grand Tour        ";
	HPStr[Ix++] = "ScrollTour        ";
	HPStr[Ix++] = "ScrollAuto        ";
	HPStr[Ix++] = "Toggle Tracking   ";
	HPStr[Ix++] = "Focus             ";
	HPStr[Ix++] = "Aux Output Control";

	IxA = 0;
	Ix = 0;
	MenuArray[IxA].Title = "File";
	MenuArray[IxA].SubTitles[Ix].Title = "Quit";
	MenuArray[IxA].SubTitles[Ix].Desc = "quit the program";
	MenuArray[IxA].SubTitles[Ix++].MenuItem = MenuQuit;
	MenuArray[IxA].SubTitles[Ix].Title = "Site";
	MenuArray[IxA].SubTitles[Ix].Desc = "enter new site coordinates";
	MenuArray[IxA].SubTitles[Ix++].MenuItem = MenuSite;
	MenuArray[IxA].SubTitles[Ix].Title = "DataFile";
	MenuArray[IxA].SubTitles[Ix].Desc = "load a data file containing object coordinates";
	MenuArray[IxA].SubTitles[Ix++].MenuItem = MenuDataFile;
	MenuArray[IxA].SubTitles[Ix].Title = "DataFile2";
	MenuArray[IxA].SubTitles[Ix].Desc = "find closest object in a data file";
	MenuArray[IxA].SubTitles[Ix++].MenuItem = MenuDataFileClosest;
	MenuArray[IxA].SubTitles[Ix].Title = "DataFile3";
	MenuArray[IxA].SubTitles[Ix].Desc = "find closest object in a data file that is not same as input";
	MenuArray[IxA].SubTitles[Ix++].MenuItem = MenuDataFileClosestNotSame;
	MenuArray[IxA].SubTitles[Ix].Title = "SearchData";
	MenuArray[IxA].SubTitles[Ix].Desc = "search data files for object";
	MenuArray[IxA].SubTitles[Ix++].MenuItem = MenuSearchDataFiles;
	MenuArray[IxA].SubTitles[Ix].Title = "ClosestGT";
	MenuArray[IxA].SubTitles[Ix].Desc = "set grand tour so that next object is closest to current equat coord";
	MenuArray[IxA].SubTitles[Ix++].MenuItem = MenuGrandTourClosest;
	MenuArray[IxA].SubTitles[Ix].Title = "ScrollTour";
	MenuArray[IxA].SubTitles[Ix].Desc = "load a file of slow smooth scrolling motions";
	MenuArray[IxA].SubTitles[Ix++].MenuItem = MenuScrollTour;
	MenuArray[IxA].SubTitles[Ix].Title = "Guide.bat";
	MenuArray[IxA].SubTitles[Ix].Desc = "call DOS version of Project Pluto Guide (cd-rom)";
	MenuArray[IxA].SubTitles[Ix++].MenuItem = MenuGuide;
	MenuArray[IxA].SubTitles[Ix].Title = "Hotkeys";
	MenuArray[IxA].SubTitles[Ix].Desc = "display a listing of hotkeys";
	MenuArray[IxA].SubTitles[Ix++].MenuItem = MenuHotkeys;
	MenuArray[IxA].SubTitles[Ix].Title = "ReadSlew";
	MenuArray[IxA].SubTitles[Ix].Desc = "read slew file generated by external control program";
	MenuArray[IxA].SubTitles[Ix++].MenuItem = MenuReadSlew;
	MenuArray[IxA].SubTitles[Ix].Title = "WriteSlew";
	MenuArray[IxA].SubTitles[Ix].Desc = "write slew file for use by external control program";
	MenuArray[IxA].SubTitles[Ix++].MenuItem = MenuWriteSlew;
	MenuArray[IxA].SubTitles[Ix].Title = "LX200";
	MenuArray[IxA].SubTitles[Ix].Desc = "toggle LX200 command link and toggle display";
	MenuArray[IxA].SubTitles[Ix++].MenuItem = MenuLX200Control;
	MenuArray[IxA].NumSubTitles = Ix;

	IxA++;
	Ix = 0;
	MenuArray[IxA].Title = "Motors";
	MenuArray[IxA].SubTitles[Ix].Title = "TrackOn/Off";
	MenuArray[IxA].SubTitles[Ix].Desc = "toggle tracking on and off";
	MenuArray[IxA].SubTitles[Ix++].MenuItem = MenuTrack;
	MenuArray[IxA].SubTitles[Ix].Title = "DecOn/Off";
	MenuArray[IxA].SubTitles[Ix].Desc = "Enable or disable the Declination motor to microstep";
	MenuArray[IxA].SubTitles[Ix++].MenuItem = MenuDecMotor;
	MenuArray[IxA].SubTitles[Ix].Title = "MsSpeed";
	MenuArray[IxA].SubTitles[Ix].Desc = "set the microstepping speed";
	MenuArray[IxA].SubTitles[Ix++].MenuItem = MenuMsSpeed;
	MenuArray[IxA].SubTitles[Ix].Title = "GuideSpeed";
	MenuArray[IxA].SubTitles[Ix].Desc = "set the speed to use during guide mode";
	MenuArray[IxA].SubTitles[Ix++].MenuItem = MenuGuideSpeed;
	MenuArray[IxA].SubTitles[Ix].Title = "FocusFastSpeed";
	MenuArray[IxA].SubTitles[Ix].Desc = "set the fast focus speed";
	MenuArray[IxA].SubTitles[Ix++].MenuItem = MenuFocusFastSpeed;
	MenuArray[IxA].SubTitles[Ix].Title = "FocusSlowSpeed";
	MenuArray[IxA].SubTitles[Ix].Desc = "set the slow focus speed";
	MenuArray[IxA].SubTitles[Ix++].MenuItem = MenuFocusSlowSpeed;
	MenuArray[IxA].SubTitles[Ix].Title = "Backlash";
	MenuArray[IxA].SubTitles[Ix].Desc = "change backlash parameters";
	MenuArray[IxA].SubTitles[Ix++].MenuItem = MenuChangeBacklashParms;
	MenuArray[IxA].SubTitles[Ix].Title = "AutoGEMFlip";
	MenuArray[IxA].SubTitles[Ix].Desc = "turn on/off the auto German Equatorial Mount meridian flip";
	MenuArray[IxA].SubTitles[Ix++].MenuItem = MenuAutoGEMFlipOnOff;
	MenuArray[IxA].NumSubTitles = Ix;

	IxA++;
	Ix = 0;
	MenuArray[IxA].Title = "Handpad";
	MenuArray[IxA].SubTitles[Ix].Title = "HandpadMode";
	MenuArray[IxA].SubTitles[Ix].Desc = "cycle through hand paddle mode settings";
	MenuArray[IxA].SubTitles[Ix++].MenuItem = MenuHandpad;
	MenuArray[IxA].SubTitles[Ix].Title = "SelectHPMode";
	MenuArray[IxA].SubTitles[Ix].Desc = "select a hand paddle mode";
	MenuArray[IxA].SubTitles[Ix++].MenuItem = MenuSelectHandpadMode;
	MenuArray[IxA].SubTitles[Ix].Title = "FlipButtons";
	MenuArray[IxA].SubTitles[Ix].Desc = "reverse up, down hand paddle buttons";
	MenuArray[IxA].SubTitles[Ix++].MenuItem = MenuReverseUpDownButtons;
	MenuArray[IxA].SubTitles[Ix].Title = "ModeSwitchLeft";
	MenuArray[IxA].SubTitles[Ix].Desc = "simulate handpad mode switch move to the left";
	MenuArray[IxA].SubTitles[Ix++].MenuItem = MenuHandpadLeft;
	MenuArray[IxA].SubTitles[Ix].Title = "ModeSwitchRight";
	MenuArray[IxA].SubTitles[Ix].Desc = "simulate handpad mode switch move to the right";
	MenuArray[IxA].SubTitles[Ix++].MenuItem = MenuHandpadRight;
	MenuArray[IxA].NumSubTitles = Ix;

	IxA++;
	Ix = 0;
	MenuArray[IxA].Title = "Init";
	MenuArray[IxA].SubTitles[Ix].Title = "DisplayInits";
	MenuArray[IxA].SubTitles[Ix].Desc = "display the initialization settings";
	MenuArray[IxA].SubTitles[Ix++].MenuItem = MenuDisplayInit;
	MenuArray[IxA].NumSubTitles = Ix;

	IxA++;
	Ix = 0;
	MenuArray[IxA].Title = "Coord";
	MenuArray[IxA].SubTitles[Ix].Title = "InputEquat";
	MenuArray[IxA].SubTitles[Ix].Desc = "enter new input equatorial coordinates";
	MenuArray[IxA].SubTitles[Ix++].MenuItem = MenuInputEquat;
	MenuArray[IxA].SubTitles[Ix].Title = "OffsetEquat";
	MenuArray[IxA].SubTitles[Ix].Desc = "enter offset from current equatorial coordinates";
	MenuArray[IxA].SubTitles[Ix++].MenuItem = MenuOffsetEquat;
	MenuArray[IxA].SubTitles[Ix].Title = "Save1";
	MenuArray[IxA].SubTitles[Ix].Desc = "saves current equatorial coordinates into #1 buffer";
	MenuArray[IxA].SubTitles[Ix++].MenuItem = MenuSav1;
	MenuArray[IxA].SubTitles[Ix].Title = "Restore1";
	MenuArray[IxA].SubTitles[Ix].Desc = "restore saved #1 equatorial coordinates into input fields";
	MenuArray[IxA].SubTitles[Ix++].MenuItem = MenuRes1;
	MenuArray[IxA].SubTitles[Ix].Title = "Save2";
	MenuArray[IxA].SubTitles[Ix].Desc = "saves current equatorial coordinates into #2 buffer";
	MenuArray[IxA].SubTitles[Ix++].MenuItem = MenuSav2;
	MenuArray[IxA].SubTitles[Ix].Title = "Restore2";
	MenuArray[IxA].SubTitles[Ix].Desc = "restore saved #2 equatorial coordinates into input fields";
	MenuArray[IxA].SubTitles[Ix++].MenuItem = MenuRes2;
	MenuArray[IxA].SubTitles[Ix].Title = "LastObject";
	MenuArray[IxA].SubTitles[Ix].Desc = "restore last selected object from the data files";
	MenuArray[IxA].SubTitles[Ix++].MenuItem = MenuRestoreLastObject;
	MenuArray[IxA].SubTitles[Ix].Title = "Home";
	MenuArray[IxA].SubTitles[Ix].Desc = "enter new altazimuth home coordinates";
	MenuArray[IxA].SubTitles[Ix++].MenuItem = MenuHomeCoord;
	MenuArray[IxA].NumSubTitles = Ix;

	IxA++;
	Ix = 0;
	MenuArray[IxA].Title = "Move";
	MenuArray[IxA].SubTitles[Ix].Title = "Equat";
	MenuArray[IxA].SubTitles[Ix].Desc = "move to input equatorial coordinates";
	MenuArray[IxA].SubTitles[Ix++].MenuItem = MenuMoveEquat;
	MenuArray[IxA].SubTitles[Ix].Title = "Home";
	MenuArray[IxA].SubTitles[Ix].Desc = "move to home coordinates";
	MenuArray[IxA].SubTitles[Ix++].MenuItem = MenuMoveHome;
	MenuArray[IxA].SubTitles[Ix].Title = "MeridianFlip";
	MenuArray[IxA].SubTitles[Ix].Desc = "flip the telescope to the other side of the meridian";
	MenuArray[IxA].SubTitles[Ix++].MenuItem = MenuMoveGEMFlip;
	MenuArray[IxA].SubTitles[Ix].Title = "MoveFocus";
	MenuArray[IxA].SubTitles[Ix].Desc = "move the focus motor to a particular position";
	MenuArray[IxA].SubTitles[Ix++].MenuItem = MenuMoveFocus;
	MenuArray[IxA].NumSubTitles = Ix;

	IxA++;
	Ix = 0;
	MenuArray[IxA].Title = "Reset";
	MenuArray[IxA].SubTitles[Ix].Title = "Equat";
	MenuArray[IxA].SubTitles[Ix].Desc = "synchronize or reset to input equatorial coordinates";
	MenuArray[IxA].SubTitles[Ix++].MenuItem = MenuResetEquat;
	MenuArray[IxA].SubTitles[Ix].Title = "Home";
	MenuArray[IxA].SubTitles[Ix].Desc = "reset to home coordinates";
	MenuArray[IxA].SubTitles[Ix++].MenuItem = MenuResetHome;
	MenuArray[IxA].SubTitles[Ix].Title = "GEMFlip";
	MenuArray[IxA].SubTitles[Ix].Desc = "do a manual GEM meridian flip";
	MenuArray[IxA].SubTitles[Ix++].MenuItem = MenuGEMMeridianFlip;
	MenuArray[IxA].SubTitles[Ix].Title =  "Focus";
	MenuArray[IxA].SubTitles[Ix].Desc = "reset focus motor position";
	MenuArray[IxA].SubTitles[Ix++].MenuItem = MenuResetFocus;
	MenuArray[IxA].NumSubTitles = Ix;

	if( DisplayOpeningMsgs)
		PrelimMsg();

	SetAttributes();

	/* setup screen */
	_wscroll = No;
	_setcursortype( _NOCURSOR);
	Clrscr();

	/* write main window */
	DrawBorder( MainFrame);

	/* write display titles */
	TextAttr = DefaultText;
	VidMemXY.X = MenuStartX;
	VidMemXY.Y = DisplayInputY;
	sprintf( StrBuf, "%s", MainStr_14);
	WriteStrBufToScreen_f_ptr();
	VidMemXY.X = MenuStartX;
	VidMemXY.Y = DisplayFileY;
	sprintf( StrBuf, "%s", MainStr_16);
	WriteStrBufToScreen_f_ptr();
	VidMemXY.X = MenuStartX;
	VidMemXY.Y = DisplayObjectY;
	sprintf( StrBuf, "%s", MainStr_17);
	WriteStrBufToScreen_f_ptr();
	VidMemXY.X = MenuStartX;
	VidMemXY.Y = DisplaySidTY;
	sprintf( StrBuf, "%s", MainStr_18);
	WriteStrBufToScreen_f_ptr();
	VidMemXY.X = MenuStartX;
	VidMemXY.Y = DisplayDateTimeY;
	sprintf( StrBuf, "%s", MainStr_19);
	WriteStrBufToScreen_f_ptr();
	VidMemXY.X = MenuStartX;
	VidMemXY.Y = DisplayPWMY;
	sprintf( StrBuf, "%s", MainStr_20);
	WriteStrBufToScreen_f_ptr();
	VidMemXY.X = MenuStartX;
	VidMemXY.Y = DisplaySlewY;
	sprintf( StrBuf, "%s", MainStr_22);
	WriteStrBufToScreen_f_ptr();
	VidMemXY.X = MenuStartX;
	VidMemXY.Y = DisplayLX200Y;
	sprintf( StrBuf, "%s", MainStr_23);
	WriteStrBufToScreen_f_ptr();

	WriteHandpadStatus();
	WriteTrackStatus();
	WriteMsArcsecSec();
	WriteGuideArcsecSec();
	WriteFocusFastSpeed();
	WriteFocusSlowSpeed();
	if( FocusMethod == FocusMethod_Pulse_1_14 || FocusMethod == FocusMethod_Pulse_16_17)
		WriteFocusPosition();
	In = Current;
	DisplayIn( NameBlanks, NameBlanks);
	DisplayGEMFlipStatus();
	DisplayLX200CommandStatus();
	DisplayMsValues_f_ptr();
	if( UseMouseFlag)
		WriteMouseMode();

	/* fill out the array of menu category video memory X values */
	for( Ix = 0; Ix < MaxMenuCats; Ix++)
		MenuCatX[Ix] = MenuStartX + Ix*(MenuEndX-MenuStartX)/MaxMenuCats;
	/* fill out the array of menu subtitle video memory X values */
	for( Ix = 0; Ix < MaxMenuSubs; Ix++)
		MenuSubX[Ix] = MenuStartX + Ix*(MenuEndX-MenuStartX)/MaxMenuSubs;
	/* start program with first category and first subtitle of that category */
	CurrentMenuCat = 0;
	CurrentMenuSub = 0;
	NewMenuCat = 0;
	NewMenuSub = 0;
	WriteMainMenu();
}

void SetAttributes( void)
{
	MenuText = MenuColor + (DefaultBackground << 4);
	SelectText = SelectColor + (SelectBackground << 4);
	CurrentText = CurrentColor + (DefaultBackground << 4);
	DisplayText = DisplayColor + (DefaultBackground << 4);
	DefaultText = DefaultColor + (DefaultBackground << 4);
	BorderText = BorderColor + (DefaultBackground << 4);
	TitleText = TitleColor + (DefaultBackground << 4);
}

void InitDisplayXY( void)
{
	DisplayXY[DisplayFocusPosition].X = DisplayFocusPositionX;
	DisplayXY[DisplayFocusPosition].Y = DisplayInputY;
	DisplayXY[DisplayInputRa].X = DisplayInputRaX;
	DisplayXY[DisplayInputRa].Y = DisplayInputY;
	DisplayXY[DisplayInputDec].X = DisplayInputDecX;
	DisplayXY[DisplayInputDec].Y = DisplayInputY;
	DisplayXY[DisplayGEMFlipMove].X = MenuStartX;
	DisplayXY[DisplayGEMFlipMove].Y = MenuSubLine3Y;
	DisplayXY[DisplayFile].X = DisplayFileX;
	DisplayXY[DisplayFile].Y = DisplayFileY;
	DisplayXY[DisplayScrollCounts].X = DisplayScrollCountsX;
	DisplayXY[DisplayScrollCounts].Y = DisplayScrollCountsY;
	DisplayXY[DisplayTrack].X = DisplayTrackX;
	DisplayXY[DisplayTrack].Y = DisplayTrackY;
	DisplayXY[DisplayGEMFlip].X = DisplayGEMFlipX;
	DisplayXY[DisplayGEMFlip].Y = DisplayGEMFlipY;
	DisplayXY[DisplayObject].X = DisplayObjectX;
	DisplayXY[DisplayObject].Y = DisplayObjectY;
	DisplayXY[DisplayHandpad].X = DisplayHandpadX;
	DisplayXY[DisplayHandpad].Y = DisplayHandpadY;
	DisplayXY[DisplayButtons].X = DisplayButtonsX;
	DisplayXY[DisplayButtons].Y = DisplayButtonsY;
	DisplayXY[DisplaySidT].X = DisplaySidTX;
	DisplayXY[DisplaySidT].Y = DisplaySidTY;
	DisplayXY[DisplayMouseMode].X = DisplayMouseModeX;
	DisplayXY[DisplayMouseMode].Y = DisplayMouseModeY;
	DisplayXY[DisplayMsSpeed].X = DisplayMsSpeedX;
	DisplayXY[DisplayMsSpeed].Y = DisplayMsSpeedY;
	DisplayXY[DisplayGuideSpeed].X = DisplayGuideX;
	DisplayXY[DisplayGuideSpeed].Y = DisplayGuideY;
	DisplayXY[DisplayDateTime].X = DisplayDateTimeX;
	DisplayXY[DisplayDateTime].Y = DisplayDateTimeY;
	DisplayXY[DisplayFocusFast].X = DisplayFocusFastX;
	DisplayXY[DisplayFocusFast].Y = DisplayFocusY;
	DisplayXY[DisplayFocusSlow].X = DisplayFocusSlowX;
	DisplayXY[DisplayFocusSlow].Y = DisplayFocusY;
	DisplayXY[DisplayFRFocusAuxAction].X = DisplayFRFocusAuxActionX;
	DisplayXY[DisplayFRFocusAuxAction].Y = DisplayFocusY;
	DisplayXY[DisplayTrackByRate].X = MenuStartX;
	DisplayXY[DisplayTrackByRate].Y = Display2MotorTrackY;
	DisplayXY[DisplayPWMReps].X = DisplayPWMX;
	DisplayXY[DisplayPWMReps].Y = DisplayPWMY;
	DisplayXY[DisplayMsStatus].X = DisplayMsStatusX;
	DisplayXY[DisplayMsStatus].Y = DisplayMsStatusY;
	DisplayXY[DisplayHsSlew].X = DisplaySlewX;
	DisplayXY[DisplayHsSlew].Y = DisplaySlewY;
	DisplayXY[DisplayHsAbort].X = DisplaySlewX+6;
	DisplayXY[DisplayHsAbort].Y = DisplaySlewY;
	DisplayXY[DisplayHsSteps].X = DisplaySlewX+17;
	DisplayXY[DisplayHsSteps].Y = DisplaySlewY;
	DisplayXY[DisplayLX200].X = DisplayLX200X;
	DisplayXY[DisplayLX200].Y = DisplayLX200Y;
	DisplayXY[DisplayLX200_2].X = DisplayLX200X2;
	DisplayXY[DisplayLX200_2].Y = DisplayLX200Y2;
}

void CalcHsMsgSteps( void)
{
	double MaxHsRad;

	/* set halfsteps to move that triggers confirming msg */
	if( HsRad.A > HsRad.Z)
		MaxHsRad = HsRad.A;
	else
		MaxHsRad = HsRad.Z;
	MoveHsMsgSteps = (long) (MoveHsMsgDeg*DegToRad/MaxHsRad);
}

void CloseKBEvent( void)
{
	LX200ComPort = HoldLX200ComPort;
	fclose( OutGuideFilePtr);
	FreeAllScroll();
	clrscr();
	_setcursortype( _NORMALCURSOR);
	if( DisplayOpeningMsgs)
	{
		printf( "\nPlease turn off motor power supply.\n");
		ContMsgRoutine();
	}
}

void PrelimMsg( void)
{
	printf( "\nWarning: to prevent overheating motors & electronics, ");
	printf( "\n         turn off the motor power supply if program not running");
	printf( "\n         and stop tracking before hot-keying to another program.");
	printf( "\nPlease turn on the motors' power supply now.");
	NewLine;
	ContMsgRoutine();
}

/* clears menu category and menu subtitle area */
void ClearMenuArea( void)
{
	window( 2, MenuCatsY+1, MaxX-1, MenuDescY+1);
	Clrscr();
	window( 1, 1, MaxX, MaxY);
}

/* clears only menu subtitle area */
void ClearMenuSubArea( void)
{
	window( 2, MenuSubMenuY+1, MaxX-1, MenuDescY+1);
	Clrscr();
	window( 1, 1, MaxX, MaxY);
}

void DrawScreenLine( const int Y)
{
	TextAttr = BorderText;
	VidMemXY.Y = Y;
	for( VidMemXY.X = 1; VidMemXY.X < MaxX-1; VidMemXY.X++)
		WriteCharToScreen( HorizBorder);
	VidMemXY.X = 0;
	WriteCharToScreen( LeftIntersect);
	VidMemXY.X = MaxX-1;
	WriteCharToScreen( RightIntersect);
}

void WriteMainMenu( void)
{
	ClearMenuArea();
	WriteMenuCats();
	SelectTextAttrMenuCat( CurrentMenuCat);
	WriteSubMenus();
	SelectTextAttrMenuSub( CurrentMenuSub);
	WriteSubMenuDesc( CurrentMenuCat, CurrentMenuSub);
	/* draw a line separating the menu category from the menu subtitle area */
	DrawScreenLine( MenuSubLine1Y);
	/* draw a line separating the menu subtitle area from the menu description */
	DrawScreenLine( MenuSubLine2Y);
	/* write in current Ra, Dec titles */
	/* draw a line separating the jumbo coordinate display area from the rest of the display area */
	DrawScreenLine( MenuSubLine3Y);
	TextAttr = DefaultText;
	VidMemXY.Y = MenuSubLine2Y;
	VidMemXY.X = 10;
	sprintf( StrBuf, "Current Right Ascension");
	WriteStrBufToScreen_f_ptr();
	VidMemXY.X = 50;
	sprintf( StrBuf, "Current Declination");
	WriteStrBufToScreen_f_ptr();
}

void WriteMenuCats( void)
{
	TextAttr = DefaultText;
	VidMemXY.Y = MenuCatsY;
	for( Ix = 0; Ix < MaxMenuCats; Ix++)
	{
		sprintf( StrBuf, "%s", MenuArray[Ix].Title);
		VidMemXY.X = MenuCatX[Ix];
		WriteStrBufToScreen_f_ptr();
	}
}

void SelectTextAttrMenuCat( const int MenuCat)
{
	TextAttr = SelectText;
	ChangeTextAttrMenuCat( MenuCat);
}

void DefaultTextAttrMenuCat( const int MenuCat)
{
	TextAttr = DefaultText;
	ChangeTextAttrMenuCat( MenuCat);
}

/* changes the attributes of the displayed text */
void ChangeTextAttrMenuCat( const int MenuCat)
{
	int StartX, EndX;

	VidMemXY.Y = MenuCatsY;
	StartX = MenuCatX[MenuCat];
	if( MenuCat+1 == MaxMenuCats)
		EndX = MenuEndX;
	else
		EndX = MenuCatX[MenuCat+1]-1;
	for( VidMemXY.X = StartX; VidMemXY.X < EndX;	VidMemXY.X++)
		Screen[VidMemXY.Y][VidMemXY.X].Attr = TextAttr;
}

void WriteSubMenus( void)
{
	TextAttr = MenuText;
	for( Ix = 0; Ix < MenuArray[CurrentMenuCat].NumSubTitles; Ix++)
	{
		TextAttr = MenuText;
		VidMemXY.Y = MenuSubMenuY + Ix%MaxMenuDisplayRows;
		VidMemXY.X = MenuSubX[Ix/MaxMenuDisplayRows];
		sprintf( StrBuf, "%s", MenuArray[CurrentMenuCat].SubTitles[Ix].Title);
		WriteStrBufToScreen_f_ptr();
	}
}

void WriteSubMenuDesc( const int MenuCat, const int SubMenu)
{
	TextAttr = DefaultText;
	VidMemXY.Y = MenuDescY;
	for( VidMemXY.X = MenuStartX; VidMemXY.X < MenuEndX; VidMemXY.X++)
		WriteCharToScreen( Blank);
	VidMemXY.X = MenuStartX;
	sprintf( StrBuf, "-> ");
	WriteStrBufToScreen_f_ptr();
	sprintf( StrBuf, "%s", MenuArray[MenuCat].SubTitles[SubMenu].Desc);
	WriteStrBufToScreen_f_ptr();
}

void SelectTextAttrMenuSub( const int MenuSub)
{
	TextAttr = SelectText;
	ChangeTextAttrMenuSub( MenuSub);
}

void MenuTextAttrMenuSub( const int MenuSub)
{
	TextAttr = MenuText;
	ChangeTextAttrMenuSub( MenuSub);
}

/* changes the attributes of the displayed text */
void ChangeTextAttrMenuSub( const int MenuSub)
{
	int StartX, EndX;

	VidMemXY.Y = MenuSubMenuY + MenuSub%MaxMenuDisplayRows;
	StartX = MenuSubX[MenuSub/MaxMenuDisplayRows];
	EndX = StartX + (MenuEndX - MenuStartX) / MaxMenuSubs - 1;
	for( VidMemXY.X = StartX; VidMemXY.X < EndX;	VidMemXY.X++)
		Screen[VidMemXY.Y][VidMemXY.X].Attr = TextAttr;
}

void UpdateMenuCatSub( void)
{
	/* new menu category and therefore new menu subtitle of that category */
	if( NewMenuCat != CurrentMenuCat)
	{
		DefaultTextAttrMenuCat( CurrentMenuCat);
		SelectTextAttrMenuCat( NewMenuCat);
		CurrentMenuCat = NewMenuCat;
		ClearMenuSubArea();
		WriteSubMenus();
		SelectTextAttrMenuSub( NewMenuSub);
		WriteSubMenuDesc( CurrentMenuCat, NewMenuSub);
		CurrentMenuSub = NewMenuSub;
	}
	else
		/* same menu category but new menu subtitle of that category */
		if( NewMenuSub != CurrentMenuSub)
		{
			MenuTextAttrMenuSub( CurrentMenuSub);
			SelectTextAttrMenuSub( NewMenuSub);
			WriteSubMenuDesc( CurrentMenuCat, NewMenuSub);
			CurrentMenuSub = NewMenuSub;
		}
}

/* draws border around inside edge of window */
void DrawBorder( const struct Frame P)
{
	TextAttr = BorderText;
	/* write top */
	VidMemXY.Y = P.Top - 1;
	for( VidMemXY.X = P.Left; VidMemXY.X < P.Right-1; VidMemXY.X++)
		WriteCharToScreen( HorizBorder);
	/* write bottom */
	VidMemXY.Y = P.Bottom - 1;
	for( VidMemXY.X = P.Left; VidMemXY.X < P.Right-1; VidMemXY.X++)
		WriteCharToScreen( HorizBorder);
	/* write left */
	VidMemXY.X = P.Left - 1;
	for( VidMemXY.Y = P.Top; VidMemXY.Y < P.Bottom-1; VidMemXY.Y++)
		WriteCharToScreen( VertBorder);
	/* write right */
	VidMemXY.X = P.Right - 1;
	for( VidMemXY.Y = P.Top; VidMemXY.Y < P.Bottom-1; VidMemXY.Y++)
		WriteCharToScreen( VertBorder);
	/* write corners */
	VidMemXY.X = P.Left - 1;
	VidMemXY.Y = P.Top - 1;
	WriteCharToScreen( TopLeftCorner);
	VidMemXY.X = P.Right - 1;
	WriteCharToScreen( TopRightCorner);
	VidMemXY.Y = P.Bottom - 1;
	WriteCharToScreen( BottomRightCorner);
	VidMemXY.X = P.Left - 1;
	WriteCharToScreen( BottomLeftCorner);

	/* write title */
	TextAttr = TitleText;
	VidMemXY.X = (P.Left + P.Right + 1)/2 - strlen( P.Title)/2 - 1;
	VidMemXY.Y = P.Top - 1;
	sprintf( StrBuf, "%s", P.Title);
	WriteStrBufToScreen_f_ptr();
}

void WriteWindow( struct Frame P)
{
	gettext( P.Left, P.Top, P.Right, P.Bottom, WinBuffer);
	window( P.Left, P.Top, P.Right, P.Bottom);
	Clrscr();
	window( 1, 1, MaxX, MaxY);
	DrawBorder( P);
	_setcursortype( _NORMALCURSOR);
}

void RemoveWindow( struct Frame P)
{
	puttext( P.Left, P.Top, P.Right, P.Bottom, WinBuffer);
	_setcursortype( _NOCURSOR);
}

/* after keyboard key is pressed, waits until next clock tick */
void PressKeyToContMsg( const char* Msg)
{
	WriteWindow( MsgFrame);
	gotoxy( MsgFrame.Left + 2, MsgFrame.Top + 2);
	printf( "%s", Msg);
	sound( 500);
	delay( 100);
	nosound();
	gotoxy( MsgFrame.Left + 2, MsgFrame.Top + 4);
	ContMsgRoutine();
	RemoveWindow( MsgFrame);
	PauseUntilNewSidTime();
	HPEventGetEquat();
}

void SetNewMenuCatSub( void)
{
	switch( Response)
	{
		case DownCursor:
			NewMenuSub = CurrentMenuSub + 1;
			if( NewMenuSub == MenuArray[CurrentMenuCat].NumSubTitles)
				NewMenuSub = 0;
			break;
		case UpCursor:
			NewMenuSub = CurrentMenuSub - 1;
			if( NewMenuSub < 0)
				NewMenuSub = MenuArray[CurrentMenuCat].NumSubTitles - 1;
			break;
		case RightCursor:
		case Tab:
			NewMenuCat = CurrentMenuCat + 1;
			if( NewMenuCat == MaxMenuCats)
				NewMenuCat = 0;
			NewMenuSub = 0;
			break;
		case LeftCursor:
		case Shift_Tab:
			NewMenuCat = CurrentMenuCat - 1;
			if( NewMenuCat < 0)
				NewMenuCat = MaxMenuCats - 1;
			NewMenuSub = 0;
	}
}

void GetNewMenuCatSubFromResponse( void)
{
	NewMenuCat = NewMenuSub = 0;

	while( Response != MenuArray[NewMenuCat].SubTitles[NewMenuSub].MenuItem)
	{
		NewMenuSub++;
		/* not == MaxMenuSub */
		if( NewMenuSub == MenuArray[NewMenuCat].NumSubTitles)
		{
			NewMenuSub = 0;
			NewMenuCat++;
			if( NewMenuCat == MaxMenuCats)
				BadExit( "couldn't match Response in GetNewMenuCatSubFromResponse()");
		}
	}
}

/* Keyboard events checked every bios clock tick */
void ProcessKBEvents( void)
{
	Response = getch();
	ProcessMenuResponse();
}

void ProcessVirtualHandpad( void)
{
	struct AZLong HoldSteps;

	Steps.A = Steps.Z = 0;

	switch( Response)
	{
		case F1:
			Steps.A = MsTick.A;
			Dir.A = CW;
			break;
		case F2:
			Steps.A = MsTick.A;
			Dir.A = CCW;
			break;
		case F3:
			Steps.Z = MsTick.Z;
			Dir.Z = CCW;
			break;
		case F4:
			Steps.Z = MsTick.Z;
			Dir.Z = CW;
	}
	HoldSteps = Steps;
	while( True)
	{
		HPEventMoveMs();
		HPEventGetEquat();
		CheckLX200Events();
		SequentialTaskController();
		/* add undone steps to steps to do for next clock tick */
		Steps.A += HoldSteps.A;
		Steps.Z += HoldSteps.Z;
		if( KeyStroke)
			getch();
		else
			break;
	}
	while( KeyStroke)
		getch();
	AlignMs();
	SetCurrentAltazToAccumMs();
	HPEventGetEquat();
}

void ProcessMenuResponse( void)
{
	int NewResponse;

	switch( Response)
	{
		/* here each case ends in a break: these are responses that do not involve menu item selection
		processing by ProcessMenuSelection() */
		case Tab:
			SetNewMenuCatSub();
			UpdateMenuCatSub();
			ProcessHPEvents();
			break;
		case ExtendedKeyboardStroke:
			Response = getch();
			if( Response == F1 || Response == F2
			|| Response == F3 || Response == F4)
				ProcessVirtualHandpad();
			else
				if( Response == DownCursor || Response == UpCursor
				|| Response == RightCursor || Response == LeftCursor
				|| Response == Shift_Tab)
				{
					SetNewMenuCatSub();
					UpdateMenuCatSub();
				}
				ProcessHPEvents();
			break;
		case 'b': case 'B':
			ScrollFlag = Yes;
			break;
		case Return:
			Response = MenuArray[CurrentMenuCat].SubTitles[CurrentMenuSub].MenuItem;
			ProcessMenuSelection();
			break;
		/* no breaks so as to fall through */
		/* use NewResponse to hold menu choice, since using Response will set Response to a new value,
		yet Response must be kept unchanged for the case statements to work */
		case Left_Apostrophe:
			if( Response == Left_Apostrophe)
				NewResponse = MenuGuide;
		case '[':
		case '{':
			if( Response == '[' || Response == '{')
				NewResponse = MenuGEMMeridianFlip;
		case '?':
			if( Response == '?')
				NewResponse = MenuInputEquat;
		case '<':
			if( Response == '<')
				NewResponse = MenuHandpadLeft;
		case '>':
			if( Response == '>')
				NewResponse = MenuHandpadRight;
		case '1':
			if( Response == '1')
				NewResponse = MenuMoveEquat;
		case '2':
			if( Response == '2')
				NewResponse = MenuTrack;
		case '3':
			if( Response == '3')
				NewResponse = MenuSav1;
		case '4':
			if( Response == '4')
				NewResponse = MenuRes1;
		case '5':
			if( Response == '5')
				NewResponse = MenuSav2;
		case '6':
			if( Response == '6')
				NewResponse = MenuRes2;
		case '7':
			if( Response == '7')
				NewResponse = MenuReadSlew;
		case '8':
			if( Response == '8')
				NewResponse = MenuWriteSlew;
		case 'a': case 'A':
		case 'd': case 'D':
			if( Response == 'd' || Response == 'D')
				NewResponse = MenuDataFile;
		case 'g': case 'G':
			if( Response == 'g' || Response == 'G')
				NewResponse = MenuGuide;
		case 'h':
			if( Response == 'h')
				NewResponse = MenuHandpad;
		case 'H':
			if( Response == 'H')
				NewResponse = MenuSelectHandpadMode;
		case 'i': case 'I':
			if( Response == 'i' || Response == 'I')
				NewResponse = MenuDisplayInit;
		case 'm': case 'M':
			if( Response == 'm' || Response == 'M')
				NewResponse = MenuMsSpeed;
		case 'o': case 'O':
			if( Response == 'o' || Response == 'O')
				NewResponse = MenuRestoreLastObject;
		case 'q': case 'Q':
			if( Response == 'q' || Response == 'Q')
				NewResponse = MenuQuit;
		case 'r': case 'R':
			if( Response == 'r' || Response == 'R')
				NewResponse = MenuResetEquat;
		case 's': case 'S':
			if( Response == 's' || Response == 'S')
				NewResponse = MenuScrollTour;
		case 't': case 'T':
			if( Response == 't' || Response == 'T')
				NewResponse = MenuTrack;
			/* all fall through to here... */
			Response = NewResponse;
			GetNewMenuCatSubFromResponse();
			UpdateMenuCatSub();
			ProcessMenuSelection();
			break;
		default:
			ProcessHPEvents();
	}
}

Flag ConfirmHsMove( void)
{
	struct AZDouble HoldAZ;
	double MaxMoveDeg;
	int X = MsgFrame.Left + 5;
	int Y = MsgFrame.Top + 2;

	if( Steps.A > MoveHsMsgSteps || Steps.Z > MoveHsMsgSteps)
	{
		if( Steps.A > Steps.Z)
			MaxMoveDeg = (double) Steps.A*HsRad.A*RadToDeg;
		else
			MaxMoveDeg = (double) Steps.Z*HsRad.Z*RadToDeg;

		WriteWindow( MsgFrame);
		gotoxy( X, Y);
		printf( "Halfstep move of %3.0f deg, ", MaxMoveDeg);
		Y += 2;
		gotoxy( X, Y);
		HoldAZ.A = (double) Steps.A*HsRad.A;
		if( Dir.A == CCW)
			HoldAZ.A = -HoldAZ.A;
		HoldAZ.A += Current.Alt;
		HoldAZ.Z = (double) Steps.Z*HsRad.Z;
		if( Dir.Z == CCW)
			HoldAZ.Z = -HoldAZ.Z;
		HoldAZ.Z += Current.Az;
		if( HoldAZ.Z > OneRev)
			HoldAZ.Z -= OneRev;
		if( HoldAZ.Z < 0)
			HoldAZ.Z += OneRev;
		printf( "   destination alt = %2.0f, az = %3.0f", HoldAZ.A*RadToDeg, HoldAZ.Z*RadToDeg);
		Y += 2;
		gotoxy( X, Y);
		printf( "      Please confirm by pressing 'Y'");
		Response = getch();
		RemoveWindow( MsgFrame);
		PauseUntilNewSidTime();
		if( Response == 'Y' || Response == 'y' || Response == Return || Response == '`')
			return Yes;
		else
			return No;
	}
	else
		return Yes;
}

/* see steppers.c for calling functions */
void KBEventMoveHs( void)
{
	struct AZLong HoldSteps;

	/* AlignMs uses Steps */
	HoldSteps = Steps;
	AlignMs();
	Steps = HoldSteps;
	/* HsTimerFlag halfstep displays done in SequentialTaskController() which is called from
	MoveHsUsingIRQTimer() */
	if( !HsTimerFlag)
	{
		TextAttr = DisplayText;
		VidMemXY = DisplayXY[DisplayHsSlew];
		sprintf( StrBuf, "On ");
		WriteStrBufToScreen_f_ptr();
		DisplayButtonsStatus();
	}
	HPEventMoveHs();
	/* don't call HPEventGetEquat() at this point in case KBEventMoveHs() being called from
	SetDirDistanceStepsThenMove(): this changes Current.Ra, Dec when Current.Ra, Dec should stay
	unchanged so as to continue slewing or resume tracking to */
	if( MoveToCurrentRaDecFlag && AbortState)
	{
		PauseUntilNewSidTime();
		HPEventGetEquat();
	}
	if( !HsTimerFlag)
	{
		TextAttr = DisplayText;
		VidMemXY = DisplayXY[DisplayHsSlew];
		sprintf( StrBuf, "Off");
		WriteStrBufToScreen_f_ptr();
		DisplayButtonsStatus();
		if( AbortState)
		{
			TextAttr = DisplayText;
			VidMemXY = DisplayXY[DisplayHsAbort];
			sprintf( StrBuf, "%s", AbortText[AbortState]);
			WriteStrBufToScreen_f_ptr();
		}
	}
}

void KBEventInitMatrix( const int Init)
{
	InitMatrix( Init);
}

void DisplayJumboRa( struct Position* P)
{
	int d1, d2, d3, d4, d5, d6;
	int startx = 2;
	int starty = DisplayCurrentY;
	int xsize = 2;
	int ysize = 4;
	int attr = CurrentColor << 4;

	d1 = P->RaHMSH.Hr/10;
	d2 = P->RaHMSH.Hr - d1*10;
	d3 = P->RaHMSH.Min/10;
	d4 = P->RaHMSH.Min - d3*10;
	d5 = P->RaHMSH.Sec/10;
	d6 = P->RaHMSH.Sec - d5*10;

	/* blank out any previous display of first number */
	DisplayLargeNum( 8, startx, starty, xsize, ysize, DefaultBackground<<4);
	DisplayLargeNum( d1, startx, starty, xsize, ysize, attr);
	startx += xsize + 3;
	DisplayLargeNum( 8, startx, starty, xsize, ysize, DefaultBackground<<4);
	DisplayLargeNum( d2, startx, starty, xsize, ysize, attr);
	startx += xsize + 1;
	DisplayColon( startx, starty, xsize, ysize, attr);
	startx += xsize + 2;
	DisplayLargeNum( 8, startx, starty, xsize, ysize, DefaultBackground<<4);
	DisplayLargeNum( d3, startx, starty, xsize, ysize, attr);
	startx += xsize + 3;
	DisplayLargeNum( 8, startx, starty, xsize, ysize, DefaultBackground<<4);
	DisplayLargeNum( d4, startx, starty, xsize, ysize, attr);
	startx += xsize + 1;
	DisplayColon( startx, starty, xsize, ysize, attr);
	startx += xsize + 2;
	DisplayLargeNum( 8, startx, starty, xsize, ysize, DefaultBackground<<4);
	DisplayLargeNum( d5, startx, starty, xsize, ysize, attr);
	startx += xsize + 3;
	DisplayLargeNum( 8, startx, starty, xsize, ysize, DefaultBackground<<4);
	DisplayLargeNum( d6, startx, starty, xsize, ysize, attr);
}

void DisplayJumboDec( struct Position* P)
{
	int d1, d2, d3, d4, d5, d6;
	int startx = 41;
	int starty = DisplayCurrentY;
	int xsize = 2;
	int ysize = 4;
	int attr = CurrentColor << 4;

	d1 = P->DecDMS.Deg/10;
	d2 = P->DecDMS.Deg - d1*10;
	d3 = P->DecDMS.Min/10;
	d4 = P->DecDMS.Min - d3*10;
	d5 = P->DecDMS.Sec/10;
	d6 = P->DecDMS.Sec - d5*10;

	/* blank out any previous display */
	DisplayNegSign( startx, starty, xsize, ysize, DefaultBackground<<4);
	if( P->Dec < 0)
		DisplayNegSign( startx, starty, xsize, ysize, attr);

	startx += xsize + 2;
	DisplayLargeNum( 8, startx, starty, xsize, ysize, DefaultBackground<<4);
	DisplayLargeNum( d1, startx, starty, xsize, ysize, attr);
	startx += xsize + 3;
	DisplayLargeNum( 8, startx, starty, xsize, ysize, DefaultBackground<<4);
	DisplayLargeNum( d2, startx, starty, xsize, ysize, attr);
	startx += xsize + 1;
	DisplayColon( startx, starty, xsize, ysize, attr);
	startx += xsize + 2;
	DisplayLargeNum( 8, startx, starty, xsize, ysize, DefaultBackground<<4);
	DisplayLargeNum( d3, startx, starty, xsize, ysize, attr);
	startx += xsize + 3;
	DisplayLargeNum( 8, startx, starty, xsize, ysize, DefaultBackground<<4);
	DisplayLargeNum( d4, startx, starty, xsize, ysize, attr);
	startx += xsize + 1;
	DisplayColon( startx, starty, xsize, ysize, attr);
	startx += xsize + 2;
	DisplayLargeNum( 8, startx, starty, xsize, ysize, DefaultBackground<<4);
	DisplayLargeNum( d5, startx, starty, xsize, ysize, attr);
	startx += xsize + 3;
	DisplayLargeNum( 8, startx, starty, xsize, ysize, DefaultBackground<<4);
	DisplayLargeNum( d6, startx, starty, xsize, ysize, attr);
}

void SequentialHiPriController( void)
{
	static double HoldRa, HoldDec = -9999;

	TextAttr = CurrentText;

	switch( HTask)
	{
		/* by not changing NextTaskTime, the following display tasks will execute sequentially
		without delay as this function is repeatedly called */
		case 0:
			DisplayButtonsStatus();
			break;
		case 1:
			DisplayFRFocusAuxActionStatus();
			break;
		case 2:
			if( SlewState != SlewDone)
				/* if middle of slew, update Current.A, Z */
				SetCurrentAltazToAccumMs();
			break;
		case 3:
			if( Two.Init)
			{
				if( SlewState != SlewDone)
				{
					SlewCurrent = Current;
					SetCurrentToSlew = True;
					HPEventGetEquat();
				}
				else
					SetCurrentToSlew = False;
				if( Current.Ra != HoldRa)
				{
					GetHMSH( RadToHundSec*Current.Ra, &Current.RaHMSH);
					DisplayJumboRa( &Current);
					HoldRa = Current.Ra;
					PUT_SCOPE;
				}
				if( SetCurrentToSlew)
					Current = SlewCurrent;
				break;
			}
		case 4:
			if( Two.Init)
			{
				if( SlewState != SlewDone)
				{
					SlewCurrent = Current;
					SetCurrentToSlew = True;
					HPEventGetEquat();
				}
				else
					SetCurrentToSlew = False;
				if( Current.Dec != HoldDec)
				{
					GetDMS( RadToArcsec*Current.Dec, &Current.DecDMS);
					DisplayJumboDec( &Current);
					HoldDec = Current.Dec;
					PUT_SCOPE;
				}
				if( SetCurrentToSlew)
					Current = SlewCurrent;
				break;
			}
		default:
			HTask = -1;
	}
	HTask++;
}

void SequentialLowPriController( void)
{
	static int HoldSlewState, HoldAbortState;

	TextAttr = DisplayText;

	switch( LTask)
	{
		case 0:
			if( HsTimerFlag && (HoldSlewState != SlewState))
			{
				VidMemXY = DisplayXY[DisplayHsSlew];
				sprintf( StrBuf, "%s", SlewText[SlewState]);
				WriteStrBufToScreen_f_ptr();
				HoldSlewState = SlewState;
				break;
			}
		case 1:
			if( HsTimerFlag && (HoldAbortState != AbortState))
			{
				VidMemXY = DisplayXY[DisplayHsAbort];
				sprintf( StrBuf, "%s", AbortText[AbortState]);
				WriteStrBufToScreen_f_ptr();
				HoldAbortState = AbortState;
				break;
			}
		case 2:
			if( SlewState == SlewDone)
			{
				getdate( &d);
				gettime( &t);
				sprintf( StrBuf, "%2d/%02d %2d:%02d:%02d", d.da_mon, d.da_day, t.ti_hour, t.ti_min,
				t.ti_sec);
				VidMemXY = DisplayXY[DisplayDateTime];
				WriteStrBufToScreen_f_ptr();
				break;
			}
		case 3:
			if( HsTimerFlag && (SlewState != SlewDone))
			{
				VidMemXY = DisplayXY[DisplayHsSteps];

				sprintf( StrBuf, "%12ld/%8ld/%8ld/%8ld/%8ld/%4d", RampHs, RampUpSteps, MaxSlewUpSteps,
				MaxSlewDownSteps, RampDownSteps, HsRepsIx);

				WriteStrBufToScreen_f_ptr();
				break;
			}
		case 4:
			VidMemXY = DisplayXY[DisplaySidT];
			VidMemSidT( &Current);
			break;
		case 5:
			if( LX200ComPort && LX200DisplayFlag)
			{
				DisplayLX200CommandsAndCharBuffer();
				break;
			}
		default:
			LTask = -1;
	}
	LTask++;
}

void SequentialTaskController( void)
{
	static int HoldFocusPosition;

	TextAttr = DisplayText;

	/* start of tasks to be executed every time... */
	CheckSlewBeep();
	/* fix current sidereal time if using IRQ8 method of timing interrupts, which disturbs the OS
	clock */
	if( HsTimerFlag && SlewState != SlewDone)
	{
		disable();
		ElapsedHSSec = (double) timer_count / TIMER_FREQ;
		enable();
		Current.SidT = StartSidTHS + ElapsedHSSec * SecToRad;
	}

	/* display focus position every time for precise focusing to a particular position */
	if( (FocusMethod == FocusMethod_Pulse_1_14 || FocusMethod == FocusMethod_Pulse_16_17)
	&& HoldFocusPosition != FocusPosition)
	{
		WriteFocusPosition();
		HoldFocusPosition = FocusPosition;
	}
	/* end of tasks to be executed every time... */

	/* now handle other tasks */
	SequentialHiPriController();
	SequentialHiPriController();
	SequentialLowPriController();
}

void CheckSlewBeep( void)
{
	if( SlewBeep == SlewBeepOn)
	{
		SlewBeep = SlewBeepOff;
		SlewBeepSound = 0;
		nosound();
	}
	else
		if( SlewBeep == SlewFinished)
		{
			switch( SlewBeepSound)
			{
				case 0:
					SlewBeepSound = 1000;
					break;
				case 1000:
					SlewBeepSound = 800;
					break;
				case 800:
					SlewBeepSound = 600;
					break;
				case 600:
					SlewBeepSound = 400;
					SlewBeep = SlewBeepOn;
					break;
			}
			sound( SlewBeepSound);
		}
}

/* sorts Position, making 'A' the closest to Current */

void SortPosition( struct Position* A, struct Position* B, const double Diff)
{
	double InitADistance, InitBDistance;
	struct Position Temp;

	InitADistance = CalcEquatAngularSep( &Current, A);
	InitBDistance = CalcEquatAngularSep( &Current, B);
	if( InitADistance > InitBDistance + Diff)
	{
		Temp = *B;
		*B = *A;
		*A = Temp;
	}
}

/* bubble sort One, Two, (Three if ok), Temp, with One being the minimum distance from Current and
Temp being the maximum distance from Current */

void BubbleSortPositions( struct Position* Temp)
{
	Flag Duplicate = No;

	if( Temp->Ra == One.Ra && Temp->Dec == One.Dec && Temp->Alt == One.Alt && Temp->Az == One.Az &&
	Temp->SidT == One.SidT)
		Duplicate = Yes;
	if( Temp->Ra == Two.Ra && Temp->Dec == Two.Dec && Temp->Alt == Two.Alt && Temp->Az == Two.Az &&
	Temp->SidT == Two.SidT)
		Duplicate = Yes;
	if( Temp->Ra == Three.Ra && Temp->Dec == Three.Dec && Temp->Alt == Three.Alt &&
	Temp->Az == Three.Az && Temp->SidT == Three.SidT)
		Duplicate = Yes;

	if( !Duplicate)
	{
		if( !Three.Init)
		{
			SortPosition( &One, &Two, ArcminToRad);
			SortPosition( &Two, Temp, ArcminToRad);

			SortPosition( &One, &Two, ArcminToRad);
		}
		else
		{
			SortPosition( &One, &Two, ArcminToRad);
			SortPosition( &Two, &Three, ArcminToRad);
			SortPosition( &Three, Temp, ArcminToRad);

			SortPosition( &One, &Two, ArcminToRad);
			SortPosition( &Two, &Three, ArcminToRad);

			SortPosition( &One, &Two, ArcminToRad);
		}
	}
}


