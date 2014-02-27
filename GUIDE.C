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

/* Guide Area... */

void InitGuide( void)
{
	GuideFlag = Yes;
	GuideRadTick = GuideArcsecSec*ArcsecToRad/ClockTicksSec;
	AccumGuide.A = AccumGuide.Z = 0;
}

void StopGuide( void)
{
	GuideFlag = Off;
	AccumGuide.A = AccumGuide.Z = 0;
}


