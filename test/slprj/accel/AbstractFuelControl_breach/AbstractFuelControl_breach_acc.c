#if 0
This file is not available for use in any application other than as a MATLAB
( R ) MEX file for use with the Simulink ( R ) product . If you do not have
the Simulink Coder licensed , this file contains encrypted block names , etc
. If you purchase the Simulink Coder , this file will contain full block
descriptions and improved variable names .
#endif
#include "AbstractFuelControl_breach_acc.h"
#include "AbstractFuelControl_breach_acc_types.h"
#include "mwmathutil.h"
#include <float.h>
#include "rtwtypes.h"
#include "AbstractFuelControl_breach_acc_private.h"
#include "simstruc_types.h"
#include <stdio.h>
#include "slexec_vm_simstruct_bridge.h"
#include "slexec_vm_zc_functions.h"
#include "slexec_vm_lookup_functions.h"
#include "slsv_diagnostic_codegen_c_api.h"
#include "simtarget/slSimTgtMdlrefSfcnBridge.h"
#include "simstruc.h"
#include "fixedpoint.h"
#define CodeFormat S-Function
#define AccDefine1 Accelerator_S-Function
#include "simtarget/slAccSfcnBridge.h"
#ifndef __RTW_UTFREE__  
extern void * utMalloc ( size_t ) ; extern void utFree ( void * ) ;
#endif
boolean_T AbstractFuelControl_breach_acc_rt_TDelayUpdateTailOrGrowBuf ( int_T
* bufSzPtr , int_T * tailPtr , int_T * headPtr , int_T * lastPtr , real_T
tMinusDelay , real_T * * uBufPtr , boolean_T isfixedbuf , boolean_T
istransportdelay , int_T * maxNewBufSzPtr ) { int_T testIdx ; int_T tail = *
tailPtr ; int_T bufSz = * bufSzPtr ; real_T * tBuf = * uBufPtr + bufSz ;
real_T * xBuf = ( NULL ) ; int_T numBuffer = 2 ; if ( istransportdelay ) {
numBuffer = 3 ; xBuf = * uBufPtr + 2 * bufSz ; } testIdx = ( tail < ( bufSz -
1 ) ) ? ( tail + 1 ) : 0 ; if ( ( tMinusDelay <= tBuf [ testIdx ] ) && !
isfixedbuf ) { int_T j ; real_T * tempT ; real_T * tempU ; real_T * tempX = (
NULL ) ; real_T * uBuf = * uBufPtr ; int_T newBufSz = bufSz + 1024 ; if (
newBufSz > * maxNewBufSzPtr ) { * maxNewBufSzPtr = newBufSz ; } tempU = (
real_T * ) utMalloc ( numBuffer * newBufSz * sizeof ( real_T ) ) ; if ( tempU
== ( NULL ) ) { return ( false ) ; } tempT = tempU + newBufSz ; if (
istransportdelay ) tempX = tempT + newBufSz ; for ( j = tail ; j < bufSz ; j
++ ) { tempT [ j - tail ] = tBuf [ j ] ; tempU [ j - tail ] = uBuf [ j ] ; if
( istransportdelay ) tempX [ j - tail ] = xBuf [ j ] ; } for ( j = 0 ; j <
tail ; j ++ ) { tempT [ j + bufSz - tail ] = tBuf [ j ] ; tempU [ j + bufSz -
tail ] = uBuf [ j ] ; if ( istransportdelay ) tempX [ j + bufSz - tail ] =
xBuf [ j ] ; } if ( * lastPtr > tail ) { * lastPtr -= tail ; } else { *
lastPtr += ( bufSz - tail ) ; } * tailPtr = 0 ; * headPtr = bufSz ; utFree (
uBuf ) ; * bufSzPtr = newBufSz ; * uBufPtr = tempU ; } else { * tailPtr =
testIdx ; } return ( true ) ; } real_T
AbstractFuelControl_breach_acc_rt_VTDelayfindtDInterpolate ( real_T x ,
real_T * uBuf , int_T bufSz , int_T head , int_T tail , int_T * pLast ,
real_T t , real_T tStart , boolean_T discrete , boolean_T
minorStepAndTAtLastMajorOutput , real_T initOutput , real_T * appliedDelay )
{ int_T n , k ; real_T f ; int_T kp1 ; real_T tminustD = 0 ; real_T tL = 0 ;
real_T tR = 0 ; real_T uL = 0 ; real_T uR = 0 ; real_T uD , fU ; real_T *
tBuf = uBuf + bufSz ; real_T * xBuf = uBuf + 2 * bufSz ; if (
minorStepAndTAtLastMajorOutput ) { if ( * pLast == head ) { * pLast = ( *
pLast == 0 ) ? bufSz - 1 : * pLast - 1 ; } head = ( head == 0 ) ? bufSz - 1 :
head - 1 ; } if ( x <= 1 ) { return initOutput ; } k = * pLast ; n = 0 ; for
( ; ; ) { n ++ ; if ( n > bufSz ) break ; if ( x - xBuf [ k ] > 1.0 ) { if (
k == head ) { int_T km1 ; f = ( x - 1.0 - xBuf [ k ] ) / ( x - xBuf [ k ] ) ;
tminustD = ( 1.0 - f ) * tBuf [ k ] + f * t ; km1 = k - 1 ; if ( km1 < 0 )
km1 = bufSz - 1 ; tL = tBuf [ km1 ] ; tR = tBuf [ k ] ; uL = uBuf [ km1 ] ;
uR = uBuf [ k ] ; break ; } kp1 = k + 1 ; if ( kp1 == bufSz ) kp1 = 0 ; if (
x - xBuf [ kp1 ] <= 1.0 ) { f = ( x - 1.0 - xBuf [ k ] ) / ( xBuf [ kp1 ] -
xBuf [ k ] ) ; tL = tBuf [ k ] ; tR = tBuf [ kp1 ] ; uL = uBuf [ k ] ; uR =
uBuf [ kp1 ] ; tminustD = ( 1.0 - f ) * tL + f * tR ; break ; } k = kp1 ; }
else { if ( k == tail ) { f = ( x - 1.0 ) / xBuf [ k ] ; if ( discrete ) {
return ( uBuf [ tail ] ) ; } kp1 = k + 1 ; if ( kp1 == bufSz ) kp1 = 0 ;
tminustD = ( 1 - f ) * tStart + f * tBuf [ k ] ; tL = tBuf [ k ] ; tR = tBuf
[ kp1 ] ; uL = uBuf [ k ] ; uR = uBuf [ kp1 ] ; break ; } k = k - 1 ; if ( k
< 0 ) k = bufSz - 1 ; } } * pLast = k ; if ( tR == tL ) { fU = 1.0 ; } else {
fU = ( tminustD - tL ) / ( tR - tL ) ; } if ( discrete ) { uD = ( fU > ( 1.0
- fU ) ) ? uR : uL ; } else { uD = ( 1.0 - fU ) * uL + fU * uR ; } *
appliedDelay = t - tminustD ; return uD ; } real_T look2_binlxpw ( real_T u0
, real_T u1 , const real_T bp0 [ ] , const real_T bp1 [ ] , const real_T
table [ ] , const uint32_T maxIndex [ ] , uint32_T stride ) { real_T
fractions [ 2 ] ; real_T frac ; real_T yL_0d0 ; real_T yL_0d1 ; uint32_T
bpIndices [ 2 ] ; uint32_T bpIdx ; uint32_T iLeft ; uint32_T iRght ; if ( u0
<= bp0 [ 0U ] ) { iLeft = 0U ; frac = ( u0 - bp0 [ 0U ] ) / ( bp0 [ 1U ] -
bp0 [ 0U ] ) ; } else if ( u0 < bp0 [ maxIndex [ 0U ] ] ) { bpIdx = maxIndex
[ 0U ] >> 1U ; iLeft = 0U ; iRght = maxIndex [ 0U ] ; while ( iRght - iLeft >
1U ) { if ( u0 < bp0 [ bpIdx ] ) { iRght = bpIdx ; } else { iLeft = bpIdx ; }
bpIdx = ( iRght + iLeft ) >> 1U ; } frac = ( u0 - bp0 [ iLeft ] ) / ( bp0 [
iLeft + 1U ] - bp0 [ iLeft ] ) ; } else { iLeft = maxIndex [ 0U ] - 1U ; frac
= ( u0 - bp0 [ maxIndex [ 0U ] - 1U ] ) / ( bp0 [ maxIndex [ 0U ] ] - bp0 [
maxIndex [ 0U ] - 1U ] ) ; } fractions [ 0U ] = frac ; bpIndices [ 0U ] =
iLeft ; if ( u1 <= bp1 [ 0U ] ) { iLeft = 0U ; frac = ( u1 - bp1 [ 0U ] ) / (
bp1 [ 1U ] - bp1 [ 0U ] ) ; } else if ( u1 < bp1 [ maxIndex [ 1U ] ] ) {
bpIdx = maxIndex [ 1U ] >> 1U ; iLeft = 0U ; iRght = maxIndex [ 1U ] ; while
( iRght - iLeft > 1U ) { if ( u1 < bp1 [ bpIdx ] ) { iRght = bpIdx ; } else {
iLeft = bpIdx ; } bpIdx = ( iRght + iLeft ) >> 1U ; } frac = ( u1 - bp1 [
iLeft ] ) / ( bp1 [ iLeft + 1U ] - bp1 [ iLeft ] ) ; } else { iLeft =
maxIndex [ 1U ] - 1U ; frac = ( u1 - bp1 [ maxIndex [ 1U ] - 1U ] ) / ( bp1 [
maxIndex [ 1U ] ] - bp1 [ maxIndex [ 1U ] - 1U ] ) ; } bpIdx = iLeft * stride
+ bpIndices [ 0U ] ; yL_0d0 = table [ bpIdx ] ; yL_0d0 += ( table [ bpIdx +
1U ] - yL_0d0 ) * fractions [ 0U ] ; bpIdx += stride ; yL_0d1 = table [ bpIdx
] ; return ( ( ( table [ bpIdx + 1U ] - yL_0d1 ) * fractions [ 0U ] + yL_0d1
) - yL_0d0 ) * frac + yL_0d0 ; } void rt_ssGetBlockPath ( void * S , int_T
sysIdx , int_T blkIdx , char_T * * path ) { _ssGetBlockPath ( ( SimStruct * )
S , sysIdx , blkIdx , path ) ; } void rt_ssSet_slErrMsg ( void * S , void *
diag ) { SimStruct * castedS = ( SimStruct * ) S ; if ( !
_ssIsErrorStatusAslErrMsg ( castedS ) ) { _ssSet_slErrMsg ( castedS , diag )
; } else { _ssDiscardDiagnostic ( castedS , diag ) ; } } void
rt_ssReportDiagnosticAsWarning ( void * S , void * diag ) {
_ssReportDiagnosticAsWarning ( ( SimStruct * ) S , diag ) ; } void
rt_ssReportDiagnosticAsInfo ( void * S , void * diag ) {
_ssReportDiagnosticAsInfo ( ( SimStruct * ) S , diag ) ; } static void
mdlOutputs ( SimStruct * S , int_T tid ) { B_AbstractFuelControl_breach_T *
_rtB ; DW_AbstractFuelControl_breach_T * _rtDW ;
P_AbstractFuelControl_breach_T * _rtP ; PrevZCX_AbstractFuelControl_breach_T
* _rtZCE ; X_AbstractFuelControl_breach_T * _rtX ; real_T ratio ; real_T
rtb_B_13_73_520 ; real_T rtb_B_15_66_464 ; real_T rtb_B_15_69_488 ; real_T
taskTimeV ; int32_T isHit ; real32_T rtb_B_13_81_564 ; real32_T
rtb_B_15_76_544 ; real32_T rtb_B_15_77_548 ; uint32_T numCycles ; boolean_T
rtb_B_13_124_736 ; ZCEventType zcEvent ; _rtDW = ( (
DW_AbstractFuelControl_breach_T * ) ssGetRootDWork ( S ) ) ; _rtZCE = ( (
PrevZCX_AbstractFuelControl_breach_T * ) _ssGetPrevZCSigState ( S ) ) ; _rtX
= ( ( X_AbstractFuelControl_breach_T * ) ssGetContStates ( S ) ) ; _rtP = ( (
P_AbstractFuelControl_breach_T * ) ssGetModelRtp ( S ) ) ; _rtB = ( (
B_AbstractFuelControl_breach_T * ) _ssGetModelBlockIO ( S ) ) ; _rtB ->
B_15_2_16 = _rtX -> p00543bar_CSTATE ; if ( ( (
ExternalUPtrs_AbstractFuelControl_breach_T * ) ssGetU ( S ) ) -> Engine_Speed
> _rtP -> P_8 ) { _rtB -> B_15_3_24 = _rtP -> P_8 ; } else if ( ( (
ExternalUPtrs_AbstractFuelControl_breach_T * ) ssGetU ( S ) ) -> Engine_Speed
< _rtP -> P_9 ) { _rtB -> B_15_3_24 = _rtP -> P_9 ; } else { _rtB ->
B_15_3_24 = ( ( ExternalUPtrs_AbstractFuelControl_breach_T * ) ssGetU ( S ) )
-> Engine_Speed ; } _rtB -> B_15_4_32 = _rtP -> P_10 * _rtB -> B_15_3_24 ;
_rtB -> B_15_5_40 = ( ( ( 0.08979 * _rtB -> B_15_2_16 * _rtB -> B_15_4_32 -
0.366 ) - 0.0337 * _rtB -> B_15_4_32 * _rtB -> B_15_2_16 * _rtB -> B_15_2_16
) + 0.0001 * _rtB -> B_15_2_16 * _rtB -> B_15_4_32 * _rtB -> B_15_4_32 ) *
_rtP -> P_86 ; { if ( _rtDW ->
TAQSigLogging_InsertedFor_IntakeManifold_at_outport_0_PWORK . AQHandles &&
ssGetLogOutput ( S ) ) { sdiWriteSignal ( _rtDW ->
TAQSigLogging_InsertedFor_IntakeManifold_at_outport_0_PWORK . AQHandles ,
ssGetTaskTime ( S , 0 ) , ( char * ) & _rtB -> B_15_5_40 + 0 ) ; } } { if (
_rtDW -> TAQSigLogging_InsertedFor_IntakeManifold_at_outport_1_PWORK .
AQHandles && ssGetLogOutput ( S ) ) { sdiWriteSignal ( _rtDW ->
TAQSigLogging_InsertedFor_IntakeManifold_at_outport_1_PWORK . AQHandles ,
ssGetTaskTime ( S , 0 ) , ( char * ) & _rtB -> B_15_2_16 + 0 ) ; } } _rtB ->
B_15_6_48 = 0.0 ; _rtB -> B_15_6_48 += _rtP -> P_12 * _rtX ->
Throttledelay_CSTATE ; _rtB -> B_15_7_56 = _rtB -> B_15_6_48 + _rtB ->
B_15_44_352 ; if ( _rtB -> B_15_7_56 > _rtP -> P_13 ) { _rtB -> B_15_8_64 =
_rtP -> P_13 ; } else if ( _rtB -> B_15_7_56 < _rtP -> P_14 ) { _rtB ->
B_15_8_64 = _rtP -> P_14 ; } else { _rtB -> B_15_8_64 = _rtB -> B_15_7_56 ; }
_rtB -> B_15_9_72 = _rtB -> B_15_2_16 / _rtB -> B_15_43_344 ; _rtB ->
B_15_10_80 = 1.0 / _rtB -> B_15_2_16 * _rtB -> B_15_43_344 ; _rtB ->
B_15_11_88 = muDoubleScalarMin ( _rtB -> B_15_9_72 , _rtB -> B_15_10_80 ) ;
if ( _rtB -> B_15_11_88 >= _rtP -> P_15 ) { rtb_B_15_66_464 = _rtB ->
B_15_11_88 - _rtB -> B_15_11_88 * _rtB -> B_15_11_88 ; if ( rtb_B_15_66_464 <
0.0 ) { rtb_B_15_69_488 = - muDoubleScalarSqrt ( - rtb_B_15_66_464 ) ; } else
{ rtb_B_15_69_488 = muDoubleScalarSqrt ( rtb_B_15_66_464 ) ; } _rtB ->
B_15_12_96 = 2.0 * rtb_B_15_69_488 ; } else { _rtB -> B_15_12_96 = _rtB ->
B_15_46_368 ; } _rtB -> B_15_13_104 = _rtB -> B_15_43_344 - _rtB -> B_15_2_16
; _rtB -> B_15_14_112 = muDoubleScalarSign ( _rtB -> B_15_13_104 ) ; _rtB ->
B_15_15_120 = ( ( ( 2.821 - 0.05231 * _rtB -> B_15_8_64 ) + 0.10299 * _rtB ->
B_15_8_64 * _rtB -> B_15_8_64 ) - 0.00063 * _rtB -> B_15_8_64 * _rtB ->
B_15_8_64 * _rtB -> B_15_8_64 ) * _rtB -> B_15_12_96 * _rtB -> B_15_14_112 ;
{ if ( _rtDW -> TAQSigLogging_InsertedFor_Throttle_at_outport_0_PWORK .
AQHandles && ssGetLogOutput ( S ) ) { sdiWriteSignal ( _rtDW ->
TAQSigLogging_InsertedFor_Throttle_at_outport_0_PWORK . AQHandles ,
ssGetTaskTime ( S , 0 ) , ( char * ) & _rtB -> B_15_15_120 + 0 ) ; } } _rtB
-> B_15_16_128 = _rtP -> P_16 * _rtB -> B_15_4_32 ; _rtB -> B_15_17_136 =
_rtB -> B_15_5_40 / _rtB -> B_15_4_32 ; _rtB -> B_15_18_144 = _rtP -> P_17 *
_rtB -> B_15_17_136 ; _rtB -> B_15_19_152 = _rtP -> P_21 * look2_binlxpw (
_rtB -> B_15_16_128 , _rtB -> B_15_18_144 , _rtP -> P_19 , _rtP -> P_20 ,
_rtP -> P_18 , _rtP -> P_87 , 5U ) ; rtb_B_15_76_544 = ( real32_T ) _rtB ->
B_15_4_32 ; rtb_B_15_77_548 = ( real32_T ) _rtB -> B_15_8_64 ;
rtb_B_15_66_464 = _rtP -> P_22 * _rtB -> B_15_15_120 ; _rtB -> B_15_20_160 =
_rtX -> Integrator_CSTATE ; _rtB -> B_15_21_168 = _rtP -> P_27 * _rtB ->
B_15_20_160 ; if ( ssGetTaskTime ( S , 0 ) < _rtP -> P_23 ) { rtb_B_15_69_488
= _rtP -> P_24 ; } else { rtb_B_15_69_488 = _rtP -> P_25 ; } if (
rtb_B_15_69_488 >= _rtP -> P_28 ) { rtb_B_15_69_488 = _rtB -> B_15_45_360 ; }
else { rtb_B_15_69_488 = _rtB -> B_15_21_168 ; } rtb_B_15_69_488 *= _rtP ->
P_29 ; if ( ssGetTaskTime ( S , 0 ) < _rtP -> P_0 ) { rtb_B_13_73_520 = _rtP
-> P_1 ; } else { rtb_B_13_73_520 = _rtP -> P_2 ; } isHit = ssIsSampleHit ( S
, 3 , 0 ) ; if ( isHit != 0 ) { taskTimeV = ssGetTaskTime ( S , 3 ) ; if (
ssGetTNextWasAdjusted ( S , 3 ) != 0 ) { _rtDW -> nextTime =
_ssGetVarNextHitTime ( S , 0 ) ; } if ( _rtDW -> justEnabled != 0 ) { _rtDW
-> justEnabled = 0 ; if ( taskTimeV >= _rtP -> P_6 ) { ratio = ( taskTimeV -
_rtP -> P_6 ) / _rtP -> P_4 ; numCycles = ( uint32_T ) muDoubleScalarFloor (
ratio ) ; if ( muDoubleScalarAbs ( ( real_T ) ( numCycles + 1U ) - ratio ) <
DBL_EPSILON * ratio ) { numCycles ++ ; } _rtDW -> numCompleteCycles =
numCycles ; ratio = ( ( real_T ) numCycles * _rtP -> P_4 + _rtP -> P_6 ) +
_rtP -> P_5 * _rtP -> P_4 / 100.0 ; if ( taskTimeV < ratio ) { _rtDW ->
currentValue = 1 ; _rtDW -> nextTime = ratio ; } else { _rtDW -> currentValue
= 0 ; _rtDW -> nextTime = ( real_T ) ( numCycles + 1U ) * _rtP -> P_4 + _rtP
-> P_6 ; } } else { if ( _rtP -> P_6 != 0.0 ) { _rtDW -> numCompleteCycles =
- 1L ; } else { _rtDW -> numCompleteCycles = 0L ; } _rtDW -> currentValue = 0
; _rtDW -> nextTime = _rtP -> P_6 ; } } else if ( _rtDW -> nextTime <=
taskTimeV ) { if ( _rtDW -> currentValue == 1 ) { _rtDW -> currentValue = 0 ;
_rtDW -> nextTime = ( real_T ) ( _rtDW -> numCompleteCycles + 1L ) * _rtP ->
P_4 + _rtP -> P_6 ; } else { _rtDW -> numCompleteCycles ++ ; _rtDW ->
currentValue = 1 ; _rtDW -> nextTime = ( _rtP -> P_5 * _rtP -> P_4 * 0.01 + (
real_T ) _rtDW -> numCompleteCycles * _rtP -> P_4 ) + _rtP -> P_6 ; } }
_ssSetVarNextHitTime ( S , 0 , _rtDW -> nextTime ) ; if ( _rtDW ->
currentValue == 1 ) { _rtB -> B_13_48_384 = _rtP -> P_3 ; } else { _rtB ->
B_13_48_384 = 0.0 ; } } _rtDW -> engine_speed = rtb_B_15_76_544 ; _rtDW ->
throttle_angle = rtb_B_15_77_548 ; _rtDW -> throttle_flow = ( real32_T )
rtb_B_15_66_464 ; _rtDW -> airbyfuel_meas = ( real32_T ) rtb_B_15_69_488 ;
_rtB -> B_12_49_392 = rtb_B_13_73_520 ; isHit = ssIsSampleHit ( S , 1 , 0 ) ;
if ( ( isHit != 0 ) && ssIsModeUpdateTimeStep ( S ) ) { zcEvent = rt_ZCFcn (
RISING_ZERO_CROSSING , & _rtZCE -> fuel_controller_pwon_Trig_ZCE , _rtB ->
B_12_49_392 ) ; if ( zcEvent != NO_ZCEVENT ) { _rtDW -> controller_mode = (
_rtP -> P_77 != 0.0F ) ; _rtDW -> commanded_fuel = _rtP -> P_78 ; _rtDW ->
airbyfuel_ref = _rtP -> P_79 ; _rtDW -> fuel_controller_pwon_SubsysRanBC = 4
; } zcEvent = rt_ZCFcn ( RISING_ZERO_CROSSING , & _rtZCE ->
fuel_controller_mode_10ms_Trig_ZCE , _rtB -> B_13_48_384 ) ; if ( zcEvent !=
NO_ZCEVENT ) { _rtB -> B_9_59_436 = ( ( _rtDW -> airbyfuel_meas <= _rtB ->
B_9_50_400 ) || _rtDW -> UnitDelay_DSTATE ) ; _rtB -> B_7_53_412 = _rtDW ->
UnitDelay2_DSTATE + _rtP -> P_72 ; _rtB -> B_7_61_438 = ( ( _rtB ->
B_7_53_412 >= _rtP -> P_73 ) || _rtDW -> UnitDelay1_DSTATE_e ) ; if ( _rtDW
-> UnitDelay1_DSTATE_a ) { rtb_B_15_76_544 = _rtB -> B_8_51_404 ; } else {
rtb_B_15_76_544 = _rtB -> B_8_52_408 ; } _rtB -> B_8_60_437 = ( _rtDW ->
throttle_angle >= rtb_B_15_76_544 ) ; _rtDW -> controller_mode = ( _rtB ->
B_9_59_436 || ( ! _rtB -> B_7_61_438 ) || _rtB -> B_8_60_437 ) ; if ( _rtB ->
B_7_61_438 && _rtB -> B_8_60_437 ) { _rtDW -> airbyfuel_ref = _rtP -> P_69 ;
} else { _rtDW -> airbyfuel_ref = _rtP -> P_70 ; } _rtDW -> UnitDelay_DSTATE
= _rtB -> B_9_59_436 ; _rtDW -> UnitDelay2_DSTATE = _rtB -> B_7_53_412 ;
_rtDW -> UnitDelay1_DSTATE_e = _rtB -> B_7_61_438 ; _rtDW ->
UnitDelay1_DSTATE_a = _rtB -> B_8_60_437 ; _rtDW ->
fuel_controller_mode_10ms_SubsysRanBC = 4 ; } zcEvent = rt_ZCFcn (
RISING_ZERO_CROSSING , & _rtZCE -> fuel_controller_10ms_Trig_ZCE , _rtB ->
B_13_48_384 ) ; if ( zcEvent != NO_ZCEVENT ) { rtb_B_15_76_544 = ( ( _rtDW ->
UnitDelay1_DSTATE_d * _rtDW -> engine_speed * _rtP -> P_56 + _rtP -> P_55 ) +
_rtDW -> UnitDelay1_DSTATE_d * _rtDW -> UnitDelay1_DSTATE_d * _rtDW ->
engine_speed * _rtP -> P_57 ) + _rtDW -> engine_speed * _rtDW -> engine_speed
* _rtDW -> UnitDelay1_DSTATE_d * _rtP -> P_58 ; _rtB -> B_1_58_432 = ( _rtDW
-> throttle_flow - rtb_B_15_76_544 ) * _rtP -> P_60 * _rtP -> P_54 + _rtDW ->
UnitDelay1_DSTATE_d ; _rtB -> B_4_62_439 = ! _rtDW -> controller_mode ; if (
_rtB -> B_4_62_439 ) { rtb_B_15_77_548 = _rtDW -> airbyfuel_meas - _rtDW ->
airbyfuel_ref ; _rtB -> B_2_56_424 = _rtP -> P_63 * rtb_B_15_77_548 * _rtP ->
P_61 + _rtDW -> UnitDelay1_DSTATE ; _rtB -> B_2_57_428 = _rtP -> P_62 *
rtb_B_15_77_548 + _rtB -> B_2_56_424 ; _rtDW ->
feedback_PI_controller_SubsysRanBC = 4 ; } if ( _rtDW -> controller_mode ) {
rtb_B_15_77_548 = _rtB -> B_4_55_420 ; } else { rtb_B_15_77_548 = _rtB ->
B_4_54_416 + _rtB -> B_2_57_428 ; if ( rtb_B_15_77_548 > _rtP -> P_52 ) {
rtb_B_15_77_548 = _rtP -> P_52 ; } else if ( rtb_B_15_77_548 < _rtP -> P_53 )
{ rtb_B_15_77_548 = _rtP -> P_53 ; } } rtb_B_15_76_544 = rtb_B_15_76_544 /
_rtDW -> airbyfuel_ref * rtb_B_15_77_548 ; if ( rtb_B_15_76_544 > _rtP ->
P_65 ) { _rtDW -> commanded_fuel = _rtP -> P_65 ; } else if ( rtb_B_15_76_544
< _rtP -> P_66 ) { _rtDW -> commanded_fuel = _rtP -> P_66 ; } else { _rtDW ->
commanded_fuel = rtb_B_15_76_544 ; } _rtDW -> UnitDelay1_DSTATE_d = _rtB ->
B_1_58_432 ; if ( _rtB -> B_4_62_439 ) { _rtDW -> UnitDelay1_DSTATE = _rtB ->
B_2_56_424 ; } _rtDW -> fuel_controller_10ms_SubsysRanBC = 4 ; } } isHit =
ssIsSampleHit ( S , 2 , 0 ) ; if ( isHit != 0 ) { rtb_B_13_124_736 = _rtDW ->
controller_mode ; rtb_B_13_81_564 = _rtDW -> airbyfuel_ref ; _rtB ->
B_15_22_176 = _rtP -> P_30 * _rtDW -> commanded_fuel ; } _rtB -> B_15_23_184
= _rtB -> B_15_19_152 * _rtB -> B_15_22_176 ; _rtB -> B_15_24_192 = _rtX ->
Integrator_CSTATE_c ; _rtB -> B_15_25_200 = _rtP -> P_35 * look2_binlxpw (
_rtB -> B_15_16_128 , _rtB -> B_15_18_144 , _rtP -> P_33 , _rtP -> P_34 ,
_rtP -> P_32 , _rtP -> P_88 , 5U ) ; _rtB -> B_15_26_208 = _rtB ->
B_15_24_192 / _rtB -> B_15_25_200 ; _rtB -> B_15_27_216 = _rtB -> B_15_23_184
+ _rtB -> B_15_26_208 ; { if ( _rtDW ->
TAQSigLogging_InsertedFor_Wallwetting_at_outport_0_PWORK . AQHandles &&
ssGetLogOutput ( S ) ) { sdiWriteSignal ( _rtDW ->
TAQSigLogging_InsertedFor_Wallwetting_at_outport_0_PWORK . AQHandles ,
ssGetTaskTime ( S , 0 ) , ( char * ) & _rtB -> B_15_27_216 + 0 ) ; } } _rtB
-> B_15_28_224 = _rtX -> Integrator_CSTATE_h ; _rtB -> B_15_29_232 = _rtB ->
B_15_28_224 - _rtB -> B_15_20_160 ; _rtB -> B_15_30_240 = _rtP -> P_37 * _rtB
-> B_15_29_232 ; _rtB -> B_15_31_248 = _rtB -> B_15_5_40 / _rtB ->
B_15_27_216 ; { real_T * * uBuffer = ( real_T * * ) & _rtDW ->
fuelsystemtransportdelay_PWORK . TUbufferPtrs [ 0 ] ; real_T simTime = ssGetT
( S ) ; real_T appliedDelay ; _rtB -> B_15_32_256 =
AbstractFuelControl_breach_acc_rt_VTDelayfindtDInterpolate ( ( (
X_AbstractFuelControl_breach_T * ) ssGetContStates ( S ) ) ->
fuelsystemtransportdelay_CSTATE , * uBuffer , _rtDW ->
fuelsystemtransportdelay_IWORK . CircularBufSize , _rtDW ->
fuelsystemtransportdelay_IWORK . Head , _rtDW ->
fuelsystemtransportdelay_IWORK . Tail , & _rtDW ->
fuelsystemtransportdelay_IWORK . Last , simTime , 0.0 , 0 , ( boolean_T ) (
ssIsMinorTimeStep ( S ) && ( ( * uBuffer + _rtDW ->
fuelsystemtransportdelay_IWORK . CircularBufSize ) [ _rtDW ->
fuelsystemtransportdelay_IWORK . Head ] == ssGetT ( S ) ) ) , _rtP -> P_39 ,
& appliedDelay ) ; } _rtB -> B_15_33_264 = _rtB -> B_15_32_256 - _rtB ->
B_15_28_224 ; _rtB -> B_15_34_272 = _rtP -> P_40 * _rtB -> B_15_33_264 ; _rtB
-> B_15_35_280 = _rtP -> P_41 * _rtB -> B_15_4_32 ; _rtB -> B_15_36_288 =
look2_binlxpw ( _rtB -> B_15_35_280 , _rtB -> B_15_18_144 , _rtP -> P_43 ,
_rtP -> P_44 , _rtP -> P_42 , _rtP -> P_89 , 5U ) ; _rtB -> B_15_37_296 =
_rtB -> B_15_15_120 - _rtB -> B_15_5_40 ; _rtB -> B_15_38_304 = _rtP -> P_45
* _rtB -> B_15_37_296 ; _rtB -> B_15_39_312 = _rtP -> P_46 * _rtB ->
B_15_19_152 ; _rtB -> B_15_40_320 = _rtB -> B_15_39_312 + _rtB -> B_15_47_376
; _rtB -> B_15_41_328 = _rtB -> B_15_22_176 * _rtB -> B_15_40_320 ; _rtB ->
B_15_42_336 = _rtB -> B_15_41_328 - _rtB -> B_15_26_208 ; isHit =
ssIsSampleHit ( S , 2 , 0 ) ; if ( isHit != 0 ) { _rtB -> B_16_0_0 =
rtb_B_13_81_564 ; ssCallAccelRunBlock ( S , 16 , 2 , SS_CALL_MDL_OUTPUTS ) ;
} ssCallAccelRunBlock ( S , 16 , 3 , SS_CALL_MDL_OUTPUTS ) ; isHit =
ssIsSampleHit ( S , 2 , 0 ) ; if ( isHit != 0 ) { _rtB -> B_16_1_8 =
rtb_B_13_124_736 ; ssCallAccelRunBlock ( S , 16 , 5 , SS_CALL_MDL_OUTPUTS ) ;
} { if ( _rtDW -> TAQSigLogging_InsertedFor_EngineSpeed_at_outport_0_PWORK .
AQHandles && ssGetLogOutput ( S ) ) { sdiWriteSignal ( _rtDW ->
TAQSigLogging_InsertedFor_EngineSpeed_at_outport_0_PWORK . AQHandles ,
ssGetTaskTime ( S , 0 ) , ( char * ) & ( (
ExternalUPtrs_AbstractFuelControl_breach_T * ) ssGetU ( S ) ) -> Engine_Speed
+ 0 ) ; } } { if ( _rtDW ->
TAQSigLogging_InsertedFor_PedalAngle_at_outport_0_PWORK . AQHandles &&
ssGetLogOutput ( S ) ) { sdiWriteSignal ( _rtDW ->
TAQSigLogging_InsertedFor_PedalAngle_at_outport_0_PWORK . AQHandles ,
ssGetTaskTime ( S , 0 ) , ( char * ) & ( (
ExternalUPtrs_AbstractFuelControl_breach_T * ) ssGetU ( S ) ) -> Pedal_Angle
+ 0 ) ; } } UNUSED_PARAMETER ( tid ) ; } static void mdlOutputsTID4 (
SimStruct * S , int_T tid ) { B_AbstractFuelControl_breach_T * _rtB ;
P_AbstractFuelControl_breach_T * _rtP ; _rtP = ( (
P_AbstractFuelControl_breach_T * ) ssGetModelRtp ( S ) ) ; _rtB = ( (
B_AbstractFuelControl_breach_T * ) _ssGetModelBlockIO ( S ) ) ; _rtB ->
B_8_51_404 = _rtP -> P_74 ; _rtB -> B_8_52_408 = _rtB -> B_8_51_404 + _rtP ->
P_75 ; _rtB -> B_9_50_400 = _rtP -> P_76 ; _rtB -> B_4_54_416 = _rtP -> P_67
; _rtB -> B_4_55_420 = _rtP -> P_68 ; _rtB -> B_15_43_344 = _rtP -> P_47 ;
_rtB -> B_15_44_352 = _rtP -> P_48 ; _rtB -> B_15_45_360 = _rtP -> P_49 ;
_rtB -> B_15_46_368 = _rtP -> P_50 ; _rtB -> B_15_47_376 = _rtP -> P_51 ;
UNUSED_PARAMETER ( tid ) ; }
#define MDL_UPDATE
static void mdlUpdate ( SimStruct * S , int_T tid ) {
B_AbstractFuelControl_breach_T * _rtB ; DW_AbstractFuelControl_breach_T *
_rtDW ; P_AbstractFuelControl_breach_T * _rtP ; _rtDW = ( (
DW_AbstractFuelControl_breach_T * ) ssGetRootDWork ( S ) ) ; _rtP = ( (
P_AbstractFuelControl_breach_T * ) ssGetModelRtp ( S ) ) ; _rtB = ( (
B_AbstractFuelControl_breach_T * ) _ssGetModelBlockIO ( S ) ) ; { real_T * *
uBuffer = ( real_T * * ) & _rtDW -> fuelsystemtransportdelay_PWORK .
TUbufferPtrs [ 0 ] ; real_T simTime = ssGetT ( S ) ; _rtDW ->
fuelsystemtransportdelay_IWORK . Head = ( ( _rtDW ->
fuelsystemtransportdelay_IWORK . Head < ( _rtDW ->
fuelsystemtransportdelay_IWORK . CircularBufSize - 1 ) ) ? ( _rtDW ->
fuelsystemtransportdelay_IWORK . Head + 1 ) : 0 ) ; if ( _rtDW ->
fuelsystemtransportdelay_IWORK . Head == _rtDW ->
fuelsystemtransportdelay_IWORK . Tail ) { if ( !
AbstractFuelControl_breach_acc_rt_TDelayUpdateTailOrGrowBuf ( & _rtDW ->
fuelsystemtransportdelay_IWORK . CircularBufSize , & _rtDW ->
fuelsystemtransportdelay_IWORK . Tail , & _rtDW ->
fuelsystemtransportdelay_IWORK . Head , & _rtDW ->
fuelsystemtransportdelay_IWORK . Last , simTime - _rtP -> P_38 , uBuffer , (
boolean_T ) 0 , ( boolean_T ) 1 , & _rtDW -> fuelsystemtransportdelay_IWORK .
MaxNewBufSize ) ) { ssSetErrorStatus ( S , "vtdelay memory allocation error"
) ; return ; } } ( * uBuffer + _rtDW -> fuelsystemtransportdelay_IWORK .
CircularBufSize ) [ _rtDW -> fuelsystemtransportdelay_IWORK . Head ] =
simTime ; ( * uBuffer + 2 * _rtDW -> fuelsystemtransportdelay_IWORK .
CircularBufSize ) [ _rtDW -> fuelsystemtransportdelay_IWORK . Head ] = ( (
X_AbstractFuelControl_breach_T * ) ssGetContStates ( S ) ) ->
fuelsystemtransportdelay_CSTATE ; ( * uBuffer ) [ _rtDW ->
fuelsystemtransportdelay_IWORK . Head ] = _rtB -> B_15_31_248 ; }
UNUSED_PARAMETER ( tid ) ; }
#define MDL_UPDATE
static void mdlUpdateTID4 ( SimStruct * S , int_T tid ) { UNUSED_PARAMETER (
tid ) ; }
#define MDL_DERIVATIVES
static void mdlDerivatives ( SimStruct * S ) { B_AbstractFuelControl_breach_T
* _rtB ; DW_AbstractFuelControl_breach_T * _rtDW ;
P_AbstractFuelControl_breach_T * _rtP ; XDot_AbstractFuelControl_breach_T *
_rtXdot ; X_AbstractFuelControl_breach_T * _rtX ; _rtDW = ( (
DW_AbstractFuelControl_breach_T * ) ssGetRootDWork ( S ) ) ; _rtXdot = ( (
XDot_AbstractFuelControl_breach_T * ) ssGetdX ( S ) ) ; _rtX = ( (
X_AbstractFuelControl_breach_T * ) ssGetContStates ( S ) ) ; _rtP = ( (
P_AbstractFuelControl_breach_T * ) ssGetModelRtp ( S ) ) ; _rtB = ( (
B_AbstractFuelControl_breach_T * ) _ssGetModelBlockIO ( S ) ) ; _rtXdot ->
p00543bar_CSTATE = _rtB -> B_15_38_304 ; _rtXdot -> Throttledelay_CSTATE =
0.0 ; _rtXdot -> Throttledelay_CSTATE += _rtP -> P_11 * _rtX ->
Throttledelay_CSTATE ; _rtXdot -> Throttledelay_CSTATE += ( (
ExternalUPtrs_AbstractFuelControl_breach_T * ) ssGetU ( S ) ) -> Pedal_Angle
; _rtXdot -> Integrator_CSTATE = _rtB -> B_15_30_240 ; _rtXdot ->
Integrator_CSTATE_c = _rtB -> B_15_42_336 ; _rtXdot -> Integrator_CSTATE_h =
_rtB -> B_15_34_272 ; { real_T instantDelay ; instantDelay = _rtB ->
B_15_36_288 ; if ( instantDelay > _rtP -> P_38 ) { instantDelay = _rtP ->
P_38 ; } if ( instantDelay < 0.0 ) { ( ( XDot_AbstractFuelControl_breach_T *
) ssGetdX ( S ) ) -> fuelsystemtransportdelay_CSTATE = 0 ; } else { ( (
XDot_AbstractFuelControl_breach_T * ) ssGetdX ( S ) ) ->
fuelsystemtransportdelay_CSTATE = 1.0 / instantDelay ; } } } static void
mdlInitializeSizes ( SimStruct * S ) { ssSetChecksumVal ( S , 0 , 3049481776U
) ; ssSetChecksumVal ( S , 1 , 2625962883U ) ; ssSetChecksumVal ( S , 2 ,
2113297773U ) ; ssSetChecksumVal ( S , 3 , 90981213U ) ; { mxArray *
slVerStructMat = ( NULL ) ; mxArray * slStrMat = mxCreateString ( "simulink"
) ; char slVerChar [ 10 ] ; int status = mexCallMATLAB ( 1 , & slVerStructMat
, 1 , & slStrMat , "ver" ) ; if ( status == 0 ) { mxArray * slVerMat =
mxGetField ( slVerStructMat , 0 , "Version" ) ; if ( slVerMat == ( NULL ) ) {
status = 1 ; } else { status = mxGetString ( slVerMat , slVerChar , 10 ) ; }
} mxDestroyArray ( slStrMat ) ; mxDestroyArray ( slVerStructMat ) ; if ( (
status == 1 ) || ( strcmp ( slVerChar , "24.1" ) != 0 ) ) { return ; } }
ssSetOptions ( S , SS_OPTION_EXCEPTION_FREE_CODE ) ; if ( ssGetSizeofDWork (
S ) != ( SLSize ) sizeof ( DW_AbstractFuelControl_breach_T ) ) { static char
msg [ 256 ] ; snprintf ( msg , 256 ,
"Unexpected error: Internal DWork sizes do "
"not match for accelerator mex file (%ld vs %lu)." , ( signed long )
ssGetSizeofDWork ( S ) , ( unsigned long ) sizeof (
DW_AbstractFuelControl_breach_T ) ) ; ssSetErrorStatus ( S , msg ) ; } if (
ssGetSizeofGlobalBlockIO ( S ) != ( SLSize ) sizeof (
B_AbstractFuelControl_breach_T ) ) { static char msg [ 256 ] ; snprintf ( msg
, 256 , "Unexpected error: Internal BlockIO sizes do "
"not match for accelerator mex file (%ld vs %lu)." , ( signed long )
ssGetSizeofGlobalBlockIO ( S ) , ( unsigned long ) sizeof (
B_AbstractFuelControl_breach_T ) ) ; ssSetErrorStatus ( S , msg ) ; } if (
ssGetSizeofU ( S ) != sizeof ( ExternalUPtrs_AbstractFuelControl_breach_T ) )
{ static char msg [ 256 ] ; snprintf ( msg , 256 ,
"Unexpected error: Internal ExternalInputs sizes do "
"not match for accelerator mex file (%ld vs %lu)." , ( signed long )
ssGetSizeofU ( S ) , ( unsigned long ) sizeof (
ExternalUPtrs_AbstractFuelControl_breach_T ) ) ; ssSetErrorStatus ( S , msg )
; } { int ssSizeofParams ; ssGetSizeofParams ( S , & ssSizeofParams ) ; if (
ssSizeofParams != sizeof ( P_AbstractFuelControl_breach_T ) ) { static char
msg [ 256 ] ; snprintf ( msg , 256 ,
"Unexpected error: Internal Parameters sizes do "
"not match for accelerator mex file (%d vs %lu)." , ssSizeofParams , (
unsigned long ) sizeof ( P_AbstractFuelControl_breach_T ) ) ;
ssSetErrorStatus ( S , msg ) ; } } _ssSetModelRtp ( S , ( real_T * ) &
AbstractFuelControl_breach_rtDefaultP ) ; } static void
mdlInitializeSampleTimes ( SimStruct * S ) { slAccRegPrmChangeFcn ( S ,
mdlOutputsTID4 ) ; } static void mdlTerminate ( SimStruct * S ) { }
#include "simulink.c"
