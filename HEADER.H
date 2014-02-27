#ifndef HEADER_H
#define HEADER_H

/* ports used:
headkey 0x0040001AL
tailkey 0x0040001CL
com1 0x3F8
com2 0x2F8
com3 0x3E8
com4 0x2E8
PIC 0x20
cmos ctrl 0x70
cmos rtn 0x71
tick addr 0x0040006CL
timer 0 0x40
timer ctrl 0x43
pport 0x378, 0x379, 0x37A
*/

/* common typedefs, defines, strings and vars... */

typedef unsigned char Byte;
typedef unsigned char Flag;
typedef unsigned char Boolean;
typedef signed char SignedByte;

#define False ((Byte) 0)
#define True ((Byte) 1) 
#define None ((Byte) 0)
#define High ((Byte) 1) 
#define Low ((Byte) 0) 
#define ByteLogicalHigh ((Byte) 255)
#define ByteLogicalLow ((Byte) 0)
#define PortOn ((Byte) 255)
#define PortOff ((Byte) 0)
#define Yes ((Byte) 1)
#define No ((Byte) 0)
#define On ((Byte) 1)
#define Off ((Byte) 0)
#define Plus ((Byte) 1)
#define Minus ((Byte) 0)
#define Positive ((Byte) 1)
#define Negative ((Byte) 0)
#define CW ((Byte) 2)
#define CCW ((Byte) 1)
#define NoRotation ((Byte) 0)

/* screen window border defines... */

/* single line */
#define HorizBorder       196
#define VertBorder        179
#define TopLeftCorner     218
#define TopRightCorner    191
#define BottomLeftCorner  192
#define BottomRightCorner 217
#define LeftIntersect     195
#define RightIntersect    180

#define DblHorizBorder       205
#define DblVertBorder        186
#define DblTopLeftCorner     201
#define DblTopRightCorner    187
#define DblBottomLeftCorner  200
#define DblBottomRightCorner 188
#define DblLeftIntersect     199
#define DblRightIntersect    182

/* keyboard defines... */

#define Backspace 8
#define Tab 9
#define Return 13
#define Esc 27
#define Blank 32
#define DEL 127

/* extended keyboard defines... */

#define ExtendedKeyboardStroke 0

#define UpCursor    72
#define DownCursor  80
#define LeftCursor  75
#define RightCursor 77
#define Shift_Tab   15
/* the left aprostrophe as found in upper left corner of most keyboards */
#define Left_Apostrophe 96
#define EqualsKey 61
#define PlusKey 43
#define MinusKey 45

#define F1  59
#define F2  60
#define F3  61
#define F4  62
#define F5  63
#define F6  64
#define F7  65
#define F8  66
#define F9  67
#define F10 68
#define F11 133
#define F12 134

#define Shift_F1  84
#define Shift_F2  85
#define Shift_F3  86
#define Shift_F4  87
#define Shift_F5  88
#define Shift_F6  89
#define Shift_F7  90
#define Shift_F8  91
#define Shift_F9  92
#define Shift_F10 93

#define Ctrl_F1  94
#define Ctrl_F2  95
#define Ctrl_F3  96
#define Ctrl_F4  97
#define Ctrl_F5  98
#define Ctrl_F6  99
#define Ctrl_F7  100
#define Ctrl_F8  101
#define Ctrl_F9  102
#define Ctrl_F10 103

#define Alt_F1  104
#define Alt_F2  105
#define Alt_F3  106
#define Alt_F4  107
#define Alt_F5  108
#define Alt_F6  109
#define Alt_F7  110
#define Alt_F8  111
#define Alt_F9  112
#define Alt_F10 113

#define HeadKeysMemoryLocation 0x0040001AL
#define TailKeysMemoryLocation 0x0040001CL
#define KeyStroke (*headkeys != *tailkeys)

/* pointers to memory areas holding start, end of keystrokes */
int far *headkeys;
int far *tailkeys;

Byte LargeNum[10];

/* common message define... */

#define ContMsg "Press any key to continue..."

/* common conversion variables... */

double OneRev;
double HalfRev;
double QtrRev;
double RadToDeg;
double DegToRad;
double RadToArcmin;
double ArcminToRad;
double RadToArcsec;
double ArcsecToRad;
double RadToTenthsArcsec;
double TenthsArcsecToRad;
double RadToHr;
double HrToRad;
double RadToMin;
double MinToRad;
double RadToSec;
double SecToRad;
double RadToHundSec;
double HundSecToRad;
double RadToMilliSec;
double MilliSecToRad;
double DaysToHr;
double DaysToMin;
double DaysToSec;
double ArcsecPerRev;
double RevPerArcsec;
double ArcsecPerDeg;
double DegPerArcsec;
double SidRate;

double ClockTicksDay;
double ClockTicksSec;
double ClockTicksMin;
double ClockTicksHr;
double RadToClockTick;
double ClockTickToRad;

void ContMsgRoutine( void);
void BadMsgExit( const char* Msg);
void InitCommonVars( void);

/* keyboard response or input */
int Response;

#define NameSize  40
#define ValueSize 40
#define ObjectNameSize 24

#define NewLine printf(" \n")
#define NameBlanks "                       "
#define EmptyString ""

#define MaxCharsFlag 1
#define MaxCharsInt 4
#define MaxCharsLong 8
#define MaxCharsDbl 7

struct ffblk Ffblk;
Flag FindFirst;
Flag FoundFile;

char Name[NameSize];
char Value[ValueSize];

typedef enum
{
	UserEscaped,
	UserDidNotEnterNumber,
	UserEnteredNumber
}NUM_RESPONSE;

NUM_RESPONSE Num_Response;

void DisplayNegSign( const int startx, const int starty, const int xsize, const int ysize,
const int attr);
void DisplayColon( const int startx, const int starty, const int xsize, const int ysize,
const int attr);
void DisplayLargeNum( const int num, const int startx, const int starty, const int xsize,
const int ysize, const int attr);
void TestDisplayLargeNum( void);
void GetCurDir( char* Dir);
Flag Gets( char Str[], const int NumChars);
Flag GetNumStr( char NumStr[], const int NumChars);
Flag GetFlag( Flag* Num);
Flag GetInt( int* Num);
Flag GetLong( long* Num);
Flag GetDouble( double* Num);

FILE* Input;
FILE* Output;

void FReadDouble( FILE* File, double* V);
void FReadFlag( FILE* File, Flag* V);
void FReadByte( FILE* File, Byte* V);
void FReadInt( FILE* File, int* V);
void FReadUnsigned( FILE* File, unsigned* V);
void FReadLong( FILE* File, long* V);
void FReadChar( FILE* File, char* V);
void FReadToNewLine( FILE* File);
void FReadToChar( FILE* File, const char V);
Flag FReadStringToCharCountOrNewLine( FILE* File, char* Name, const int CharCount);
int FindNextIncrFilename( char* NameStart, char* Extension);

struct AZByte
{
	Byte A, Z;
};
struct AZFlag
{
	Flag A, Z;
};
struct AZInt
{
	int A, Z;
};
struct AZUnsigned
{
	unsigned A, Z;
};
struct AZLong
{
	long A, Z;
};
struct AZLongV
{
	volatile long A;
	volatile long Z;
};
struct AZDouble
{
	double A, Z;
};

struct HMSH
{
	int Sign;
	int Hr;
	int Min;
	int Sec;
	int HundSec;
};

struct DMS
{
	int Sign;
	int Deg;
	int Min;
	int Sec;
};

/* Ra, Dec, Alt, Az, SidT kept in radians */
struct Position
{
	double Ra;
	struct HMSH RaHMSH;
	double Dec;
	struct DMS DecDMS;
	double HA;
	struct HMSH HAHMSH;
	double Alt;
	double Az;
	double SidT;
	struct HMSH SidTimeHMSH;
	Flag Init;
};

#define FilenameSize 13

typedef enum
{
	NameIsADirectory,
	NameIsAFile
}FNAME_TYPE;

struct LINK_FNAME
{
	char Name[FilenameSize];
	FNAME_TYPE FNameType;
	struct LINK_FNAME* PrevLinkFname;
	struct LINK_FNAME* NextLinkFname;
};

struct LINK_FNAME* FirstLinkFname;
struct LINK_FNAME* CurrentLinkFname;
struct LINK_FNAME* LastLinkFname;

void AddLinkFname( struct LINK_FNAME* LinkFname);
void FreeAllLinkFname( void);
struct LINK_FNAME* InitLinkFname( void);
void SortLinkFname( void);
void DisplayLinkFnames( void);
void LoadFnames( char* Name);

typedef enum
{
	absolute_altaz,
	offset_altaz,
	absolute_equat,
	offset_equat,
	drift_equat,
	drift_altaz,
	init1,
	init2,
	init3,
	init1usinginput,
	init2usinginput,
	init3usinginput,
	file1,
	file2,
	file3,
	trackon,
	trackoff,
	auto_scroll_alert_off,
	analyze,
	alt_offset,
	auto_scroll,
	auto_scroll_off,
	move_zero_pec,
	save1,
	restore1,
	msarcsecsec,
   handpad_mode,
	halfstep,
   new_equat,
	new_altaz,
	set_equat,
	set_altaz,
	reset_equat,
	reset_altaz,
	reset_home,
	data_file,
	move_file,
	prompt,
	no_type
}SCROLL_TYPE;

struct SCROLL
{
	SCROLL_TYPE ScrollType;
	struct Position* P;
	struct AZLong* L;
   int i1;
	double Sec;
	char Note[NameSize];
	struct SCROLL* PrevScroll;
	struct SCROLL* NextScroll;
};

struct SCROLL* FirstScroll;
struct SCROLL* LastScroll;
struct SCROLL* CurrentScroll;
struct SCROLL* PrevScroll;
struct SCROLL* NextScroll;

int TotalScrollCount;
int CurrentScrollCount;

void AddScroll( struct SCROLL* Scr);
void FreeAllScroll( void);
struct SCROLL* InitScroll( Flag InitPosition, Flag InitLong);
struct Position* InitScrollPosition( void);
struct AZLong* InitScrollLong( void);

struct RA_DEC_INIT
{
	double Ra;
	double Dec;
	struct RA_DEC_INIT* PrevRaDecInit;
	struct RA_DEC_INIT* NextRaDecInit;
};

struct RA_DEC_INIT* RaDecInit;
struct RA_DEC_INIT* FirstRaDecInit;
struct RA_DEC_INIT* LastRaDecInit;

void AddRaDecInit( struct RA_DEC_INIT* RaDecInit);
void FreeAllRaDecInit( void);
struct RA_DEC_INIT* InitRaDecInit( void);

/* serial port... */

#define Com1Base 0x3F8
#define Com2Base 0x2F8
#define Com3Base 0x3E8
#define Com4Base 0x2E8

#define IER 1 		/* interrupt enable register */
#define IIR 2     /* interrupt identication register, also the write fifo control register (16550 or better) */
#define LCR 3		/* line control register */
#define MCR 4		/* modem control register */
#define LSR 5		/* line status register */
#define MSR 6 		/* modem status register */
#define MCR_DTR 1 /* bit to turn on DTR */
#define MCR_RTS 2 /* bit to turn on RTS */

/* IER defines */
#define INT_ON_RCV 0x01
#define INT_ON_XMT 0x02

/* IIR defines */
#define INT_PENDING 0x01
#define XMT_INT 0x02
#define RCV_INT 0x04

/* LCR defines */
#define DLAB 0x08

/* LSR defines */
#define DATA_READY 0x01
#define XMT_READY 0x20
#define XMT_COMPLETE 0x40

#define Int4Mask 0x10 /* bit 5, ie, 10000 */
#define Int3Mask 0x08 /* bit 4, ie, 1000 */
#define Com1Int 0x0C  /* IRQ 4 */
#define Com2Int 0x0B  /* IRQ 3 */
#define Com3Int 0x0C  /* IRQ 4 */
#define Com4Int 0x0B  /* IRQ 3 */

#define IntMaskReg 0x21
#define PIC_EOI_ADDR 0x20
#define EOI 0x20

#define NoneParity 0
#define OddParity 8
#define EvenParity 24
#define MarkParity 40
#define SpaceParity 56

#define LF 10
#define SI 15
#define SY 22
#define D2 18

#define MaxComPorts 4

/* for buffers to work, SerBufEnd must be 2^n - 1 */
#define SerBufEnd 255

/* buffer index goes from 0 to SerBufEnd, so array size is SerBufEnd+1 */
/* write buffers */
volatile Byte SerBufWrite[MaxComPorts][SerBufEnd+1];
volatile unsigned SerBufWriteBegIx[MaxComPorts];
volatile unsigned SerBufWriteEndIx[MaxComPorts];
volatile unsigned int SerWriteCount[MaxComPorts];

/* read buffers */
volatile Byte SerBufRead[MaxComPorts][SerBufEnd+1];
volatile unsigned SerBufReadBegIx[MaxComPorts];
volatile unsigned SerBufReadEndIx[MaxComPorts];
volatile unsigned int SerReadCount[MaxComPorts];

unsigned SerialBase;
Byte Parity;
Byte DataBits;
Byte StopBits;

void interrupt( *OldSer1IntVec)();
void interrupt( *OldSer2IntVec)();
void interrupt( *OldSer3IntVec)();
void interrupt( *OldSer4IntVec)();
void interrupt Ser1ISR( void);
void interrupt Ser2ISR( void);
void interrupt Ser3ISR( void);
void interrupt Ser4ISR( void);
Flag InitSerial( const int ComPort, const long BaudRate, const int Parity, const int DataBits,
const int StopBits);
Flag CloseSerial( const int ComPort);
void InitRingBuffers( const int ComPort);
Flag ReadSerial( const int ComPort, Byte* B);
void WriteSerial( const int ComPort, Byte B);
void WriteSerialString( const int ComPort, char C[], const int CharCount);
void WriteSerialPauseUntilXmtFinished( const int ComPort, Byte B);
void WriteSerialStringPauseUntilXmtFinished( const int ComPort, char C[], const int CharCount);
Flag WriteSerialFinished( const int ComPort);
void DisplaySerialString( char C[], const int CharCount);
void TestSerial( const int ComPort);

/* video... */

/* as in 'text' element, analagous to 'pixel' = picture element */
struct Texel
{
	char Ch;
	unsigned char Attr;
};

/* Using this structure we can define a two - dimensional array to correspond to the memory
arrangement of the screen where X is across or columns and Y is up and down or rows (X, Y are
backwards from the x, y in goto( x, y)) */

#define MaxY 25
#define MaxX 80

/* ScreenArea is type defined as a two dimensional array of 'Texel's */
typedef struct Texel ScreenArea[MaxY][MaxX];

#define Screen (*ScreenPtr)

struct XY
{
	int X, Y;
};

char StrBuf[MaxX];
char StrBuf2[MaxX];

/* X = across or columns, and Y = up and down or rows */
struct XY VidMemXY;
int TextAttr;
unsigned MemSeg, MemOff;
ScreenArea far* ScreenPtr;
union REGS Regs;
struct text_info ti;
void( *WriteCharToScreen_f_ptr) ( const char Char);
void( *WriteStrBufToScreen_f_ptr) (void);
void InitVideo( const Flag DisplayOpeningMsgs);
void NULL_FUNCTION( void);
void WriteCharToScreen( const char Char);
void WriteStrBufToScreen( void);
void Clrscr( void);
void TestVideo( void);

/* CMOS clock... */

/* To read a byte from CMOS, do an OUT 70H,addr; followed by IN 71H.
To write a byte to CMOS,  do an OUT 70H,addr; followed by OUT 71H,value. */

#define CMOS_Ctrl_Port 0x70
#define CMOS_Rtn_Port  0x71

#define CCP      CMOS_Ctrl_Port
#define CMOS_RTN inport( CMOS_Rtn_Port)
/* BCD = binary coded decimal; if 0xAB, then 'A' is tens and 'B' is ones in
binary */
#define DECODE_BCD (int) (((BCD & 0xF0)>>4)*10 + (BCD & 0x0F))
#define ENCODE_BCD (int) (((BCD/10)<<4) + BCD - (BCD/10)*10)

#define CMOS_Reg_BCD_Sec   0
#define CMOS_Reg_BCD_Min   2
#define CMOS_Reg_BCD_Hr    4
#define CMOS_Reg_BCD_Day   7
#define CMOS_Reg_BCD_Mon   8
#define CMOS_Reg_BCD_Yr    9
#define CMOS_Reg_Status_A 10
#define CMOS_Reg_Status_B 11
#define CMOS_Reg_Status_C 12
#define CMOS_Reg_Status_D 13

#define Update_In_Progress 128

#define TimeOfDayInterrupt 0x1A

struct time t;
struct date d;

/* vars for CMOS clock... */

int DOS_Yr;
int DOS_Mon;
int DOS_Day;
int DOS_Hr;
int DOS_Min;
int DOS_Sec;

int CMOS_Status_A;
int BCD;
int Hold_BCD;
int CMOS_Sec;
int CMOS_Min;
int CMOS_Hr;
int CMOS_Day;
int CMOS_Mon;
int CMOS_Yr;
int Century;

/* access to CMOS Real Time Clock (RTC): 0 for direct port access, 1 for bios interrupt call access
- use 1 only if machine locks up on direct port call */
Flag CMOS_RTC_Access;

void( *SetDOSToCMOS_RTC_f_ptr) (void);

int UpdateInProgress( void);
void DisplayCMOSTimeDate( void);
void SetDOSToCMOS_RTC_ViaPort( void);
void SetDOSToCMOS_RTC_ViaBios( void);
void SetDOSToCMOS_Vars( void);
void SetRealTimeClock( const int hr, const int min, const int sec);
void SetDateofRealTimeClock( const int century, const int year, const int mon, const int day);
Flag ReadRealTimeClock( int* hr, int* min, int* sec, int* daylight);
Flag ReadDateofRealTimeClock( int* century, int* year, int* mon, int* day);

/* astronomical times... */

#define ETZ 5
#define TICK_ADDR 0x0040006CL

/* pointer to bios memory area holding bios clock tick count */
long far* Ticks;
long StartTicks;
int Days;
double JD;
double SidHr;
double SidT;
double StartSidT;

struct HMSH SidTimeHMSH;

/* current coordinates */
/* define here bec. of NewSidT(); */
struct Position Current;

void InitTimes(  const Flag DisplayOpeningMsgs, const double Tz, const int DST, double LongitudeDeg);

void SetStartBiosClockAndSidTime( const long Yr, const int M, const int D, const int h, const int m,
const double s, const double Tz, const int DST,
double LongitudeDeg);

int NewSidT( void);

void CalcJD( const long Y, const int M, const int D, const double Tz, const int DST, const int h,
const int m, const double s);

void CalcSidHr( const long Y, const int M, const int D, const double Tz, const int DST, const int h,
const int m, const double s, const double LongitudeDeg);

void GetHMSH( long TotalHundSec, struct HMSH* V);
void DisplayHMSH( const struct HMSH V);
void StrBufHMSH( const struct HMSH V);
void DisplayHMST( const struct HMSH V);
void DisplayHMS( const struct HMSH V);
void StrBufHMS( const struct HMSH V);
void DisplaySHMS( const struct HMSH V);
void StrBufSHMS( const struct HMSH V);
void DisplaySHMSH( const struct HMSH V);
void StrBufSHMSH( const struct HMSH V);
void CalcRadFromHMSH( double* R, struct HMSH V);
void CalcRadFromDMS( double* R, struct DMS V);

void TestTimes( void);

/* coordinates... */

#define SizeofHMSX 8
#define SizeofDMSX 9
#define SizeofDegX 7

void ValidRa( struct Position* Pos);
void ValidHA( struct Position* Pos);
void ValidAz( struct Position* Pos);

void SetCoordDeg( struct Position* Pos, const double RaDeg, const double DecDeg,
const double AltDeg, const double AzDeg, const double SidTDeg);

void ShowCoord( struct Position* Pos);
void GetDMS( long TotalSec, struct DMS* V);
void DisplayDMS( const struct DMS D);
void StrBufDMS( const struct DMS D);
void VidMemDeg( const double Deg);
void VidMemRaHMS( struct Position* Pos);
void VidMemRaHMSH( struct Position* Pos);
void VidMemHAHMS( struct Position* Pos);
void VidMemHAHMSH( struct Position* Pos);
void VidMemDecDMS( struct Position* Pos);
void VidMemSidT( struct Position* Pos);

/* parallel port... */

#define PPortPin1 1
#define PPortPin14 2
#define PPortPin16 4
#define PPortPin17 8
#define PPortPins1_14 3
#define PPortPins16_17 12

unsigned PPortAddr;
unsigned PPortAddrOutByte;
unsigned PPortAddrInNibble;
unsigned PPortAddrInOutNibble;
Byte OutValue;
Byte InNibbleValue;
Byte BiDirOutNibbleValue;
Byte BiDirInNibbleValue;

void InitPPort( void);
void ClosePPort( void);
unsigned GetPPortAddr( int lpt);
void DisplayLpts (void);
void OutByte( void);
void BiDirOutNibble( void);
Byte InNibble4Bit( void);
Byte InNibble5Bit( void);
Byte BiDirInNibble( void);
void TestPPort( void);

/* to save screen window */
char WinBuffer[MaxX*MaxY*2];

/* mouse functions */

#define MOUSE_INTERRUPT 0x33

#define RESET_MOUSE 0
#define DISPLAY_MOUSE 1
#define HIDE_MOUSE 2
#define GET_MOUSE_POSITION 3
#define GET_MOUSE_POSITION_RELATIVE 0x0B
#define SET_MOUSE_POSITION 4
#define GET_MOUSE_PRESS_INFO 5
#define GET_MOUSE_RELEASE_INFO 6
#define SET_MOUSE_SENSITIVITY 0x1A
#define GET_MOUSE_SENSITIVITY 0x1B

#define SUCCESSFULL_MOUSE_RESET -1

#define LEFT_BUTTON 0
#define RIGHT_BUTTON 1

#define MaxXText 80
#define MaxYText 25
#define XYTextRatio (MaxXText/MaxYText)
#define MouseMaxXText (MaxXText-1)
#define MouseMaxYText (MaxYText-1)
/* max MouseXGraph = 640 (8*80 chars), max MouseYGraph = 200 (8*25 chars) */
#define MaxMouseXGraph (8*MaxXText)
#define MaxMouseYGraph (8*MaxYText)
#define MidMouseXGraph (MaxMouseXGraph/2)
#define MidMouseYGraph (MaxMouseYGraph/2)

int inax;
int inbx;
int incx;
int indx;
int outax;
int outbx;
int outcx;
int outdx;
int HorizMickeySensitivity;
int VertMickeySensitivity;
int ThresholdDblSpeed;
int MouseXGraph;
int MouseYGraph;
int MouseXMickeyRelative;
int MouseYMickeyRelative;
int MouseXText;
int MouseYText;

int MouseLeftClickCount;
int MouseRightClickCount;
Flag MouseLeftButtonDown;
Flag MouseRightButtonDown;

void InitMouse( void);
void CallMouse( void);
void GetDefaultMouseSensitivity( void);
/* this function decodes cx and dx into horiz and vert positions; call CallMouse() first */
void DecodeMousePosition( void);
void ResetMouse( void);
void LowMouseSensitivity( void);
void ResetMouseSensitivity( void);
void DisplayMouse( void);
void HideMouse( void);
void GetMousePosition( void);
void GetMousePositionRelative( void);
/* mouse stays hidden if HideMouse() called previously */
void SetMousePosition(int Horiz, int Vert);
void CenterMouseCursor( void);
int MouseLeftButtonPressCount( void);
int MouseRightButtonPressCount( void);
int MouseLeftButtonReleaseCount( void);
int MouseRightButtonReleaseCount( void);
void TestMouse( void);

/* config... */

#define DefaultConfigFile "CONFIG.DAT"
#define MaxMs 40

/* configuration vars... */

typedef enum
{
	StandardHandpad,
	DirectionOnlyHandpad
}HANDPAD_DESIGN;

typedef enum
{
	FocusMethod_OnOff_16_17,
	FocusMethod_OnOff_16_17_Slow1_14,
	FocusMethod_Pulse_1_14,
	FocusMethod_Pulse_16_17
}FOCUS_METHOD;

typedef enum
{
	MotorControl_PWM_PCB,
	MotorControl_DanGray_Serial,
	MotorControl_PulseDir
}MOTOR_CONTROL_METHOD;

char ConfigFile[NameSize];

int DefaultBackground;
int DefaultColor;
int TitleColor;
int BorderColor;
int MenuColor;
int DisplayColor;
int SelectColor;
int CurrentColor;
int SelectBackground;
/* if 1, then confirm 'quit' */
Flag ConfirmQuit;
/* if 1, display opening messages */
Flag DisplayOpeningMsgs;
/* move of this distance or greater triggers confirmation msg */
double MoveHsMsgDeg;
/* location where SlewFile and SlewOutFile are located */
char* InterfacePath;
/* if desiring to use mount for menu, scope control functions */
Flag UseMouseFlag;
/* 1 = use IACA */
Flag IACA_Flag;
Flag AutoGEMFlip;
double AutoGEMFlipFuzzDeg;
Flag Siderostat;
Flag Equatorial;
double HomeAltDeg;
double HomeAzDeg;
int MsArcsecSec;
double AltFullStepSizeArcsec;
double AzFullStepSizeArcsec;
/* if handpad present, then set to 1 */
Flag HandpadPresentFlag;
int StartingHandpadMode;
/* standard design, or, direction input spread across the 4 input lines */
int HandpadDesign;
/* flip handpad's up/down buttons if GEM flipped since declination direction inverted when GEM flipped */
Flag HandpadFlipUpDownWithGEMFlip;
int BacklashMsArcsecSec;
double AltBacklashArcmin;
double AzBacklashArcmin;
/* move beyond this limits not allowed while slewing using interrupt timer */
/* to disable, set both limits to zero */
double AltLowLimitDeg;
double AltHighLimitDeg;
/* to disable, set both limits to zero */
double AzLowLimitDeg;
double AzHighLimitDeg;
/* guiding speed */
int GuideArcsecSec;
FOCUS_METHOD FocusMethod;
Flag ReverseFocusMotor;
int FocusFastStepsSec;
int FocusSlowStepsSec;
int FocusPosition;
MOTOR_CONTROL_METHOD MotorControlMethod;
int MotorWindings;
/* if 1, then output is inverted, ie, from logical high to logical low */
Flag InvertOutput;
Flag ReverseAMotor;
Flag ReverseZMotor;
/* 0 if simple ramp, 1 if S ramp */
Flag HsRampStyle;
/* if 1, use IRQ 8 to time halfsteps, otherwise disable IRQs and time halfsteps with a loop */
Flag HsTimerFlag;
/* delay loop value for slowest halfstep motor speed */
int MaxDelay;
/* delay loop value for highest halfstep motor speed */
int MinDelay;
/* # of times to repeat delay loop: cuts down on delay array size */
int HsDelayX;
/* halfstep ramp time multiplier */
int HsRampX;
/* # of steps ok to do without disabling IRQs: ignored if HsTimerFlag is 1 */
int InterruptHs;
/* # of repeats of slowest motor speed to hold motors in position */
int HoldReps;
/* if non-zero, then overvoltage for halfstepping enabled: this value contains the IRQ8 timer value
that raises then lowers the control line, which is parallel port 17; if delay method of timing
halfsteps, then if this value if non-zero, overvoltage line turned at at start of halfstepping */
int HsOverVoltageControl;
/* max # of consecutive slews before program recalcs Current equatorial based on Current altazimuth
*/
int MaxConsecutiveSlews;
/* # of seconds before idle motor centered on a rotor will power down when microstepping */
int MsPowerDownSec;
/* number of pulse width modulations per bios clock tick */
int PWMRepsTick;
/* whether the PWMRepsTick value should be automatically calculated by averaging */
Flag AvgPWMRepsTickOnFlag;
/* reps of PWM[] to form a voltage waveform */
int MsDelayX;
/* delay loop at end of each PWM */
int MsPause;
/* number of microsteps per fullstep; cannot exceed MaxMs */
int Ms;
/* user defined max # of microstep increments per each PWM: cannot exceed a halfstep */
int MaxIncrMsPerPWM;
Flag MaxIncrMsPerPWMWasRead;
/* user defined max # of microstep increments per each PWM for microstepping motion; above this
value and up to MaxIncrMsPerPWM, the microstepping routine switches to halfstep mode */
int MsHsToggleIncrMsPerPWM;
Flag MsHsToggleIncrMsPerPWMWasRead;
/* max that any value of PWM[] can attain */
int MaxPWM;
/* if MaxPWM is in config file */
Flag MaxPWMFoundFlag;
/* used to designate type of PWM entry in config.dat: simple form is PWM[x] <value>, complex form is
PWM[Index] <ValueA> : <ValueB> */
Flag UseComplexPWMFlag;
/* ditto for the PWMZ fields */
Flag UseComplexPWMZFlag;
/* array to hold the configured PWM values */
struct AZInt PWM[MaxMs];
/* holds optional 'Z' motor PWM values */
struct AZInt PWMZ[MaxMs];
Flag SavePWMComp;
/* holds compensating values for PWM current for each winding */
double PWM_A_Comp[5];
double PWM_Z_Comp[5];
/* array to hold the configured PWM values after being corrected for winding current compensation */
struct AZInt PWM_Comp[MaxMs];
/* comm port to accept LX200 serial protocol commands */
int LX200ComPort;
long LX200BaudRate;
int LX200MotionTimeoutSec;
long LX200SlewHs;
/* toggles between standard and long format for coordinates */
Flag LX200_LongFormat;
/* program startup state of initializations */
Flag StartInitState;
double Z1Deg;
double Z2Deg;
double Z3Deg;
double LatitudeDeg;
double LongitudeDeg;
double Tz;
int DST;
char* TestString;
unsigned StartPPortAddr;
long BaudRate;
     
void ReadConfig( void);
void ReadPWMValue( void);
void WriteConfig( void);

/* handpad... */

#define RawUpKey 16
#define RawDownKey 32
#define RawCCWKey 64
#define RawCWKey (16 + 32)
#define RawAltCWKey 128
#define RawLeftKey (16 + 64)
#define RawRightKey (32 + 64)
#define RawMsKey 128
#define AutoPECSynch101213 (16 + 32 + 64)

#define UpKey 1
#define DownKey 2
#define CCWKey 4
#define CWKey 8
#define LeftKey 16
#define RightKey 32
#define MsKey 64

/* from InNibble(): only select upper 4 bits and then invert highest bit (128) */
#define SET_HANDPAD (Handpad = (inportb( PPortAddrInNibble)& 240)^128)

/* return value from handpad */
int Handpad;
/* value of Handpad at start of move */
int InitHandpad;
/* if handPad changes during move, HandpadOKFlag set to No */
int HandpadOKFlag;
/* Pad's speed switch setting: if true, speed == microstepping */
int MsSpeed;
/* Pad's buttons */
int Buttons;
/* up, down buttons reversed */
Flag UpDownButtonsReversedFlag;

void( *ReadHandpad_f_ptr) (void);

void InitializeHandpad( void);
void ReadHandpadSubr( void);
void ReadHandpad( void);
void NoHandpad( void);
void SetHandpadOKFlag( void);
void TestHandpad( void);

/* steppers.. */

/* vars for single pulse/dir control of motors */

#define A_Pulse 1
#define A_Dir 2
#define Z_Pulse 4
#define Z_Dir 8

Flag A_Pulse_State;
Flag Z_Pulse_State;
Flag Aux_Pulse_State;

/* vars relating to determining the PWM reps average value */
#define SizeofPWMRepsTickArray 64

int AvgPWMRepsTick;
int PWMRepsTickArray[SizeofPWMRepsTickArray];
int TotalPWMRepsTickArray;
int PWMRepsTickArrayIx;

/* vars relating to interrupt timing of steps */
#define Timer_Int 8
#define Timer_0 0x40
/* control for the 8253 timer chip */
#define Timer_Control 0x43
/* frequency of the timer (1193181 decimal): a timer tick is normally called every 0x10000 counts,
or 18.2 per sec */
#define TIMER_FREQ 0x1234DDL

/* used to store the original interrupt vector of the system timer */
void interrupt( *ClockVect)();

void( *MoveMs_f_ptr) (void);
void( *DisplayMsValues_f_ptr) (void);
void( *SteppersOff_f_ptr) (void);
void( *Hold_f_ptr) (void);
void( *MoveHs_f_ptr) (void);
void( *DoOneHs_f_ptr) ( void);

typedef enum
{
	RampUp,
	MaxSlewUp,
	MaxSlewDown,
	RampDown,
	SlewDone
}SLEW_STATE;

typedef enum
{
	NoAbort,
	HandpadAbort,
	MouseAbort,
	KeyStrokeAbort,
	LX200Abort,
	AltLowLimitAbort,
	AltHighLimitAbort,
	AzLowLimitAbort,
	AzHighLimitAbort,
	GEMflipAbort
}ABORT_STATE;

/* elapsed time for the current slew in progress */
double ElapsedHSSec;
/* sidereal time at start of slew */
double StartSidTHS;
/* holds positions to move to using motor control via serial port */
struct AZLong N;

/* variables that turn off all windings belonging to the stepper motor */
struct AZByte MotorOff;
/* actual PWM loop counts per clock tick */
int ActualPWMRepsTick;
/* steps: must always be positive */
struct AZLong Steps;
struct AZFlag Dir;
struct AZFlag EnableMotor;

/* variables relating to halfstepping... */

volatile long timer_count;
int MaxHsIx;
double AltLowLimit;
double AltHighLimit;
double AzLowLimit;
double AzHighLimit;
Flag AltLimitFlag;
Flag AzLimitFlag;
long RampSteps;
struct AZDouble RampRad;
struct AZDouble InstantRampRad;
/* if the overvoltage control line used in halfstepping is set logical high */
Flag HsOverVoltageSet;

/* holds halfstep bit pattern for parallel port output */
struct AZByte* HsOut;
/* used to tell Hold() which motors to energize */
struct AZByte HoldMotor;

/* for speed control and linear ramping:
HsReps is array of # of half step repetitions to do for each delay or timer value: delay value is
set by moving through an empty for next loop HsRepsIx + MinDelay number of times;
timer value set by HsRepsIx + MinDelay;
HsRepsIx varies from 0 to MaxDelay - MinDelay;
HsReps[0] = MAXLONG; */
long* HsReps;
struct AZInt HsIx;
volatile int HsRepsIx;
int MaxHsRepsIx;
/* max halfsteps that ASteps or ZSteps can be set to */
long MaxHs;
/* # of steps to move */
long MvmtHs;
/* initial # of steps to ramp up */
long RampHs;
/* # of steps to take for a particular HsReps[HsRepsIx] delay or timer value */
long SameSpeedHs;
/* if 1, then don't need IRQs disabled for halfstepping */
Flag InterruptFlag;
unsigned int Timer;
/* # of halfsteps for each stage of halfstep slewing */
long RampUpSteps, MaxSlewUpSteps, MaxSlewDownSteps, RampDownSteps;
/* = 1 if slew was commanded from hand paddle */
Flag SlewStartedFromHandpad;
Flag TrackingViaHs;
SLEW_STATE SlewState;
ABORT_STATE AbortState;

/* variables relating to microstepping... */

/* for microstepping functions */
struct AZInt MsToDo;
struct AZDouble MsRunningTotal;
int HoldAvgPWMRepsTick;
/* use unique PWM values for 'Z' motor */
Flag UsePWMZFlag;
/* # of timer ticks until idle motor centered on a winding is powered down while microstepping */
int StartMsPowerDownCount;
/* current MsPowerDownCounts for each motor */
struct AZInt MsPowerDownCount;
/* set to True to indicate that PWMTickHandler() is installed */
Flag PWMTickHandlerInstalledFlag;
/* set to True by new clock tick interrupt routine: used to signal MoveMs() to
stop the pulse width modulations */
Flag PWMTickHandlerCalledFlag;
/* set by MoveMs() to indicate that pulse width modulations will begin: used by the PWMTickHandler
interrupt routine to determine if SteppersOff() should be called */
Flag PWMUnderwayFlag;

/* # of microsteps per halfstep */
int MsPerHs;
/* conversion factor for microstep array indexes to halfstep array indexes */
int HsIxToMsIxConvFactor;
/* # of microsteps in (4 or 5) windings */
int MsInWindings;
/* accumulated motors' movement stored in form of microsteps */
struct AZLongV AccumMs;
/* Ms increment per Microstep PWM; determines speed of microstepping; # of microsteps per PWM */
struct AZDouble IncrMs;
/* halfstep increment per PWM time */
struct AZDouble IncrHs;
Flag MsAlignFlag;
/* MaxMsIx is the maximum index for MsPWM; the portion behind MaxMsIx is indexed by MsMotorOffIx and
consists of motor off commands */
int MaxMsIx;
int MsMotorOffIx;
/* microstepping arrays containing on/offs generating voltage values */
struct AZByte* MsPWM;
struct AZInt MsIx;
/* microstep distances in arc sec */
struct AZDouble MicroStepSizeArcsec;
/* alt, az microstep sizes in radians */
struct AZDouble MsRad;
/* alt, az halfstep sizes in radians */
struct AZDouble HsRad;
/* max microstepping distance to move in radians per tick */
struct AZDouble MaxMsDistanceRadTick;
/* max microstepping distance to track in radians */
struct AZDouble MaxMsDistanceRadTrack;
/* maximum microstepping speed possible */
int MaxMsSpeed;
/* microsteps per clock tick */
struct AZDouble MsTick;
/* motor powered down status during microstepping */
Flag MsPowerDown;
/* is microstepping proceeding by halfsteps in order to spin as fast as possible? */
struct AZFlag GoByHs;
/* steps actually moved in microstepping routine */
struct AZInt StepsMoved;
struct AZLong StepsToMove;
Flag MsZeroSoundOn;

/* following are used for moving motors at a constant rate and acceleration */
Flag TrackByRateFlag;
struct AZDouble TrackMsPerSec;
struct AZDouble TrackMsAccel;
struct AZDouble TrackMsTick;
struct AZFlag TrackDir;
struct AZInt TrackHoldDir;
struct AZDouble TrackHoldMsTick;
struct AZDouble TrackAccumMsAccel;

void InitSteppers( void);
void CalcVarsRelatingToMs( void);
void CalcVarsRelatingToStepSizes( void);
void CalcVarsRelatingToPWMRepsTick( void);
void StartPWMTickHandler( void);
void StopPWMTickHandler( void);
void CreateHsArrays( void);
void InitHsArrays( void);
void WritePWMValues( void);
void FreeHsArrays( void);
void CreateMsArrays( void);
void CleanupPWM( struct AZInt* PWM);
void InitMsArrays( void);
void FreeMsArrays( void);
void CloseSteppers( void);
void InitMsTickVars( const int MsArcsecSec);
void SteppersOffPWM( void);
void SteppersOffPulseDir( void);
void HoldPWM( void);
void HoldPulseDir( void);
void PauseUntilNewSidTime( void);
void DoOneHsPulseDir( void);
void DoOneHs( void);
void DoHsDelayLoop( void);
void SetTickDelay( void);
void interrupt SlewTickHandler( void);
void RoundUpHs( void);
void MoveHs( void);
void MoveHsUsingIRQTimer( void);
void MoveHsUsingDelayLoop( void);
void interrupt PWMTickHandler( void);
void MoveMs( void);
void UpdatePWMAverage( void);
void DisplayMsValuesPWM( void);
void AlignMs( void);

/* convert... */

/* Conversion routines are based on Sky and Tel, Feb, '89, pg 194-196,
Convert functions input and output horizon increasing CW, then convert to CCW internally for
calculating as per S&T program.
Set coordinates including SidT before making function calls.
Avoid repeatedly alternating GetEquat() and GetAltaz() calls, which can lead to hysteresis or creep
of the coordinates of approx 0.1 arc sec */

/* storage arrays for matrix multiplication */
double QQ[4][4];
double VV[4][4];
double RR[4][4];
double XX[4][4];
double YY[4][4];

/* fabrication errors (in radians): */

/* offset of elevation to perpendicular of horizon, ie, one side of rocker box higher than the other
*/
double Z1;
/* optical axis pointing error in same plane, ie, tube horiz.: optical axis error left to right
(horiz) */
double Z2;
/* correction to zero setting of elevation, ie, vertical offset error */
double Z3;
/* if Z1, Z2, or Z3 is non-zero */
Flag Z1Z2NonZeroFlag;
/* coordinates from initialize position #1, and #2 */
struct Position One, Two, Three;

/* following values in radians */
double Lat;
double LongitudeRad;
double SinLat;
double CosLat;
/* vars baed on config.dat values */
double ConfigLat;
double ConfigSinLat;
double ConfigCosLat;
/* this value, also in radians, used by field rotation angle calculation */
double SinLatDividedByCosLat;
double HoldCurrentDec;
double HoldCosCurrentDec;
double HoldSinCurrentDec;
/* angles that scope should be moved in order to be polar aligned */
struct AZDouble PolarAlign;
/* azimuth offset of init'ed position from true azimuth */
double AzOff;
/* Hour Angle Offset = Local Sidereal Time - scope's meridian, or, HAOff = LST - HA - Ra, or,
HA = LST - HAOff - Ra; (+) offset = scope tilted to West, (-) offset = scope tilted to East;
HAOff varies from - offset to + offset (should only be a few deg.) */
double HAOff;
struct HMSH HAOffHMSH;
/* working vars */
double F, H, W;
/* pick between different subroutines to solve for scope's altaz coordinates (Z1 and Z2 intertwined - must be solved iteratively);
valid values 'S' (simple)
				 'B' (per Taki's naming)
				 'T' (revised by Taki for exact solution)
				 'L' (per Larry Bell)
				 'U' (Larry Bell's altitude solution and Taki's exact solution for azimuth) */
char SubrFlag;

void InitConvert( void);
void InitConvertEquat( void);
void InitConvertAltaz( void);
void FReadPosition( FILE* File, struct Position* Pos);
void FReadPositionToNewLine( FILE* File, struct Position* Pos);
void FWritePosition( FILE* File, struct Position* Pos, Flag CRLF);
Flag LoadAlign( void);
void SetMountErrorsDeg( const double Z1Deg, const double Z2Deg,
const double Z3Deg);
void ZeroArrays( void);
void CheckHoldSinCosCurrentDec( void);
void ArrayAsignInit( const int Init);
void GenerateThirdInit( void);
void TransformMatrix( void);
void InitMatrix( const int Init);
void SubrA( void);
void SubrSwitcher( void);
void SubrS( double CosF, double CosH, double SinF, double SinH);
void SubrB( double CosF, double CosH, double SinF, double SinH, double CosZ1, double CosZ2, double SinZ1, double SinZ2);
void SubrT( double CosF, double CosH, double SinF, double SinH, double CosZ1, double CosZ2, double SinZ1, double SinZ2);
double GetApparentAlt( double CosZ1, double CosZ2, double SinZ1, double SinZ2);
void SubrL( double CosF, double CosH, double SinF, double SinH, double CosZ1, double CosZ2, double SinZ1, double SinZ2);
void SubrU( double CosF, double CosH, double SinF, double SinH, double CosZ1, double CosZ2, double SinZ1, double SinZ2);
void DeterminateSubr( void);
void AngleSubr( void);
void CalcPostInitVars( void);
void GetAltaz( void);
void GetEquat( void);

/* advanced convert routines */

double CalcEquatAngularSep( struct Position* A, struct Position* Z);
double CalcAltazAngularSep( struct Position* A, struct Position* Z);
double AngSepDiff( struct Position* A, struct Position* Z);

/* guide... */

int Ix;

Flag GuideFlag;

/* guiding speed in radians per clock tick */
double GuideRadTick;
/* accumulated guiding amounts in altitude and azimuth */
struct AZDouble AccumGuide;

void InitGuide( void);
void StopGuide( void);

/* IACA event... */

#define PUT_SCOPE if( IACA_Flag) PutScope( Current.Ra, Current.Dec)

long far* IACA_Ra_Decimal_Hrs_Ptr;
long far* IACA_Dec_Decimal_Degrees_Ptr;
long far* Scope_Ra_Decimal_Hrs_Ptr;
long far* Scope_Dec_Decimal_Degrees_Ptr;
double Conv_Factor;

void InitIACA( void);
void PutScope( double RaRad, double DecRad);
int NewIACA( void);
void GetIACA( double* RaRad, double* DecRad);
void TestIACA( void);

/* LX200 protocol...
see lx200.txt for lx200 protocol notes;
the serial interrupt routine reads input from an external device outputting lx200 protocol serial
commands;
every bios clock tick serial new characters stored in the serial input buffer are copied to the
lx200 circular queue;
these characters are then scanned and acted upon: several commands may be acted upon in a single
bios clock tick;
setting the test mode to lx200 will display inputed characters and the deciphered commands */

#define LX200_ACK 6
#define DegSym (char) 223
#define LX200_OK '1'
#define LX200QueueSize 78
#define LX200_Cmd_Array_Size 16
#define LX200_CMD_STR_SIZE 3

typedef enum
{
	NoAck,
	Ack,
	AlignAltaz,
	AlignLand,
	AlignPolar,
	GetDistance,
   ReticleCommand,
	FocusOut,
	FocusIn,
	FocusQuit,
	FocusSetFast,
	FocusSetSlow,
	GetRa,
	GetDec,
	GetAlt,
	GetAz,
	GetSidT,
	GetLocalT24,
	GetLocalT12,
   GetSiteName,
	GetMinQualityFind,
	GetDate,
	GetClockStatus,
	GetLat,
	GetLongitude,
	GetTz,
	GetField,
	TimeQuartz,
   LI_command,
	SetNGCLibrary,
   SetStarLibrary,
	MoveDirRateNorth,
	MoveDirRateSouth,
	MoveDirRateEast,
	MoveDirRateWest,
	StartSlew,
	StopSlew,
	StopMotionNorth,
	StopMotionSouth,
	StopMotionEast,
	StopMotionWest,
	SetMotionRateGuide,
	SetMotionRateCenter,
	SetMotionRateFind,
	SetMotionRateSlew,
	SetRa,
	SetDec,
	SetField,
	SetCurrentHigherLimit,
	SetSidT,
	SetLocalT,
	SetDate,
	SetGMTOffset,
	SetSiteNumber_S,
	SetLat,
	SetLongitude,
	SetBrightMagLimitFindOperation,
	SetFaintMagLimitFindOperation,
	LargeSizeLimitFindOperation,
	SmallSizeLimitFindOperation,
	Sw_command,
	NextMinQualityFind,
	SetTypeStringForFind,
	Sync,
	SetSiteNumber_W,
	ToggleLongFormat,
	GetFirmwareIDString,
	ReadGuideArcsecSec,
	SendGuideArcsecSec,
	SetHandpadMode,
	HandpadLeftKey,
	HandpadRightKey,
	ReadMsArcsecSec,
	SendMsArcsecSec,
   LX200_Clear_Display,
	LX200_Unfinished_Cmd,
	LX200_Unknown_Cmd0,
	LX200_Unknown_Cmd1,
	LX200_Unknown_Cmd2,
	LX200_Ignored_Cmd,
	MaxLX200_COMMAND_TYPE
}LX200_COMMAND_TYPE;

struct Position In;

/* 2 character descriptions of command names */
char LX200_Cmd_Str[MaxLX200_COMMAND_TYPE][LX200_CMD_STR_SIZE];
LX200_COMMAND_TYPE LX200_Motor_Cmd, LX200_Speed_Cmd;
LX200_COMMAND_TYPE LX200_Focus_Cmd, LX200_Focus_Speed_Cmd;
int LX200_Cmd_Array_Ix;
/* array of commands */
LX200_COMMAND_TYPE LX200_Cmd_Array[LX200_Cmd_Array_Size];
/* circular queue that LX200 protocol serial input is read into */
char LX200Queue[LX200QueueSize];
int LX200_Ix, Beg_LX200_Ix;
/* count of commands processed */
int LX200_Cmd_Count;
Flag LX200_OpenEndedSlewOn;
struct AZLong LX200SlewSteps;
struct Position SlewCurrent;
Flag SetCurrentToSlew;
int LX200MinQualityFind;
Flag LX200LeftButton;
Flag LX200RightButton;

const enum
{
	MenuQuit,
	MenuSite,
	MenuDataFile,
	MenuDataFileClosest,
	MenuDataFileClosestNotSame,
	MenuSearchDataFiles,
	MenuGrandTourClosest,
	MenuScrollTour,
	MenuGuide,
	MenuHotkeys,
	MenuHandpad,
	MenuSelectHandpadMode,
	MenuReverseUpDownButtons,
	MenuTrack,
	MenuMsSpeed,
	MenuGuideSpeed,
	MenuFocusFastSpeed,
	MenuFocusSlowSpeed,
	MenuHandpadLeft,
	MenuHandpadRight,
	MenuReadSlew,
	MenuWriteSlew,
	MenuDecMotor,
	MenuChangeBacklashParms,
	MenuAutoGEMFlipOnOff,
	MenuLX200Control,
	MenuDisplayInit,
	MenuInputEquat,
	MenuOffsetEquat,
	MenuSav1,
	MenuRes1,
	MenuSav2,
	MenuRes2,
	MenuRestoreLastObject,
	MenuHomeCoord,
	MenuMoveEquat,
	MenuMoveHome,
	MenuMoveGEMFlip,
	MenuMoveFocus,
	MenuResetFocus,
	MenuResetEquat,
	MenuResetHome,
	MenuGEMMeridianFlip,
	MaxMenu
};

typedef enum
{
	LX200Initiated,
	HandpadInitiated,
	MouseInitiated,
	KeyboardInitiated,
	ScrollInitiated,
	NotInitiated
} INITIATED_TYPE;

INITIATED_TYPE InitiatedType;

void InitLX200Input( void);
void InsertLX200CmdIntoArray( const LX200_COMMAND_TYPE LX200_Cmd_To_Add);
void ReadLX200Input( void);
void LX200_Write_Ra( void);
void LX200_Write_Dec( void);
void LX200_Write_Alt( void);
void LX200_Write_Az( void);
void LX200_Write_SidT( void);
void LX200_Write_LocalT( void);
void LX200_GetSiteName( void);
void LX200_Write_MinQualityFind( void);
void LX200_Write_Date( void);
void LX200_Write_Clock_Status( void);
void LX200_Write_Lat( void);
void LX200_Write_Long( void);
void LX200_Write_Tz( void);
void LX200_Write_Field( void);
void LX200_GetDistance( void);
void LX200_GetFirmwareIDString( void);
Flag CharIsNumeric( int IxQ);
int LX200_Read_Int_From_Queue( int IxQ);
void LX200_Incr_Ix_Until_Terminate_Char( int* Ix);
void LX200_Set_IncrIx( const int Ix, int *IncrIx);
void LX200_Read_Ra( const int Ix);
void LX200_Read_Dec( const int Ix);
void LX200_Read_SidT( const int Ix);
void LX200_Read_LocalT( const int Ix);
void LX200_Read_CurrentDate( const int Ix);
void ProcessLX200Cmds( void);
void AbortLX200_IRQ8Slew( void);
void ProcessLX200_Motor_Cmd( void);
void ProcessLX200FocusByMethod( void);
void ProcessLX200Focus_OnOff_16_17( void);
void ProcessLX200Focus_OnOff_16_17_Slow1_14( void);

/* handpad event... */

#define LastDirSize 6
/* 5 sec delay */
#define StartHandpadTimer 91

#define RecordEquatFile "RECORD.DAT"
#define AnalysisFile "ANALYSIS.DAT"

typedef enum
{
	HandpadOff,
	GuideStayOn,
	GrandTour,
	ScrollTour,
	ScrollAutoTour,
	ToggleTrack,
	HandpadFocus,
	HandpadAux,
	MaxHandpadFlag
}HANDPAD_FLAG;

HANDPAD_FLAG HandpadFlag;

typedef enum
{
	Aux1,
	Aux14,
	Aux16,
	Aux17,
	AuxOff
}AUX_CONTROL;

AUX_CONTROL AuxControl;

/* tracking to current Ra, Dec stopped when flag == false; */
/* altaz mvmt initiated by handpad allowed regardless of flag */
Flag TrackFlag;
/* non-zero backlash in either Alt or Az */
Flag BacklashFlag;
/* if negative backlash: motor moves in opposite direction of motion */
struct AZFlag NegBacklash;
/* check for motor direction oscillation caused by backlash updating */
Flag CheckBacklashOscillation;
/* grand tour on */
Flag GrandTourFlag;
/* scroll tour on */
Flag ScrollFlag;
/* = 1 if in handpadaux mode, a directional button has been pressed */
Flag HandpadAuxButtonPressed;
/* signifies that a directional button has been pressed while handpad is in focus mode */
Flag HandpadFocusButtonPressed;
int HandpadButtonTimer;
int RecordEquatTimer;
Flag DisplayAltAutoSynchFlag;
Flag DisplayAzAutoSynchFlag;
Flag HandpadAuxControlOnFlag;

/* variables relating to focusing via stepper motor... */

typedef enum
{
	FocusPlus,
	FocusMinus,
	FocusStop
} FOCUS_CONTROL;

FOCUS_CONTROL FocusControl;

/* holds parallel port pin assignment for focus motor direction control */
Byte FocusDir;
/* holds parallel port pin assignment for focus motor step control */
Byte FocusPulseBit;
/* flag indicating that the field rotation motor step control line has been raised */
Flag FocusPulse;
/* value or state of the pair of unused parallel port lines: to be presevered while outputting the
focus motor step and direction control lines */
Byte UnusedFocusPPortLines;
/* true if MoveMs() says to pulse the focus motor */
Flag PulseFocus;
int FocusDiagPulseHighCW;
int FocusDiagPulseHighCCW;
int FocusDiagPulseLow;
Flag HandpadFocusMotorControlOnFlag;
double FocusStepsToMove;
double FocusStepIncrPerPWM;
double FocusStepsDone;
double HoldFocusSteps;

/* counts of consecutive slews when scope should be microstepping tracking...
if counts reach max values defined in the config file, then scope resets current equatorial to
current altazimuth */
int ConsecutiveSlews;

/* move initiated by MoveToCurrentRaDec() */
Flag MoveToCurrentRaDecFlag;
Flag GEMflippedFlag;
double AutoGEMFlipFuzzRad;

/* in radians */
struct AZDouble Backlash;
/* direction that backlash has been taken up, or reduced to zero, in */
struct AZFlag BacklashDir;
/* steps to move to take up backlash */
struct AZLong BacklashMs;
struct AZDouble HoldMsTick;
/* max # of microsteps per clock tick */
double MaxMsTick;
struct AZDouble Delta;
struct Position Offset;
Flag InsideProcessHPEventsFocusMethod_Pulse_Dir;

void InitHPEvent( void);
void CalcBacklashVars( void);
void SetCurrentAltazToAccumMs( void);
void SetAccumMsToCurrentAltaz( void);
void HPEventMoveHs( void);
void HPEventMoveMs( void);
void AltazMoveMs( void);
Flag MoveBacklash( void);
void GetUnusedPPortLines( void);
void SetFocusControlLines( void);
void ReverseFocusDir( void);
void SetFocusStepsDirMoveMs( void);
void SetPulseFRFocusFRInMoveMs( void);
void PulseFRFocusPerPWM( void);
void CheckSiderostatAltaz( void);
void ProcessLimitError( const char* Msg);
void ProcessGEMFlippedFlag( void);
void MoveToCurrentRaDec( void);
Flag SetDirDistanceStepsThenMove( void);
void HPEventGetEquat( void);
void HPEventInitMatrix( const int Init);
void RecordEquat( void);
Flag IACA_Event( void);
void CheckLX200Events( void);
void CheckMiscEvents( void);
void ProcessHPEventsGuide( void);
void ProcessHPEventsFocusMethod_Pulse_Dir( void);
void ProcessHPEventsHandpadAux( void);
void AuxControlBiDirPPort( void);
void ProcessHPEventsFocus_OnOff_16_17( void);
void FocusControl_OnOff_16_17( void);
void ProcessHPEventsFocus_OnOff_16_17_Slow1_14( void);
void FocusControl_OnOff_16_17_Slow1_14( void);
void ProcessHPEventsMoveMotors( void);
void ProcessHPEventsModeSwitch( void);
void ProcessHPEvents( void);

/* keyboard event... */

#define ALTITUDE 0
#define AZIMUTH 1
#define MaxNameArray 24
#define MaxCoord 24
#define MaxMainStr 14
#define GuideStartupMarFile "STARTUP.MAR"
#define ScopeStartupMarFile "SCOPE.MAR"
#define SlewFile "SLEW.DAT"
#define SlewOutFile "SLEW_OUT.DAT"
#define OutGuideFile "OUTGUIDE.DAT"
#define NotAvailableStr "N/A    "
#define DataFile "*.dat"
#define ScrollFile "*.scr"
/* # of menu categories to display from left to right in a single row */
#define MaxMenuCats 7
/* # of menu subtitles to display in a single row */
#define MaxMenuSubs 5
/* # of rows to use to display menu subtitles */
#define MaxMenuDisplayRows 3
/* MaxMenuSubs is # of display columns of menu subtitles, so make total # of displayed subtitles
this number times the max number of display rows */
#define MaxSubTitles (MaxMenuSubs * MaxMenuDisplayRows)
/* colors used in graphing analysis, etc */
#define gridcolor RED
#define altcolor LIGHTGREEN
#define azcolor LIGHTBLUE
#define highlightcolor WHITE
/* graph scale factor: pixels per degree */
#define pixelsdeg 1.
#define Sprintf2Blanks (sprintf( StrBuf, "  "))
#define SprintfOn (sprintf( StrBuf, "On"))
#define FilenameSet 1
#define FilenameNotSet 0

#define DisplayAux1Bit  4
#define DisplayAux14Bit 8
#define DisplayAux16Bit 16
#define DisplayAux17Bit 32
#define DisplayFocusInBit 64
#define DisplayFocusOutBit 128

int DisplayBiDirOut;
int HoldDisplayBiDirOut;

char GuideStartupMarFilePtr[NameSize];
char ScopeStartupMarFilePtr[NameSize];
char* HPStr[MaxHandpadFlag];

/* each main menu category has a number of associated menu subtitles;
when a main menu category is selected, its menu subtitles are displayed */

struct SubTitle
{
	char* Title;
	char* Desc;
	int MenuItem;
};

/* main menu categories displayed in a row from left to right */
struct MenuCat
{
	char* Title;
	int NumSubTitles;
	struct SubTitle SubTitles[MaxSubTitles];
};

struct MenuCat MenuArray[MaxMenuCats];

/* starting X values for display of menu categories */
int MenuCatX[MaxMenuCats];
/* starting X values for display of menu subtitles; if number of subtitles exceed number of display
columns, then subtitles displayed in next row, repeating the X values for the columns */
int MenuSubX[MaxMenuSubs];

struct Frame
{
	int Left;
	int Top;
	int Right;
	int Bottom;
	char* Title;
};

struct Object
{
	char Name[ObjectNameSize];
	struct HMSH RaHMSH;
	struct DMS DecDMS;
};

struct FilenameString
{
	char Name[FilenameSize];
	FNAME_TYPE FNameType;
};

typedef enum
{
	ExactInputFieldsMatchOK,
	AvoidMatchInputFields,
	DoNotMatchInputFile
}INIT_MATCH_LIST;

INIT_MATCH_LIST InitMatchList;

/* MenuColor and DefaultBackground */
int MenuText;
/* SelectColor and SelectBackground */
int SelectText;
/* CurrentColor and DefaultBackground */
int CurrentText;
/* DisplayColor and DefaultBackground */
int DisplayText;
/* DefaultColor and DefaultBackground */
int DefaultText;
/* BorderColor and DefaultBackground */
int BorderText;
/* TitleColor and DefaultBackground */
int TitleText;
/* currently selected menu category and subtitle in that category */
int CurrentMenuCat;
int CurrentMenuSub;
/* newly selected menu category and subtitle in that category */
int NewMenuCat;
int NewMenuSub;
Flag FocusFastDisplayFlag;
/* if move exceeds this, then confirm move */
long MoveHsMsgSteps;
int HoldLX200ComPort;
Flag LX200DisplayFlag;
Flag ValidResponseFlag;
Flag GrandTourLoaded;
Flag ScrollLoaded;
Flag CurrentScrollFlag;
Flag AutoScrollFlag;
Flag ScrollUnderway;
Flag AutoScrollAlertFlag;
Flag DataFileNameSet;
Flag MsParmsAllowedFlag;
Flag QuitFlag;
int SpawnReturn;
int GrandTourRecNum;
char FullyQualifiedSlewFile[NameSize];
char FullyQualifiedSlewOutFile[NameSize];
char DefaultDir[MAXPATH];
char DataDir[MAXPATH];
struct Position SavedIn, SavedIn2, LastDataObject;
char Filename[NameSize];
char GrandTourFilename[NameSize];
char ScrollFilename[NameSize];
FILE* OutGuideFilePtr;
/* used to start and end lines in graphing */
int Abegin, Aend, Adif;
int Zbegin, Zend, Zdif;
double errscale;
/* keeps track of encoder, high priority, and low priority tasks: used to ascertain which task to
execute next time the function is called */
int ETask;
int HTask;
int LTask;
Flag KeyboardLeftButton;
Flag KeyboardRightButton;

/* means array of 5 char pointers to strings */
char* SlewText[5];
char* AbortText[10];

/* pts to file positions: allows goback to previous points in file */
fpos_t* FPosPtr;
#define MAX_FPOS_PTR 100
int FPosPtrIx;

typedef enum
{
	SlewOff,
	SlewReady,
	SlewUnderway,
	SlewFinished,
	SlewBeepOn,
	SlewBeepOff
} SLEW_BEEP;

SLEW_BEEP SlewBeep;
int SlewBeepSound;

struct Frame MsgFrame;
struct Frame CoordFrame;
struct Frame DataFileFrame;
struct Frame InitFrame;
struct Frame HotkeyFrame;
struct Frame HandpadFrame;
struct Frame MainFrame;

char* MainStr_9;
char* MainStr_10;
char* MainStr_11;
char* MainStr_12;
char* MainStr_13;
char* MainStr_14;
char* MainStr_16;
char* MainStr_17;
char* MainStr_18;
char* MainStr_19;
char* MainStr_20;
char* MainStr_21;
char* MainStr_22;
char* MainStr_23;

char* BadFocusMethodMsgStrPtr;

/* following section defines X, Y coordinates... */

/* reused X coordinates */
const enum
{
	MenuStartX = 1, MenuEndX = 79,
	MenuDescX = 5,
	DisplayInputRaX = 11, DisplayInputDecX = 26, DisplayFocusPositionX = 51,

	DisplayFileX = 8, DisplayScrollCountsX = 22, DisplayTrackX = 37, DisplayGEMFlipX = 58,

	DisplayObjectX = 8, DisplayHandpadX = 39, DisplayButtonsX = 69,
	DisplaySidTX = 17, DisplayMouseModeX = 37, DisplayMsSpeedX = 54, DisplayGuideX = 70,
	DisplayDateTimeX = 11, DisplayFocusFastX = 41, DisplayFocusSlowX = 61, DisplayFRFocusAuxActionX = 69,
	DisplayPWMX = 35, DisplayMsStatusX = 45,

	DisplaySlewX = 7,
	DisplayLX200X = 7,
	DisplayLX200X2 = 1
};

/* reused Y coordinates; top line = 0 */
const enum
{
	MenuCatsY = 1,
	MenuSubLine1Y = 2,
	MenuSubMenuY = 3,
	MenuDescY = 6,
	MenuSubLine2Y = 8,
	DisplayCurrentY = 9,
	MenuSubLine3Y = 14,
	Display2MotorTrackY = 14,
	DisplayInputY = 15,
	DisplayFocusPositionY = 15,
	DisplayFileY = 17, DisplayScrollCountsY = 17, DisplayTrackY = 17, DisplayInitY = 17, DisplayGEMFlipY = 17,
	DisplayObjectY = 18, DisplayHandpadY = 18, DisplayButtonsY = 18,
	DisplaySidTY = 19, DisplayMouseModeY = 19, DisplayMsSpeedY = 19, DisplayGuideY = 19,
	DisplayDateTimeY = 20, DisplayFocusY = 20,
	DisplayPWMY = 21, DisplayMsStatusY = 21,

	DisplaySlewY = 22,
	DisplayLX200Y = 23,
	DisplayLX200Y2 = 24
};

/* named indexes into DisplayXY[] */
const enum
{
	DisplayInputRa, DisplayInputDec, DisplayFocusPosition,

	DisplayTrackByRate, DisplayGEMFlipMove,

	DisplayFile, DisplayScrollCounts, DisplayTrack, DisplayGEMFlip,

	DisplayObject, DisplayHandpad, DisplayButtons,
	DisplaySidT, DisplayMouseMode, DisplayMsSpeed, DisplayGuideSpeed,
	DisplayDateTime, DisplayFocusFast, DisplayFocusSlow, DisplayFRFocusAuxAction,
	DisplayPWMReps, DisplayMsStatus,
	DisplayHsSlew, DisplayHsAbort, DisplayHsSteps,
	DisplayLX200,
	DisplayLX200_2,

	MaxDisplay
};

struct XY DisplayXY[MaxDisplay];

/* following are in kbmain.c */
void InitKBEvent( void);
void SetAttributes( void);
void InitDisplayXY( void);
void CalcHsMsgSteps( void);
void CloseKBEvent( void);
void PrelimMsg( void);
void ClearMenuArea( void);
void ClearMenuSubArea( void);
void DrawScreenLine( const int Y);
void WriteMainMenu( void);
void WriteMenuCats( void);
void SelectTextAttrMenuCat( const int MenuCat);
void DefaultTextAttrMenuCat( const int MenuCat);
void ChangeTextAttrMenuCat( const int MenuChoice);
void WriteSubMenus( void);
void WriteSubMenuDesc( const int MenuCat, const int SubMenu);
void SelectTextAttrMenuSub( const int MenuSub);
void MenuTextAttrMenuSub( const int MenuSub);
void ChangeTextAttrMenuSub( const int MenuChoice);
void UpdateMenuCatSub( void);
void DrawBorder( const struct Frame P);
void WriteWindow( struct Frame P);
void RemoveWindow( struct Frame P);
void PressKeyToContMsg( const char* Msg);
void SetNewMenuCatSub( void);
void GetNewMenuCatSubFromResponse( void);
void ProcessKBEvents( void);
void ProcessVirtualHandpad( void);
void ProcessMenuResponse( void);
Flag ConfirmHsMove( void);
void KBEventMoveHs( void);
void KBEventInitMatrix( const int Init);
void DisplayJumboRa( struct Position* P);
void DisplayJumboDec( struct Position* P);
void SequentialHiPriController( void);
void SequentialLowPriController( void);
void SequentialTaskController( void);
void CheckSlewBeep( void);
Flag WriteGuideStartup( void);
void ReadGuideStartup( void);
void ClosestInit( void);
void SortPosition( struct Position* A, struct Position* B, const double Diff);
void BubbleSortPositions( struct Position* Temp);

/* following are in kbdfile.c */
void GetDir( void);
void GetFileName( char* Name);
Flag IsProgramFile( char* Name);
Flag SelectDataFilename( char* Name);
void MenuCoordFile( void);
void ObjectNameTextAttr( const struct Frame F, const int Length, const int Select, const int TextAttr);
void ReadRa( FILE* File, struct HMSH* V);
void ReadDec( FILE* File, struct DMS* V);
Flag FillObjects( struct Object O[], int* Count);
void ProcessMenuRestoreLastObject( void);
Flag ProcessMenuDataFileClosest( Flag FilenameSetFlag, Flag HowToMatchFlag);
void RemoveBlanks( char* s);
Flag FindStr( const char* str, char* fname, int* Rec);
Flag FindFile( const char* Path, const char* Search, char* File, int* Rec);
Flag ProcessMenuSearch( void);
void DisplayIn( char* FileName, char* ObjectName);
void LoadScrollFile( void);
void DisplayScrollCountsOnScreen( void);
void ReadScrollEquatCoord( struct SCROLL* Scroll);
void ProcessScroll( void);
void SetInputToScroll( struct SCROLL* Scroll);
void GetGrandTourRec( void);
void GetGrandTourRecClosestCurrentEquat( void);
void ProcessGrandTour( void);
void Move_Update_Handpad_Subr( void);

/* following are in kbguide.c */
Flag WriteGuideStartup( void);
void ReadGuideStartup( void);

/* following are in kbinproc.c */
void DisplayInit( void);
void ProcessMenuSetZ1Z2Z3( void);

/* following are in kbproc.c */
void WriteFocusPosition( void);
void WriteHandpadStatus( void);
void WriteTrackByRate( void);
void WriteTrackStatus( void);
void WritePECStatus( void);
void WriteMsArcsecSec( void);
void WriteGuideArcsecSec( void);
void WriteFocusFastSpeed( void);
void WriteFocusSlowSpeed( void);
Flag InputEquat( struct Position* P, int X, int Y);
void DisplayLX200CommandsAndCharBuffer( void);
void DisplayButtonsStatus( void);
void DisplayFRFocusAuxActionStatus( void);
void DisplayHotkeys( void);
Flag EnterHsParms( void);
void ProcessMenuMoveGEMFlip( void);
void ProcessMenuMoveGEMFlipCancel( void);
void ProcessMenuFocusFastSpeed( void);
void ProcessMenuFocusSlowSpeed( void);
void ProcessMenuMoveFocus( void);
void ProcessMenuResetFocus( void);
void ProcessSav1( void);
void ProcessRes1( void);
void ProcessSav2( void);
void ProcessRes2( void);
void ProcessMenuDecMotor( void);
void ProcessMenuQuit( void);
void ProcessMenuSite( void);
void ProcessMenuGuide( void);
void ClearMsgFrameLine( const int Y);
void ProcessMenuHandpad( void);
void ProcessMenuSelectHandpadMode( void);
void ProcessMenuHandpadLeft( void);
void ProcessMenuHandpadRight( void);
void ProcessMenuReverseUpDownButtons( void);
void ProcessMenuTrack( void);
void ProcessMenuMsSpeed( void);
void ProcessMenuGuideSpeed( void);
void ProcessMenuLX200Control( void);
void BlankOutLX200DisplayAreas( void);
void DisplayLX200CommandStatus( void);
void ProcessMenuChangeBacklashParms( void);
void ProcessMenuAutoGEMFlipOnOff( void);
void ProcessMenuInputEquat( void);
void ProcessMenuOffsetEquat( void);
void ProcessMenuMoveEquat( void);
void ProcessMenuMoveHome( void);
void ProcessMenuResetEquat( void);
void ProcessMenuResetHome( void);
void DisplayGEMFlipStatus( void);
void ProcessMenuGEMMeridianFlip( void);
void ProcessMenuHomeCoord( void);
void ProcessMenuSelection( void);

/* mouse event */

typedef enum
{
	MouseMovedUp,
	MouseMovedDown,
	MouseMovedRight,
	MouseMovedLeft,
	MouseMovedOff
}MOUSE_MOVE_RESULT;

MOUSE_MOVE_RESULT MouseMoveResult;

typedef enum
{
	MouseModeMenu,
	MouseModeHs,
	MouseModeMs,
	MouseModeFocus,
	MouseModeAux,
	MaxMouseMode
}MOUSE_MODE;

MOUSE_MODE MouseMode;

struct AZInt StartMouseXY;

int MouseLeftTimer;
int MouseRightTimer;
Flag SlewStartedFromMouse;
Flag MouseModeAuxPinActive;

void InitMouseControl( void);
void CloseMouseControl( void);
Flag ProcessMouseEvent( void);
Flag ProcessMouseEventMenu( void);
void ProcessMouseModeHsMs( void);
void ProcessMouseModeAux( void);
Flag ProcessMouseModeFocusByMethod( void);
void ProcessMouseFocus_OnOff_16_17( void);
void ProcessMouseFocus_OnOff_16_17_Slow1_14( void);
void WriteMouseMode( void);

/* main, or scope.c */

Flag StartScrollFlag;
void BadExit( const char* Msg);

#endif
