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

/* writes ContMsg and waits for keyboard tap */
void ContMsgRoutine( void)
{
	printf( "%s", ContMsg);
	while( True)
	{
		if( KeyStroke)
		{
			getch();
			break;
		}
		if( UseMouseFlag && MouseLeftButtonPressCount())
			break;
	}
}

void BadMsgExit( const char* Msg)
{
	clrscr();
	gotoxy( 1, 1);
	printf( "Error: %s.\n", Msg);
	ContMsgRoutine();
	_exit( 1);
}

void InitCommonVars( void)
{
	headkeys = (int far*) HeadKeysMemoryLocation;
	tailkeys = (int far*) TailKeysMemoryLocation;

	/* all internal values kept in radians:
	1 revolution = 2 Pi Radians = 360 Degrees = 24 Hours = 1440 Minutes */
	OneRev = 2*M_PI;
	HalfRev = OneRev/2;
	QtrRev = OneRev/4;
	RadToDeg = 360./OneRev;
	DegToRad = OneRev/360.;
	RadToArcmin = 60.*RadToDeg;
	ArcminToRad = DegToRad/60.;
	RadToArcsec = 60.*RadToArcmin;
	ArcsecToRad = ArcminToRad/60.;
	RadToTenthsArcsec = 10.*RadToArcsec;
	TenthsArcsecToRad = ArcsecToRad/10.;
	RadToHr = 24./OneRev;
	HrToRad = OneRev/24.;
	RadToMin = 60.*RadToHr;
	MinToRad = HrToRad/60.;
	RadToSec = 60.*RadToMin;
	SecToRad = MinToRad/60.;
	RadToHundSec = 100.*RadToSec;
	HundSecToRad = SecToRad/100.;
	RadToMilliSec = 1000.*RadToSec;
	MilliSecToRad = SecToRad/1000.;
	DaysToHr = 24.;
	DaysToMin = 60.*DaysToHr;
	DaysToSec = 60.*DaysToMin; 
	ArcsecPerRev = 1296000.;
	RevPerArcsec = 1/1296000.;
	ArcsecPerDeg = 3600.;
   DegPerArcsec = 1/3600.;

	SidRate = 1.002737909; 

	/* ~18.2065 clock ticks per second */
	ClockTicksDay = 1573040.;
	ClockTicksSec = ClockTicksDay/DaysToSec;
	ClockTicksMin = ClockTicksDay/DaysToMin;
	ClockTicksHr = ClockTicksDay/DaysToHr;
	RadToClockTick = ClockTicksDay/OneRev;
	ClockTickToRad = OneRev/ClockTicksDay;

	LargeNum[0] = 1+2+4+8+16+32;
	LargeNum[1] = 2+4;
	LargeNum[2] = 1+2+8+16+64;
	LargeNum[3] = 1+2+4+8+64;
	LargeNum[4] = 2+4+32+64;
	LargeNum[5] = 1+4+8+32+64;
	LargeNum[6] = 4+8+16+32+64;
	LargeNum[7] = 1+2+4;
	LargeNum[8] = 1+2+4+8+16+32+64;
	LargeNum[9] = 1+2+4+32+64;
}

void DisplayNegSign( const int startx, const int starty, const int xsize, const int ysize,
const int attr)
{
	int endx;

	endx = startx + 2*xsize/3;
	VidMemXY.Y = starty+ysize/2;
	for( VidMemXY.X = startx+xsize/3; VidMemXY.X <= endx; VidMemXY.X++)
		Screen[VidMemXY.Y][VidMemXY.X].Attr = attr;
}

void DisplayColon( const int startx, const int starty, const int xsize, const int ysize,
const int attr)
{
	Screen[starty+ysize/4][1+startx+xsize/2].Attr = attr;
	Screen[starty+3*ysize/4][1+startx+xsize/2].Attr = attr;
}

void DisplayLargeNum( const int num, const int startx, const int starty, const int xsize,
const int ysize, const int attr)
{
	int endx, endy;

	if( num > 9)
		BadExit( "bad display digit in DisplayLargeNum(...)");

	if( LargeNum[num] & 1)
	{
		endx = startx + xsize+1;
		VidMemXY.Y = starty;
		for( VidMemXY.X = startx; VidMemXY.X <= endx; VidMemXY.X++)
			Screen[VidMemXY.Y][VidMemXY.X].Attr = attr;
	}
	if( LargeNum[num] & 64)
	{
		endx = startx + xsize+1;
		VidMemXY.Y = starty + ysize/2;
		for( VidMemXY.X = startx; VidMemXY.X <= endx; VidMemXY.X++)
			Screen[VidMemXY.Y][VidMemXY.X].Attr = attr;
	}
	if( LargeNum[num] & 8)
	{
		endx = startx + xsize+1;
		VidMemXY.Y = starty + ysize;
		for( VidMemXY.X = startx; VidMemXY.X <= endx; VidMemXY.X++)
			Screen[VidMemXY.Y][VidMemXY.X].Attr = attr;
	}
	if( LargeNum[num] & 32)
	{
		endy = starty + ysize/2;
		VidMemXY.X = startx;
		for( VidMemXY.Y = starty; VidMemXY.Y <= endy; VidMemXY.Y++)
		{
			Screen[VidMemXY.Y][VidMemXY.X].Attr = attr;
		}
	}
	if( LargeNum[num] & 2)
	{
		endy = starty + ysize/2;
		VidMemXY.X = startx + xsize + 1;
		for( VidMemXY.Y = starty; VidMemXY.Y <= endy; VidMemXY.Y++)
		{
			Screen[VidMemXY.Y][VidMemXY.X].Attr = attr;
		}
	}
	if( LargeNum[num] & 16)
	{
		endy = starty + ysize;
		VidMemXY.X = startx;
		for( VidMemXY.Y = starty + ysize/2; VidMemXY.Y <= endy; VidMemXY.Y++)
		{
			Screen[VidMemXY.Y][VidMemXY.X].Attr = attr;
		}
	}
	if( LargeNum[num] & 4)
	{
		endy = starty + ysize;
		VidMemXY.X = startx + xsize + 1;
		for( VidMemXY.Y = starty + ysize/2; VidMemXY.Y <= endy; VidMemXY.Y++)
		{
			Screen[VidMemXY.Y][VidMemXY.X].Attr = attr;
		}
	}
}

/*
void TestDisplayLargeNum( void)
{
	int startx = 1;
	int starty = 1;
	int xsize = 7;
	int ysize = 8;
	int attr = GREEN << 4;

	clrscr();
	for( Ix = 0; Ix <= 9; Ix++)
	{
		DisplayLargeNum( Ix, startx, starty, xsize, ysize, attr);
		startx += xsize + 3;
		if( startx >= 70)
		{
			startx = 1;
			starty = ysize + 3;
		}
	}
	ContMsgRoutine();
}
*/

void GetCurDir( char* Dir)
{
	/* fill string with "X:\" */
	strcpy( Dir, "X:\\");
	/* change the 'X' to the current drive */
	Dir[0] = 'A' + getdisk();
	/* fill rest of string with path */
	getcurdir( 0, Dir + 3);
}

Flag Gets( char Str[], const int NumChars)
{
	int IxA, IxB;

	for( IxA = 0; IxA < NumChars; IxA++)
		Str[IxA] = ' ';
	Str[NumChars] = '\0';
	printf( "[");
	VidMemXY.X = wherex();
	VidMemXY.Y = wherey();
	gotoxy( VidMemXY.X + NumChars, VidMemXY.Y);
	printf( "]");
	gotoxy( VidMemXY.X, VidMemXY.Y);
	IxA = 0;
	Response = getch();
	while( Response != Return && Response != Esc)
	{
		if( IxA && (Response == Backspace || Response == LeftCursor))
			Str[--IxA] = ' ';
		else if( IxA < NumChars)
			Str[IxA++] = Response;
		for( IxB = 0; IxB < IxA; IxB++)
		{
			gotoxy( VidMemXY.X + IxB, VidMemXY.Y);
			printf( "%c", Str[IxB]);
		}
		for( IxB = IxA; IxB < NumChars; IxB++)
		{
			gotoxy( VidMemXY.X + IxB, VidMemXY.Y);
			printf( " ");
		}
		gotoxy( VidMemXY.X + IxA, VidMemXY.Y);
		Response = getch();
	}
	if( Response == Return)
		if( IxA)
			return UserEnteredNumber;
		else
			return UserDidNotEnterNumber;
	else
		return UserEscaped;
}

Flag GetNumStr( char NumStr[], int NumChars)
{
	int IxA, IxB;

	for( IxA = 0; IxA < NumChars; IxA++)
		NumStr[IxA] = ' ';
	NumStr[NumChars] = '\0';
	printf( "[");
	VidMemXY.X = wherex();
	VidMemXY.Y = wherey();
	gotoxy( VidMemXY.X + NumChars, VidMemXY.Y);
	printf( "]");
	gotoxy( VidMemXY.X, VidMemXY.Y);
	IxA = 0;
	Response = getch();
	while( Response != Return && Response != Esc)
	{
		if( IxA && (Response == Backspace || Response == LeftCursor))
			NumStr[--IxA] = ' ';
		else if( IxA < NumChars && ((Response >= '0' && Response <= '9') || Response == '.' ||
		Response == '+' || Response == '-'))
				NumStr[IxA++] = Response;
		for( IxB = 0; IxB < IxA; IxB++)
		{
			gotoxy( VidMemXY.X + IxB, VidMemXY.Y);
			printf( "%c", NumStr[IxB]);
		}
		for( IxB = IxA; IxB < NumChars; IxB++)
		{
			gotoxy( VidMemXY.X + IxB, VidMemXY.Y);
			printf( " ");
		}
		gotoxy( VidMemXY.X + IxA, VidMemXY.Y);
		Response = getch();
	}
	if( Response == Return)
		if( IxA)
			return UserEnteredNumber;
		else
			return UserDidNotEnterNumber;
	else
		return UserEscaped;
}

Flag GetFlag( Flag* Num)
{
	char NumStr[MaxCharsFlag+1];
	Flag R;

	R = GetNumStr( NumStr, MaxCharsFlag);
	if( R == UserEnteredNumber)
		*Num = (Byte) atoi( NumStr);
	return R;
}

Flag GetInt( int* Num)
{
	char NumStr[MaxCharsInt+1];
	Flag R;

	R = GetNumStr( NumStr, MaxCharsInt);
	if( R == UserEnteredNumber)
		*Num = atoi( NumStr);
	return R;
}

Flag GetLong( long* Num)
{
	char NumStr[MaxCharsLong+1];
	Flag R;

	R = GetNumStr( NumStr, MaxCharsLong);
	if( R == UserEnteredNumber)
		*Num = atol( NumStr);
	return R;
}

Flag GetDouble( double* Num)
{
	char NumStr[MaxCharsDbl+1];
	Flag R;

	R = GetNumStr( NumStr, MaxCharsDbl);
	if( R == UserEnteredNumber)
		*Num = atof( NumStr);
	return R;
}

void FReadDouble( FILE* File, double* V)
{
	if( !feof( File))
	{
		fscanf( File, "%s", Value);
		*V = atof( Value);
	}
}

void FReadFlag( FILE* File, Flag* V)
{
	if( !feof( File))
	{
		fscanf( File, "%s", Value);
		*V = (Flag) atoi( Value);
	}
}

void FReadByte( FILE* File, Byte* V)
{
	if( !feof( File))
	{
		fscanf( File, "%s", Value);
		*V = (Byte) atoi( Value);
	}
}

void FReadInt( FILE* File, int* V)
{
	if( !feof( File))
	{
		fscanf( File, "%s", Value);
		*V = atoi( Value);
	}
}

void FReadUnsigned( FILE* File, unsigned* V)
{
	if( !feof( File))
	{
		fscanf( File, "%s", Value);
		*V = (unsigned) atoi( Value);
	}
}

void FReadLong( FILE* File, long* V)
{
	if( !feof( File))
	{
		fscanf( File, "%s", Value);
		*V = atol( Value);
	}
}

void FReadChar( FILE* File, char* V)
{
	if( !feof( File))
		fread( V, 1, 1, File);
	else
		*V = ' ';
}

void FReadToNewLine( FILE* File)
{
	char Dummy = ' ';

	while( !feof( File) && Dummy != '\n')
			fread( &Dummy, 1, 1, File);
}

void FReadToChar( FILE* File, const char V)
{
	char Dummy = ' ';

	while( !feof( File) && Dummy != V)
			fread( &Dummy, 1, 1, File);
}

/* read until char count exhausted or newline: return yes if newline encountered */
Flag FReadStringToCharCountOrNewLine( FILE* File, char* Name, const int CharCount)
{
	int Ix;
	Flag NewLineFlag;

	Name[0] = '\0';
	/* read past blanks */
	do
	{
		FReadChar( File, &Name[0]);
	}while( !feof( File) && Name[0] == ' ');
	Ix = 0;
	while( !feof( Input) && Ix < CharCount && Name[Ix++] != '\n')
		FReadChar( File, &Name[Ix]);
	Ix--;
	if( Name[Ix] == '\n')
		NewLineFlag = True;
	else
		NewLineFlag = False;
	/* fill out name */
	while( Ix < CharCount)
		Name[Ix++] = '\0';
	return NewLineFlag;
}

/* returns next increment count above highest numbered file name, ie, if name00.txt and name02.txt
exist, then 3 is returned, if no file exists, then returns 0 */
int FindNextIncrFilename( char* Name, char* Extension)
{
	char Fname[13];
	int value;
	int highestvalue = -1;

	strcpy( Fname, Name);
	strcat( Fname, "??.");
	strcat( Fname, Extension);

	FindFirst = True;
	FoundFile = True;

	while( FoundFile)
	{
		if( FindFirst)
		{
			FindFirst = False;
			/* returns 0 if successful */
			FoundFile = !findfirst( Fname, &Ffblk, 0);
		}
		else
			FoundFile = !findnext( &Ffblk);

		if( FoundFile)
			if( Ffblk.ff_name[strlen( Name)] >= '0' && Ffblk.ff_name[strlen( Name)] <= '9' &&
			Ffblk.ff_name[strlen( Name)+1] >= '0' && Ffblk.ff_name[strlen( Name)+1] <= '9')
			{
				value = (Ffblk.ff_name[strlen( Name)]-'0') * 10 + Ffblk.ff_name[strlen( Name)+1]-'0';

				if( value > highestvalue)
					highestvalue = value;
			}
	}
	return highestvalue + 1;
}

/* serial port... */

/* interrupt driven receive and transmit */

void interrupt Ser1ISR( void)
{
	/* receive */
	while( inportb( Com1Base + LSR) & DATA_READY)
	{
		SerBufRead[0][SerBufReadEndIx[0]++] = inportb( Com1Base);
		/* SerBufEnd must be 2^n - 1 for this to work: much faster than comparing and setting to zero */
		SerBufReadEndIx[0] &= SerBufEnd;
		SerReadCount[0]++;
	}
	/* transmit */
	if( (inportb( Com1Base + IIR) & 7) == XMT_INT)
		/* if no data to transmit then turn off transmit interrupt */
		if( SerBufWriteBegIx[0] == SerBufWriteEndIx[0])
		{
			/* clear DLAB */
			outportb( Com1Base + LCR, inportb( Com1Base+LCR) & ~DLAB);
			/* disable transmit interrupt */
			outportb( Com1Base + IER, INT_ON_RCV);
		}
		/* else transmit Byte */
		else
		{
			outportb( Com1Base, SerBufWrite[0][SerBufWriteBegIx[0]]);
			SerBufWriteBegIx[0]++;
			SerBufWriteBegIx[0] &= SerBufEnd;
			SerWriteCount[0]++;
		}
	/* enable PIC to service this interrupt again */
	outportb( PIC_EOI_ADDR, EOI);
}

void interrupt Ser2ISR( void)
{
	while( inportb( Com2Base + LSR) & DATA_READY)
	{
		SerBufRead[1][SerBufReadEndIx[1]++] = inportb( Com2Base);
		SerBufReadEndIx[1] &= SerBufEnd;
		SerReadCount[1]++;
	}
	if( (inportb( Com2Base + IIR) & 7) == XMT_INT)
		if( SerBufWriteBegIx[1] == SerBufWriteEndIx[1])
		{
			outportb( Com2Base + LCR, inportb( Com2Base+LCR) & ~DLAB);
			outportb( Com2Base + IER, INT_ON_RCV);
		}
		else
		{
			outportb( Com2Base, SerBufWrite[1][SerBufWriteBegIx[1]]);
			SerBufWriteBegIx[1]++;
			SerBufWriteBegIx[1] &= SerBufEnd;
			SerWriteCount[1]++;
		}
	outportb( PIC_EOI_ADDR, EOI);
}

void interrupt Ser3ISR( void)
{
	while( inportb( Com3Base + LSR) & DATA_READY)
	{
		SerBufRead[2][SerBufReadEndIx[2]++] = inportb( Com3Base);
		SerBufReadEndIx[2] &= SerBufEnd;
		SerReadCount[2]++;
	}
	if( (inportb( Com3Base + IIR) & 7) == XMT_INT)
		if( SerBufWriteBegIx[2] == SerBufWriteEndIx[2])
		{
			outportb( Com3Base + LCR, inportb( Com3Base+LCR) & ~DLAB);
			outportb( Com3Base + IER, INT_ON_RCV);
		}
		else
		{
			outportb( Com3Base, SerBufWrite[2][SerBufWriteBegIx[2]]);
			SerBufWriteBegIx[2]++;
			SerBufWriteBegIx[2] &= SerBufEnd;
			SerWriteCount[2]++;
		}
	outportb( PIC_EOI_ADDR, EOI);
}

void interrupt Ser4ISR( void)
{
	while( inportb( Com4Base + LSR) & DATA_READY)
	{
		SerBufRead[3][SerBufReadEndIx[3]++] = inportb( Com4Base);
		SerBufReadEndIx[3] &= SerBufEnd;
		SerReadCount[3]++;
	}
	if( (inportb( Com4Base + IIR) & 7) == XMT_INT)
		if( SerBufWriteBegIx[3] == SerBufWriteEndIx[3])
		{
			outportb( Com4Base + LCR, inportb( Com4Base+LCR) & ~DLAB);
			outportb( Com4Base + IER, INT_ON_RCV);
		}
		else
		{
			outportb( Com4Base, SerBufWrite[3][SerBufWriteBegIx[3]]);
			SerBufWriteBegIx[3]++;
			SerBufWriteBegIx[3] &= SerBufEnd;
			SerWriteCount[3]++;
		}
	outportb( PIC_EOI_ADDR, EOI);
}

Flag InitSerial( const int ComPort, const long BaudRate, const int Parity, const int DataBits,
const int StopBits)
{
	unsigned R;
	int Ix, IxA;

	if( ComPort < 1 || ComPort > MaxComPorts)
		return False;

	for( Ix = 0; Ix < MaxComPorts; Ix++)
	{
		for( IxA = 0; IxA <= SerBufEnd; IxA++)
			SerBufWrite[Ix][IxA] = 0;
		SerBufWriteBegIx[Ix] = SerBufWriteEndIx[Ix] = 0;
		SerWriteCount[Ix] = SerBufEnd+1;
	}
	switch( ComPort)
	{
		case 1:
			SerialBase = Com1Base;
			/* save current serial interrupt routine */
			OldSer1IntVec = getvect( Com1Int);
			/* set new serial interrupt routine */
			setvect( Com1Int, Ser1ISR);
			/* set PIC by zeroing relevant bit */
			outportb( IntMaskReg, inportb( IntMaskReg) & (~Int4Mask));
			InitRingBuffers( ComPort);
			break;
		case 2:
			SerialBase = Com2Base;
			/* save current serial interrupt routine */
			OldSer2IntVec = getvect( Com2Int);
			/* set new serial interrupt routine */
			setvect( Com2Int, Ser2ISR);
			/* set PIC by zeroing relevant bit */
			outportb( IntMaskReg, inportb( IntMaskReg) & (~Int3Mask));
			InitRingBuffers( ComPort);
			break;
		case 3:
			SerialBase = Com3Base;
			/* save current serial interrupt routine */
			OldSer3IntVec = getvect( Com3Int);
			/* set new serial interrupt routine */
			setvect( Com3Int, Ser3ISR);
			/* set PIC by zeroing relevant bit */
			outportb( IntMaskReg, inportb( IntMaskReg) & (~Int4Mask));
			InitRingBuffers( ComPort);
			break;
		case 4:
			SerialBase = Com4Base;
			/* save current serial interrupt routine */
			OldSer4IntVec = getvect( Com4Int);
			/* set new serial interrupt routine */
			setvect( Com4Int, Ser4ISR);
			/* set PIC by zeroing relevant bit */
			outportb( IntMaskReg, inportb( IntMaskReg) & (~Int3Mask));
			InitRingBuffers( ComPort);
	}
	/* set UART */
	R = (unsigned) (115200L / BaudRate);
	/* loopback required by early 8250s */
	outportb( SerialBase + MCR, 0x10);
	/* set DLAB = 1, if divisor latch access bit == 1, then ports 0, 1 used
	as baud rate divisor, otherwise ports 0, 1 used to receive/transmit */
	outportb( SerialBase + LCR, 0x80);
	/* load rate divisor LSB */
	outportb( SerialBase, R & 0xFF);
	/* load divisor MSB */
	outportb( SerialBase + IER, (R>>8) & 0xFF);
	/* data bits: 1st two bits used to set DataBits; DataBits can range from 5 (bits value of 0)
	to 8 (bits value of 3) */
	/* stop bits: uses 3rd bit: if 2 StopBits, then set to 1, otherwise 0 */
	/* parity bits: uses 4th to 6th bits */
	/* 7th bit is break bit, 8th bit is DLAB */
	/* set DLAB back to zero, set DataBits, StopBits */
	outportb( SerialBase + LCR, ((DataBits-5) & 3) | Parity | (StopBits==2? 4:0));
	/* turn on DTR, RTS (set to 0 to turn off MCR) */
	outportb( SerialBase + MCR, MCR_DTR | MCR_RTS);
	/* enable UART */
	/* clear out any stray char */
	inportb( SerialBase);
	/* read LSR to clear it */
	inportb( SerialBase + LSR);
	/* ditto MSR */
	inportb( SerialBase + MSR);
	/* enable 8250 Rx interrupt only */
	outportb( SerialBase + IER, INT_ON_RCV);
	/* turn on RTS, DTR, OUT2 */
	outportb( SerialBase + MCR, 0x0B);
	/* code to turn on then off the 16550 fifo
	outportb( SerialBase + IIR, 0x07);
	printf( "\nserial fifo %s", (inportb( SerialBase + IIR) & 0xC0)? "on": "off");
	outportb( SerialBase + IIR, 0);
	printf( "\nserial fifo %s", (inportb( SerialBase + IIR) & 0xC0)? "on": "off");*/
	return True;
}

Flag CloseSerial( const int ComPort)
{
	Byte B;

	if( ComPort < 1 || ComPort > MaxComPorts)
		return False;

	switch( ComPort)
	{
		case 1:
			/* clear out any remaining chars to read */
			while( ReadSerial( ComPort, &B))
				;
			SerialBase = Com1Base;
			/* OUT2 off to disable int */
			outportb( SerialBase + MCR, inportb( SerialBase + MCR) ^ 0x08);
			/* reset PIC, set the relevant bit */
			outportb( IntMaskReg, inportb( IntMaskReg) | Int4Mask);
			/* restore original interrupt vector */
			setvect( Com1Int, OldSer1IntVec);
			break;
		case 2:
			/* clear out any remaining chars to read */
			while( ReadSerial( ComPort, &B))
				;
			SerialBase = Com2Base;
			/* OUT2 off to disable int */
			outportb( SerialBase + MCR, inportb( SerialBase + MCR) ^ 0x08);
			/* reset PIC, set the relevant bit */
			outportb( IntMaskReg, inportb( IntMaskReg) | Int3Mask);
			/* restore original interrupt vector */
			setvect( Com2Int, OldSer2IntVec);
			break;
		case 3:
			/* clear out any remaining chars to read */
			while( ReadSerial( ComPort, &B))
				;
			SerialBase = Com3Base;
			/* OUT2 off to disable int */
			outportb( SerialBase + MCR, inportb( SerialBase + MCR) ^ 0x08);
			/* reset PIC, set the relevant bit */
			outportb( IntMaskReg, inportb( IntMaskReg) | Int4Mask);
			/* restore original interrupt vector */
			setvect( Com3Int, OldSer3IntVec);
			break;
		case 4:
			/* clear out any remaining chars to read */
			while( ReadSerial( ComPort, &B))
				;
			SerialBase = Com4Base;
			/* OUT2 off to disable int */
			outportb( SerialBase + MCR, inportb( SerialBase + MCR) ^ 0x08);
			/* reset PIC, set the relevant bit */
			outportb( IntMaskReg, inportb( IntMaskReg) | Int3Mask);
			/* restore original interrupt vector */
			setvect( Com4Int, OldSer4IntVec);
	}
	return True;
}

void InitRingBuffers( const int ComPort)
{
	if( ComPort < 1 || ComPort > MaxComPorts)
		BadExit( "Bad ComPort in InitRingBuffers()");
	disable();
	SerBufReadBegIx[ComPort-1] = SerBufReadEndIx[ComPort-1] = SerReadCount[ComPort-1] = 0;
	SerBufWriteBegIx[ComPort-1] = SerBufWriteEndIx[ComPort-1] = SerWriteCount[ComPort-1] = 0;
	enable();
}

Flag ReadSerial( const int ComPort, Byte* B)
{
	if( ComPort < 1 || ComPort > MaxComPorts)
		return False;

	disable();
	if( SerReadCount[ComPort-1])
	{
		*B = SerBufRead[ComPort-1][SerBufReadBegIx[ComPort-1]++];
		/* SerBufEnd must be 2^n - 1 for this to work: very much faster than comparing and setting to zero */
		SerBufReadBegIx[ComPort-1] &= SerBufEnd;
		SerReadCount[ComPort-1]--;
		enable();
		return True;
	}
	else
	{
		enable();
		return False;
	}
}

void WriteSerial( const int ComPort, Byte B)
{
	if( ComPort < 1 || ComPort > MaxComPorts)
		BadExit( "Bad ComPort in WriteSerial()");

	SerBufWrite[ComPort-1][SerBufWriteEndIx[ComPort-1]] = B;
	SerBufWriteEndIx[ComPort-1]++;
	SerBufWriteEndIx[ComPort-1] &= SerBufEnd;

	switch( ComPort)
	{
		case 1:
			SerialBase = Com1Base;
			break;
		case 2:
			SerialBase = Com2Base;
			break;
		case 3:
			SerialBase = Com3Base;
			break;
		case 4:
			SerialBase = Com4Base;
			break;
	}
	/* clear DLAB */
	outportb( SerialBase + LCR, inportb( SerialBase+LCR) & ~DLAB);
	/* enable transmit interrupt */
	outportb( SerialBase + IER, INT_ON_RCV + INT_ON_XMT);
}

/* 9600 bps ~= 1000 cps (char/sec) or ~55 char/clocktick */
void WriteSerialString( const int ComPort, char C[], const int CharCount)
{
	int Ix;

	if( ComPort < 1 || ComPort > MaxComPorts)
		BadExit( "Bad ComPort in WriteSerial()");

	for( Ix = 0; Ix < CharCount; Ix++)
	{
		SerBufWrite[ComPort-1][SerBufWriteEndIx[ComPort-1]] = C[Ix];
		SerBufWriteEndIx[ComPort-1]++;
		SerBufWriteEndIx[ComPort-1] &= SerBufEnd;
	}

	switch( ComPort)
	{
		case 1:
			SerialBase = Com1Base;
			break;
		case 2:
			SerialBase = Com2Base;
			break;
		case 3:
			SerialBase = Com3Base;
			break;
		case 4:
			SerialBase = Com4Base;
			break;
	}
	/* clear DLAB */
	outportb( SerialBase + LCR, inportb( SerialBase+LCR) & ~DLAB);
	/* enable transmit interrupt */
	outportb( SerialBase + IER, INT_ON_RCV + INT_ON_XMT);
}

void WriteSerialPauseUntilXmtFinished( const int ComPort, Byte B)
{
	WriteSerial( ComPort, B);
	while( !WriteSerialFinished(ComPort))
		;
}

void WriteSerialStringPauseUntilXmtFinished( const int ComPort, char C[], const int CharCount)
{
	WriteSerialString( ComPort, C, CharCount);
	while( !WriteSerialFinished(ComPort))
		;
}

Flag WriteSerialFinished( const int ComPort)
{
	if( ComPort < 1 || ComPort > MaxComPorts)
		BadExit( "Bad ComPort in WriteSerialFinished()");

	switch( ComPort)
	{
		case 1:
			SerialBase = Com1Base;
			break;
		case 2:
			SerialBase = Com2Base;
			break;
		case 3:
			SerialBase = Com3Base;
			break;
		case 4:
			SerialBase = Com4Base;
			break;
	}
	if( inportb( SerialBase + LSR) & XMT_COMPLETE)
		return True;
	else
		return False;
}

void DisplaySerialString( char C[], const int CharCount)
{
	printf( "\nserial output: ");
	for( Ix = 0; Ix < CharCount; Ix++)
		printf( "%c", C[Ix]);
	printf( " (hex: ");
	for( Ix = 0; Ix < CharCount; Ix++)
		printf( " %#x ", (Byte) C[Ix]);
	printf( ")");
}

void TestSerial( const int ComPort)
{
	Flag QuitFlag = No;
	Byte B;
	Flag Hex;

	CloseSerial( ComPort);
	InitSerial( ComPort, LX200BaudRate, Parity, DataBits, StopBits);

	printf( "\n\n\nTest of serial port on com %1d (%ld,8,n,1):\n", ComPort, LX200BaudRate);
	printf( "Display in hex (y/n)? ");
	Response = getch();
	printf( "%c", Response);
	printf( "\nPress ? to enter a string, end with the return key");
	printf( "\nPress ! to quit\n\n");
	if( Response == 'Y' || Response == 'y' || Response == Return)
		Hex = Yes;
	else
		Hex = No;

	while( !QuitFlag)
	{
		if( ReadSerial( ComPort, &B))
			if( Hex)
				printf( " %#x ", (int)B);
			else
				printf( "%c", B);

		if( KeyStroke)
		{
			B = getch();
			if( B == '!')
				QuitFlag = Yes;
			else
				if( B == '?')
				{
					gets( StrBuf);
					WriteSerialString( ComPort, StrBuf, strlen( StrBuf));
				}
				else
				{
					WriteSerial( ComPort, B);
					printf( "%c", B);
				}
		}
	}
	NewLine;
	ContMsgRoutine();
}

/* video... courtesy Dale Eason */

/* Speed gain by using direct video memory is considerable */

/* The PC's screen in text mode consists of 25 rows and 80 columns.
The first row and column is addressed as location (0, 0), and the last row and column as location
(24, 79).  This is swapped from the goto( x, y) function.  Each screen location actually consists of
two components: a character and an attribute.  To support this arragement, a structure called
'Texel' is used to represent the character and the attribute written to a screen location.

To write a character and its attribute to a screen location, a screen pointer is used:
ScreenArea far *ScreenPtr;

This screen pointer can be set to one of two values: B000:000h for a monochrome display adapter and
B800:000h for a graphics adapter.  To determine which adapter is installed, use a call to the
ROM-BIOS (interrupt 0x10).

Once ScreenPtr has been assigned a screen address, a character and an attribute can be written to
the screen.  But first, to simplify the notation, we've defined the macro constant: #define Screen
(*ScreenPtr)

note: (*ScreenPtr)[][] is a pointer to an array, vs *ScreenPtr[][] which is an array of pointers,
'[]' taking precedence over '*': (*ScreenPtr)[5][10].Ch = 'A' must be written with parenthesis;

Putting all this together, we can write the character A with normal video to row 6, column 11 using:

	  Screen[5][10].Ch = 'A';
	  Screen[5][10].Attr = 7;

We can also use this notation to read a character and its attributes at a specific screen location:

	 Ch = Screen[5][10].Ch;
	 Attr = Screen[5][10].Attr; */

void InitVideo( const Flag DisplayOpeningMsgs)
{
	Regs.h.ah = 15;
	int86( 0x10, &Regs, &Regs);
	if( Regs.h.al == 7)
		MemSeg = 0xB000;
	else
		MemSeg = 0xB800;
	MemOff = Regs.h.bh * (unsigned) 0x1000;
	ScreenPtr = (ScreenArea far*) (( (long) MemSeg << 16) | (long) MemOff);

	WriteCharToScreen_f_ptr = WriteCharToScreen;
	WriteStrBufToScreen_f_ptr = WriteStrBufToScreen;

	if( DisplayOpeningMsgs)
   	printf( "\ndirect video memory access initialized");
}

void NULL_FUNCTION( void) {}

/* write a character to the screen at Y and X */
void WriteCharToScreen( const char Char)
{
	Screen[VidMemXY.Y][VidMemXY.X].Ch = Char;
	Screen[VidMemXY.Y][VidMemXY.X].Attr = TextAttr;
	/*
	textattr( TextAttr);
	gotoxy( VidMemXY.X+1, VidMemXY.Y+1);
	cprintf( "%c", Char);
	*/
}

/* write a character string using StrBuf to the screen at Y and X; VidMemXY.X is changed, VidMemXY.Y
is not */
void WriteStrBufToScreen( void)
{
	char* StrPtr = StrBuf;

	while( *StrPtr)
	{
		Screen[VidMemXY.Y][VidMemXY.X].Attr = TextAttr;
		Screen[VidMemXY.Y][VidMemXY.X++].Ch = *StrPtr++;
	}
	/* textattr() and cprintf() will also print text to the console with the correct attributes, ie,
	textattr( TextAttr);
	gotoxy( VidMemXY.X+1, VidMemXY.Y+1);
	while( *StrPtr)
		cprintf( "%c", *StrPtr++);
	*/
}

void Clrscr( void)
{
	clrscr();

	TextAttr = DefaultText;
	gettextinfo( &ti);
	for( VidMemXY.X = ti.winleft-1; VidMemXY.X < ti.winright; VidMemXY.X++)
		for( VidMemXY.Y = ti.wintop-1; VidMemXY.Y < ti.winbottom; VidMemXY.Y++)
			WriteCharToScreen( ' ');

}

/*
void TestVideo( void)
{
	struct XY HoldXY;
	int i;
	const maxi = MAXINT - 1;
	time_t t;
	time_t start;

	clrscr();
	printf( "\n\n\nTest of video code...\n");
	printf( "segment %u   offset %d\n\n", MemSeg, MemOff);

	directvideo = 1;
	HoldXY.X = wherex();
	HoldXY.Y = wherey();
	printf( "To display ");

	// my turn
	start = time( NULL);
	for( i = 1; i <= maxi; i++)
	{
		// my direct video
		sprintf( StrBuf, "%6d", i);
		VidMemXY.X = HoldXY.X + 11;
		VidMemXY.Y = HoldXY.Y - 1;
		WriteStrBufToScreen_f_ptr();
	}
	t = time( NULL);
	gotoxy( HoldXY.X + 20, HoldXY.Y);
	printf( " times");
	gotoxy( HoldXY.X, HoldXY.Y + 1);
	printf( "Took my code %d seconds", t - start);

	// Borland's turn
	start = time( NULL);
	for( i = 1; i <= maxi; i++)
	{
		gotoxy( HoldXY.X + 12, HoldXY.Y);
		printf( "%6d", i);
	}
	t = time( NULL);
	gotoxy( HoldXY.X, HoldXY.Y + 2);
	printf( "Took Borland with direct video %d seconds", t - start);

	// direct video off
	directvideo = 0;
	start = time( NULL);
	for( i = 1; i <= maxi; i++)
	{
		gotoxy( HoldXY.X + 12, HoldXY.Y);
		printf( "%6d", i);
	}
	t = time( NULL);
	gotoxy( HoldXY.X, HoldXY.Y + 3);
	printf( "Borland w/o direct video %d seconds", t - start);

	NewLine;
	ContMsgRoutine();
}
*/

/* CMOS clock used to reset DOS clock when DOS clock has been disturbed */

int UpdateInProgress( void)
{
	int Ix;

	/* docs at http://www.cubic.org/source/archive/hardware/cmos/cmos.txt say that CMOS_Reg_Status_B
	can also be used */
	outportb( CCP, CMOS_Reg_Status_A);
	/* add delay to give time for return to settle */
	for( Ix=0; Ix < 5; Ix++)
		;
	return CMOS_RTN & Update_In_Progress;
}

void DisplayCMOSTimeDate( void)
{
	printf( "%2d:%02d:%02d   (%2d)%02d/%02d/%02d", CMOS_Hr, CMOS_Min, CMOS_Sec,
	Century, CMOS_Yr, CMOS_Mon, CMOS_Day);
}

void SetDOSToCMOS_RTC_ViaPort( void)
{
	disable();
	/* wait until CMOS clock is ready to increment to next second */
	while( !UpdateInProgress())
		;
	/* wait until CMOS clock is finished updating */
	while( UpdateInProgress())
		;

	/* get CMOS values */
	outportb( CCP, CMOS_Reg_BCD_Sec);
	BCD = CMOS_RTN;
	CMOS_Sec = DECODE_BCD;
	outportb( CCP, CMOS_Reg_BCD_Min);
	BCD = CMOS_RTN;
	CMOS_Min = DECODE_BCD;
	outportb( CCP, CMOS_Reg_BCD_Hr);
	BCD = CMOS_RTN;
	CMOS_Hr = DECODE_BCD;
	outportb( CCP, CMOS_Reg_BCD_Day);
	BCD = CMOS_RTN;
	CMOS_Day = DECODE_BCD;
	outportb( CCP, CMOS_Reg_BCD_Mon);
	BCD = CMOS_RTN;
	CMOS_Mon = DECODE_BCD;
	outportb( CCP, CMOS_Reg_BCD_Yr);
	BCD = CMOS_RTN;
	CMOS_Yr = DECODE_BCD;

	SetDOSToCMOS_Vars();
	enable();
}

void SetDOSToCMOS_RTC_ViaBios( void)
{
	ReadRealTimeClock( &CMOS_Hr, &CMOS_Min, &CMOS_Sec, &daylight);
	ReadDateofRealTimeClock( &Century, &CMOS_Yr, &CMOS_Mon, &CMOS_Day);
	SetDOSToCMOS_Vars();
}

void SetDOSToCMOS_Vars( void)
{
	static int PrevYr, PrevMon, PrevDay;

	if( CMOS_Yr == 0 && PrevYr == 99 && CMOS_Mon == 1 && PrevMon == 12 && CMOS_Day == 1 &&
	PrevDay == 31 && Century == 19)
		Century = 20;
	d.da_year = Century * 100 + CMOS_Yr;
	d.da_mon = CMOS_Mon;
	d.da_day = CMOS_Day;
	setdate( &d);
	t.ti_hour = CMOS_Hr;
	t.ti_min = CMOS_Min;
	t.ti_sec = CMOS_Sec;
	t.ti_hund = 0;
	settime( &t);
	PrevYr = CMOS_Yr;
	PrevMon = CMOS_Mon;
	PrevDay = CMOS_Day;
}

void SetRealTimeClock( const int hr, const int min, const int sec)
{
	int daylight = 0;

	Regs.h.ah = 3;
	/* hr, min, sec in BCD format */
	BCD = hr;
	Regs.h.ch = ENCODE_BCD;
	BCD = min;
	Regs.h.cl = ENCODE_BCD;
	BCD = sec;
	Regs.h.dh = ENCODE_BCD;
	Regs.h.dl = daylight;
	int86( TimeOfDayInterrupt, &Regs, &Regs);
}

void SetDateofRealTimeClock( const int century, const int year, const int mon, const int day)
{
	Regs.h.ah = 5;
	/* century (19 or 20), year, mon, day in BCD format */
	BCD = century;
	Regs.h.ch = ENCODE_BCD;
	BCD = year;
	Regs.h.cl = ENCODE_BCD;
	BCD = mon;
	Regs.h.dh = ENCODE_BCD;
	BCD = day;
	Regs.h.dl = ENCODE_BCD;
	int86( TimeOfDayInterrupt, &Regs, &Regs);
}

Flag ReadRealTimeClock( int* hr, int* min, int* sec, int* daylight)
{
	Regs.h.ah = 2;
	int86( TimeOfDayInterrupt, &Regs, &Regs);
	BCD = Regs.h.ch;
	*hr = DECODE_BCD;
	BCD = Regs.h.cl;
	*min = DECODE_BCD;
	BCD = Regs.h.dh;
	*sec = DECODE_BCD;
	*daylight = Regs.h.dl;
	/* carry flag set if RTC not running (inaccessible) */
	return !Regs.x.cflag;
}

Flag ReadDateofRealTimeClock( int* century, int* year, int* mon, int* day)
{
	Regs.h.ah = 4;
	int86( TimeOfDayInterrupt, &Regs, &Regs);
	BCD = Regs.h.ch;
	*century = DECODE_BCD;
	BCD = Regs.h.cl;
	*year = DECODE_BCD;
	BCD = Regs.h.dh;
	*mon = DECODE_BCD;
	BCD = Regs.h.dl;
	*day = DECODE_BCD;
	/* carry flag set if RTC not running (inaccessible) */
	return !Regs.x.cflag;
}

/* astronomical times... */

/* Sidereal time algorithm: at program start, determine sidereal time based on date, time and
longitude, and read bios clock tick count; then repeatedly read the new bios clock tick count,
calculate # of ticks since program start, calculate the sidereal time increment based on tick count,
and add to program start sidereal time to find the new sidereal time */

void InitTimes( const Flag DisplayOpeningMsgs, const double Tz, const int DST, double LongitudeDeg)
{
	long Yr;
	int M;
	int D;
	int h;
	int m;
	double s;

	Ticks = (long far*) TICK_ADDR;
	/* set century for Y2K problem with CMOS clock */
	getdate( &d);
	Century = d.da_year / 100;

	if( CMOS_RTC_Access)
		SetDOSToCMOS_RTC_f_ptr = SetDOSToCMOS_RTC_ViaBios;
	else
		SetDOSToCMOS_RTC_f_ptr = SetDOSToCMOS_RTC_ViaPort;

	SetDOSToCMOS_RTC_f_ptr();

	if( DisplayOpeningMsgs)
		DisplayCMOSTimeDate();

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
}

void SetStartBiosClockAndSidTime( const long Yr, const int M, const int D, const int h, const int m,
const double s, const double Tz, const int DST, double LongitudeDeg)
{
	/* set starting bios clock tick count */
	StartTicks = *Ticks;
	Days = 0;
	CalcJD( Yr, M, D, Tz, DST, h, m, s);
	CalcSidHr( Yr, M, D, Tz, DST, h, m, s, LongitudeDeg);
	StartSidT = SidT = SidHr*HrToRad;
}

int NewSidT( void)
{
	static long LastTicks;

	/* bios clock ticks... */
	if( *Ticks == LastTicks)
		return No;
	else
	{
		if( *Ticks < LastTicks)
			Days++;
		LastTicks = *Ticks;
		/* calc sidereal time based on bios clock ticks since program start */
		SidT = StartSidT + SidRate * (LastTicks + Days*ClockTicksDay - StartTicks) / RadToClockTick;

		while( SidT > OneRev)
			SidT -= OneRev;
		while( SidT < 0)
			SidT += OneRev;
		Current.SidT = SidT;
		return Yes;
	}
}

void CalcJD( const long Y, const int M, const int D, const double Tz, const int DST, const int h,
const int m, const double s)
{
	long A;
	double B;

	/* return 1st formula if Gregorian calendar, otherwise 2nd formula for Julian calendar: Sky and
	Telescope, August, 1991, pg 183 */
	if( (Y > 1582) || (Y == 1582 && M > 10) || (Y == 1582 && M == 10 && D > 15))
		A = 367*Y - 7*(Y + (M + 9)/12)/4 - 3*((Y + (M - 9)/7)/100 + 1)/4 + 275*M/9 + D + 1721029L;
	else
		A = 367*Y - 7*(Y + 5001 + (M - 9)/7)/4 + 275*M/9 + D + 1729777L;
	/* subtract 12hrs since JD starts at 12noon UT */
	B = (Tz - DST - 12 + h + m/60. + s/3600)/24;
	JD = A + B;
}

void CalcSidHr( const long Y, const int M, const int D, const double Tz, const int DST, const int h,
const int m, const double s, const double LongitudeDeg)
{
	/* JD at 0hrs UT (will be in form of a whole number + 0.5) */
	double JD0HrUT; 
	/* fractional part of JD beyond JD0HrUT */
	double FracDay;
	/* intermediate calculated result */
	double T; 
	/* sidereal time at 0hrs UT */
	double Sid0HrUT;

	CalcJD( Y, M, D, Tz, DST, h, m, s);

	FracDay = fmod( JD, 1);
	/* FracDay meas. from 0hr UT or < JD > .5 */
	if( FracDay > 0.5)
		FracDay -= 0.5;
	else
		FracDay += 0.5; 
	JD0HrUT = JD - FracDay; 

	/* Astronomical Formulae for Calculators, by Jean Meeus, pg 39 */
	T = (JD0HrUT - 2415020.)/36525.; 
	Sid0HrUT = 6.6460656 + 2400.051262*T + 0.00002581*T*T;
	SidHr = fmod( FracDay*SidRate*24. + Sid0HrUT - LongitudeDeg/15., 24); 
}

void GetHMSH( long TotalHundSec, struct HMSH* V)
{
	long HrHundSec;
	long MinHundSec;

	if( TotalHundSec < 0)
	{
		TotalHundSec = -TotalHundSec;
		V->Sign = Minus;
	}
	else
		V->Sign = Plus;

	V->Hr = (int) ( TotalHundSec/360000L);
	HrHundSec = (long) V->Hr*360000L;
	V->Min = (int) ((TotalHundSec - HrHundSec)/6000);
	MinHundSec = (long) V->Min*6000;
	V->Sec = (int) ((TotalHundSec - HrHundSec - MinHundSec)/100);
	V->HundSec = (int) (TotalHundSec - HrHundSec - MinHundSec - V->Sec*100);
}

void DisplayHMSH( const struct HMSH V)
{
	printf( "%2d:%02d:%02d.%02d", V.Hr, V.Min, V.Sec, V.HundSec);
}

void StrBufHMSH( const struct HMSH V)
{
	sprintf( StrBuf, "%2d:%02d:%02d.%02d", V.Hr, V.Min, V.Sec, V.HundSec);
}

void DisplayHMST( const struct HMSH V)
{
	printf( "%2d:%02d:%02d.%1d", V.Hr, V.Min, V.Sec, (V.HundSec + 5)/10);
}

void DisplayHMS( const struct HMSH V)
{
	printf( "%2d:%02d:%02d", V.Hr, V.Min, V.Sec);
}

void StrBufHMS( const struct HMSH V)
{
	sprintf( StrBuf, "%2d:%02d:%02d", V.Hr, V.Min, V.Sec);
}

void DisplaySHMS( const struct HMSH V)
{
	char Sign;

	if( V.Sign == Plus)
		Sign = '+';
	else
		Sign = '-';
	printf( "%c%2d:%02d:%02d", Sign, V.Hr, V.Min, V.Sec);
}

void StrBufSHMS( const struct HMSH V)
{
	char Sign;

	if( V.Sign == Plus)
		Sign = '+';
	else
		Sign = '-';
	sprintf( StrBuf, "%c%2d:%02d:%02d", Sign, V.Hr, V.Min, V.Sec);
}

void DisplaySHMSH( const struct HMSH V)
{
	char Sign;

	if( V.Sign == Plus)
		Sign = '+';
	else
		Sign = '-';
	printf( "%c%2d:%02d:%02d.%02d", Sign, V.Hr, V.Min, V.Sec, V.HundSec);
}

void StrBufSHMSH( const struct HMSH V)
{
	char Sign;

	if( V.Sign == Plus)
		Sign = '+';
	else
		Sign = '-';
	sprintf( StrBuf, "%c%2d:%02d:%02d.%02d", Sign, V.Hr, V.Min, V.Sec, V.HundSec);
}

void CalcRadFromHMSH( double* R, struct HMSH V)
{
	*R = (double) V.Hr*HrToRad + (double) V.Min*MinToRad +
	(double) V.Sec*SecToRad + (double) V.HundSec*HundSecToRad;

	if( V.Sign == Minus)
		*R = -*R;
}

void CalcRadFromDMS( double* R, struct DMS V)
{
	*R = (double) V.Deg*DegToRad + (double) V.Min*ArcminToRad +
	 (double) V.Sec*ArcsecToRad;
	if( V.Sign == Minus)
		*R = -*R; 
}

/*
void TestTimes( void)
{
	printf( "\n\n\nTest of astronomical times code, press any key to stop...\n");
	printf( "\nJulian Date %f   Sidereal Time ", JD);
	VidMemXY.X = wherex();
	VidMemXY.Y = wherey();
	while( !KeyStroke)
		if( NewSidT())
		{
			GetHMSH( RadToHundSec*SidT + 0.5, &SidTimeHMSH);
			gotoxy( VidMemXY.X, VidMemXY.Y);
			DisplayHMSH( SidTimeHMSH);
		}
	getch();

	if( CMOS_RTC_Access)
		printf( "\nCMOS RTC access via bios interrupt:");
	else
		printf( "\nCMOS RTC access via direct port calls:");

	printf( "\nTest of CMOS RTC access, press any key to stop...\n");
	VidMemXY.X = wherex();
	VidMemXY.Y = wherey();
	while( !KeyStroke)
	{
		SetDOSToCMOS_RTC_f_ptr();
		gotoxy( VidMemXY.X, VidMemXY.Y);
		DisplayCMOSTimeDate();
	}
	getch();
	NewLine;
	ContMsgRoutine();
}
*/

/* coordinates... */

void ValidRa( struct Position* Pos)
{
	while( Pos->Ra > OneRev)
		Pos->Ra -= OneRev;
	while( Pos->Ra < 0)
		Pos->Ra += OneRev;
}

void ValidHA( struct Position* Pos)
{
	while( Pos->HA > HalfRev)
		Pos->HA -= OneRev;
	while( Pos->HA < -HalfRev)
		Pos->HA += OneRev;
}

void ValidAz( struct Position* Pos)
{
	while( Pos->Az > OneRev)
		Pos->Az -= OneRev;
	while( Pos->Az < 0)
		Pos->Az += OneRev;
}

void SetCoordDeg( struct Position* Pos, const double RaDeg, const double DecDeg,
const double AltDeg, const double AzDeg, const double SidTDeg)
{
	Pos->Ra = RaDeg*DegToRad;
	Pos->Dec = DecDeg*DegToRad;
	Pos->Alt = AltDeg*DegToRad;
	Pos->Az = AzDeg*DegToRad;
	Pos->SidT = SidTDeg*DegToRad;
}

void ShowCoord( struct Position* Pos)
{
	printf( "\ncoordinates in degrees are:\n"); 
	printf( " Ra: %7.3f", Pos->Ra * RadToDeg);
	printf( "  Dec: %7.3f", Pos->Dec * RadToDeg);
	printf( "  Alt: %7.3f", Pos->Alt * RadToDeg);
	printf( "  Az: %7.3f", Pos->Az * RadToDeg);
	printf( "  SidT: %7.3f", Pos->SidT * RadToDeg);
}

void GetDMS( long TotalSec, struct DMS* V)
{
	if( TotalSec < 0)
	{
		TotalSec = -TotalSec;
		V->Sign = Minus;
	}
	else
		V->Sign = Plus;
	V->Deg = (int) (TotalSec/3600);
	V->Min = (int) ((TotalSec - (long) (V->Deg) * 3600)/60);
	V->Sec = (int) (TotalSec - V->Deg * 3600 - V->Min * 60);
}

void DisplayDMS( const struct DMS D)
{
	char Sign;

	if( D.Sign)
		Sign = '+';
	else
		Sign = '-';
	printf( "%c%2d:%02d:%02d", Sign, D.Deg, D.Min, D.Sec);
}

void StrBufDMS( const struct DMS D)
{
	char Sign;

	if( D.Sign)
		Sign = '+';
	else
		Sign = '-';
	sprintf( StrBuf, "%c%2d:%02d:%02d", Sign, D.Deg, D.Min, D.Sec);
}

void VidMemDeg( const double Deg)
{
	sprintf( StrBuf, "%7.3f", Deg * RadToDeg);
	WriteStrBufToScreen_f_ptr();
}

void VidMemRaHMS( struct Position* Pos)
{
	double R;

	if( Pos->Ra < 0)
		R = -.5;
	else
		R = .5;

	GetHMSH( RadToHundSec*Pos->Ra + R, &Pos->RaHMSH);
	StrBufHMS( Pos->RaHMSH);
	WriteStrBufToScreen_f_ptr();
}

void VidMemRaHMSH( struct Position* Pos)
{
	double R;

	if( Pos->Ra < 0)
		R = -.5;
	else
		R = .5;

	GetHMSH( RadToHundSec*Pos->Ra + R, &Pos->RaHMSH);
	StrBufHMSH( Pos->RaHMSH);
	WriteStrBufToScreen_f_ptr();
}

void VidMemHAHMS( struct Position* Pos)
{
	double R;

	if( Pos->HA < 0)
		R = -.5;
	else
		R = .5;

	GetHMSH( RadToHundSec*Pos->HA + R, &Pos->HAHMSH);
	StrBufSHMS( Pos->HAHMSH);
	WriteStrBufToScreen_f_ptr();
}

void VidMemHAHMSH( struct Position* Pos)
{
	double R;

	if( Pos->HA < 0)
		R = -.5;
	else
		R = .5;

	GetHMSH( RadToHundSec*Pos->HA + R, &Pos->HAHMSH);
	StrBufSHMSH( Pos->HAHMSH);
	WriteStrBufToScreen_f_ptr();
}

void VidMemDecDMS( struct Position* Pos)
{
	double R;

	if( Pos->Dec < 0)
		R = -.5;
	else
		R = .5;

	GetDMS( RadToArcsec*Pos->Dec + R, &Pos->DecDMS);
	StrBufDMS( Pos->DecDMS);
	WriteStrBufToScreen_f_ptr();
}

void VidMemSidT( struct Position* Pos)
{
	double R;

	if( Pos->SidT < 0)
		R = -.5;
	else
		R = .5;

	GetHMSH( RadToHundSec*Pos->SidT + R, &Pos->SidTimeHMSH);
	StrBufHMS( Pos->SidTimeHMSH);
	WriteStrBufToScreen_f_ptr();
}

/* parallel port */

/* Normally, a PC can have up to 3 parallel printer ports - LPT1, LPT2 & LPT3.
Their (16-bit) base addresses in the processor's I/O space are loaded into memory when the machine
is booted, starting at address 408 Hex.

A well-written program requiring direct access to any of these ports should endeavour to find out
which are present and where at run-time (rather than use constant declarations)

Due to 80x86 processors being " little-endian", the addresses are stored as follows:

408 409  40a 40b  40c 40d
LPT1     LPT2     LPT3
low/high low/high low/high
byte     byte     byte

For example, the following values -

408 409 40a 40b 40c 40d
78  03  78  02  00  00

mean that LPT1 is at 378 Hex, LPT2 is at 278 Hex, and LPT3 is not present.

8 bits out (data port): base address portid lpt1 = 0x378 (decimal 888), lpt2 = 0x278 (decimal 632),
monochrome video card = 0x3BC (decimal 956);

25 - pin connector pins 2 (least significant) through 9 (most significant),
logical low in produces logical low out;

5 bits in (status port): base address + 1, portid 0x379 (or try 0x279 or 0x3BD):

	8		16		32		64		128	bit
	H		H		H		H		H		in
	15		13		12		10		11		25 pin connector pin #
	L		H		H		H		L		result, eg, inverted for bit 128;

	module logic rectifies the inverted bit;

4 bits in or out (control port): base address + 2, portid 0x37A (or try 0x27A or 0x3BE):

	1       2       4       8    		bit
	H       H       H       H        in or out
	1       14      16      17       25 pin connector pin #
	L       L       H       L        result, eg, inverted for all bits except for bit 4;

	module logic rectifies the inverted bits;

more info on the control port (4 bits I/O):
bit 16 = irq enable, 32 = bi-dir enable for data port;
these pins have two states, 0V and open collector or high impedence; to read, it may be necessary
to put the pins in open collector mode by outputting xxxx0100 (turn on all pins, respecting the
inversions), now the input on the pins if pulled low can be read;

putting I/O port into high impedance, all pins respond to grounding, otherwise,
all input pins respond to +5VDC except for pin 15, only pin 15 responds to grounding;

remaining pin connections: 25 pin connector pins 18 through 25 are grounds; */

void InitPPort( void)
{
	int HoldPPortAddr = PPortAddr;

	/* if PPortAddr <=3, treat it as a lpt num */
	if( PPortAddr > 0 && PPortAddr <= 3)
		if( !(PPortAddr = GetPPortAddr( PPortAddr)))
		{
			sprintf( StrBuf, "Bad lpt:%01d", HoldPPortAddr);
			BadExit( StrBuf);
		}
	PPortAddrOutByte = PPortAddr;
	PPortAddrInNibble = PPortAddr + 1;
	PPortAddrInOutNibble = PPortAddr + 2;
	/* make the 8 output parallel port pins logical low */
	OutValue = Off;
	if( InvertOutput)
		OutValue = ~OutValue;
	OutByte();
	BiDirOutNibbleValue = 0;
	BiDirOutNibble();
}

void ClosePPort( void)
{
	OutValue = Off;
	if( InvertOutput)
		OutValue = ~OutValue;
	OutByte();
}

unsigned GetPPortAddr( int lptnum)
{
	/* make a far pointer with segment 0x40 and offset 0x008+, then return its value */
	return *(unsigned far *)(MK_FP( 0x40, 0x008 + (lptnum-1)*2));
}

void DisplayLpts (void)
{
	int x;

	printf( "\n\n\ndisplaying lpt ports:");
	for( x=1; x<=3; x++)
		printf( "\nlpt%1d: 0x%04x hex or %4d decimal", x, GetPPortAddr( x), GetPPortAddr( x));
	printf( "\n\n");
}

void OutByte( void)
{
	outportb( PPortAddrOutByte, OutValue);
}

void BiDirOutNibble( void)
{
	/* 4 bits out: parallel port lines 1, 14, 16, 17 */
	outportb( PPortAddrInOutNibble, (BiDirOutNibbleValue &15) ^11);
}

Byte InNibble4Bit( void)
{
	/* 4 bits in: parallel port lines 13, 12, 10, 11 using bits 16, 32, 64, 128 */
	return (inportb( PPortAddrInNibble) &240) ^128;
}

Byte InNibble5Bit( void)
{
	/* 5 bits in: parallel port lines 15, 13, 12, 10, 11 using bits 8, 16, 32, 64, 128 */
	return (inportb( PPortAddrInNibble) &248) ^136;
}

Byte BiDirInNibble( void)
{
	/* 4 bits in: parallel port lines 1, 14, 16, 17 */
	return (inportb( PPortAddrInOutNibble) &15) ^11;
}

void TestPPort( void)
{
	int Input;
	int x, y;

	printf( "\n\n\Test of parallel port:");
	DisplayLpts();
	printf( "\nusing parallel port address of %d\n", PPortAddr);
	printf( "\ntest of 8 bit output port (parallel port pins 2-9):\n");
	printf( "'h' for logical high, 'l' for logical low, 'q' to quit.\n");
	Input = ' ';
	while( Input != 'q')
	{
		Input = getch();
		if( Input == 'h' || Input == 'H')
		{
			printf( "outputting logical high...\n");
			OutValue = PortOn;
			OutByte();
		}
		else if( Input == 'l' || Input == 'L')
		{
			printf( "outputting logical low...\n");
			OutValue = PortOff;
			OutByte();
		}
	}
	OutValue = PortOff;
	OutByte();

	printf( "\ntest of 4 bit I-O port (parallel port pins 1, 14, 16, 17):\n");
	printf( "'h' for logical high, 'l' for logical low, 'q' to quit.\n");
	Input = ' ';
	while( Input != 'q')
	{
		Input = getch();
		if( Input == 'h' || Input == 'H')
		{
			printf( "outputting logical high...\n");
			BiDirOutNibbleValue = PortOn;
			BiDirOutNibble();
		}
		else if( Input == 'l' || Input == 'L')
		{
			printf( "outputting logical low...\n");
			BiDirOutNibbleValue = PortOff;
			BiDirOutNibble();
		}
	}
	BiDirOutNibbleValue = PortOff;
	BiDirOutNibble();

	printf( "\ntest of parallel input pins (press any key to quit):\n\n");
	x = wherex();
	y = wherey();
	while( !KeyStroke)
	{
		gotoxy( x, y);
		InNibbleValue = InNibble5Bit();
		BiDirInNibbleValue = BiDirInNibble();
		printf( "1:%c ", BiDirInNibbleValue&1? 'l':'H');
		printf( "10:%c ", InNibbleValue&64? 'H':'l');
		printf( "11:%c ", InNibbleValue&128? 'l':'H');
		printf( "12:%c ", InNibbleValue&32? 'H':'l');
		printf( "13:%c ", InNibbleValue&16? 'H':'l');
		printf( "14:%c ", BiDirInNibbleValue&2? 'l':'H');
		printf( "15:%c ", InNibbleValue&8? 'l':'H');
		printf( "16:%c ", BiDirInNibbleValue&4? 'H':'l');
		printf( "17:%c", BiDirInNibbleValue&8? 'l':'H');
	}
	getch();
	NewLine;
	ContMsgRoutine();
}

/* mouse functions */

void InitMouse( void)
{
	ResetMouse();
}

void CallMouse( void)
{
	union REGS inregs, outregs;

	inregs.x.ax = inax;
	inregs.x.bx = inbx;
	inregs.x.cx = incx;
	inregs.x.dx = indx;

	int86( MOUSE_INTERRUPT, &inregs, &outregs);

	outax = outregs.x.ax;
	outbx = outregs.x.bx;
	outcx = outregs.x.cx;
	outdx = outregs.x.dx;
}

void GetDefaultMouseSensitivity( void)
{
	inax = GET_MOUSE_SENSITIVITY;
	CallMouse();

	HorizMickeySensitivity = outbx;
	VertMickeySensitivity = outcx;
	ThresholdDblSpeed = outdx;
}

void DecodeMousePosition( void)
{
	MouseXGraph = outcx;
	MouseYGraph = outdx;
	MouseXText = MouseXGraph/8 + 1;
	MouseYText = MouseYGraph/8 + 1;
}

/* resets mouse to default values:
mouse is positioned to screen center
mouse cursor is reset and hidden
no interrupts are enabled (mask = 0)
double speed threshold set to 64 mickeys per second
horizontal mickey to pixel ratio (8 to 8)
vertical mickey to pixel ratio (16 to 8)
max width and height are set to maximum for video mode */
void ResetMouse( void)
{
	inax = RESET_MOUSE;
	CallMouse();
	if(outax != SUCCESSFULL_MOUSE_RESET)
		BadExit( "could not find mouse driver");
}

void LowMouseSensitivity( void)
{
	GetDefaultMouseSensitivity();

	inax = SET_MOUSE_SENSITIVITY;
	inbx = 4;
	incx = 4;
	indx = 64;
	CallMouse();
}

void ResetMouseSensitivity( void)
{
	inax = SET_MOUSE_SENSITIVITY;
	inbx = HorizMickeySensitivity;
	incx = VertMickeySensitivity;
	indx = ThresholdDblSpeed;
	CallMouse();
}

void DisplayMouse( void)
{
	inax = DISPLAY_MOUSE;
	CallMouse();
}

void HideMouse( void)
{
	inax = HIDE_MOUSE;
	CallMouse();
}

void GetMousePosition( void)
{
	inax = GET_MOUSE_POSITION;
	CallMouse();
	DecodeMousePosition();
	MouseLeftButtonDown = outbx & 1;
	MouseRightButtonDown = outbx & 2;
}

void GetMousePositionRelative( void)
{
	inax = GET_MOUSE_POSITION_RELATIVE;
	CallMouse();
	MouseXMickeyRelative = outcx;
	MouseYMickeyRelative = outdx;
}

void SetMousePosition(int Horiz, int Vert)
{
	inax = SET_MOUSE_POSITION;
	incx = Horiz;
	indx = Vert;
	CallMouse();
}

void CenterMouseCursor( void)
{
	SetMousePosition( MidMouseXGraph, MidMouseYGraph);
}

int MouseLeftButtonPressCount( void)
{
	inax = GET_MOUSE_PRESS_INFO;
	inbx = LEFT_BUTTON;
	CallMouse();
	if( outbx)
		/* get position at last press */
		DecodeMousePosition();
	MouseLeftButtonDown = outax & 1;
	MouseRightButtonDown = outax & 2;
	return outbx;
}

int MouseRightButtonPressCount( void)
{
	inax = GET_MOUSE_PRESS_INFO;
	inbx = RIGHT_BUTTON;
	CallMouse();
	if( outbx)
		/* get position at last press */
		DecodeMousePosition();
	MouseLeftButtonDown = outax & 1;
	MouseRightButtonDown = outax & 2;
	return outbx;
}

int MouseLeftButtonReleaseCount( void)
{
	inax = GET_MOUSE_RELEASE_INFO;
	inbx = LEFT_BUTTON;
	CallMouse();
	if( outbx)
		/* get position at last press */
		DecodeMousePosition();
	MouseLeftButtonDown = outax & 1;
	MouseRightButtonDown = outax & 2;
	return outbx;
}

int MouseRightButtonReleaseCount( void)
{
	inax = GET_MOUSE_RELEASE_INFO;
	inbx = RIGHT_BUTTON;
	CallMouse();
	if( outbx)
		/* get position at last press */
		DecodeMousePosition();
	MouseLeftButtonDown = outax & 1;
	MouseRightButtonDown = outax & 2;
	return outbx;
}

/*
void TestMouse( void)
{
	clrscr();
	printf( "Test of mouse module: press any key to exit...\n");
	while( !KeyStroke)
	{
		DisplayMouse();
		if( MouseLeftButtonPressCount())
			printf( "\nleft button was pressed...");
		else if( MouseRightButtonPressCount())
			printf( "\nright button was pressed...");
		else
		{
			GetMousePositionRelative();
			printf( "\nrelative mouse position: %d %d", MouseXMickeyRelative,
			MouseYMickeyRelative);
			delay( 1000);
		}
	}
	getch();
}
*/
