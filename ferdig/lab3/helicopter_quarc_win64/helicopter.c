/*
 * helicopter.c
 *
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * Code generation for model "helicopter".
 *
 * Model version              : 11.12
 * Simulink Coder version : 9.4 (R2020b) 29-Jul-2020
 * C source code generated on : Thu Apr 21 18:47:34 2022
 *
 * Target selection: quarc_win64.tlc
 * Note: GRT includes extra infrastructure and instrumentation for prototyping
 * Embedded hardware selection: 32-bit Generic
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#include "helicopter.h"
#include "helicopter_private.h"
#include "helicopter_dt.h"

/* Block signals (default storage) */
B_helicopter_T helicopter_B;

/* Continuous states */
X_helicopter_T helicopter_X;

/* Block states (default storage) */
DW_helicopter_T helicopter_DW;

/* Real-time model */
static RT_MODEL_helicopter_T helicopter_M_;
RT_MODEL_helicopter_T *const helicopter_M = &helicopter_M_;

/*
 * Writes out MAT-file header.  Returns success or failure.
 * Returns:
 *      0 - success
 *      1 - failure
 */
int_T rt_WriteMat4FileHeader(FILE *fp, int32_T m, int32_T n, const char *name)
{
  typedef enum { ELITTLE_ENDIAN, EBIG_ENDIAN } ByteOrder;

  int16_T one = 1;
  ByteOrder byteOrder = (*((int8_T *)&one)==1) ? ELITTLE_ENDIAN : EBIG_ENDIAN;
  int32_T type = (byteOrder == ELITTLE_ENDIAN) ? 0: 1000;
  int32_T imagf = 0;
  int32_T name_len = (int32_T)strlen(name) + 1;
  if ((fwrite(&type, sizeof(int32_T), 1, fp) == 0) ||
      (fwrite(&m, sizeof(int32_T), 1, fp) == 0) ||
      (fwrite(&n, sizeof(int32_T), 1, fp) == 0) ||
      (fwrite(&imagf, sizeof(int32_T), 1, fp) == 0) ||
      (fwrite(&name_len, sizeof(int32_T), 1, fp) == 0) ||
      (fwrite(name, sizeof(char), name_len, fp) == 0)) {
    return(1);
  } else {
    return(0);
  }
}                                      /* end rt_WriteMat4FileHeader */

/*
 * This function updates continuous states using the ODE1 fixed-step
 * solver algorithm
 */
static void rt_ertODEUpdateContinuousStates(RTWSolverInfo *si )
{
  time_T tnew = rtsiGetSolverStopTime(si);
  time_T h = rtsiGetStepSize(si);
  real_T *x = rtsiGetContStates(si);
  ODE1_IntgData *id = (ODE1_IntgData *)rtsiGetSolverData(si);
  real_T *f0 = id->f[0];
  int_T i;
  int_T nXc = 4;
  rtsiSetSimTimeStep(si,MINOR_TIME_STEP);
  rtsiSetdX(si, f0);
  helicopter_derivatives();
  rtsiSetT(si, tnew);
  for (i = 0; i < nXc; ++i) {
    x[i] += h * f0[i];
  }

  rtsiSetSimTimeStep(si,MAJOR_TIME_STEP);
}

/* Model output function */
void helicopter_output(void)
{
  /* local block i/o variables */
  real_T rtb_Derivative;
  real_T rtb_Frontgain;
  real_T rtb_Clock;
  real_T rtb_Backgain;
  real_T rtb_HILReadEncoderTimebase_o1;
  real_T rtb_HILReadEncoderTimebase_o2;
  real_T rtb_HILReadEncoderTimebase_o3;
  real_T rtb_Clock_tmp;
  real_T rtb_K_pp;
  real_T *lastU;
  int8_T rtAction;
  if (rtmIsMajorTimeStep(helicopter_M)) {
    /* set solver stop time */
    if (!(helicopter_M->Timing.clockTick0+1)) {
      rtsiSetSolverStopTime(&helicopter_M->solverInfo,
                            ((helicopter_M->Timing.clockTickH0 + 1) *
        helicopter_M->Timing.stepSize0 * 4294967296.0));
    } else {
      rtsiSetSolverStopTime(&helicopter_M->solverInfo,
                            ((helicopter_M->Timing.clockTick0 + 1) *
        helicopter_M->Timing.stepSize0 + helicopter_M->Timing.clockTickH0 *
        helicopter_M->Timing.stepSize0 * 4294967296.0));
    }
  }                                    /* end MajorTimeStep */

  /* Update absolute time of base rate at minor time step */
  if (rtmIsMinorTimeStep(helicopter_M)) {
    helicopter_M->Timing.t[0] = rtsiGetT(&helicopter_M->solverInfo);
  }

  /* Reset subsysRan breadcrumbs */
  srClearBC(helicopter_DW.IfActionSubsystem_SubsysRanBC);
  if (rtmIsMajorTimeStep(helicopter_M)) {
    /* S-Function (hil_read_encoder_timebase_block): '<S4>/HIL Read Encoder Timebase' */

    /* S-Function Block: helicopter/Helicopter_interface/HIL Read Encoder Timebase (hil_read_encoder_timebase_block) */
    {
      t_error result;
      result = hil_task_read_encoder(helicopter_DW.HILReadEncoderTimebase_Task,
        1, &helicopter_DW.HILReadEncoderTimebase_Buffer[0]);
      if (result < 0) {
        msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
          (_rt_error_message));
        rtmSetErrorStatus(helicopter_M, _rt_error_message);
      } else {
        rtb_HILReadEncoderTimebase_o1 =
          helicopter_DW.HILReadEncoderTimebase_Buffer[0];
        rtb_HILReadEncoderTimebase_o2 =
          helicopter_DW.HILReadEncoderTimebase_Buffer[1];
        rtb_HILReadEncoderTimebase_o3 =
          helicopter_DW.HILReadEncoderTimebase_Buffer[2];
      }
    }

    /* Gain: '<S4>/Travel: Count to rad' incorporates:
     *  Gain: '<S4>/Travel_gain'
     */
    helicopter_B.TravelCounttorad = helicopter_P.travel_gain *
      rtb_HILReadEncoderTimebase_o1 * helicopter_P.TravelCounttorad_Gain;

    /* Gain: '<S13>/Gain' */
    helicopter_B.Gain = helicopter_P.Gain_Gain * helicopter_B.TravelCounttorad;

    /* Sum: '<Root>/Sum3' incorporates:
     *  Constant: '<Root>/Constant'
     */
    helicopter_B.Sum3 = helicopter_B.Gain + helicopter_P.Constant_Value;

    /* Gain: '<S4>/Pitch: Count to rad' */
    helicopter_B.PitchCounttorad = helicopter_P.PitchCounttorad_Gain *
      rtb_HILReadEncoderTimebase_o2;

    /* Gain: '<S10>/Gain' */
    helicopter_B.Gain_i = helicopter_P.Gain_Gain_a *
      helicopter_B.PitchCounttorad;
  }

  /* Clock: '<S3>/Clock' incorporates:
   *  TransferFcn: '<S4>/Travel: Transfer Fcn'
   */
  rtb_Clock = 0.0;
  rtb_Clock += helicopter_P.TravelTransferFcn_C *
    helicopter_X.TravelTransferFcn_CSTATE;
  rtb_Clock += helicopter_P.TravelTransferFcn_D * helicopter_B.TravelCounttorad;

  /* Gain: '<S14>/Gain' */
  helicopter_B.Gain_d = helicopter_P.Gain_Gain_l * rtb_Clock;

  /* Clock: '<S3>/Clock' incorporates:
   *  TransferFcn: '<S4>/Pitch: Transfer Fcn'
   */
  rtb_Clock = 0.0;
  rtb_Clock += helicopter_P.PitchTransferFcn_C *
    helicopter_X.PitchTransferFcn_CSTATE;
  rtb_Clock += helicopter_P.PitchTransferFcn_D * helicopter_B.PitchCounttorad;

  /* Gain: '<S11>/Gain' */
  helicopter_B.Gain_b = helicopter_P.Gain_Gain_ae * rtb_Clock;
  if (rtmIsMajorTimeStep(helicopter_M)) {
    /* Gain: '<S4>/Elevation: Count to rad' incorporates:
     *  Gain: '<S4>/Elevation_gain'
     */
    helicopter_B.ElevationCounttorad = helicopter_P.elevation_gain *
      rtb_HILReadEncoderTimebase_o3 * helicopter_P.ElevationCounttorad_Gain;

    /* Gain: '<S8>/Gain' */
    helicopter_B.Gain_e = helicopter_P.Gain_Gain_lv *
      helicopter_B.ElevationCounttorad;

    /* Sum: '<Root>/Sum' incorporates:
     *  Constant: '<Root>/elavation_offset [deg]'
     */
    helicopter_B.Sum = helicopter_B.Gain_e +
      helicopter_P.elavation_offsetdeg_Value;
  }

  /* Clock: '<S3>/Clock' incorporates:
   *  TransferFcn: '<S4>/Elevation: Transfer Fcn'
   */
  rtb_Clock = 0.0;
  rtb_Clock += helicopter_P.ElevationTransferFcn_C *
    helicopter_X.ElevationTransferFcn_CSTATE;
  rtb_Clock += helicopter_P.ElevationTransferFcn_D *
    helicopter_B.ElevationCounttorad;

  /* Gain: '<S9>/Gain' */
  helicopter_B.Gain_dg = helicopter_P.Gain_Gain_n * rtb_Clock;

  /* Gain: '<S2>/Gain1' */
  helicopter_B.Gain1[0] = helicopter_P.Gain1_Gain * helicopter_B.Sum3;
  helicopter_B.Gain1[1] = helicopter_P.Gain1_Gain * helicopter_B.Gain_d;
  helicopter_B.Gain1[2] = helicopter_P.Gain1_Gain * helicopter_B.Gain_i;
  helicopter_B.Gain1[3] = helicopter_P.Gain1_Gain * helicopter_B.Gain_b;
  helicopter_B.Gain1[4] = helicopter_P.Gain1_Gain * helicopter_B.Sum;
  helicopter_B.Gain1[5] = helicopter_P.Gain1_Gain * helicopter_B.Gain_dg;

  /* Sum: '<Root>/Sum4' incorporates:
   *  Constant: '<Root>/Constant1'
   */
  helicopter_B.Sum4 = helicopter_B.Gain1[0] - helicopter_P.Constant1_Value;
  if (rtmIsMajorTimeStep(helicopter_M)) {
    /* SignalConversion generated from: '<Root>/To File1' */
    helicopter_B.TmpSignalConversionAtToFile1Inp[0] = helicopter_B.Sum4;
    helicopter_B.TmpSignalConversionAtToFile1Inp[1] = helicopter_B.Gain1[1];
    helicopter_B.TmpSignalConversionAtToFile1Inp[2] = helicopter_B.Gain1[2];
    helicopter_B.TmpSignalConversionAtToFile1Inp[3] = helicopter_B.Gain1[3];
  }

  /* FromWorkspace: '<S6>/From Workspace3' */
  {
    real_T *pDataValues = (real_T *) helicopter_DW.FromWorkspace3_PWORK.DataPtr;
    real_T *pTimeValues = (real_T *) helicopter_DW.FromWorkspace3_PWORK.TimePtr;
    int_T currTimeIndex = helicopter_DW.FromWorkspace3_IWORK.PrevIndex;
    real_T t = helicopter_M->Timing.t[0];

    /* Get index */
    if (t <= pTimeValues[0]) {
      currTimeIndex = 0;
    } else if (t >= pTimeValues[140]) {
      currTimeIndex = 139;
    } else {
      if (t < pTimeValues[currTimeIndex]) {
        while (t < pTimeValues[currTimeIndex]) {
          currTimeIndex--;
        }
      } else {
        while (t >= pTimeValues[currTimeIndex + 1]) {
          currTimeIndex++;
        }
      }
    }

    helicopter_DW.FromWorkspace3_IWORK.PrevIndex = currTimeIndex;

    /* Post output */
    {
      real_T t1 = pTimeValues[currTimeIndex];
      real_T t2 = pTimeValues[currTimeIndex + 1];
      if (t1 == t2) {
        if (t < t1) {
          rtb_Clock = pDataValues[currTimeIndex];
        } else {
          rtb_Clock = pDataValues[currTimeIndex + 1];
        }
      } else {
        real_T f1 = (t2 - t) / (t2 - t1);
        real_T f2 = 1.0 - f1;
        real_T d1;
        real_T d2;
        int_T TimeIndex= currTimeIndex;
        d1 = pDataValues[TimeIndex];
        d2 = pDataValues[TimeIndex + 1];
        rtb_Clock = (real_T) rtInterpolate(d1, d2, f1, f2);
        pDataValues += 141;
      }
    }
  }

  /* FromWorkspace: '<S6>/From Workspace2' */
  {
    real_T *pDataValues = (real_T *) helicopter_DW.FromWorkspace2_PWORK.DataPtr;
    real_T *pTimeValues = (real_T *) helicopter_DW.FromWorkspace2_PWORK.TimePtr;
    int_T currTimeIndex = helicopter_DW.FromWorkspace2_IWORK.PrevIndex;
    real_T t = helicopter_M->Timing.t[0];

    /* Get index */
    if (t <= pTimeValues[0]) {
      currTimeIndex = 0;
    } else if (t >= pTimeValues[140]) {
      currTimeIndex = 139;
    } else {
      if (t < pTimeValues[currTimeIndex]) {
        while (t < pTimeValues[currTimeIndex]) {
          currTimeIndex--;
        }
      } else {
        while (t >= pTimeValues[currTimeIndex + 1]) {
          currTimeIndex++;
        }
      }
    }

    helicopter_DW.FromWorkspace2_IWORK.PrevIndex = currTimeIndex;

    /* Post output */
    {
      real_T t1 = pTimeValues[currTimeIndex];
      real_T t2 = pTimeValues[currTimeIndex + 1];
      if (t1 == t2) {
        if (t < t1) {
          rtb_Derivative = pDataValues[currTimeIndex];
        } else {
          rtb_Derivative = pDataValues[currTimeIndex + 1];
        }
      } else {
        real_T f1 = (t2 - t) / (t2 - t1);
        real_T f2 = 1.0 - f1;
        real_T d1;
        real_T d2;
        int_T TimeIndex= currTimeIndex;
        d1 = pDataValues[TimeIndex];
        d2 = pDataValues[TimeIndex + 1];
        rtb_Derivative = (real_T) rtInterpolate(d1, d2, f1, f2);
        pDataValues += 141;
      }
    }
  }

  /* FromWorkspace: '<S6>/From Workspace1' */
  {
    real_T *pDataValues = (real_T *) helicopter_DW.FromWorkspace1_PWORK.DataPtr;
    real_T *pTimeValues = (real_T *) helicopter_DW.FromWorkspace1_PWORK.TimePtr;
    int_T currTimeIndex = helicopter_DW.FromWorkspace1_IWORK.PrevIndex;
    real_T t = helicopter_M->Timing.t[0];

    /* Get index */
    if (t <= pTimeValues[0]) {
      currTimeIndex = 0;
    } else if (t >= pTimeValues[140]) {
      currTimeIndex = 139;
    } else {
      if (t < pTimeValues[currTimeIndex]) {
        while (t < pTimeValues[currTimeIndex]) {
          currTimeIndex--;
        }
      } else {
        while (t >= pTimeValues[currTimeIndex + 1]) {
          currTimeIndex++;
        }
      }
    }

    helicopter_DW.FromWorkspace1_IWORK.PrevIndex = currTimeIndex;

    /* Post output */
    {
      real_T t1 = pTimeValues[currTimeIndex];
      real_T t2 = pTimeValues[currTimeIndex + 1];
      if (t1 == t2) {
        if (t < t1) {
          rtb_Frontgain = pDataValues[currTimeIndex];
        } else {
          rtb_Frontgain = pDataValues[currTimeIndex + 1];
        }
      } else {
        real_T f1 = (t2 - t) / (t2 - t1);
        real_T f2 = 1.0 - f1;
        real_T d1;
        real_T d2;
        int_T TimeIndex= currTimeIndex;
        d1 = pDataValues[TimeIndex];
        d2 = pDataValues[TimeIndex + 1];
        rtb_Frontgain = (real_T) rtInterpolate(d1, d2, f1, f2);
        pDataValues += 141;
      }
    }
  }

  /* FromWorkspace: '<S6>/From Workspace' */
  {
    real_T *pDataValues = (real_T *) helicopter_DW.FromWorkspace_PWORK.DataPtr;
    real_T *pTimeValues = (real_T *) helicopter_DW.FromWorkspace_PWORK.TimePtr;
    int_T currTimeIndex = helicopter_DW.FromWorkspace_IWORK.PrevIndex;
    real_T t = helicopter_M->Timing.t[0];

    /* Get index */
    if (t <= pTimeValues[0]) {
      currTimeIndex = 0;
    } else if (t >= pTimeValues[140]) {
      currTimeIndex = 139;
    } else {
      if (t < pTimeValues[currTimeIndex]) {
        while (t < pTimeValues[currTimeIndex]) {
          currTimeIndex--;
        }
      } else {
        while (t >= pTimeValues[currTimeIndex + 1]) {
          currTimeIndex++;
        }
      }
    }

    helicopter_DW.FromWorkspace_IWORK.PrevIndex = currTimeIndex;

    /* Post output */
    {
      real_T t1 = pTimeValues[currTimeIndex];
      real_T t2 = pTimeValues[currTimeIndex + 1];
      if (t1 == t2) {
        if (t < t1) {
          rtb_Backgain = pDataValues[currTimeIndex];
        } else {
          rtb_Backgain = pDataValues[currTimeIndex + 1];
        }
      } else {
        real_T f1 = (t2 - t) / (t2 - t1);
        real_T f2 = 1.0 - f1;
        real_T d1;
        real_T d2;
        int_T TimeIndex= currTimeIndex;
        d1 = pDataValues[TimeIndex];
        d2 = pDataValues[TimeIndex + 1];
        rtb_Backgain = (real_T) rtInterpolate(d1, d2, f1, f2);
        pDataValues += 141;
      }
    }
  }

  /* Gain: '<S1>/Back gain' incorporates:
   *  Gain: '<S6>/Gain'
   *  Sum: '<S6>/Sum'
   */
  rtb_Backgain = (((helicopter_B.Gain1[0] - rtb_Clock) * helicopter_P.K[0] +
                   (helicopter_B.Gain1[1] - rtb_Derivative) * helicopter_P.K[1])
                  + (helicopter_B.Gain1[2] - rtb_Frontgain) * helicopter_P.K[2])
    + (helicopter_B.Gain1[3] - rtb_Backgain) * helicopter_P.K[3];

  /* FromWorkspace: '<S6>/From Workspace4' */
  {
    real_T *pDataValues = (real_T *) helicopter_DW.FromWorkspace4_PWORK.DataPtr;
    real_T *pTimeValues = (real_T *) helicopter_DW.FromWorkspace4_PWORK.TimePtr;
    int_T currTimeIndex = helicopter_DW.FromWorkspace4_IWORK.PrevIndex;
    real_T t = helicopter_M->Timing.t[0];

    /* Get index */
    if (t <= pTimeValues[0]) {
      currTimeIndex = 0;
    } else if (t >= pTimeValues[140]) {
      currTimeIndex = 139;
    } else {
      if (t < pTimeValues[currTimeIndex]) {
        while (t < pTimeValues[currTimeIndex]) {
          currTimeIndex--;
        }
      } else {
        while (t >= pTimeValues[currTimeIndex + 1]) {
          currTimeIndex++;
        }
      }
    }

    helicopter_DW.FromWorkspace4_IWORK.PrevIndex = currTimeIndex;

    /* Post output */
    {
      real_T t1 = pTimeValues[currTimeIndex];
      real_T t2 = pTimeValues[currTimeIndex + 1];
      if (t1 == t2) {
        if (t < t1) {
          rtb_Frontgain = pDataValues[currTimeIndex];
        } else {
          rtb_Frontgain = pDataValues[currTimeIndex + 1];
        }
      } else {
        real_T f1 = (t2 - t) / (t2 - t1);
        real_T f2 = 1.0 - f1;
        real_T d1;
        real_T d2;
        int_T TimeIndex= currTimeIndex;
        d1 = pDataValues[TimeIndex];
        d2 = pDataValues[TimeIndex + 1];
        rtb_Frontgain = (real_T) rtInterpolate(d1, d2, f1, f2);
        pDataValues += 141;
      }
    }
  }

  /* Sum: '<S6>/Sum1' */
  helicopter_B.Sum1 = rtb_Frontgain - rtb_Backgain;
  if (rtmIsMajorTimeStep(helicopter_M)) {
    /* ToFile: '<Root>/To File' */
    {
      if (!(++helicopter_DW.ToFile_IWORK.Decimation % 1) &&
          (helicopter_DW.ToFile_IWORK.Count * (1 + 1)) + 1 < 100000000 ) {
        FILE *fp = (FILE *) helicopter_DW.ToFile_PWORK.FilePtr;
        if (fp != (NULL)) {
          real_T u[1 + 1];
          helicopter_DW.ToFile_IWORK.Decimation = 0;
          u[0] = helicopter_M->Timing.t[1];
          u[1] = helicopter_B.Sum3;
          if (fwrite(u, sizeof(real_T), 1 + 1, fp) != 1 + 1) {
            rtmSetErrorStatus(helicopter_M, "Error writing to MAT-file plot.mat");
            return;
          }

          if (((++helicopter_DW.ToFile_IWORK.Count) * (1 + 1))+1 >= 100000000) {
            (void)fprintf(stdout,
                          "*** The ToFile block will stop logging data before\n"
                          "    the simulation has ended, because it has reached\n"
                          "    the maximum number of elements (100000000)\n"
                          "    allowed in MAT-file plot.mat.\n");
          }
        }
      }
    }

    /* ToFile: '<Root>/To File1' */
    {
      if (!(++helicopter_DW.ToFile1_IWORK.Decimation % 1) &&
          (helicopter_DW.ToFile1_IWORK.Count * (4 + 1)) + 1 < 100000000 ) {
        FILE *fp = (FILE *) helicopter_DW.ToFile1_PWORK.FilePtr;
        if (fp != (NULL)) {
          real_T u[4 + 1];
          helicopter_DW.ToFile1_IWORK.Decimation = 0;
          u[0] = helicopter_M->Timing.t[1];
          u[1] = helicopter_B.TmpSignalConversionAtToFile1Inp[0];
          u[2] = helicopter_B.TmpSignalConversionAtToFile1Inp[1];
          u[3] = helicopter_B.TmpSignalConversionAtToFile1Inp[2];
          u[4] = helicopter_B.TmpSignalConversionAtToFile1Inp[3];
          if (fwrite(u, sizeof(real_T), 4 + 1, fp) != 4 + 1) {
            rtmSetErrorStatus(helicopter_M,
                              "Error writing to MAT-file allPlots.mat");
            return;
          }

          if (((++helicopter_DW.ToFile1_IWORK.Count) * (4 + 1))+1 >= 100000000)
          {
            (void)fprintf(stdout,
                          "*** The ToFile block will stop logging data before\n"
                          "    the simulation has ended, because it has reached\n"
                          "    the maximum number of elements (100000000)\n"
                          "    allowed in MAT-file allPlots.mat.\n");
          }
        }
      }
    }

    /* ToFile: '<Root>/To File2' */
    {
      if (!(++helicopter_DW.ToFile2_IWORK.Decimation % 1) &&
          (helicopter_DW.ToFile2_IWORK.Count * (1 + 1)) + 1 < 100000000 ) {
        FILE *fp = (FILE *) helicopter_DW.ToFile2_PWORK.FilePtr;
        if (fp != (NULL)) {
          real_T u[1 + 1];
          helicopter_DW.ToFile2_IWORK.Decimation = 0;
          u[0] = helicopter_M->Timing.t[1];
          u[1] = helicopter_B.Sum1;
          if (fwrite(u, sizeof(real_T), 1 + 1, fp) != 1 + 1) {
            rtmSetErrorStatus(helicopter_M, "Error writing to MAT-file pref.mat");
            return;
          }

          if (((++helicopter_DW.ToFile2_IWORK.Count) * (1 + 1))+1 >= 100000000)
          {
            (void)fprintf(stdout,
                          "*** The ToFile block will stop logging data before\n"
                          "    the simulation has ended, because it has reached\n"
                          "    the maximum number of elements (100000000)\n"
                          "    allowed in MAT-file pref.mat.\n");
          }
        }
      }
    }

    /* ToFile: '<Root>/To File3' */
    {
      if (!(++helicopter_DW.ToFile3_IWORK.Decimation % 1) &&
          (helicopter_DW.ToFile3_IWORK.Count * (1 + 1)) + 1 < 100000000 ) {
        FILE *fp = (FILE *) helicopter_DW.ToFile3_PWORK.FilePtr;
        if (fp != (NULL)) {
          real_T u[1 + 1];
          helicopter_DW.ToFile3_IWORK.Decimation = 0;
          u[0] = helicopter_M->Timing.t[1];
          u[1] = helicopter_B.Sum4;
          if (fwrite(u, sizeof(real_T), 1 + 1, fp) != 1 + 1) {
            rtmSetErrorStatus(helicopter_M,
                              "Error writing to MAT-file travel.mat");
            return;
          }

          if (((++helicopter_DW.ToFile3_IWORK.Count) * (1 + 1))+1 >= 100000000)
          {
            (void)fprintf(stdout,
                          "*** The ToFile block will stop logging data before\n"
                          "    the simulation has ended, because it has reached\n"
                          "    the maximum number of elements (100000000)\n"
                          "    allowed in MAT-file travel.mat.\n");
          }
        }
      }
    }

    /* ToFile: '<Root>/To File4' */
    {
      if (!(++helicopter_DW.ToFile4_IWORK.Decimation % 1) &&
          (helicopter_DW.ToFile4_IWORK.Count * (1 + 1)) + 1 < 100000000 ) {
        FILE *fp = (FILE *) helicopter_DW.ToFile4_PWORK.FilePtr;
        if (fp != (NULL)) {
          real_T u[1 + 1];
          helicopter_DW.ToFile4_IWORK.Decimation = 0;
          u[0] = helicopter_M->Timing.t[1];
          u[1] = helicopter_B.Gain1[1];
          if (fwrite(u, sizeof(real_T), 1 + 1, fp) != 1 + 1) {
            rtmSetErrorStatus(helicopter_M,
                              "Error writing to MAT-file travel_dot.mat");
            return;
          }

          if (((++helicopter_DW.ToFile4_IWORK.Count) * (1 + 1))+1 >= 100000000)
          {
            (void)fprintf(stdout,
                          "*** The ToFile block will stop logging data before\n"
                          "    the simulation has ended, because it has reached\n"
                          "    the maximum number of elements (100000000)\n"
                          "    allowed in MAT-file travel_dot.mat.\n");
          }
        }
      }
    }

    /* ToFile: '<Root>/To File5' */
    {
      if (!(++helicopter_DW.ToFile5_IWORK.Decimation % 1) &&
          (helicopter_DW.ToFile5_IWORK.Count * (1 + 1)) + 1 < 100000000 ) {
        FILE *fp = (FILE *) helicopter_DW.ToFile5_PWORK.FilePtr;
        if (fp != (NULL)) {
          real_T u[1 + 1];
          helicopter_DW.ToFile5_IWORK.Decimation = 0;
          u[0] = helicopter_M->Timing.t[1];
          u[1] = helicopter_B.Gain1[2];
          if (fwrite(u, sizeof(real_T), 1 + 1, fp) != 1 + 1) {
            rtmSetErrorStatus(helicopter_M,
                              "Error writing to MAT-file pitch.mat");
            return;
          }

          if (((++helicopter_DW.ToFile5_IWORK.Count) * (1 + 1))+1 >= 100000000)
          {
            (void)fprintf(stdout,
                          "*** The ToFile block will stop logging data before\n"
                          "    the simulation has ended, because it has reached\n"
                          "    the maximum number of elements (100000000)\n"
                          "    allowed in MAT-file pitch.mat.\n");
          }
        }
      }
    }

    /* ToFile: '<Root>/To File6' */
    {
      if (!(++helicopter_DW.ToFile6_IWORK.Decimation % 1) &&
          (helicopter_DW.ToFile6_IWORK.Count * (1 + 1)) + 1 < 100000000 ) {
        FILE *fp = (FILE *) helicopter_DW.ToFile6_PWORK.FilePtr;
        if (fp != (NULL)) {
          real_T u[1 + 1];
          helicopter_DW.ToFile6_IWORK.Decimation = 0;
          u[0] = helicopter_M->Timing.t[1];
          u[1] = helicopter_B.Gain1[3];
          if (fwrite(u, sizeof(real_T), 1 + 1, fp) != 1 + 1) {
            rtmSetErrorStatus(helicopter_M,
                              "Error writing to MAT-file pitch_dot.mat");
            return;
          }

          if (((++helicopter_DW.ToFile6_IWORK.Count) * (1 + 1))+1 >= 100000000)
          {
            (void)fprintf(stdout,
                          "*** The ToFile block will stop logging data before\n"
                          "    the simulation has ended, because it has reached\n"
                          "    the maximum number of elements (100000000)\n"
                          "    allowed in MAT-file pitch_dot.mat.\n");
          }
        }
      }
    }

    /* ToFile: '<Root>/To File7' */
    {
      if (!(++helicopter_DW.ToFile7_IWORK.Decimation % 1) &&
          (helicopter_DW.ToFile7_IWORK.Count * (1 + 1)) + 1 < 100000000 ) {
        FILE *fp = (FILE *) helicopter_DW.ToFile7_PWORK.FilePtr;
        if (fp != (NULL)) {
          real_T u[1 + 1];
          helicopter_DW.ToFile7_IWORK.Decimation = 0;
          u[0] = helicopter_M->Timing.t[1];
          u[1] = helicopter_B.Gain1[4];
          if (fwrite(u, sizeof(real_T), 1 + 1, fp) != 1 + 1) {
            rtmSetErrorStatus(helicopter_M,
                              "Error writing to MAT-file elevation.mat");
            return;
          }

          if (((++helicopter_DW.ToFile7_IWORK.Count) * (1 + 1))+1 >= 100000000)
          {
            (void)fprintf(stdout,
                          "*** The ToFile block will stop logging data before\n"
                          "    the simulation has ended, because it has reached\n"
                          "    the maximum number of elements (100000000)\n"
                          "    allowed in MAT-file elevation.mat.\n");
          }
        }
      }
    }

    /* ToFile: '<Root>/To File8' */
    {
      if (!(++helicopter_DW.ToFile8_IWORK.Decimation % 1) &&
          (helicopter_DW.ToFile8_IWORK.Count * (1 + 1)) + 1 < 100000000 ) {
        FILE *fp = (FILE *) helicopter_DW.ToFile8_PWORK.FilePtr;
        if (fp != (NULL)) {
          real_T u[1 + 1];
          helicopter_DW.ToFile8_IWORK.Decimation = 0;
          u[0] = helicopter_M->Timing.t[1];
          u[1] = helicopter_B.Gain1[5];
          if (fwrite(u, sizeof(real_T), 1 + 1, fp) != 1 + 1) {
            rtmSetErrorStatus(helicopter_M,
                              "Error writing to MAT-file elevation_dot.mat");
            return;
          }

          if (((++helicopter_DW.ToFile8_IWORK.Count) * (1 + 1))+1 >= 100000000)
          {
            (void)fprintf(stdout,
                          "*** The ToFile block will stop logging data before\n"
                          "    the simulation has ended, because it has reached\n"
                          "    the maximum number of elements (100000000)\n"
                          "    allowed in MAT-file elevation_dot.mat.\n");
          }
        }
      }
    }
  }

  /* Gain: '<S1>/Back gain' incorporates:
   *  Sum: '<S5>/Sum2'
   */
  rtb_Backgain = helicopter_B.Sum1 - helicopter_B.Gain1[2];

  /* Gain: '<S5>/K_pp' */
  rtb_K_pp = helicopter_P.K_pp * rtb_Backgain;

  /* Gain: '<S1>/Back gain' incorporates:
   *  Constant: '<Root>/Vd_bias'
   *  Gain: '<S5>/K_pd'
   *  Sum: '<Root>/Sum1'
   *  Sum: '<S5>/Sum3'
   */
  rtb_Backgain = helicopter_P.K_pd * helicopter_B.Gain1[3];
  rtb_Backgain = (rtb_K_pp - rtb_Backgain) + helicopter_P.Vd_ff;

  /* Integrator: '<S3>/Integrator' */
  /* Limited  Integrator  */
  if (helicopter_X.Integrator_CSTATE >= helicopter_P.Integrator_UpperSat) {
    helicopter_X.Integrator_CSTATE = helicopter_P.Integrator_UpperSat;
  } else {
    if (helicopter_X.Integrator_CSTATE <= helicopter_P.Integrator_LowerSat) {
      helicopter_X.Integrator_CSTATE = helicopter_P.Integrator_LowerSat;
    }
  }

  /* Gain: '<S1>/Front gain' incorporates:
   *  Integrator: '<S3>/Integrator'
   */
  rtb_Frontgain = helicopter_X.Integrator_CSTATE;

  /* Derivative: '<S4>/Derivative' incorporates:
   *  Constant: '<Root>/elevation_ref'
   *  Sum: '<S3>/Sum'
   */
  rtb_Derivative = helicopter_P.elevation_ref_Value - helicopter_B.Gain1[4];

  /* Clock: '<S3>/Clock' incorporates:
   *  Gain: '<S3>/K_ed'
   */
  rtb_Clock = helicopter_P.K_ed * helicopter_B.Gain1[5];

  /* Gain: '<S1>/Front gain' incorporates:
   *  Constant: '<Root>/Vs_bias'
   *  Gain: '<S3>/K_ep'
   *  Sum: '<Root>/Sum2'
   *  Sum: '<S3>/Sum1'
   */
  rtb_Frontgain = ((helicopter_P.K_ep * rtb_Derivative + rtb_Frontgain) -
                   rtb_Clock) + helicopter_P.Vs_ff;

  /* Sum: '<S1>/Add' */
  rtb_K_pp = rtb_Backgain + rtb_Frontgain;

  /* Gain: '<S1>/Back gain' incorporates:
   *  Sum: '<S1>/Subtract'
   */
  rtb_Backgain = (rtb_Frontgain - rtb_Backgain) * helicopter_P.Backgain_Gain;

  /* Gain: '<S1>/Front gain' */
  rtb_Frontgain = helicopter_P.Frontgain_Gain * rtb_K_pp;

  /* Clock: '<S3>/Clock' incorporates:
   *  Derivative: '<S4>/Derivative'
   */
  rtb_Clock_tmp = helicopter_M->Timing.t[0];

  /* Clock: '<S3>/Clock' */
  rtb_Clock = rtb_Clock_tmp;

  /* If: '<S3>/If' incorporates:
   *  Gain: '<S3>/K_ei'
   *  Inport: '<S7>/In1'
   */
  if (rtmIsMajorTimeStep(helicopter_M)) {
    rtAction = (int8_T)!(rtb_Clock >= 2.0);
    helicopter_DW.If_ActiveSubsystem = rtAction;
  } else {
    rtAction = helicopter_DW.If_ActiveSubsystem;
  }

  if (rtAction == 0) {
    /* Outputs for IfAction SubSystem: '<S3>/If Action Subsystem' incorporates:
     *  ActionPort: '<S7>/Action Port'
     */
    helicopter_B.In1 = helicopter_P.K_ei * rtb_Derivative;
    if (rtmIsMajorTimeStep(helicopter_M)) {
      srUpdateBC(helicopter_DW.IfActionSubsystem_SubsysRanBC);
    }

    /* End of Outputs for SubSystem: '<S3>/If Action Subsystem' */
  }

  /* End of If: '<S3>/If' */
  if (rtmIsMajorTimeStep(helicopter_M)) {
  }

  /* Derivative: '<S4>/Derivative' */
  if ((helicopter_DW.TimeStampA >= rtb_Clock_tmp) && (helicopter_DW.TimeStampB >=
       rtb_Clock_tmp)) {
    /* Derivative: '<S4>/Derivative' */
    rtb_Derivative = 0.0;
  } else {
    rtb_K_pp = helicopter_DW.TimeStampA;
    lastU = &helicopter_DW.LastUAtTimeA;
    if (helicopter_DW.TimeStampA < helicopter_DW.TimeStampB) {
      if (helicopter_DW.TimeStampB < rtb_Clock_tmp) {
        rtb_K_pp = helicopter_DW.TimeStampB;
        lastU = &helicopter_DW.LastUAtTimeB;
      }
    } else {
      if (helicopter_DW.TimeStampA >= rtb_Clock_tmp) {
        rtb_K_pp = helicopter_DW.TimeStampB;
        lastU = &helicopter_DW.LastUAtTimeB;
      }
    }

    /* Derivative: '<S4>/Derivative' */
    rtb_Derivative = (helicopter_B.PitchCounttorad - *lastU) / (rtb_Clock_tmp -
      rtb_K_pp);
  }

  /* Gain: '<S12>/Gain' */
  helicopter_B.Gain_l = helicopter_P.Gain_Gain_a1 * rtb_Derivative;
  if (rtmIsMajorTimeStep(helicopter_M)) {
  }

  /* Saturate: '<S4>/Back motor: Saturation' */
  if (rtb_Backgain > helicopter_P.BackmotorSaturation_UpperSat) {
    /* Saturate: '<S4>/Back motor: Saturation' */
    helicopter_B.BackmotorSaturation = helicopter_P.BackmotorSaturation_UpperSat;
  } else if (rtb_Backgain < helicopter_P.BackmotorSaturation_LowerSat) {
    /* Saturate: '<S4>/Back motor: Saturation' */
    helicopter_B.BackmotorSaturation = helicopter_P.BackmotorSaturation_LowerSat;
  } else {
    /* Saturate: '<S4>/Back motor: Saturation' */
    helicopter_B.BackmotorSaturation = rtb_Backgain;
  }

  /* End of Saturate: '<S4>/Back motor: Saturation' */
  if (rtmIsMajorTimeStep(helicopter_M)) {
  }

  /* Saturate: '<S4>/Front motor: Saturation' */
  if (rtb_Frontgain > helicopter_P.FrontmotorSaturation_UpperSat) {
    /* Saturate: '<S4>/Front motor: Saturation' */
    helicopter_B.FrontmotorSaturation =
      helicopter_P.FrontmotorSaturation_UpperSat;
  } else if (rtb_Frontgain < helicopter_P.FrontmotorSaturation_LowerSat) {
    /* Saturate: '<S4>/Front motor: Saturation' */
    helicopter_B.FrontmotorSaturation =
      helicopter_P.FrontmotorSaturation_LowerSat;
  } else {
    /* Saturate: '<S4>/Front motor: Saturation' */
    helicopter_B.FrontmotorSaturation = rtb_Frontgain;
  }

  /* End of Saturate: '<S4>/Front motor: Saturation' */
  if (rtmIsMajorTimeStep(helicopter_M)) {
    /* S-Function (hil_write_analog_block): '<S4>/HIL Write Analog' */

    /* S-Function Block: helicopter/Helicopter_interface/HIL Write Analog (hil_write_analog_block) */
    {
      t_error result;
      helicopter_DW.HILWriteAnalog_Buffer[0] = helicopter_B.FrontmotorSaturation;
      helicopter_DW.HILWriteAnalog_Buffer[1] = helicopter_B.BackmotorSaturation;
      result = hil_write_analog(helicopter_DW.HILInitialize_Card,
        helicopter_P.HILWriteAnalog_channels, 2,
        &helicopter_DW.HILWriteAnalog_Buffer[0]);
      if (result < 0) {
        msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
          (_rt_error_message));
        rtmSetErrorStatus(helicopter_M, _rt_error_message);
      }
    }
  }
}

/* Model update function */
void helicopter_update(void)
{
  real_T *lastU;

  /* Update for Derivative: '<S4>/Derivative' */
  if (helicopter_DW.TimeStampA == (rtInf)) {
    helicopter_DW.TimeStampA = helicopter_M->Timing.t[0];
    lastU = &helicopter_DW.LastUAtTimeA;
  } else if (helicopter_DW.TimeStampB == (rtInf)) {
    helicopter_DW.TimeStampB = helicopter_M->Timing.t[0];
    lastU = &helicopter_DW.LastUAtTimeB;
  } else if (helicopter_DW.TimeStampA < helicopter_DW.TimeStampB) {
    helicopter_DW.TimeStampA = helicopter_M->Timing.t[0];
    lastU = &helicopter_DW.LastUAtTimeA;
  } else {
    helicopter_DW.TimeStampB = helicopter_M->Timing.t[0];
    lastU = &helicopter_DW.LastUAtTimeB;
  }

  *lastU = helicopter_B.PitchCounttorad;

  /* End of Update for Derivative: '<S4>/Derivative' */
  if (rtmIsMajorTimeStep(helicopter_M)) {
    rt_ertODEUpdateContinuousStates(&helicopter_M->solverInfo);
  }

  /* Update absolute time for base rate */
  /* The "clockTick0" counts the number of times the code of this task has
   * been executed. The absolute time is the multiplication of "clockTick0"
   * and "Timing.stepSize0". Size of "clockTick0" ensures timer will not
   * overflow during the application lifespan selected.
   * Timer of this task consists of two 32 bit unsigned integers.
   * The two integers represent the low bits Timing.clockTick0 and the high bits
   * Timing.clockTickH0. When the low bit overflows to 0, the high bits increment.
   */
  if (!(++helicopter_M->Timing.clockTick0)) {
    ++helicopter_M->Timing.clockTickH0;
  }

  helicopter_M->Timing.t[0] = rtsiGetSolverStopTime(&helicopter_M->solverInfo);

  {
    /* Update absolute timer for sample time: [0.002s, 0.0s] */
    /* The "clockTick1" counts the number of times the code of this task has
     * been executed. The absolute time is the multiplication of "clockTick1"
     * and "Timing.stepSize1". Size of "clockTick1" ensures timer will not
     * overflow during the application lifespan selected.
     * Timer of this task consists of two 32 bit unsigned integers.
     * The two integers represent the low bits Timing.clockTick1 and the high bits
     * Timing.clockTickH1. When the low bit overflows to 0, the high bits increment.
     */
    if (!(++helicopter_M->Timing.clockTick1)) {
      ++helicopter_M->Timing.clockTickH1;
    }

    helicopter_M->Timing.t[1] = helicopter_M->Timing.clockTick1 *
      helicopter_M->Timing.stepSize1 + helicopter_M->Timing.clockTickH1 *
      helicopter_M->Timing.stepSize1 * 4294967296.0;
  }
}

/* Derivatives for root system: '<Root>' */
void helicopter_derivatives(void)
{
  XDot_helicopter_T *_rtXdot;
  boolean_T lsat;
  boolean_T usat;
  _rtXdot = ((XDot_helicopter_T *) helicopter_M->derivs);

  /* Derivatives for TransferFcn: '<S4>/Travel: Transfer Fcn' */
  _rtXdot->TravelTransferFcn_CSTATE = 0.0;
  _rtXdot->TravelTransferFcn_CSTATE += helicopter_P.TravelTransferFcn_A *
    helicopter_X.TravelTransferFcn_CSTATE;
  _rtXdot->TravelTransferFcn_CSTATE += helicopter_B.TravelCounttorad;

  /* Derivatives for TransferFcn: '<S4>/Pitch: Transfer Fcn' */
  _rtXdot->PitchTransferFcn_CSTATE = 0.0;
  _rtXdot->PitchTransferFcn_CSTATE += helicopter_P.PitchTransferFcn_A *
    helicopter_X.PitchTransferFcn_CSTATE;
  _rtXdot->PitchTransferFcn_CSTATE += helicopter_B.PitchCounttorad;

  /* Derivatives for TransferFcn: '<S4>/Elevation: Transfer Fcn' */
  _rtXdot->ElevationTransferFcn_CSTATE = 0.0;
  _rtXdot->ElevationTransferFcn_CSTATE += helicopter_P.ElevationTransferFcn_A *
    helicopter_X.ElevationTransferFcn_CSTATE;
  _rtXdot->ElevationTransferFcn_CSTATE += helicopter_B.ElevationCounttorad;

  /* Derivatives for Integrator: '<S3>/Integrator' */
  lsat = (helicopter_X.Integrator_CSTATE <= helicopter_P.Integrator_LowerSat);
  usat = (helicopter_X.Integrator_CSTATE >= helicopter_P.Integrator_UpperSat);
  if (((!lsat) && (!usat)) || (lsat && (helicopter_B.In1 > 0.0)) || (usat &&
       (helicopter_B.In1 < 0.0))) {
    _rtXdot->Integrator_CSTATE = helicopter_B.In1;
  } else {
    /* in saturation */
    _rtXdot->Integrator_CSTATE = 0.0;
  }

  /* End of Derivatives for Integrator: '<S3>/Integrator' */
}

/* Model initialize function */
void helicopter_initialize(void)
{
  /* Start for S-Function (hil_initialize_block): '<Root>/HIL Initialize' */

  /* S-Function Block: helicopter/HIL Initialize (hil_initialize_block) */
  {
    t_int result;
    t_boolean is_switching;
    result = hil_open("q8_usb", "0", &helicopter_DW.HILInitialize_Card);
    if (result < 0) {
      msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
        (_rt_error_message));
      rtmSetErrorStatus(helicopter_M, _rt_error_message);
      return;
    }

    is_switching = false;
    result = hil_set_card_specific_options(helicopter_DW.HILInitialize_Card,
      "update_rate=normal;decimation=1", 32);
    if (result < 0) {
      msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
        (_rt_error_message));
      rtmSetErrorStatus(helicopter_M, _rt_error_message);
      return;
    }

    result = hil_watchdog_clear(helicopter_DW.HILInitialize_Card);
    if (result < 0 && result != -QERR_HIL_WATCHDOG_CLEAR) {
      msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
        (_rt_error_message));
      rtmSetErrorStatus(helicopter_M, _rt_error_message);
      return;
    }

    if ((helicopter_P.HILInitialize_AIPStart && !is_switching) ||
        (helicopter_P.HILInitialize_AIPEnter && is_switching)) {
      {
        int_T i1;
        real_T *dw_AIMinimums = &helicopter_DW.HILInitialize_AIMinimums[0];
        for (i1=0; i1 < 8; i1++) {
          dw_AIMinimums[i1] = (helicopter_P.HILInitialize_AILow);
        }
      }

      {
        int_T i1;
        real_T *dw_AIMaximums = &helicopter_DW.HILInitialize_AIMaximums[0];
        for (i1=0; i1 < 8; i1++) {
          dw_AIMaximums[i1] = helicopter_P.HILInitialize_AIHigh;
        }
      }

      result = hil_set_analog_input_ranges(helicopter_DW.HILInitialize_Card,
        helicopter_P.HILInitialize_AIChannels, 8U,
        &helicopter_DW.HILInitialize_AIMinimums[0],
        &helicopter_DW.HILInitialize_AIMaximums[0]);
      if (result < 0) {
        msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
          (_rt_error_message));
        rtmSetErrorStatus(helicopter_M, _rt_error_message);
        return;
      }
    }

    if ((helicopter_P.HILInitialize_AOPStart && !is_switching) ||
        (helicopter_P.HILInitialize_AOPEnter && is_switching)) {
      {
        int_T i1;
        real_T *dw_AOMinimums = &helicopter_DW.HILInitialize_AOMinimums[0];
        for (i1=0; i1 < 8; i1++) {
          dw_AOMinimums[i1] = (helicopter_P.HILInitialize_AOLow);
        }
      }

      {
        int_T i1;
        real_T *dw_AOMaximums = &helicopter_DW.HILInitialize_AOMaximums[0];
        for (i1=0; i1 < 8; i1++) {
          dw_AOMaximums[i1] = helicopter_P.HILInitialize_AOHigh;
        }
      }

      result = hil_set_analog_output_ranges(helicopter_DW.HILInitialize_Card,
        helicopter_P.HILInitialize_AOChannels, 8U,
        &helicopter_DW.HILInitialize_AOMinimums[0],
        &helicopter_DW.HILInitialize_AOMaximums[0]);
      if (result < 0) {
        msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
          (_rt_error_message));
        rtmSetErrorStatus(helicopter_M, _rt_error_message);
        return;
      }
    }

    if ((helicopter_P.HILInitialize_AOStart && !is_switching) ||
        (helicopter_P.HILInitialize_AOEnter && is_switching)) {
      {
        int_T i1;
        real_T *dw_AOVoltages = &helicopter_DW.HILInitialize_AOVoltages[0];
        for (i1=0; i1 < 8; i1++) {
          dw_AOVoltages[i1] = helicopter_P.HILInitialize_AOInitial;
        }
      }

      result = hil_write_analog(helicopter_DW.HILInitialize_Card,
        helicopter_P.HILInitialize_AOChannels, 8U,
        &helicopter_DW.HILInitialize_AOVoltages[0]);
      if (result < 0) {
        msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
          (_rt_error_message));
        rtmSetErrorStatus(helicopter_M, _rt_error_message);
        return;
      }
    }

    if (helicopter_P.HILInitialize_AOReset) {
      {
        int_T i1;
        real_T *dw_AOVoltages = &helicopter_DW.HILInitialize_AOVoltages[0];
        for (i1=0; i1 < 8; i1++) {
          dw_AOVoltages[i1] = helicopter_P.HILInitialize_AOWatchdog;
        }
      }

      result = hil_watchdog_set_analog_expiration_state
        (helicopter_DW.HILInitialize_Card, helicopter_P.HILInitialize_AOChannels,
         8U, &helicopter_DW.HILInitialize_AOVoltages[0]);
      if (result < 0) {
        msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
          (_rt_error_message));
        rtmSetErrorStatus(helicopter_M, _rt_error_message);
        return;
      }
    }

    if ((helicopter_P.HILInitialize_EIPStart && !is_switching) ||
        (helicopter_P.HILInitialize_EIPEnter && is_switching)) {
      {
        int_T i1;
        int32_T *dw_QuadratureModes =
          &helicopter_DW.HILInitialize_QuadratureModes[0];
        for (i1=0; i1 < 8; i1++) {
          dw_QuadratureModes[i1] = helicopter_P.HILInitialize_EIQuadrature;
        }
      }

      result = hil_set_encoder_quadrature_mode(helicopter_DW.HILInitialize_Card,
        helicopter_P.HILInitialize_EIChannels, 8U, (t_encoder_quadrature_mode *)
        &helicopter_DW.HILInitialize_QuadratureModes[0]);
      if (result < 0) {
        msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
          (_rt_error_message));
        rtmSetErrorStatus(helicopter_M, _rt_error_message);
        return;
      }
    }

    if ((helicopter_P.HILInitialize_EIStart && !is_switching) ||
        (helicopter_P.HILInitialize_EIEnter && is_switching)) {
      {
        int_T i1;
        int32_T *dw_InitialEICounts =
          &helicopter_DW.HILInitialize_InitialEICounts[0];
        for (i1=0; i1 < 8; i1++) {
          dw_InitialEICounts[i1] = helicopter_P.HILInitialize_EIInitial;
        }
      }

      result = hil_set_encoder_counts(helicopter_DW.HILInitialize_Card,
        helicopter_P.HILInitialize_EIChannels, 8U,
        &helicopter_DW.HILInitialize_InitialEICounts[0]);
      if (result < 0) {
        msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
          (_rt_error_message));
        rtmSetErrorStatus(helicopter_M, _rt_error_message);
        return;
      }
    }

    if ((helicopter_P.HILInitialize_POPStart && !is_switching) ||
        (helicopter_P.HILInitialize_POPEnter && is_switching)) {
      uint32_T num_duty_cycle_modes = 0;
      uint32_T num_frequency_modes = 0;

      {
        int_T i1;
        int32_T *dw_POModeValues = &helicopter_DW.HILInitialize_POModeValues[0];
        for (i1=0; i1 < 8; i1++) {
          dw_POModeValues[i1] = helicopter_P.HILInitialize_POModes;
        }
      }

      result = hil_set_pwm_mode(helicopter_DW.HILInitialize_Card,
        helicopter_P.HILInitialize_POChannels, 8U, (t_pwm_mode *)
        &helicopter_DW.HILInitialize_POModeValues[0]);
      if (result < 0) {
        msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
          (_rt_error_message));
        rtmSetErrorStatus(helicopter_M, _rt_error_message);
        return;
      }

      {
        int_T i1;
        const uint32_T *p_HILInitialize_POChannels =
          helicopter_P.HILInitialize_POChannels;
        int32_T *dw_POModeValues = &helicopter_DW.HILInitialize_POModeValues[0];
        for (i1=0; i1 < 8; i1++) {
          if (dw_POModeValues[i1] == PWM_DUTY_CYCLE_MODE || dw_POModeValues[i1] ==
              PWM_ONE_SHOT_MODE || dw_POModeValues[i1] == PWM_TIME_MODE ||
              dw_POModeValues[i1] == PWM_RAW_MODE) {
            helicopter_DW.HILInitialize_POSortedChans[num_duty_cycle_modes] =
              (p_HILInitialize_POChannels[i1]);
            helicopter_DW.HILInitialize_POSortedFreqs[num_duty_cycle_modes] =
              helicopter_P.HILInitialize_POFrequency;
            num_duty_cycle_modes++;
          } else {
            helicopter_DW.HILInitialize_POSortedChans[7U - num_frequency_modes] =
              (p_HILInitialize_POChannels[i1]);
            helicopter_DW.HILInitialize_POSortedFreqs[7U - num_frequency_modes] =
              helicopter_P.HILInitialize_POFrequency;
            num_frequency_modes++;
          }
        }
      }

      if (num_duty_cycle_modes > 0) {
        result = hil_set_pwm_frequency(helicopter_DW.HILInitialize_Card,
          &helicopter_DW.HILInitialize_POSortedChans[0], num_duty_cycle_modes,
          &helicopter_DW.HILInitialize_POSortedFreqs[0]);
        if (result < 0) {
          msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
            (_rt_error_message));
          rtmSetErrorStatus(helicopter_M, _rt_error_message);
          return;
        }
      }

      if (num_frequency_modes > 0) {
        result = hil_set_pwm_duty_cycle(helicopter_DW.HILInitialize_Card,
          &helicopter_DW.HILInitialize_POSortedChans[num_duty_cycle_modes],
          num_frequency_modes,
          &helicopter_DW.HILInitialize_POSortedFreqs[num_duty_cycle_modes]);
        if (result < 0) {
          msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
            (_rt_error_message));
          rtmSetErrorStatus(helicopter_M, _rt_error_message);
          return;
        }
      }

      {
        int_T i1;
        int32_T *dw_POModeValues = &helicopter_DW.HILInitialize_POModeValues[0];
        for (i1=0; i1 < 8; i1++) {
          dw_POModeValues[i1] = helicopter_P.HILInitialize_POConfiguration;
        }
      }

      {
        int_T i1;
        int32_T *dw_POAlignValues = &helicopter_DW.HILInitialize_POAlignValues[0];
        for (i1=0; i1 < 8; i1++) {
          dw_POAlignValues[i1] = helicopter_P.HILInitialize_POAlignment;
        }
      }

      {
        int_T i1;
        int32_T *dw_POPolarityVals =
          &helicopter_DW.HILInitialize_POPolarityVals[0];
        for (i1=0; i1 < 8; i1++) {
          dw_POPolarityVals[i1] = helicopter_P.HILInitialize_POPolarity;
        }
      }

      result = hil_set_pwm_configuration(helicopter_DW.HILInitialize_Card,
        helicopter_P.HILInitialize_POChannels, 8U,
        (t_pwm_configuration *) &helicopter_DW.HILInitialize_POModeValues[0],
        (t_pwm_alignment *) &helicopter_DW.HILInitialize_POAlignValues[0],
        (t_pwm_polarity *) &helicopter_DW.HILInitialize_POPolarityVals[0]);
      if (result < 0) {
        msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
          (_rt_error_message));
        rtmSetErrorStatus(helicopter_M, _rt_error_message);
        return;
      }

      {
        int_T i1;
        real_T *dw_POSortedFreqs = &helicopter_DW.HILInitialize_POSortedFreqs[0];
        for (i1=0; i1 < 8; i1++) {
          dw_POSortedFreqs[i1] = helicopter_P.HILInitialize_POLeading;
        }
      }

      {
        int_T i1;
        real_T *dw_POValues = &helicopter_DW.HILInitialize_POValues[0];
        for (i1=0; i1 < 8; i1++) {
          dw_POValues[i1] = helicopter_P.HILInitialize_POTrailing;
        }
      }

      result = hil_set_pwm_deadband(helicopter_DW.HILInitialize_Card,
        helicopter_P.HILInitialize_POChannels, 8U,
        &helicopter_DW.HILInitialize_POSortedFreqs[0],
        &helicopter_DW.HILInitialize_POValues[0]);
      if (result < 0) {
        msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
          (_rt_error_message));
        rtmSetErrorStatus(helicopter_M, _rt_error_message);
        return;
      }
    }

    if ((helicopter_P.HILInitialize_POStart && !is_switching) ||
        (helicopter_P.HILInitialize_POEnter && is_switching)) {
      {
        int_T i1;
        real_T *dw_POValues = &helicopter_DW.HILInitialize_POValues[0];
        for (i1=0; i1 < 8; i1++) {
          dw_POValues[i1] = helicopter_P.HILInitialize_POInitial;
        }
      }

      result = hil_write_pwm(helicopter_DW.HILInitialize_Card,
        helicopter_P.HILInitialize_POChannels, 8U,
        &helicopter_DW.HILInitialize_POValues[0]);
      if (result < 0) {
        msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
          (_rt_error_message));
        rtmSetErrorStatus(helicopter_M, _rt_error_message);
        return;
      }
    }

    if (helicopter_P.HILInitialize_POReset) {
      {
        int_T i1;
        real_T *dw_POValues = &helicopter_DW.HILInitialize_POValues[0];
        for (i1=0; i1 < 8; i1++) {
          dw_POValues[i1] = helicopter_P.HILInitialize_POWatchdog;
        }
      }

      result = hil_watchdog_set_pwm_expiration_state
        (helicopter_DW.HILInitialize_Card, helicopter_P.HILInitialize_POChannels,
         8U, &helicopter_DW.HILInitialize_POValues[0]);
      if (result < 0) {
        msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
          (_rt_error_message));
        rtmSetErrorStatus(helicopter_M, _rt_error_message);
        return;
      }
    }
  }

  /* Start for S-Function (hil_read_encoder_timebase_block): '<S4>/HIL Read Encoder Timebase' */

  /* S-Function Block: helicopter/Helicopter_interface/HIL Read Encoder Timebase (hil_read_encoder_timebase_block) */
  {
    t_error result;
    result = hil_task_create_encoder_reader(helicopter_DW.HILInitialize_Card,
      helicopter_P.HILReadEncoderTimebase_SamplesI,
      helicopter_P.HILReadEncoderTimebase_Channels, 3,
      &helicopter_DW.HILReadEncoderTimebase_Task);
    if (result >= 0) {
      result = hil_task_set_buffer_overflow_mode
        (helicopter_DW.HILReadEncoderTimebase_Task, (t_buffer_overflow_mode)
         (helicopter_P.HILReadEncoderTimebase_Overflow - 1));
    }

    if (result < 0) {
      msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
        (_rt_error_message));
      rtmSetErrorStatus(helicopter_M, _rt_error_message);
    }
  }

  /* Start for FromWorkspace: '<S6>/From Workspace3' */
  {
    static real_T pTimeValues0[] = { 0.0, 0.25, 0.5, 0.75, 1.0, 1.25, 1.5, 1.75,
      2.0, 2.25, 2.5, 2.75, 3.0, 3.25, 3.5, 3.75, 4.0, 4.25, 4.5, 4.75, 5.0,
      5.25, 5.5, 5.75, 6.0, 6.25, 6.5, 6.75, 7.0, 7.25, 7.5, 7.75, 8.0, 8.25,
      8.5, 8.75, 9.0, 9.25, 9.5, 9.75, 10.0, 10.25, 10.5, 10.75, 11.0, 11.25,
      11.5, 11.75, 12.0, 12.25, 12.5, 12.75, 13.0, 13.25, 13.5, 13.75, 14.0,
      14.25, 14.5, 14.75, 15.0, 15.25, 15.5, 15.75, 16.0, 16.25, 16.5, 16.75,
      17.0, 17.25, 17.5, 17.75, 18.0, 18.25, 18.5, 18.75, 19.0, 19.25, 19.5,
      19.75, 20.0, 20.25, 20.5, 20.75, 21.0, 21.25, 21.5, 21.75, 22.0, 22.25,
      22.5, 22.75, 23.0, 23.25, 23.5, 23.75, 24.0, 24.25, 24.5, 24.75, 25.0,
      25.25, 25.5, 25.75, 26.0, 26.25, 26.5, 26.75, 27.0, 27.25, 27.5, 27.75,
      28.0, 28.25, 28.5, 28.75, 29.0, 29.25, 29.5, 29.75, 30.0, 30.25, 30.5,
      30.75, 31.0, 31.25, 31.5, 31.75, 32.0, 32.25, 32.5, 32.75, 33.0, 33.25,
      33.5, 33.75, 34.0, 34.25, 34.5, 34.75, 35.0 } ;

    static real_T pDataValues0[] = { 3.1415926535897931, 3.1415926535897931,
      3.1415926535897931, 3.1415926535897931, 3.1415926535897931,
      3.1415926535897931, 3.1415926535897931, 3.1415926535897931,
      3.1415926535897931, 3.1415926535897931, 3.1415926535897931,
      3.1415926535897931, 3.1415926535897931, 3.1415926535897931,
      3.1415926535897931, 3.1415926535897931, 3.1415926535897931,
      3.1415926535897931, 3.1415926535897931, 3.1415926535897931,
      3.1415926535897931, 3.1415926535897931, 3.1415926535897931,
      3.1415926535897931, 3.1406550255325376, 3.1377483785625975,
      3.1318794129648659, 3.1221108522541785, 3.1075613081355367,
      3.0874024307201458, 3.0608557942443757, 3.0271899005430343,
      2.9857173520687956, 2.9357921934224511, 2.8768074128727079,
      2.8081925945930215, 2.729411712295212, 2.6401361790341742,
      2.54056597887942, 2.4313763460011288, 2.3136165842036305,
      2.1886126060139737, 2.05786849348669, 1.9228390213920725,
      1.784897889513396, 1.6453370422305227, 1.5053707048406553,
      1.366139926015647, 1.2287169665900677, 1.0941094430678957,
      0.96326422427977987, 0.83707109374624533, 0.71636619120433043,
      0.60193524670293563, 0.49446698973385722, 0.39449892296034944,
      0.30241597707194912, 0.21845907875262788, 0.14273637815136755,
      0.075235643322439039, 0.015837288765321884, -0.035672306130977917,
      -0.079587468533259931, -0.1162701302683614, -0.14613715176456504,
      -0.16964823564832676, -0.1872945612627798, -0.19958824117896631,
      -0.20705267329170948, -0.21021383707980171, -0.20959256048343092,
      -0.20569776416489893, -0.19902067290604963, -0.19002996929233548,
      -0.1791678526329665, -0.16684695607992087, -0.15344806698402902,
      -0.13931858956758378, -0.12477168474942253, -0.1100860193380214,
      -0.095506055577285476, -0.081242812082383861, -0.067475028296617043,
      -0.05435066664287979, -0.041988689357818194, -0.030481050405605861,
      -0.01989484679951712, -0.010274577914193684, -0.0016444659181313259,
      0.0059892048803532692, 0.012635767261395859, 0.018317901677683213,
      0.023069446362884504, 0.026933310701979671, 0.029959511304737677,
      0.03220334625241511, 0.033723719191608437, 0.034581621508768368,
      0.034838777636658672, 0.03455645567588253, 0.03379444295082884,
      0.03261018385089802, 0.031058075320355871, 0.029188913646750313,
      0.027049484737778025, 0.024682288844229572, 0.02212538967156703,
      0.019412376987630285, 0.016572431168401028, 0.013630477593387751,
      0.010607418391938324, 0.0075204287170799822, 0.0043833044801589918,
      0.0012068482797124302, -0.0020007199077554556, -0.0052333408568250861,
      -0.0084866967741428385, -0.011757808424843063, -0.015044662785526583,
      -0.018345885514741835, -0.021660472744107376, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 } ;

    helicopter_DW.FromWorkspace3_PWORK.TimePtr = (void *) pTimeValues0;
    helicopter_DW.FromWorkspace3_PWORK.DataPtr = (void *) pDataValues0;
    helicopter_DW.FromWorkspace3_IWORK.PrevIndex = 0;
  }

  /* Start for FromWorkspace: '<S6>/From Workspace2' */
  {
    static real_T pTimeValues0[] = { 0.0, 0.25, 0.5, 0.75, 1.0, 1.25, 1.5, 1.75,
      2.0, 2.25, 2.5, 2.75, 3.0, 3.25, 3.5, 3.75, 4.0, 4.25, 4.5, 4.75, 5.0,
      5.25, 5.5, 5.75, 6.0, 6.25, 6.5, 6.75, 7.0, 7.25, 7.5, 7.75, 8.0, 8.25,
      8.5, 8.75, 9.0, 9.25, 9.5, 9.75, 10.0, 10.25, 10.5, 10.75, 11.0, 11.25,
      11.5, 11.75, 12.0, 12.25, 12.5, 12.75, 13.0, 13.25, 13.5, 13.75, 14.0,
      14.25, 14.5, 14.75, 15.0, 15.25, 15.5, 15.75, 16.0, 16.25, 16.5, 16.75,
      17.0, 17.25, 17.5, 17.75, 18.0, 18.25, 18.5, 18.75, 19.0, 19.25, 19.5,
      19.75, 20.0, 20.25, 20.5, 20.75, 21.0, 21.25, 21.5, 21.75, 22.0, 22.25,
      22.5, 22.75, 23.0, 23.25, 23.5, 23.75, 24.0, 24.25, 24.5, 24.75, 25.0,
      25.25, 25.5, 25.75, 26.0, 26.25, 26.5, 26.75, 27.0, 27.25, 27.5, 27.75,
      28.0, 28.25, 28.5, 28.75, 29.0, 29.25, 29.5, 29.75, 30.0, 30.25, 30.5,
      30.75, 31.0, 31.25, 31.5, 31.75, 32.0, 32.25, 32.5, 32.75, 33.0, 33.25,
      33.5, 33.75, 34.0, 34.25, 34.5, 34.75, 35.0 } ;

    static real_T pDataValues0[] = { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      -0.0037505123109028214, -0.011626587929681262, -0.02347586240257115,
      -0.039074242829848137, -0.058198176490635467, -0.080635509653682075,
      -0.10618654594028341, -0.13466357478323221, -0.16589019392691831,
      -0.19970063454189238, -0.23593912226552027, -0.27445927315627411,
      -0.31512352914530534, -0.35710213303399707, -0.3982808006526839,
      -0.43675853141882731, -0.47103904716894091, -0.50001591278724344,
      -0.522976450008906, -0.54011788840487729, -0.55176452754791838,
      -0.5582433891242975, -0.55986534954785772, -0.55692311533478145,
      -0.54969183764757745, -0.53843009414709286, -0.52338087520168219,
      -0.50477252214685142, -0.48281961018970077, -0.45772377803234016,
      -0.42987302786297765, -0.39987226701278655, -0.36833178356159418,
      -0.33582759329105377, -0.30289080237339633, -0.2700029392969987,
      -0.23759341820010652, -0.20603837961148186, -0.17566064960073871,
      -0.14673064692295454, -0.11946808597365488, -0.094044335550325764,
      -0.070585302461130831, -0.049174719667138531, -0.029857728456776235,
      -0.012644655154655319, 0.0024851063847649867, 0.015579185272262139,
      0.026708365036831071, 0.035962814455387994, 0.043448466637606528,
      0.049283586210933629, 0.053595556382629889, 0.056517909667555342,
      0.058187619272947681, 0.058742661642975071, 0.058319855044608779,
      0.0570529739810467, 0.055071135144086757, 0.052497446611790941,
      0.049447909140627744, 0.046030555810954714, 0.042344814426367554,
      0.038481075539715241, 0.034520447987419371, 0.0305346831930299,
      0.02658624952544705, 0.022728537663494263, 0.019006178739946016,
      0.015455457357903902, 0.012104802410745531, 0.00897533979091166,
      0.0060814917575822648, 0.0034316092681225142, 0.001028624511556531,
      -0.001129287843045587, -0.0030480509002727726, -0.0047370363995421763,
      -0.0062084341222413296, -0.007476646694631676, -0.008557715636193312,
      -0.0094687835740373642, -0.010227596690915219, -0.010852050735929123,
      -0.011359783277141539, -0.011767814300711839, -0.012092236805090643,
      -0.012347958699686188, -0.012548496947930123, -0.01270582480168396,
      -0.012830272750005858, -0.012930483796447029, -0.013013423669149588,
      -0.013084446602835151, -0.013147417442810885, -0.013204890916847798,
      -0.0132583489174635, -0.013308495776118704, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 } ;

    helicopter_DW.FromWorkspace2_PWORK.TimePtr = (void *) pTimeValues0;
    helicopter_DW.FromWorkspace2_PWORK.DataPtr = (void *) pDataValues0;
    helicopter_DW.FromWorkspace2_IWORK.PrevIndex = 0;
  }

  /* Start for FromWorkspace: '<S6>/From Workspace1' */
  {
    static real_T pTimeValues0[] = { 0.0, 0.25, 0.5, 0.75, 1.0, 1.25, 1.5, 1.75,
      2.0, 2.25, 2.5, 2.75, 3.0, 3.25, 3.5, 3.75, 4.0, 4.25, 4.5, 4.75, 5.0,
      5.25, 5.5, 5.75, 6.0, 6.25, 6.5, 6.75, 7.0, 7.25, 7.5, 7.75, 8.0, 8.25,
      8.5, 8.75, 9.0, 9.25, 9.5, 9.75, 10.0, 10.25, 10.5, 10.75, 11.0, 11.25,
      11.5, 11.75, 12.0, 12.25, 12.5, 12.75, 13.0, 13.25, 13.5, 13.75, 14.0,
      14.25, 14.5, 14.75, 15.0, 15.25, 15.5, 15.75, 16.0, 16.25, 16.5, 16.75,
      17.0, 17.25, 17.5, 17.75, 18.0, 18.25, 18.5, 18.75, 19.0, 19.25, 19.5,
      19.75, 20.0, 20.25, 20.5, 20.75, 21.0, 21.25, 21.5, 21.75, 22.0, 22.25,
      22.5, 22.75, 23.0, 23.25, 23.5, 23.75, 24.0, 24.25, 24.5, 24.75, 25.0,
      25.25, 25.5, 25.75, 26.0, 26.25, 26.5, 26.75, 27.0, 27.25, 27.5, 27.75,
      28.0, 28.25, 28.5, 28.75, 29.0, 29.25, 29.5, 29.75, 30.0, 30.25, 30.5,
      30.75, 31.0, 31.25, 31.5, 31.75, 32.0, 32.25, 32.5, 32.75, 33.0, 33.25,
      33.5, 33.75, 34.0, 34.25, 34.5, 34.75, 35.0 } ;

    static real_T pDataValues0[] = { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.026507188016066996, 0.055665094604699816, 0.0837461473401031,
      0.11024339520850734, 0.13516065901792951, 0.1585785015546618,
      0.18058496563590953, 0.201264763473322, 0.22069781749377038,
      0.23895928222999407, 0.25611978972947058, 0.27224571494958916,
      0.28739943100966447, 0.29668873984284605, 0.29103509542131989,
      0.271945906817736, 0.2422815924257381, 0.20479741910773719,
      0.16227630817427252, 0.12114913972386676, 0.082313996728273886,
      0.04579011918347442, 0.01146339728926038, -0.020794587637796402,
      -0.051107908688067361, -0.079593702689483847, -0.10636213222186691,
      -0.13151673201094644, -0.15515479703807283, -0.17736775699004209,
      -0.19683846575081443, -0.21203391888721995, -0.22291609152522629,
      -0.22972720314442374, -0.23278465846980312, -0.23243885569306982,
      -0.22905811719307168, -0.22301896151881578, -0.21469819388254963,
      -0.20446620990344128, -0.19268136855972695, -0.17968535818465803,
      -0.16579948632476832, -0.1513218219382732, -0.13652511634312392,
      -0.12165542803466484, -0.10693137616852721, -0.092543948627494155,
      -0.078656792043356349, -0.065406914022905938, -0.052905730679981389,
      -0.041240396610206953, -0.03047535833240167, -0.020654076880584848,
      -0.011800869772600975, -0.0039228274987891387, 0.0029882356200343629,
      0.0089538315303657745, 0.01400687997424821, 0.018189847554924032,
      0.021552966175109045, 0.024152548228253412, 0.026049412454375087,
      0.027307430769824337, 0.02799220292424176, 0.028169863292149766,
      0.027906021179804674, 0.027264834102128432, 0.026308211199491116,
      0.025095142664464554, 0.02368114944765376, 0.022117846592202062,
      0.020452612685554539, 0.018728357371433524, 0.016983378502163138,
      0.015251300368374965, 0.013561084468859175, 0.011937104447350411,
      0.010399277142745023, 0.0089632420980270133, 0.00764058239344001,
      0.0064390802273620595, 0.0053630013004193344, 0.0044134027006718091,
      0.0035884596913663414, 0.0028838074293332516, 0.0022928943643805155,
      0.0018073446790253245, 0.0014173277469976542, 0.0011119331834156254,
      0.000879550566623255, 0.000708253405963033, 0.00058618734575810461,
      0.00050196296957683018, 0.00044505384559589967, 0.0004062005645218969,
      0.00037782073159464691, 0.00035441884472586782, 0.00033295148150180864,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0 } ;

    helicopter_DW.FromWorkspace1_PWORK.TimePtr = (void *) pTimeValues0;
    helicopter_DW.FromWorkspace1_PWORK.DataPtr = (void *) pDataValues0;
    helicopter_DW.FromWorkspace1_IWORK.PrevIndex = 0;
  }

  /* Start for FromWorkspace: '<S6>/From Workspace' */
  {
    static real_T pTimeValues0[] = { 0.0, 0.25, 0.5, 0.75, 1.0, 1.25, 1.5, 1.75,
      2.0, 2.25, 2.5, 2.75, 3.0, 3.25, 3.5, 3.75, 4.0, 4.25, 4.5, 4.75, 5.0,
      5.25, 5.5, 5.75, 6.0, 6.25, 6.5, 6.75, 7.0, 7.25, 7.5, 7.75, 8.0, 8.25,
      8.5, 8.75, 9.0, 9.25, 9.5, 9.75, 10.0, 10.25, 10.5, 10.75, 11.0, 11.25,
      11.5, 11.75, 12.0, 12.25, 12.5, 12.75, 13.0, 13.25, 13.5, 13.75, 14.0,
      14.25, 14.5, 14.75, 15.0, 15.25, 15.5, 15.75, 16.0, 16.25, 16.5, 16.75,
      17.0, 17.25, 17.5, 17.75, 18.0, 18.25, 18.5, 18.75, 19.0, 19.25, 19.5,
      19.75, 20.0, 20.25, 20.5, 20.75, 21.0, 21.25, 21.5, 21.75, 22.0, 22.25,
      22.5, 22.75, 23.0, 23.25, 23.5, 23.75, 24.0, 24.25, 24.5, 24.75, 25.0,
      25.25, 25.5, 25.75, 26.0, 26.25, 26.5, 26.75, 27.0, 27.25, 27.5, 27.75,
      28.0, 28.25, 28.5, 28.75, 29.0, 29.25, 29.5, 29.75, 30.0, 30.25, 30.5,
      30.75, 31.0, 31.25, 31.5, 31.75, 32.0, 32.25, 32.5, 32.75, 33.0, 33.25,
      33.5, 33.75, 34.0, 34.25, 34.5, 34.75, 35.0 } ;

    static real_T pDataValues0[] = { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.42411500823778464, 0.46652650905563747, 0.44929683689722272,
      0.42395596522029855, 0.39867622544526654, 0.37468548075696939,
      0.35210342245091336, 0.33087676415793249, 0.31092886251479085,
      0.29218343609214215, 0.27456811972051576, 0.25801480159034396,
      0.24245945869970775, 0.14862894009833005, -0.090458311568454991,
      -0.30542701691640661, -0.47462902976811028, -0.59974677501537765,
      -0.680337775502126, -0.65803469525190406, -0.621362287359886,
      -0.58438204011229367, -0.549227549576404, -0.51612775974070035,
      -0.48501313595686574, -0.45577270577365253, -0.42829487281657252,
      -0.40247359636332708, -0.378209040826091, -0.35540735941703472,
      -0.31153133959580132, -0.2431272496709059, -0.17411476254815236,
      -0.10897778610323472, -0.048919284685329863, 0.00553284452060138,
      0.054091815927982484, 0.096626490554632308, 0.13313228213178016,
      0.16371174364469476, 0.18855746161995368, 0.20793616601352666,
      0.2221739497289891, 0.23164263021425704, 0.23674728948090726,
      0.2379150129778195, 0.23558482984033055, 0.23019884078078412,
      0.2221945054099336, 0.21199804842430992, 0.20001893345005037,
      0.18664534505521746, 0.17224061240749164, 0.15714050331218074,
      0.14165131377727141, 0.12604867636475317, 0.11057701003949544,
      0.095449534655645249, 0.080848774959014907, 0.066927481348917517,
      0.05380989800866099, 0.041593312818698054, 0.030349827704070187,
      0.020128293060341241, 0.01095635453859406, 0.0028425658485385229,
      -0.00422147373561924, -0.010258993269061182, -0.015305966444969715,
      -0.019409096552420218, -0.022623891455713068, -0.025012845684971564,
      -0.026643742505797929, -0.02758808502724381, -0.0279196619160994,
      -0.027713250143491217, -0.027043454394437394, -0.025983680339191809,
      -0.02460523687730656, -0.022976560724593705, -0.021162555284588963,
      -0.019224034661641305, -0.017217262846544828, -0.015193577585238137,
      -0.013199088150442561, -0.011274436197083217, -0.0094546090332519976,
      -0.0077687949683492357, -0.00624027091305598, -0.0048863130180961814,
      -0.0037181218685990026, -0.00274075457469644, -0.001953056962261382,
      -0.0013475900180719847, -0.0009105459840971123, -0.00062165249749899739,
      -0.00045407732671631594, -0.00037443018992735892, -0.00034347781158546481,
      -0.00032142704538649946, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 } ;

    helicopter_DW.FromWorkspace_PWORK.TimePtr = (void *) pTimeValues0;
    helicopter_DW.FromWorkspace_PWORK.DataPtr = (void *) pDataValues0;
    helicopter_DW.FromWorkspace_IWORK.PrevIndex = 0;
  }

  /* Start for FromWorkspace: '<S6>/From Workspace4' */
  {
    static real_T pTimeValues0[] = { 0.0, 0.25, 0.5, 0.75, 1.0, 1.25, 1.5, 1.75,
      2.0, 2.25, 2.5, 2.75, 3.0, 3.25, 3.5, 3.75, 4.0, 4.25, 4.5, 4.75, 5.0,
      5.25, 5.5, 5.75, 6.0, 6.25, 6.5, 6.75, 7.0, 7.25, 7.5, 7.75, 8.0, 8.25,
      8.5, 8.75, 9.0, 9.25, 9.5, 9.75, 10.0, 10.25, 10.5, 10.75, 11.0, 11.25,
      11.5, 11.75, 12.0, 12.25, 12.5, 12.75, 13.0, 13.25, 13.5, 13.75, 14.0,
      14.25, 14.5, 14.75, 15.0, 15.25, 15.5, 15.75, 16.0, 16.25, 16.5, 16.75,
      17.0, 17.25, 17.5, 17.75, 18.0, 18.25, 18.5, 18.75, 19.0, 19.25, 19.5,
      19.75, 20.0, 20.25, 20.5, 20.75, 21.0, 21.25, 21.5, 21.75, 22.0, 22.25,
      22.5, 22.75, 23.0, 23.25, 23.5, 23.75, 24.0, 24.25, 24.5, 24.75, 25.0,
      25.25, 25.5, 25.75, 26.0, 26.25, 26.5, 26.75, 27.0, 27.25, 27.5, 27.75,
      28.0, 28.25, 28.5, 28.75, 29.0, 29.25, 29.5, 29.75, 30.0, 30.25, 30.5,
      30.75, 31.0, 31.25, 31.5, 31.75, 32.0, 32.25, 32.5, 32.75, 33.0, 33.25,
      33.5, 33.75, 34.0, 34.25, 34.5, 34.75, 35.0 } ;

    static real_T pDataValues0[] = { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.52359877559829882,
      0.52359877559829882, 0.52359877559829882, 0.52359877559829882,
      0.52359877559829882, 0.52359877559829882, 0.52359877559829882,
      0.52359877559829882, 0.52359877559829882, 0.52359877559829882,
      0.52359877559829882, 0.52359877559829859, 0.52359877559829837,
      0.42580496703940829, 0.15737325132668367, -0.069213958626687722,
      -0.25721963061282166, -0.40988603394171735, -0.52359877537218358,
      -0.52359877559829748, -0.52359877559829815, -0.52359877559829826,
      -0.52359877559829837, -0.52359877559829837, -0.52359877559829837,
      -0.52359877559829826, -0.52359877559829815, -0.52359877559829793,
      -0.52359877559829715, -0.5235987755982543, -0.49588393734637265,
      -0.43906419616131903, -0.38177925288156628, -0.32507874584487995,
      -0.26985622248882712, -0.21685711303784727, -0.16668770602459604,
      -0.11982489401552832, -0.076626471425013609, -0.037341781978575628,
      -0.0021225306080601136, 0.028966407187887011, 0.055936326660951052,
      0.07886431495415569, 0.097883262390149661, 0.11317235587731644,
      0.12494812876919326, 0.13345612479790936, 0.13896321806775591,
      0.14175061666160171, 0.14210756426592419, 0.14032574241287732,
      0.13669436549452219, 0.13149595162436167, 0.12500274468210981,
      0.11747375643802238, 0.10915239245655139, 0.10026462145639292,
      0.091017644876242088, 0.081599021476467248, 0.072176200805244672,
      0.062896419179219065, 0.053886912378190654, 0.045255400435876392,
      0.037090801631548653, 0.029464134960499377, 0.0224295728989925,
      0.016025609100709937, 0.010276308691113067, 0.0051926119939416093,
      0.00077366576672344145, -0.0029918407175490991, -0.0061243541784030686,
      -0.00865216671216229, -0.010610169306256578, -0.012038659853197964,
      -0.012982213325424263, -0.013488619260953238, -0.01360788941224178,
      -0.013391336338680393, -0.012890721884399436, -0.012157472881241582,
      -0.011241960052035815, -0.010192834958666075, -0.0090564189369330972,
      -0.00787613727732639, -0.00669199143664756, -0.005540061786899475,
      -0.0044520333099214993, -0.0034547367120452144, -0.0025696976440391373,
      -0.0018126870485961621, -0.0011932661059507188, -0.00071431983102365759,
      -0.00037157436087276174, -0.0001530961650882956, -3.8788224715702313E-5,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 } ;

    helicopter_DW.FromWorkspace4_PWORK.TimePtr = (void *) pTimeValues0;
    helicopter_DW.FromWorkspace4_PWORK.DataPtr = (void *) pDataValues0;
    helicopter_DW.FromWorkspace4_IWORK.PrevIndex = 0;
  }

  /* Start for ToFile: '<Root>/To File' */
  {
    FILE *fp = (NULL);
    char fileName[509] = "plot.mat";
    if ((fp = fopen(fileName, "wb")) == (NULL)) {
      rtmSetErrorStatus(helicopter_M, "Error creating .mat file plot.mat");
      return;
    }

    if (rt_WriteMat4FileHeader(fp, 1 + 1, 0, "ans")) {
      rtmSetErrorStatus(helicopter_M,
                        "Error writing mat file header to file plot.mat");
      return;
    }

    helicopter_DW.ToFile_IWORK.Count = 0;
    helicopter_DW.ToFile_IWORK.Decimation = -1;
    helicopter_DW.ToFile_PWORK.FilePtr = fp;
  }

  /* Start for ToFile: '<Root>/To File1' */
  {
    FILE *fp = (NULL);
    char fileName[509] = "allPlots.mat";
    if ((fp = fopen(fileName, "wb")) == (NULL)) {
      rtmSetErrorStatus(helicopter_M, "Error creating .mat file allPlots.mat");
      return;
    }

    if (rt_WriteMat4FileHeader(fp, 4 + 1, 0, "ans")) {
      rtmSetErrorStatus(helicopter_M,
                        "Error writing mat file header to file allPlots.mat");
      return;
    }

    helicopter_DW.ToFile1_IWORK.Count = 0;
    helicopter_DW.ToFile1_IWORK.Decimation = -1;
    helicopter_DW.ToFile1_PWORK.FilePtr = fp;
  }

  /* Start for ToFile: '<Root>/To File2' */
  {
    FILE *fp = (NULL);
    char fileName[509] = "pref.mat";
    if ((fp = fopen(fileName, "wb")) == (NULL)) {
      rtmSetErrorStatus(helicopter_M, "Error creating .mat file pref.mat");
      return;
    }

    if (rt_WriteMat4FileHeader(fp, 1 + 1, 0, "ans")) {
      rtmSetErrorStatus(helicopter_M,
                        "Error writing mat file header to file pref.mat");
      return;
    }

    helicopter_DW.ToFile2_IWORK.Count = 0;
    helicopter_DW.ToFile2_IWORK.Decimation = -1;
    helicopter_DW.ToFile2_PWORK.FilePtr = fp;
  }

  /* Start for ToFile: '<Root>/To File3' */
  {
    FILE *fp = (NULL);
    char fileName[509] = "travel.mat";
    if ((fp = fopen(fileName, "wb")) == (NULL)) {
      rtmSetErrorStatus(helicopter_M, "Error creating .mat file travel.mat");
      return;
    }

    if (rt_WriteMat4FileHeader(fp, 1 + 1, 0, "ans")) {
      rtmSetErrorStatus(helicopter_M,
                        "Error writing mat file header to file travel.mat");
      return;
    }

    helicopter_DW.ToFile3_IWORK.Count = 0;
    helicopter_DW.ToFile3_IWORK.Decimation = -1;
    helicopter_DW.ToFile3_PWORK.FilePtr = fp;
  }

  /* Start for ToFile: '<Root>/To File4' */
  {
    FILE *fp = (NULL);
    char fileName[509] = "travel_dot.mat";
    if ((fp = fopen(fileName, "wb")) == (NULL)) {
      rtmSetErrorStatus(helicopter_M, "Error creating .mat file travel_dot.mat");
      return;
    }

    if (rt_WriteMat4FileHeader(fp, 1 + 1, 0, "ans")) {
      rtmSetErrorStatus(helicopter_M,
                        "Error writing mat file header to file travel_dot.mat");
      return;
    }

    helicopter_DW.ToFile4_IWORK.Count = 0;
    helicopter_DW.ToFile4_IWORK.Decimation = -1;
    helicopter_DW.ToFile4_PWORK.FilePtr = fp;
  }

  /* Start for ToFile: '<Root>/To File5' */
  {
    FILE *fp = (NULL);
    char fileName[509] = "pitch.mat";
    if ((fp = fopen(fileName, "wb")) == (NULL)) {
      rtmSetErrorStatus(helicopter_M, "Error creating .mat file pitch.mat");
      return;
    }

    if (rt_WriteMat4FileHeader(fp, 1 + 1, 0, "ans")) {
      rtmSetErrorStatus(helicopter_M,
                        "Error writing mat file header to file pitch.mat");
      return;
    }

    helicopter_DW.ToFile5_IWORK.Count = 0;
    helicopter_DW.ToFile5_IWORK.Decimation = -1;
    helicopter_DW.ToFile5_PWORK.FilePtr = fp;
  }

  /* Start for ToFile: '<Root>/To File6' */
  {
    FILE *fp = (NULL);
    char fileName[509] = "pitch_dot.mat";
    if ((fp = fopen(fileName, "wb")) == (NULL)) {
      rtmSetErrorStatus(helicopter_M, "Error creating .mat file pitch_dot.mat");
      return;
    }

    if (rt_WriteMat4FileHeader(fp, 1 + 1, 0, "ans")) {
      rtmSetErrorStatus(helicopter_M,
                        "Error writing mat file header to file pitch_dot.mat");
      return;
    }

    helicopter_DW.ToFile6_IWORK.Count = 0;
    helicopter_DW.ToFile6_IWORK.Decimation = -1;
    helicopter_DW.ToFile6_PWORK.FilePtr = fp;
  }

  /* Start for ToFile: '<Root>/To File7' */
  {
    FILE *fp = (NULL);
    char fileName[509] = "elevation.mat";
    if ((fp = fopen(fileName, "wb")) == (NULL)) {
      rtmSetErrorStatus(helicopter_M, "Error creating .mat file elevation.mat");
      return;
    }

    if (rt_WriteMat4FileHeader(fp, 1 + 1, 0, "ans")) {
      rtmSetErrorStatus(helicopter_M,
                        "Error writing mat file header to file elevation.mat");
      return;
    }

    helicopter_DW.ToFile7_IWORK.Count = 0;
    helicopter_DW.ToFile7_IWORK.Decimation = -1;
    helicopter_DW.ToFile7_PWORK.FilePtr = fp;
  }

  /* Start for ToFile: '<Root>/To File8' */
  {
    FILE *fp = (NULL);
    char fileName[509] = "elevation_dot.mat";
    if ((fp = fopen(fileName, "wb")) == (NULL)) {
      rtmSetErrorStatus(helicopter_M,
                        "Error creating .mat file elevation_dot.mat");
      return;
    }

    if (rt_WriteMat4FileHeader(fp, 1 + 1, 0, "ans")) {
      rtmSetErrorStatus(helicopter_M,
                        "Error writing mat file header to file elevation_dot.mat");
      return;
    }

    helicopter_DW.ToFile8_IWORK.Count = 0;
    helicopter_DW.ToFile8_IWORK.Decimation = -1;
    helicopter_DW.ToFile8_PWORK.FilePtr = fp;
  }

  /* Start for If: '<S3>/If' */
  helicopter_DW.If_ActiveSubsystem = -1;

  /* InitializeConditions for TransferFcn: '<S4>/Travel: Transfer Fcn' */
  helicopter_X.TravelTransferFcn_CSTATE = 0.0;

  /* InitializeConditions for TransferFcn: '<S4>/Pitch: Transfer Fcn' */
  helicopter_X.PitchTransferFcn_CSTATE = 0.0;

  /* InitializeConditions for TransferFcn: '<S4>/Elevation: Transfer Fcn' */
  helicopter_X.ElevationTransferFcn_CSTATE = 0.0;

  /* InitializeConditions for Integrator: '<S3>/Integrator' */
  helicopter_X.Integrator_CSTATE = helicopter_P.Integrator_IC;

  /* InitializeConditions for Derivative: '<S4>/Derivative' */
  helicopter_DW.TimeStampA = (rtInf);
  helicopter_DW.TimeStampB = (rtInf);
}

/* Model terminate function */
void helicopter_terminate(void)
{
  /* Terminate for S-Function (hil_initialize_block): '<Root>/HIL Initialize' */

  /* S-Function Block: helicopter/HIL Initialize (hil_initialize_block) */
  {
    t_boolean is_switching;
    t_int result;
    t_uint32 num_final_analog_outputs = 0;
    t_uint32 num_final_pwm_outputs = 0;
    hil_task_stop_all(helicopter_DW.HILInitialize_Card);
    hil_monitor_stop_all(helicopter_DW.HILInitialize_Card);
    is_switching = false;
    if ((helicopter_P.HILInitialize_AOTerminate && !is_switching) ||
        (helicopter_P.HILInitialize_AOExit && is_switching)) {
      {
        int_T i1;
        real_T *dw_AOVoltages = &helicopter_DW.HILInitialize_AOVoltages[0];
        for (i1=0; i1 < 8; i1++) {
          dw_AOVoltages[i1] = helicopter_P.HILInitialize_AOFinal;
        }
      }

      num_final_analog_outputs = 8U;
    } else {
      num_final_analog_outputs = 0;
    }

    if ((helicopter_P.HILInitialize_POTerminate && !is_switching) ||
        (helicopter_P.HILInitialize_POExit && is_switching)) {
      {
        int_T i1;
        real_T *dw_POValues = &helicopter_DW.HILInitialize_POValues[0];
        for (i1=0; i1 < 8; i1++) {
          dw_POValues[i1] = helicopter_P.HILInitialize_POFinal;
        }
      }

      num_final_pwm_outputs = 8U;
    } else {
      num_final_pwm_outputs = 0;
    }

    if (0
        || num_final_analog_outputs > 0
        || num_final_pwm_outputs > 0
        ) {
      /* Attempt to write the final outputs atomically (due to firmware issue in old Q2-USB). Otherwise write channels individually */
      result = hil_write(helicopter_DW.HILInitialize_Card
                         , helicopter_P.HILInitialize_AOChannels,
                         num_final_analog_outputs
                         , helicopter_P.HILInitialize_POChannels,
                         num_final_pwm_outputs
                         , NULL, 0
                         , NULL, 0
                         , &helicopter_DW.HILInitialize_AOVoltages[0]
                         , &helicopter_DW.HILInitialize_POValues[0]
                         , (t_boolean *) NULL
                         , NULL
                         );
      if (result == -QERR_HIL_WRITE_NOT_SUPPORTED) {
        t_error local_result;
        result = 0;

        /* The hil_write operation is not supported by this card. Write final outputs for each channel type */
        if (num_final_analog_outputs > 0) {
          local_result = hil_write_analog(helicopter_DW.HILInitialize_Card,
            helicopter_P.HILInitialize_AOChannels, num_final_analog_outputs,
            &helicopter_DW.HILInitialize_AOVoltages[0]);
          if (local_result < 0) {
            result = local_result;
          }
        }

        if (num_final_pwm_outputs > 0) {
          local_result = hil_write_pwm(helicopter_DW.HILInitialize_Card,
            helicopter_P.HILInitialize_POChannels, num_final_pwm_outputs,
            &helicopter_DW.HILInitialize_POValues[0]);
          if (local_result < 0) {
            result = local_result;
          }
        }

        if (result < 0) {
          msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
            (_rt_error_message));
          rtmSetErrorStatus(helicopter_M, _rt_error_message);
        }
      }
    }

    hil_task_delete_all(helicopter_DW.HILInitialize_Card);
    hil_monitor_delete_all(helicopter_DW.HILInitialize_Card);
    hil_close(helicopter_DW.HILInitialize_Card);
    helicopter_DW.HILInitialize_Card = NULL;
  }

  /* Terminate for ToFile: '<Root>/To File' */
  {
    FILE *fp = (FILE *) helicopter_DW.ToFile_PWORK.FilePtr;
    if (fp != (NULL)) {
      char fileName[509] = "plot.mat";
      if (fclose(fp) == EOF) {
        rtmSetErrorStatus(helicopter_M, "Error closing MAT-file plot.mat");
        return;
      }

      if ((fp = fopen(fileName, "r+b")) == (NULL)) {
        rtmSetErrorStatus(helicopter_M, "Error reopening MAT-file plot.mat");
        return;
      }

      if (rt_WriteMat4FileHeader(fp, 1 + 1, helicopter_DW.ToFile_IWORK.Count,
           "ans")) {
        rtmSetErrorStatus(helicopter_M,
                          "Error writing header for ans to MAT-file plot.mat");
      }

      if (fclose(fp) == EOF) {
        rtmSetErrorStatus(helicopter_M, "Error closing MAT-file plot.mat");
        return;
      }

      helicopter_DW.ToFile_PWORK.FilePtr = (NULL);
    }
  }

  /* Terminate for ToFile: '<Root>/To File1' */
  {
    FILE *fp = (FILE *) helicopter_DW.ToFile1_PWORK.FilePtr;
    if (fp != (NULL)) {
      char fileName[509] = "allPlots.mat";
      if (fclose(fp) == EOF) {
        rtmSetErrorStatus(helicopter_M, "Error closing MAT-file allPlots.mat");
        return;
      }

      if ((fp = fopen(fileName, "r+b")) == (NULL)) {
        rtmSetErrorStatus(helicopter_M, "Error reopening MAT-file allPlots.mat");
        return;
      }

      if (rt_WriteMat4FileHeader(fp, 4 + 1, helicopter_DW.ToFile1_IWORK.Count,
           "ans")) {
        rtmSetErrorStatus(helicopter_M,
                          "Error writing header for ans to MAT-file allPlots.mat");
      }

      if (fclose(fp) == EOF) {
        rtmSetErrorStatus(helicopter_M, "Error closing MAT-file allPlots.mat");
        return;
      }

      helicopter_DW.ToFile1_PWORK.FilePtr = (NULL);
    }
  }

  /* Terminate for ToFile: '<Root>/To File2' */
  {
    FILE *fp = (FILE *) helicopter_DW.ToFile2_PWORK.FilePtr;
    if (fp != (NULL)) {
      char fileName[509] = "pref.mat";
      if (fclose(fp) == EOF) {
        rtmSetErrorStatus(helicopter_M, "Error closing MAT-file pref.mat");
        return;
      }

      if ((fp = fopen(fileName, "r+b")) == (NULL)) {
        rtmSetErrorStatus(helicopter_M, "Error reopening MAT-file pref.mat");
        return;
      }

      if (rt_WriteMat4FileHeader(fp, 1 + 1, helicopter_DW.ToFile2_IWORK.Count,
           "ans")) {
        rtmSetErrorStatus(helicopter_M,
                          "Error writing header for ans to MAT-file pref.mat");
      }

      if (fclose(fp) == EOF) {
        rtmSetErrorStatus(helicopter_M, "Error closing MAT-file pref.mat");
        return;
      }

      helicopter_DW.ToFile2_PWORK.FilePtr = (NULL);
    }
  }

  /* Terminate for ToFile: '<Root>/To File3' */
  {
    FILE *fp = (FILE *) helicopter_DW.ToFile3_PWORK.FilePtr;
    if (fp != (NULL)) {
      char fileName[509] = "travel.mat";
      if (fclose(fp) == EOF) {
        rtmSetErrorStatus(helicopter_M, "Error closing MAT-file travel.mat");
        return;
      }

      if ((fp = fopen(fileName, "r+b")) == (NULL)) {
        rtmSetErrorStatus(helicopter_M, "Error reopening MAT-file travel.mat");
        return;
      }

      if (rt_WriteMat4FileHeader(fp, 1 + 1, helicopter_DW.ToFile3_IWORK.Count,
           "ans")) {
        rtmSetErrorStatus(helicopter_M,
                          "Error writing header for ans to MAT-file travel.mat");
      }

      if (fclose(fp) == EOF) {
        rtmSetErrorStatus(helicopter_M, "Error closing MAT-file travel.mat");
        return;
      }

      helicopter_DW.ToFile3_PWORK.FilePtr = (NULL);
    }
  }

  /* Terminate for ToFile: '<Root>/To File4' */
  {
    FILE *fp = (FILE *) helicopter_DW.ToFile4_PWORK.FilePtr;
    if (fp != (NULL)) {
      char fileName[509] = "travel_dot.mat";
      if (fclose(fp) == EOF) {
        rtmSetErrorStatus(helicopter_M, "Error closing MAT-file travel_dot.mat");
        return;
      }

      if ((fp = fopen(fileName, "r+b")) == (NULL)) {
        rtmSetErrorStatus(helicopter_M,
                          "Error reopening MAT-file travel_dot.mat");
        return;
      }

      if (rt_WriteMat4FileHeader(fp, 1 + 1, helicopter_DW.ToFile4_IWORK.Count,
           "ans")) {
        rtmSetErrorStatus(helicopter_M,
                          "Error writing header for ans to MAT-file travel_dot.mat");
      }

      if (fclose(fp) == EOF) {
        rtmSetErrorStatus(helicopter_M, "Error closing MAT-file travel_dot.mat");
        return;
      }

      helicopter_DW.ToFile4_PWORK.FilePtr = (NULL);
    }
  }

  /* Terminate for ToFile: '<Root>/To File5' */
  {
    FILE *fp = (FILE *) helicopter_DW.ToFile5_PWORK.FilePtr;
    if (fp != (NULL)) {
      char fileName[509] = "pitch.mat";
      if (fclose(fp) == EOF) {
        rtmSetErrorStatus(helicopter_M, "Error closing MAT-file pitch.mat");
        return;
      }

      if ((fp = fopen(fileName, "r+b")) == (NULL)) {
        rtmSetErrorStatus(helicopter_M, "Error reopening MAT-file pitch.mat");
        return;
      }

      if (rt_WriteMat4FileHeader(fp, 1 + 1, helicopter_DW.ToFile5_IWORK.Count,
           "ans")) {
        rtmSetErrorStatus(helicopter_M,
                          "Error writing header for ans to MAT-file pitch.mat");
      }

      if (fclose(fp) == EOF) {
        rtmSetErrorStatus(helicopter_M, "Error closing MAT-file pitch.mat");
        return;
      }

      helicopter_DW.ToFile5_PWORK.FilePtr = (NULL);
    }
  }

  /* Terminate for ToFile: '<Root>/To File6' */
  {
    FILE *fp = (FILE *) helicopter_DW.ToFile6_PWORK.FilePtr;
    if (fp != (NULL)) {
      char fileName[509] = "pitch_dot.mat";
      if (fclose(fp) == EOF) {
        rtmSetErrorStatus(helicopter_M, "Error closing MAT-file pitch_dot.mat");
        return;
      }

      if ((fp = fopen(fileName, "r+b")) == (NULL)) {
        rtmSetErrorStatus(helicopter_M, "Error reopening MAT-file pitch_dot.mat");
        return;
      }

      if (rt_WriteMat4FileHeader(fp, 1 + 1, helicopter_DW.ToFile6_IWORK.Count,
           "ans")) {
        rtmSetErrorStatus(helicopter_M,
                          "Error writing header for ans to MAT-file pitch_dot.mat");
      }

      if (fclose(fp) == EOF) {
        rtmSetErrorStatus(helicopter_M, "Error closing MAT-file pitch_dot.mat");
        return;
      }

      helicopter_DW.ToFile6_PWORK.FilePtr = (NULL);
    }
  }

  /* Terminate for ToFile: '<Root>/To File7' */
  {
    FILE *fp = (FILE *) helicopter_DW.ToFile7_PWORK.FilePtr;
    if (fp != (NULL)) {
      char fileName[509] = "elevation.mat";
      if (fclose(fp) == EOF) {
        rtmSetErrorStatus(helicopter_M, "Error closing MAT-file elevation.mat");
        return;
      }

      if ((fp = fopen(fileName, "r+b")) == (NULL)) {
        rtmSetErrorStatus(helicopter_M, "Error reopening MAT-file elevation.mat");
        return;
      }

      if (rt_WriteMat4FileHeader(fp, 1 + 1, helicopter_DW.ToFile7_IWORK.Count,
           "ans")) {
        rtmSetErrorStatus(helicopter_M,
                          "Error writing header for ans to MAT-file elevation.mat");
      }

      if (fclose(fp) == EOF) {
        rtmSetErrorStatus(helicopter_M, "Error closing MAT-file elevation.mat");
        return;
      }

      helicopter_DW.ToFile7_PWORK.FilePtr = (NULL);
    }
  }

  /* Terminate for ToFile: '<Root>/To File8' */
  {
    FILE *fp = (FILE *) helicopter_DW.ToFile8_PWORK.FilePtr;
    if (fp != (NULL)) {
      char fileName[509] = "elevation_dot.mat";
      if (fclose(fp) == EOF) {
        rtmSetErrorStatus(helicopter_M,
                          "Error closing MAT-file elevation_dot.mat");
        return;
      }

      if ((fp = fopen(fileName, "r+b")) == (NULL)) {
        rtmSetErrorStatus(helicopter_M,
                          "Error reopening MAT-file elevation_dot.mat");
        return;
      }

      if (rt_WriteMat4FileHeader(fp, 1 + 1, helicopter_DW.ToFile8_IWORK.Count,
           "ans")) {
        rtmSetErrorStatus(helicopter_M,
                          "Error writing header for ans to MAT-file elevation_dot.mat");
      }

      if (fclose(fp) == EOF) {
        rtmSetErrorStatus(helicopter_M,
                          "Error closing MAT-file elevation_dot.mat");
        return;
      }

      helicopter_DW.ToFile8_PWORK.FilePtr = (NULL);
    }
  }
}

/*========================================================================*
 * Start of Classic call interface                                        *
 *========================================================================*/

/* Solver interface called by GRT_Main */
#ifndef USE_GENERATED_SOLVER

void rt_ODECreateIntegrationData(RTWSolverInfo *si)
{
  UNUSED_PARAMETER(si);
  return;
}                                      /* do nothing */

void rt_ODEDestroyIntegrationData(RTWSolverInfo *si)
{
  UNUSED_PARAMETER(si);
  return;
}                                      /* do nothing */

void rt_ODEUpdateContinuousStates(RTWSolverInfo *si)
{
  UNUSED_PARAMETER(si);
  return;
}                                      /* do nothing */

#endif

void MdlOutputs(int_T tid)
{
  helicopter_output();
  UNUSED_PARAMETER(tid);
}

void MdlUpdate(int_T tid)
{
  helicopter_update();
  UNUSED_PARAMETER(tid);
}

void MdlInitializeSizes(void)
{
}

void MdlInitializeSampleTimes(void)
{
}

void MdlInitialize(void)
{
}

void MdlStart(void)
{
  helicopter_initialize();
}

void MdlTerminate(void)
{
  helicopter_terminate();
}

/* Registration function */
RT_MODEL_helicopter_T *helicopter(void)
{
  /* Registration code */

  /* initialize non-finites */
  rt_InitInfAndNaN(sizeof(real_T));

  /* non-finite (run-time) assignments */
  helicopter_P.Integrator_UpperSat = rtInf;
  helicopter_P.Integrator_LowerSat = rtMinusInf;

  /* initialize real-time model */
  (void) memset((void *)helicopter_M, 0,
                sizeof(RT_MODEL_helicopter_T));

  {
    /* Setup solver object */
    rtsiSetSimTimeStepPtr(&helicopter_M->solverInfo,
                          &helicopter_M->Timing.simTimeStep);
    rtsiSetTPtr(&helicopter_M->solverInfo, &rtmGetTPtr(helicopter_M));
    rtsiSetStepSizePtr(&helicopter_M->solverInfo,
                       &helicopter_M->Timing.stepSize0);
    rtsiSetdXPtr(&helicopter_M->solverInfo, &helicopter_M->derivs);
    rtsiSetContStatesPtr(&helicopter_M->solverInfo, (real_T **)
                         &helicopter_M->contStates);
    rtsiSetNumContStatesPtr(&helicopter_M->solverInfo,
      &helicopter_M->Sizes.numContStates);
    rtsiSetNumPeriodicContStatesPtr(&helicopter_M->solverInfo,
      &helicopter_M->Sizes.numPeriodicContStates);
    rtsiSetPeriodicContStateIndicesPtr(&helicopter_M->solverInfo,
      &helicopter_M->periodicContStateIndices);
    rtsiSetPeriodicContStateRangesPtr(&helicopter_M->solverInfo,
      &helicopter_M->periodicContStateRanges);
    rtsiSetErrorStatusPtr(&helicopter_M->solverInfo, (&rtmGetErrorStatus
      (helicopter_M)));
    rtsiSetRTModelPtr(&helicopter_M->solverInfo, helicopter_M);
  }

  rtsiSetSimTimeStep(&helicopter_M->solverInfo, MAJOR_TIME_STEP);
  helicopter_M->intgData.f[0] = helicopter_M->odeF[0];
  helicopter_M->contStates = ((real_T *) &helicopter_X);
  rtsiSetSolverData(&helicopter_M->solverInfo, (void *)&helicopter_M->intgData);
  rtsiSetSolverName(&helicopter_M->solverInfo,"ode1");

  /* Initialize timing info */
  {
    int_T *mdlTsMap = helicopter_M->Timing.sampleTimeTaskIDArray;
    mdlTsMap[0] = 0;
    mdlTsMap[1] = 1;
    helicopter_M->Timing.sampleTimeTaskIDPtr = (&mdlTsMap[0]);
    helicopter_M->Timing.sampleTimes = (&helicopter_M->Timing.sampleTimesArray[0]);
    helicopter_M->Timing.offsetTimes = (&helicopter_M->Timing.offsetTimesArray[0]);

    /* task periods */
    helicopter_M->Timing.sampleTimes[0] = (0.0);
    helicopter_M->Timing.sampleTimes[1] = (0.002);

    /* task offsets */
    helicopter_M->Timing.offsetTimes[0] = (0.0);
    helicopter_M->Timing.offsetTimes[1] = (0.0);
  }

  rtmSetTPtr(helicopter_M, &helicopter_M->Timing.tArray[0]);

  {
    int_T *mdlSampleHits = helicopter_M->Timing.sampleHitArray;
    mdlSampleHits[0] = 1;
    mdlSampleHits[1] = 1;
    helicopter_M->Timing.sampleHits = (&mdlSampleHits[0]);
  }

  rtmSetTFinal(helicopter_M, -1);
  helicopter_M->Timing.stepSize0 = 0.002;
  helicopter_M->Timing.stepSize1 = 0.002;

  /* External mode info */
  helicopter_M->Sizes.checksums[0] = (2377367895U);
  helicopter_M->Sizes.checksums[1] = (3170518385U);
  helicopter_M->Sizes.checksums[2] = (2516073430U);
  helicopter_M->Sizes.checksums[3] = (2049168385U);

  {
    static const sysRanDType rtAlwaysEnabled = SUBSYS_RAN_BC_ENABLE;
    static RTWExtModeInfo rt_ExtModeInfo;
    static const sysRanDType *systemRan[2];
    helicopter_M->extModeInfo = (&rt_ExtModeInfo);
    rteiSetSubSystemActiveVectorAddresses(&rt_ExtModeInfo, systemRan);
    systemRan[0] = &rtAlwaysEnabled;
    systemRan[1] = (sysRanDType *)&helicopter_DW.IfActionSubsystem_SubsysRanBC;
    rteiSetModelMappingInfoPtr(helicopter_M->extModeInfo,
      &helicopter_M->SpecialInfo.mappingInfo);
    rteiSetChecksumsPtr(helicopter_M->extModeInfo, helicopter_M->Sizes.checksums);
    rteiSetTPtr(helicopter_M->extModeInfo, rtmGetTPtr(helicopter_M));
  }

  helicopter_M->solverInfoPtr = (&helicopter_M->solverInfo);
  helicopter_M->Timing.stepSize = (0.002);
  rtsiSetFixedStepSize(&helicopter_M->solverInfo, 0.002);
  rtsiSetSolverMode(&helicopter_M->solverInfo, SOLVER_MODE_SINGLETASKING);

  /* block I/O */
  helicopter_M->blockIO = ((void *) &helicopter_B);

  {
    int32_T i;
    for (i = 0; i < 6; i++) {
      helicopter_B.Gain1[i] = 0.0;
    }

    helicopter_B.TravelCounttorad = 0.0;
    helicopter_B.Gain = 0.0;
    helicopter_B.Sum3 = 0.0;
    helicopter_B.Gain_d = 0.0;
    helicopter_B.PitchCounttorad = 0.0;
    helicopter_B.Gain_i = 0.0;
    helicopter_B.Gain_b = 0.0;
    helicopter_B.ElevationCounttorad = 0.0;
    helicopter_B.Gain_e = 0.0;
    helicopter_B.Sum = 0.0;
    helicopter_B.Gain_dg = 0.0;
    helicopter_B.Sum4 = 0.0;
    helicopter_B.TmpSignalConversionAtToFile1Inp[0] = 0.0;
    helicopter_B.TmpSignalConversionAtToFile1Inp[1] = 0.0;
    helicopter_B.TmpSignalConversionAtToFile1Inp[2] = 0.0;
    helicopter_B.TmpSignalConversionAtToFile1Inp[3] = 0.0;
    helicopter_B.Sum1 = 0.0;
    helicopter_B.Gain_l = 0.0;
    helicopter_B.BackmotorSaturation = 0.0;
    helicopter_B.FrontmotorSaturation = 0.0;
    helicopter_B.In1 = 0.0;
  }

  /* parameters */
  helicopter_M->defaultParam = ((real_T *)&helicopter_P);

  /* states (continuous) */
  {
    real_T *x = (real_T *) &helicopter_X;
    helicopter_M->contStates = (x);
    (void) memset((void *)&helicopter_X, 0,
                  sizeof(X_helicopter_T));
  }

  /* states (dwork) */
  helicopter_M->dwork = ((void *) &helicopter_DW);
  (void) memset((void *)&helicopter_DW, 0,
                sizeof(DW_helicopter_T));

  {
    int32_T i;
    for (i = 0; i < 8; i++) {
      helicopter_DW.HILInitialize_AIMinimums[i] = 0.0;
    }
  }

  {
    int32_T i;
    for (i = 0; i < 8; i++) {
      helicopter_DW.HILInitialize_AIMaximums[i] = 0.0;
    }
  }

  {
    int32_T i;
    for (i = 0; i < 8; i++) {
      helicopter_DW.HILInitialize_AOMinimums[i] = 0.0;
    }
  }

  {
    int32_T i;
    for (i = 0; i < 8; i++) {
      helicopter_DW.HILInitialize_AOMaximums[i] = 0.0;
    }
  }

  {
    int32_T i;
    for (i = 0; i < 8; i++) {
      helicopter_DW.HILInitialize_AOVoltages[i] = 0.0;
    }
  }

  {
    int32_T i;
    for (i = 0; i < 8; i++) {
      helicopter_DW.HILInitialize_FilterFrequency[i] = 0.0;
    }
  }

  {
    int32_T i;
    for (i = 0; i < 8; i++) {
      helicopter_DW.HILInitialize_POSortedFreqs[i] = 0.0;
    }
  }

  {
    int32_T i;
    for (i = 0; i < 8; i++) {
      helicopter_DW.HILInitialize_POValues[i] = 0.0;
    }
  }

  helicopter_DW.TimeStampA = 0.0;
  helicopter_DW.LastUAtTimeA = 0.0;
  helicopter_DW.TimeStampB = 0.0;
  helicopter_DW.LastUAtTimeB = 0.0;
  helicopter_DW.HILWriteAnalog_Buffer[0] = 0.0;
  helicopter_DW.HILWriteAnalog_Buffer[1] = 0.0;

  /* data type transition information */
  {
    static DataTypeTransInfo dtInfo;
    (void) memset((char_T *) &dtInfo, 0,
                  sizeof(dtInfo));
    helicopter_M->SpecialInfo.mappingInfo = (&dtInfo);
    dtInfo.numDataTypes = 16;
    dtInfo.dataTypeSizes = &rtDataTypeSizes[0];
    dtInfo.dataTypeNames = &rtDataTypeNames[0];

    /* Block I/O transition table */
    dtInfo.BTransTable = &rtBTransTable;

    /* Parameters transition table */
    dtInfo.PTransTable = &rtPTransTable;
  }

  /* Initialize Sizes */
  helicopter_M->Sizes.numContStates = (4);/* Number of continuous states */
  helicopter_M->Sizes.numPeriodicContStates = (0);
                                      /* Number of periodic continuous states */
  helicopter_M->Sizes.numY = (0);      /* Number of model outputs */
  helicopter_M->Sizes.numU = (0);      /* Number of model inputs */
  helicopter_M->Sizes.sysDirFeedThru = (0);/* The model is not direct feedthrough */
  helicopter_M->Sizes.numSampTimes = (2);/* Number of sample times */
  helicopter_M->Sizes.numBlocks = (80);/* Number of blocks */
  helicopter_M->Sizes.numBlockIO = (19);/* Number of block outputs */
  helicopter_M->Sizes.numBlockPrms = (150);/* Sum of parameter "widths" */
  return helicopter_M;
}

/*========================================================================*
 * End of Classic call interface                                          *
 *========================================================================*/
