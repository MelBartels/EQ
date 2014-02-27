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

/* handpad area... */

void InitHPEvent( void)
{
	if( FocusFastStepsSec < 1)
		FocusFastStepsSec = 1;
	if( FocusSlowStepsSec < 1)
		FocusSlowStepsSec = 1;
	if( FocusFastStepsSec > 500)
		FocusFastStepsSec = 500;
	if( FocusSlowStepsSec > 500)
		FocusSlowStepsSec = 500;

	InsideProcessHPEventsFocusMethod_Pulse_Dir = No;
	DisplayAltAutoSynchFlag = No;
	DisplayAzAutoSynchFlag = No;
	TrackFlag = No;
	MoveToCurrentRaDecFlag = No;
	GuideFlag = No;
	GrandTourFlag = No;
	ScrollFlag = No;

	InitiatedType = NotInitiated;
	HandpadFlag = StartingHandpadMode;
	HandpadAuxButtonPressed = No;
	HandpadFocusButtonPressed = No;
	HandpadAuxControlOnFlag = No;
	HandpadFocusMotorControlOnFlag = No;
	HandpadButtonTimer = 0;
	RecordEquatTimer = 0;
	ConsecutiveSlews = 0;

	BacklashDir.A = BacklashDir.Z = CW;
	CheckBacklashOscillation = Yes;
	CalcBacklashVars();

	/* set microstepping index:
	(AccumMs.A%MsInWindings)/Ms is winding #, so multiply this by MaxPWM*Ms to get index */
	MsIx.A = (int)(MaxPWM*Ms*((AccumMs.A%MsInWindings)/Ms));
	if( MsIx.A < 0)
		MsIx.A += MaxMsIx + 1;
	MsIx.Z = (int)(MaxPWM*Ms*((AccumMs.Z%MsInWindings)/Ms));
	if( MsIx.Z < 0)
		MsIx.Z += MaxMsIx + 1;

	SetCurrentAltazToAccumMs();
	HPEventGetEquat();
}

void CalcBacklashVars( void)
{
	BacklashFlag = No;
	NegBacklash.A = NegBacklash.Z = False;
	if( AltBacklashArcmin != 0)
	{
		BacklashFlag = Yes;
		if( AltBacklashArcmin < 0)
		{
			AltBacklashArcmin = -AltBacklashArcmin;
			NegBacklash.A = True;
		}
	}
	if( AzBacklashArcmin != 0)
	{
		BacklashFlag = Yes;
		if( AzBacklashArcmin < 0)
		{
			AzBacklashArcmin = -AzBacklashArcmin;
			NegBacklash.Z = True;
		}
	}
	/* set backlash microsteps to move */
	BacklashMs.A = AltBacklashArcmin * ArcminToRad / MsRad.A;
	BacklashMs.Z = AzBacklashArcmin * ArcminToRad / MsRad.Z;
	/* set exact amount of backlash movement: mvmt (rad) = steps*stepsize */
	Backlash.A = BacklashMs.A * MsRad.A;
	Backlash.Z = BacklashMs.Z * MsRad.Z;
}

/* AccumMs is the telescope position derived from counting the revolutions of the motors while
Current. is the actual telescope's pointing position */
void SetCurrentAltazToAccumMs( void)
{
	/* set to AccumMs */
	Current.Alt = AccumMs.A*MsRad.A;
	Current.Az = AccumMs.Z*MsRad.Z;

	/* BacklashDir. is direction that backlash has been taken up in, if it is CCW, motors have been
	spun extra CCW to takeup backlash up with no change in scope's apparent pointing direction
	resulting in AccumMs being too low, so	must add backlash amount to AccumMs to arrive at true
	pointing position */
	if( BacklashFlag)
	{
		if( BacklashDir.A == CCW)
			if( NegBacklash.A)
				Current.Alt -= Backlash.A;
			else
				Current.Alt += Backlash.A;
		if( BacklashDir.Z == CCW)
			if( NegBacklash.Z)
				Current.Az -= Backlash.Z;
			else
				Current.Az += Backlash.Z;
	}
	/* positive PEC, Drift, and Guide, indicate excess CW movement, that is, the scope with respect
	to the stars has drifted CW, therefore the Current. position that AccumMs would yield is too
	small and must be increased to match the altaz coordinates as derived from the stars;
	in the same manner, positive QSC means excess CW movement (halfsteps too large);
	positive AccumGuide means that the a CCW button had to be pressed to compensate for excess CW
	movement	(CCW button reduces AccumMs and would result in lower Current.	unless AccumGuide added
	in to	compensate); AccumDrift is based on a guiding session where a drift rate of AccumGuide over
	time is calculated */
	if( GuideFlag)
	{
		Current.Alt += AccumGuide.A;
		Current.Az += AccumGuide.Z;
	}
	ValidAz( &Current);
}

void SetAccumMsToCurrentAltaz( void)
{
	struct AZDouble HoldAZ;

	/* use Hold to calculate Alt and Az for AccumAltMs and AccumAzMs:
	this leaves Current. values unchanged */
	HoldAZ.A = Current.Alt;
	HoldAZ.Z = Current.Az;

	if( GuideFlag)
	{
		HoldAZ.A -= AccumGuide.A;
		HoldAZ.Z -= AccumGuide.Z;
	}
	if( BacklashFlag)
	{
		if( BacklashDir.A == CCW)
			if( NegBacklash.A)
				HoldAZ.A += Backlash.A;
			else
				HoldAZ.A -= Backlash.A;
		if( BacklashDir.Z == CCW)
			if( NegBacklash.Z)
				HoldAZ.Z += Backlash.Z;
			else
				HoldAZ.Z -= Backlash.Z;
	}

	AccumMs.A = (long) (0.5 + HoldAZ.A/MsRad.A);
	/* set to nearest full step */
	AccumMs.A -= AccumMs.A%Ms;

	AccumMs.Z = (long) (0.5 + HoldAZ.Z/MsRad.Z);
	/* set to nearest full step */
	AccumMs.Z -= AccumMs.Z%Ms;
}

/* called by KBEventMoveHs() */
void HPEventMoveHs( void)
{
	CheckBacklashOscillation = No;
	MoveBacklash();
	CheckBacklashOscillation = Yes;
	MoveHs_f_ptr();
	SetCurrentAltazToAccumMs();
}

void HPEventMoveMs( void)
{
	DisplayButtonsStatus();
	MoveBacklash();
	MoveMs_f_ptr();
	SetCurrentAltazToAccumMs();
}

/* moves in (altaz) Ms until handpad buttons change */
void AltazMoveMs( void)
{
	struct AZLong HoldSteps = Steps;

	InitHandpad = Handpad;
	HandpadOKFlag = Yes;
	while( HandpadOKFlag)
	{
		HPEventMoveMs();
		HPEventGetEquat();
		ReadLX200Input();
		SequentialTaskController();
		/* add undone steps to steps to do for next clock tick */
		Steps.A += HoldSteps.A;
		Steps.Z += HoldSteps.Z;
		SetHandpadOKFlag();
	}
	AlignMs();
	SetCurrentAltazToAccumMs();
	HPEventGetEquat();
}

/* moves to takeup backlash if necessary; returns true if mvmt occured */
Flag MoveBacklash( void)
{
	struct AZLong SaveSteps, TotalSteps;
	static struct AZFlag LastDir[LastDirSize];
	int Ix;
	static int HoldIx = 0;
	Flag CancelBacklash = No;
	struct AZFlag SaveDir;

	/* if backlash 'on', motor(s) can oscillate to and fro when microstepping tracking and steps = 0
	or 1 because Dir can flip/flop;
	ignore oscillation check if handpad initiated move or halfstep slew:
	CheckBacklashOscillation turned off by HPEventMoveHs() just before call to	MoveBacklash() and
	turned on afterwards;
	MoveToCurrentRaDecFlag turned on while in MoveToCurrentRaDec() */
	if( CheckBacklashOscillation && MoveToCurrentRaDecFlag)
		/* prevent backlash action if to and fro motor oscillations */
		for( Ix = 0; Ix < LastDirSize; Ix++)
			if( LastDir[Ix].A != Dir.A || LastDir[Ix].Z != Dir.Z)
			{
				CancelBacklash = Yes;
				break;
			}
	if( ++HoldIx == LastDirSize)
		HoldIx = 0;
	LastDir[HoldIx] = Dir;

	if( BacklashFlag && !CancelBacklash)
	{
		/* preserve original steps to move */
		SaveSteps = Steps;
		Steps.A = Steps.Z = 0;
		/* may need to disallow backlash if step = 1 because Dir can slowly flip/flop every couple of
		seconds causing slow oscillation, however, when step sizes are large, the tracking 'steps to
		move' is often 0 or 1 */
		if( SaveSteps.A > 0)
		{
			if( Dir.A == CW && BacklashDir.A == CCW)
			{
				BacklashDir.A = CW;
				Steps.A = BacklashMs.A;
			}
			if( Dir.A == CCW && BacklashDir.A == CW)
			{
				BacklashDir.A = CCW;
				Steps.A = BacklashMs.A;
			}
		}
		if( SaveSteps.Z > 0)
		{
			if( Dir.Z == CCW && BacklashDir.Z == CW)
			{
				BacklashDir.Z = CCW;
				Steps.Z = BacklashMs.Z;
			}
			if( Dir.Z == CW && BacklashDir.Z == CCW)
			{
				BacklashDir.Z = CW;
				Steps.Z = BacklashMs.Z;
			}
		}
		SaveDir = Dir;
		if( NegBacklash.A == True)
		{
			if( Dir.A == CW)
				Dir.A = CCW;
			else
				Dir.A = CW;
		}
		if( NegBacklash.Z == True)
		{
			if( Dir.Z == CW)
				Dir.Z = CCW;
			else
				Dir.Z = CW;
		}

		if( Steps.A || Steps.Z)
		{
			/* set MsTick.A, MsTick.Z */
			InitMsTickVars( BacklashMsArcsecSec);
			/* Steps used by MoveMs(), so go with TotalSteps */
			TotalSteps = Steps;
			/* if PWMRepsTick set above actual # of PWMRepsTick, MoveMs() will not complete last
			microstep, so stop loop when steps are 1 or less */
			while( TotalSteps.A > 1 || TotalSteps.Z > 1)
			{
				/* set steps to do */
				Steps = TotalSteps;
				/* adjust Steps to move at backlash microstepping speed */
				if( Steps.A > MsTick.A)
					Steps.A = MsTick.A;
				if( Steps.Z > MsTick.Z)
					Steps.Z = MsTick.Z;
				/* subtract steps to do, anticipating MoveMs()'s decrement of Steps to 0 */
				TotalSteps.A -= Steps.A;
				TotalSteps.Z -= Steps.Z;
				/* not HPEventMoveMs() 'cause HPEventMoveMs() calls MoveBacklash()
				causing infinite loop */
				MoveMs_f_ptr();
				/* add back in any steps not done by MoveMs() */
				TotalSteps.A += Steps.A;
				TotalSteps.Z += Steps.Z;
			}
			/* recover original steps to move */
			Steps = SaveSteps;
			/* restore direction */
			Dir = SaveDir;
			/* restore microstep speed */
			InitMsTickVars( MsArcsecSec);
			return True;
		}
		else
		{
			/* restore original steps to move */
			Steps = SaveSteps;
			/* restore direction */
			Dir = SaveDir;
			return False;
		}
	}
	else
		return False;
}

void GetUnusedPPortLines( void)
{
	BiDirInNibbleValue = BiDirInNibble();

	switch( FocusMethod)
	{
		case FocusMethod_Pulse_1_14:
			UnusedFocusPPortLines = BiDirInNibbleValue & PPortPins16_17;
			break;
		case FocusMethod_Pulse_16_17:
			  FocusMethod_OnOff_16_17:
			UnusedFocusPPortLines = BiDirInNibbleValue & PPortPins1_14;
			break;
		case FocusMethod_OnOff_16_17_Slow1_14:
			UnusedFocusPPortLines = 0;
	}
}

void SetFocusControlLines( void)
{
	if( FocusMethod == FocusMethod_Pulse_1_14)
	{
		FocusPulseBit = PPortPin1;
		FocusDir = PPortPin14;
	}
	else
		if( FocusMethod == FocusMethod_Pulse_16_17)
		{
			FocusPulseBit = PPortPin16;
			FocusDir = PPortPin17;
		}
		else
		{
			FocusPulseBit = Off;
			FocusDir = Off;
		}
}

void ReverseFocusDir( void)
{
	if( FocusDir == Off)
	{
		if( FocusMethod == FocusMethod_Pulse_1_14)
			FocusDir = PPortPin14;
		else
			if( FocusMethod == FocusMethod_Pulse_16_17)
				FocusDir = PPortPin17;
			else
				FocusDir = Off;
	}
	else
		FocusDir = Off;
}

/* called from SetPulseFRFocusFRInMoveMs() which is called from start of MoveMs(): sets up the
focus motor for movement */
void SetFocusStepsDirMoveMs( void)
{
	SetFocusControlLines();
	if( InitiatedType == HandpadInitiated && Buttons & CCWKey ||
	InitiatedType == LX200Initiated && LX200_Focus_Cmd == FocusIn ||
	InitiatedType == MouseInitiated && (MouseMoveResult == MouseMovedLeft || MouseMoveResult == MouseMovedDown))
		ReverseFocusDir();
	if( ReverseFocusMotor)
		ReverseFocusDir();
	/* set speed */
	if( InitiatedType == HandpadInitiated && MsSpeed ||
	InitiatedType == LX200Initiated && LX200_Focus_Speed_Cmd == FocusSetSlow ||
	InitiatedType == MouseInitiated && (MouseMoveResult == MouseMovedUp || MouseMoveResult == MouseMovedDown))
	{
		FocusFastDisplayFlag = False;
		FocusStepsToMove = (double) FocusSlowStepsSec / ClockTicksSec;
	}
	else
	{
		FocusFastDisplayFlag = True;
		FocusStepsToMove = (double) FocusFastStepsSec / ClockTicksSec;
	}

	FocusStepIncrPerPWM = FocusStepsToMove / PWMRepsTick;
	/* max number of Focus pulses per clock tick is PWMRepsTick/2 because of the on/off bit toggling */
	if( FocusStepIncrPerPWM > .5)
		FocusStepIncrPerPWM = .5;
	FocusStepsDone = 0;
}

/* called from start of MoveMs() */
void SetPulseFRFocusFRInMoveMs( void)
{
	PulseFocus = False;
	if( (FocusMethod == FocusMethod_Pulse_1_14 || FocusMethod == FocusMethod_Pulse_16_17) &&
	((HandpadFocusMotorControlOnFlag && (Buttons & CCWKey || Buttons & CWKey)) ||
	InitiatedType == LX200Initiated && LX200_Focus_Cmd != FocusQuit ||
	InitiatedType == MouseInitiated && MouseMode == MouseModeFocus && MouseLeftButtonDown))
	{
		SetFocusStepsDirMoveMs();
		PulseFocus = True;
	}
}

/* called from MoveMs() at end of each PWM */
void PulseFRFocusPerPWM( void)
{
	BiDirOutNibbleValue = 0;

	if( PulseFocus)
	{
		BiDirOutNibbleValue += FocusDir;
		if( FocusPulse)
		{
			FocusPulse = Off;
			BiDirOutNibbleValue += FocusPulseBit;
			FocusStepsDone++;
			FocusDiagPulseLow++;
			if( (FocusDir && !ReverseFocusMotor) || (!FocusDir && ReverseFocusMotor))
				FocusPosition++;
			else
				FocusPosition--;
		}
		else
		{
			HoldFocusSteps += FocusStepIncrPerPWM;
			if( HoldFocusSteps >= 1 && FocusStepsToMove > FocusStepsDone)
			{
				HoldFocusSteps -= 1;
				if( (FocusDir && !ReverseFocusMotor) || (!FocusDir && ReverseFocusMotor))
					FocusDiagPulseHighCW++;
				else
					FocusDiagPulseHighCCW++;
				FocusPulse = On;
			}
		}
	}
	/* leave untouched unused control lines */
	if( !PulseFocus)
		BiDirOutNibbleValue += UnusedFocusPPortLines;

	if( PulseFocus && FocusPulse)
		if( FocusDir)
			DisplayBiDirOut |= DisplayFocusOutBit;
		else
			DisplayBiDirOut |= DisplayFocusInBit;

	BiDirOutNibble();
}

/* for siderostat or uranostat or equat mounts: normally the az would be flipped 180 deg when moving
to other side of zenith, however, certain mounts cannot make this flip because the polar axis would
rotate to unacceptable positions, hence the need for this check */
void CheckSiderostatAltaz( void)
{
	if( Siderostat)
		if( Current.Az < AzLowLimit || Current.Az > AzHighLimit)
		{
			Current.Alt = M_PI - Current.Alt;
			Current.Az += M_PI;
			ValidAz( &Current);
		}
}

void ProcessLimitError( const char* Msg)
{
	if( HandpadFlag == GrandTour || HandpadFlag == ScrollTour || HandpadFlag == ScrollAutoTour)
	{
		WriteWindow( MsgFrame);
		gotoxy( MsgFrame.Left + 2, MsgFrame.Top + 2);
		printf( "%s", Msg);
		sound( 500);
		delay( 100);
		nosound();
		delay( 100);
		sound( 500);
		delay( 100);
		nosound();
      delay( 700);
		RemoveWindow( MsgFrame);
		PauseUntilNewSidTime();
	}
	else
		PressKeyToContMsg( Msg);
}

void ProcessGEMFlippedFlag( void)
{
	GEMflippedFlag = !GEMflippedFlag;
	if( HandpadFlipUpDownWithGEMFlip)
		UpDownButtonsReversedFlag = !UpDownButtonsReversedFlag;
	DisplayGEMFlipStatus();
}

/* scope is brought to position calculated at start of clock tick by microstepping until next clock
tick (scope is always 1 clock tick behind during tracking or microstepping) if possible, or by
halfstepping if move too large;
if slew does not bring scope close enough to initiate microstepping, then next pass through function
will also slew, eventually moving scope close enough to desired position to track by microstepping
*/
void MoveToCurrentRaDec( void)
{
	struct AZDouble HoldAZ;

	MoveToCurrentRaDecFlag = Yes;
	if( Two.Init)
		if( TrackFlag)
		{
			HoldAZ.A = Current.Alt;
			HoldAZ.Z = Current.Az;
			GetAltaz();
			CheckSiderostatAltaz();

			if( AutoGEMFlip)
			{
				/* Current.Az increases as scope moves west of meridian;
				when on meridian, if no GEM flip, Current.Az == 0 else if GEM flip, Current.Az = 180;
				if not GEM flip, then commonly 0<Current.Az<90deg
				if GEM flip, then commonly 90deg<Current.Az<180deg (scope is flipped 180 degrees, plus scope is
				aimed at opposite hemisphere); */
				if( (GEMflippedFlag && Current.Az > HalfRev+AutoGEMFlipFuzzRad && Current.Az < OneRev)
				|| (!GEMflippedFlag && Current.Az > HalfRev && Current.Az < OneRev-AutoGEMFlipFuzzRad))
				{
					ProcessGEMFlippedFlag();
					GetAltaz();
				}
			}
			if( AltLimitFlag && Current.Alt < AltLowLimit)
			{
				Current.Alt = HoldAZ.A;
				Current.Az = HoldAZ.Z;
				TrackFlag = No;
				WriteTrackStatus();
				ProcessLimitError( "Tried to move below AltLowLimt");
			}
			else
				if( AltLimitFlag && Current.Alt > AltHighLimit)
				{
					Current.Alt = HoldAZ.A;
					Current.Az = HoldAZ.Z;
					TrackFlag = No;
					WriteTrackStatus();
					ProcessLimitError( "Tried to move above AltHighLimt");
				}
				else
					if( AzLimitFlag && Current.Az < AzLowLimit)
					{
						Current.Alt = HoldAZ.A;
						Current.Az = HoldAZ.Z;
						TrackFlag = No;
						WriteTrackStatus();
						ProcessLimitError( "Tried to move below AzLowLimt");
					}
					else
						if( AzLimitFlag && Current.Az > AzHighLimit)
						{
							Current.Alt = HoldAZ.A;
							Current.Az = HoldAZ.Z;
							TrackFlag = No;
							WriteTrackStatus();
							ProcessLimitError( "Tried to move above AzHighLimt");
						}
						else
						{
							/* Delta = new - old:
							Hold., representing current telescope pointing, is corrected for various
							errors, but the position to move to, Current. is not; therefore any change in
							overall correction will not be taken care of until the next bios clock tick
							(when in effect the change in corrective values will be added to the move to
							make, ie, if the scope is to move 10 steps per tick, and if the corrective
							factors go from 0 to 5, then the scope will move 10 steps, but will in
							actuality be pointed at 15, so scope has overshot by 5, and the next move will
							be 5, bringing the scope onto target, thus the error will be the overall change
							in corrective values from bios clock tick to tick); this is done to save CPU
							cycles because of the intensive nature of calculating all the corrective
							factors twice (where the scope is truly pointed to after the move that has just
							occured, and where the scope would be truly pointed to after the proposed move)
							instead of once per bios clock tick, also, it is not conceivable that the
							change in these factors would exceed 1 arcsec over 1/18 of a second; finally, a
							delay can also occur when the MsRepsTick value in config.dat is much larger
							than the ActualMsRepsTick - here steps are not completed before running out of
							time and therefore will not be made up until the next bios clock tick */
							Delta.A = Current.Alt - HoldAZ.A;
							Delta.Z = Current.Az - HoldAZ.Z;
							Current.Alt = HoldAZ.A;
							Current.Az = HoldAZ.Z;
							/* if ok, then calc the move and make it so */
							if( !SetDirDistanceStepsThenMove())
							{
								TrackFlag = No;
								WriteTrackStatus();
								PauseUntilNewSidTime();
								HPEventGetEquat();
							}
						}
		}
		else
		{
			PauseUntilNewSidTime();
			HPEventGetEquat();
		}
	else
		PauseUntilNewSidTime();

	MoveToCurrentRaDecFlag = No;
}

Flag SetDirDistanceStepsThenMove( void)
{
	/* altitude... */
	if( Delta.A < 0)
	{
		Delta.A = -Delta.A;
		Dir.A = CCW;
	}
	else
		Dir.A = CW;

	/* azimuth (increases CW)...
	4 possibilities of calculating az steps to move:
		start		end		delta		+ 360 if nec.	 > 180?	result
	1. 350,     200,     -150,    210,           yes,     -150
	2. 200,     350,     150,                    no,      150
	3. 350,     100,     -250,    110,           no,      110
	4. 100,     350,     250,                    yes,     -110 */
	if( Delta.Z < 0)
		Delta.Z += OneRev;
	/* if > 1/2 rev, go other way */
	if( Delta.Z > M_PI)
		/* get distance to go (- or +) */
		Delta.Z -= OneRev;
	if( Delta.Z < 0)
	{
		Delta.Z = -Delta.Z;
		Dir.Z = CCW;
	}
	else
		Dir.Z = CW;

	/* check to see if distance can be covered by microstepping */
	if( Delta.A <= MaxMsDistanceRadTrack.A && Delta.Z <= MaxMsDistanceRadTrack.Z)
	{
		Steps.A = (long) (0.5 + Delta.A/MsRad.A);
		Steps.Z = (long) (0.5 + Delta.Z/MsRad.Z);
		ConsecutiveSlews = 0;
		if( SlewBeep == SlewUnderway)
			SlewBeep = SlewFinished;
		HPEventMoveMs();
		return True;
	}
	else
	{
		Steps.A = (long) (0.5 + (Delta.A/HsRad.A));
		Steps.Z = (long) (0.5 + (Delta.Z/HsRad.Z));
		/* if too many steps, confirm move */
		if( ConfirmHsMove())
		{
			ConsecutiveSlews++;
			KBEventMoveHs();
			if( SlewBeep != SlewBeepOn)
				SlewBeep = SlewUnderway;
			return True;
		}
		else
			return False;
	}
}

void HPEventGetEquat( void)
{
	if( Two.Init)
		GetEquat();
}

/* record equatorial position when handpad mode switch activated */
void RecordEquat( void)
{
	Output = fopen( RecordEquatFile, "a");
	if( Output == NULL)
		BadExit( strcat( "Could not open ", RecordEquatFile));

	GetHMSH( RadToHundSec*Current.Ra + 0.5, &Current.RaHMSH);
	GetDMS( RadToArcsec*Current.Dec + 0.5, &Current.DecDMS);
	if( Current.DecDMS.Sign == Minus)
	{
		Current.DecDMS.Deg = -Current.DecDMS.Deg;
		Current.DecDMS.Min = -Current.DecDMS.Min;
		Current.DecDMS.Sec = -Current.DecDMS.Sec;
	}
	fprintf( Output, "%3d %3d %3d   %3d %3d %3d   handpad_record\n",
	Current.RaHMSH.Hr, Current.RaHMSH.Min, Current.RaHMSH.Sec,
	Current.DecDMS.Deg, Current.DecDMS.Min, Current.DecDMS.Sec );

	fclose( Output);
}

Flag IACA_Event( void)
{
	if( NewIACA())
	{
		GetIACA( &In.Ra, &In.Dec);
		GetHMSH( In.Ra*RadToHundSec+.5, &In.RaHMSH);
		GetDMS( In.Dec*RadToArcsec+.5, &In.DecDMS);
		DisplayIn( "from IACA", NameBlanks);
		return True;
	}
	else
		return False;
}

int FindClosestInit( void)
{
	double Init1Distance, Init2Distance, Init3Distance;

	Init1Distance = CalcEquatAngularSep( &Current, &One);
	Init2Distance = CalcEquatAngularSep( &Current, &Two);
	Init3Distance = CalcEquatAngularSep( &Current, &Three);
	if( Init1Distance < Init2Distance && Init1Distance < Init3Distance)
		return 1;
	else
		if( Init2Distance < Init3Distance)
			return 2;
		else
			return 3;
}

void CheckLX200Events( void)
{
	if( LX200ComPort)
	{
		ReadLX200Input();

		if( SlewState == SlewDone && (LX200_Motor_Cmd == StartSlew
												|| LX200_Motor_Cmd == MoveDirRateNorth
												|| LX200_Motor_Cmd == MoveDirRateSouth
												|| LX200_Motor_Cmd == MoveDirRateEast
												|| LX200_Motor_Cmd == MoveDirRateWest))

			ProcessLX200_Motor_Cmd();
	}
}

void CheckMiscEvents( void)
{
	if( IACA_Flag && IACA_Event())
		;
	else
	{
		CheckLX200Events();
		MoveToCurrentRaDec();
	}

	/* handle consecutive slews when scope should be microstepping tracking;
	by resetting current equatorial to current altazimuth, tracking can start fresh from current
	position instead of repeatedly trying to slew to previous current equatorial coordinates */
	if( ConsecutiveSlews >= MaxConsecutiveSlews)
	{
		ConsecutiveSlews = 0;
		PauseUntilNewSidTime();
		HPEventGetEquat();
		In = Current;
		DisplayIn( "consecutive slews", NameBlanks);
	}
}

void ProcessHPEventsGuide( void)
{
	if( Buttons & UpKey)
		AccumGuide.A -= GuideRadTick;
	if( Buttons & DownKey)
		AccumGuide.A += GuideRadTick;
	if( Buttons & CWKey)
		AccumGuide.Z -= GuideRadTick;
	if( Buttons & CCWKey)
		AccumGuide.Z += GuideRadTick;
}

/* for moving the focus motor while tracking is off */
void ProcessHPEventsFocusMethod_Pulse_Dir( void)
{
	int msdelay;

	SetFocusControlLines();
	/* set focus motor direction */
	if( InitiatedType == HandpadInitiated && Buttons & CCWKey ||
	InitiatedType == LX200Initiated && LX200_Focus_Cmd == FocusIn ||
	InitiatedType == MouseInitiated && (MouseMoveResult == MouseMovedLeft || MouseMoveResult == MouseMovedDown))
		ReverseFocusDir();
	if( ReverseFocusMotor)
		ReverseFocusDir();
	/* set focus motor speed */
	if( InitiatedType == HandpadInitiated && MsSpeed ||
	InitiatedType == LX200Initiated && LX200_Focus_Speed_Cmd == FocusSetSlow ||
	InitiatedType == MouseInitiated && (MouseMoveResult == MouseMovedUp || MouseMoveResult == MouseMovedDown))
	{
		FocusFastDisplayFlag = False;
		msdelay = 1000 / (FocusSlowStepsSec * 2);
	}
	else
	{
		FocusFastDisplayFlag = True;
		msdelay = 1000 / (FocusFastStepsSec * 2);
	}
	if( msdelay < 1)
		msdelay = 1;
	/* get state of port so as to not disturb unused parallel port pins */
	GetUnusedPPortLines();
	/* setup handpad vars */
	InitHandpad = Handpad;
	HandpadOKFlag = Yes;
	/* keep control here */
	while( HandpadOKFlag &&	!KeyStroke &&
	(InitiatedType != LX200Initiated || InitiatedType == LX200Initiated && LX200_Focus_Cmd != FocusQuit) &&
	(InitiatedType != MouseInitiated || InitiatedType == MouseInitiated && !MouseLeftButtonReleaseCount()))
	{
		InsideProcessHPEventsFocusMethod_Pulse_Dir = Yes;
		delay( msdelay);
		if( FocusDir)
			DisplayBiDirOut |= DisplayFocusOutBit;
		else
			DisplayBiDirOut |= DisplayFocusInBit;
		/* raise focus motor pulse */
		BiDirOutNibbleValue = UnusedFocusPPortLines + FocusPulseBit + FocusDir;
		BiDirOutNibble();
		if( (FocusDir && !ReverseFocusMotor) || (!FocusDir && ReverseFocusMotor))
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
		/* other movement functions that retain control until user stops the movement use MoveMs()
		which calls NewSidT() */
		NewSidT();
		HPEventGetEquat();
		CheckLX200Events();
		SequentialTaskController();
	}
	InsideProcessHPEventsFocusMethod_Pulse_Dir = No;
	if( KeyStroke)
		getch();
}

/* raise/lower the bidrectional parallel port pins */
void ProcessHPEventsHandpadAux( void)
{
	InitiatedType = HandpadInitiated;
	switch( Buttons)
	{
		case UpKey:
			AuxControl = Aux1;
			HandpadAuxButtonPressed = Yes;
			break;
		case DownKey:
			AuxControl = Aux14;
			HandpadAuxButtonPressed = Yes;
			break;
		case CWKey:
			AuxControl = Aux16;
			HandpadAuxButtonPressed = Yes;
			break;
		case CCWKey:
			AuxControl = Aux17;
			HandpadAuxButtonPressed = Yes;
			break;
		default:
			AuxControl = AuxOff;
			HandpadAuxButtonPressed = No;
	}
	AuxControlBiDirPPort();
}

/* raise/lower the bidrectional parallel port pins */
void AuxControlBiDirPPort( void)
{
	BiDirInNibbleValue = BiDirInNibble();
	switch( AuxControl)
	{
		case Aux1:
			/* parallel port pin #1 */
			BiDirOutNibbleValue = (BiDirInNibbleValue & PPortPins16_17) + PPortPin1;
			DisplayBiDirOut |= DisplayAux1Bit;
			break;
		case Aux14:
			/* parallel port pin #14 */
			BiDirOutNibbleValue = (BiDirInNibbleValue & PPortPins16_17) + PPortPin14;
			DisplayBiDirOut |= DisplayAux14Bit;
			break;
		case Aux16:
			/* parallel port pin #16 */
			BiDirOutNibbleValue = (BiDirInNibbleValue & PPortPins1_14) + PPortPin16;
			DisplayBiDirOut |= DisplayAux16Bit;
			break;
		case Aux17:
			/* parallel port pin #17 */
			BiDirOutNibbleValue = (BiDirInNibbleValue & PPortPins1_14) + PPortPin17;
			DisplayBiDirOut |= DisplayAux17Bit;
			break;
		case AuxOff:
			BiDirInNibbleValue = Off;
	}
	BiDirOutNibble();
}

void ProcessHPEventsFocus_OnOff_16_17( void)
{
	InitiatedType = HandpadInitiated;
	if( Buttons & CCWKey)
	{
		FocusControl = FocusMinus;
		HandpadFocusButtonPressed = Yes;
	}
	else
		if( Buttons & CWKey)
		{
			FocusControl = FocusPlus;
			HandpadFocusButtonPressed = Yes;
		}
		else
		{
			FocusControl = FocusStop;
			HandpadFocusButtonPressed = No;
		}

	FocusControl_OnOff_16_17();
}

void FocusControl_OnOff_16_17( void)
{
	BiDirInNibbleValue = BiDirInNibble();
	switch( FocusControl)
	{
		case FocusPlus:
			if( ReverseFocusMotor)
				FocusPulseBit = PPortPin17;
			else
				FocusPulseBit = PPortPin16;
			break;
		case FocusMinus:
			if( ReverseFocusMotor)
				FocusPulseBit = PPortPin16;
			else
				FocusPulseBit = PPortPin17;
			break;
		case FocusStop:
			FocusPulseBit = Off;
	}
	if( FocusPulseBit == PPortPin16)
		DisplayBiDirOut |= DisplayFocusOutBit;
	else
		if( FocusPulseBit == PPortPin17)
			DisplayBiDirOut |= DisplayFocusInBit;
	BiDirOutNibbleValue = (BiDirInNibbleValue & PPortPins1_14) + FocusPulseBit;
	BiDirOutNibble();
	FocusFastDisplayFlag = True;
}

void ProcessHPEventsFocus_OnOff_16_17_Slow1_14( void)
{
	if( !MsSpeed)
		ProcessHPEventsFocus_OnOff_16_17();
	else
	{
		InitiatedType = HandpadInitiated;
		if( Buttons & CCWKey)
		{
			FocusControl = FocusMinus;
			HandpadFocusButtonPressed = Yes;
		}
		else
			if( Buttons & CWKey)
			{
				FocusControl = FocusPlus;
				HandpadFocusButtonPressed = Yes;
			}
			else
			{
				FocusControl = FocusStop;
				HandpadFocusButtonPressed = No;
			}

		FocusControl_OnOff_16_17_Slow1_14();
	}
}

void FocusControl_OnOff_16_17_Slow1_14( void)
{
	BiDirInNibbleValue = BiDirInNibble();
	switch( FocusControl)
	{
		case FocusPlus:
			if( ReverseFocusMotor)
				FocusPulseBit = PPortPin14;
			else
				FocusPulseBit = PPortPin1;
			break;
		case FocusMinus:
			if( ReverseFocusMotor)
				FocusPulseBit = PPortPin1;
			else
				FocusPulseBit = PPortPin14;
			break;
		case FocusStop:
			FocusPulseBit = Off;
	}
	if( FocusPulseBit == PPortPin1)
		DisplayBiDirOut |= DisplayFocusOutBit;
	else
		if( FocusPulseBit == PPortPin14)
			DisplayBiDirOut |= DisplayFocusInBit;
	BiDirOutNibbleValue = (BiDirInNibbleValue & PPortPins16_17) + FocusPulseBit;
	BiDirOutNibble();
	FocusFastDisplayFlag = False;
}

void ProcessHPEventsMoveMotors( void)
{
	/* align motors to fullstep in case halfstepping chosen */
	AlignMs();
	SetCurrentAltazToAccumMs();
	/* if microstep switch selection on */
	if( MsSpeed)
	{
		StepsToMove.A = MsTick.A;
		StepsToMove.Z = MsTick.Z;
	}
	/* else halfstep */
	else
		StepsToMove.A = StepsToMove.Z = MaxHs;

	Steps.A = Steps.Z = 0;
	switch( Buttons)
	{
		case UpKey:
			Steps.A = StepsToMove.A;
			Dir.A = CW;
			break;
		case DownKey:
			Steps.A = StepsToMove.A;
			Dir.A = CCW;
			break;
		case CCWKey:
			Steps.Z = StepsToMove.Z;
			Dir.Z = CCW;
			break;
		case CWKey:
			Steps.Z = StepsToMove.Z;
			Dir.Z = CW;
	}
	if( MsSpeed)
		/* keeps control until Ms mvmt ends */
		AltazMoveMs();
	else
	{
		SlewStartedFromHandpad = True;
		KBEventMoveHs();
		SlewStartedFromHandpad = False;
		PauseUntilNewSidTime();
		HPEventGetEquat();
	}
}

void ProcessHPEventsModeSwitch( void)
{
	switch( HandpadFlag)
	{
		case HandpadOff:
			CheckMiscEvents();
			break;
		case GuideStayOn:
			if( !GuideFlag && Buttons & LeftKey && Two.Init)
			{
				InitGuide();
				WriteHandpadStatus();
			}
			else
				if( GuideFlag && Buttons & RightKey)
				{
					AlignMs();
					/* add accum guide, drift to current position */
					SetCurrentAltazToAccumMs();
					HPEventGetEquat();
					StopGuide();
					/* AccumMs = new altaz that includes accum guide=0, drift */
					SetAccumMsToCurrentAltaz();
					WriteHandpadStatus();
				}
				else
					CheckMiscEvents();
			break;
		case GrandTour:
			/* sets flag, then continues tracking, flag set to no in ProcessGrandTour() */
			if( Two.Init && (Buttons & LeftKey || Buttons & RightKey))
				GrandTourFlag = Yes;
			CheckMiscEvents();
			break;
		case ScrollTour:
		case ScrollAutoTour:
			/* sets flag, then continues tracking if possible; flag set to no in
			Move_Update_Handpad_Subr() called from ProcessScroll() */
			if( Buttons & LeftKey)
				ScrollFlag = Yes;
			CheckMiscEvents();
			break;
		case ToggleTrack:
			if( Buttons & LeftKey)
			{
				TrackFlag = On;
				WriteTrackStatus();
			}
			else
				if( Buttons & RightKey)
				{
					TrackFlag = Off;
					WriteTrackStatus();
				}
			CheckMiscEvents();
			break;
		case HandpadFocus:
			if( Buttons & LeftKey)
			{
				HandpadFocusMotorControlOnFlag = On;
				WriteHandpadStatus();
			}
			else
				if( Buttons & RightKey)
				{
					HandpadFocusMotorControlOnFlag = Off;
					WriteHandpadStatus();
				}
			CheckMiscEvents();
			break;
		case HandpadAux:
			if( Buttons & LeftKey)
			{
			/* turned off in ProcessHPEvents() */
				HandpadAuxControlOnFlag = On;
				WriteHandpadStatus();
			}
			CheckMiscEvents();
		default:
			CheckMiscEvents();
	}
}

void ProcessHPEvents( void)
{
	/* if a directional button is pressed, and the handpad mode is changed away from HandpadAux,
	call ProcessHPEventsHandpadAux() in order to set the bidirectional port pins properly */
	if( HandpadAuxButtonPressed && HandpadFlag != HandpadAux)
		ProcessHPEventsHandpadAux();
	/* similarly for focus */
	if( HandpadFocusButtonPressed && HandpadFlag != HandpadFocus)
		if( FocusMethod == FocusMethod_OnOff_16_17)
			ProcessHPEventsFocus_OnOff_16_17();
		else
			if( FocusMethod == FocusMethod_OnOff_16_17_Slow1_14)
				ProcessHPEventsFocus_OnOff_16_17_Slow1_14();

	/* set default so that upon entering handpad mode, buttons will initially control the A and Z
	motors, not the focus rotation motor */
	if( HandpadFocusMotorControlOnFlag && HandpadFlag != HandpadFocus)
		HandpadFocusMotorControlOnFlag = Off;

	/* to prevent too rapid init'ing */
	if( HandpadButtonTimer)
		HandpadButtonTimer--;
	/* to prevent too rapid recording of equat position */
	if( RecordEquatTimer)
		RecordEquatTimer--;

	/* check for handpad event... */
	ReadHandpad_f_ptr();

	/* if handpad mode is set to auxiliary output, then process auxiliary handpad control, whether a
	button is pressed or not */
	if( HandpadFlag == HandpadAux && HandpadAuxControlOnFlag)
	{
		if( Buttons & RightKey)
		{
			HandpadAuxControlOnFlag = Off;
			WriteHandpadStatus();
		}
		else
			ProcessHPEventsHandpadAux();
		CheckMiscEvents();
	}
	else
		/* similarly for focus mode */
		if( HandpadFlag == HandpadFocus && HandpadFocusMotorControlOnFlag
		&& (FocusMethod == FocusMethod_OnOff_16_17 || FocusMethod == FocusMethod_OnOff_16_17_Slow1_14))
		{
			if( Buttons & RightKey)
			{
				HandpadFocusMotorControlOnFlag = Off;
				WriteHandpadStatus();
			}
			else
				if( FocusMethod == FocusMethod_OnOff_16_17)
					ProcessHPEventsFocus_OnOff_16_17();
				else
					if( FocusMethod == FocusMethod_OnOff_16_17_Slow1_14)
						ProcessHPEventsFocus_OnOff_16_17_Slow1_14();
			CheckMiscEvents();
		}
		else
		{
			/* if direction button pressed: */
			if( Buttons & UpKey || Buttons & DownKey || Buttons & CCWKey || Buttons & CWKey)
			{
				/* if guide on, then use guiding speed */
				if( GuideFlag)
				{
					ProcessHPEventsGuide();
					MoveToCurrentRaDec();
				}
				else
					if( (FocusMethod == FocusMethod_Pulse_1_14 || FocusMethod == FocusMethod_Pulse_16_17)
					&& HandpadFocusMotorControlOnFlag && (Buttons & CCWKey || Buttons & CWKey))
					{
						InitiatedType = HandpadInitiated;
						if( TrackFlag)
							CheckMiscEvents();
						else
							ProcessHPEventsFocusMethod_Pulse_Dir();
					}
					else
						/* else either microstep or halfstep move: keeps control until microstep or
						halfstep movement ends */
						ProcessHPEventsMoveMotors();
			}
			/* else no valid direction buttons so look at mode switch and follow its commands; many
			commands	and no command at all end with CheckMiscEvents(); */
			else
				ProcessHPEventsModeSwitch();
		}
}

