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

void GetDir( void)
{
	Flag ValidName = False;

	FoundFile = True;
	while( FoundFile && !ValidName)
	{
		if( FindFirst)
		{
			FindFirst = False;
			/* returns 0 if successful */
			FoundFile = !findfirst( "*.*", &Ffblk, FA_DIREC);
		}
		else
			FoundFile = !findnext( &Ffblk);
		if( FoundFile)
			if( Ffblk.ff_attrib == FA_DIREC)
				ValidName = True;
	}
}

void GetFileName( char* Name)
{
	Flag ValidName = False;

	FoundFile = True;
	while( FoundFile && !ValidName)
	{
		if( FindFirst)
		{
			FindFirst = False;
			/* returns 0 if successful */
			FoundFile = !findfirst( Name, &Ffblk, 0);
		}
		else
			FoundFile = !findnext( &Ffblk);
		if( FoundFile && !IsProgramFile( Ffblk.ff_name))
			ValidName = True;
	}
}

Flag IsProgramFile( char* Name)
{
	if( strcmpi( AnalysisFile, Name)==0)
		return True;
	if( strcmpi( ConfigFile, Name)==0)
		return True;
	if( strcmpi( OutGuideFile, Name)==0)
		return True;
	if( strcmpi( RecordEquatFile, Name)==0)
		return True;
	if( strcmpi( SlewFile, Name)==0)
		return True;
	if( strcmpi( SlewOutFile, Name)==0)
		return True;

	return False;
}

/* copies into Filename the selected file name, found by matching to 'Name' */
Flag SelectDataFilename( char* Name)
{
	struct FilenameString N[MaxNameArray];
	int Left, Top;
	int Menu;
	int HighlightSelect, LastHighlightSelect;
	int ColumnSpacing;
	int IxB;

	LoadFnames( Name);
	SortLinkFname();

	ValidResponseFlag = No;
	CurrentLinkFname = FirstLinkFname;
	if( CurrentLinkFname != NULL)
		while( True)
		{
			Menu = 'a';
			WriteWindow( DataFileFrame);
			Left = DataFileFrame.Left + 2;
			Top = DataFileFrame.Top + 2;
			ColumnSpacing = (DataFileFrame.Right - DataFileFrame.Left)/3 - 1;
			while( CurrentLinkFname != NULL && (Menu-'a') < MaxNameArray)
			{
				gotoxy( Left, Top);
				if( CurrentLinkFname->FNameType == NameIsADirectory)
				{
					printf( "%c. [%s]\n", Menu, CurrentLinkFname->Name);
					N[Menu-'a'].FNameType = NameIsADirectory;
				}
				else
				{
					printf( "%c. %s\n", Menu, CurrentLinkFname->Name);
					N[Menu-'a'].FNameType = NameIsAFile;
				}
				strcpy( N[Menu-'a'].Name, CurrentLinkFname->Name);
				Menu++;
				/* print next on line below unless time for new column */
				if( (Menu-'a') % 8)
					Top++;
				else
				{
					Top -= 7;
					Left += ColumnSpacing;
				}
				CurrentLinkFname = CurrentLinkFname->NextLinkFname;
			}
			gotoxy( DataFileFrame.Left + 4, DataFileFrame.Bottom - 3);
			printf( "select a-%c or any other key to escape", Menu-1);
			gotoxy( DataFileFrame.Left + 4, DataFileFrame.Bottom - 2);
			printf( "'-' to go back a page");
			if( CurrentLinkFname != NULL)
			{
				gotoxy( DataFileFrame.Left + 4, DataFileFrame.Bottom - 1);
				printf( "<space bar> for more");
			}
			HighlightSelect = LastHighlightSelect = 0;
			ObjectNameTextAttr( DataFileFrame, ColumnSpacing, HighlightSelect, SelectText);
			/* get response: either keyboard hit or mouse click */
			while( True)
			{
				if( HighlightSelect != LastHighlightSelect)
				{
					ObjectNameTextAttr( DataFileFrame, ColumnSpacing, LastHighlightSelect, DefaultText);
					ObjectNameTextAttr( DataFileFrame, ColumnSpacing, HighlightSelect, SelectText);
					LastHighlightSelect = HighlightSelect;
				}
				if( KeyStroke)
				{
					Response = getch();
					if( Response == Return)
					{
						Response = HighlightSelect + 'a';
						break;
					}
					else
						if( Response == ExtendedKeyboardStroke)
						{
							Response = getch();
							if( Response == UpCursor)
							{
								HighlightSelect--;
								if( HighlightSelect < 0)
								HighlightSelect = Menu - 'a' - 1;
							}
							else
								if( Response == DownCursor)
								{
									HighlightSelect++;
									if( HighlightSelect >= Menu - 'a')
										HighlightSelect = 0;
								}
						}
						else
						{
							Response = tolower( Response);
							break;
						}
				}
				if( UseMouseFlag && MouseLeftButtonPressCount())
				{
					if( MouseXText > DataFileFrame.Left+2 && MouseXText < DataFileFrame.Right-2)
					{
						/* 8 rows go from +2 to +9 */
						if( MouseYText >= DataFileFrame.Top+2 && MouseYText <= DataFileFrame.Top+9)
						{
							/* get the row */
							Response = MouseYText - (DataFileFrame.Top+2);
							/* get the column: 8 rows spaced 21 chars apart */
							Response += 8 * ((MouseXText - (DataFileFrame.Left+2)) / 21);
							Response += 'a';
						}
						else
							if( MouseYText == DataFileFrame.Bottom-1)
								Response = ' ';
							else
								if( MouseYText == DataFileFrame.Bottom-2)
									Response = '-';
								else
									Response = 0;
						break;
					}
					else
						Response = 0;
					break;
				}
			}
			RemoveWindow( DataFileFrame);
			/* exit loop if valid response */
			if( Response >= 'a' && Response < Menu)
			{
				ValidResponseFlag = Yes;
				Ix = Response - (int) 'a';
				break;
			}
			else
				/* go back a page if '-' key pressed */
				if( Response == '-')
				{
					/* go back to start of displayed names, plus another page back */
					for( IxB = 0; IxB < MaxNameArray*2 && CurrentLinkFname->PrevLinkFname != NULL; IxB++)
						CurrentLinkFname = CurrentLinkFname->PrevLinkFname;
					Menu = 'a';
				}
				else
					/* read in more if space bar was pressed */
					if( CurrentLinkFname != NULL && Response == ' ')
						Menu = 'a';
					else
						break;
		}
	else
		PressKeyToContMsg( "No data files found");
	FreeAllLinkFname();
	if( ValidResponseFlag)
		if( N[Response-'a'].FNameType == NameIsAFile)
		{
			strcpy( Filename, N[Response-'a'].Name);
			return True;
		}
		else
		{
			chdir( N[Response-'a'].Name);
			/* if a directory selected, repeatedly call SelectDataFilename(): return the last called
			function value as the stack is popped so that MenuCoordFile() can handle the result */
			return SelectDataFilename( Name);
		}
	else
	{
		PressKeyToContMsg( "No data file selected");
		return False;
	}
}

void MenuCoordFile( void)
{
	struct Object O[MaxCoord];
	int Left, Top;
	int Ix;
	char Menu;
	int Count;
	char LookAheadChar = ' ';
	Flag LookAheadCharRead = No;
	int HighlightSelect, LastHighlightSelect;

	FPosPtr = (fpos_t*) malloc( MAX_FPOS_PTR * sizeof( fpos_t));
	if( FPosPtr == NULL)
		BadExit( "Problem with malloc of FPosPtr in MenuCoordFile()");

	/* change to last data file directory */
	chdir( DataDir);
	/* this function copies a selected file name into Filename, or changes directories, then copies
	the file name */
	if( DataFileNameSet || SelectDataFilename( DataFile))
	{
		Input = fopen( Filename, "r");
		if( Input == NULL)
			BadExit( strcat( "Could not open ", Filename));

		GrandTourLoaded = Yes;
		GrandTourRecNum = 0;
		strcpy( GrandTourFilename, Filename);
		if( !feof( Input))
		{
			FPosPtrIx = 0;
			fgetpos( Input, &FPosPtr[FPosPtrIx++]);

			/* while there are objects in the input file */
			while( FillObjects( O, &Count))
			{
				/* Restore char read while determining eof on previous screen? */
				/* each line in the data files starts with a blank, so char will always be ' ' so no
				need to restore char at this time */
				if( LookAheadCharRead) { }
				/* save screen & draw border */
				WriteWindow( CoordFrame);
				/* write objects and instructions */
				Left = CoordFrame.Left + 1;
				Top = CoordFrame.Top + 2;
				gotoxy( Left, Top);
				Menu = 'a';
				for( Ix = 1; Ix <= Count; Ix++)
				{
					gotoxy( Left, Top);
					printf( "%c. %s ", Menu++, O[Ix-1].Name);
					if( Ix%8)
						Top++;
					else
					{
						Top -= 7;
						Left += ObjectNameSize+2;
					}
				}
				gotoxy( CoordFrame.Left + 13, CoordFrame.Bottom - 3);
				printf( "select a-%c or any other key to escape", (char) ('a' + Count - 1));
				gotoxy( CoordFrame.Left + 13, CoordFrame.Bottom - 2);
				printf( "'-' to go back a page");
				/* look ahead to see if file is finished */
				FReadChar( Input, &LookAheadChar);
				LookAheadCharRead = Yes;
				if( !feof( Input))
				{
					gotoxy( CoordFrame.Left + 13, CoordFrame.Bottom - 1);
					printf( "<space bar> for more");
				}
				HighlightSelect = LastHighlightSelect = 0;
				ObjectNameTextAttr( CoordFrame, ObjectNameSize+2, HighlightSelect, SelectText);
				/* get response: either keyboard hit or mouse click */
				while( True)
				{
					if( HighlightSelect != LastHighlightSelect)
					{
						ObjectNameTextAttr( CoordFrame, ObjectNameSize+2, LastHighlightSelect, DefaultText);
						ObjectNameTextAttr( CoordFrame, ObjectNameSize+2, HighlightSelect, SelectText);
						LastHighlightSelect = HighlightSelect;
					}
					if( KeyStroke)
					{
						Response = getch();
						if( Response == Return)
						{
							Response = HighlightSelect + 'a';
							break;
						}
						else
							if( Response == ExtendedKeyboardStroke)
							{
								Response = getch();
								if( Response == UpCursor)
								{
									HighlightSelect--;
									if( HighlightSelect < 0)
										HighlightSelect = Count - 1;
								}
								else
									if( Response == DownCursor)
									{
										HighlightSelect++;
										if( HighlightSelect >= Count)
											HighlightSelect = 0;
									}
							}
							else
							{
								Response = tolower( Response);
								break;
							}
					}
					if( UseMouseFlag && MouseLeftButtonPressCount())
					{
						if( MouseXText > CoordFrame.Left+1 && MouseXText < CoordFrame.Right-1)
						{
							/* 8 rows go from +2 to +9 */
							if( MouseYText >= CoordFrame.Top+2 && MouseYText <= CoordFrame.Top+9)
							{
								/* get the row */
								Response = MouseYText - (CoordFrame.Top+2);
								/* get the column: 8 rows spaced ObjectNameSize+2 chars apart */
								Response += 8 * ((MouseXText - (CoordFrame.Left+2)) / (ObjectNameSize+2));
								Response += 'a';
							}
							else
								if( MouseYText == CoordFrame.Bottom-1)
									Response = ' ';
								else
									if( MouseYText == CoordFrame.Bottom-2)
										Response = '-';
									else
										Response = 0;
							break;
						}
						else
							Response = 0;
						break;
					}
				}
				/* restore screen */
				RemoveWindow( CoordFrame);
				/* exit loop if valid response */
				ValidResponseFlag = No;
				if( Response >= 'a' && Response < ('a' + Count))
				{
					ValidResponseFlag = Yes;
					Ix = Response - (int) 'a';
					break;
				}
				/* go back a page if '-' key pressed */
				if( Response == '-')
				{
					FPosPtrIx -= 2;
					if( FPosPtrIx < 0)
						FPosPtrIx = 0;
					fsetpos( Input, &FPosPtr[FPosPtrIx]);
					FPosPtrIx++;
				}
				/* read in more if space bar was pressed */
				else
					if( !feof( Input) && Response == ' ')
						fgetpos( Input, &FPosPtr[FPosPtrIx++]);
					else
						break;
			}
			/* if valid input, process input */
			if( ValidResponseFlag)
			{
				In.RaHMSH = O[Ix].RaHMSH;
				In.DecDMS = O[Ix].DecDMS;
				CalcRadFromHMSH( &In.Ra, In.RaHMSH);
				CalcRadFromDMS( &In.Dec, In.DecDMS);
				DisplayIn( Filename, O[Ix].Name);
				LastDataObject = In;
			}
			else
			{
				WriteWindow( MsgFrame);
				gotoxy( MsgFrame.Left + 5, MsgFrame.Top + 2);
				printf( "No object selected");
				gotoxy( MsgFrame.Left + 5, MsgFrame.Top + 4);
				ContMsgRoutine();
				RemoveWindow( MsgFrame);
				DisplayIn( Filename, NameBlanks);
			}
		}
		fclose( Input);
	}
	/* save data file directory */
	GetCurDir( DataDir);
	/* return to default directory */
	chdir( DefaultDir);
	free( FPosPtr);
}

void ObjectNameTextAttr( const struct Frame F, const int Length, const int Select, const int TextAttr)
{
	int StartX, EndX;

	VidMemXY.Y = Select%8 + F.Top+1;
	StartX = F.Left + Length*(Select/8);
	EndX = StartX + Length;
	for( VidMemXY.X = StartX; VidMemXY.X < EndX;	VidMemXY.X++)
		Screen[VidMemXY.Y][VidMemXY.X].Attr = TextAttr;
}

void ReadRa( FILE* File, struct HMSH* V)
{
	char c = ' ';

	/* check for -0 hr */
	while( !feof( Input) && c != '+' && c != '-' && c != '.' && !(c >= '0' && c <= '9'))
		c = fgetc( File);
	ungetc( c, File);

	fscanf( File, "%d", &V->Hr);
	fscanf( File, "%d", &V->Min);
	fscanf( File, "%d", &V->Sec);

	if( c == '-' || V->Hr < 0 || V->Min < 0 || V->Sec < 0)
		V->Sign = Minus;
	else
		V->Sign = Plus;

	if( V->Hr < 0)
		V->Hr = -V->Hr;
	if( V->Min < 0)
		V->Min = -V->Min;
	if( V->Sec < 0)
		V->Sec = -V->Sec;

	V->HundSec = 0;
}

void ReadDec( FILE* File, struct DMS* V)
{
	char c = ' ';

	/* check for -0 deg */
	while( c != '+' && c != '-' && c != '.' && !(c >= '0' && c <= '9'))
		c = fgetc( File);
	ungetc( c, File);

	fscanf( File, "%d", &V->Deg);
	fscanf( File, "%d", &V->Min);
	fscanf( File, "%d", &V->Sec);

	if( c == '-' || V->Deg < 0 || V->Min < 0 || V->Sec < 0)
		V->Sign = Minus;
	else
		V->Sign = Plus;

	if( V->Deg < 0)
		V->Deg = -V->Deg;
	if( V->Min < 0)
		V->Min = -V->Min;
	if( V->Sec < 0)
		V->Sec = -V->Sec;
}

/* ability to page down through lists of unlimited size courtesy Dale Eason */
/* used by MenuCoordFile() to fill window until out of data from file */
Flag FillObjects( struct Object O[], int* Count)
{
	int Ix = 0;

	while( !feof( Input) && Ix < MaxCoord)
	{
		ReadRa( Input, &O[Ix].RaHMSH);
		if( !feof( Input))
			ReadDec( Input, &O[Ix].DecDMS);
		if( !feof( Input))
			if( FReadStringToCharCountOrNewLine( Input, O[Ix].Name, sizeof( O->Name)))
				;
			else
				FReadToNewLine( Input);
		Ix++;
		*Count = Ix;
	}
	if( feof( Input))
		(*Count)--;
	for( ; Ix < MaxCoord; Ix++)
	{
		strcpy( O[Ix].Name, " ");
		O[Ix].RaHMSH.Hr = O[Ix].RaHMSH.Min = O[Ix].RaHMSH.Sec = 0;
		O[Ix].RaHMSH.HundSec = 0;
		O[Ix].DecDMS.Deg = O[Ix].DecDMS.Min = O[Ix].DecDMS.Sec = 0;
		O[Ix].RaHMSH.Sign = O[Ix].DecDMS.Sign = Plus;
	}
	if( *Count)
		return True;
	else
		return False;
}

void ProcessMenuRestoreLastObject( void)
{
	In = LastDataObject;
	DisplayIn( "last data object", NameBlanks);
	ProcessHPEvents();
}

Flag ProcessMenuDataFileClosest( Flag FilenameSetFlag, Flag HowToMatchFlag)
{
	struct Position P, BestP;
	char Name[ObjectNameSize], BestName[ObjectNameSize];
	double Sep;
	double BestSep = MAXDOUBLE;
	Flag CheckBestFlag;
	Flag BestFoundFlag = False;

	/* either Filename is already set, or call a function where user selects a Filename from a file
   name pattern */
	if( FilenameSetFlag || SelectDataFilename( DataFile))
	{
		Input = fopen( Filename, "r");
		if( Input == NULL)
			BadExit( strcat( "Could not open ", Filename));

		P.SidT = Current.SidT;
		while( !feof( Input))
		{
			if( !feof( Input))
				ReadRa( Input, &P.RaHMSH);
			if( !feof( Input))
				ReadDec( Input, &P.DecDMS);
			if( !feof( Input))
			{
				if( FReadStringToCharCountOrNewLine( Input, Name, sizeof( Name)))
					;
				else
					FReadToNewLine( Input);
				CalcRadFromHMSH( &P.Ra, P.RaHMSH);
				CalcRadFromDMS( &P.Dec, P.DecDMS);
				Sep = CalcEquatAngularSep( &Current, &P);
				if( Sep < BestSep)
				{
					switch( HowToMatchFlag)
					{
						case ExactInputFieldsMatchOK:
							CheckBestFlag = Yes;
							break;
						case AvoidMatchInputFields:
							if( P.Ra!=In.Ra || P.Dec!=In.Dec)
								CheckBestFlag = Yes;
							else
								CheckBestFlag = No;
							break;
						case DoNotMatchInputFile:
							CheckBestFlag = Yes;
							RaDecInit = FirstRaDecInit;
							while( RaDecInit != NULL && CheckBestFlag)
							{
								if( P.Ra==RaDecInit->Ra && P.Dec==RaDecInit->Dec)
									CheckBestFlag = No;
								RaDecInit = RaDecInit->NextRaDecInit;
							}
					}
					if( CheckBestFlag)
					{
						BestFoundFlag = True;
						BestSep = Sep;
						BestP = P;
						strcpy( BestName, Name);
					}
				}
			}
		}
		if( BestFoundFlag)
		{
			In.RaHMSH = BestP.RaHMSH;
			In.DecDMS = BestP.DecDMS;
			CalcRadFromHMSH( &In.Ra, In.RaHMSH);
			CalcRadFromDMS( &In.Dec, In.DecDMS);
			DisplayIn( Filename, BestName);
			LastDataObject = In;
		}
		fclose( Input);
	}
	if( HowToMatchFlag == DoNotMatchInputFile)
		FreeAllRaDecInit();

	return BestFoundFlag;
}

/* remove spaces from the end of a string generated with Gets() (in common.c) */
void RemoveBlanks( char* s)
{
	int i = strlen( s);

	while( isspace( s[--i]))
	{
		if (i < 0)
			break;
		s[i]='\0';
	}
}

/* attempts to find string str in file fname */
Flag FindStr( const char* str, char* fname, int* Rec)
{
	int c;
	int Left, Top;

	*Rec = 0;

	if( IsProgramFile( Ffblk.ff_name))
		return False;

	if( (Input = fopen( fname, "r")) != NULL)
	{
		while( fgets( StrBuf2, sizeof( StrBuf2), Input) != NULL)
		{
			(*Rec)++;
			if (strstr( strlwr( StrBuf2), str) != NULL)
			{
				Left = MsgFrame.Left + 2;
				Top = MsgFrame.Top + 4;
				gotoxy( Left, Top);
				for( Ix = 0; Ix < 50; Ix++)
					printf( " ");
				gotoxy( Left, Top);
				printf( "Found %s", StrBuf2);
				gotoxy( Left, Top + 1);
				for( Ix = 0; Ix < 30; Ix++)
					printf( " ");
				gotoxy( Left, Top + 1);
				printf( "in %s, rec #%d ", fname, *Rec);
				gotoxy( Left, Top + 3);
				printf( "'y' to select, esc to terminate, any key to continue");
				c = getch();
				switch( c)
				{
					case 'Y': case 'y':
						return (int) c;
					case Esc:
						fclose( Input);
						return Esc;
				}
			}
		}
	}
	fclose( Input);
	return False;
}

Flag FindFile( const char* Path, const char* Search, char* File, int* Rec)
{
	char path[MAXPATH];
	char drive[MAXDRIVE];
	char dir[MAXDIR];
	char file[MAXFILE];
	char ext[MAXEXT];
	char xdr[MAXDRIVE];
	char xdi[MAXDIR];
	int done;
	int FindStrResults;

	fnsplit( Path, drive, dir, file, ext);
	if( strlen( file) == 0)
		strcpy( file, "*");
	if( stricmp( ext, ".dat") != 0)
		strcpy( ext,".dat");
	fnmerge( path, drive, dir, file, ext);

	done = findfirst( path, &Ffblk, 0);
	while( !done)
	{
		fnsplit( Ffblk.ff_name, xdr, xdi, file, ext);
		fnmerge( path, drive, dir, file, "dat");
		FindStrResults = FindStr( Search, path, Rec);
		switch( FindStrResults)
		{
			case 'Y': case 'y':
				fnmerge( File, "", "", file, ext);
				return True;
			case Esc:
				return False;
			default:
				done = findnext( &Ffblk);
				break;
		}
	}
	return False;
}

Flag ProcessMenuSearch( void)
{
	int Left, Top;
	static char Path[26] = "*.dat";
	char InputPath[26];
	char Search[30];
	char InputSearch[30];
	Flag UserResponseFlag;
	Flag ResultFlag = False;
	int Rec;
	char Name[ObjectNameSize];

	WriteWindow( MsgFrame);
	Left = MsgFrame.Left + 2;
	Top = MsgFrame.Top + 2;
	gotoxy( Left, Top);
	printf( "Data file path (%s): ", Path);
	UserResponseFlag = Gets( InputPath, 25);
	if( UserResponseFlag != UserEscaped)
	{
		RemoveBlanks( InputPath);
		if( UserResponseFlag && strlen( InputPath) > 0)
			strcpy( Path, InputPath);

		gotoxy( Left, Top + 1);
		printf( "Search string: ");
		UserResponseFlag = Gets( InputSearch, 29);
		if( UserResponseFlag != UserEscaped)
		{
			strcpy( Search, InputSearch);
			RemoveBlanks( Search);
			if( FindFile( Path, strlwr( Search), Filename, &Rec))
			{
				rewind( Input);
				for( Ix = 0; Ix < Rec-1; Ix++)
					FReadToNewLine( Input);
				if( !feof( Input))
					ReadRa( Input, &In.RaHMSH);
				if( !feof( Input))
					ReadDec( Input, &In.DecDMS);
				if( !feof( Input))
					if( FReadStringToCharCountOrNewLine( Input, Name, sizeof( Name)))
						;
				fclose( Input);
				CalcRadFromHMSH( &In.Ra, In.RaHMSH);
				CalcRadFromDMS( &In.Dec, In.DecDMS);
				ResultFlag = True;
			}
		}
	}
	RemoveWindow(MsgFrame);
	if( ResultFlag)
	{
		DisplayIn( Filename, Name);
		LastDataObject = In;
	}
	return ResultFlag;
}

void DisplayIn( char* FileName, char* ObjectName)
{
	TextAttr = CurrentText;
	VidMemXY = DisplayXY[DisplayInputRa];
	VidMemRaHMS( &In);
	VidMemXY = DisplayXY[DisplayInputDec];
	VidMemDecDMS( &In);

	TextAttr = CurrentText;
	VidMemXY = DisplayXY[DisplayFile];
	sprintf( StrBuf, "%s", NameBlanks);
	WriteStrBufToScreen_f_ptr();
	VidMemXY = DisplayXY[DisplayFile];
	sprintf( StrBuf, "%s", FileName);
	/* limit # of displayed chars */
	StrBuf[23] = '\0';
	WriteStrBufToScreen_f_ptr();

	VidMemXY = DisplayXY[DisplayObject];
	sprintf( StrBuf, NameBlanks);
	WriteStrBufToScreen_f_ptr();
	VidMemXY = DisplayXY[DisplayObject];
	sprintf( StrBuf, "%s", ObjectName);
	/* limit # of displayed chars */
	StrBuf[23] = '\0';
	WriteStrBufToScreen_f_ptr();
}

void LoadScrollFile( void)
{
	SCROLL_TYPE ScrollType;
	struct SCROLL* Scroll;


	Input = fopen( ScrollFilename, "r");
	if( Input == NULL)
		BadExit( strcat( "Could not open ", ScrollFilename));

	AutoScrollAlertFlag = Yes;

	/* make scroll linked list */
	FreeAllScroll();
	while( !feof( Input))
	{
		ScrollType = no_type;
		strcpy( Name, "");
		fscanf( Input, "%s", Name);
		/* if 1st char is ';' then goto next line */
		if( Name[0] == ';')
			FReadToNewLine( Input);
		if( (strncmpi( Name, "drift_altaz", sizeof( Name))) == 0)
			ScrollType = drift_altaz;
		if( (strncmpi( Name, "drift_equat", sizeof( Name))) == 0)
			ScrollType = drift_equat;
		if( (strncmpi( Name, "abs_altaz", sizeof( Name))) == 0)
			ScrollType = absolute_altaz;
		if( (strncmpi( Name, "off_altaz", sizeof( Name))) == 0)
			ScrollType = offset_altaz;
		if( (strncmpi( Name, "a", sizeof( Name))) == 0)
			ScrollType = absolute_equat;
		if( (strncmpi( Name, "o", sizeof( Name))) == 0)
			ScrollType = offset_equat;
		if( (strncmpi( Name, "1", sizeof( Name))) == 0)
			ScrollType = init1;
		if( (strncmpi( Name, "2", sizeof( Name))) == 0)
			ScrollType = init2;
		if( (strncmpi( Name, "3", sizeof( Name))) == 0)
			ScrollType = init3;
		if( (strncmpi( Name, "1i", sizeof( Name))) == 0)
			ScrollType = init1usinginput;
		if( (strncmpi( Name, "2i", sizeof( Name))) == 0)
			ScrollType = init2usinginput;
		if( (strncmpi( Name, "3i", sizeof( Name))) == 0)
			ScrollType = init3usinginput;
		if( (strncmpi( Name, "f1", sizeof( Name))) == 0)
			ScrollType = file1;
		if( (strncmpi( Name, "f2", sizeof( Name))) == 0)
			ScrollType = file2;
		if( (strncmpi( Name, "f3", sizeof( Name))) == 0)
			ScrollType = file3;
		if( (strncmpi( Name, "trackon", sizeof( Name))) == 0)
			ScrollType = trackon;
		if( (strncmpi( Name, "trackoff", sizeof( Name))) == 0)
			ScrollType = trackoff;
		if( (strncmpi( Name, "auto_scroll_alert_off", sizeof( Name))) == 0)
			ScrollType = auto_scroll_alert_off;
		if( (strncmpi( Name, "analyze", sizeof( Name))) == 0)
			ScrollType = analyze;
		if( (strncmpi( Name, "alt_offset", sizeof( Name))) == 0)
			ScrollType = alt_offset;
		if( (strncmpi( Name, "auto_scroll", sizeof( Name))) == 0)
			ScrollType = auto_scroll;
		if( (strncmpi( Name, "auto_scroll_off", sizeof( Name))) == 0)
			ScrollType = auto_scroll_off;
		if( (strncmpi( Name, "move_zero_pec", sizeof( Name))) == 0)
			ScrollType = move_zero_pec;
		if( (strncmpi( Name, "save1", sizeof( Name))) == 0)
			ScrollType = save1;
		if( (strncmpi( Name, "restore1", sizeof( Name))) == 0)
			ScrollType = restore1;
		if( (strncmpi( Name, "msarcsecsec", sizeof( Name))) == 0)
			ScrollType = msarcsecsec;
		if( (strncmpi( Name, "handpad_mode", sizeof( Name))) == 0)
			ScrollType = handpad_mode;
		if( (strncmpi( Name, "hs", sizeof( Name))) == 0)
			ScrollType = halfstep;
		if( (strncmpi( Name, "new_equat", sizeof( Name))) == 0)
			ScrollType = new_equat;
		if( (strncmpi( Name, "new_altaz", sizeof( Name))) == 0)
			ScrollType = new_altaz;
		if( (strncmpi( Name, "set_equat", sizeof( Name))) == 0)
			ScrollType = set_equat;
		if( (strncmpi( Name, "set_altaz", sizeof( Name))) == 0)
			ScrollType = set_altaz;
		if( (strncmpi( Name, "reset_equat", sizeof( Name))) == 0)
			ScrollType = reset_equat;
		if( (strncmpi( Name, "reset_altaz", sizeof( Name))) == 0)
			ScrollType = reset_altaz;
		if( (strncmpi( Name, "reset_home", sizeof( Name))) == 0)
			ScrollType = reset_home;
		if( (strncmpi( Name, "data_file", sizeof( Name))) == 0)
			ScrollType = data_file;
		if( (strncmpi( Name, "move_file", sizeof( Name))) == 0)
			ScrollType = move_file;
		if( (strncmpi( Name, "prompt", sizeof( Name))) == 0)
			ScrollType = prompt;

		switch( ScrollType)
		{
			case drift_altaz:
			case absolute_altaz:
			case offset_altaz:
			case new_altaz:
				Scroll = InitScroll( Yes, No);
				Scroll->ScrollType = ScrollType;
				fscanf( Input, "%lf", &Scroll->P->Alt);
				/* 9999 means use current value when scroll file is executing, so leave 9999 intact*/
				if( Scroll->P->Alt == 9999)
					;
				else
					Scroll->P->Alt *= DegToRad;
				fscanf( Input, "%lf", &Scroll->P->Az);
				if( Scroll->P->Az == 9999)
					;
				else
					Scroll->P->Az *= DegToRad;
				FReadDouble( Input, &Scroll->Sec);
				if( FReadStringToCharCountOrNewLine( Input, Scroll->Note, sizeof( Scroll->Note)))
					;
				else
					FReadToNewLine( Input);
				AddScroll( Scroll);
				break;
			case drift_equat:
			case absolute_equat:
			case offset_equat:
			case init1:
			case init2:
			case init3:
			case new_equat:
				Scroll = InitScroll( Yes, No);
				Scroll->ScrollType = ScrollType;
				ReadScrollEquatCoord( Scroll);
				AddScroll( Scroll);
				break;
			case init1usinginput:
			case init2usinginput:
			case init3usinginput:
			case file1:
			case file2:
			case file3:
			case trackon:
			case trackoff:
			case auto_scroll_alert_off:
			case analyze:
			case alt_offset:
			case auto_scroll:
			case auto_scroll_off:
			case move_zero_pec:
			case save1:
			case restore1:
			case set_equat:
			case set_altaz:
			case reset_equat:
			case reset_altaz:
			case reset_home:
			case data_file:
			case move_file:
			case prompt:
				Scroll = InitScroll( No, No);
				Scroll->ScrollType = ScrollType;
				if( FReadStringToCharCountOrNewLine( Input, Scroll->Note, sizeof( Scroll->Note)))
					;
				else
					FReadToNewLine( Input);
				AddScroll( Scroll);
				break;
			case msarcsecsec:
			case handpad_mode:
				Scroll = InitScroll( No, No);
				Scroll->ScrollType = ScrollType;
				fscanf( Input, "%d", &Scroll->i1);
				if( FReadStringToCharCountOrNewLine( Input, Scroll->Note, sizeof( Scroll->Note)))
					;
				else
					FReadToNewLine( Input);
				AddScroll( Scroll);
				break;
			case halfstep:
				Scroll = InitScroll( No, Yes);
				Scroll->ScrollType = ScrollType;
				fscanf( Input, "%ld", &Scroll->L->A);
				fscanf( Input, "%ld", &Scroll->L->Z);
				if( FReadStringToCharCountOrNewLine( Input, Scroll->Note, sizeof( Scroll->Note)))
					;
				else
					FReadToNewLine( Input);
				AddScroll( Scroll);
				break;
			}
	}
	fclose( Input);
	if( TotalScrollCount)
	{
		DisplayIn( ScrollFilename, NameBlanks);
		ScrollLoaded = Yes;
		DisplayScrollCountsOnScreen();
	}
	else
	{
		PressKeyToContMsg( "scroll file contains no scroll commands");
		DisplayIn( NameBlanks, NameBlanks);
	}
}

void DisplayScrollCountsOnScreen( void)
{
	TextAttr = CurrentText;
	VidMemXY = DisplayXY[DisplayScrollCounts];
	sprintf( StrBuf, "%3d/%3d", CurrentScrollCount, TotalScrollCount);
	WriteStrBufToScreen_f_ptr();
}

void ClearScrollCountsDisplayArea( void)
{
	TextAttr = CurrentText;
	VidMemXY = DisplayXY[DisplayScrollCounts];
	sprintf( StrBuf, "       ");
	WriteStrBufToScreen_f_ptr();
}

void ReadScrollEquatCoord( struct SCROLL* Scroll)
{
	ReadRa( Input, &Scroll->P->RaHMSH);
	CalcRadFromHMSH( &Scroll->P->Ra, Scroll->P->RaHMSH);
	ReadDec( Input, &Scroll->P->DecDMS);
	CalcRadFromDMS( &Scroll->P->Dec, Scroll->P->DecDMS);
	FReadDouble( Input, &Scroll->Sec);
	if( FReadStringToCharCountOrNewLine( Input, Scroll->Note, sizeof( Scroll->Note)))
		;
	else
		FReadToNewLine( Input);
}

void ProcessScroll( void)
{
	int TicksForMove;
	int TickCount;
	struct AZDouble MovePerTick;
	struct AZDouble StartAltaz;


	ScrollUnderway = Yes;
	AutoScrollFlag = No;
	WriteHandpadStatus();
	CurrentScroll = NULL;
	while( ScrollFlag)
	{
		AlignMs();
		/* get the next Scroll */
		if( CurrentScroll == NULL)
			CurrentScroll = FirstScroll;
		else
			CurrentScroll = CurrentScroll->NextScroll;
		/* if still NULL (end of scroll tour) and auto repeat set, start over */
		if( CurrentScroll == NULL && HandpadFlag == ScrollAutoTour)
		{
			CurrentScroll = FirstScroll;
			CurrentScrollCount = 0;
		}

		/* if finished, clear out display scroll count area, else continue on with scroll commands */
		if( CurrentScroll == NULL)
		{
			ScrollFlag = No;
			ClearScrollCountsDisplayArea();
		}
		else
		{
			CurrentScrollCount++;
			DisplayScrollCountsOnScreen();
			if( !ScrollAutoTour || (ScrollAutoTour && AutoScrollAlertFlag))
			{
				/* give 1/2 sec to move hand pad mode switch back to center position, otherwise moving
				hand paddle switch back to center position while the slew is starting will result in
				cancelled slew */
				sound( 200);
				delay( 55);
				nosound();
				delay( 445);
			}

			Buttons = Off;
			ReadHandpad_f_ptr();

			CurrentScrollFlag = Yes;

			/* display scroll action to be done */
			VidMemXY = DisplayXY[DisplayObject];
			sprintf( StrBuf, NameBlanks);
			WriteStrBufToScreen_f_ptr();
			TextAttr = CurrentText;
			VidMemXY = DisplayXY[DisplayObject];
			switch( CurrentScroll->ScrollType)
			{
				case drift_altaz:
					sprintf( StrBuf, "drfaz:%s", CurrentScroll->Note);
					break;
				case offset_altaz:
					sprintf( StrBuf, "offaz:%s", CurrentScroll->Note);
					break;
				case absolute_altaz:
					sprintf( StrBuf, "absaz:%s", CurrentScroll->Note);
					break;
				case drift_equat:
					sprintf( StrBuf, "drfeq:%s", CurrentScroll->Note);
					break;
				case offset_equat:
					sprintf( StrBuf, "offeq:%s", CurrentScroll->Note);
					break;
				case absolute_equat:
					sprintf( StrBuf, "abseq:%s", CurrentScroll->Note);
					break;
				case init1:
					sprintf( StrBuf, "init1:%s", CurrentScroll->Note);
					break;
				case init2:
					sprintf( StrBuf, "init2:%s", CurrentScroll->Note);
					break;
				case init3:
					sprintf( StrBuf, "init3:%s", CurrentScroll->Note);
					break;
				case init1usinginput:
					sprintf( StrBuf, "in1in:%s", CurrentScroll->Note);
					break;
				case init2usinginput:
					sprintf( StrBuf, "in2in:%s", CurrentScroll->Note);
					break;
				case init3usinginput:
					sprintf( StrBuf, "in3in:%s", CurrentScroll->Note);
					break;
				case file1:
					sprintf( StrBuf, "file1:%s", CurrentScroll->Note);
					break;
				case file2:
					sprintf( StrBuf, "file2:%s", CurrentScroll->Note);
					break;
				case file3:
					sprintf( StrBuf, "file3:%s", CurrentScroll->Note);
					break;
				case trackon:
					sprintf( StrBuf, "tckon:%s", CurrentScroll->Note);
					break;
				case trackoff:
					sprintf( StrBuf, "tckof:%s", CurrentScroll->Note);
					break;
				case auto_scroll_alert_off:
					sprintf( StrBuf, "autof:%s", CurrentScroll->Note);
					break;
				case analyze:
					sprintf( StrBuf, "analy:%s", CurrentScroll->Note);
					break;
				case alt_offset:
					sprintf( StrBuf, "altof:%s", CurrentScroll->Note);
					break;
				case auto_scroll:
					sprintf( StrBuf, "autos:%s", CurrentScroll->Note);
					break;
				case auto_scroll_off:
					sprintf( StrBuf, "auts0:%s", CurrentScroll->Note);
					break;
				case move_zero_pec:
					sprintf( StrBuf, "m0pec:%s", CurrentScroll->Note);
					break;
				case save1:
					sprintf( StrBuf, "save1:%s", CurrentScroll->Note);
					break;
				case restore1:
					sprintf( StrBuf, "rest1:%s", CurrentScroll->Note);
					break;
				case msarcsecsec:
					sprintf( StrBuf, "msarc:%s", CurrentScroll->Note);
					break;
				case handpad_mode:
					sprintf( StrBuf, "hndpd:%s", CurrentScroll->Note);
					break;
				case halfstep:
					sprintf( StrBuf, "hstep:%s", CurrentScroll->Note);
					break;
				case new_equat:
					sprintf( StrBuf, "newEQ:%s", CurrentScroll->Note);
					break;
				case new_altaz:
					sprintf( StrBuf, "newAZ:%s", CurrentScroll->Note);
					break;
				case set_equat:
					sprintf( StrBuf, "setEQ:%s", CurrentScroll->Note);
					break;
				case set_altaz:
					sprintf( StrBuf, "setAZ:%s", CurrentScroll->Note);
					break;
				case reset_equat:
					sprintf( StrBuf, "rstEQ:%s", CurrentScroll->Note);
					break;
				case reset_altaz:
					sprintf( StrBuf, "rstAZ:%s", CurrentScroll->Note);
					break;
				case reset_home:
					sprintf( StrBuf, "rstHm:%s", CurrentScroll->Note);
					break;
				case data_file:
					sprintf( StrBuf, "in file:%s", CurrentScroll->Note);
					break;
				case move_file:
					sprintf( StrBuf, "mv file:%s", CurrentScroll->Note);
					break;
				case prompt:
					sprintf( StrBuf, "%s", CurrentScroll->Note);
					break;
				default:
					sprintf( StrBuf, "unkno:", CurrentScroll->Note);
			}
			/* limit # of displayed chars */
			StrBuf[23] = '\0';
			WriteStrBufToScreen_f_ptr();

			switch( CurrentScroll->ScrollType)
			{
				case drift_altaz:
					break;
				case drift_equat:
					break;
				case offset_altaz:
				case absolute_altaz:
					/* 9999 means use current value */
					if( CurrentScroll->P->Alt == 9999)
						CurrentScroll->P->Alt = Current.Alt;
					if( CurrentScroll->P->Az == 9999)
						CurrentScroll->P->Az = Current.Az;
					if( CurrentScroll->Sec > 0)
					{
						/* spread move over CurrentScroll->Sec time, calc # of ticks to use */
						TicksForMove = CurrentScroll->Sec * ClockTicksSec;
						/* if offset coordinates */
						if( CurrentScroll->ScrollType == offset_altaz)
						{
							MovePerTick.A = CurrentScroll->P->Alt / (double) TicksForMove;
							MovePerTick.Z = CurrentScroll->P->Az / (double) TicksForMove;
						}
						/* else absolute coordinates */
						else
						{
							MovePerTick.A = (CurrentScroll->P->Alt - Current.Alt) / (double) TicksForMove;
							MovePerTick.Z = (CurrentScroll->P->Az - Current.Az) / (double) TicksForMove;
						}
						TickCount = 0;

						/* if microstepping not possible, then slew: otherwise a total of TicksForMove
						slews will occur */
						if( fabs( MovePerTick.A) > MaxMsDistanceRadTick.A ||
						fabs( MovePerTick.Z) > MaxMsDistanceRadTick.Z)
						{
							Delta.A = CurrentScroll->P->Alt - Current.Alt;
							Delta.Z = CurrentScroll->P->Az - Current.Az;
							/* slew called from here: */
							SetDirDistanceStepsThenMove();
						}
						else
						{
							StartAltaz.A = Current.Alt;
							StartAltaz.Z = Current.Az;
							/* keep control here, microstepping to 'scroll to' position */
							while( TickCount < TicksForMove)
							{
								TickCount++;

								/* compare this with the similar microstep scrolling in offset_equat/
								absolute_equat: here altaz microstepping is done then new coordinates
								calculated, there, for the equat microstepping, Current equat is updated
								and the scope 'tracks' to the newly updated coordinates via
								MoveToCurrentRaDec() which is called from  Move_Update_Handpad_Subr() */
								Delta.A = StartAltaz.A + MovePerTick.A * TickCount - Current.Alt;
								Delta.Z = StartAltaz.Z + MovePerTick.Z * TickCount - Current.Az;
								SetDirDistanceStepsThenMove();
								/* here we are microstepping, so check for LX200 port requests like
								returning the new equatorial coordinates, and, go through the task
								controller, doing such things as displaying the new coordinates */
								CheckLX200Events();
								SequentialTaskController();
							}
						}
					}
					else
					{
						/* if offset coordinates */
						if( CurrentScroll->ScrollType == offset_altaz)
						{
							Delta.A = CurrentScroll->P->Alt;
							Delta.Z = CurrentScroll->P->Az;
						}
						else
						{
							Delta.A = CurrentScroll->P->Alt - Current.Alt;
							Delta.Z = CurrentScroll->P->Az - Current.Az;
						}
						SetDirDistanceStepsThenMove();
					}
					PauseUntilNewSidTime();
					HPEventGetEquat();
					break;
				case offset_equat:
				case absolute_equat:
					SetInputToScroll( CurrentScroll);
					if( CurrentScroll->Sec > 0)
					{
						/* spread move over CurrentScroll->Sec time, calc # of ticks to use */
						TicksForMove = CurrentScroll->Sec * ClockTicksSec;
						/* if offset equatorial coordinates */
						if( CurrentScroll->ScrollType == offset_equat)
						{
							MovePerTick.A = CurrentScroll->P->Ra / (double) TicksForMove;
							MovePerTick.Z = CurrentScroll->P->Dec / (double) TicksForMove;
						}
						/* else absolute equatorial coordinates */
						else
						{
							MovePerTick.A = (CurrentScroll->P->Ra - Current.Ra) / (double) TicksForMove;
							MovePerTick.Z = (CurrentScroll->P->Dec - Current.Dec) / (double) TicksForMove;
						}
						TickCount = 0;
						/* if microstepping not possible, then slew: otherwise a total of TicksForMove
						slews will occur */
						if( fabs( MovePerTick.A) > MaxMsDistanceRadTick.A ||
						fabs( MovePerTick.Z) > MaxMsDistanceRadTick.Z)
						{
							Current.Ra = In.Ra;
							Current.Dec = In.Dec;
						}
						else
							/* keep control here, microstepping to 'scroll to' position */
							while( TickCount < TicksForMove && ScrollFlag)
							{
								TickCount++;

								Current.Ra += MovePerTick.A;
								Current.Dec += MovePerTick.Z;
								Move_Update_Handpad_Subr();
							}
					}
					/* else time for move in seconds is 0, so slew */
					else
					{
						Current.Ra = In.Ra;
						Current.Dec = In.Dec;
					}
					break;
				case init1:
				case init2:
				case init3:
					if( CurrentScroll->ScrollType == init1 ||
					(CurrentScroll->ScrollType == init2 && One.Init) ||
					(CurrentScroll->ScrollType == init3 && Two.Init))
					{
						SetInputToScroll( CurrentScroll);
						Current.Ra = In.Ra;
						Current.Dec = In.Dec;
						switch( CurrentScroll->ScrollType)
						{
							case init1:
								KBEventInitMatrix( 1);
								break;
							case init2:
								KBEventInitMatrix( 2);
								break;
							case init3:
								KBEventInitMatrix( 3);
						}
					}
					break;
				case init1usinginput:
				case init2usinginput:
				case init3usinginput:
					if( CurrentScroll->ScrollType == init1usinginput ||
					(CurrentScroll->ScrollType == init2usinginput && One.Init) ||
					(CurrentScroll->ScrollType == init3usinginput && Two.Init))
					{
						Current.Ra = In.Ra;
						Current.Dec = In.Dec;
						switch (CurrentScroll->ScrollType)
						{
						case init1usinginput :
							KBEventInitMatrix( 1);
							break;
						case init2usinginput :
							KBEventInitMatrix( 2);
							break;
						case init3usinginput :
							KBEventInitMatrix( 3);
						}
					}
					break;
				case file1:
					strcpy( Filename, CurrentScroll->Note);
					ProcessMenuDataFileClosest( FilenameSet, ExactInputFieldsMatchOK);
					Current.Ra = In.Ra;
					Current.Dec = In.Dec;
					break;
				case file2:
					strcpy( Filename, CurrentScroll->Note);
					ProcessMenuDataFileClosest( FilenameSet, AvoidMatchInputFields);
					Current.Ra = In.Ra;
					Current.Dec = In.Dec;
					break;
				case file3:
					strcpy( Filename, CurrentScroll->Note);
					ProcessMenuDataFileClosest( FilenameSet, DoNotMatchInputFile);
					Current.Ra = In.Ra;
					Current.Dec = In.Dec;
					break;
				case trackon:
					TrackFlag = On;
					WriteTrackStatus();
					break;
				case trackoff:
					TrackFlag = Off;
					WriteTrackStatus();
					break;
				case auto_scroll_alert_off:
					AutoScrollAlertFlag = No;
					break;
				case analyze:
					break;
				case alt_offset:
					break;
				case auto_scroll:
					AutoScrollFlag = True;
					break;
				case auto_scroll_off :
					AutoScrollFlag = False;
					break;
				case move_zero_pec:
					break;
				case save1:
					ProcessSav1();
					break;
				case restore1:
					ProcessRes1();
					break;
				case msarcsecsec:
					MsArcsecSec = CurrentScroll->i1;
					if( MsArcsecSec > MaxMsSpeed)
						MsArcsecSec = MaxMsSpeed;
					InitMsTickVars( MsArcsecSec);
					WriteMsArcsecSec();
					break;
				case handpad_mode:
					HandpadFlag = CurrentScroll->i1;
					WriteHandpadStatus();
					break;
				case halfstep:
					Dir.A = CW;
					if( CurrentScroll->L->A < 0)
					{
						CurrentScroll->L->A = -CurrentScroll->L->A;
						Dir.A = CCW;
					}
					Dir.Z = CW;
					if( CurrentScroll->L->Z < 0)
					{
						CurrentScroll->L->Z = -CurrentScroll->L->Z;
						Dir.Z = CCW;
					}
					Steps.A = CurrentScroll->L->A;
					Steps.Z = CurrentScroll->L->Z;
					KBEventMoveHs();
					PauseUntilNewSidTime();
					HPEventGetEquat();
					break;
				case new_equat:
					In.Ra = CurrentScroll->P->Ra;
					In.Dec = CurrentScroll->P->Dec;
					ProcessMenuResetEquat();
					break;
				case new_altaz:
					break;
				case set_equat:
					ProcessMenuInputEquat();
					break;
				case reset_equat:
					ProcessMenuResetEquat();
					break;
				case set_altaz:
					break;
				case reset_altaz:
					break;
				case reset_home:
					ProcessMenuResetHome();
					break;
				case data_file:
					strcpy( Filename, CurrentScroll->Note);
					DataFileNameSet = Yes;
					MenuCoordFile();
					DataFileNameSet = No;
					break;
				case move_file:
					strcpy( Filename, CurrentScroll->Note);
					DataFileNameSet = Yes;
					MenuCoordFile();
					DataFileNameSet = No;
					Current.Ra = In.Ra;
					Current.Dec = In.Dec;
					break;
				case prompt:
					break;
			}

			if( AutoScrollFlag)
			{
				if( KeyStroke)
				{
					AutoScrollFlag = No;
					ScrollFlag = No;
					ClearScrollCountsDisplayArea();
				}
			}
			else
				/* either: after 'scroll to' finished, mark time by tracking, or, slew to position */
				while( CurrentScrollFlag && ScrollFlag)
				{
					Move_Update_Handpad_Subr();
					/* if LeftKey, start next scroll */
					if( Buttons & LeftKey)
						CurrentScrollFlag = No;
				}
		}
	}
	TextAttr = CurrentText;
	VidMemXY = DisplayXY[DisplayObject];
	sprintf( StrBuf, NameBlanks);
	WriteStrBufToScreen_f_ptr();
	ScrollUnderway = No;
	WriteHandpadStatus();
}

void SetInputToScroll( struct SCROLL* Scroll)
{
	if( Scroll->ScrollType == offset_equat)
	{
		In.Ra = Current.Ra + Scroll->P->Ra;
		In.Dec = Current.Dec + Scroll->P->Dec;
	}
	else
	{
		In.Ra = Scroll->P->Ra;
		In.Dec = Scroll->P->Dec;
	}
	In.RaHMSH = Scroll->P->RaHMSH;
	In.DecDMS = Scroll->P->DecDMS;
	/* don't DisplayIn() as it will wipe out just displayed scroll counts and scroll action */
}

void GetGrandTourRec( void)
{
	struct Object O;
	int RecNum = 1;

	/* scan through file, ending with read of desired record */
	while( !feof( Input))
	{
		ReadRa( Input, &O.RaHMSH);
		if( !feof( Input))
			ReadDec( Input, &O.DecDMS);
		if( !feof( Input))
			if( FReadStringToCharCountOrNewLine( Input, O.Name, sizeof( O.Name)))
				;
			else
				FReadToNewLine( Input);

		if( RecNum == GrandTourRecNum)
			break;
		else
			RecNum++;
	}
	if( !feof( Input) && RecNum == GrandTourRecNum)
	{
		/* set Current to grand tour coordinates, then set input coordinates to current coordinates */
		CalcRadFromHMSH( &Current.Ra, O.RaHMSH);
		CalcRadFromDMS( &Current.Dec, O.DecDMS);
		In = Current;
		DisplayIn( GrandTourFilename, O.Name);
	}
	else
	{
		GrandTourRecNum--;
		PressKeyToContMsg( "grand tour: at end of data file");
	}
}

void GetGrandTourRecClosestCurrentEquat( void)
{
	int RecNum, BestRecNum;
	double Angsep, BestAngsep;

	Input = fopen( GrandTourFilename, "r");
	if( Input == NULL)
		PressKeyToContMsg( "data file not loaded");
	else
	{
		RecNum = 1;
		BestRecNum = 1;
		BestAngsep = MAXDOUBLE;

		/* scan through file, ending with read of desired record */
		while( !feof( Input))
		{
			ReadRa( Input, &In.RaHMSH);
			if( !feof( Input))
				ReadDec( Input, &In.DecDMS);
			if( !feof( Input))
				if( FReadStringToCharCountOrNewLine( Input, Name, sizeof( Name)))
					;
				else
					FReadToNewLine( Input);
			if( !feof( Input))
			{
				CalcRadFromHMSH( &In.Ra, In.RaHMSH);
				CalcRadFromDMS( &In.Dec, In.DecDMS);
				Angsep = CalcEquatAngularSep( &Current, &In);
				if( Angsep < BestAngsep)
				{
					BestAngsep = Angsep;
					BestRecNum = RecNum;
				}
			}
			RecNum++;
		}
		fclose( Input);

		GrandTourRecNum = BestRecNum;

		Input = fopen( GrandTourFilename, "r");
		if( Input == NULL)
			BadExit( strcat( "Could not open ", GrandTourFilename));
		GetGrandTourRec();
		fclose( Input);

		GrandTourRecNum--;
	}
}

void ProcessGrandTour( void)
{
	/* time to reset handpad key */
	delay( 1000);
	Input = fopen( GrandTourFilename, "r");
	if( Input == NULL)
		BadExit( strcat( "Could not open ", GrandTourFilename));

	if( Buttons & LeftKey || KeyboardLeftButton || LX200LeftButton)
	{
		KeyboardLeftButton = LX200LeftButton = False;
		KeyboardLeftButton = False;
		GrandTourRecNum++;
		GetGrandTourRec();
	}
	/* Buttons & RightKey */
	else
	{
		KeyboardRightButton = LX200RightButton = False;
		if( GrandTourRecNum == 0)
		{
			PressKeyToContMsg( "Grand Tour: first object not moved to yet");
			DisplayIn( GrandTourFilename, NameBlanks);
		}
		else
		{
			GrandTourRecNum--;
			if( GrandTourRecNum < 1)
			{
				PressKeyToContMsg( "Grand Tour: at first object");
				GrandTourRecNum = 1;
			}
			else
				GetGrandTourRec();
		}
	}
	fclose( Input);
	/* set to yes in ProcessHPEvents() */
	GrandTourFlag = No;
}

void Move_Update_Handpad_Subr( void)
{
	if( Buttons & UpKey || Buttons & DownKey || Buttons & CCWKey || Buttons & CWKey)
		ProcessHPEventsMoveMotors();
	else
	{
		CheckMiscEvents();
		SequentialTaskController();
	}
	ReadHandpad_f_ptr();

	/* if RightKey or KeyStoke, finished with all Scroll */
	if( Buttons & RightKey)
	{
		ScrollFlag = No;
		ClearScrollCountsDisplayArea();
	}
	else
		/* certain keystrokes are defined to emulate the handpad mode switches */
		if( KeyStroke)
		{
			Response = getch();
			if( Response == ExtendedKeyboardStroke)
				Response = getch();
			if( Response == 'l' || Response == 'L' || Response == '<')
				Buttons = LeftKey;
			else
				if( Response == '>')
					Buttons = RightKey;
				else
				{
					ScrollFlag = No;
					ClearScrollCountsDisplayArea();
				}
		}
}

