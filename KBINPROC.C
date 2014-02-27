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

void DisplayInit( void)
{
	int StartX = InitFrame.Left + 5;
	int StartY = InitFrame.Top + 1;
	int AltX, AzX, RaX, DecX, SidTX;

	AltX = StartX + 3;
	AzX = AltX + SizeofDegX + 1;
	RaX = AzX + SizeofDegX + 1;
	DecX = RaX + SizeofHMSX + 1;
	SidTX = DecX + SizeofDMSX + 1;

	WriteWindow( InitFrame);

	TextAttr = DefaultText;
	VidMemXY.Y = StartY;
	VidMemXY.X = AltX;

	if( One.Init)
	{
		sprintf( StrBuf, "Alt");
		WriteStrBufToScreen_f_ptr();
		VidMemXY.X = AzX;
		sprintf( StrBuf, "Az");
		WriteStrBufToScreen_f_ptr();
		VidMemXY.X = RaX;
		sprintf( StrBuf, "Ra");
		WriteStrBufToScreen_f_ptr();
		VidMemXY.X = DecX;
		sprintf( StrBuf, "Dec");
		WriteStrBufToScreen_f_ptr();
		VidMemXY.X = SidTX;
		sprintf( StrBuf, "SidT");
		WriteStrBufToScreen_f_ptr();

		VidMemXY.Y++;
		VidMemXY.X = StartX;
		sprintf( StrBuf, "1:");
		WriteStrBufToScreen_f_ptr();
		VidMemXY.X = AltX;
		VidMemDeg( One.Alt);
		VidMemXY.X = AzX;
		VidMemDeg( One.Az);
		VidMemXY.X = RaX;
		VidMemRaHMS( &One);
		VidMemXY.X = DecX;
		VidMemDecDMS( &One);
		VidMemXY.X = SidTX;
		VidMemSidT( &One);
	}
	else
	{
		sprintf( StrBuf, "no inits to display");
		WriteStrBufToScreen_f_ptr();
		VidMemXY.Y++;
	}

	if( Two.Init)
	{
		VidMemXY.Y++;
		VidMemXY.X = StartX;
		sprintf( StrBuf, "2:");
		WriteStrBufToScreen_f_ptr();
		VidMemXY.X = AltX;
		VidMemDeg( Two.Alt);
		VidMemXY.X = AzX;
		VidMemDeg( Two.Az);
		VidMemXY.X = RaX;
		VidMemRaHMS( &Two);
		VidMemXY.X = DecX;
		VidMemDecDMS( &Two);
		VidMemXY.X = SidTX;
		VidMemSidT( &Two);
	}
	if( Three.Init)
	{
		VidMemXY.Y++;
		VidMemXY.X = StartX;
		sprintf( StrBuf, "3:");
		WriteStrBufToScreen_f_ptr();
		VidMemXY.X = AltX;
		VidMemDeg( Three.Alt);
		VidMemXY.X = AzX;
		VidMemDeg( Three.Az);
		VidMemXY.X = RaX;
		VidMemRaHMS( &Three);
		VidMemXY.X = DecX;
		VidMemDecDMS( &Three);
		VidMemXY.X = SidTX;
		VidMemSidT( &Three);                 
	}
	if( Two.Init)
	{
		VidMemXY.Y += 2;
		VidMemXY.X = StartX;
		sprintf( StrBuf, "Lat: ");
		WriteStrBufToScreen_f_ptr();
		VidMemDeg( Lat);
		sprintf( StrBuf, " (dif %3.3f)", Lat*RadToDeg - LatitudeDeg);
		WriteStrBufToScreen_f_ptr();

		sprintf( StrBuf, " Long: ");
		WriteStrBufToScreen_f_ptr();
		VidMemDeg( LongitudeRad);
		sprintf( StrBuf, " (dif %3.3f)", LongitudeRad*RadToDeg - LongitudeDeg);
		WriteStrBufToScreen_f_ptr();

		VidMemXY.Y++;
		VidMemXY.X = StartX;
		sprintf( StrBuf, "AzOff: ");
		WriteStrBufToScreen_f_ptr();
		VidMemDeg( AzOff);

		sprintf( StrBuf, " HAOff: ");
		WriteStrBufToScreen_f_ptr();
		GetHMSH( RadToHundSec*HAOff + 0.5, &HAOffHMSH);
		StrBufSHMS( HAOffHMSH);
		WriteStrBufToScreen_f_ptr();
		sprintf( StrBuf, " or %3.3f deg", HAOff*RadToDeg);
		WriteStrBufToScreen_f_ptr();

		VidMemXY.Y+=2;
		VidMemXY.X = StartX;
		sprintf( StrBuf, "To polar align, move %3.3f deg %s, %3.3f deg %s",
		fabs( PolarAlign.A*RadToDeg), PolarAlign.A<0.?"down":"up",
		fabs( PolarAlign.Z*RadToDeg), PolarAlign.Z<0.?"left":"right");
		WriteStrBufToScreen_f_ptr();
		VidMemXY.Y++;
	}
	VidMemXY.Y++;
	VidMemXY.X = StartX;
	sprintf( StrBuf, "Z1: %3.3f   Z2: %3.3f   Z3: %3.3f", Z1*RadToDeg,
	Z2*RadToDeg, Z3*RadToDeg);
	WriteStrBufToScreen_f_ptr();

	gotoxy( StartX + 6, VidMemXY.Y += 3);
	ContMsgRoutine();
	RemoveWindow( InitFrame);
}


