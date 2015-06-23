#include "globalvars.h"

#if DISPLAY_PROCTIME
// processing time variables
double dGetAudioSamplesStartTime = 0;
double dGetAudioSamplesEndTime = 0;
double dOMEstartTime = 0;
double dOMEendTime = 0;
double dAudioDevIOstartTime = 0;
double dAudioDevIOendTime = 0;
double dPrevAudioDevIOstartTime = 0;
double dNumSamples = 0;
double dPlotRenderStartTime = 0;
double dPlotRenderEndTime = 0;
double dPlotRenderTime = 0;
double dGfxPaintStartTime = 0;
double dGfxPaintMidTime = 0;
double dGfxPaintEndTime = 0;
double dGfxPaintFuncTime = 0;
double dGfxPaintTime = 0;
double dAlgoStartTime[2];
double dAlgoEndTime[2];
double dWrite2FileStartTime = 0;
double dWrite2FileEndTime = 0;
double dAudioDevIOtime = 0;
double dGetAuSampTime = 0;
double dOMEtime = 0;
double dAlgoTime = 0;
double dNumAudioDevIOinvk = 0;
double dNumGetAuSampInvk = 0;
double dNumOMEinvk = 0;
double dNumAlgoInvk = 0;
double dNumPlotRenderInvk = 0;
double dNumGfxPaintInvk = 0;
double dSummedAudioDevIOtime = 0;
double dSummedGetAuSamp = 0;
double dSummedOMEtime = 0;
double dSummedAlgoTime = 0;
double dSummedPlotRenderTime = 0;
double dSummedGfxPaintTime = 0;
double dRTAPsetThingsToDo = 2;
#endif

// semaphore initialisations
double dAlgoProcSem = 0;
double dPixRenderSem = 0;
double dPlotDispSem = 0;

double dDisplayNumBFchannels = 1;
double dDisplayNumANfibreTypes = 1;
double dDisplayNumANchannels = 1;

bool bRTAPrunning = false;
bool bFunctionProcFlag = false;
bool bMemAllocated = false;				// indicates mem allocation & de-allocation
bool bPlayPauseStatus = false;
bool bRecordProcData = false;
bool bRecordStatus = false;
bool bRecordThreadEngaged = false;
bool bRenderPlotInProgress = false;	// spectrogram rendering status
bool bRefreshPlot = false;
bool bRefreshXaxis = false;
bool bAutoRefreshPlot = false;
bool bErasePlot = false;
bool bProcRefreshPlot = false;
bool bScrollPlot = false;
bool bMathOpt = false;
bool bWindowBoundaryDisp = false;
bool bPeakAmpDisp = false;
bool bRTAPsetBtnDispSync = false;
bool bXmlUpdate = false;
#if DOUBLE_PRECISION_BUTTON
bool bDoublePrecision = false;
#endif
char cProcThreadStatus[2];				// processing thread running status for feedback display
char cFileWriterThreadStatus[2];		// file writer thread running status for feedback display
char cProcessPriority[60];
char cAudioSourceName[60];
char cFunctionName[60];
char cFileStatus[100];
char cMathOpt[5];
char cWindowBoundaryDisp[5];
char cPeakAmpDisp[5];
char cYaxisLabel[NUMOFYLABELS][10];					// max 25 BF Y-axis labels
char cXaxisLabel[NUMOFXLABELS][10];
char cANfibreSelect[10];
#if SINGLE_PRECISION
float fAudioSamples2Display[2048];
float fAudioHiResSamples[HIRESBUFFSIZE];
float fBMBuff[2048];
float fNextYPixel = 0;
float fSampleRate = 0;
float fAudioLvldBSPL = 50;
float fAudioInGain = 50;
float fSineInpAmpAdj = 1.f;
float fSamplingRate = 22050;
float fMinBFlist = 250;
float fMaxBFlist = 6000;
float fComprThreshBMdB = 10;
float fDRNLa = 5e4f;
float fDRNLb = 3e-4f;
float fDRNLc = 0.2f;
float fLinGain = 500;
float AudioInGain = 0.f;
float fMinPixDisplayVal = 655636;
float fMaxPixDisplayVal = -655635;
float SampFreqTest = 35.f;
float f2PIoverFs;
float fMaxPeak[3];
#elif DOUBLE_PRECISION
double fAudioSamples2Display[2048];
double fAudioHiResSamples[HIRESBUFFSIZE];
double fBMBuff[2048];
double fNextYPixel = 0;
double fSampleRate = 0;
double fAudioLvldBSPL = 0;
double fAudioInGain = 0;
double fSineInpAmpAdj = 1.f;
double fSamplingRate = 22050;
double fMinBFlist = 250;
double fMaxBFlist = 6000;
double fComprThreshBMdB = 10;
double fDRNLa = 5e4;
double fDRNLb = 3e-4;
double fDRNLc = 0.2;
double fLinGain = 500;
double AudioInGain = 0;
double fMinPixDisplayVal = 655636;
double fMaxPixDisplayVal = -655635;
double SampFreqTest = 35.f;
double f2PIoverFs;
double fMaxPeak[3];
#endif
UCHAR cSaveTimings2TxtFile = 0;
UCHAR cWaveformType = SCATTER_PLOT;
UCHAR cDisplayMode = STATIC_DISPLAY;
char cAudioSource = 0;

UINT uiBFlabelYaxis[NUMOFYLABELS];
UINT uiTimeLabelXaxis[NUMOFXLABELS];
UINT* uiYstart;
UINT* uiYscale;
UINT* uiImgBuffWriteTrack;
UINT uiYspacing;
UINT uiNumofBFchannels = 1;
UINT uiNumofANfibreTypes = 1;
int iANfibreType = 1;
UINT uiBFChannelOffset;
UINT uiHiResAudioWriteTracker = 0;
UINT uiHiResAudioReadTracker = 0;

// Sine tone generator
UINT uiWindowSize = 1;
UINT uiSampPerCycle = 0;
UINT uiCyclesPerWin = 0;
UINT uiUnusedSamp = 0;
int iSineFreq = 500;

UINT uiDestWidth = 0;
UINT uiIncomingNumSamples = 0;
UINT uiFunction2Rec = DRNL_ST_OPT;
UINT uiOffsetbySamplingRate = 0;
UINT uiAudioPackets2BeProcessed = 0;
UINT uiImgSrcX = 0;
UINT uiDestX = 0;
UINT uiDisplayDestX = 0;
UINT uiImgBuffIndxRenderStart = 0;
UINT uiImgBuffDispStartIndex = 0;
UINT uiNumImgBuff2Disp = 0;
UINT uiScreenStartRender = 0;
UINT uiScrollSpeed = 10;
UINT uiBFpoint2Save = 0;
UINT uiNumAuPcktRx = 0;
UINT uiProcDataWin2Disp = 0;
UINT uiRTAPplayStart = 0;
int iRTAPpriority = NORMAL;
int iFunctionToRun = DRNL_ST_OPT;
int siDisplayWindowWidth = 0;
int siNumProcData2Display = 0;
int siNumProcDataRendered = 0;
int siDisplayStartPoint = DISPLAYWINDOWSIZE;
int siSamp2Skip = -1;					// to be used in audioDeviceIOCallback
int iProcessPriority = 0;

// pthread variables
pthread_t hProcThread[2];									// hard coded (2 threads - dual core) for now
pthread_t hAlgoThread;
pthread_t hFileWriterThread;
pthread_t hDrawPixThread[2];
pthread_mutex_t CallProcFuncMutex		=	PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t PlayPauseMutex			=	PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t CallWriteFuncMutex		=	PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t DrawPlotMutex			=	PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t ComputeAlgoMutex		=	PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cvWaitOnPlayButton		=	PTHREAD_COND_INITIALIZER;
pthread_cond_t cvDRNLwaitonAudioSamp	=	PTHREAD_COND_INITIALIZER;
pthread_cond_t cvFileWriteWaitonRecBtn	=	PTHREAD_COND_INITIALIZER;
pthread_cond_t cvDrawPlot				=	PTHREAD_COND_INITIALIZER;
pthread_cond_t cvComputeAlgo			=	PTHREAD_COND_INITIALIZER;
pthread_mutex_t Write2FileMutex[2];
pthread_cond_t cvFileWriteWaitonSignal[2];

UINT uiGlobalShow1=0;
UINT uiGlobalShow2=0;
UINT uiGlobalShow3=0;
UINT uiGlobalShow4=0;
UINT uiGlobalShow5=0;
UINT uiGlobalShow6=0;
UINT uiNumYaxisLabels = 1;
UINT uiMaxBF = 0;
UINT uiMinBF = 0;
UINT uiXintPix = 0;
UINT uiRTAPprocTime = 0;
UINT uiTimeAcc = 0;
UINT uiNumofPeaks = 0;
UINT uiRise = 0;
UINT uiMeanPeakBF[3];
UINT uiRTAPsetStatus = 0;
int iGammatoneFiltOrder = 3;

/* implemented with Matlab command: 
					lowestBF=250; 	highestBF= 5000; 	numChannels=1~30;
					BFlist=100*log10(logspace(lowestBF/100, highestBF/100, numChannels))

					Offset formula to gain access to n channel:
					sum(0:n-1)
					e.g. for 7 channels, the start freq is offset 21 indices from the start of the array:
					1+2+3+4+5+6 = 21
*/

// Variables for plotting real-time waveforms
#if SINGLE_PRECISION
float fImgBuff[NUMOFBFCHANNELS][IMGTEMPSTORESIZE];
float fAudioImgBuff[IMGTEMPSTORESIZE];
float fScrollAudImgBuff[IMGTEMPSTORESIZE];
float* fPeakScore;
float* fMeanPeakScore;
float* fMaxDisp;
float* fMinDisp;
#elif DOUBLE_PRECISION
double fImgBuff[NUMOFBFCHANNELS][IMGTEMPSTORESIZE];
double fAudioImgBuff[IMGTEMPSTORESIZE];
double fScrollAudImgBuff[IMGTEMPSTORESIZE];
double* fPeakScore;
double* fMeanPeakScore;
double* fMaxDisp;
double* fMinDisp;
#endif
float* fPrevXposProcData;
float* fPrevYposProcData;
float fPrevXposAudData = 0;
float fPrevYposAudData = 0;
UINT uiAudioImgBuffWriteTracker = 0;
UINT uiAudioInputWinSize = 0;

