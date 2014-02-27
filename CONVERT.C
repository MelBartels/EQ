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

/* convert... */

/* if re-init'ing for GEM meridian flip, keep RA&Dec&SidT the same, but add 180 deg to az and
reflect alt across pole, ie, alt = 180-alt, ie, if alt = 45, then reflected alt = 135;
Z1, Z2, Z3 reversed for GEM meridian flip because tube rotates 180 degrees */

void InitConvert( void)
{
	GEMflippedFlag = No;

	/* set mount errors from the config.dat file */
	SetMountErrorsDeg( Z1Deg, Z2Deg, Z3Deg);

	/* set hold, hold sin, and hold cos values of Current.Dec */
	HoldCurrentDec = -9999;
	CheckHoldSinCosCurrentDec();

	if( DisplayOpeningMsgs)
		printf( "\n*** adopting an EQUATORIAL alignment ***");

	InitConvertEquat();
}

void InitConvertEquat( void)
{
	double Ra;
	struct Position Temp;

	if( LatitudeDeg >= 0)
	{
		/* point south on celestian equator on meridian */
		/* Az of zero == meridian to the south; on meridian, Ra == SidT */
		SetCoordDeg( &One, Current.SidT*RadToDeg, 0, 0, 0, Current.SidT*RadToDeg);
		/* point west on celestial equator */
		Ra = One.Ra - QtrRev;
		if( Ra < 0)
			Ra += OneRev;
		SetCoordDeg( &Two, Ra*RadToDeg, 45, 45, 90, One.SidT*RadToDeg);
	}
	else
	{
		/* southern hemisphere */
		/* point north on celestian equator on meridian */
		/* Az of zero == meridian to the north; on meridian, Ra == SidT */
		SetCoordDeg( &One, Current.SidT*RadToDeg, 0, 0, 0, Current.SidT*RadToDeg);
		/* point west on celestial equator */
		Ra = One.Ra - QtrRev;
		if( Ra < 0)
			Ra += OneRev;
		SetCoordDeg( &Two, Ra*RadToDeg, -45, 45, 270, One.SidT*RadToDeg);
	}

	One.Init = Two.Init = Yes;
	Three.Init = No;
	Temp = Current;
	Current = One;
	InitMatrix( 1);
	Current = Temp;
	if( !Siderostat && (LatitudeDeg > 0 && Current.Alt > QtrRev || LatitudeDeg < 0 && Current.Alt < -QtrRev))
		GEMflippedFlag = Yes;
}

void InitConvertAltaz( void)
{
	struct Position Temp;

	/* point to pole */
	SetCoordDeg( &One, 0, QtrRev*RadToDeg, LatitudeDeg, 0, Current.SidT*RadToDeg);

	/* point on celestial equator on meridian */
	SetCoordDeg( &Two, Current.SidT*RadToDeg, 0, 90-LatitudeDeg, HalfRev*RadToDeg,
	Current.SidT*RadToDeg);

	One.Init = Two.Init = Yes;
	Three.Init = No;
	Temp = Current;
	Current = One;
	InitMatrix( 1);
	Current = Temp;
}

void FReadPosition( FILE* File, struct Position* Pos)
{
	FReadDouble( File, &Pos->Ra);
	Pos->Ra /= RadToDeg;
	FReadDouble( File, &Pos->Dec);
	Pos->Dec /= RadToDeg;
	FReadDouble( File, &Pos->Alt);
	Pos->Alt /= RadToDeg;
	FReadDouble( File, &Pos->Az);
	Pos->Az /= RadToDeg;
	FReadDouble( File, &Pos->SidT);
	Pos->SidT /= RadToDeg;
	if( !feof( File))
		Pos->Init = Yes;
}

void FReadPositionToNewLine( FILE* File, struct Position* Pos)
{
	FReadPosition( File, Pos);
	FReadToNewLine( File);
}

void FWritePosition( FILE* File, struct Position* Pos, Flag CRLF)
{
	fprintf( File, "%f ", Pos->Ra * RadToDeg);
	fprintf( File, "%f ", Pos->Dec * RadToDeg);
	fprintf( File, "%f ", Pos->Alt * RadToDeg);
	fprintf( File, "%f ", Pos->Az * RadToDeg);
	fprintf( File, "%f ", Pos->SidT * RadToDeg);
	if( CRLF)
		fprintf( File, "\n");
}

Flag LoadAlign( void)
{
	/* temporary coordinates */
	struct Position Temp;

	if( One.Init)
	{
		/* local Temp */
		Temp = Current;
		Current = One;
		InitMatrix( 1);
		Current = Temp;
	}
	if( One.Init && Two.Init)
		return True;
	else
		return False;
}

void SetMountErrorsDeg( const double z1Deg, const double z2Deg, const double z3Deg)
{
	Z1Deg = z1Deg;
	Z2Deg = z2Deg;
	Z3Deg = z3Deg;
	Z1 = z1Deg*DegToRad;
	Z2 = z2Deg*DegToRad;
	Z3 = z3Deg*DegToRad;
	if( Z1 != 0 || Z2 != 0)
		Z1Z2NonZeroFlag = Yes;
	else
		Z1Z2NonZeroFlag = No;
}

void ZeroArrays( void)
{
	int I, J;

	for( I = 0; I < 4; I++)
		for( J = 0; J < 4; J++)
			QQ[I][J] = VV[I][J] = RR[I][J] = XX[I][J] = YY[I][J] = 0;
}

void CheckHoldSinCosCurrentDec( void)
{
	if( HoldCurrentDec != Current.Dec)
	{
		HoldCosCurrentDec = cos( Current.Dec);
		HoldSinCurrentDec = sin( Current.Dec);
		HoldCurrentDec = Current.Dec;
	}
}

void ArrayAsignInit( const int Init)
{
	double B;

	if( Init == 1)
		ZeroArrays();

	CheckHoldSinCosCurrentDec();

	/* B is CCW so HA formula backwards */
	B = Current.Ra - Current.SidT;
	/* XX is telescope matrix; convert parameters into rectangular (cartesian) coordinates */
	XX[1][Init] = HoldCosCurrentDec * cos( B);
	XX[2][Init] = HoldCosCurrentDec * sin( B);
	XX[3][Init] = HoldSinCurrentDec;
	/* F is CCW */
	F = OneRev - Current.Az;
	if( GEMflippedFlag)
		H = Current.Alt - Z3;
	else
		H = Current.Alt + Z3;
	SubrA();
	/* YY is celestial matrix; convert parameters into rectangular (cartesian) coordinates  */
	YY[1][Init] = YY[1][0];
	YY[2][Init] = YY[2][0];
	YY[3][Init] = YY[3][0];
}

void GenerateThirdInit( void)
{
	int I;
	double A;

	/* generate 3rd initialization point from the first two using vector product formula */
	XX[1][3] = XX[2][1]*XX[3][2] - XX[3][1]*XX[2][2];
	XX[2][3] = XX[3][1]*XX[1][2] - XX[1][1]*XX[3][2];
	XX[3][3] = XX[1][1]*XX[2][2] - XX[2][1]*XX[1][2];
	A = sqrt( XX[1][3]*XX[1][3] + XX[2][3]*XX[2][3] + XX[3][3]*XX[3][3]);
	for( I = 1; I <= 3; I++)
	{
		if( A == 0)
			XX[I][3] = MAXDOUBLE;
		else
			XX[I][3] /= A;
	}
	YY[1][3] = YY[2][1]*YY[3][2] - YY[3][1]*YY[2][2];
	YY[2][3] = YY[3][1]*YY[1][2] - YY[1][1]*YY[3][2];
	YY[3][3] = YY[1][1]*YY[2][2] - YY[2][1]*YY[1][2];
	A = sqrt( YY[1][3]*YY[1][3] + YY[2][3]*YY[2][3] + YY[3][3]*YY[3][3]);
	for( I = 1; I <= 3; I++)
	{
		if( A == 0)
			YY[I][3] = MAXDOUBLE;
		else
			YY[I][3] /= A;
	}
}

void TransformMatrix( void)
{
	int I, J, L, M, N;
	double E;

	for( I = 1; I <= 3; I++)
		for( J = 1; J <= 3; J++)
			VV[I][J] = XX[I][J];
	/* get determinate from copied into array VV */
	DeterminateSubr();
	/* save it */
	E = W;
	for( M = 1; M <= 3; M++)
	{
		for( I = 1; I <= 3; I++)
			for( J = 1; J <= 3; J++)
				VV[I][J] = XX[I][J];
		for( N = 1; N <= 3; N++)
		{
			VV[1][M] = 0;
			VV[2][M] = 0;
			VV[3][M] = 0;
			VV[N][M] = 1;
			DeterminateSubr();
			if( E == 0)
				QQ[M][N] = MAXDOUBLE;
			else
				QQ[M][N] = W/E;
		}
	}
	for( I = 1; I <= 3; I++)
		for( J = 1; J <= 3; J++)
			RR[I][J] = 0;
	for( I = 1; I <= 3; I++)
		for( J = 1; J <= 3; J++)
			for( L = 1; L <= 3; L++)
				RR[I][J] += (YY[I][L] * QQ[L][J]);
	for( M = 1; M <= 3; M++)
	{
		for( I = 1; I <= 3; I++)
			for( J = 1; J <= 3; J++)
				VV[I][J] = RR[I][J];
		DeterminateSubr();
		E = W;
		for( N = 1; N <= 3; N++)
		{
			VV[1][M] = 0;
			VV[2][M] = 0;
			VV[3][M] = 0;
			VV[N][M] = 1;
			DeterminateSubr();
			if( E == 0)
				QQ[M][N] = MAXDOUBLE;
			else
				QQ[M][N] = W/E;
		}
	}
}

/* to use, put values to init into Current., then call InitMatrix( x) with x = desired Init;
function performs all possible inits from the beginning: for example, need only call InitMatrix( 1)
once to also init Two and Three */
void InitMatrix( const int Init)
{
	struct Position Temp;

	if( Init == 3 && One.Init && Two.Init)
	{
		Temp = Current;
		Current = One;
		ArrayAsignInit( 1);
		Current = Two;
		ArrayAsignInit( 2);
		Current = Temp;
		ArrayAsignInit( 3);
		TransformMatrix();
		CalcPostInitVars();
		Three = Current;
		Three.Init = Yes;
	}
	else if( Init == 2 && One.Init && Two.Init && Three.Init)
	{
		Temp = Current;
		Current = One;
		ArrayAsignInit( 1);
		Current = Temp;
		ArrayAsignInit( 2);
		Temp = Current;
		Current = Three;
		ArrayAsignInit( 3);
		Current = Temp;
		TransformMatrix();
		CalcPostInitVars();
		Two = Current;
		Two.Init = Yes;
	}
	else if( Init == 2 && One.Init && !Three.Init)
	{
		Temp = Current;
		Current = One;
		ArrayAsignInit( 1);
		Current = Temp;
		ArrayAsignInit( 2);
		GenerateThirdInit();
		TransformMatrix();
		CalcPostInitVars();
		Two = Current;
		Two.Init = Yes;
	}
	else if( Init == 1 && One.Init && Two.Init && Three.Init)
	{
		ArrayAsignInit( 1);
		Temp = Current;
		Current = Two;
		ArrayAsignInit( 2);
		Current = Three;
		ArrayAsignInit( 3);
		Current = Temp;
		TransformMatrix();
		CalcPostInitVars();
		One = Current;
		One.Init = Yes;
	}
	else if( Init == 1 && Two.Init && !Three.Init)
	{
		ArrayAsignInit( 1);
		Temp = Current;
		Current = Two;
		ArrayAsignInit( 2);
		Current = Temp;
		GenerateThirdInit();
		TransformMatrix();
		CalcPostInitVars();
		One = Current;
		One.Init = Yes;
	}
	else if( Init == 1 && !Two.Init)
	{
		ArrayAsignInit( 1);
		One = Current;
		One.Init = Yes;
	}
	else
	{
		sprintf( StrBuf, "InitMatrix() failure: Init=%d, One.Init=%d, Two.Init=%d, Three.Init=%d",
		Init, One.Init, Two.Init, Three.Init);
		BadExit( StrBuf);
	}
}

void SubrA( void)
{
	double CosF, CosH, CosZ1, CosZ2, SinF, SinH, SinZ1, SinZ2;

	CosF = cos( F);
	CosH = cos( H);
	SinF = sin( F);
	SinH = sin( H);
	if( Z1Z2NonZeroFlag)
	{
		if( GEMflippedFlag)
		{
			CosZ1 = cos( -Z1);
			CosZ2 = cos( -Z2);
			SinZ1 = sin( -Z1);
			SinZ2 = sin( -Z2);
		}
		else
		{
			CosZ1 = cos( Z1);
			CosZ2 = cos( Z2);
			SinZ1 = sin( Z1);
			SinZ2 = sin( Z2);
		}
		YY[1][0] = CosF*CosH*CosZ2 - SinF*CosZ1*SinZ2 + SinF*SinH*SinZ1*CosZ2;
		YY[2][0] = SinF*CosH*CosZ2 + CosF*SinZ2*CosZ1 - CosF*SinH*SinZ1*CosZ2;
		YY[3][0] = SinH*CosZ1*CosZ2 + SinZ1*SinZ2;
	}
	else
	{
		YY[1][0] = CosF*CosH;
		YY[2][0] = SinF*CosH;
		YY[3][0] = SinH;
	}
}

void SubrSwitcher( void)
{
	double CosF, CosH, SinF, SinH;
	double CosZ1, CosZ2, SinZ1, SinZ2;

	CosF = cos( F);
	CosH = cos( H);
	SinF = sin( F);
	SinH = sin( H);

	if( Z1Z2NonZeroFlag == False)
	{
		YY[1][1] = CosF*CosH;
		YY[2][1] = SinF*CosH;
		YY[3][1] = SinH;
	}
	else
	{
		if( GEMflippedFlag == True)
		{
			CosZ1 = cos( -Z1);
			CosZ2 = cos( -Z2);
			SinZ1 = sin( -Z1);
			SinZ2 = sin( -Z2);
		}
		else
		{
			CosZ1 = cos( Z1);
			CosZ2 = cos( Z2);
			SinZ1 = sin( Z1);
			SinZ2 = sin( Z2);
		}
		switch( SubrFlag)
		{
			case 'S':
				SubrS( CosF, CosH, SinF, SinH);
				break;
			case 'B':
				SubrB( CosF, CosH, SinF, SinH, CosZ1, CosZ2, SinZ1, SinZ2);
				break;
			case 'T':
				SubrT( CosF, CosH, SinF, SinH, CosZ1, CosZ2, SinZ1, SinZ2);
				break;
			case 'L':
				SubrL( CosF, CosH, SinF, SinH, CosZ1, CosZ2, SinZ1, SinZ2);
				break;
			case 'U':
				SubrU( CosF, CosH, SinF, SinH, CosZ1, CosZ2, SinZ1, SinZ2);
				break;
		  default:
				BadExit( "unknown SubrFlag in Convert.SubrSwitcher()");
		}
	}
}

// per Taki's eq 5.3-4
void SubrS( double CosF, double CosH, double SinF, double SinH)
{
	YY[1][1] = CosH*CosF + Z2*SinF - Z1*SinH*SinF;
	YY[2][1] = CosH*SinF - Z2*CosF - Z1*SinH*CosF;
	YY[3][1] = SinH;
}

// per Taki's eq 5.3-2
void SubrB( double CosF, double CosH, double SinF, double SinH, double CosZ1, double CosZ2, double SinZ1, double SinZ2)
{
	// 'H' is alt, 'F' is az
	YY[1][1] = (CosH*CosF + SinF*CosZ1*SinZ2 - SinH*SinF*SinZ1*CosZ2)/CosZ2;
	YY[2][1] = (CosH*SinF - CosF*CosZ1*SinZ2 + SinH*CosF*SinZ1*CosZ2)/CosZ2;
	YY[3][1] = (SinH - SinZ1*SinZ2)/(CosZ1*CosZ2);
}

// per Taki's eq 5.3-5/6 (2 loops sufficient for Z errors of 1 deg)
void SubrT( double CosF, double CosH, double SinF, double SinH, double CosZ1, double CosZ2, double SinZ1, double SinZ2)
{
	double CosF1, SinF1;
	int MaxLoopCount = 12;
	struct AZDouble Last;
	struct AZDouble Err;
	int SubrTCount = 0;

	Last.A = Last.Z = MAXDOUBLE;

	// start with best guess using Taki's 'subroutine B'
	SubrB( CosF, CosH, SinF, SinH, CosZ1, CosZ2, SinZ1, SinZ2);
	do
	{
		AngleSubr();

		Err.A = fabs( Last.A - H);
		Err.Z = fabs( Last.Z - F);
		Last.A = H;
		Last.Z = F;

		CosF1 = cos( F);
		SinF1 = sin( F);

		YY[1][1] = (CosH*CosF + SinF1*CosZ1*SinZ2 - (SinH-SinZ1*SinZ2)*SinF1*SinZ1/CosZ1)/CosZ2;
		YY[2][1] = (CosH*SinF - CosF1*CosZ1*SinZ2 + (SinH-SinZ1*SinZ2)*CosF1*SinZ1/CosZ1)/CosZ2;
		YY[3][1] = (SinH-SinZ1*SinZ2)/(CosZ1*CosZ2);

		SubrTCount++;
		if( SubrTCount > MaxLoopCount)
			BadExit( "too many loops in SubrT()");
	}while( Err.A > TenthsArcsecToRad ||  Err.Z > TenthsArcsecToRad);
}

// use apparent alt derivation from Larry Bell, apparent az from Taki's iterative solution
void SubrU( double CosF, double CosH, double SinF, double SinH, double CosZ1, double CosZ2, double SinZ1, double SinZ2)
{
	double ApparentAlt;

	ApparentAlt = GetApparentAlt( CosZ1, CosZ2, SinZ1, SinZ2);

	SubrT( CosF, CosH, SinF, SinH, CosZ1, CosZ2, SinZ1, SinZ2);
	AngleSubr();

	CosH = cos( ApparentAlt);
	SinH = sin( ApparentAlt);
	CosF = cos( F);
	SinF = sin( F);

	YY[1][1] = CosF*CosH;
	YY[2][1] = SinF*CosH;
	YY[3][1] = SinH;
}

/* per Larry Bell's derivation
	Z1 rotation done between alt and az rotations so no closed algebraic solution, instead, search iteratively
	'H' is alt, 'F' is az
	apparent coordinates are what the encoders see, and are our goal */

double GetApparentAlt( double CosZ1, double CosZ2, double SinZ1, double SinZ2)
{
	double V1;

	V1 = (sin(H)-SinZ1*SinZ2)*CosZ1*(CosZ2/((SinZ1*SinZ1-1)*(SinZ2*SinZ2-1)));
	if( V1 > 1.)
		V1 = 1.;
	if( V1 < -1.)
		V1 = -1.;
	return asin( V1);
}

void SubrL( double CosF, double CosH, double SinF, double SinH, double CosZ1, double CosZ2, double SinZ1, double SinZ2)
{
	double TrueAz, TanTrueAz;
	double ApparentAlt, ApparentAz, BestApparentAz;
	double EE, FF, GG, HH;
	double GoalSeek, HoldGoalSeek;
	double Incr, MinIncr;
	Flag Dir;
	int SubrLCount;

	TrueAz = F;
	TanTrueAz = tan( TrueAz);

	ApparentAlt = GetApparentAlt( CosZ1, CosZ2, SinZ1, SinZ2);

	EE = cos( ApparentAlt);
	FF = sin( ApparentAlt);
	GG = CosZ2*SinZ1*FF*TanTrueAz - TanTrueAz*SinZ2*CosZ1 - CosZ2*EE;
	HH = SinZ2*CosZ1 - CosZ2*SinZ1*FF - TanTrueAz*CosZ2*EE;

	// start with best guess using Taki's 'subroutine B' for ApparentAz
	SubrB( CosF, CosH, SinF, SinH, CosZ1, CosZ2, SinZ1, SinZ2);
	AngleSubr();
	ApparentAz = F;

	// iteratively solve for best apparent azimuth by searching for a goal of 0 for GoalSeek
	BestApparentAz = ApparentAz;
	HoldGoalSeek = MAXDOUBLE;
	Incr = ArcsecToRad*2.;
	MinIncr = ArcsecToRad;
	Dir = True;
	SubrLCount = 0;
	do
	{
		if( Dir == True)
			ApparentAz += Incr;
		else
			ApparentAz -= Incr;

		GoalSeek = GG*sin( ApparentAz)-HH*cos( ApparentAz);
		// System.out.println( "GoalSeek " + GoalSeek*1000000. + " Dir " + Dir);

		if( fabs( GoalSeek) <= fabs( HoldGoalSeek))
		{
			BestApparentAz = ApparentAz;
			// System.out.println( "BestApparentAz " + BestApparentAz*Units.RadToDeg);
		}
		else
		{
			// GoakSeek getting worse, so reverse direction and cut increment by half
			Incr /= 2.;
			Dir = !Dir;
		}
		HoldGoalSeek = GoalSeek;
		SubrLCount++;
	}while( Incr >= MinIncr);

	CosF = cos( BestApparentAz);
	SinF = sin( BestApparentAz);
	CosH = cos( ApparentAlt);
	SinH = sin( ApparentAlt);

	YY[1][1] = CosF*CosH;
	YY[2][1] = SinF*CosH;
	YY[3][1] = SinH;
}

void DeterminateSubr( void)
{
	W = VV[1][1]*VV[2][2]*VV[3][3] + VV[1][2]*VV[2][3]*VV[3][1]
	  + VV[1][3]*VV[3][2]*VV[2][1] - VV[1][3]*VV[2][2]*VV[3][1]
	  - VV[1][1]*VV[3][2]*VV[2][3] - VV[1][2]*VV[2][1]*VV[3][3];
}

void AngleSubr( void)
{
	double C;

	C = sqrt( YY[1][1]*YY[1][1] + YY[2][1]*YY[2][1]);
	if( C == 0 && YY[3][1] > 0)
		H = QtrRev;
	else if( C == 0 && YY[3][1] < 0)
		H = -QtrRev;
	else if( C != 0)
		H = atan( YY[3][1]/C);

	if( C == 0)
		/* F should be indeterminate: program listing is F = 1000*DegToRad; */
		F = 0;
	else if( C != 0 && YY[1][1] == 0 && YY[2][1] > 0)
		F = QtrRev;
	else if( C != 0 && YY[1][1] == 0 && YY[2][1] < 0)
		F = OneRev - QtrRev;
	else if( YY[1][1] > 0)
		F = atan( YY[2][1]/YY[1][1]);
	else if( YY[1][1] < 0)
		F = atan( YY[2][1]/YY[1][1]) + HalfRev;
	while( F > OneRev)
		F -= OneRev;
	while( F < 0)
		F += OneRev;
}

/* call after init 2, calculates apparent scope latitude, longitude, offset azimuth, and updates
field rotation */
void CalcPostInitVars( void)
{
	double PE;
	/* temporary coordinates */
	struct Position Temp = Current;

	/* use current time */
	Current.SidT = SidT;
	/* find Lat by setting for 90 deg declination( NCP) */
	Current.Dec = QtrRev;
	Current.Ra = 0;
	GetAltaz();
	Lat = Current.Alt;
	SinLat = sin( Lat);
	CosLat = cos( Lat);
	SinLatDividedByCosLat = SinLat/CosLat;

	/* set azimuth offset (fixed up later for equatorial alignments) */
	AzOff = Current.Az;

	/* set angles that scope should be moved in order to be polar aligned:
	if .A > 0, then scope should be moved up, if .Z > 0, then scope should be moved to the right */
	PE = QtrRev - Current.Alt;
	PolarAlign.A = PE * cos( Current.Az);
	PolarAlign.Z = PE * sin( Current.Az);

	/* find longitude and HAOff by aiming at zenith */
	if( Lat == QtrRev || Lat == -QtrRev)
		/* LatitudeDeg comes from config.dat */
		Current.Alt = LatitudeDeg*DegToRad;
	else
		Current.Alt = QtrRev;
	Current.Az = 0;
	GetEquat();
	/* LongitudeDeg*DegToRad + Current.SidT = Greenwich Sidereal Time;
	difference between GST and Current.Ra (== zenith) will be scope longitude */
	LongitudeRad = LongitudeDeg*DegToRad + Current.SidT - Current.Ra;
	while( LongitudeRad < 0)
		LongitudeRad += OneRev;
	while( LongitudeRad > OneRev)
		LongitudeRad -= OneRev;

	/* find hour angle offset = LST( Current.SidT) - scope's meridian,
	HA = LST - HAOff - Ra, or, HAOff = LST - Ra, by setting for zenith (HA = 0);
	+ offset = scope tilted to West, - offset = scope tilted to East;
	HAOff varies from - offset to + offset( should be a small amount) */
	HAOff = Current.SidT - Current.Ra;
	while( HAOff > M_PI)
		HAOff -= OneRev;
	while( HAOff < -M_PI)
		HAOff += OneRev;

	/* fixup for equatorial alignments */
	if( Lat == QtrRev || Lat == -QtrRev)
		AzOff = HAOff;

	/* for conversion routines that need site latitude such as CalcAirMass() and CalcDomeAzimuth() */
	ConfigLat = LatitudeDeg * DegToRad;
	ConfigSinLat = sin( ConfigLat);
	ConfigCosLat = cos( ConfigLat);

	/* restore current coordinates */
	Current = Temp;
}

void GetAltaz( void)
{
	int I, J;
	double B;

	CheckHoldSinCosCurrentDec();

	/* B is CCW so HA formula backwards */
	B = Current.Ra - Current.SidT;
	if( GEMflippedFlag)
		B += HalfRev;
	/* convert to rectangular coordinates and put in XX */
	XX[1][1] = HoldCosCurrentDec * cos( B);
	XX[2][1] = HoldCosCurrentDec * sin( B);
	XX[3][1] = HoldSinCurrentDec;
	YY[1][1] = 0;
	YY[2][1] = 0;
	YY[3][1] = 0;
	/* mutiply XX by transform matrix RR to get equatorial rectangular coordinates */
	for( I = 1; I <= 3; I++)
		for( J = 1; J <= 3; J++)
			YY[I][1] += (RR[I][J] * XX[J][1]);
	/* convert to celestial coordinates */
	AngleSubr();
	/* modify for non-zero Z1Z2Z3 mount error values */
	SubrSwitcher();
	AngleSubr();
	if( GEMflippedFlag)
		H += Z3;
	else
		H -= Z3;
	Current.Alt = H;
	if( GEMflippedFlag)
		Current.Alt = HalfRev - Current.Alt;
	/* convert back to CW */
	Current.Az = OneRev - F;
}

/* if GEM (German Equatorial Mount) is flipped, then flipped Ra differs from original setting circle
Ra by 12 hrs; altitude reading is mirrored across the pole, that is, an alt of 80 is actually 100
(mirrored across 90) as read from the original setting circle orientation */

void GetEquat( void)
{
	int I, J;

	/* convert from CW to CCW az */
	F = OneRev - Current.Az;
	if( GEMflippedFlag)
	{
		H = Current.Alt - Z3;
		H = HalfRev - H;
	}
	else
		H = Current.Alt + Z3;
	SubrA();
	XX[1][1] = YY[1][0];
	XX[2][1] = YY[2][0];
	XX[3][1] = YY[3][0];
	YY[1][1] = 0;
	YY[2][1] = 0;
	YY[3][1] = 0;
	for( I = 1; I <= 3; I++)
		for( J = 1; J <= 3; J++)
			YY[I][1] += (QQ[I][J] * XX[J][1]);
	AngleSubr();
	F += Current.SidT;
	Current.Ra = F;
	if( GEMflippedFlag)
		Current.Ra += HalfRev;
	ValidRa( &Current);
	Current.Dec = H;
}


