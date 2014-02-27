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

/* advanced convert routines */

double CalcEquatAngularSep( struct Position* A, struct Position* Z)
{
	double angsep;
	double AHA, ZHA, HADiff;

	/* hour angles */
	AHA = A->SidT - A->Ra;
	ZHA = Z->SidT - Z->Ra;
	HADiff = AHA - ZHA;

	angsep =  acos( sin( A->Dec) * sin( Z->Dec)
	+ cos( A->Dec) * cos( Z->Dec) * cos( HADiff));

	return angsep;
}

double CalcAltazAngularSep( struct Position* A, struct Position* Z)
{
	double angsep;
	double AzDiff;

	AzDiff = A->Az - Z->Az;
	angsep = acos( sin( A->Alt) * sin( Z->Alt)
	+ cos( A->Alt) * cos( Z->Alt) * cos( AzDiff));

	return angsep;
}

double AngSepDiff( struct Position* A, struct Position* Z)
{
	double Diff;

	Diff = fabs( CalcEquatAngularSep( A, Z)) - fabs( CalcAltazAngularSep( A, Z));

	if( Diff < 0)
		Diff = -Diff;
	return Diff;
}


