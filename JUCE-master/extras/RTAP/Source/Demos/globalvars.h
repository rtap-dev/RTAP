#ifndef __GLOBALVARS_H__
#define __GLOBALVARS_H__

#include "pthread.h"
#include "../JuceDemoHeader.h"
#include "nconventions.h"
#include <fstream>
#include <fcntl.h>
#include <io.h>								// header file for file access
#include <stdio.h>							// primarily for sprintf
#include <sys/stat.h>						// contain syntax for file access masking
#include <iostream>							// ofstream
#include <string>

#define RTAP_REALTIME_PRIORITY_BUILD		0
#define RTAP_HIGH_PRIORITY_BUILD			1
#define RTAP_ABOVE_NORMAL_PRIORITY_BUILD	0
#define RTAP_NORMAL_PRIORITY_BUILD			0

// --------- > Build options < ---------
#define JUCE_DISPLAY			1			// only 1 of these 2 options must be set during build
#define OPENGL_DISPLAY			0			// if both are set, it will result in RUN-TIME ERROR!!!
// ----- > End of build options < ------

// ----- > Floating point precision settings < -----
#define SINGLE_PRECISION			1
#define DOUBLE_PRECISION			0
#define DOUBLE_PRECISION_BUTTON		0
// -------- > End of FP precision settings < -------

#define OPENGL_SCROLLING_BOX	1
#define OPENGL_DRAW_SINE		0
#define OPENGL_LINESCROLL		0
#define DRAW_INPUTSTIMULUS		0

#define GFX_THREAD_ENABLE		1
#define SCROLL_ON				1

#define MATH_OPTIMISATION		1

#define TEST_AUDIODEVICESETUP	0
#define SHOW_COLOURBAR			0

#define MULTICHANNELS			0			// required for multi-channel BF array
#define MULTITHREADS			0
#define DISPLAY_PROCTIME		1
#define PROCESS_DRNLTIME		1
#define DISPLAY_PROCTIME_DEBUG	0
#define NUMOFBFCHANNELS			300
#define NUMOFFIBERCHANNELS		2
#define VERIFY					0
#define	DISPLAYWINDOWSIZE		674
#define IMGTEMPSTORESIZE		16384//32768
#define PIXELWIDTH				2
#define PIXELHEIGHT				4
#define IMGBUFFSIZE				(IMGTEMPSTORESIZE * PIXELWIDTH)
#define IMGHEIGHT				600
#define PIXEL_YOFFSET			240
#define YSTARTPLOT				100
#define YENDPLOT				490		// End of display window: 490
#define BUFFERGENSIZE			2048
#define HIRESBUFFSIZE			5120
#define TIMEDURATION			58
#define NUMOFCORES				2
#define RECORD_MAXVAL			0
#define USE_WRITE				0
#define USE_FWRITE				1
#define JUCE_FILELOGGER			0
#define SIZEOFHEADER			50
#define PROCPACKETSIZE			16
#define COLOURBARPIXWIDTH		40
#define COLOURBARPIXHEIGHT		40
#define NUMOFYLABELS			25
#define NUMOFXLABELS			6
#define SCROLLNUMSAMP			100
#define MULTI_SAMPLINGFREQ		0
#define VAR_FB					0
// Local origin axis to divide +ve &-ve cylces for different stages of AP
#define LOAXIS_IHCRP			-0.060637951f	// this constant is pre-computed & stored in pEar->pIHC_RP->fRestingPotential
#define LOAXIS_NRR_LSR			0.42197487f
#define LOAXIS_ANSP_LSR			4.9473825f
#define LOAXIS_NRR_HSR			8.0018930f
#define LOAXIS_ANSP_HSR			66.473648f

// -------- > Thread status definition < --------
#define IDLE					0x01
#define READY_TO_RUN			0x02
#define RUNNING					0x03
// ----- > End of thread status definition < ----

// -----> Start of fast exponential macro <-----
#define LITTLE_ENDIAN		1

static union
{
	double d;
	struct
	{
	#ifdef LITTLE_ENDIAN
			int j1, i1;
	#else
			int i1, j1;
	#endif
	} n;
} eco;

#define LN2		0.69314718056		// natural log 2
#define EXP_A	(1048576/LN2)		// 220 / ln2
#define EXP_B	1072693248			// 1023.220
#define EXP_C	60801				// based on lowest RMS relative error

#define EXP(y)	(eco.n.i1 = EXP_A*(y) + (EXP_B - EXP_C), eco.d)
// -----> End of fast exponential macro <-----

// -----> Start of waveform display type options <-----
#define SCATTER_PLOT		0x01
#define LINE_PLOT			0x02
#define SPECTROGRAM_PLOT	0x03
// -----> End of waveform display type options <-----

// -----> Start of display mode options <-----
#define STATIC_DISPLAY		0x01
#define AUTOREFRESH_DISPLAY	0x02
#define SCROLL_DISPLAY		0x03
// -----> End of display mode options <-----

// -----> Line plot variables <-----
extern float* fPrevXposProcData;
extern float* fPrevYposProcData;
extern float fPrevXposAudData;
extern float fPrevYposAudData;

#if SINGLE_PRECISION
typedef struct {
	float* b;
	float* a;
	float* zi;
	float* zf;
	float* x;
	float* y;
	float* prevX;				// input variables from preceeding window (last DFT2filter call)
	float* prevY;				// output variables from preceeding window (last DFT2filter call)
	float* prevS;				// input variables from previous window of 1st gammatone pass
	float* prevG1Y;				// output variables from previous window of 1st gammatone pass
	float* prevG2X;				// input variables from previous window of 2nd gammatone pass
	float* prevG2Y;				// output variables from previous window of 2nd gammatone pass (1st order gammatone filter buffer utlisation end)
	float* prevG3X;				// input variables from previous window of 3rd gammatone pass
	float* prevG3Y;				// output variables from previous window of 3rd gammatone pass
	float* prevG4X;				// input variables from previous window of 4th gammatone pass
	float* prevG4Y;				// output variables from previous window of 4th gammatone pass (2nd order gammatone filter buffer utlisation end)
	float* prevG5X;				// input variables from previous window of 5th gammatone pass
	float* prevG5Y;				// output variables from previous window of 5th gammatone pass
	float* prevG6X;				// input variables from previous window of 6th gammatone pass
	float* prevG6Y;				// output variables from previous window of 6th gammatone pass (3rd order gammatone filter buffer utulisation end)
	float* prevG7X;				// input variables from previous window of 7th gammatone pass
	float* prevG7Y;				// output variables from previous window of 7th gammatone pass
	float* prevG8X;				// input variables from previous window of 8th gammatone pass
	float* prevG8Y;				// output variables from previous window of 8th gammatone pass (4th order gammatone filter buffer utilisation end)
	float* prevG9X;				// input variables from previous window of 9th gammatone pass
	float* prevG9Y;				// output variables from previous window of 9th gammatone pass
	float* prevG10X;			// input variables from previous window of 10th gammatone pass
	float* prevG10Y;			// output variables from previous window of 10th gammatone pass (5th order gammatone filter buffer utlisation end)
	UINT uiNumOrder;
	UINT uiDenOrder;
	UINT uiAudioInputWinSize;
	UINT uiZSize;
	UINT uiWinContinuity;
	SINT iGammatonePassStatus;
	bool bLoopLock;
	bool bFirstWindow;			// flag status represents whether or not 1st window has been processed
	bool bBoundaryValue;		// required for using z values
}strFilterParams;

typedef struct {
	strFilterParams* pLinGammatone;
	strFilterParams* pNonlinGammatone;
	strFilterParams* pIHCciliaFilter;
}strEarFilters;

typedef struct {
	strFilterParams* pEERfilter;
	strFilterParams* pTMfilter;
	strFilterParams* pSIfilter;
	float* fInpEER;
	float* fInpTMdisplacement;
	float* fInpMidEarStapesInertia;
	float* fStapesDisplacement;
	UINT uiIOBuffSize;
}OuterMiddleEarStruct;

typedef struct {
	float* fBFlist;
	float* fLinCF;
	float* fLinBW;
	float* fNonLinBW;
	float* fNonLinCF;
	float fLinGain;
	float fDRNLa;
	float fDRNLb;
	float fDRNLc;
	float fCompThreshBMdB;
	float fCompThreshBM;
	float fCompThreshScaled;
	float fP;
	float fQ;
	float fDBSvar1;				// optimised parameters for speedup
	UINT uiLinOrder;
	UINT uiNonLinOrder;
	UINT uiFilterOrderParity;
}DRNLstruct;

typedef struct {
	float* fIHCciliaDisplacement;
	float* fGu;
	float* fInitialV;			// initial voltage for different BF
	float* fIHCciliaDispScaled;	// temp scaled cilia displacement buffer
	float fTC;
	float fC;
	float fU0;
	float fS0;
	float fU1;
	float fS1;
	float fGmax;				// max conductane (Siemens)
	float fGa;					// fixed apical membrane conductance
	float fCap;					// IHC capacitance
	float fEt;					// Endocochlear potential
	float fGk;					// Potassium conductance
	float fEk;					// Potassium equilibrium potential
	float fRpc;					// combined resistances
	float fGu0;
	float fEkp;
	float fRestingPotential;
	float fIHCrpVar1;			// optimised parameter for speedup
	float fExpU0oS0;			// stores exp (U0 / S0)
	float fExpU1oS1;			// stores exp (U1 / S1)
	float fExpUtoS0;			// stores exp (U(t) / S0)
	float fExpUtoS1;			// stores exp (U(t) / S1)
}IHC_RPstruct;

typedef struct {
	double dwZ;					// scalar Ca -> vesicle release rate
	float** fmICaCurrent;
	float** fCaConcentration;			// calcium current to calculate initial vesicle release rate
	float* fTauCa;				// = [fLSRtauCa fHSRtauCa]
	float* fInitial_mICaCurrent;
	float* fInitial_CaConcentration;
	float* fKT0;				// initial vesicle release rate
	float* fmICaINF;			// look-up table for target calcium channels open for a given IHC_RP (ptr 4 mt use)
	float* fExponent;			// exponent value (ptr 4 mt use)
	float* fICa;				// calcium current during recursive function call (ptr 4 mt use)
	float fGmaxCa;				// maximum calcium conductance
	float fECa;					// calcium equilibrium conductance
	float fBeta;				// determines calcium channel opening
	float fGamma;				// determines calcium channel opening
	float fTauM;				// membrane time constant
	float fPower;
	float fIHCpresynapseVar1;	// optimised parameter for speedup
}IHC_presynapseStruct;

typedef struct {
	float*** fPrevANprobOutputRate;
	float** fANProbNotFiring;
	float** fPrevANprobNotFiring;
	float** fProbCleft;
	float** fProbAvailable;
	float** fProbReprocess;
	bool** bFirstWindow;
	float* fProbAN_ydt;
	float* fProbAN_ldt;
	float* fProbAN_xdt;
	float* fProbAN_rdt;
	float* fProbAN_rdtldt;
	float* fInitProbCleft;
	float* fInitProbAvailable;
	float* fInitProbReprocess;
	float* fM_Pq;
	float* fProbReplenish;
	float* fProbEjected;
	float* fProbReprocessed;
	float* fReuptakeAndLost;
	float* fReuptake;
	float fRefractoryPeriod;			// absolute and relative refractory period is the same
	float fTWdelay;						// delay before stimulus 1st spike
	float fSpikesTargetSampleRate;
	float fM;							// max vesicles at synapse
	float fY;							// depleted vesicle replacement rate
	float fX;							// replenishment from re-uptake store
	float fL;							// loss rate of vesicle from cleft
	float fR;							// re-uptake rate from cleft into cell
	UINT uiRefractoryPeriodIndex;		// index to the previous values of ANprobRateOutput
	UINT uiRefrac;
}AN_IHCsynapseStruct;

typedef struct {
	OuterMiddleEarStruct* pOMEarStruct;
	DRNLstruct* pDRNLstruct;
	IHC_RPstruct* pIHC_RP;
	IHC_presynapseStruct* pIHC_presynapse;
	AN_IHCsynapseStruct* pAN_IHCsynapse;
	strEarFilters* pEarFilters;
	strFilterParams* pFilterParams;
	float** fDRNLlinInput;
	float** fDRNLlinOutput;				// 2 instances of DRNL outputs for 2 threads running concurrently
	float** fDRNLnonlinInput;
	float** fDRNLnonlinOutput;
	float** fDRNLresponse;				// DRNL buff of interest
	float** fIHCrpResponse;				// IHC receptor potential of interest
	float** fVesicleReleaseRate;		// IHC vesicle release rate of interest
	float** fANprobOutputRate;			// AN probability rate output output
	float* fYvalues;					// for storage of max & min processed samples
	float* fProcData2Sav1;
	float* fProcData2Sav2;
	float* fProcData2Sav3;
	float fMaxAcc;
	float fMinAcc;
	float fDt;							// sampling period
	float fRestingV;					// resting membrane potential
	float fSignalLeveldB;
	float fAudSigDataDisAcc;
	float fProcDataDispAcc;
	UINT** uiRecBuffInc;
	UINT* uiStartAudioSampPt;
	UINT* uiBFprocStart;
	UINT* uiBFprocEnd;
	UINT uiAudioInRecBuffTrack;
	UINT uiMaxCount;
	UINT uiMinCount;
	UINT uiTimerCount;
	UINT uiThreadNum;
	UINT uiAudioInputWinSize;
	UINT uiNumSampToRec;
	UINT uiBFChannelOffset;
	UINT uiNumBFchannels;
	UINT uiMTnumBFchannelsMidPt;		// the mid boundary for separating 2 groups of BF pts.
	UINT uiMTnumANfTypesMidPt;			// the mid boundary for segregating ICH & AN presynapse based on num of cores (num of BF pt. groups).
	UINT uiMTnumANfTypesEndPt;			// the end cap for num of AN fiber types based on num of CPU cores (num of BF pt. groups).
	UINT uiNumANfibreTypes;				// number of AN fiber types - LSR & HSR
	UINT uiNumANchannels;
	UINT uiPacketSize;
	UINT uiCurrBF2Dump;
	SINT iBFcounter;					// counter that increments to instruct thread to process BF points (stored in array above).
	SINT iFileWriterCounter;			// counter that increments when proc data file writer thread created
	bool* bComputationDone;				// flag to indicate whether the thread finish processing 1 packet
}EarStruct;

extern float AudioInGain;

extern float fAudioSamples2Display[2048];
extern float fAudioHiResSamples[HIRESBUFFSIZE];
extern float fBMBuff[2048];
extern float fNextYPixel;
extern float fSampleRate;
extern float fSineInpAmpAdj;
extern float fImgBuff[NUMOFBFCHANNELS][IMGTEMPSTORESIZE];					// image buffer (holds pixel contents of user-selected waveform)
extern float fAudioImgBuff[IMGTEMPSTORESIZE];
extern float fScrollAudImgBuff[IMGTEMPSTORESIZE];
extern float* fPeakScore;
extern float* fMeanPeakScore;
extern float fMaxPeak[3];
extern float* fMaxDisp;
extern float* fMinDisp;

// User-modifiable system parameters start
extern float fAudioLvldBSPL;
extern float fAudioInGain;
extern float fSamplingRate;
extern float fMinBFlist;
extern float fMaxBFlist;
extern float fComprThreshBMdB;
extern float fDRNLa;
extern float fDRNLb;
extern float fDRNLc;
extern float fLinGain;
extern float SampFreqTest;
extern float f2PIoverFs;
// User-modifiable system parameters end

extern float fMinPixDisplayVal;
extern float fMaxPixDisplayVal;

// End of single precision initialisation
#elif DOUBLE_PRECISION
// Start of double precision initialisation

typedef struct {
	double* b;
	double* a;
	double* zi;
	double* zf;
	double* x;
	double* y;
	double* prevX;					// input variables from preceeding window (last DFT2filter call)
	double* prevY;					// output variables from preceeding window (last DFT2filter call)
	double* prevS;					// input variables from previous window of 1st gammatone pass
	double* prevG1Y;				// output variables from previous window of 1st gammatone pass
	double* prevG2X;				// input variables from previous window of 2nd gammatone pass
	double* prevG2Y;				// output variables from previous window of 2nd gammatone pass
	double* prevG3X;				// input variables from previous window of 3rd gammatone pass
	double* prevG3Y;				// output variables from previous window of 3rd gammatone pass
	double* prevG4X;				// input variables from previous window of 4th gammatone pass
	double* prevG4Y;				// output variables from previous window of 4th gammatone pass
	double* prevG5X;				// input variables from previous window of 5th gammatone pass
	double* prevG5Y;				// output variables from previous window of 5th gammatone pass
	double* prevG6X;				// input variables from previous window of 6th gammatone pass
	double* prevG6Y;				// output variables from previous window of 6th gammatone pass
	UINT uiNumOrder;
	UINT uiDenOrder;
	UINT uiAudioInputWinSize;
	UINT uiZSize;
	UINT uiWinContinuity;
	SINT iGammatonePassStatus;
	bool bLoopLock;
	bool bFirstWindow;			// flag status represents whether or not 1st window has been processed
	bool bBoundaryValue;		// required for using z values
}strFilterParams;

typedef struct {
	strFilterParams* pLinGammatone;
	strFilterParams* pNonlinGammatone;
	strFilterParams* pIHCciliaFilter;
}strEarFilters;

typedef struct {
	strFilterParams* pEERfilter;
	strFilterParams* pTMfilter;
	strFilterParams* pSIfilter;
	double* fInpEER;
	double* fInpTMdisplacement;
	double* fInpMidEarStapesInertia;
	double* fStapesDisplacement;
	UINT uiIOBuffSize;
}OuterMiddleEarStruct;

typedef struct {
	double* fBFlist;
	double* fLinCF;
	double* fLinBW;
	double* fNonLinBW;
	double* fNonLinCF;
	double fLinGain;
	double fDRNLa;
	double fDRNLb;
	double fDRNLc;
	double fCompThreshBMdB;
	double fCompThreshBM;
	double fCompThreshScaled;
	double fP;
	double fQ;
	double fDBSvar1;				// optimised parameters for speedup
	UINT uiLinOrder;
	UINT uiNonLinOrder;
	UINT uiFilterOrderParity;
}DRNLstruct;

typedef struct {
	double* fIHCciliaDisplacement;
	double* fGu;
	double* fInitialV;				// initial voltage for different BF
	double* fIHCciliaDispScaled;	// temp scaled cilia displacement buffer
	double fTC;
	double fC;
	double fU0;
	double fS0;
	double fU1;
	double fS1;
	double fGmax;					// max conductane (Siemens)
	double fGa;						// fixed apical membrane conductance
	double fCap;					// IHC capacitance
	double fEt;						// Endocochlear potential
	double fGk;						// Potassium conductance
	double fEk;						// Potassium equilibrium potential
	double fRpc;					// combined resistances
	double fGu0;
	double fEkp;
	double fRestingPotential;
	double fIHCrpVar1;				// optimised parameter for speedup
	double fExpU0oS0;				// stores exp (U0 / S0)
	double fExpU1oS1;				// stores exp (U1 / S1)
	double fExpUtoS0;				// stores exp (U(t) / S0)
	double fExpUtoS1;				// stores exp (U(t) / S1)
}IHC_RPstruct;

typedef struct {
	double dwZ;					// scalar Ca -> vesicle release rate
	double** fmICaCurrent;
	double** fCaConcentration;			// calcium current to calculate initial vesicle release rate
	double* fTauCa;				// = [fLSRtauCa fHSRtauCa]
	double* fInitial_mICaCurrent;
	double* fInitial_CaConcentration;
	double* fKT0;				// initial vesicle release rate
	double* fmICaINF;			// look-up table for target calcium channels open for a given IHC_RP (ptr 4 mt use)
	double* fExponent;			// exponent value (ptr 4 mt use)
	double* fICa;				// calcium current during recursive function call (ptr 4 mt use)
	double fGmaxCa;				// maximum calcium conductance
	double fECa;					// calcium equilibrium conductance
	double fBeta;				// determines calcium channel opening
	double fGamma;				// determines calcium channel opening
	double fTauM;				// membrane time constant
	double fPower;
	double fIHCpresynapseVar1;	// optimised parameter for speedup
}IHC_presynapseStruct;

typedef struct {
	double*** fPrevANprobOutputRate;
	double** fANProbNotFiring;
	double** fPrevANprobNotFiring;
	double** fProbCleft;
	double** fProbAvailable;
	double** fProbReprocess;
	bool** bFirstWindow;
	double* fProbAN_ydt;
	double* fProbAN_ldt;
	double* fProbAN_xdt;
	double* fProbAN_rdt;
	double* fProbAN_rdtldt;
	double* fInitProbCleft;
	double* fInitProbAvailable;
	double* fInitProbReprocess;
	double* fM_Pq;
	double* fProbReplenish;
	double* fProbEjected;
	double* fProbReprocessed;
	double* fReuptakeAndLost;
	double* fReuptake;
	double fRefractoryPeriod;			// absolute and relative refractory period is the same
	double fTWdelay;						// delay before stimulus 1st spike
	double fSpikesTargetSampleRate;
	double fM;							// max vesicles at synapse
	double fY;							// depleted vesicle replacement rate
	double fX;							// replenishment from re-uptake store
	double fL;							// loss rate of vesicle from cleft
	double fR;							// re-uptake rate from cleft into cell
	UINT uiRefractoryPeriodIndex;		// index to the previous values of ANprobRateOutput
	UINT uiRefrac;
}AN_IHCsynapseStruct;

typedef struct {
	OuterMiddleEarStruct* pOMEarStruct;
	DRNLstruct* pDRNLstruct;
	IHC_RPstruct* pIHC_RP;
	IHC_presynapseStruct* pIHC_presynapse;
	AN_IHCsynapseStruct* pAN_IHCsynapse;
	strEarFilters* pEarFilters;
	strFilterParams* pFilterParams;
	double** fDRNLlinInput;
	double** fDRNLlinOutput;				// 2 instances of DRNL outputs for 2 threads running concurrently
	double** fDRNLnonlinInput;
	double** fDRNLnonlinOutput;
	double** fDRNLresponse;				// DRNL buff of interest
	double** fIHCrpResponse;				// IHC receptor potential of interest
	double** fVesicleReleaseRate;		// IHC vesicle release rate of interest
	double** fANprobOutputRate;			// AN probability rate output output
	double* fYvalues;					// for storage of max & min processed samples
	double* fProcData2Sav1;
	double* fProcData2Sav2;
	double* fProcData2Sav3;
	double fAudSigDataDisAcc;
	double fProcDataDispAcc;
	double fMaxAcc;
	double fMinAcc;
	double fDt;							// sampling period
	double fRestingV;					// resting membrane potential
	double fSignalLeveldB;
	UINT** uiRecBuffInc;
	UINT* uiStartAudioSampPt;
	UINT* uiBFprocStart;
	UINT* uiBFprocEnd;
	UINT uiAudioInRecBuffTrack;
	UINT uiMaxCount;
	UINT uiMinCount;
	UINT uiTimerCount;
	UINT uiThreadNum;
	UINT uiAudioInputWinSize;
	UINT uiNumSampToRec;
	UINT uiBFChannelOffset;
	UINT uiNumBFchannels;
	UINT uiMTnumBFchannelsMidPt;		// the mid boundary for separating 2 groups of BF pts.
	UINT uiMTnumANfTypesMidPt;			// the mid boundary for segregating ICH & AN presynapse based on num of cores (num of BF pt. groups).
	UINT uiMTnumANfTypesEndPt;			// the end cap for num of AN fiber types based on num of CPU cores (num of BF pt. groups).
	UINT uiNumANfibreTypes;				// number of AN fiber types - LSR & HSR
	UINT uiNumANchannels;
	UINT uiPacketSize;
	UINT uiCurrBF2Dump;
	SINT iBFcounter;					// counter that increments to instruct thread to process BF points (stored in array above).
	SINT iFileWriterCounter;			// counter that increments when proc data file writer thread created
	bool* bComputationDone;				// flag to indicate whether the thread finish processing 1 packet
}EarStruct;

extern double AudioInGain;

extern double fAudioSamples2Display[2048];
extern double fAudioHiResSamples[HIRESBUFFSIZE];
extern double fBMBuff[2048];
extern double fNextYPixel;
extern double fSampleRate;
extern double fSineInpAmpAdj;
extern double fImgBuff[NUMOFBFCHANNELS][IMGTEMPSTORESIZE];					// image buffer (holds pixel contents of user-selected waveform)
extern double fAudioImgBuff[IMGTEMPSTORESIZE];
extern double fScrollAudImgBuff[IMGTEMPSTORESIZE];
extern double* fPeakScore;
extern double* fMeanPeakScore;
extern double fMaxPeak[3];
extern double* fMaxDisp;
extern double* fMinDIsp;

// User-modifiable system parameters start
extern double fAudioLvldBSPL;
extern double fAudioInGain;
extern double fSamplingRate;
extern double fMinBFlist;
extern double fMaxBFlist;
extern double fComprThreshBMdB;
extern double fDRNLa;
extern double fDRNLb;
extern double fDRNLc;
extern double fLinGain;
extern double SampFreqTest;
extern double f2PIoverFs;
// User-modifiable system parameters end

extern double fMinPixDisplayVal;
extern double fMaxPixDisplayVal;

#endif		// End of double precision segment initialisation

#if DISPLAY_PROCTIME
// processing time variables
extern double dGetAudioSamplesStartTime;
extern double dGetAudioSamplesEndTime;
extern double dOMEstartTime;
extern double dOMEendTime;
extern double dAudioDevIOstartTime;
extern double dAudioDevIOendTime;
extern double dPrevAudioDevIOstartTime;
extern double dNumSamples;
extern double dPlotRenderStartTime;
extern double dPlotRenderEndTime;
extern double dPlotRenderTime;
extern double dGfxPaintStartTime;
extern double dGfxPaintMidTime;
extern double dGfxPaintEndTime;
extern double dGfxPaintFuncTime;
extern double dGfxPaintTime;
extern double dAlgoStartTime[2];
extern double dAlgoEndTime[2];
extern double dWrite2FileStartTime;
extern double dWrite2FileEndTime;
extern double dNumAudioDevIOinvk;
extern double dNumGetAuSampInvk;
extern double dNumOMEinvk;
extern double dNumAlgoInvk;
extern double dNumPlotRenderInvk;
extern double dNumGfxPaintInvk;
extern double dAudioDevIOtime;
extern double dGetAuSampTime;
extern double dOMEtime;
extern double dAlgoTime;
extern double dSummedAudioDevIOtime;
extern double dSummedGetAuSamp;
extern double dSummedOMEtime;
extern double dSummedAlgoTime;
extern double dSummedPlotRenderTime;
extern double dSummedGfxPaintTime;
extern double dRTAPsetThingsToDo;
#endif

// customised counting semaphore variables to track task & ensure 
// that they do not interfere with on another's operations
extern double dAlgoProcSem;			// algorithm processing time frame traking
extern double dPixRenderSem;		// pixel rendering time frame tracking
extern double dPlotDispSem;			// plot display time frame tracking

// RTAP info page content variable display
extern double dDisplayNumBFchannels;
extern double dDisplayNumANchannels;
extern double dDisplayNumANfibreTypes;

extern UINT uiBFlabelYaxis[NUMOFYLABELS];
extern UINT uiTimeLabelXaxis[NUMOFXLABELS];
extern UINT* uiYstart;
extern UINT* uiYscale;
extern UINT* uiImgBuffWriteTrack;
//extern UINT* uiImgBuffReadStart;
extern UINT uiYspacing;
extern UINT uiAudioImgBuffWriteTracker;
extern UINT uiAudioInputWinSize;
extern UINT uiIncomingNumSamples;
extern UINT uiNumofBFchannels;
extern UINT uiNumofANfibreTypes;
extern int iANfibreType;
extern UINT uiBFChannelOffset;
extern UINT uiHiResAudioWriteTracker;
extern UINT uiHiResAudioReadTracker;

// Sine tone generator variables
extern UINT uiWindowSize;
extern UINT uiSampPerCycle;
extern UINT uiCyclesPerWin;
extern UINT uiUnusedSamp;
extern int iSineFreq;

extern UINT uiDestWidth;
extern UINT uiFunction2Rec;
extern UINT uiOffsetbySamplingRate;
extern UINT uiAudioPackets2BeProcessed;
extern UINT uiImgSrcX;
extern UINT uiDestX;
extern UINT uiDisplayDestX;
extern UINT uiImgBuffIndxRenderStart;
extern UINT uiImgBuffDispStartIndex;
extern UINT uiNumImgBuff2Disp;
extern UINT uiScreenStartRender;
extern UINT uiScrollSpeed;
extern UINT uiBFpoint2Save;
extern UINT uiNumYaxisLabels;
extern UINT uiMaxBF;
extern UINT uiMinBF;
extern UINT uiXintPix;
extern UINT uiRTAPprocTime;
extern UINT uiTimeAcc;
extern UINT uiNumofPeaks;
extern UINT uiRise;
extern UINT uiMeanPeakBF[3];
extern UINT uiRTAPsetStatus;
extern UINT uiNumAuPcktRx;					// tracks number of audio packets (windowed data) received
extern UINT uiProcDataWin2Disp;				// tracks the ith window of processed data to display
extern UINT uiRTAPplayStart;
extern int iRTAPpriority;
extern int iFunctionToRun;
extern int siDisplayWindowWidth;
extern int siNumProcData2Display;
extern int siNumProcDataRendered;
extern int siDisplayStartPoint;
extern int siSamp2Skip;						// to be used in audioDeviceIOCallback
extern int iProcessPriority;
extern int iGammatoneFiltOrder;

extern UCHAR cSaveTimings2TxtFile;
extern UCHAR cWaveformType;
extern UCHAR cDisplayMode;
extern char cAudioSource;

extern UINT uiGlobalShow1;
extern UINT uiGlobalShow2;
extern UINT uiGlobalShow3;
extern UINT uiGlobalShow4;
extern UINT uiGlobalShow5;
extern UINT uiGlobalShow6;

extern char cProcThreadStatus[2];
extern char cFileWriterThreadStatus[2];
extern char cProcessPriority[60];
extern char cAudioSourceName[60];
extern char cYaxisLabel[NUMOFYLABELS][10];
extern char cXaxisLabel[NUMOFXLABELS][10];
extern char cFunctionName[60];
extern char cFileStatus[100];
extern char cMathOpt[5];
extern char cWindowBoundaryDisp[5];
extern char cPeakAmpDisp[5];
extern char cANfibreSelect[10];
extern bool bRTAPrunning;
extern bool bFunctionProcFlag;
extern bool bMemAllocated;
extern bool bPlayPauseStatus;
extern bool bRecordProcData;
extern bool bRecordStatus;
extern bool bRecordThreadEngaged;
extern bool bRenderPlotInProgress;
extern bool bRefreshPlot;
extern bool bRefreshXaxis;
extern bool bAutoRefreshPlot;
extern bool bErasePlot;
extern bool bProcRefreshPlot;
extern bool bScrollPlot;
extern bool bMathOpt;
extern bool bWindowBoundaryDisp;
extern bool bPeakAmpDisp;
extern bool bRTAPsetBtnDispSync;
extern bool bXmlUpdate;
#if DOUBLE_PRECISION_BUTTON
extern bool bDoublePrecision;
#endif

enum RTAPpriority
{
	REAL_TIME=1,
	HIGH,
	ABOVE_NORMAL,
	NORMAL
};

enum AudioSrc
{
	AUDIO_SYSTEM,
	AUDIO_SINE,
	AUDIO_SQUARE,
	AUDIO_SAWTOOTH
};

enum AuditoryPeriphery2Process
{
	AUDIO_IN = 1,
	EER_OUT,
	TM_DISP,
	STAPES_DISP,
	DRNL_ST,
	DRNL2IHCRP_ST,
	DRNL2IHCpresynapse_ST,
	DRNL2ANsynapse_ST,
	DRNL_ST_OPT,
	DRNL2IHCRP_ST_OPT,
	DRNL2IHCpresynapse_ST_OPT,
	DRNL2ANsynapse_ST_OPT,
	DRNL_MT,
	DRNL2IHCRP_MT,
	DRNL2IHCpresynapse_MT,
	DRNL2ANsynapse_MT
};

enum RTAPconfigStatus
{
	MemFreed = 1,
	MemReallocated,
	APparamsSet,
	APalgoSet,
	ReinitialiseCompleted
};

// pthread variables
extern pthread_t hProcThread[2];									// hard coded (2 threads - dual core) for now
extern pthread_t hAlgoThread;
extern pthread_t hFileWriterThread;
extern pthread_t hDrawPixThread[2];
extern pthread_mutex_t CallProcFuncMutex;
extern pthread_mutex_t PlayPauseMutex;
extern pthread_mutex_t CallWriteFuncMutex;
extern pthread_mutex_t DrawPlotMutex;
extern pthread_mutex_t ComputeAlgoMutex;
extern pthread_cond_t cvWaitOnPlayButton;
extern pthread_cond_t cvDRNLwaitonAudioSamp;
extern pthread_cond_t cvFileWriteWaitonRecBtn;
extern pthread_cond_t cvDrawPlot;
extern pthread_cond_t cvComputeAlgo;

extern pthread_mutex_t Write2FileMutex[2];
extern pthread_cond_t cvFileWriteWaitonSignal[2];

#endif