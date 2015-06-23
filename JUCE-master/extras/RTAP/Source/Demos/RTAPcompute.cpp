/*
	Real-time Auditory Periphery (RTAP)

	RTAPcompute.cpp

	This file contains the algorithm for computing stapes displacement, basilar membrane displacement, inner hair cell receptor potential,
	neurotransmitter release rate and auditory nerve spiking probabilities.

*/
#include <Windows.h>
#include "RTAPcompute.h"

using namespace std;

RTAPcompute::RTAPcompute()
{
	// allocate mem for various structs here
	pEar = new EarStruct;
	pFilterParams = new strFilterParams;
	pEar->pOMEarStruct = new OuterMiddleEarStruct;
	pEar->pOMEarStruct->pEERfilter = new strFilterParams;
	pEar->pOMEarStruct->pTMfilter = new strFilterParams;
	pEar->pOMEarStruct->pSIfilter = new strFilterParams;
	pEar->pEarFilters = new strEarFilters;
	pEar->pDRNLstruct = new DRNLstruct;
	pEar->pIHC_RP = new IHC_RPstruct;
	pEar->pIHC_presynapse = new IHC_presynapseStruct;
	pEar->pAN_IHCsynapse = new AN_IHCsynapseStruct;

	// allocate EER filter struct mem
	pEar->pOMEarStruct->pEERfilter->uiNumOrder = 3;
	pEar->pOMEarStruct->pEERfilter->uiDenOrder = 3;
	pEar->pOMEarStruct->pEERfilter->uiZSize = pEar->pOMEarStruct->pEERfilter->uiDenOrder - 1;
#if SINGLE_PRECISION
	pEar->pOMEarStruct->pEERfilter->prevX = new float[pEar->pOMEarStruct->pEERfilter->uiNumOrder-1];
	pEar->pOMEarStruct->pEERfilter->prevY = new float[pEar->pOMEarStruct->pEERfilter->uiDenOrder-1];
	pEar->pOMEarStruct->pEERfilter->b = new float[pEar->pOMEarStruct->pEERfilter->uiNumOrder];
	pEar->pOMEarStruct->pEERfilter->a = new float[pEar->pOMEarStruct->pEERfilter->uiDenOrder];
	pEar->pOMEarStruct->pEERfilter->zi = new float [pEar->pOMEarStruct->pEERfilter->uiZSize];
	pEar->pOMEarStruct->pEERfilter->zf = new float [pEar->pOMEarStruct->pEERfilter->uiZSize];
#elif DOUBLE_PRECISION
	pEar->pOMEarStruct->pEERfilter->prevX = new double[pEar->pOMEarStruct->pEERfilter->uiNumOrder-1];
	pEar->pOMEarStruct->pEERfilter->prevY = new double[pEar->pOMEarStruct->pEERfilter->uiDenOrder-1];
	pEar->pOMEarStruct->pEERfilter->b = new double[pEar->pOMEarStruct->pEERfilter->uiNumOrder];
	pEar->pOMEarStruct->pEERfilter->a = new double[pEar->pOMEarStruct->pEERfilter->uiDenOrder];
	pEar->pOMEarStruct->pEERfilter->zi = new double [pEar->pOMEarStruct->pEERfilter->uiZSize];
	pEar->pOMEarStruct->pEERfilter->zf = new double [pEar->pOMEarStruct->pEERfilter->uiZSize];
#endif
	pEar->pOMEarStruct->pEERfilter->iGammatonePassStatus = -1;

	// allocate TM filter struct mem
	pEar->pOMEarStruct->pTMfilter->uiNumOrder = 1;
	pEar->pOMEarStruct->pTMfilter->uiDenOrder = 2;
	pEar->pOMEarStruct->pTMfilter->uiZSize = pEar->pOMEarStruct->pTMfilter->uiDenOrder - 1;
#if SINGLE_PRECISION
	pEar->pOMEarStruct->pTMfilter->prevX = new float [pEar->pOMEarStruct->pTMfilter->uiNumOrder-1];
	pEar->pOMEarStruct->pTMfilter->prevY = new float [pEar->pOMEarStruct->pTMfilter->uiDenOrder-1];
	pEar->pOMEarStruct->pTMfilter->b = new float [pEar->pOMEarStruct->pTMfilter->uiNumOrder];
	pEar->pOMEarStruct->pTMfilter->a = new float [pEar->pOMEarStruct->pTMfilter->uiDenOrder];
	pEar->pOMEarStruct->pTMfilter->zi = new float [pEar->pOMEarStruct->pTMfilter->uiZSize];
	pEar->pOMEarStruct->pTMfilter->zf = new float [pEar->pOMEarStruct->pTMfilter->uiZSize];
#elif DOUBLE_PRECISION
	pEar->pOMEarStruct->pTMfilter->prevX = new double [pEar->pOMEarStruct->pTMfilter->uiNumOrder-1];
	pEar->pOMEarStruct->pTMfilter->prevY = new double [pEar->pOMEarStruct->pTMfilter->uiDenOrder-1];
	pEar->pOMEarStruct->pTMfilter->b = new double [pEar->pOMEarStruct->pTMfilter->uiNumOrder];
	pEar->pOMEarStruct->pTMfilter->a = new double [pEar->pOMEarStruct->pTMfilter->uiDenOrder];
	pEar->pOMEarStruct->pTMfilter->zi = new double [pEar->pOMEarStruct->pTMfilter->uiZSize];
	pEar->pOMEarStruct->pTMfilter->zf = new double [pEar->pOMEarStruct->pTMfilter->uiZSize];
#endif
	pEar->pOMEarStruct->pTMfilter->iGammatonePassStatus = -1;
	
	// allocate SI filter struct mem
	pEar->pOMEarStruct->pSIfilter->uiNumOrder = 2;
	pEar->pOMEarStruct->pSIfilter->uiDenOrder = 2;
	pEar->pOMEarStruct->pSIfilter->uiZSize = pEar->pOMEarStruct->pSIfilter->uiDenOrder - 1;
#if SINGLE_PRECISION
	pEar->pOMEarStruct->pSIfilter->prevX = new float [pEar->pOMEarStruct->pSIfilter->uiNumOrder-1];
	pEar->pOMEarStruct->pSIfilter->prevY = new float [pEar->pOMEarStruct->pSIfilter->uiDenOrder-1];
	pEar->pOMEarStruct->pSIfilter->b = new float [pEar->pOMEarStruct->pSIfilter->uiNumOrder];
	pEar->pOMEarStruct->pSIfilter->a = new float [pEar->pOMEarStruct->pSIfilter->uiDenOrder];
	pEar->pOMEarStruct->pSIfilter->zi = new float [pEar->pOMEarStruct->pSIfilter->uiZSize];
	pEar->pOMEarStruct->pSIfilter->zf = new float [pEar->pOMEarStruct->pSIfilter->uiZSize];
#elif DOUBLE_PRECISION
	pEar->pOMEarStruct->pSIfilter->prevX = new double [pEar->pOMEarStruct->pSIfilter->uiNumOrder-1];
	pEar->pOMEarStruct->pSIfilter->prevY = new double [pEar->pOMEarStruct->pSIfilter->uiDenOrder-1];
	pEar->pOMEarStruct->pSIfilter->b = new double [pEar->pOMEarStruct->pSIfilter->uiNumOrder];
	pEar->pOMEarStruct->pSIfilter->a = new double [pEar->pOMEarStruct->pSIfilter->uiDenOrder];
	pEar->pOMEarStruct->pSIfilter->zi = new double [pEar->pOMEarStruct->pSIfilter->uiZSize];
	pEar->pOMEarStruct->pSIfilter->zf = new double [pEar->pOMEarStruct->pSIfilter->uiZSize];
#endif
	pEar->pOMEarStruct->pSIfilter->iGammatonePassStatus = -1;

	// initialise scrolling waveform display variables
	uiSubSamples = 0;
	uiImgSrcX = 0;

	// allocates dynamic mem whose size varies w.r.t. system parameters
	Reinitialise();

	// clear flags for the BM buffer usage / availability
	uiOffset = 0;

	// initialise POSIX threads here
	pthread_mutex_init (&Write2FileMutex[0], NULL);
	pthread_mutex_init (&Write2FileMutex[1], NULL);
	pthread_cond_init (&cvFileWriteWaitonSignal[0], NULL);
	pthread_cond_init (&cvFileWriteWaitonSignal[1], NULL);

	// set flags
	bRTAPrunning = true;
}

RTAPcompute::~RTAPcompute()
{
	// if not already de-allocated by FreeResources() then, proceed to free
	FreeResources();
#if 0
	delete [] pFilterParams->zi;
	delete [] pFilterParams->zf;
	delete [] pFilterParams->b;
	delete [] pFilterParams->a;
	delete [] pCParams;
#endif
	delete [] pEar->pOMEarStruct->pSIfilter->zf;
	delete [] pEar->pOMEarStruct->pSIfilter->zi;
	delete [] pEar->pOMEarStruct->pSIfilter->a;
	delete [] pEar->pOMEarStruct->pSIfilter->b;
	delete [] pEar->pOMEarStruct->pSIfilter->prevY;
	delete [] pEar->pOMEarStruct->pSIfilter->prevX;
	delete [] pEar->pOMEarStruct->pTMfilter->zf;
	delete [] pEar->pOMEarStruct->pTMfilter->zi;
	delete [] pEar->pOMEarStruct->pTMfilter->a;
	delete [] pEar->pOMEarStruct->pTMfilter->b;
	delete [] pEar->pOMEarStruct->pTMfilter->prevY;
	delete [] pEar->pOMEarStruct->pTMfilter->prevX;
	delete [] pEar->pOMEarStruct->pEERfilter->zf;
	delete [] pEar->pOMEarStruct->pEERfilter->zi;
	delete [] pEar->pOMEarStruct->pEERfilter->a;
	delete [] pEar->pOMEarStruct->pEERfilter->b;
	delete [] pEar->pOMEarStruct->pEERfilter->prevY;
	delete [] pEar->pOMEarStruct->pEERfilter->prevX;
	delete pEar->pAN_IHCsynapse;
	delete pEar->pIHC_presynapse;
	delete pEar->pIHC_RP;
	delete pEar->pDRNLstruct;
	delete pEar->pEarFilters;
	delete pEar->pOMEarStruct->pSIfilter;
	delete pEar->pOMEarStruct->pTMfilter;
	delete pEar->pOMEarStruct->pEERfilter;
	delete pEar->pOMEarStruct;
	delete pFilterParams;
	delete pEar;

	delete FileHeader;
//	delete FileData;

//#if 0

	pthread_mutex_destroy (&Write2FileMutex[0]);
	pthread_mutex_destroy (&Write2FileMutex[1]);
	pthread_cond_destroy (&cvFileWriteWaitonSignal[0]);
	pthread_cond_destroy (&cvFileWriteWaitonSignal[1]);

	bRTAPrunning = false;
//#endif
//	pthread_exit ( NULL );
}

void RTAPcompute::FreeResources()
{
	UINT i, j;

	// de-allocate mem
	if (bMemAllocated)
	{
		delete [] pEar->pAN_IHCsynapse->fReuptake;
		delete [] pEar->pAN_IHCsynapse->fReuptakeAndLost;
		delete [] pEar->pAN_IHCsynapse->fProbReprocessed;
		delete [] pEar->pAN_IHCsynapse->fProbEjected;
		delete [] pEar->pAN_IHCsynapse->fProbReplenish;
		delete [] pEar->pAN_IHCsynapse->fM_Pq;
		delete [] pEar->pAN_IHCsynapse->fInitProbReprocess;
		delete [] pEar->pAN_IHCsynapse->fInitProbAvailable;
		delete [] pEar->pAN_IHCsynapse->fInitProbCleft;
		delete [] pEar->pAN_IHCsynapse->fProbAN_rdtldt;
		delete [] pEar->pAN_IHCsynapse->fProbAN_rdt;
		delete [] pEar->pAN_IHCsynapse->fProbAN_xdt;
		delete [] pEar->pAN_IHCsynapse->fProbAN_ldt;
		delete [] pEar->pAN_IHCsynapse->fProbAN_ydt;
		for (i=0; i<pEar->uiMTnumANfTypesEndPt; i++)
		{
			delete [] pEar->fVesicleReleaseRate[i];

			delete [] pEar->pAN_IHCsynapse->fANProbNotFiring[i];
			delete [] pEar->fANprobOutputRate[i];
		}
		delete [] pEar->pAN_IHCsynapse->fANProbNotFiring;

		delete [] pEar->pIHC_presynapse->fmICaINF;
		delete [] pEar->pIHC_presynapse->fExponent;
		delete [] pEar->pIHC_presynapse->fKT0;
		delete [] pEar->pIHC_presynapse->fTauCa;
		delete [] pEar->pIHC_presynapse->fInitial_mICaCurrent;
		delete [] pEar->pIHC_presynapse->fInitial_CaConcentration;

		delete [] pEar->pIHC_RP->fIHCciliaDisplacement;
		delete [] pEar->pIHC_RP->fIHCciliaDispScaled;
		delete [] pEar->pIHC_RP->fInitialV;
		delete [] pEar->pIHC_RP->fGu;

		for(i=0; i<pEar->uiNumBFchannels; i++)
		{
			delete [] pEar->pAN_IHCsynapse->fProbReprocess[i];
			delete [] pEar->pAN_IHCsynapse->fProbAvailable[i];
			delete [] pEar->pAN_IHCsynapse->fProbCleft[i];

			delete [] pEar->pIHC_presynapse->fCaConcentration[i];
			delete [] pEar->pIHC_presynapse->fmICaCurrent[i];

			delete [] pEar->pEarFilters->pLinGammatone[i].b;
			delete [] pEar->pEarFilters->pLinGammatone[i].a;
			delete [] pEar->pEarFilters->pLinGammatone[i].zi;
			delete [] pEar->pEarFilters->pLinGammatone[i].zf;
			delete [] pEar->pEarFilters->pLinGammatone[i].prevX;
			delete [] pEar->pEarFilters->pLinGammatone[i].prevY;
			delete [] pEar->pEarFilters->pLinGammatone[i].prevS;
			delete [] pEar->pEarFilters->pLinGammatone[i].prevG2X;
			delete [] pEar->pEarFilters->pLinGammatone[i].prevG3X;
			delete [] pEar->pEarFilters->pLinGammatone[i].prevG4X;
			delete [] pEar->pEarFilters->pLinGammatone[i].prevG5X;
			delete [] pEar->pEarFilters->pLinGammatone[i].prevG1Y;
			delete [] pEar->pEarFilters->pLinGammatone[i].prevG2Y;
			delete [] pEar->pEarFilters->pLinGammatone[i].prevG3Y;
			delete [] pEar->pEarFilters->pLinGammatone[i].prevG4Y;
			delete [] pEar->pEarFilters->pLinGammatone[i].prevG5Y;
			delete [] pEar->pEarFilters->pNonlinGammatone[i].b;
			delete [] pEar->pEarFilters->pNonlinGammatone[i].a;
			delete [] pEar->pEarFilters->pNonlinGammatone[i].zi;
			delete [] pEar->pEarFilters->pNonlinGammatone[i].zf;
			delete [] pEar->pEarFilters->pNonlinGammatone[i].prevX;
			delete [] pEar->pEarFilters->pNonlinGammatone[i].prevY;
			delete [] pEar->pEarFilters->pNonlinGammatone[i].prevS;
			delete [] pEar->pEarFilters->pNonlinGammatone[i].prevG2X;
			delete [] pEar->pEarFilters->pNonlinGammatone[i].prevG3X;
			delete [] pEar->pEarFilters->pNonlinGammatone[i].prevG4X;
			delete [] pEar->pEarFilters->pNonlinGammatone[i].prevG5X;
			delete [] pEar->pEarFilters->pNonlinGammatone[i].prevG6X;
			delete [] pEar->pEarFilters->pNonlinGammatone[i].prevG7X;
			delete [] pEar->pEarFilters->pNonlinGammatone[i].prevG8X;
			delete [] pEar->pEarFilters->pNonlinGammatone[i].prevG9X;
			delete [] pEar->pEarFilters->pNonlinGammatone[i].prevG10X;
			delete [] pEar->pEarFilters->pNonlinGammatone[i].prevG1Y;
			delete [] pEar->pEarFilters->pNonlinGammatone[i].prevG2Y;
			delete [] pEar->pEarFilters->pNonlinGammatone[i].prevG3Y;
			delete [] pEar->pEarFilters->pNonlinGammatone[i].prevG4Y;
			delete [] pEar->pEarFilters->pNonlinGammatone[i].prevG5Y;
			delete [] pEar->pEarFilters->pNonlinGammatone[i].prevG6Y;
			delete [] pEar->pEarFilters->pNonlinGammatone[i].prevG7Y;
			delete [] pEar->pEarFilters->pNonlinGammatone[i].prevG8Y;
			delete [] pEar->pEarFilters->pNonlinGammatone[i].prevG9Y;
			delete [] pEar->pEarFilters->pNonlinGammatone[i].prevG10Y;
			delete [] pEar->pEarFilters->pIHCciliaFilter[i].b;
			delete [] pEar->pEarFilters->pIHCciliaFilter[i].a;
			delete [] pEar->pEarFilters->pIHCciliaFilter[i].zi;
			delete [] pEar->pEarFilters->pIHCciliaFilter[i].zf;
			delete [] pEar->pEarFilters->pIHCciliaFilter[i].prevX;
			delete [] pEar->pEarFilters->pIHCciliaFilter[i].prevY;

			delete [] pEar->pAN_IHCsynapse->fPrevANprobNotFiring[i];

			delete [] pEar->fIHCrpResponse[i];
			delete [] pEar->fDRNLresponse[i];
			delete [] pEar->fDRNLnonlinOutput[i];
			delete [] pEar->fDRNLlinOutput[i];
			delete [] pEar->fDRNLlinInput[i];
			delete [] pEar->fDRNLnonlinInput[i];

			delete [] pEar->uiRecBuffInc[i];

			for (j=0; j<pEar->uiNumANfibreTypes; j++)
			{
				delete [] pEar->pAN_IHCsynapse->fPrevANprobOutputRate[i][j];
//				delete [] fProcData2Dump[i][j];
			}
			delete [] pEar->pAN_IHCsynapse->fPrevANprobOutputRate[i];
			delete [] pEar->pAN_IHCsynapse->bFirstWindow[i];
//			delete [] fProcData2Dump[i];
		}
		delete [] pEar->pAN_IHCsynapse->fPrevANprobNotFiring;
		delete [] pEar->pAN_IHCsynapse->fPrevANprobOutputRate;
		delete [] pEar->pAN_IHCsynapse->bFirstWindow;
		delete [] pEar->pAN_IHCsynapse->fProbReprocess;
		delete [] pEar->pAN_IHCsynapse->fProbAvailable;
		delete [] pEar->pAN_IHCsynapse->fProbCleft;

		delete [] pEar->pIHC_presynapse->fCaConcentration;
		delete [] pEar->pIHC_presynapse->fmICaCurrent;

		delete [] pEar->pEarFilters->pLinGammatone;
		delete [] pEar->pEarFilters->pNonlinGammatone;
		delete [] pEar->pEarFilters->pIHCciliaFilter;
		
		delete [] pEar->pDRNLstruct->fBFlist;
		delete [] pEar->pDRNLstruct->fLinBW;
		delete [] pEar->pDRNLstruct->fLinCF;
		delete [] pEar->pDRNLstruct->fNonLinBW;
		delete [] pEar->pDRNLstruct->fNonLinCF;

		delete [] pEar->pOMEarStruct->fInpTMdisplacement;
		delete [] pEar->pOMEarStruct->fInpMidEarStapesInertia;
		delete [] pEar->pOMEarStruct->fStapesDisplacement;
		delete [] pEar->fANprobOutputRate;
		delete [] pEar->fVesicleReleaseRate;

		delete [] pEar->uiRecBuffInc;

//		delete [] fProcData2Dump;
#if 0
		for (i=0; i<NUMOFCORES; i++)
		{
			delete [] pEar->fIHCrpResponse[i];
			delete [] pEar->fDRNLresponse[i];
			delete [] pEar->fDRNLnonlinOutput[i];
			delete [] pEar->fDRNLlinOutput[i];
			delete [] pEar->fDRNLlinInput[i];
			delete [] pEar->fDRNLnonlinInput[i];
		}
#endif
		delete [] pEar->fIHCrpResponse;
		delete [] pEar->fDRNLresponse;
		delete [] pEar->fDRNLnonlinOutput;
		delete [] pEar->fDRNLnonlinInput;
		delete [] pEar->fDRNLlinOutput;
		delete [] pEar->fDRNLlinInput;
		delete pEar->bComputationDone;
		delete pEar->uiBFprocStart;
		delete pEar->uiBFprocEnd;
		delete pEar->fYvalues;
		delete pEar->uiStartAudioSampPt;
		delete pEar->fProcData2Sav1;
		delete pEar->fProcData2Sav2;
		delete pEar->fProcData2Sav3;

		delete fPrevXposProcData;
		delete fPrevYposProcData;
		delete fPeakScore;
		delete fMeanPeakScore;
		delete fMaxDisp;
		delete fMinDisp;
		delete uiYstart;
		delete uiYscale;
		delete uiImgBuffWriteTrack;

		bMemAllocated = false;
		uiRTAPsetStatus = MemFreed;
	}
}

void RTAPcompute::Reinitialise ()
{
#if SINGLE_PRECISION
	float fExponent;
	float fBF;
#elif DOUBLE_PRECISION
	double fExponent;
	double fBf;
#endif
	int ret;
	UINT i, j;

	switch (iRTAPpriority)
	{
		case REAL_TIME: SetPriorityClass (GetCurrentProcess(), REALTIME_PRIORITY_CLASS); break;
		case HIGH: SetPriorityClass (GetCurrentProcess(), HIGH_PRIORITY_CLASS); break;
		case ABOVE_NORMAL: SetPriorityClass (GetCurrentProcess(), ABOVE_NORMAL_PRIORITY_CLASS); break;
		case NORMAL: SetPriorityClass (GetCurrentProcess(), NORMAL_PRIORITY_CLASS); break;
	}

	iProcessPriority = GetPriorityClass(GetCurrentProcess());

	switch (iProcessPriority)
	{
		case 0x00008000	: strcpy_s (cProcessPriority, "ABOVE_NORMAL_PRIORITY_CLASS "); break;
		case 0x00004000 : strcpy_s (cProcessPriority, "BELOW_NORMAL_PRIORITY_CLASS "); break;
		case 0x00000080 : strcpy_s (cProcessPriority, "HIGH_PRIORITY_CLASS "); break;
		case 0x00000040 : strcpy_s (cProcessPriority, "IDLE_PRIORITY_CLASS "); break;
		case 0x00000020 : strcpy_s (cProcessPriority, "NORMAL_PRIORITY_CLASS "); break;
		case 0x00100000 : strcpy_s (cProcessPriority, "PROCESS_MODE_BACKGROUND_BEGIN "); break;
		case 0x00200000 : strcpy_s (cProcessPriority, "PROCESS_MODE_BACKGROUND_END "); break;
		case 0x00000100 : strcpy_s (cProcessPriority, "REALTIME_PRIORITY_CLASS "); break;
		default: strcpy_s (cProcessPriority, "UNKNOWN PRIORITY CLASS ");
	}

	switch (cAudioSource)
	{
		case AUDIO_SYSTEM : strcpy_s (cAudioSourceName, "System audio (Mic-in or Speaker loopback)"); break;
		case AUDIO_SINE: strcpy_s (cAudioSourceName, "Sine tone"); break;
		case AUDIO_SQUARE: strcpy_s (cAudioSourceName, "Square pulse tone"); break;
		case AUDIO_SAWTOOTH: strcpy_s (cAudioSourceName, "Sawtooth tone"); break;
		default: strcpy_s (cAudioSourceName, "Invalid audio source");
	}

	// update the strings holding algorithm function names and their associated parameters
	switch (iFunctionToRun)
	{
		case DRNL_ST:
			strcpy_s (cFunctionName, "DRNL (ST) ");
			strcpy_s (cANfibreSelect, "N/A");
			uiFunction2Rec = DRNL_ST;
			break;
		case DRNL2IHCRP_ST:
			strcpy_s (cFunctionName, "DRNL-to-IHC receptor potential (ST) ");
			strcpy_s (cANfibreSelect, "N/A");
			uiFunction2Rec = DRNL2IHCRP_ST;
			break;
		case DRNL2IHCpresynapse_ST:
			strcpy_s (cFunctionName, "DRNL-to-IHC presynapse (ST) ");
			if (uiNumofANfibreTypes == 1)
			{
				if (iANfibreType == 1) strcpy_s (cANfibreSelect, "LSR");
				else if (iANfibreType == 2) strcpy_s (cANfibreSelect, "HSR");
			}
			else if (uiNumofANfibreTypes == 2)
				strcpy_s (cANfibreSelect, "LSR & HSR");
			else
				strcpy_s (cANfibreSelect, "N/A");
			uiFunction2Rec = DRNL2IHCpresynapse_ST;
			break;
		case DRNL2ANsynapse_ST:
			strcpy_s (cFunctionName, "DRNL-to-AN synapse (ST) ");
			if (uiNumofANfibreTypes == 1)
			{
				if (iANfibreType == 1) strcpy_s (cANfibreSelect, "LSR");
				else if (iANfibreType == 2) strcpy_s (cANfibreSelect, "HSR");
			}
			else if (uiNumofANfibreTypes == 2)
				strcpy_s (cANfibreSelect, "LSR & HSR");
			else
				strcpy_s (cANfibreSelect, "N/A");
			uiFunction2Rec = DRNL2ANsynapse_ST;
			break;
		case DRNL_ST_OPT:
			strcpy_s (cFunctionName, "DRNL (opt ST) ");
			strcpy_s (cANfibreSelect, "N/A");
			uiFunction2Rec = DRNL_ST_OPT;
			break;
		case DRNL2IHCRP_ST_OPT:
			strcpy_s (cFunctionName, "DRNL-to-IHC receptor potential (opt ST) ");
			strcpy_s (cANfibreSelect, "N/A");
			uiFunction2Rec = DRNL2IHCRP_ST_OPT;
			break;
		case DRNL2IHCpresynapse_ST_OPT:
			strcpy_s (cFunctionName, "DRNL-to-IHC presynapse (opt ST) ");
			if (uiNumofANfibreTypes == 1)
			{
				if (iANfibreType == 1) strcpy_s (cANfibreSelect, "LSR");
				else if (iANfibreType == 2) strcpy_s (cANfibreSelect, "HSR");
			}
			else if (uiNumofANfibreTypes == 2)
				strcpy_s (cANfibreSelect, "LSR & HSR");
			else
				strcpy_s (cANfibreSelect, "N/A");
			uiFunction2Rec = DRNL2IHCpresynapse_ST_OPT;
			break;
		case DRNL2ANsynapse_ST_OPT:
			strcpy_s (cFunctionName, "DRNL-to-IHC AN synapse (opt ST) ");
			if (uiNumofANfibreTypes == 1)
			{
				if (iANfibreType == 1) strcpy_s (cANfibreSelect, "LSR");
				else if (iANfibreType == 2) strcpy_s (cANfibreSelect, "HSR");
			}
			else if (uiNumofANfibreTypes == 2)
				strcpy_s (cANfibreSelect, "LSR & HSR");
			else
				strcpy_s (cANfibreSelect, "N/A");
			uiFunction2Rec = DRNL2ANsynapse_ST_OPT;
			break;
		case DRNL_MT:
			strcpy_s (cFunctionName, "DRNL (MT) ");
			strcpy_s (cANfibreSelect, "N/A");
			uiFunction2Rec = DRNL_MT;
			break;
		case DRNL2IHCRP_MT:
			strcpy_s (cFunctionName, "DRNL-to-IHC receptor potential (MT) ");
			strcpy_s (cANfibreSelect, "N/A");
			uiFunction2Rec = DRNL2IHCRP_MT;
			break;
		case DRNL2IHCpresynapse_MT:
			strcpy_s (cFunctionName, "DRNL-to-IHC presynapse (MT) ");
			if (uiNumofANfibreTypes == 1)
			{
				if (iANfibreType == 1) strcpy_s (cANfibreSelect, "LSR");
				else if (iANfibreType == 2) strcpy_s (cANfibreSelect, "HSR");
			}
			else if (uiNumofANfibreTypes == 2)
				strcpy_s (cANfibreSelect, "LSR & HSR");
			else
				strcpy_s (cANfibreSelect, "N/A");
			uiFunction2Rec = DRNL2IHCpresynapse_MT;
			break;
		case DRNL2ANsynapse_MT:
			strcpy_s (cFunctionName, "DRNL-to-AN synapse (MT) ");
			if (uiNumofANfibreTypes == 1)
			{
				if (iANfibreType == 1) strcpy_s (cANfibreSelect, "LSR");
				else if (iANfibreType == 2) strcpy_s (cANfibreSelect, "HSR");
			}
			else if (uiNumofANfibreTypes == 2)
				strcpy_s (cANfibreSelect, "LSR & HSR");
			else
				strcpy_s (cANfibreSelect, "N/A");
			uiFunction2Rec = DRNL2ANsynapse_MT;
			break;
	}

	bMathOpt? strcpy_s (cMathOpt, "YES") : strcpy_s (cMathOpt, "NO");
	bWindowBoundaryDisp? strcpy_s (cWindowBoundaryDisp, "YES") : strcpy_s (cWindowBoundaryDisp, "NO");
	bPeakAmpDisp? strcpy_s (cPeakAmpDisp, "YES") : strcpy_s (cPeakAmpDisp, "NO");

	// allocate mem for DRNL() if only it is not allocated yet
	if (!bMemAllocated)
	{
		siSamp2Skip = (44100 / fSamplingRate) - 1;										// compute num audio samp to skip
		pEar->fDt = 1/fSamplingRate;													// compute sampling period
		pEar->iBFcounter = -1;
		pEar->iFileWriterCounter = -1;
		pEar->uiAudioInputWinSize = (int)(2560 / (44100 / fSamplingRate));				// compute packet size based on fs: 44.1kHz & 2560 samples/packet
		pEar->uiAudioInRecBuffTrack = 0;												// initialise audio in rec buff track to 0
		pEar->uiNumSampToRec = pEar->uiAudioInputWinSize * NUMWIN2REC;
		uiWindowSize = pEar->uiAudioInputWinSize;										// update global value for sine tone generation
		uiOffsetbySamplingRate = pEar->uiAudioInputWinSize;
		pEar->uiNumBFchannels = uiNumofBFchannels;
		dDisplayNumBFchannels = uiNumofBFchannels;										// RTAP info page update: num of BF channels
		pEar->uiMTnumBFchannelsMidPt = pEar->uiNumBFchannels >> 1;
		if ((iFunctionToRun == DRNL_ST) || (iFunctionToRun == DRNL2IHCRP_ST) ||
			(iFunctionToRun == DRNL_ST_OPT) || (iFunctionToRun == DRNL2IHCRP_ST_OPT) ||
			(iFunctionToRun == DRNL_MT) || (iFunctionToRun == DRNL2IHCRP_MT))
			pEar->uiNumANfibreTypes = 1;
		else
			pEar->uiNumANfibreTypes = uiNumofANfibreTypes;
		dDisplayNumANfibreTypes = pEar->uiNumANfibreTypes;
		pEar->uiNumANchannels = pEar->uiNumANfibreTypes * pEar->uiNumBFchannels;		// num of AN channels
		dDisplayNumANchannels = pEar->uiNumANchannels;									// RTAP info page update: num of AN channels
		pEar->uiMTnumANfTypesEndPt = pEar->uiNumANfibreTypes * NUMOFCORES;
		pEar->uiMTnumANfTypesMidPt = pEar->uiMTnumANfTypesEndPt >> 1;
		pEar->uiStartAudioSampPt = new UINT [NUMOFCORES];
		fMaxPeak[0] = 0.f;
		fMaxPeak[1] = 0.f;
		fMaxPeak[2] = 0.f;
		uiMeanPeakBF[0] = 0;
		uiMeanPeakBF[1] = 0;
		uiMeanPeakBF[2] = 0;
#if SINGLE_PRECISION
		fPeakScore = new float [pEar->uiNumBFchannels];				// score for maintaining peaks in BF signals
		fMeanPeakScore = new float [pEar->uiNumBFchannels];
		fMaxDisp = new float [pEar->uiNumBFchannels];
		fMinDisp = new float [pEar->uiNumBFchannels];
#elif DOUBLE_PRECISION
		fPeakScore = new double [pEar->uiNumBFchannels];
		fMeanPeakScore = new double [pEar->uiNumBFchannels];
		fMaxDisp = new double [pEar->uiNumBFchannels];
		fMinDisp = new double [pEar->uiNumBFchannels];
#endif
		uiYstart = new UINT [pEar->uiNumBFchannels];
		uiYscale = new UINT [pEar->uiNumBFchannels];
		uiImgBuffWriteTrack = new UINT [pEar->uiNumBFchannels];

		// allocate mem for samp values to determine max & min pixel values
#if SINGLE_PRECISION
		pEar->fYvalues = new float [3];
#elif DOUBLE_PRECISION
		pEar->fYvalues = new double [3];
#endif

		pEar->uiBFprocStart = new UINT [NUMOFCORES];
		pEar->uiBFprocEnd = new UINT [NUMOFCORES];
//		pEar->iBFthread = new SINT [NUMOFCORES];
//		pEar->iFileWriterthread = new SINT [NUMOFCORES];
//		pEar->bThreadRunning = new bool [NUMOFCORES];
#if 0
		pEar->fDRNLlinInput = new float* [NUMOFCORES];
		pEar->fDRNLnonlinInput = new float* [NUMOFCORES];
		pEar->fDRNLlinOutput = new float* [NUMOFCORES];
		pEar->fDRNLnonlinOutput = new float* [NUMOFCORES];
		pEar->fDRNLresponse = new float* [NUMOFCORES];
		pEar->fIHCrpResponse = new float* [NUMOFCORES];
#endif
		pEar->bComputationDone = new bool [NUMOFCORES];

		for (i=0; i<NUMOFCORES; i++)
		{
#if 0
			pEar->fDRNLlinInput[i] = new float [pEar->uiAudioInputWinSize];
			pEar->fDRNLnonlinInput[i] = new float [pEar->uiAudioInputWinSize];
			pEar->fDRNLlinOutput[i] = new float [pEar->uiAudioInputWinSize];
			pEar->fDRNLnonlinOutput[i] = new float [pEar->uiAudioInputWinSize];
			pEar->fDRNLresponse[i] = new float [pEar->uiAudioInputWinSize];
			pEar->fIHCrpResponse[i] = new float [pEar->uiAudioInputWinSize];
#endif
			cFileWriterThreaduiStatus[i] = IDLE;
			cProcThreaduiStatus[i] = IDLE;
			pEar->bComputationDone[i] = false;
			pEar->uiStartAudioSampPt[i] = 0;						// initialise the audio sample startpoints for DRNL
		}
		// allocate mem for OME
#if SINGLE_PRECISION
		pEar->pOMEarStruct->fInpTMdisplacement = new float [pEar->uiAudioInputWinSize];
		pEar->pOMEarStruct->fInpMidEarStapesInertia = new float [pEar->uiAudioInputWinSize];
		pEar->pOMEarStruct->fStapesDisplacement = new float [pEar->uiAudioInputWinSize];
		pEar->pDRNLstruct->fBFlist = new float [pEar->uiNumBFchannels];
		pEar->fDRNLlinInput = new float* [pEar->uiNumBFchannels];
		pEar->fDRNLnonlinInput = new float* [pEar->uiNumBFchannels];
		pEar->fDRNLlinOutput = new float* [pEar->uiNumBFchannels];
		pEar->fDRNLnonlinOutput = new float* [pEar->uiNumBFchannels];
		pEar->fDRNLresponse = new float* [pEar->uiNumBFchannels];
		pEar->fIHCrpResponse = new float* [pEar->uiNumBFchannels];
		pEar->pDRNLstruct->fLinBW = new float [pEar->uiNumBFchannels];
		pEar->pDRNLstruct->fLinCF = new float [pEar->uiNumBFchannels];
		pEar->pDRNLstruct->fNonLinBW = new float [pEar->uiNumBFchannels];
		pEar->pDRNLstruct->fNonLinCF = new float [pEar->uiNumBFchannels];
#elif DOUBLE_PRECISION
		pEar->pOMEarStruct->fInpTMdisplacement = new double [pEar->uiAudioInputWinSize];
		pEar->pOMEarStruct->fInpMidEarStapesInertia = new double [pEar->uiAudioInputWinSize];
		pEar->pOMEarStruct->fStapesDisplacement = new double [pEar->uiAudioInputWinSize];
		pEar->pDRNLstruct->fBFlist = new double [pEar->uiNumBFchannels];
		pEar->fDRNLlinInput = new double* [pEar->uiNumBFchannels];
		pEar->fDRNLnonlinInput = new double* [pEar->uiNumBFchannels];
		pEar->fDRNLlinOutput = new double* [pEar->uiNumBFchannels];
		pEar->fDRNLnonlinOutput = new double* [pEar->uiNumBFchannels];
		pEar->fDRNLresponse = new double* [pEar->uiNumBFchannels];
		pEar->fIHCrpResponse = new double* [pEar->uiNumBFchannels];
		pEar->pDRNLstruct->fLinBW = new double [pEar->uiNumBFchannels];
		pEar->pDRNLstruct->fLinCF = new double [pEar->uiNumBFchannels];
		pEar->pDRNLstruct->fNonLinBW = new double [pEar->uiNumBFchannels];
		pEar->pDRNLstruct->fNonLinCF = new double [pEar->uiNumBFchannels];
#endif
		RTAP_logspace (pEar->pDRNLstruct->fBFlist, fMinBFlist, fMaxBFlist, pEar->uiNumBFchannels);

		uiMinBF = (unsigned int) fMinBFlist;
		uiMaxBF = (unsigned int) fMaxBFlist;
		
		pEar->pEarFilters->pLinGammatone = new strFilterParams [pEar->uiNumBFchannels];
		pEar->pEarFilters->pNonlinGammatone = new strFilterParams [pEar->uiNumBFchannels];
		pEar->pEarFilters->pIHCciliaFilter = new strFilterParams [pEar->uiNumBFchannels];

#if SINGLE_PRECISION
		pEar->pIHC_presynapse->fmICaCurrent = new float* [pEar->uiNumBFchannels];
		pEar->pIHC_presynapse->fCaConcentration = new float* [pEar->uiNumBFchannels];

		pEar->pAN_IHCsynapse->fProbCleft = new float* [pEar->uiNumBFchannels];
		pEar->pAN_IHCsynapse->fProbAvailable = new float* [pEar->uiNumBFchannels];
		pEar->pAN_IHCsynapse->fProbReprocess = new float* [pEar->uiNumBFchannels];
		pEar->pAN_IHCsynapse->fPrevANprobNotFiring = new float* [pEar->uiNumBFchannels];
		pEar->pAN_IHCsynapse->fPrevANprobOutputRate = new float** [pEar->uiNumBFchannels];
		pEar->pAN_IHCsynapse->bFirstWindow = new bool* [pEar->uiNumBFchannels];

		// mem for buffer to write processed data to text file
		pEar->fProcData2Sav1 = new float [pEar->uiNumBFchannels];
		pEar->fProcData2Sav2 = new float [pEar->uiNumBFchannels];
		pEar->fProcData2Sav3 = new float [pEar->uiNumBFchannels];
#elif DOUBLE_PRECISION
		pEar->pIHC_presynapse->fmICaCurrent = new double* [pEar->uiNumBFchannels];
		pEar->pIHC_presynapse->fCaConcentration = new double* [pEar->uiNumBFchannels];

		pEar->pAN_IHCsynapse->fProbCleft = new double* [pEar->uiNumBFchannels];
		pEar->pAN_IHCsynapse->fProbAvailable = new double* [pEar->uiNumBFchannels];
		pEar->pAN_IHCsynapse->fProbReprocess = new double* [pEar->uiNumBFchannels];
		pEar->pAN_IHCsynapse->fPrevANprobNotFiring = new double* [pEar->uiNumBFchannels];
		pEar->pAN_IHCsynapse->fPrevANprobOutputRate = new double** [pEar->uiNumBFchannels];
		pEar->pAN_IHCsynapse->bFirstWindow = new bool* [pEar->uiNumBFchannels];

		// mem for buffer to write processed data to text file
		pEar->fProcData2Sav1 = new double [pEar->uiNumBFchannels];
		pEar->fProcData2Sav2 = new double [pEar->uiNumBFchannels];
		pEar->fProcData2Sav3 = new double [pEar->uiNumBFchannels];
#endif
		pEar->uiRecBuffInc = new UINT* [pEar->uiNumBFchannels];

		// allocate buffer for line plotting
		fPrevXposProcData = new float [pEar->uiNumBFchannels];
		fPrevYposProcData = new float [pEar->uiNumBFchannels];

		// allocate mem for filter structs for every BF channel in DRNL and IHC_RP
		for (i=0; i<pEar->uiNumBFchannels; i++)
		{
			fBF = pEar->pDRNLstruct->fBFlist[i];
			switch (iFunctionToRun)
			{
				case DRNL_ST:
				case DRNL_ST_OPT:
					// TO_DO
					fMaxDisp[i] = 0.0005;
					fMinDisp[i] = -0.0005;
					break;
				case DRNL2IHCRP_ST:
				case DRNL2IHCRP_ST_OPT:
				case DRNL2IHCpresynapse_ST:
				case DRNL2IHCpresynapse_ST_OPT:
				case DRNL2ANsynapse_ST:
				case DRNL2ANsynapse_ST_OPT:
					fMaxDisp[i] = 3e-24*pow(fBF, 6.f) - 4e-20*pow(fBF, 5.f) + 2e-16*pow(fBF, 4.f) - 1e-13*pow(fBF,3.f) -2e-9*pow(fBF, 2.f) + 3e-6*fBF - 0.0598;
					fMinDisp[i] = -1e-24*pow(fBF, 6.f) + 2e-20*pow(fBF, 5.f) - 4e-17*pow(fBF, 4.f) - 4e-13*pow(fBF, 3.f) + 2e-9*pow(fBF, 2.f) -3e-6*fBF - 0.0614;
					break;
			}
			// pre-allocate mem for different stages in AP model
#if SINGLE_PRECISION
			pEar->fDRNLlinInput[i] = new float [pEar->uiAudioInputWinSize];
			pEar->fDRNLnonlinInput[i] = new float [pEar->uiAudioInputWinSize];
			pEar->fDRNLlinOutput[i] = new float [pEar->uiAudioInputWinSize];
			pEar->fDRNLnonlinOutput[i] = new float [pEar->uiAudioInputWinSize];
			pEar->fDRNLresponse[i] = new float [pEar->uiAudioInputWinSize];
			pEar->fIHCrpResponse[i] = new float [pEar->uiAudioInputWinSize];
#elif DOUBLE_PRECISION
			pEar->fDRNLlinInput[i] = new double [pEar->uiAudioInputWinSize];
			pEar->fDRNLnonlinInput[i] = new double [pEar->uiAudioInputWinSize];
			pEar->fDRNLlinOutput[i] = new double [pEar->uiAudioInputWinSize];
			pEar->fDRNLnonlinOutput[i] = new double [pEar->uiAudioInputWinSize];
			pEar->fDRNLresponse[i] = new double [pEar->uiAudioInputWinSize];
			pEar->fIHCrpResponse[i] = new double [pEar->uiAudioInputWinSize];
#endif

			pEar->pEarFilters->pLinGammatone[i].uiDenOrder = 3;
			pEar->pEarFilters->pLinGammatone[i].uiNumOrder = 2;
			pEar->pEarFilters->pLinGammatone[i].uiZSize = pEar->pEarFilters->pLinGammatone[i].uiDenOrder - 1;
#if SINGLE_PRECISION
			pEar->pEarFilters->pLinGammatone[i].b = new float [pEar->pEarFilters->pLinGammatone[i].uiNumOrder];
			pEar->pEarFilters->pLinGammatone[i].a = new float [pEar->pEarFilters->pLinGammatone[i].uiDenOrder];
			pEar->pEarFilters->pLinGammatone[i].zi = new float [pEar->pEarFilters->pLinGammatone[i].uiZSize];
			pEar->pEarFilters->pLinGammatone[i].zf = new float [pEar->pEarFilters->pLinGammatone[i].uiZSize];
			// required for filter state saves and loads under real-time runtime
			pEar->pEarFilters->pLinGammatone[i].prevX = new float [pEar->pEarFilters->pLinGammatone[i].uiNumOrder - 1];
			pEar->pEarFilters->pLinGammatone[i].prevY = new float [pEar->pEarFilters->pLinGammatone[i].uiDenOrder - 1];
			pEar->pEarFilters->pLinGammatone[i].prevS = new float [pEar->pEarFilters->pLinGammatone[i].uiNumOrder - 1];
			pEar->pEarFilters->pLinGammatone[i].prevG2X = new float [pEar->pEarFilters->pLinGammatone[i].uiNumOrder - 1];
			pEar->pEarFilters->pLinGammatone[i].prevG3X = new float [pEar->pEarFilters->pLinGammatone[i].uiNumOrder - 1];
			pEar->pEarFilters->pLinGammatone[i].prevG4X = new float [pEar->pEarFilters->pLinGammatone[i].uiNumOrder - 1];
			pEar->pEarFilters->pLinGammatone[i].prevG5X = new float [pEar->pEarFilters->pLinGammatone[i].uiNumOrder - 1];
			pEar->pEarFilters->pLinGammatone[i].prevG1Y = new float [pEar->pEarFilters->pLinGammatone[i].uiDenOrder - 1];
			pEar->pEarFilters->pLinGammatone[i].prevG2Y = new float [pEar->pEarFilters->pLinGammatone[i].uiDenOrder - 1];
			pEar->pEarFilters->pLinGammatone[i].prevG3Y = new float [pEar->pEarFilters->pLinGammatone[i].uiDenOrder - 1];
			pEar->pEarFilters->pLinGammatone[i].prevG4Y = new float [pEar->pEarFilters->pLinGammatone[i].uiDenOrder - 1];
			pEar->pEarFilters->pLinGammatone[i].prevG5Y = new float [pEar->pEarFilters->pLinGammatone[i].uiDenOrder - 1];
#elif DOUBLE_PRECISION
			pEar->pEarFilters->pLinGammatone[i].b = new double [pEar->pEarFilters->pLinGammatone[i].uiNumOrder];
			pEar->pEarFilters->pLinGammatone[i].a = new double [pEar->pEarFilters->pLinGammatone[i].uiDenOrder];
			pEar->pEarFilters->pLinGammatone[i].zi = new double [pEar->pEarFilters->pLinGammatone[i].uiZSize];
			pEar->pEarFilters->pLinGammatone[i].zf = new double [pEar->pEarFilters->pLinGammatone[i].uiZSize];
			// required for filter state saves and loads under real-time runtime
			pEar->pEarFilters->pLinGammatone[i].prevX = new double [pEar->pEarFilters->pLinGammatone[i].uiNumOrder - 1];
			pEar->pEarFilters->pLinGammatone[i].prevY = new double [pEar->pEarFilters->pLinGammatone[i].uiDenOrder - 1];
			pEar->pEarFilters->pLinGammatone[i].prevS = new double [pEar->pEarFilters->pLinGammatone[i].uiNumOrder - 1];
			pEar->pEarFilters->pLinGammatone[i].prevG2X = new double [pEar->pEarFilters->pLinGammatone[i].uiNumOrder - 1];
			pEar->pEarFilters->pLinGammatone[i].prevG3X = new double [pEar->pEarFilters->pLinGammatone[i].uiNumOrder - 1];
			pEar->pEarFilters->pLinGammatone[i].prevG1Y = new double [pEar->pEarFilters->pLinGammatone[i].uiDenOrder - 1];
			pEar->pEarFilters->pLinGammatone[i].prevG2Y = new double [pEar->pEarFilters->pLinGammatone[i].uiDenOrder - 1];
			pEar->pEarFilters->pLinGammatone[i].prevG3Y = new double [pEar->pEarFilters->pLinGammatone[i].uiDenOrder - 1];
#endif
			pEar->pEarFilters->pLinGammatone[i].iGammatonePassStatus = -1;
			pEar->pEarFilters->pLinGammatone[i].bFirstWindow = true;
			pEar->pEarFilters->pLinGammatone[i].bLoopLock = false;
			pEar->pEarFilters->pLinGammatone[i].uiWinContinuity = 0;
			// delay variables have been omitted for optimised functions but keep initialisation if need to revert to original settings
			pEar->pEarFilters->pLinGammatone[i].zi[0] = 0;
			pEar->pEarFilters->pLinGammatone[i].zi[1] = 0;
			pEar->pEarFilters->pLinGammatone[i].bBoundaryValue = false;

			pEar->pEarFilters->pNonlinGammatone[i].uiDenOrder = 3;
			pEar->pEarFilters->pNonlinGammatone[i].uiNumOrder= 2;
			pEar->pEarFilters->pNonlinGammatone[i].uiZSize = pEar->pEarFilters->pNonlinGammatone[i].uiDenOrder - 1;
#if SINGLE_PRECISION
			pEar->pEarFilters->pNonlinGammatone[i].b = new float [pEar->pEarFilters->pNonlinGammatone[i].uiNumOrder];
			pEar->pEarFilters->pNonlinGammatone[i].a = new float [pEar->pEarFilters->pNonlinGammatone[i].uiDenOrder];
			pEar->pEarFilters->pNonlinGammatone[i].zi = new float [pEar->pEarFilters->pNonlinGammatone[i].uiZSize];
			pEar->pEarFilters->pNonlinGammatone[i].zf = new float [pEar->pEarFilters->pNonlinGammatone[i].uiZSize];
			// filter save states for real-time runtime
			pEar->pEarFilters->pNonlinGammatone[i].prevX = new float [pEar->pEarFilters->pNonlinGammatone[i].uiNumOrder - 1];
			pEar->pEarFilters->pNonlinGammatone[i].prevY = new float [pEar->pEarFilters->pNonlinGammatone[i].uiDenOrder - 1];
			pEar->pEarFilters->pNonlinGammatone[i].prevS = new float [pEar->pEarFilters->pLinGammatone[i].uiNumOrder - 1];
			pEar->pEarFilters->pNonlinGammatone[i].prevG2X = new float [pEar->pEarFilters->pNonlinGammatone[i].uiNumOrder - 1];
			pEar->pEarFilters->pNonlinGammatone[i].prevG3X = new float [pEar->pEarFilters->pNonlinGammatone[i].uiNumOrder - 1];
			pEar->pEarFilters->pNonlinGammatone[i].prevG4X = new float [pEar->pEarFilters->pNonlinGammatone[i].uiDenOrder - 1];
			pEar->pEarFilters->pNonlinGammatone[i].prevG5X = new float [pEar->pEarFilters->pNonlinGammatone[i].uiNumOrder - 1];
			pEar->pEarFilters->pNonlinGammatone[i].prevG6X = new float [pEar->pEarFilters->pNonlinGammatone[i].uiNumOrder - 1];
			pEar->pEarFilters->pNonlinGammatone[i].prevG7X = new float [pEar->pEarFilters->pNonlinGammatone[i].uiNumOrder - 1];
			pEar->pEarFilters->pNonlinGammatone[i].prevG8X = new float [pEar->pEarFilters->pNonlinGammatone[i].uiNumOrder - 1];
			pEar->pEarFilters->pNonlinGammatone[i].prevG9X = new float [pEar->pEarFilters->pNonlinGammatone[i].uiNumOrder - 1];
			pEar->pEarFilters->pNonlinGammatone[i].prevG10X = new float [pEar->pEarFilters->pNonlinGammatone[i].uiNumOrder - 1];
			pEar->pEarFilters->pNonlinGammatone[i].prevG1Y = new float [pEar->pEarFilters->pNonlinGammatone[i].uiDenOrder - 1];
			pEar->pEarFilters->pNonlinGammatone[i].prevG2Y = new float [pEar->pEarFilters->pNonlinGammatone[i].uiDenOrder - 1];
			pEar->pEarFilters->pNonlinGammatone[i].prevG3Y = new float [pEar->pEarFilters->pNonlinGammatone[i].uiDenOrder - 1];
			pEar->pEarFilters->pNonlinGammatone[i].prevG4Y = new float [pEar->pEarFilters->pNonlinGammatone[i].uiDenOrder - 1];
			pEar->pEarFilters->pNonlinGammatone[i].prevG5Y = new float [pEar->pEarFilters->pNonlinGammatone[i].uiDenOrder - 1];
			pEar->pEarFilters->pNonlinGammatone[i].prevG6Y = new float [pEar->pEarFilters->pNonlinGammatone[i].uiDenOrder - 1];
			pEar->pEarFilters->pNonlinGammatone[i].prevG7Y = new float [pEar->pEarFilters->pNonlinGammatone[i].uiDenOrder - 1];
			pEar->pEarFilters->pNonlinGammatone[i].prevG8Y = new float [pEar->pEarFilters->pNonlinGammatone[i].uiDenOrder - 1];
			pEar->pEarFilters->pNonlinGammatone[i].prevG9Y = new float [pEar->pEarFilters->pNonlinGammatone[i].uiDenOrder - 1];
			pEar->pEarFilters->pNonlinGammatone[i].prevG10Y = new float [pEar->pEarFilters->pNonlinGammatone[i].uiDenOrder - 1];
#elif DOUBLE_PRECISION
			pEar->pEarFilters->pNonlinGammatone[i].b = new double [pEar->pEarFilters->pNonlinGammatone[i].uiNumOrder];
			pEar->pEarFilters->pNonlinGammatone[i].a = new double [pEar->pEarFilters->pNonlinGammatone[i].uiDenOrder];
			pEar->pEarFilters->pNonlinGammatone[i].zi = new double [pEar->pEarFilters->pNonlinGammatone[i].uiZSize];
			pEar->pEarFilters->pNonlinGammatone[i].zf = new double [pEar->pEarFilters->pNonlinGammatone[i].uiZSize];
			// filter save states for real-time runtime
			pEar->pEarFilters->pNonlinGammatone[i].prevX = new double [pEar->pEarFilters->pNonlinGammatone[i].uiNumOrder - 1];
			pEar->pEarFilters->pNonlinGammatone[i].prevY = new double [pEar->pEarFilters->pNonlinGammatone[i].uiDenOrder - 1];
			pEar->pEarFilters->pNonlinGammatone[i].prevS = new double [pEar->pEarFilters->pLinGammatone[i].uiNumOrder - 1];
			pEar->pEarFilters->pNonlinGammatone[i].prevG2X = new double [pEar->pEarFilters->pNonlinGammatone[i].uiNumOrder - 1];
			pEar->pEarFilters->pNonlinGammatone[i].prevG3X = new double [pEar->pEarFilters->pNonlinGammatone[i].uiNumOrder - 1];
			pEar->pEarFilters->pNonlinGammatone[i].prevG4X = new double [pEar->pEarFilters->pNonlinGammatone[i].uiDenOrder - 1];
			pEar->pEarFilters->pNonlinGammatone[i].prevG5X = new double [pEar->pEarFilters->pNonlinGammatone[i].uiNumOrder - 1];
			pEar->pEarFilters->pNonlinGammatone[i].prevG6X = new double [pEar->pEarFilters->pNonlinGammatone[i].uiNumOrder - 1];
			pEar->pEarFilters->pNonlinGammatone[i].prevG7X = new double [pEar->pEarFilters->pNonlinGammatone[i].uiNumOrder - 1];
			pEar->pEarFilters->pNonlinGammatone[i].prevG8X = new double [pEar->pEarFilters->pNonlinGammatone[i].uiNumOrder - 1];
			pEar->pEarFilters->pNonlinGammatone[i].prevG9X = new double [pEar->pEarFilters->pNonlinGammatone[i].uiNumOrder - 1];
			pEar->pEarFilters->pNonlinGammatone[i].prevG10X = new double [pEar->pEarFilters->pNonlinGammatone[i].uiNumOrder - 1];
			pEar->pEarFilters->pNonlinGammatone[i].prevG1Y = new double [pEar->pEarFilters->pNonlinGammatone[i].uiDenOrder - 1];
			pEar->pEarFilters->pNonlinGammatone[i].prevG2Y = new double [pEar->pEarFilters->pNonlinGammatone[i].uiDenOrder - 1];
			pEar->pEarFilters->pNonlinGammatone[i].prevG3Y = new double [pEar->pEarFilters->pNonlinGammatone[i].uiDenOrder - 1];
			pEar->pEarFilters->pNonlinGammatone[i].prevG4Y = new double [pEar->pEarFilters->pNonlinGammatone[i].uiDenOrder - 1];
			pEar->pEarFilters->pNonlinGammatone[i].prevG5Y = new double [pEar->pEarFilters->pNonlinGammatone[i].uiDenOrder - 1];
			pEar->pEarFilters->pNonlinGammatone[i].prevG6Y = new double [pEar->pEarFilters->pNonlinGammatone[i].uiDenOrder - 1];
			pEar->pEarFilters->pNonlinGammatone[i].prevG7Y = new double [pEar->pEarFilters->pNonlinGammatone[i].uiDenOrder - 1];
			pEar->pEarFilters->pNonlinGammatone[i].prevG8Y = new double [pEar->pEarFilters->pNonlinGammatone[i].uiDenOrder - 1];
			pEar->pEarFilters->pNonlinGammatone[i].prevG9Y = new double [pEar->pEarFilters->pNonlinGammatone[i].uiDenOrder - 1];
			pEar->pEarFilters->pNonlinGammatone[i].prevG10Y = new double [pEar->pEarFilters->pNonlinGammatone[i].uiDenOrder - 1];
#endif
			pEar->pEarFilters->pNonlinGammatone[i].iGammatonePassStatus = -1;
			pEar->pEarFilters->pNonlinGammatone[i].uiWinContinuity = 0;
			pEar->pEarFilters->pNonlinGammatone[i].bFirstWindow = true;
			pEar->pEarFilters->pNonlinGammatone[i].bLoopLock = false;
			// delay variables omitted for optimised function but maintain them here
			pEar->pEarFilters->pNonlinGammatone[i].zi[0] = 0;
			pEar->pEarFilters->pNonlinGammatone[i].zi[1] = 0;
			pEar->pEarFilters->pNonlinGammatone[i].bBoundaryValue = false;

			pEar->pEarFilters->pIHCciliaFilter[i].uiDenOrder = 1;
			pEar->pEarFilters->pIHCciliaFilter[i].uiNumOrder = 2;
			// if uiDenOrder == 1, then uiZSize = uiNumOrder - 1 if UidenOrder < uiNumOrder
			pEar->pEarFilters->pIHCciliaFilter[i].uiZSize = pEar->pEarFilters->pIHCciliaFilter[i].uiNumOrder - 1;
#if SINGLE_PRECISION
			pEar->pEarFilters->pIHCciliaFilter[i].b = new float [pEar->pEarFilters->pIHCciliaFilter[i].uiNumOrder];
			pEar->pEarFilters->pIHCciliaFilter[i].a = new float [pEar->pEarFilters->pIHCciliaFilter[i].uiDenOrder];
			pEar->pEarFilters->pIHCciliaFilter[i].zi = new float [pEar->pEarFilters->pIHCciliaFilter[i].uiZSize];
			pEar->pEarFilters->pIHCciliaFilter[i].zf = new float [pEar->pEarFilters->pIHCciliaFilter[i].uiZSize];
			pEar->pEarFilters->pIHCciliaFilter[i].prevX = new float [pEar->pEarFilters->pIHCciliaFilter[i].uiNumOrder - 1];
			pEar->pEarFilters->pIHCciliaFilter[i].prevY = new float [pEar->pEarFilters->pIHCciliaFilter[i].uiDenOrder - 1];
#elif DOUBLE_PRECISION
			pEar->pEarFilters->pIHCciliaFilter[i].b = new double [pEar->pEarFilters->pIHCciliaFilter[i].uiNumOrder];
			pEar->pEarFilters->pIHCciliaFilter[i].a = new double [pEar->pEarFilters->pIHCciliaFilter[i].uiDenOrder];
			pEar->pEarFilters->pIHCciliaFilter[i].zi = new double [pEar->pEarFilters->pIHCciliaFilter[i].uiZSize];
			pEar->pEarFilters->pIHCciliaFilter[i].zf = new double [pEar->pEarFilters->pIHCciliaFilter[i].uiZSize];
			pEar->pEarFilters->pIHCciliaFilter[i].prevX = new double [pEar->pEarFilters->pIHCciliaFilter[i].uiNumOrder - 1];
			pEar->pEarFilters->pIHCciliaFilter[i].prevY = new double [pEar->pEarFilters->pIHCciliaFilter[i].uiDenOrder - 1];
#endif
			pEar->pEarFilters->pIHCciliaFilter[i].iGammatonePassStatus = -1;
			pEar->pEarFilters->pIHCciliaFilter[i].bLoopLock = false;
			pEar->pEarFilters->pIHCciliaFilter[i].uiWinContinuity = 0;
			pEar->pEarFilters->pIHCciliaFilter[i].zi[0] = 0;
			pEar->pEarFilters->pIHCciliaFilter[i].bBoundaryValue = false;

#if SINGLE_PRECISION
			pEar->pIHC_presynapse->fmICaCurrent[i] = new float [pEar->uiMTnumANfTypesEndPt];
			pEar->pIHC_presynapse->fCaConcentration[i] = new float [pEar->uiMTnumANfTypesEndPt];

			pEar->pAN_IHCsynapse->fProbCleft[i] = new float [pEar->uiMTnumANfTypesEndPt];
			pEar->pAN_IHCsynapse->fProbAvailable[i] = new float [pEar->uiMTnumANfTypesEndPt];
			pEar->pAN_IHCsynapse->fProbReprocess[i] = new float [pEar->uiMTnumANfTypesEndPt];
			pEar->pAN_IHCsynapse->fPrevANprobNotFiring[i] = new float [pEar->uiMTnumANfTypesEndPt];
			pEar->pAN_IHCsynapse->fPrevANprobOutputRate[i] = new float* [pEar->uiMTnumANfTypesEndPt];
#elif DOUBLE_PRECISION
			pEar->pIHC_presynapse->fmICaCurrent[i] = new double [pEar->uiMTnumANfTypesEndPt];
			pEar->pIHC_presynapse->fCaConcentration[i] = new double [pEar->uiMTnumANfTypesEndPt];

			pEar->pAN_IHCsynapse->fProbCleft[i] = new double [pEar->uiMTnumANfTypesEndPt];
			pEar->pAN_IHCsynapse->fProbAvailable[i] = new double [pEar->uiMTnumANfTypesEndPt];
			pEar->pAN_IHCsynapse->fProbReprocess[i] = new double [pEar->uiMTnumANfTypesEndPt];
			pEar->pAN_IHCsynapse->fPrevANprobNotFiring[i] = new double [pEar->uiMTnumANfTypesEndPt];
			pEar->pAN_IHCsynapse->fPrevANprobOutputRate[i] = new double* [pEar->uiMTnumANfTypesEndPt];
#endif
			pEar->pAN_IHCsynapse->bFirstWindow[i] = new bool [pEar->uiMTnumANfTypesEndPt];

			pEar->uiRecBuffInc[i] = new UINT [pEar->uiNumANfibreTypes];
//			fProcData2Dump[i] = new float* [pEar->uiNumANfibreTypes];

			for (j=0; j<pEar->uiNumANfibreTypes; j++)
			{
#if SINGLE_PRECISION
				pEar->pAN_IHCsynapse->fPrevANprobOutputRate[i][j] = new float [pEar->uiAudioInputWinSize];
#elif DOUBLE_PRECISION
				pEar->pAN_IHCsynapse->fPrevANprobOutputRate[i][j] = new double [pEar->uiAudioInputWinSize];
#endif
				pEar->pAN_IHCsynapse->bFirstWindow[i][j] = true;
				pEar->uiRecBuffInc[i][j] = 0;														// initialise record buff inc index to 0
//				fProcData2Dump[i][j] = new float [pEar->uiAudioInputWinSize];
			}

			// initialise all the proc data max val to -65535
			pEar->fProcData2Sav1[i] = -65535;
			pEar->fProcData2Sav2[i] = -65535;
			pEar->fProcData2Sav3[i] = -65535;

			// calculate interval between every channel based inclusive of audio stimulus
			uiYspacing = ((YENDPLOT - YSTARTPLOT) / (pEar->uiNumBFchannels+2));
			// calculate ERB scale
			uiYscale[i] = (21.4*log10(0.00437*pEar->pDRNLstruct->fBFlist[i] + 1));
			// calculate y-axis offset start points for every line to be displayed
			uiYstart[i] = YENDPLOT - uiYscale[i] - uiYspacing * (i+1);
			// initialise all Imgbuff write tracker to 0
			uiImgBuffWriteTrack[i] = 0;

			fPrevXposProcData[i] = 0.f;
			fPrevYposProcData[i] = 0.f;
		}

#if SINGLE_PRECISION
		// allocate mem for IHC receptor potential processing
		pEar->pIHC_RP->fInitialV = new float [pEar->uiNumBFchannels];
		pEar->pIHC_RP->fIHCciliaDisplacement = new float [NUMOFCORES];
		pEar->pIHC_RP->fIHCciliaDispScaled = new float [NUMOFCORES];
		pEar->pIHC_RP->fGu = new float [NUMOFCORES];

		// allocate mem for IHC presynapse processing
		pEar->fVesicleReleaseRate = new float* [pEar->uiMTnumANfTypesEndPt];

		// each array below holds 1 variable for each thread
		pEar->pIHC_presynapse->fInitial_mICaCurrent = new float [pEar->uiNumANfibreTypes];
		pEar->pIHC_presynapse->fInitial_CaConcentration = new float [pEar->uiNumANfibreTypes];
		pEar->pIHC_presynapse->fmICaINF = new float [NUMOFCORES];
		pEar->pIHC_presynapse->fExponent = new float [NUMOFCORES];
		pEar->pIHC_presynapse->fICa = new float [NUMOFCORES];
		// each array below holds 2 or more variables for each thread
		pEar->pIHC_presynapse->fTauCa = new float [pEar->uiMTnumANfTypesEndPt];
		pEar->pIHC_presynapse->fKT0 = new float [pEar->uiMTnumANfTypesEndPt];

		// allocate mem for AN synapse processing
		pEar->pAN_IHCsynapse->fProbAN_ydt = new float [pEar->uiMTnumANfTypesEndPt];
		pEar->pAN_IHCsynapse->fProbAN_ldt = new float [pEar->uiMTnumANfTypesEndPt];
		pEar->pAN_IHCsynapse->fProbAN_xdt = new float [pEar->uiMTnumANfTypesEndPt];
		pEar->pAN_IHCsynapse->fProbAN_rdt = new float [pEar->uiMTnumANfTypesEndPt];
		pEar->pAN_IHCsynapse->fProbAN_rdtldt = new float [pEar->uiMTnumANfTypesEndPt];
		pEar->pAN_IHCsynapse->fInitProbCleft = new float [pEar->uiMTnumANfTypesEndPt];
		pEar->pAN_IHCsynapse->fInitProbAvailable = new float [pEar->uiMTnumANfTypesEndPt];
		pEar->pAN_IHCsynapse->fInitProbReprocess = new float [pEar->uiMTnumANfTypesEndPt];
		pEar->pAN_IHCsynapse->fM_Pq = new float [pEar->uiMTnumANfTypesEndPt];
		pEar->pAN_IHCsynapse->fProbReplenish = new float [pEar->uiMTnumANfTypesEndPt];
		pEar->pAN_IHCsynapse->fProbEjected = new float [pEar->uiMTnumANfTypesEndPt];
		pEar->pAN_IHCsynapse->fProbReprocessed = new float [pEar->uiMTnumANfTypesEndPt];
		pEar->pAN_IHCsynapse->fReuptakeAndLost = new float [pEar->uiMTnumANfTypesEndPt];
		pEar->pAN_IHCsynapse->fReuptake = new float [pEar->uiMTnumANfTypesEndPt];
		pEar->pAN_IHCsynapse->fANProbNotFiring = new float* [pEar->uiMTnumANfTypesEndPt];
		pEar->fANprobOutputRate = new float* [pEar->uiMTnumANfTypesEndPt];
		for (i=0; i<pEar->uiMTnumANfTypesEndPt; i++)
		{
			// IHC presynapse mem allocation based on num of AN channels
			pEar->fVesicleReleaseRate[i] = new float [pEar->uiAudioInputWinSize];

			// AN synapse mem allocation based on num of AN channels
			pEar->pAN_IHCsynapse->fANProbNotFiring[i] = new float [pEar->uiAudioInputWinSize];
			pEar->fANprobOutputRate[i] = new float [pEar->uiAudioInputWinSize];
		}
#elif DOUBLE_PRECISION
		// allocate mem for IHC receptor potential processing
		pEar->pIHC_RP->fInitialV = new double [pEar->uiNumBFchannels];
		pEar->pIHC_RP->fIHCciliaDisplacement = new double [NUMOFCORES];
		pEar->pIHC_RP->fIHCciliaDispScaled = new double [NUMOFCORES];
		pEar->pIHC_RP->fGu = new double [NUMOFCORES];

		// allocate mem for IHC presynapse processing
		pEar->fVesicleReleaseRate = new double* [pEar->uiMTnumANfTypesEndPt];

		// each array below holds 1 variable for each thread
		pEar->pIHC_presynapse->fInitial_mICaCurrent = new double [pEar->uiNumANfibreTypes];
		pEar->pIHC_presynapse->fInitial_CaConcentration = new double [pEar->uiNumANfibreTypes];
		pEar->pIHC_presynapse->fmICaINF = new double [NUMOFCORES];
		pEar->pIHC_presynapse->fExponent = new double [NUMOFCORES];
		pEar->pIHC_presynapse->fICa = new double [NUMOFCORES];
		// each array below holds 2 or more variables for each thread
		pEar->pIHC_presynapse->fTauCa = new double [pEar->uiMTnumANfTypesEndPt];
		pEar->pIHC_presynapse->fKT0 = new double [pEar->uiMTnumANfTypesEndPt];

		// allocate mem for AN synapse processing
		pEar->pAN_IHCsynapse->fProbAN_ydt = new double [pEar->uiMTnumANfTypesEndPt];
		pEar->pAN_IHCsynapse->fProbAN_ldt = new double [pEar->uiMTnumANfTypesEndPt];
		pEar->pAN_IHCsynapse->fProbAN_xdt = new double [pEar->uiMTnumANfTypesEndPt];
		pEar->pAN_IHCsynapse->fProbAN_rdt = new double [pEar->uiMTnumANfTypesEndPt];
		pEar->pAN_IHCsynapse->fProbAN_rdtldt = new double [pEar->uiMTnumANfTypesEndPt];
		pEar->pAN_IHCsynapse->fInitProbCleft = new double [pEar->uiMTnumANfTypesEndPt];
		pEar->pAN_IHCsynapse->fInitProbAvailable = new double [pEar->uiMTnumANfTypesEndPt];
		pEar->pAN_IHCsynapse->fInitProbReprocess = new double [pEar->uiMTnumANfTypesEndPt];
		pEar->pAN_IHCsynapse->fM_Pq = new double [pEar->uiMTnumANfTypesEndPt];
		pEar->pAN_IHCsynapse->fProbReplenish = new double [pEar->uiMTnumANfTypesEndPt];
		pEar->pAN_IHCsynapse->fProbEjected = new double [pEar->uiMTnumANfTypesEndPt];
		pEar->pAN_IHCsynapse->fProbReprocessed = new double [pEar->uiMTnumANfTypesEndPt];
		pEar->pAN_IHCsynapse->fReuptakeAndLost = new double [pEar->uiMTnumANfTypesEndPt];
		pEar->pAN_IHCsynapse->fReuptake = new double [pEar->uiMTnumANfTypesEndPt];
		pEar->pAN_IHCsynapse->fANProbNotFiring = new double* [pEar->uiMTnumANfTypesEndPt];
		pEar->fANprobOutputRate = new double* [pEar->uiMTnumANfTypesEndPt];
		for (i=0; i<pEar->uiMTnumANfTypesEndPt; i++)
		{
			// IHC presynapse mem allocation based on num of AN channels
			pEar->fVesicleReleaseRate[i] = new double [pEar->uiAudioInputWinSize];

			// AN synapse mem allocation based on num of AN channels
			pEar->pAN_IHCsynapse->fANProbNotFiring[i] = new double [pEar->uiAudioInputWinSize];
			pEar->fANprobOutputRate[i] = new double [pEar->uiAudioInputWinSize];
		}
#endif

		uiRTAPsetStatus = MemReallocated;

		// set the filter window continuity flag
		pEar->pOMEarStruct->pEERfilter->bLoopLock = false;
		pEar->pOMEarStruct->pTMfilter->bLoopLock = false;
		pEar->pOMEarStruct->pSIfilter->bLoopLock = false;
		pEar->pEarFilters->pLinGammatone->bLoopLock = false;
		pEar->pEarFilters->pNonlinGammatone->bLoopLock = false;
		pEar->pEarFilters->pIHCciliaFilter->bLoopLock = false;

		// set the Audio input gain
		fExponent = fAudioLvldBSPL / 20.f;
		fAudioInGain = 28e-6*pow(10, fExponent);
		f2PIoverFs = 2 * PI / fSamplingRate;

		// pre-calculate a sine tone cycle here
		uiSampPerCycle = (int)(fSamplingRate / iSineFreq);
		uiUnusedSamp = uiWindowSize % uiSampPerCycle;				// uiWindowSize = pEar->uiAudioInputWinSize
		uiCyclesPerWin = uiWindowSize / uiSampPerCycle;

		int iMax = fSamplingRate / (4 * iSineFreq);					// x-axis index for peak amplitude (y-axis)
		float fSawtoothGradient = 4 * fAudioInGain * fSineInpAmpAdj * iSineFreq / fSamplingRate;
		float fSawAcc = 0;
		int iSawIdx = 0;											// +ve gradient: even, -ve gradient: odd
		for (i=0; i<uiSampPerCycle; i++)
		{
			fAudioHiResSamples[i] = fAudioInGain * fSineInpAmpAdj * sin(f2PIoverFs * iSineFreq * i);

			if (cAudioSource == AUDIO_SQUARE)
			{
				if (fAudioHiResSamples[i] > 0)
					fAudioHiResSamples[i] = fAudioInGain * fSineInpAmpAdj;
				else if (fAudioHiResSamples[i] < 0)
					fAudioHiResSamples[i] = fAudioInGain * fSineInpAmpAdj * -1.f;
			}
			else if (cAudioSource == AUDIO_SAWTOOTH)
			{
				if (iSawIdx % 2)	fSawAcc -= fSawtoothGradient;
				else				fSawAcc += fSawtoothGradient;

				fAudioHiResSamples[i] = fSawAcc;

				// detect max peak-to-peak amplitude & increment index
				if (!(i % iMax))
					iSawIdx++;
			}

			// this is where a sine tone cycle is repeated over to form a continuous sine wave
			for (j=1; j<uiCyclesPerWin; j++)
			{
				// copy over the 1st cycle data over to the next cycle
				fAudioHiResSamples[(j*uiSampPerCycle)+i] = fAudioHiResSamples[i];
			}
		}

		// update the processed data per BF channel based if sine tone option chosen
		if ((cAudioSource == AUDIO_SINE) || (cAudioSource == AUDIO_SQUARE) || (cAudioSource == AUDIO_SAWTOOTH))
		{
			pEar->uiAudioInputWinSize = uiWindowSize - uiUnusedSamp;
			pEar->uiNumSampToRec = pEar->uiAudioInputWinSize * NUMWIN2REC;
		}

		// reset the variables for spectrogram scrolling display
		uiAudioPackets2BeProcessed = 0;
		// offset start position to the left of display window by 50 pixels
		siDisplayStartPoint = siDisplayWindowWidth - 50;
//		uiScreenStartRender = siDisplayWindowWidth;
		uiDestWidth = 0;
		siNumProcData2Display = 0;
		uiImgSrcX = 0;
		pEar->fYvalues[0] = 0;
		pEar->fYvalues[1] = 0;
		pEar->fYvalues[2] = 0;
		pEar->fMaxAcc = 0;
		pEar->fMinAcc = 0;
		pEar->uiMaxCount = 0;
		pEar->uiMinCount = 0;

		// semaphore initialisations
		dAlgoProcSem = 0;
		dPixRenderSem = 0;
		dPlotDispSem = 0;

		// allocate mem for file header dump
		FileHeader = new char [SIZEOFHEADER];

		// handle the data dump file header parameters
		FileHeader[0] = 'R';
		FileHeader[1] = 'T';
		FileHeader[2] = 'A';
		FileHeader[3] = 'P';
		FileHeader[4] = cAudioSource;
		// if sine tones used as input stimulus, then record its frequency
		if ((cAudioSource == AUDIO_SINE) || (cAudioSource == AUDIO_SQUARE) || (cAudioSource == AUDIO_SAWTOOTH))
		{
			FileHeader[5] = iSineFreq & 0xFF;
			FileHeader[6] = (iSineFreq >> 8) & 0xFF;
			FileHeader[7] = (iSineFreq >> 16) & 0xFF;
			FileHeader[8] = (iSineFreq >> 24) & 0xFF;
		}
		// if any other stimulus source then store an invalid data
		else
		{
			FileHeader[5] = 0xFF;
			FileHeader[6] = 0xFF;
			FileHeader[7] = 0xFF;
			FileHeader[8] = 0xFF;
		}
		// num of BF channels to be dumped into header file
		FileHeader[9] = pEar->uiNumBFchannels & 0xFF;				// get 1st byte
		FileHeader[10] = (pEar->uiNumBFchannels >> 8) & 0xFF;		// get 2nd byte
		FileHeader[11] = (pEar->uiNumBFchannels >> 16) & 0xFF;		// get 3rd byte
		FileHeader[12] = (pEar->uiNumBFchannels >> 24) & 0xFF;		// get 4th byte
		// num of AN fibre type to be dumped into header file
		FileHeader[13] = pEar->uiNumANfibreTypes & 0xFF;			// get 1st byte
		FileHeader[14] = (pEar->uiNumANfibreTypes >> 8) & 0xFF;		// get 2nd byte
		FileHeader[15] = (pEar->uiNumANfibreTypes >> 16) & 0xFF;	// get 3rd byte
		FileHeader[16] = (pEar->uiNumANfibreTypes >> 24) & 0xFF;	// get 4th byte
		// num of AN channels to be dumped into header file
		FileHeader[17] = pEar->uiNumANchannels & 0xFF;				// get 1st byte
		FileHeader[18] = (pEar->uiNumANchannels >> 8) & 0xFF;		// get 2nd byte
		FileHeader[19] = (pEar->uiNumANchannels >> 16) & 0xFF;		// get 3rd byte
		FileHeader[20] = (pEar->uiNumANchannels >> 24) & 0xFF;		// get 4th byte
		// function ID number to be dumped into header file
		FileHeader[21] = uiFunction2Rec & 0xFF;						// get 1st byte
		FileHeader[22] = (uiFunction2Rec >> 8) & 0xFF;				// get 2nd byte
		FileHeader[23] = (uiFunction2Rec >> 16) & 0xFF;				// get 3rd byte
		FileHeader[24] = (uiFunction2Rec >> 24) & 0xFF;				// get 4th byte
		// num of processed data per BF channel to be dumped into header file
		FileHeader[25] = pEar->uiAudioInputWinSize & 0xFF;			// get 1st byte
		FileHeader[26] = (pEar->uiAudioInputWinSize >> 8) & 0xFF;	// get 2nd byte
		FileHeader[27] = (pEar->uiAudioInputWinSize >> 16) & 0xFF;	// get 3rd byte
		FileHeader[28] = (pEar->uiAudioInputWinSize >> 24) & 0xFF;	// get 4th byte
		// store min BF parameter
		FileHeader[29] = uiMinBF & 0xFF;							// get LSByte
		FileHeader[30] = (uiMinBF >> 8) & 0xFF;
		FileHeader[31] = (uiMinBF >> 16) & 0xFF;
		FileHeader[32] = (uiMinBF >> 24) & 0xFF;					// get HSByte
		// store max BF parameter
		FileHeader[33] = uiMaxBF & 0xFF;							// get LSByte
		FileHeader[34] = (uiMaxBF >> 8) & 0xFF;
		FileHeader[35] = (uiMaxBF >> 16) & 0xFF;
		FileHeader[36] = (uiMaxBF >> 24) & 0xFF;
		// store AN fibre type select
		FileHeader[37] = iANfibreType & 0xFF;
		FileHeader[38] = (iANfibreType >> 8) & 0xFF;
		FileHeader[39] = (iANfibreType >> 16) & 0xFF;
		FileHeader[40] = (iANfibreType >> 24) & 0xFF;

		FileHeader[41] = 0x41;
		FileHeader[42] = 0x42;
		FileHeader[43] = 0x43;
		FileHeader[44] = 0x44;
		FileHeader[45] = 0x45;
		FileHeader[46] = 0x46;
		FileHeader[47] = 0x47;
		FileHeader[48] = 0x48;
		FileHeader[49] = 0x49;

		bMemAllocated = true;
	}

	uiAudioInputWinSize = pEar->uiAudioInputWinSize;

	// reset number of y-axis labels
	uiNumYaxisLabels = 0;
	LabelYaxis (pEar->pDRNLstruct->fBFlist);
	LabelXaxis();


	// set following variables for multi-threaded computing
	for (i=0; i<NUMOFCORES; i++)
	{
		// cater for dual core CPU (anything more & start & end points have to be modified accordingly as below)
		if (i == 0)
		{
			pEar->uiBFprocStart[0] = 0;
			pEar->uiBFprocEnd[0] = pEar->uiMTnumBFchannelsMidPt;
		}
		else
		{
			pEar->uiBFprocStart[i] = pEar->uiMTnumBFchannelsMidPt;
			pEar->uiBFprocEnd[i] = pEar->uiNumBFchannels;
		}

		pEar->fAudSigDataDisAcc = 0;	// initialise audio signal data accumulator to 0
		pEar->fProcDataDispAcc = 0;		// initialise processed data accumulator to 0
	}

	// reset variables
	fMaxPixDisplayVal = -655635;
	fMinPixDisplayVal = 655636;
	uiSubSamples = SCROLLNUMSAMP;

	uiRTAPsetStatus = APparamsSet;

	// set variables for respective structure in preparation for loop run
	setOMEparams (pEar);
	setDRNLparams (pEar);
	setIHCparams (pEar);
	setANparams (pEar);

	uiRTAPsetStatus = APalgoSet;

	// reset variables for storing runtime info
	dNumGetAuSampInvk = 0;
	dSummedAudioDevIOtime = 0;
	dNumOMEinvk = 0;
	dSummedOMEtime = 0;
	dNumAlgoInvk = 0;
	dSummedAlgoTime = 0;
	dNumPlotRenderInvk = 0;
	dSummedPlotRenderTime = 0;

	// File stream test - TO BE DELETED AFTER TESTING
	File fileTest (File::getCurrentWorkingDirectory().getChildFile("test.brtap"));
	FileOutputStream testStream (fileTest);

	testStream.write (FileHeader, SIZEOFHEADER);
	// File stream test - TO BE DELETED AFTER TESTING

	// start the threads to run in parallel
	ret = pthread_create (&hAlgoThread, NULL, &RTAPcompute::StartAlgoThread, this);
	ret = pthread_create (&hFileWriterThread, NULL, &RTAPcompute::StartRecThreads, this);

	uiRTAPsetStatus = ReinitialiseCompleted;
}

void RTAPcompute::LabelYaxis (float* BFlist)
{
	UINT i, j, k, uiBF, uiBFLabelInt, uiBFlabelOffset;
	UINT uiInterval, uiIntervalCap, uiSpectroIntOffset;

	uiBFLabelInt = 0;
	j = 0;
	uiSpectroIntOffset = 0;
	
	if (cWaveformType == SPECTROGRAM_PLOT)
	{
		uiInterval = PIXELHEIGHT;			// 1 row: 4 pixels for spectrogram
		uiIntervalCap = 10;
	}
	else
	{
		uiInterval = uiYspacing;			// interval varies for ERB scaled display
		uiIntervalCap = 15;
	}

	// set the number of y-axis labels to display
//	if (iFunctionToRun == DRNL_ST_OPT)
		uiBFlabelOffset = uiYspacing / 2;
//	else
//		uiBFlabelOffset = 0;

	// BF selection for labelling
	for (i=0; i<pEar->uiNumBFchannels; i++)
	{
		if ((uiBFLabelInt > uiIntervalCap) || (i == 0))
		{
			if (cWaveformType == SPECTROGRAM_PLOT)
				uiBFlabelYaxis[j] = uiYstart[0] - uiSpectroIntOffset;
			else
				uiBFlabelYaxis[j] = uiYstart[i] - uiBFlabelOffset;
			uiBFLabelInt = 0;					// reset accumulator only when BF label selected for display on y-axis
			uiNumYaxisLabels++;

			uiBF = (UINT)BFlist[i];
			itoa (uiBF, cYaxisLabel[j], 10);

			if (uiBF / 10000)
				k = 5;
			else if (uiBF / 1000)
				k = 4;
			else if (uiBF / 100)
				k = 3;
			else
				k = 0;

			cYaxisLabel[j][k] = 0x20;				// <space>
			cYaxisLabel[j][k+1] = 0x48;				// 'H'
			cYaxisLabel[j][k+2] = 0x7A;				// 'z'

			j++;
		}

		// regularly add increment only if the index is less than num of BF channels
		if ((i >= 1) || (i < pEar->uiNumBFchannels))
		{
			uiBFLabelInt += uiInterval;
			uiSpectroIntOffset += uiInterval;
		}
	}
}

void RTAPcompute::LabelXaxis()
{
	UINT i, j, uiXinterval, uiTimeInterval, uiXintAcc;

	uiTimeInterval = j = 0;
	uiXintAcc = 0;
	uiTimeAcc = 0;

	if (bScrollPlot)
	{
		uiXintPix = fSamplingRate / SCROLLNUMSAMP;		// 1-sec x-axis label intervals in terms of pixels

		for (i=0; i<NUMOFXLABELS; i++)
		{
			uiXintAcc += uiXintPix;
			uiTimeLabelXaxis[i] = uiXintAcc;
		}
	}
	else
	{
		uiXinterval = pEar->uiAudioInputWinSize / NUMOFXLABELS;
		uiTimeInterval = 1000*uiXinterval/fSamplingRate;		// time interval in milliseconds
		for (i=0; i<NUMOFXLABELS; i++)
		{
			uiXintAcc += uiXinterval;
			uiTimeAcc += uiTimeInterval;
			uiTimeLabelXaxis[i] = uiXintAcc;
			itoa (uiTimeAcc, cXaxisLabel[i], 10);

			if (uiTimeAcc / 10)
				j = 2;
			else if (uiTimeAcc % 10)
				j = 1;

			cXaxisLabel[i][j] = 0x20;			// <space>
			cXaxisLabel[i][j+1] = 0x6D;			// 'm'
			cXaxisLabel[i][j+2] = 0x73;			// 's'
		}
	}
}

void RTAPcompute::setOMEparams (EarStruct* pEar)
{
	setButterworthCoeff(pEar);
	// set EER filter zi coefficients
	pEar->pOMEarStruct->pEERfilter->zi[0] = 0;
	pEar->pOMEarStruct->pEERfilter->zi[1] = 0;
	// set TM displacement filter coefficients (convert from pressure to velocity using smoothing filter with 50Hz cutoff)
	pEar->pOMEarStruct->pTMfilter->a[0] = 1;
	pEar->pOMEarStruct->pTMfilter->a[1] = pEar->fDt/(1/(2*PI*50)) - 1;//-0.992876;
	pEar->pOMEarStruct->pTMfilter->b[0] = pEar->pOMEarStruct->pTMfilter->a[1] + 1;//0.00712379;
	pEar->pOMEarStruct->pTMfilter->zi[0] = 0;
	// set SI filter zi coefficients
	pEar->pOMEarStruct->pSIfilter->zi[0] = 0;
}

void RTAPcompute::setDRNLparams ( EarStruct* pEar )
{
#if SINGLE_PRECISION
	float fPhi, fTheta, fCosTheta, fSinTheta, fAlpha, fZReal[4], fZImag[4], fZ0;
#elif DOUBLE_PRECISION
	double fPhi, fTheta, fCosTheta, fSinTheta, fAlpha, fZReal[4], fZImag[4], fZ0;
#endif
	UINT i;

#if SINGLE_PRECISION
	// p = 0.2895, q = 250 (for humans) p = 0.14, q = 366 (for cat)
	const float p = 0.2895;
	const float q = 250;
	const float minLinCF = 153.13;
	const float coeffLinCF = 0.7341;
	const float minLinBW = 100;
	const float coeffLinBW = 0.6531;
#elif DOUBLE_PRECISION
	// p = 0.2895, q = 250 (for humans) p = 0.14, q = 366 (for cat)
	const double p = 0.2895;
	const double q = 250;
	const double minLinCF = 153.13;
	const double coeffLinCF = 0.7341;
	const double minLinBW = 100;
	const double coeffLinBW = 0.6531;
#endif

	// Acquire these constants from the RTAP system parameter settings
	pEar->pDRNLstruct->fDRNLa = fDRNLa;									// MAP1_14e default = 5e4
	pEar->pDRNLstruct->fDRNLb = fDRNLb;									// MAP1_14e default = 3e-4
	pEar->pDRNLstruct->fDRNLc = fDRNLc;									// MAP1_14e default = 0.2

	pEar->pDRNLstruct->fCompThreshBMdB = fComprThreshBMdB;				// MAP1_14e default = 10;
	pEar->pDRNLstruct->fLinGain = fLinGain;								// MAP1_14e default = 500

	pEar->pDRNLstruct->uiLinOrder = iGammatoneFiltOrder;				// MAP1_14e default = 3
	pEar->pDRNLstruct->uiNonLinOrder = iGammatoneFiltOrder;			// MAP1_14e default = 3
	pEar->pDRNLstruct->fP = p;											// MAP1_14e SYNC
	pEar->pDRNLstruct->fQ = q;											// MAP1_14e SYNC
	pEar->pDRNLstruct->uiFilterOrderParity = iGammatoneFiltOrder % 2;	// Compute gammatone filter order even/odd parity
	pEar->pDRNLstruct->fCompThreshBM = 10e-9f*pow(10, pEar->pDRNLstruct->fCompThreshBMdB/20.f);
	pEar->pDRNLstruct->fCompThreshScaled = pEar->pDRNLstruct->fCompThreshBM / pEar->pDRNLstruct->fDRNLa;

	for (i=0; i<pEar->uiNumBFchannels; i++)
	{
		// calculate linBW
		pEar->pDRNLstruct->fLinBW[i] = minLinBW + (coeffLinBW * pEar->pDRNLstruct->fBFlist[i]);
		// calculate linCF
		pEar->pDRNLstruct->fLinCF[i] = minLinCF + (coeffLinCF * pEar->pDRNLstruct->fBFlist[i]);

		pEar->pDRNLstruct->fNonLinBW[i] = (pEar->pDRNLstruct->fP * pEar->pDRNLstruct->fBFlist[i]) + pEar->pDRNLstruct->fQ;
		pEar->pDRNLstruct->fNonLinCF[i] = pEar->pDRNLstruct->fBFlist[i];

		// calculate gammatone filter coefficients for linear path
		fPhi = 2 * PI * pEar->pDRNLstruct->fLinBW[i] * pEar->fDt;
		fTheta = 2 * PI * pEar->pDRNLstruct->fLinCF[i] * pEar->fDt;
		fCosTheta = cos(fTheta);
		fSinTheta = sin(fTheta);
		fAlpha = -exp(-fPhi) * fCosTheta;
		// *** contents of b & a swapped w.r.t. MAP1_14e
		pEar->pEarFilters->pLinGammatone[i].a[0] = 1.0f;
		pEar->pEarFilters->pLinGammatone[i].a[1] = 2 * fAlpha;
		pEar->pEarFilters->pLinGammatone[i].a[2] = exp(-2 * fPhi);
		fZReal[0] = 1 + (fAlpha * fCosTheta);
		fZImag[0] = -(fAlpha * fSinTheta);
		// convert to polar coordinate for division
		fZ0 = (sqrt(pow(fZReal[0],2) + pow(fZImag[0],2)));
		if (!fZ0)
			fZ0 = 1;										// ensure that there is no divide by 0 scenario
		fZReal[1] = 1 + (pEar->pEarFilters->pLinGammatone[i].a[1] * fCosTheta);
		fZImag[1] = -(pEar->pEarFilters->pLinGammatone[i].a[1] * fSinTheta);
		fZReal[2] = pEar->pEarFilters->pLinGammatone[i].a[2] * cos(2* fTheta);
		fZImag[2] = -(pEar->pEarFilters->pLinGammatone[i].a[2] * sin(2*fTheta));
		fZReal[3] = fZReal[1] + fZReal[2];
		fZImag[3] = fZImag[1] + fZImag[2];
		// calculate coeff by converting real & imag parts into polar coord & then dividing by z0
		pEar->pEarFilters->pLinGammatone[i].b[0] = (sqrt(pow(fZReal[3],2) + pow(fZImag[3],2))) / fZ0;
		pEar->pEarFilters->pLinGammatone[i].b[1] = fAlpha * pEar->pEarFilters->pLinGammatone[i].b[0];

		// calculate gammatone filter coefficients for nonlinear path
		fPhi = 2 * PI * pEar->pDRNLstruct->fNonLinBW[i] * pEar->fDt;
		fTheta = 2 * PI * pEar->pDRNLstruct->fNonLinCF[i] *pEar->fDt;
		fCosTheta = cos(fTheta);
		fSinTheta = sin(fTheta);
		fAlpha = -exp(-fPhi) * fCosTheta;
		pEar->pEarFilters->pNonlinGammatone[i].a[0] = 1.0f;
		pEar->pEarFilters->pNonlinGammatone[i].a[1] = 2 * fAlpha;
		pEar->pEarFilters->pNonlinGammatone[i].a[2] = exp(-2 * fPhi);
		pEar->pEarFilters->pNonlinGammatone[i].a[0] = 1.0f;
		pEar->pEarFilters->pNonlinGammatone[i].a[1] = 2 * fAlpha;
		pEar->pEarFilters->pNonlinGammatone[i].a[2] = exp(-2 * fPhi);
		fZReal[0] = 1 + (fAlpha * fCosTheta);
		fZImag[0] = -(fAlpha * fSinTheta);
		// convert to polar coordinate for division
		fZ0 = (sqrt(pow(fZReal[0],2) + pow(fZImag[0],2)));
		if (!fZ0)
			fZ0 = 1;										// ensure that there is no divide by 0 scenario
		fZReal[1] = 1 + (pEar->pEarFilters->pNonlinGammatone[i].a[1] * fCosTheta);
		fZImag[1] = -(pEar->pEarFilters->pNonlinGammatone[i].a[1] * fSinTheta);
		fZReal[2] = pEar->pEarFilters->pNonlinGammatone[i].a[2] * cos(2* fTheta);
		fZImag[2] = -(pEar->pEarFilters->pNonlinGammatone[i].a[2] * sin(2*fTheta));
		fZReal[3] = fZReal[1] + fZReal[2];
		fZImag[3] = fZImag[1] + fZImag[2];
		// calculate coeff by converting real & imag parts into polar coord & then dividing by z0
		pEar->pEarFilters->pNonlinGammatone[i].b[0] = (sqrt(pow(fZReal[3],2) + pow(fZImag[3],2))) / fZ0;
		pEar->pEarFilters->pNonlinGammatone[i].b[1] = fAlpha * pEar->pEarFilters->pNonlinGammatone[i].b[0];
	}

	// optimised parameter
	pEar->pDRNLstruct->fDBSvar1 = pEar->pDRNLstruct->fDRNLa / pEar->pDRNLstruct->fCompThreshBM;
}

void RTAPcompute::setIHCparams (EarStruct* pEar)
{
	UINT i, j;

	// set constants for IHC receptor potential struct
	pEar->pIHC_RP->fTC = 0.00012f;					// 0.0003 by Shanma
	pEar->pIHC_RP->fC = 0.08f;
	pEar->pIHC_RP->fU0 = 5e-9f;
	pEar->pIHC_RP->fS0 = 30e-9f;
	pEar->pIHC_RP->fU1 = 1e-9f;
	pEar->pIHC_RP->fS1 = 1e-9f;
	pEar->pIHC_RP->fGmax = 6e-9f;					// max conductance (Siemens)
	pEar->pIHC_RP->fGa = 0.8e-9f;					// fixed apical conductance
	pEar->pIHC_RP->fCap = 4e-12f;					// IHC capacitance (F)
	pEar->pIHC_RP->fEt = 0.1f;						// endocochlear potential (v)
	pEar->pIHC_RP->fGk = 2e-8f;						// potassium conductance (S)
	pEar->pIHC_RP->fEk = -0.08f;						// potassium equilibrium potential
	pEar->pIHC_RP->fRpc = 0.04f;						// combined resistances

	// Prepare for IHC apical conductance (Boltzmann function) calculation
	pEar->pIHC_RP->fGu0 = pEar->pIHC_RP->fGa + pEar->pIHC_RP->fGmax / (1 + exp(pEar->pIHC_RP->fU0/pEar->pIHC_RP->fS0) * (1 + exp(pEar->pIHC_RP->fU1/pEar->pIHC_RP->fS1)));
	// Prepare for receptor potential calculation
	pEar->pIHC_RP->fEkp  = pEar->pIHC_RP->fEk + pEar->pIHC_RP->fEt * pEar->pIHC_RP->fRpc;
	pEar->pIHC_RP->fRestingPotential = (pEar->pIHC_RP->fGk * pEar->pIHC_RP->fEkp + pEar->pIHC_RP->fGu0 * pEar->pIHC_RP->fEt) / (pEar->pIHC_RP->fGu0 + pEar->pIHC_RP->fGk);
	// pre-compute for speed up
	pEar->pIHC_RP->fIHCrpVar1 = pEar->fDt / pEar->pIHC_RP->fCap;
	pEar->pIHC_RP->fExpU0oS0 = exp(pEar->pIHC_RP->fU0 / pEar->pIHC_RP->fS0);
	pEar->pIHC_RP->fExpU1oS1 = exp(pEar->pIHC_RP->fU1 / pEar->pIHC_RP->fS1);

	// Effect of viscous coupling between BM and stereocilia displacement
	// Set high pass filter coefficients
	for (i=0; i<pEar->uiNumBFchannels; i++)
	{
		pEar->pEarFilters->pIHCciliaFilter[i].b[0] = 1.0f;
		pEar->pEarFilters->pIHCciliaFilter[i].b[1] = pEar->fDt / pEar->pIHC_RP->fTC - 1;
		pEar->pEarFilters->pIHCciliaFilter[i].a[0] = 1 + pEar->pEarFilters->pIHCciliaFilter->b[1];

		pEar->pIHC_RP->fInitialV[i] = pEar->pIHC_RP->fRestingPotential;
	}

	// set constants for IHC presynapse struct
	pEar->pIHC_presynapse->fGmaxCa = 14e-9f;		// max calcium conductance
	pEar->pIHC_presynapse->fECa = 0.066f;			// calcium equilibrium potential
	pEar->pIHC_presynapse->fBeta = 400;				// determines calcium channel opening
	pEar->pIHC_presynapse->fGamma = 100;			// determines calcium channel opening
	pEar->pIHC_presynapse->fTauM = 0.00005f;		// membrane time constant
	pEar->pIHC_presynapse->fPower = 3;
	// note: changing fZ has a strong effect on HF thresholds (like fEt)
	pEar->pIHC_presynapse->dwZ = 2e42;				// calcium scalar -> vesicle release rate

	// check to see if only 1 type of AN channel required
	if (pEar->uiNumANfibreTypes == 1)
	{
		if (iANfibreType == 1)								// LSR AN channel selected
			pEar->pIHC_presynapse->fTauCa[0] = 30e-6f;		// LSRtauCa - seconds
		else if (iANfibreType == 2)						// HSR AN channel selected
			pEar->pIHC_presynapse->fTauCa[0] = 80e-6f;		// HSRtauCa - seconds
	}
	// 2 types of AN channels required
	else 
	{
		pEar->pIHC_presynapse->fTauCa[0] = 30e-6f;			// LSRtauCa - seconds
		pEar->pIHC_presynapse->fTauCa[1] = 80e-6f;			// HSRtauCa - seconds
	}
	
	// pre-compute variables for speed up
	pEar->pIHC_presynapse->fIHCpresynapseVar1 = pEar->fDt / pEar->pIHC_presynapse->fTauM;

	for (i=0; i<pEar->uiNumANfibreTypes; i++)
	{
		pEar->pIHC_presynapse->fInitial_mICaCurrent[i] = 1 / (1 + (1/pEar->pIHC_presynapse->fBeta) * exp(-pEar->pIHC_presynapse->fGamma * pEar->pIHC_RP->fRestingPotential));
		pEar->pIHC_presynapse->fInitial_CaConcentration[i] = pEar->pIHC_presynapse->fGmaxCa * pow(pEar->pIHC_presynapse->fInitial_mICaCurrent[i], 3) * (pEar->pIHC_RP->fRestingPotential - pEar->pIHC_presynapse->fECa) * pEar->pIHC_presynapse->fTauCa[i];
		for (j=i; j<pEar->uiMTnumANfTypesEndPt; j+=2)
#if SINGLE_PRECISION
			pEar->pIHC_presynapse->fKT0[j] = (float) (-pEar->pIHC_presynapse->dwZ * pow(pEar->pIHC_presynapse->fInitial_CaConcentration[i], pEar->pIHC_presynapse->fPower));
#elif DOUBLE_PRECISION
			pEar->pIHC_presynapse->fKT0[j] = (double) (-pEar->pIHC_presynapse->dwZ * pow(pEar->pIHC_presynapse->fInitial_CaConcentration[i], pEar->pIHC_presynapse->fPower));
#endif
	}

	for (i=0; i<pEar->uiNumBFchannels; i++)
	{
		// initialise the following variables for IHC presynapse
		pEar->pIHC_presynapse->fmICaCurrent[i][0] = pEar->pIHC_presynapse->fInitial_mICaCurrent[0];
		pEar->pIHC_presynapse->fCaConcentration[i][0] = pEar->pIHC_presynapse->fInitial_CaConcentration[0];
		if (pEar->uiNumANfibreTypes == 2)
		{
			pEar->pIHC_presynapse->fmICaCurrent[i][1] = pEar->pIHC_presynapse->fInitial_mICaCurrent[1];
			pEar->pIHC_presynapse->fCaConcentration[i][1] = pEar->pIHC_presynapse->fInitial_CaConcentration[1];
		}
	}
}

void RTAPcompute::setANparams ( EarStruct* pEar )
{
	UINT i;

	pEar->pAN_IHCsynapse->fRefractoryPeriod = 0.00075f;
	pEar->pAN_IHCsynapse->fTWdelay = 0.004f;
	pEar->pAN_IHCsynapse->fSpikesTargetSampleRate = 10000.f;
	pEar->pAN_IHCsynapse->fM = 12.0f;						// Maximum vesicles at synapse
	pEar->pAN_IHCsynapse->fY = 6.0f;						// Depleted vesicle replacement rate
	pEar->pAN_IHCsynapse->fX = 60.0f;						// Replenishment from re-uptake store
	pEar->pAN_IHCsynapse->fL = 250.0f;						// Vesicle loss rate from the cleft
	pEar->pAN_IHCsynapse->fR = 500.0f;						// Reuptake rate from cleft into IHC
	pEar->pAN_IHCsynapse->uiRefractoryPeriodIndex = RTAP_round(pEar->pAN_IHCsynapse->fRefractoryPeriod / pEar->fDt);
	// in MAP this refrac variable is plugged into vector starting from an index of 1 but in C its 0 so to accomodate this change -1 is used here
	pEar->pAN_IHCsynapse->uiRefrac = RTAP_round(pEar->pAN_IHCsynapse->uiRefractoryPeriodIndex * 1.5);

	for (i=0; i<pEar->uiMTnumANfTypesEndPt; i++)
	{
		// compute initial probability values
		pEar->pAN_IHCsynapse->fProbAN_ydt[i] = pEar->pAN_IHCsynapse->fY * pEar->fDt;
		pEar->pAN_IHCsynapse->fProbAN_ldt[i] = pEar->pAN_IHCsynapse->fL * pEar->fDt;
		pEar->pAN_IHCsynapse->fProbAN_xdt[i] = pEar->pAN_IHCsynapse->fX * pEar->fDt;
		pEar->pAN_IHCsynapse->fProbAN_rdt[i] = pEar->pAN_IHCsynapse->fR * pEar->fDt;
		pEar->pAN_IHCsynapse->fProbAN_rdtldt[i] = pEar->pAN_IHCsynapse->fProbAN_rdt[i] + pEar->pAN_IHCsynapse->fProbAN_ldt[i];

		// compute starting values
		pEar->pAN_IHCsynapse->fInitProbCleft[i] = pEar->pIHC_presynapse->fKT0[i] * pEar->pAN_IHCsynapse->fY * pEar->pAN_IHCsynapse->fM / (pEar->pAN_IHCsynapse->fY * (pEar->pAN_IHCsynapse->fL + pEar->pAN_IHCsynapse->fR) + pEar->pIHC_presynapse->fKT0[i] * pEar->pAN_IHCsynapse->fL);
		pEar->pAN_IHCsynapse->fInitProbAvailable[i] = pEar->pAN_IHCsynapse->fInitProbCleft[i] * (pEar->pAN_IHCsynapse->fL + pEar->pAN_IHCsynapse->fR) / pEar->pIHC_presynapse->fKT0[i];
		pEar->pAN_IHCsynapse->fInitProbReprocess[i] = pEar->pAN_IHCsynapse->fInitProbCleft[i] * pEar->pAN_IHCsynapse->fR / pEar->pAN_IHCsynapse->fX;
	}

	for (i=0; i<pEar->uiNumBFchannels; i++)
	{
		// initialise the following variables for AN syanpse
		pEar->pAN_IHCsynapse->fProbCleft[i][0] = pEar->pAN_IHCsynapse->fInitProbCleft[0];
		pEar->pAN_IHCsynapse->fProbAvailable[i][0] = pEar->pAN_IHCsynapse->fInitProbAvailable[0];
		pEar->pAN_IHCsynapse->fProbReprocess[i][0] = pEar->pAN_IHCsynapse->fInitProbReprocess[0];
		if (pEar->uiNumANfibreTypes == 2)
		{
			pEar->pAN_IHCsynapse->fProbCleft[i][1] = pEar->pAN_IHCsynapse->fInitProbCleft[1];
			pEar->pAN_IHCsynapse->fProbAvailable[i][1] = pEar->pAN_IHCsynapse->fInitProbAvailable[1];
			pEar->pAN_IHCsynapse->fProbReprocess[i][1] = pEar->pAN_IHCsynapse->fInitProbReprocess[1];
		}
	}
}

#if SINGLE_PRECISION
void RTAPcompute::RTAP_logspace (float* fNum, float fXlower, float fXupper, UINT uiNumPoints)
#elif DOUBLE_PRECISION
void RTAPcompute::RTAP_logspace (double* fNum, double fXlower, double fXupper, UINT uiNumPoints)
#endif
{
#if SINGLE_PRECISION
	float fStep;
#elif DOUBLE_PRECISION
	double fStep;
#endif
	UINT i;

	fXlower = log10(fXlower);
	fXupper = log10(fXupper);
	// ensure no divide by 0 scenarios
	if (uiNumPoints == 1)
		fStep = fXupper - fXlower;
	else
		fStep = (fXupper - fXlower) / (uiNumPoints - 1);

	for (i=0; i<uiNumPoints; i++)
	{
		fNum[i] = fXlower + (i * fStep);
		fNum[i] = pow(10, fNum[i]);
	}
}

#if SINGLE_PRECISION
float RTAPcompute::RTAP_exp (float x)
#elif DOUBLE_PRECISION
double RTAPcompute::RTAP_exp (double x)
#endif
{
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
	} e;

	e.n.i1 = EXP_A*(x) + (EXP_B - EXP_C);

	return (e.d);
}

#if SINGLE_PRECISION
UINT RTAPcompute::RTAP_round (float fX)
#elif DOUBLE_PRECISION
UINT RTAPcompute::RTAP_round (double fX)
#endif
{
	UINT uiRounded;
#if SINGLE_PRECISION
	float fDecimal;
#elif DOUBLE_PRECISION
	double fDecimal;
#endif

	uiRounded = (UINT) fX;
	fDecimal = fX - uiRounded;
	if (fDecimal >= 0.5)
		uiRounded += 1;

	return uiRounded;
}

#if SINGLE_PRECISION
float RTAPcompute::RTAP_abs (float fX)
#elif DOUBLE_PRECISION
double RTAPcompute::RTAP_abs (double fX)
#endif
{
	int siSign;

	if (fX > 0)
		siSign = 1;
	else if (fX < 0)
		siSign = -1;
	else
		siSign = 0;

	return (siSign * fX);
}

#if SINGLE_PRECISION
int RTAPcompute::RTAP_sign (float fX)
#elif DOUBLE_PRECISION
int RTAPcompute::RTAP_sign (double fX)
#endif
{
	if (fX > 0)
		return 1;
	else if (fX == 0)
		return 0;
	else
		return -1;
}

void RTAPcompute::RTAP_MaxMinSymmetry ()
{
	// Ensure max & min values are symmetrical
	if ((fMaxPixDisplayVal >= 0.f) && (fMinPixDisplayVal <= 0.f) && (iFunctionToRun == DRNL_ST_OPT))
	{
		if (RTAP_abs (fMaxPixDisplayVal) > RTAP_abs (fMinPixDisplayVal))
			fMinPixDisplayVal = 0 - fMaxPixDisplayVal;		// extend min val acquired to be symmetrical to max val
		else
			fMaxPixDisplayVal = 0 - fMinPixDisplayVal;		// extend max val acquired to be symmetrical to min val
	}
	else if ((fMaxPixDisplayVal <= 0.f) && (fMinPixDisplayVal <= 0.f) && (iFunctionToRun == DRNL2IHCRP_ST_OPT))
	{
		// ensure value is below 0 especially for ymax
		if (fMaxPixDisplayVal == 0.f)
			fMaxPixDisplayVal = fMinPixDisplayVal;
		
		if (RTAP_abs (fMaxPixDisplayVal - pEar->pIHC_RP->fRestingPotential) > RTAP_abs (pEar->pIHC_RP->fRestingPotential - fMinPixDisplayVal))
			fMinPixDisplayVal = pEar->pIHC_RP->fRestingPotential + (pEar->pIHC_RP->fRestingPotential - fMaxPixDisplayVal);
		else if (RTAP_abs (fMaxPixDisplayVal - pEar->pIHC_RP->fRestingPotential) < RTAP_abs (pEar->pIHC_RP->fRestingPotential - fMinPixDisplayVal))
			fMaxPixDisplayVal = pEar->pIHC_RP->fRestingPotential + (pEar->pIHC_RP->fRestingPotential - fMinPixDisplayVal);
	}
	else if ((fMaxPixDisplayVal > 0.f) && (fMinPixDisplayVal > 0.f) && (iFunctionToRun == DRNL2IHCpresynapse_ST_OPT))
	{
		if (RTAP_abs (fMaxPixDisplayVal - LOAXIS_NRR_LSR) > (LOAXIS_NRR_LSR - fMinPixDisplayVal))
			fMinPixDisplayVal = LOAXIS_NRR_LSR - (fMaxPixDisplayVal - LOAXIS_NRR_LSR);
		else if (RTAP_abs (fMaxPixDisplayVal - LOAXIS_NRR_LSR) < RTAP_abs (LOAXIS_NRR_LSR - fMinPixDisplayVal))
			fMaxPixDisplayVal = LOAXIS_NRR_LSR + (LOAXIS_NRR_LSR - fMinPixDisplayVal);
	}
	else if ((fMaxPixDisplayVal > 0.f) && (fMinPixDisplayVal > 0.f) && (iFunctionToRun == DRNL2ANsynapse_ST_OPT))
	{
		if (RTAP_abs (fMaxPixDisplayVal - LOAXIS_ANSP_LSR) > (LOAXIS_ANSP_LSR - fMinPixDisplayVal))
			fMinPixDisplayVal = LOAXIS_ANSP_LSR - (fMaxPixDisplayVal - LOAXIS_ANSP_LSR);
		else if (RTAP_abs (fMaxPixDisplayVal - LOAXIS_ANSP_LSR) < RTAP_abs (LOAXIS_ANSP_LSR - fMinPixDisplayVal))
			fMaxPixDisplayVal = LOAXIS_ANSP_LSR + (LOAXIS_ANSP_LSR - fMinPixDisplayVal);
	}
}

void RTAPcompute::setButterworthCoeff (EarStruct* pEar)
{
	// General variables to be used by all Butterworth filter types
    const int n = 1;				// filter order
    int i;							// loop variables
	double sf;						// scaling factor
    double *dcof;					// d coefficients
    int *ccof;						// c coefficients
	// Butterworth bandpass filter variables
	const UINT f1 = 1000;			// lower cut-off freq (Hz)
	const UINT f2 = 4000;			// higher cut-off freq (Hz)
    double f1f;						// lower cutoff frequency (fraction of pi)
    double f2f;						// upper cutoff frequency (fraction of pi)
	// Butterworth highpass filter variables (for stapes displacement)
	const int fc = 1000;			// cut-ff freq (Hz)
	double fcf;						// cutoff frequency (fraction of pi)

	f1f = (double) f1 / (fSamplingRate / 2);
	f2f = (double) f2 / (fSamplingRate / 2);
	fcf = (double) fc / (fSamplingRate / 2);

	// Calculate the coeff for EER BPF 1st
	// -> calculate the d BPF coefficients
	dcof = dcof_bwbp( n, f1f, f2f );
	if ( dcof == NULL )
	{
		perror( "Unable to calculate d coefficients" );
	}

	// -> calculate the c BPF coefficients
	ccof = ccof_bwbp( n );
	if ( ccof == NULL )
		perror( "Unable to calculate c coefficients" );

	sf = sf_bwbp( n, f1f, f2f ); // scaling factor for the c BPF coefficients

	for( i = 0; i <= 2*n; ++i)
	{
#if SINGLE_PRECISION
		// Output the c BPF coefficients
		pEar->pOMEarStruct->pEERfilter->b[i] = (float)ccof[i]*sf;
		// Output the d BPF coefficients
		pEar->pOMEarStruct->pEERfilter->a[i] = (float)dcof[i];
#elif DOUBLE_PRECISION
		// Output the c BPF coefficients
		pEar->pOMEarStruct->pEERfilter->b[i] = ccof[i]*sf;
		// Output the d BPF coefficients
		pEar->pOMEarStruct->pEERfilter->a[i] = dcof[i];
#endif
	}

	free (dcof);
	free (ccof);

	// Calculate the coeff for Stapes displacement HPF
	// calculate the d coefficients
	dcof = dcof_bwhp( n, fcf );
	if( dcof == NULL )
		perror( "Unable to calculate d coefficients" );

	// calculate the c coefficients
	ccof = ccof_bwhp( n );
	if( ccof == NULL )
		perror( "Unable to calculate c coefficients" );

	sf = sf_bwhp( n, fcf );		// scaling factor for the c coefficients

	for( i = 0; i <= n; ++i)
	{
#if SINGLE_PRECISION
		// Output the c coefficients
		pEar->pOMEarStruct->pSIfilter->b[i] = (float)ccof[i]*sf;
		// Output the d coefficients
		pEar->pOMEarStruct->pSIfilter->a[i] = (float)dcof[i];
#elif DOUBLE_PRECISION
		// Output the c coefficients
		pEar->pOMEarStruct->pSIfilter->b[i] = ccof[i]*sf;
		// Output the d coefficients
		pEar->pOMEarStruct->pSIfilter->a[i] = dcof[i];
#endif
	}

	free (dcof);
	free (ccof);
}

void RTAPcompute::getAudioSamples()
{
#if DISPLAY_PROCTIME
	dGetAudioSamplesStartTime = Time::getMillisecondCounterHiRes();
#endif

#if OME_INPUT_CHECK
#if SINGLE_PRECISION
	float fMin = (2<<sizeof(float)), fMax = 0;			// for debug purposes
#elif DOUBLE_PRECISION
	double fMin = (2<<sizeof(double)), fMax = 0;			// for debug purposes
#endif
#endif
	UINT i, temp;

#if OME_INPUT_CHECK
	char filename2[50] = "Debug_files/OMEinput.txt";
	ofstream OMEinput;
	OMEinput.open(filename2, ios::app);
	OMEinput << "In getAudioSamples() now" << endl;
#endif

	for (i=0; i<pEar->uiAudioInputWinSize; i++)
	{
		// cycle back to start once end of buff reached
		temp = (uiHiResAudioReadTracker++)%pEar->uiAudioInputWinSize;
		fAudioInput[i] = fAudioHiResSamples[temp];

		// only perform amplitude checking for audio streamed from Mic In
		if (cAudioSource == AUDIO_SYSTEM)
		{
			fAudioInput[i] = fAudioInput[i] * fAudioInGain;

			if (fAudioInput[i] > 1) fAudioInput[i] = 1;
			else if (fAudioInput[i] < -1) fAudioInput[i] = -1;
		}
		else if ((cAudioSource == AUDIO_SINE) || (cAudioSource == AUDIO_SQUARE) || (cAudioSource == AUDIO_SAWTOOTH))
		{
			if (bRecordProcData)
				bRecordStatus = true;
			// this case is when data to be recorded is explicitly defined as "Audio In"
			if ((!bRecordThreadEngaged) && bRecordStatus && (uiFunction2Rec == AUDIO_IN))
			{
				fProcData2Dump[0][0][i + pEar->uiRecBuffInc[0][0]] = fAudioHiResSamples[temp];
			}
			// this is when other processed data is to be recorded. Audio input data is also dumped as well to complement the proc data
			else if ((!bRecordThreadEngaged) && bRecordStatus && ((uiFunction2Rec > AUDIO_IN) && (uiFunction2Rec <= DRNL2ANsynapse_MT)))
			{
				fAudioData2Dump[i + pEar->uiAudioInRecBuffTrack] = fAudioHiResSamples[temp];
			}
		}
		
		if (bScrollPlot)	fScrollAudImgBuff[i] = fAudioInput[i] / fAudioInGain;
		else				fAudioImgBuff[i] = fAudioInput[i] / fAudioInGain;

#if OME_INPUT_CHECK
		OMEinput << fAudioInput[i] << endl;
#endif
	}

	uiAudioImgBuffWriteTracker = (uiAudioImgBuffWriteTracker + pEar->uiAudioInputWinSize) % (pEar->uiAudioInputWinSize << 2);

	if (bRecordStatus && (!bRecordThreadEngaged))
	{
		if (uiFunction2Rec == AUDIO_IN)
			pEar->uiRecBuffInc[0][0] += pEar->uiAudioInputWinSize;									// increment rec buff index by audio inp size
		else
			pEar->uiAudioInRecBuffTrack += pEar->uiAudioInputWinSize;								// increment rec buff index by audio inp size
		// only signal the thread to dump the audio data if the option has been explicitly set
		if (pEar->uiRecBuffInc[0][0] == pEar->uiNumSampToRec)
		{
			bRecordProcData = false;
			bRecordStatus = false;
			pEar->uiCurrBF2Dump = 0;
			pEar->uiRecBuffInc[0][0] = 0;									// reset rec buff increment index
			pthread_mutex_lock (&Write2FileMutex[0]);
			pthread_cond_signal (&cvFileWriteWaitonSignal[0]);
			pthread_mutex_unlock (&Write2FileMutex[0]);
		}
	}

	// reset the counter once the complete set copied
	if ((cAudioSource == AUDIO_SINE) || (cAudioSource == AUDIO_SQUARE) || (cAudioSource == AUDIO_SAWTOOTH))
		uiHiResAudioReadTracker = 0;

#if OME_INPUT_CHECK
//	OMEinput << "fMin = " << fMin << ", fMax = " << fMax << endl;
	OMEinput.close();
#endif
#if DISPLAY_PROCTIME
	dGetAudioSamplesEndTime = Time::getMillisecondCounterHiRes();
	dGetAuSampTime = dGetAudioSamplesEndTime - dGetAudioSamplesStartTime;
	dSummedGetAuSamp += dGetAuSampTime;
	dNumGetAuSampInvk++;
#endif
}

void* RTAPcompute::StartAlgoThread (void* obj)
{
	reinterpret_cast<RTAPcompute*>(obj)->ProcessFunction ();

	return 0;
}

// master function to call all other function based on user selections
void* RTAPcompute::ProcessFunction()
{
	while (bRTAPrunning)
	{
		// wait for Play button to be pressed before continuing on
		pthread_mutex_lock (&PlayPauseMutex);
		pthread_cond_wait (&cvWaitOnPlayButton, &PlayPauseMutex);
		pthread_mutex_unlock (&PlayPauseMutex);

		// process only when Play/Pause button is set, audio packets are present to be processed and DRNL() is not being processed
		if (bPlayPauseStatus && uiAudioPackets2BeProcessed)
		{
			if (bRecordProcData)
			{
				pthread_mutex_lock (&CallWriteFuncMutex);
				pthread_cond_broadcast (&cvFileWriteWaitonRecBtn);
				pthread_mutex_unlock (&CallWriteFuncMutex);
			}

			pthread_mutex_lock (&ComputeAlgoMutex);
			pthread_cond_wait (&cvComputeAlgo, &ComputeAlgoMutex);
			pthread_mutex_unlock (&ComputeAlgoMutex);

			// update the audio packets to be processed counter
			uiAudioPackets2BeProcessed--;
			getAudioSamples ();
			OMEprocess (pEar);
			
			// track the audio data window to be processed
			// used for x-axis (time) update in auto plot display refresh feature
			uiProcDataWin2Disp = uiNumAuPcktRx - 1;
			
			switch (iFunctionToRun)
			{
				case DRNL_ST_OPT:
					if (!bFunctionProcFlag)
						DRNL_SingleThreadedOpt (pEar);
					break;
				case DRNL2IHCRP_ST_OPT:
					if (!bFunctionProcFlag)
						DRNL_to_IHC_RP_SingleThreadedOpt (pEar);
					break;
				case DRNL2IHCpresynapse_ST_OPT:
					if (!bFunctionProcFlag)
						DRNL_to_IHCpresyanpse_SingleThreadedOpt (pEar);
					break;
				case DRNL2ANsynapse_ST_OPT:
					if (!bFunctionProcFlag)
						DRNL_to_AN_SingleThreadedOpt (pEar);
					break;
				case DRNL_MT:
						pEar->iBFcounter = -1;
						pthread_mutex_lock (&CallProcFuncMutex);
						pthread_cond_broadcast (&cvDRNLwaitonAudioSamp);
						pthread_mutex_unlock (&CallProcFuncMutex);
					break;
			}
		}
	}

	return 0;
}

void RTAPcompute::OMEprocess ( EarStruct* pEar )
{
#if DISPLAY_PROCTIME
	dOMEstartTime = Time::getMillisecondCounterHiRes();
#endif
#if EER_WRITE2FILE
	char EERfilename[50] = "Debug_files/EER.txt";
	ofstream EERfile;
	EERfile.open(EERfilename, ios::app);
#endif
#if TMDISP_WRITE2FILE
	char TMdispFilename[50] = "Debug_files/TMdisp.txt";
	ofstream TMdispFile;
	TMdispFile.open(TMdispFilename, ios::app);
#endif
#if STAPESDISP_WRITE2FILE
	char StapesDispFilename[50] = "Debug_files/StapesDisp.txt";
	ofstream StapesDispFile;
	StapesDispFile.open(StapesDispFilename, ios::app);
#endif

#if SINGLE_PRECISION
	const float fStapesScalar = 45e-9;
#elif DOUBLE_PRECISION
	const double fStapesScalar = 45e-9;
#endif
	UINT i, j;

	// external ear resonances computations
	pEar->pOMEarStruct->pEERfilter->uiAudioInputWinSize = pEar->uiAudioInputWinSize;
	pEar->pOMEarStruct->fInpEER = fAudioInput;
	pEar->pOMEarStruct->pEERfilter->x = pEar->pOMEarStruct->fInpEER;
	pEar->pOMEarStruct->pEERfilter->y = pEar->pOMEarStruct->fInpTMdisplacement;

	DFT2filterOpt (pEar->pOMEarStruct->pEERfilter);

	// handle window continuity flag for filter
	pEar->pOMEarStruct->pEERfilter->uiWinContinuity = 1;

	// after the 1st instance of execution, set the window continuity lock
	pEar->pOMEarStruct->pEERfilter->bLoopLock = true;

	for (i=0; i<pEar->pOMEarStruct->pEERfilter->uiAudioInputWinSize; i++)
	{
		pEar->pOMEarStruct->pEERfilter->y[i] = (pEar->pOMEarStruct->pEERfilter->y[i] * 3.16227766017) + pEar->pOMEarStruct->pEERfilter->x[i];
#if EER_WRITE2FILE
		EERfile << pEar->pOMEarStruct->pEERfilter->x[i] << "\t" << pEar->pOMEarStruct->pEERfilter->y[i] << endl;
#endif
		// this flag is redundant in this scenanrio but will come in handy from DRNL onwards where there are may be multiple channels
		if (bRecordProcData)
			bRecordStatus = true;
		// only start acquiring data for saving when record thread is inactive & either of 2 rec btns clicked
		if ((!bRecordThreadEngaged) && bRecordStatus && (uiFunction2Rec == EER_OUT))
		{
			// data to be dumped to binary file
			fProcData2Dump[0][0][i + pEar->uiRecBuffInc[0][0]] =  pEar->pOMEarStruct->pEERfilter->y[i];
		}
	}

	// tympanic membrane displacement computations
	pEar->pOMEarStruct->pTMfilter->uiAudioInputWinSize = pEar->uiAudioInputWinSize;
	pEar->pOMEarStruct->pTMfilter->x = pEar->pOMEarStruct->fInpTMdisplacement;
	pEar->pOMEarStruct->pTMfilter->y = pEar->pOMEarStruct->fInpMidEarStapesInertia;

	DFT2filterOpt (pEar->pOMEarStruct->pTMfilter);

	// set the window continuity flag after the 1st segment window is processed
	pEar->pOMEarStruct->pTMfilter->uiWinContinuity = 1;

	// set the window conitnuity flag after 1st window is executed
	pEar->pOMEarStruct->pTMfilter->bLoopLock = true;

#if TMDISP_WRITE2FILE
	for (i=0; i<pEar->pOMEarStruct->pTMfilter->uiAudioInputWinSize; i++)
		TMdispFile << pEar->pOMEarStruct->pTMfilter->x[i] << "\t" << pEar->pOMEarStruct->pTMfilter->y[i] << endl;
#endif

	// stapes displacement computations
	pEar->pOMEarStruct->pSIfilter->uiAudioInputWinSize = pEar->uiAudioInputWinSize;
	pEar->pOMEarStruct->pSIfilter->x = pEar->pOMEarStruct->fInpMidEarStapesInertia;
	pEar->pOMEarStruct->pSIfilter->y = pEar->pOMEarStruct->fStapesDisplacement;

	DFT2filterOpt (pEar->pOMEarStruct->pSIfilter);

	// set the window continuity flag after the 1st window is processed
	pEar->pOMEarStruct->pSIfilter->uiWinContinuity = 1;
	// for handling 2nd window onwards, set the LoopLock flag
	pEar->pOMEarStruct->pSIfilter->bLoopLock = true;

	for (i=0; i<pEar->pOMEarStruct->pSIfilter->uiAudioInputWinSize; i++)
	{
		pEar->pOMEarStruct->pSIfilter->y[i] = pEar->pOMEarStruct->pSIfilter->y[i] * fStapesScalar;

		// set params for DRNL execution
		for (j=0; j<pEar->uiNumBFchannels; j++)
		{
			// for nonlinear branch, input 
			pEar->fDRNLnonlinOutput[j][i] = pEar->pOMEarStruct->fStapesDisplacement[i];
			pEar->fDRNLnonlinInput[j][i] = 0;

			// handle buffer settings for odd gammatone filter order
			if (pEar->pDRNLstruct->uiFilterOrderParity)
			{
				pEar->fDRNLlinInput[j][i] = pEar->pOMEarStruct->fStapesDisplacement[i] * pEar->pDRNLstruct->fLinGain;
				pEar->fDRNLlinOutput[j][i] = 0;											// initialise to 0 1st
				// for nonlinear path, assign both inp & outp buff w input values
//				pEar->fDRNLnonlinOutput[j][i] = pEar->fDRNLnonlinInput[j][i];
			}
			else		// handle buffer settings for even gammatone filter order
			{
				pEar->fDRNLlinOutput[j][i] = pEar->pOMEarStruct->fStapesDisplacement[i] * pEar->pDRNLstruct->fLinGain;
				pEar->fDRNLlinInput[j][i] = 0;											// initialise to 0 1st
				// for nonlinear path, assign both inp & outp buff w input values
//				pEar->fDRNLnonlinOutput[j][i] = pEar->fDRNLnonlinInput[j][i];
			}
		}

		if (bRecordProcData)
			bRecordStatus = true;
		if ((!bRecordThreadEngaged) && bRecordStatus && (uiFunction2Rec == TM_DISP))
		{
			fProcData2Dump[0][0][i + pEar->uiRecBuffInc[0][0]] = pEar->pOMEarStruct->pTMfilter->y[i];
		}
		else if ((!bRecordThreadEngaged) && bRecordStatus && (uiFunction2Rec == STAPES_DISP))
		{
			fProcData2Dump[0][0][i + pEar->uiRecBuffInc[0][0]] = pEar->pOMEarStruct->pSIfilter->y[i];
		}

#if STAPESDISP_WRITE2FILE
		StapesDispFile << pEar->pOMEarStruct->pSIfilter->x[i] << "\t" << pEar->pOMEarStruct->fStapesDisplacement[i] << endl;
#endif
	}

	if (bRecordStatus && (!bRecordThreadEngaged) && ((uiFunction2Rec == EER_OUT) || (uiFunction2Rec == TM_DISP) || (uiFunction2Rec == STAPES_DISP)))
	{
		pEar->uiRecBuffInc[0][0] += pEar->uiAudioInputWinSize;										// increment rec buff index by audio inp size

		if (pEar->uiRecBuffInc[0][0] == pEar->uiNumSampToRec)
		{
			bRecordStatus = false;
			bRecordProcData = false;
			pEar->uiCurrBF2Dump = 0;
			pEar->uiRecBuffInc[0][0] = 0;
			pEar->uiAudioInRecBuffTrack = 0;
			pthread_mutex_lock ( &Write2FileMutex[0] );
			pthread_cond_signal ( &cvFileWriteWaitonSignal[0] );
			pthread_mutex_unlock ( &Write2FileMutex[0] );
		}
	}
#if EER_WRITE2FILE
	EERfile.close();
#endif
#if STAPESDISP_WRITE2FILE
	StapesDispFile.close();
#endif
#if DISPLAY_PROCTIME
	dOMEendTime = Time::getMillisecondCounterHiRes();
	dOMEtime = dOMEendTime - dOMEstartTime;
	dSummedOMEtime += dOMEtime;
	dNumOMEinvk++;
#endif
}

void RTAPcompute::DRNL_SingleThreadedOpt (EarStruct* pEar)
{
	//flag DRNL() is in use to the invoking function
	bFunctionProcFlag = true;

#if DISPLAY_PROCTIME
	dAlgoStartTime[0] = Time::getMillisecondCounterHiRes();
#endif
#if DRNL_WRITE2FILE
	char filename[50] = "Debug_files/DRNL_output.txt";
	ofstream BMfile;
	BMfile.open(filename, ios::app);
	BMfile << "In DRNL_SingleThreaded now" << endl;
#endif

#if SINGLE_PRECISION
	float temp;
#elif DOUBLE_PRECISION
	double temp;
#endif
	UINT i, j, m;

	m = 1;
	// ----------------------------------
	// -----> SINGLE-THREADED DRNL <-----
	// ----------------------------------

	for (i=0; i<pEar->uiNumBFchannels; i++)
	{
		if ((pEar->pDRNLstruct->fDRNLa <= 0) && (pEar->pDRNLstruct->fLinGain <= 0))
		{
			// neither the linear nor the nonlinear channel is active. Calling program designated this as "dead region"
		}
		else
		{
			//------------- Linear Path -----------------
			if (pEar->pDRNLstruct->fLinGain != 0)
			{
				// set audio window size (gammatone buff to be swapped for higher order filter invokation)
				pEar->pEarFilters->pLinGammatone[i].uiAudioInputWinSize = pEar->uiAudioInputWinSize;

				// 1st check filter order odd/even parity, then set input/output buffer accordingly for filter order buffer swapping
				if (pEar->pDRNLstruct->uiFilterOrderParity)
				{
					// set initial gammatone buff (to be swapped for higher order filter invokation)
					pEar->pEarFilters->pLinGammatone[i].x = pEar->fDRNLlinInput[i];
					pEar->pEarFilters->pLinGammatone[i].y = pEar->fDRNLlinOutput[i];
				}
				else
				{
					// set initial gammatone buff (to be swapped for higher order filter invokation)
					pEar->pEarFilters->pLinGammatone[i].x = pEar->fDRNLlinOutput[i];
					pEar->pEarFilters->pLinGammatone[i].y = pEar->fDRNLlinInput[i];
				}
	
				GammatoneOpt (&pEar->pEarFilters->pLinGammatone[i], pEar->pDRNLstruct->uiLinOrder);
				
				// after 1st instance of execution gammatone filter, set window continuity flag
				pEar->pEarFilters->pLinGammatone[i].bLoopLock = true;		// this is usually set in the gammatone func but check for it here just in case
				// reset incremental flag for next window processing
				pEar->pEarFilters->pLinGammatone[i].iGammatonePassStatus = -1;
				// this flag signals the end of the 1st window for processing
				pEar->pEarFilters->pLinGammatone[i].bFirstWindow = false;
			}
			//------------- Nonlinear Path -----------------
			if (pEar->pDRNLstruct->fDRNLa)
			{
				// set audio window size 
				pEar->pEarFilters->pNonlinGammatone[i].uiAudioInputWinSize = pEar->uiAudioInputWinSize;

				// 1st pass - input & output buff will be swapped in the 2nd pass so don't be confused with the settings below
				pEar->pEarFilters->pNonlinGammatone[i].x = pEar->fDRNLnonlinOutput[i];
				pEar->pEarFilters->pNonlinGammatone[i].y = pEar->fDRNLnonlinInput[i];
				GammatoneOpt(&pEar->pEarFilters->pNonlinGammatone[i], pEar->pDRNLstruct->uiNonLinOrder);

				if (pEar->pDRNLstruct->uiFilterOrderParity)		// even parity gammatone filter order buffer settings
				{
					// apply compression here
					DRNL_brokenstick_nl(pEar->fDRNLnonlinInput[i], pEar->fDRNLnonlinInput[i], pEar);

					// 2nd pass - now swap the IO buff to its right naming convention
					pEar->pEarFilters->pNonlinGammatone[i].x = pEar->fDRNLnonlinInput[i];
					pEar->pEarFilters->pNonlinGammatone[i].y = pEar->fDRNLnonlinOutput[i];
					GammatoneOpt(&pEar->pEarFilters->pNonlinGammatone[i], pEar->pDRNLstruct->uiNonLinOrder);
				}
				else											// odd parity gammatone filter order buffer settings
				{
					// for even parity gammatone filter order, apply compression to the output buffer
					DRNL_brokenstick_nl(pEar->fDRNLnonlinOutput[i], pEar->fDRNLnonlinOutput[i], pEar);

					// 2nd pass - now swap the IO buff to its right naming convention
					pEar->pEarFilters->pNonlinGammatone[i].x = pEar->fDRNLnonlinOutput[i];
					pEar->pEarFilters->pNonlinGammatone[i].y = pEar->fDRNLnonlinInput[i];
					GammatoneOpt(&pEar->pEarFilters->pNonlinGammatone[i], pEar->pDRNLstruct->uiNonLinOrder);
				}

				// after the 1st instance of execution, set the window continuity flag
				pEar->pEarFilters->pNonlinGammatone[i].bLoopLock = true;
				// reset incremental flag for the next window
				pEar->pEarFilters->pNonlinGammatone[i].iGammatonePassStatus = -1;
				// signal end of 1st window processing
				pEar->pEarFilters->pNonlinGammatone[i].bFirstWindow = false;
			}
			for (j=0; j<pEar->uiAudioInputWinSize; j++)
			{
				pEar->fDRNLresponse[i][j] = pEar->fDRNLlinOutput[i][j] + pEar->fDRNLnonlinOutput[i][j];

				// peak detector
				if (j)
				{
					if (pEar->fDRNLresponse[i][j] > pEar->fDRNLresponse[i][j-1])
					{
						uiRise++;			// increment Level-Rise counter
					}
					else if (pEar->fDRNLresponse[i][j] < pEar->fDRNLresponse[i][j-1])
					{
						if (uiRise)
						{
							// sum up peaks
							fPeakScore[i] += pEar->fDRNLresponse[i][j-1];
							// track num of peaks encountered
							uiNumofPeaks++;
							// reset Level-Rise counter
							uiRise = 0;
						}
					}
				}
				else
				{
					// reset peak detector variables
					fPeakScore[i] = 0;
					uiNumofPeaks = 0;
					uiRise = 0;
				}

				if (bScrollPlot)
				{
					pEar->fAudSigDataDisAcc = fScrollAudImgBuff[j];
					pEar->fProcDataDispAcc = pEar->fDRNLresponse[i][j];

					if (!siNumProcData2Display)
						uiImgSrcX = uiImgBuffWriteTrack[i];						// record the img buff start location where data begins to be dumped

					if (uiSubSamples == 0)
					{
						fAudioImgBuff[uiImgBuffWriteTrack[0]] = pEar->fAudSigDataDisAcc;
						fImgBuff[i][uiImgBuffWriteTrack[i]] = pEar->fProcDataDispAcc;
						siNumProcData2Display++;
						uiSubSamples = SCROLLNUMSAMP;
						pEar->fProcDataDispAcc = 0;

						// find local maxima & minima for 1st window frame
						if (fMaxPixDisplayVal < fImgBuff[i][uiImgBuffWriteTrack[i]])
							fMaxPixDisplayVal = fImgBuff[i][uiImgBuffWriteTrack[i]];
						if (fMinPixDisplayVal > fImgBuff[i][uiImgBuffWriteTrack[i]])
							fMinPixDisplayVal = fImgBuff[i][uiImgBuffWriteTrack[i]];

						// point to the next img buff offset
						uiImgBuffWriteTrack[i] = (uiImgBuffWriteTrack[i]++) % IMGTEMPSTORESIZE;
					}
					else
						uiSubSamples--;
				}
				else
				{
					if ((bRefreshPlot || bAutoRefreshPlot) && (i == 0))
					{
						bErasePlot = true;			// ensure that previous plot is erased before plotting new one
						bProcRefreshPlot = true;
					}
					if ((dNumAlgoInvk == 0) || bProcRefreshPlot)
					{
						// find local maxima & minima for current window frame
						if (fMaxPixDisplayVal < pEar->fDRNLresponse[i][j])
							fMaxPixDisplayVal = pEar->fDRNLresponse[i][j];
						if (fMinPixDisplayVal > pEar->fDRNLresponse[i][j])
							fMinPixDisplayVal = pEar->fDRNLresponse[i][j];

						RTAP_MaxMinSymmetry();

						pEar->fProcDataDispAcc+= pEar->fDRNLresponse[i][j];

						if (j == 0)
							uiImgBuffWriteTrack[i] = 0;

						if (!siNumProcData2Display)
							uiImgSrcX = uiImgBuffWriteTrack[i];						// record the img buff start location where data begins to be dumped

						// get average over "SampFreqTest" num of samples
						fImgBuff[i][uiImgBuffWriteTrack[i]] = pEar->fProcDataDispAcc;
						siNumProcData2Display++;									// size of processed data to display
	//					uiSubSamples = SampFreqTest;								// re-initialise for next pixel computation
						pEar->fProcDataDispAcc = 0;										// reset accumulator
						// point to the next img buff offset
						uiImgBuffWriteTrack[i] = (uiImgBuffWriteTrack[i]++) % IMGTEMPSTORESIZE;

						// set flag to display plot
						bProcRefreshPlot = true;
					}
				}

				// start recording from BF channel 0
				if (bRecordProcData && (i == 0))
					bRecordStatus = true;
				// only start acquiring data for saving when record thread is inactive & either of 2 rec btns clicked & from BF channel 0
				if ((!bRecordThreadEngaged) && bRecordStatus && (uiFunction2Rec == DRNL_ST_OPT))
				{
					// data to be dumped to binary file
					fProcData2Dump[i][0][j + pEar->uiRecBuffInc[i][0]] =  pEar->fDRNLresponse[i][j];
				}

#if DRNL_WRITE2FILE
				// debug mode to write DRNL response samples to a txt file for further analysis
				BMfile << pEar->pOMEarStruct->fStapesDisplacement[j] << "\t" << pEar->fDRNLlinOutput[0][j] << "\t" << pEar->fDRNLnonlinOutput[0][j] << "\t" << pEar->fDRNLresponse[0][j] << endl;
//				BMfile << fImgBuff[i][uiImgBuffWriteTrack[i]+j] << endl;
#endif
			}

			// Compute mean peak score
			fMeanPeakScore[i] = fPeakScore[i] / uiNumofPeaks;

			if (bRecordStatus && (!bRecordThreadEngaged) && (uiFunction2Rec == DRNL_ST_OPT))
				// increment rec buff index by audio inp size only if record button is clicked
				pEar->uiRecBuffInc[i][0] += pEar->uiAudioInputWinSize;
		}
	}

	// Acquire peak detector score
	for (j=0; j<3; j++)
	{
		for (i=m; i<pEar->uiNumBFchannels; i++)
		{
			if (fMeanPeakScore[j] < fMeanPeakScore[i])
			{
				temp = fMeanPeakScore[j];
				fMeanPeakScore[j] = fMeanPeakScore[i];
				fMeanPeakScore[i] = temp;
				uiMeanPeakBF[j] = i+1;
			}
		}
		fMaxPeak[j] = fMeanPeakScore[j];
		m++;
	}

	// call gfx thread to render plot
//	if (!bRenderPlotInProgress)
	{
		if (bProcRefreshPlot)
		{
			bRefreshPlot = false;
			bProcRefreshPlot = false;
			bRefreshXaxis = true;		// update X-axis time labels
		}

		// increment counting semaphore
		dAlgoProcSem++;
		pthread_mutex_lock ( &DrawPlotMutex );
		pthread_cond_signal ( &cvDrawPlot );
		pthread_mutex_unlock ( &DrawPlotMutex );
	}

	// only invoke the record thread when all the BF channels processed
	if (bRecordStatus && (!bRecordThreadEngaged) && (uiFunction2Rec == DRNL_ST_OPT))
	{
		// since relevant elements of uiRecBuffInc array hold the same value, just use the very 1st element
		if (pEar->uiRecBuffInc[0][0] ==  pEar->uiNumSampToRec)
		{
			bRecordStatus = false;
			bRecordProcData = false;
			pEar->uiCurrBF2Dump = i;
//			pEar->uiRecBuffInc = 0;
			pEar->uiAudioInRecBuffTrack = 0;
			pthread_mutex_lock ( &Write2FileMutex[0] );
			pthread_cond_signal ( &cvFileWriteWaitonSignal[0] );
			pthread_mutex_unlock ( &Write2FileMutex[0] );
		}
	}

#if DRNL_WRITE2FILE
	BMfile.close();
#endif
#if DISPLAY_PROCTIME
	dAlgoEndTime[0] = Time::getMillisecondCounterHiRes();
	dAlgoTime = dAlgoEndTime[0] - dAlgoStartTime[0];
	dSummedAlgoTime += dAlgoTime;
	dNumAlgoInvk++;
#endif

	// flag DRNL() not in use
	bFunctionProcFlag = false;
}

void RTAPcompute::DRNL_to_IHC_RP_SingleThreadedOpt ( EarStruct* pEar )
{
	//flag DRNL() is in use to the invoking function
	bFunctionProcFlag = true;

#if DISPLAY_PROCTIME
	dAlgoStartTime[0] = Time::getMillisecondCounterHiRes();
#endif
#if DRNL_WRITE2FILE
	char filename[50] = "Debug_files/DRNL_output.txt";
	ofstream BMfile;
	BMfile.open(filename, ios::app);
	BMfile << "In DRNL_SingleThreaded now" << endl;
#endif

#if SINGLE_PRECISION
	float temp;
#elif DOUBLE_PRECISION
	double temp;
#endif
	int i, j, m;

	m = 1;
	// ----------------------------------
	// -----> SINGLE-THREADED DRNL <-----
	// ----------------------------------
	for (i=0; i<pEar->uiNumBFchannels; i++)
	{
		if ((pEar->pDRNLstruct->fDRNLa <= 0) && (pEar->pDRNLstruct->fLinGain <= 0))
		{
			// neither the linear nor the nonlinear channel is active. Calling program designated this as "dead region"
		}
		else
		{
			//------------- Linear Path -----------------
			if (pEar->pDRNLstruct->fLinGain != 0)
			{
				// set audio window size (gammatone buff to be swapped for higher order filter invokation)
				pEar->pEarFilters->pLinGammatone[i].uiAudioInputWinSize = pEar->uiAudioInputWinSize;

				// 1st check filter order odd/even parity, then set input/output buffer accordingly for filter order buffer swapping
				if (pEar->pDRNLstruct->uiFilterOrderParity)
				{
					// set initial gammatone buff (to be swapped for higher order filter invokation)
					pEar->pEarFilters->pLinGammatone[i].x = pEar->fDRNLlinInput[i];
					pEar->pEarFilters->pLinGammatone[i].y = pEar->fDRNLlinOutput[i];
				}
				else
				{
					// set initial gammatone buff (to be swapped for higher order filter invokation)
					pEar->pEarFilters->pLinGammatone[i].x = pEar->fDRNLlinOutput[i];
					pEar->pEarFilters->pLinGammatone[i].y = pEar->fDRNLlinInput[i];
				}
	
				GammatoneOpt ( &pEar->pEarFilters->pLinGammatone[i], pEar->pDRNLstruct->uiLinOrder );
				
				// after 1st instance of execution gammatone filter, set window continuity flag
				pEar->pEarFilters->pLinGammatone[i].bLoopLock = true;		// this is usually set in the gammatone func but check for it here just in case
				// reset incremental flag for next window processing
				pEar->pEarFilters->pLinGammatone[i].iGammatonePassStatus = -1;
				// this flag signals the end of the 1st window for processing
				pEar->pEarFilters->pLinGammatone[i].bFirstWindow = false;

				// flag for delay settigs
//				pEar->pEarFilters->pLinGammatone[i].bBoundaryValue = false;
			}
			//------------- Nonlinear Path -----------------
			if (pEar->pDRNLstruct->fDRNLa)
			{
				// 1st pass - input & output buff will be swapped in the 2nd pass so don't be confused at with the settings below
				pEar->pEarFilters->pNonlinGammatone[i].uiAudioInputWinSize = pEar->uiAudioInputWinSize;
				pEar->pEarFilters->pNonlinGammatone[i].x = pEar->fDRNLnonlinOutput[i];
				pEar->pEarFilters->pNonlinGammatone[i].y = pEar->fDRNLnonlinInput[i];
				GammatoneOpt(&pEar->pEarFilters->pNonlinGammatone[i], pEar->pDRNLstruct->uiNonLinOrder);

				if (pEar->pDRNLstruct->uiFilterOrderParity)		// even parity gammatone filter order buffer settings
				{
					// apply compression here
					DRNL_brokenstick_nl(pEar->fDRNLnonlinInput[i], pEar->fDRNLnonlinInput[i], pEar);

					// 2nd pass - now swap the IO buff to its right naming convention
					pEar->pEarFilters->pNonlinGammatone[i].x = pEar->fDRNLnonlinInput[i];
					pEar->pEarFilters->pNonlinGammatone[i].y = pEar->fDRNLnonlinOutput[i];
					GammatoneOpt(&pEar->pEarFilters->pNonlinGammatone[i], pEar->pDRNLstruct->uiNonLinOrder);
				}
				else											// odd parity gammatone filter order buffer settings
				{
					// for even parity gammatone filter order, apply compression to the output buffer
					DRNL_brokenstick_nl(pEar->fDRNLnonlinOutput[i], pEar->fDRNLnonlinOutput[i], pEar);

					// 2nd pass - now swap the IO buff to its right naming convention
					pEar->pEarFilters->pNonlinGammatone[i].x = pEar->fDRNLnonlinOutput[i];
					pEar->pEarFilters->pNonlinGammatone[i].y = pEar->fDRNLnonlinInput[i];
					GammatoneOpt(&pEar->pEarFilters->pNonlinGammatone[i], pEar->pDRNLstruct->uiNonLinOrder);
				}

				// after the 1st instance of execution, set the window continuity flag
				pEar->pEarFilters->pNonlinGammatone[i].bLoopLock = true;
				// reset incremental flag for the next window
				pEar->pEarFilters->pNonlinGammatone[i].iGammatonePassStatus = -1;
				// signal end of 1st window processing
				pEar->pEarFilters->pNonlinGammatone[i].bFirstWindow = false;
			}
			for (j=0; j<pEar->uiAudioInputWinSize; j++)
			{
				pEar->fDRNLresponse[i][j] = pEar->fDRNLlinOutput[i][j] + pEar->fDRNLnonlinOutput[i][j];
			}
		}

		// ----- IHC receptor potential computation start -----;
		pEar->pEarFilters->pIHCciliaFilter[i].uiAudioInputWinSize = pEar->uiAudioInputWinSize;
		pEar->pEarFilters->pIHCciliaFilter[i].x = pEar->fDRNLresponse[i];					// single thread initialisation
		pEar->pEarFilters->pIHCciliaFilter[i].y = pEar->fIHCrpResponse[i];					// output IHC displacement to buffer 1st

		DFT2filterOpt(&pEar->pEarFilters->pIHCciliaFilter[i]);
		// set flag for loop locking
		pEar->pEarFilters->pIHCciliaFilter[i].bLoopLock = true;
		// set flag for window continuity
		pEar->pEarFilters->pIHCciliaFilter[i].uiWinContinuity = 1;

		for (j=0; j<pEar->uiAudioInputWinSize; j++)
		{
			pEar->pIHC_RP->fIHCciliaDisplacement[0] = pEar->fIHCrpResponse[i][j];			// temp storage for writing to file (debug purpose)
			// apply scalar before calculating apical conductance
			pEar->pIHC_RP->fIHCciliaDispScaled[0] = pEar->fIHCrpResponse[i][j] * pEar->pIHC_RP->fC;
			// calculate the apical conductance
#if MATH_OPTIMISATION
			if (bMathOpt)
			{
				pEar->pIHC_RP->fExpUtoS0 = RTAP_exp(-pEar->pIHC_RP->fIHCciliaDispScaled[0] / pEar->pIHC_RP->fS0);
				pEar->pIHC_RP->fExpUtoS1 = RTAP_exp(-pEar->pIHC_RP->fIHCciliaDispScaled[0] / pEar->pIHC_RP->fS1);
			}
			else
#endif
			{
				pEar->pIHC_RP->fExpUtoS0 = exp(-pEar->pIHC_RP->fIHCciliaDispScaled[0] / pEar->pIHC_RP->fS0);
				pEar->pIHC_RP->fExpUtoS1 = exp(-pEar->pIHC_RP->fIHCciliaDispScaled[0] / pEar->pIHC_RP->fS1);
			}
			pEar->pIHC_RP->fGu[0] = (pEar->pIHC_RP->fGmax / (1 + (pEar->pIHC_RP->fExpUtoS0 * pEar->pIHC_RP->fExpU0oS0) * (1 + (pEar->pIHC_RP->fExpUtoS1 * pEar->pIHC_RP->fExpU1oS1)))) + pEar->pIHC_RP->fGa;

			// calculate the IHC receptor potential
			pEar->pIHC_RP->fInitialV[i] = pEar->pIHC_RP->fInitialV[i] + (-pEar->pIHC_RP->fGu[0] * (pEar->pIHC_RP->fInitialV[i] - pEar->pIHC_RP->fEt) - pEar->pIHC_RP->fGk * (pEar->pIHC_RP->fInitialV[i] - pEar->pIHC_RP->fEkp)) * pEar->pIHC_RP->fIHCrpVar1;
			pEar->fIHCrpResponse[i][j] = pEar->pIHC_RP->fInitialV[i];

			// peak detector
			if (j)
			{
				if (pEar->fIHCrpResponse[i][j] > pEar->fIHCrpResponse[i][j-1])
				{
					uiRise++;			// increment Level-Rise counter
				}
				else if (pEar->fIHCrpResponse[i][j] < pEar->fIHCrpResponse[i][j-1])
				{
					if (uiRise)
					{
						// sum up peaks
						fPeakScore[i] += pEar->fIHCrpResponse[i][j-1];
						// track num of peaks encountered
						uiNumofPeaks++;
						// reset Level-Rise counter
						uiRise = 0;
					}
				}
			}
			else
			{
				// reset peak detector variables
				fPeakScore[i] = 0;
				uiNumofPeaks = 0;
				uiRise = 0;
			}

			if (bScrollPlot)
			{
				pEar->fAudSigDataDisAcc = fScrollAudImgBuff[j];
				pEar->fProcDataDispAcc = pEar->fIHCrpResponse[i][j];

				if (!siNumProcData2Display)
					uiImgSrcX = uiImgBuffWriteTrack[i];						// record the img buff start location where data begins to be dumped

				if (uiSubSamples == 0)
				{
					fAudioImgBuff[uiImgBuffWriteTrack[0]] = pEar->fAudSigDataDisAcc;
					fImgBuff[i][uiImgBuffWriteTrack[i]] = pEar->fProcDataDispAcc; // SCROLLNUMSAMP;
					siNumProcData2Display++;
					uiSubSamples = SCROLLNUMSAMP;
					pEar->fProcDataDispAcc = 0;
#if 0
					if (i == 655)
						uiGlobalShow6 = i;
#endif
					if (fImgBuff[i][uiImgBuffWriteTrack[i]] == 0.f)
						fImgBuff[i][uiImgBuffWriteTrack[i]] = pEar->pIHC_RP->fRestingPotential;

					// find local maxima & minima for 1st window frame
					if (fMaxPixDisplayVal < fImgBuff[i][uiImgBuffWriteTrack[i]])
						fMaxPixDisplayVal = fImgBuff[i][uiImgBuffWriteTrack[i]];
					if (fMinPixDisplayVal > fImgBuff[i][uiImgBuffWriteTrack[i]])
						fMinPixDisplayVal = fImgBuff[i][uiImgBuffWriteTrack[i]];

					RTAP_MaxMinSymmetry();

					// point to the next img buff offset
					uiImgBuffWriteTrack[i] = (uiImgBuffWriteTrack[i]++) % IMGTEMPSTORESIZE;
				}
				else
					uiSubSamples--;

			}
			else
			{
				if ((bRefreshPlot || bAutoRefreshPlot) && (i == 0))
				{
					bErasePlot = true;			// ensure that previous plot is erased before plotting new one
					bProcRefreshPlot = true;
				}
				if ((dNumAlgoInvk == 0) || bProcRefreshPlot)
				{
					// find local maxima & minima for 1st window frame
					if (fMaxPixDisplayVal < pEar->fIHCrpResponse[i][j])
						fMaxPixDisplayVal = pEar->fIHCrpResponse[i][j];
					if (fMinPixDisplayVal > pEar->fIHCrpResponse[i][j])
						fMinPixDisplayVal = pEar->fIHCrpResponse[i][j];

					RTAP_MaxMinSymmetry();

					pEar->fProcDataDispAcc += pEar->fIHCrpResponse[i][j];

					if (j == 0)
						uiImgBuffWriteTrack[i] = 0;

					if (!siNumProcData2Display)
						uiImgSrcX = uiImgBuffWriteTrack[i];						// record the img buff start location where data begins to be dumped

					// get average over "SampFreqTest" num of samples
					fImgBuff[i][uiImgBuffWriteTrack[i]] = pEar->fProcDataDispAcc;
					siNumProcData2Display++;									// size of processed data to display
	//				uiSubSamples = SampFreqTest;								// re-initialise for next pixel computation
					pEar->fProcDataDispAcc = 0;										// reset accumulator
					uiImgBuffWriteTrack[i] = (uiImgBuffWriteTrack[i]++) % IMGTEMPSTORESIZE;

					bProcRefreshPlot = true;
				}
			}

			// start recording from BF channel 0
			if (bRecordProcData && (i == 0))
				bRecordStatus = true;
			// only start acquiring data for saving when record thread is inactive & either of 2 rec btns clicked & from BF channel 0
			if (bRecordStatus && (!bRecordThreadEngaged) && (uiFunction2Rec == DRNL2IHCRP_ST_OPT))
			{
					// data to be dumped to binary file
					fProcData2Dump[i][0][j + pEar->uiRecBuffInc[i][0]] =  pEar->fIHCrpResponse[i][j];
			}
		}
		// Compute mean peak score
		fMeanPeakScore[i] = fPeakScore[i] / uiNumofPeaks;
			
		if (bRecordStatus && (!bRecordThreadEngaged) && (uiFunction2Rec == DRNL2IHCRP_ST_OPT))
			// increment rec buff index by audio inp size
			pEar->uiRecBuffInc[i][0] += pEar->uiAudioInputWinSize;
	}

	// Acquire peak detector score
	for (j=0; j<3; j++)
	{
		for (i=m; i<pEar->uiNumBFchannels; i++)
		{
			if (fMeanPeakScore[j] < fMeanPeakScore[i])
			{
				temp = fMeanPeakScore[j];
				fMeanPeakScore[j] = fMeanPeakScore[i];
				fMeanPeakScore[i] = temp;
				uiMeanPeakBF[j] = i+1;
			}
		}
		fMaxPeak[j] = fMeanPeakScore[j];
		m++;
	}

	// call gfx thread to render plot
//	if (!bRenderPlotInProgress)
	{
		if ( bProcRefreshPlot )
		{
			bRefreshPlot = false;
			bProcRefreshPlot = false;
			bRefreshXaxis = true;		// update X-axis time labels
		}
		// increment counting semaphore
		dAlgoProcSem++;
		pthread_mutex_lock ( &DrawPlotMutex );
		pthread_cond_signal ( &cvDrawPlot );
		pthread_mutex_unlock ( &DrawPlotMutex );
	}

	// check if conditions check out for IHCRP data recording
	if (bRecordStatus && (!bRecordThreadEngaged) && (uiFunction2Rec == DRNL2IHCRP_ST_OPT))
	{
		if (pEar->uiRecBuffInc[0][0] == pEar->uiNumSampToRec)
		{
			bRecordStatus = false;
			bRecordProcData = false;
			pEar->uiCurrBF2Dump = i;
//			pEar->uiRecBuffInc = 0;
			pEar->uiAudioInRecBuffTrack = 0;
			pthread_mutex_lock (&Write2FileMutex[0]);
			pthread_cond_signal (&cvFileWriteWaitonSignal[0]);
			pthread_mutex_unlock (&Write2FileMutex[0]);
		}
	}

#if DRNL_WRITE2FILE
	BMfile.close();
#endif
#if DISPLAY_PROCTIME
	dAlgoEndTime[0] = Time::getMillisecondCounterHiRes();
	dAlgoTime = dAlgoEndTime[0] - dAlgoStartTime[0];
	dSummedAlgoTime += dAlgoTime;
	dNumAlgoInvk++;
#endif

	// flag DRNL() not in use
	bFunctionProcFlag = false;
}

void RTAPcompute::DRNL_to_IHCpresyanpse_SingleThreadedOpt ( EarStruct* pEar )
{
	//flag DRNL() is in use to the invoking function
	bFunctionProcFlag = true;

#if DISPLAY_PROCTIME
	dAlgoStartTime[0] = Time::getMillisecondCounterHiRes();
#endif
#if DRNL_WRITE2FILE
	char filename[50] = "Debug_files/DRNL_output.txt";
	ofstream BMfile;
	BMfile.open(filename, ios::app);
	BMfile << "In DRNL_SingleThreaded now" << endl;
#endif

#if SINGLE_PRECISION
	float temp;
#elif DOUBLE_PRECISION
	double temp;
#endif
	int i, j, k, m;

	m = 1;
	// ----------------------------------
	// -----> SINGLE-THREADED DRNL <-----
	// ----------------------------------

	for (i=0; i<pEar->uiNumBFchannels; i++)
	{
		if ((pEar->pDRNLstruct->fDRNLa <= 0) && (pEar->pDRNLstruct->fLinGain <= 0))
		{
			// neither the linear nor the nonlinear channel is active. Calling program designated this as "dead region"
		}
		else
		{
			//------------- Linear Path -----------------
			if (pEar->pDRNLstruct->fLinGain != 0)
			{
				// set audio window size (gammatone buff to be swapped for higher order filter invokation)
				pEar->pEarFilters->pLinGammatone[i].uiAudioInputWinSize = pEar->uiAudioInputWinSize;

				// 1st check filter order odd/even parity, then set input/output buffer accordingly for filter order buffer swapping
				if (pEar->pDRNLstruct->uiFilterOrderParity)
				{
					// set initial gammatone buff (to be swapped for higher order filter invokation)
					pEar->pEarFilters->pLinGammatone[i].x = pEar->fDRNLlinInput[i];
					pEar->pEarFilters->pLinGammatone[i].y = pEar->fDRNLlinOutput[i];
				}
				else
				{
					// set initial gammatone buff (to be swapped for higher order filter invokation)
					pEar->pEarFilters->pLinGammatone[i].x = pEar->fDRNLlinOutput[i];
					pEar->pEarFilters->pLinGammatone[i].y = pEar->fDRNLlinInput[i];
				}
	
				GammatoneOpt ( &pEar->pEarFilters->pLinGammatone[i], pEar->pDRNLstruct->uiLinOrder );
				
				// after 1st instance of execution gammatone filter, set window continuity flag
				pEar->pEarFilters->pLinGammatone[i].bLoopLock = true;		// this is usually set in the gammatone func but check for it here just in case
				// reset incremental flag for next window processing
				pEar->pEarFilters->pLinGammatone[i].iGammatonePassStatus = -1;
				// this flag signals the end of the 1st window for processing
				pEar->pEarFilters->pLinGammatone[i].bFirstWindow = false;

				// flag for delay settigs
//				pEar->pEarFilters->pLinGammatone[i].bBoundaryValue = false;
			}
			//------------- Nonlinear Path -----------------
			if (pEar->pDRNLstruct->fDRNLa)
			{
				// 1st pass - input & output buff will be swapped in the 2nd pass so don't be confused at with the settings below
				pEar->pEarFilters->pNonlinGammatone[i].uiAudioInputWinSize = pEar->uiAudioInputWinSize;
				pEar->pEarFilters->pNonlinGammatone[i].x = pEar->fDRNLnonlinOutput[i];
				pEar->pEarFilters->pNonlinGammatone[i].y = pEar->fDRNLnonlinInput[i];
				GammatoneOpt(&pEar->pEarFilters->pNonlinGammatone[i], pEar->pDRNLstruct->uiNonLinOrder);

				if (pEar->pDRNLstruct->uiFilterOrderParity)		// even parity gammatone filter order buffer settings
				{
					// apply compression here
					DRNL_brokenstick_nl(pEar->fDRNLnonlinInput[i], pEar->fDRNLnonlinInput[i], pEar);

					// 2nd pass - now swap the IO buff to its right naming convention
					pEar->pEarFilters->pNonlinGammatone[i].x = pEar->fDRNLnonlinInput[i];
					pEar->pEarFilters->pNonlinGammatone[i].y = pEar->fDRNLnonlinOutput[i];
					GammatoneOpt(&pEar->pEarFilters->pNonlinGammatone[i], pEar->pDRNLstruct->uiNonLinOrder);
				}
				else											// odd parity gammatone filter order buffer settings
				{
					// for even parity gammatone filter order, apply compression to the output buffer
					DRNL_brokenstick_nl(pEar->fDRNLnonlinOutput[i], pEar->fDRNLnonlinOutput[i], pEar);

					// 2nd pass - now swap the IO buff to its right naming convention
					pEar->pEarFilters->pNonlinGammatone[i].x = pEar->fDRNLnonlinOutput[i];
					pEar->pEarFilters->pNonlinGammatone[i].y = pEar->fDRNLnonlinInput[i];
					GammatoneOpt(&pEar->pEarFilters->pNonlinGammatone[i], pEar->pDRNLstruct->uiNonLinOrder);
				}

				// after the 1st instance of execution, set the window continuity flag
				pEar->pEarFilters->pNonlinGammatone[i].bLoopLock = true;
				// reset incremental flag for the next window
				pEar->pEarFilters->pNonlinGammatone[i].iGammatonePassStatus = -1;
				// signal end of 1st window processing
				pEar->pEarFilters->pNonlinGammatone[i].bFirstWindow = false;
			}
			for (j=0; j<pEar->uiAudioInputWinSize; j++)
			{
				pEar->fDRNLresponse[i][j] = pEar->fDRNLlinOutput[i][j] + pEar->fDRNLnonlinOutput[i][j];
			}
		}

		// ----- IHC receptor potential computation start -----;
		pEar->pEarFilters->pIHCciliaFilter[i].uiAudioInputWinSize = pEar->uiAudioInputWinSize;
		pEar->pEarFilters->pIHCciliaFilter[i].x = pEar->fDRNLresponse[i];					// single thread initialisation
		pEar->pEarFilters->pIHCciliaFilter[i].y = pEar->fIHCrpResponse[i];		// output IHC displacement to buffer 1st

		DFT2filterOpt(&pEar->pEarFilters->pIHCciliaFilter[i]);

		// set flag for loop locking
		pEar->pEarFilters->pIHCciliaFilter[i].bLoopLock = true;
		// set flag for window continuity
		pEar->pEarFilters->pIHCciliaFilter[i].uiWinContinuity = 1;

		for (j=0; j<pEar->uiAudioInputWinSize; j++)
		{
			pEar->pIHC_RP->fIHCciliaDisplacement[0] = pEar->fIHCrpResponse[i][j];			// temp storage for writing to file (debug purpose)
			// apply scalar before calculating apical conductance
			pEar->pIHC_RP->fIHCciliaDispScaled[0] = pEar->fIHCrpResponse[i][j] * pEar->pIHC_RP->fC;
			// calculate the apical conductance
#if MATH_OPTIMISATION
			if (bMathOpt)
			{
				pEar->pIHC_RP->fExpUtoS0 = RTAP_exp(-pEar->pIHC_RP->fIHCciliaDispScaled[0] / pEar->pIHC_RP->fS0);
				pEar->pIHC_RP->fExpUtoS1 = RTAP_exp(-pEar->pIHC_RP->fIHCciliaDispScaled[0] / pEar->pIHC_RP->fS1);
			}
			else
#endif
			{
				pEar->pIHC_RP->fExpUtoS0 = exp(-pEar->pIHC_RP->fIHCciliaDispScaled[0] / pEar->pIHC_RP->fS0);
				pEar->pIHC_RP->fExpUtoS1 = exp(-pEar->pIHC_RP->fIHCciliaDispScaled[0] / pEar->pIHC_RP->fS1);
			}
			pEar->pIHC_RP->fGu[0] = (pEar->pIHC_RP->fGmax / (1 + (pEar->pIHC_RP->fExpUtoS0 * pEar->pIHC_RP->fExpU0oS0) * (1 + (pEar->pIHC_RP->fExpUtoS1 * pEar->pIHC_RP->fExpU1oS1)))) + pEar->pIHC_RP->fGa;

			// calculate the IHC receptor potential
			pEar->pIHC_RP->fInitialV[i] = pEar->pIHC_RP->fInitialV[i] + (-pEar->pIHC_RP->fGu[0] * (pEar->pIHC_RP->fInitialV[i] - pEar->pIHC_RP->fEt) - pEar->pIHC_RP->fGk * (pEar->pIHC_RP->fInitialV[i] - pEar->pIHC_RP->fEkp)) * pEar->pIHC_RP->fIHCrpVar1;
			pEar->fIHCrpResponse[i][j] = pEar->pIHC_RP->fInitialV[i];

			for (k=0; k<pEar->uiNumANfibreTypes; k++)			// uiNumANchannels = uiNumANfibreTypes * uiNumBFchannels
			{
				// initialise for AN sysnapse firing
				pEar->pAN_IHCsynapse->fANProbNotFiring[k][j] = 1.0f;

				// ----- IHC presynapse computation start -----
				// plug variables into formulae
#if MATH_OPTIMISATION
				if (bMathOpt)
					pEar->pIHC_presynapse->fmICaINF[0] = 1 / (1 + RTAP_exp(-pEar->pIHC_presynapse->fGamma * pEar->fIHCrpResponse[i][j]) / pEar->pIHC_presynapse->fBeta);
				else
#endif
					pEar->pIHC_presynapse->fmICaINF[0] = 1 / (1 + exp(-pEar->pIHC_presynapse->fGamma * pEar->fIHCrpResponse[i][j]) / pEar->pIHC_presynapse->fBeta);
				pEar->pIHC_presynapse->fmICaCurrent[i][k] = pEar->pIHC_presynapse->fmICaCurrent[i][k] + (pEar->pIHC_presynapse->fmICaINF[0] - pEar->pIHC_presynapse->fmICaCurrent[i][k]) * pEar->pIHC_presynapse->fIHCpresynapseVar1;
				// replace pow function with multiply function (much faster - 0.5ms shaved off)
				pEar->pIHC_presynapse->fExponent[0] = pEar->pIHC_presynapse->fmICaCurrent[i][k] * pEar->pIHC_presynapse->fmICaCurrent[i][k] * pEar->pIHC_presynapse->fmICaCurrent[i][k];
				pEar->pIHC_presynapse->fICa[0] = pEar->pIHC_presynapse->fGmaxCa * pEar->pIHC_presynapse->fExponent[0] * (pEar->fIHCrpResponse[i][j] - pEar->pIHC_presynapse->fECa);
				pEar->pIHC_presynapse->fCaConcentration[i][k] = pEar->pIHC_presynapse->fCaConcentration[i][k] + (pEar->pIHC_presynapse->fICa[0] * pEar->fDt) - (pEar->pIHC_presynapse->fCaConcentration[i][k] * pEar->fDt / pEar->pIHC_presynapse->fTauCa[k]);
				pEar->pIHC_presynapse->fExponent[0] = -pEar->pIHC_presynapse->fCaConcentration[i][k];
//				for (m=0; m<pEar->pIHC_presynapse->fPower-1; m++)
//					pEar->pIHC_presynapse->fExponent[0] *= -pEar->pIHC_presynapse->fCaConcentration[k];
#if SINGLE_PRECISION
				pEar->fVesicleReleaseRate[k][j] = (float)(pEar->pIHC_presynapse->dwZ * pEar->pIHC_presynapse->fExponent[0] * pEar->pIHC_presynapse->fExponent[0] * pEar->pIHC_presynapse->fExponent[0]);
#elif DOUBLE_PRECISION
				pEar->fVesicleReleaseRate[k][j] = (double)(pEar->pIHC_presynapse->dwZ * pEar->pIHC_presynapse->fExponent[0] * pEar->pIHC_presynapse->fExponent[0] * pEar->pIHC_presynapse->fExponent[0]);
#endif
				// peak detector
				if ((pEar->uiNumANfibreTypes == 1) || ((pEar->uiNumANfibreTypes == 2) && (k == 1)))
				{
					if (j)
					{
						if (pEar->fVesicleReleaseRate[k][j] > pEar->fVesicleReleaseRate[k][j-1])
						{
							uiRise++;			// increment Level-Rise counter
						}
						else if (pEar->fVesicleReleaseRate[k][j] < pEar->fVesicleReleaseRate[k][j-1])
						{
							if (uiRise)
							{
								// sum up peaks
								fPeakScore[i] += pEar->fVesicleReleaseRate[k][j-1];
								// track num of peaks encountered
								uiNumofPeaks++;
								// reset Level-Rise counter
								uiRise = 0;
							}
						}
					}
					else
					{
						// reset peak detector variables
						fPeakScore[i] = 0;
						uiNumofPeaks = 0;
						uiRise = 0;
					}
				}

				if (bScrollPlot)
				{
					if ((pEar->uiNumANfibreTypes == 1) || ((pEar->uiNumANfibreTypes == 2) && (k == 1)))
					{
						pEar->fAudSigDataDisAcc = fScrollAudImgBuff[j];
						pEar->fProcDataDispAcc = pEar->fVesicleReleaseRate[k][j];

						if (!siNumProcData2Display)
							uiImgSrcX = uiImgBuffWriteTrack[i];						// record the img buff start location where data begins to be dumped

						if (uiSubSamples == 0)
						{
							fAudioImgBuff[uiImgBuffWriteTrack[0]] = pEar->fAudSigDataDisAcc;
							fImgBuff[i][uiImgBuffWriteTrack[i]] = pEar->fProcDataDispAcc; // SCROLLNUMSAMP;
							siNumProcData2Display++;
							uiSubSamples = SCROLLNUMSAMP;
							pEar->fProcDataDispAcc = 0;

							// find local maxima & minima for 1st window frame
							if (fMaxPixDisplayVal < fImgBuff[i][uiImgBuffWriteTrack[i]])
								fMaxPixDisplayVal = fImgBuff[i][uiImgBuffWriteTrack[i]];
							if (fMinPixDisplayVal > fImgBuff[i][uiImgBuffWriteTrack[i]])
								fMinPixDisplayVal = fImgBuff[i][uiImgBuffWriteTrack[i]];

							RTAP_MaxMinSymmetry();

							// point to the next img buff offset
							uiImgBuffWriteTrack[i] = (uiImgBuffWriteTrack[i]++) % IMGTEMPSTORESIZE;
						}
						else
							uiSubSamples--;
					}
				}
				else
				{
					if ((bRefreshPlot || bAutoRefreshPlot) && (i == 0))
					{
						bErasePlot = true;			// ensure that previous plot is erased before plotting new one
						bProcRefreshPlot = true;
					}
					// display only for 1st time frame or whenever 'Display Plot' button
					if ((dNumAlgoInvk == 0) || bProcRefreshPlot)
					{
						if ((pEar->uiNumANfibreTypes == 1) || ((pEar->uiNumANfibreTypes == 2) && (k == 1)))
						{
							// find local maxima & minima for 1st window frame
							if (fMaxPixDisplayVal < pEar->fVesicleReleaseRate[k][j])
								fMaxPixDisplayVal = pEar->fVesicleReleaseRate[k][j];
							if (fMinPixDisplayVal > pEar->fVesicleReleaseRate[k][j])
								fMinPixDisplayVal = pEar->fVesicleReleaseRate[k][j];

							RTAP_MaxMinSymmetry();

							pEar->fProcDataDispAcc += pEar->fVesicleReleaseRate[k][j];

							if (j == 0)
								uiImgBuffWriteTrack[i] = 0;

							if (!siNumProcData2Display)
								uiImgSrcX = uiImgBuffWriteTrack[i];						// record the img buff start location where data begins to be dumped

							// get average over "SampFreqTest" num of samples
							fImgBuff[i][uiImgBuffWriteTrack[i]] = pEar->fProcDataDispAcc;
							siNumProcData2Display++;									// size of processed data to display
							pEar->fProcDataDispAcc = 0;									// reset accumulator
							uiImgBuffWriteTrack[i] = (uiImgBuffWriteTrack[i]++) % IMGTEMPSTORESIZE;

							bProcRefreshPlot = true;
						}
					}
				}

				// start recording from BF channel 0
				if (bRecordProcData && (i == 0))
					bRecordStatus = true;
				// only start acquiring data for saving when record thread is inactive & either of 2 rec btns clicked & from BF channel 0
				if ( bRecordStatus && (!bRecordThreadEngaged) && (uiFunction2Rec == DRNL2IHCpresynapse_ST_OPT))
				{
					// data to be dumped to binary file
					fProcData2Dump[i][k][j + pEar->uiRecBuffInc[i][k]] =  pEar->fVesicleReleaseRate[k][j];
				}
			}
		}
		// Compute mean peak score
		fMeanPeakScore[i] = fPeakScore[i] / uiNumofPeaks;

		if (bRecordStatus && (!bRecordThreadEngaged) && (uiFunction2Rec == DRNL2IHCpresynapse_ST_OPT))
		{
			// increment rec buff index by audio inp size
			pEar->uiRecBuffInc[i][0] += pEar->uiAudioInputWinSize;
			if (pEar->uiNumANfibreTypes == 2)
				pEar->uiRecBuffInc[i][1] += pEar->uiAudioInputWinSize;
		}
	}

	// Acquire peak detector score
	for (j=0; j<3; j++)
	{
		for (i=m; i<pEar->uiNumBFchannels; i++)
		{
			if (fMeanPeakScore[j] < fMeanPeakScore[i])
			{
				temp = fMeanPeakScore[j];
				fMeanPeakScore[j] = fMeanPeakScore[i];
				fMeanPeakScore[i] = temp;
				uiMeanPeakBF[j] = i+1;
			}
		}
		fMaxPeak[j] = fMeanPeakScore[j];
		m++;
	}

	// call gfx thread to render plot
//	if (!bRenderPlotInProgress)
	{
		if ( bProcRefreshPlot )
		{
			bRefreshPlot = false;
			bProcRefreshPlot = false;
			bRefreshXaxis = true;		// update X-axis time labels
		}
		// increment counting semaphore
		dAlgoProcSem++;
		pthread_mutex_lock ( &DrawPlotMutex );
		pthread_cond_signal ( &cvDrawPlot );
		pthread_mutex_unlock ( &DrawPlotMutex );
	}

	// check to see if record NRR btn is clicked and that the record thread is not currently recording b4 calling the record thread
	if (bRecordStatus && (!bRecordThreadEngaged) && (uiFunction2Rec == DRNL2IHCpresynapse_ST_OPT))
	{
		if (pEar->uiRecBuffInc[0][0] == pEar->uiNumSampToRec)
		{
			bRecordStatus = false;
			bRecordProcData = false;
			pEar->uiCurrBF2Dump = i;
//			pEar->uiRecBuffInc = 0;
			pEar->uiAudioInRecBuffTrack = 0;
			pthread_mutex_lock ( &Write2FileMutex[0] );
			pthread_cond_signal ( &cvFileWriteWaitonSignal[0] );
			pthread_mutex_unlock ( &Write2FileMutex[0] );
		}
	}

#if DRNL_WRITE2FILE
	BMfile.close();
#endif
#if DISPLAY_PROCTIME
	dAlgoEndTime[0] = Time::getMillisecondCounterHiRes();
	dAlgoTime = dAlgoEndTime[0] - dAlgoStartTime[0];
	dSummedAlgoTime += dAlgoTime;
	dNumAlgoInvk++;
#endif

	// flag DRNL() not in use
	bFunctionProcFlag = false;
}

void RTAPcompute::DRNL_to_AN_SingleThreadedOpt ( EarStruct* pEar )
{
	//flag DRNL() is in use to the invoking function
	bFunctionProcFlag = true;

#if DISPLAY_PROCTIME
	dAlgoStartTime[0] = Time::getMillisecondCounterHiRes();
#endif
#if DRNL_WRITE2FILE
	char filename[50] = "Debug_files/DRNL_output.txt";
	ofstream BMfile;
	BMfile.open(filename, ios::app);
	BMfile << "In DRNL_SingleThreaded now" << endl;
#endif

#if SINGLE_PRECISION
	float temp;
#elif DOUBLE_PRECISION
	double temp;
#endif
	int i, j, k, m;

	m = 1;
	// ----------------------------------
	// -----> SINGLE-THREADED DRNL <-----
	// ----------------------------------

	for (i=0; i<pEar->uiNumBFchannels; i++)
	{
		if ((pEar->pDRNLstruct->fDRNLa <= 0) && (pEar->pDRNLstruct->fLinGain <= 0))
		{
			// neither the linear nor the nonlinear channel is active. Calling program designated this as "dead region"
		}
		else
		{
			//------------- Linear Path -----------------
			if (pEar->pDRNLstruct->fLinGain != 0)
			{
				// set audio window size (gammatone buff to be swapped for higher order filter invokation)
				pEar->pEarFilters->pLinGammatone[i].uiAudioInputWinSize = pEar->uiAudioInputWinSize;

				// 1st check filter order odd/even parity, then set input/output buffer accordingly for filter order buffer swapping
				if (pEar->pDRNLstruct->uiFilterOrderParity)
				{
					// set initial gammatone buff (to be swapped for higher order filter invokation)
					pEar->pEarFilters->pLinGammatone[i].x = pEar->fDRNLlinInput[i];
					pEar->pEarFilters->pLinGammatone[i].y = pEar->fDRNLlinOutput[i];
				}
				else
				{
					// set initial gammatone buff (to be swapped for higher order filter invokation)
					pEar->pEarFilters->pLinGammatone[i].x = pEar->fDRNLlinOutput[i];
					pEar->pEarFilters->pLinGammatone[i].y = pEar->fDRNLlinInput[i];
				}
	
				GammatoneOpt ( &pEar->pEarFilters->pLinGammatone[i], pEar->pDRNLstruct->uiLinOrder );
				
				// after 1st instance of execution gammatone filter, set window continuity flag
				pEar->pEarFilters->pLinGammatone[i].bLoopLock = true;		// this is usually set in the gammatone func but check for it here just in case
				// reset incremental flag for next window processing
				pEar->pEarFilters->pLinGammatone[i].iGammatonePassStatus = -1;
				// this flag signals the end of the 1st window for processing
				pEar->pEarFilters->pLinGammatone[i].bFirstWindow = false;

				// flag for delay settigs
//				pEar->pEarFilters->pLinGammatone[i].bBoundaryValue = false;
			}
			//------------- Nonlinear Path -----------------
			if (pEar->pDRNLstruct->fDRNLa)
			{
				// 1st pass - input & output buff will be swapped in the 2nd pass so don't be confused at with the settings below
				pEar->pEarFilters->pNonlinGammatone[i].uiAudioInputWinSize = pEar->uiAudioInputWinSize;
				pEar->pEarFilters->pNonlinGammatone[i].x = pEar->fDRNLnonlinOutput[i];
				pEar->pEarFilters->pNonlinGammatone[i].y = pEar->fDRNLnonlinInput[i];
				GammatoneOpt(&pEar->pEarFilters->pNonlinGammatone[i], pEar->pDRNLstruct->uiNonLinOrder);

				if (pEar->pDRNLstruct->uiFilterOrderParity)		// even parity gammatone filter order buffer settings
				{
					// apply compression here
					DRNL_brokenstick_nl(pEar->fDRNLnonlinInput[i], pEar->fDRNLnonlinInput[i], pEar);

					// 2nd pass - now swap the IO buff to its right naming convention
					pEar->pEarFilters->pNonlinGammatone[i].x = pEar->fDRNLnonlinInput[i];
					pEar->pEarFilters->pNonlinGammatone[i].y = pEar->fDRNLnonlinOutput[i];
					GammatoneOpt(&pEar->pEarFilters->pNonlinGammatone[i], pEar->pDRNLstruct->uiNonLinOrder);
				}
				else											// odd parity gammatone filter order buffer settings
				{
					// for even parity gammatone filter order, apply compression to the output buffer
					DRNL_brokenstick_nl(pEar->fDRNLnonlinOutput[i], pEar->fDRNLnonlinOutput[i], pEar);

					// 2nd pass - now swap the IO buff to its right naming convention
					pEar->pEarFilters->pNonlinGammatone[i].x = pEar->fDRNLnonlinOutput[i];
					pEar->pEarFilters->pNonlinGammatone[i].y = pEar->fDRNLnonlinInput[i];
					GammatoneOpt(&pEar->pEarFilters->pNonlinGammatone[i], pEar->pDRNLstruct->uiNonLinOrder);
				}

				// after the 1st instance of execution, set the window continuity flag
				pEar->pEarFilters->pNonlinGammatone[i].bLoopLock = true;
				// reset incremental flag for the next window
				pEar->pEarFilters->pNonlinGammatone[i].iGammatonePassStatus = -1;
				// signal end of 1st window processing
				pEar->pEarFilters->pNonlinGammatone[i].bFirstWindow = false;
			}
			for (j=0; j<pEar->uiAudioInputWinSize; j++)
			{
				pEar->fDRNLresponse[i][j] = pEar->fDRNLlinOutput[i][j] + pEar->fDRNLnonlinOutput[i][j];
			}
		}

		// ----- IHC receptor potential computation start -----;
		pEar->pEarFilters->pIHCciliaFilter[i].uiAudioInputWinSize = pEar->uiAudioInputWinSize;
		pEar->pEarFilters->pIHCciliaFilter[i].x = pEar->fDRNLresponse[i];					// single thread initialisation
		pEar->pEarFilters->pIHCciliaFilter[i].y = pEar->fIHCrpResponse[i];		// output IHC displacement to buffer 1st

		DFT2filterOpt(&pEar->pEarFilters->pIHCciliaFilter[i]);

		// set flag for loop locking
		pEar->pEarFilters->pIHCciliaFilter[i].bLoopLock = true;
		// set flag for window continuity
		pEar->pEarFilters->pIHCciliaFilter[i].uiWinContinuity = 1;

		for (j=0; j<pEar->uiAudioInputWinSize; j++)
		{
			pEar->pIHC_RP->fIHCciliaDisplacement[0] = pEar->fIHCrpResponse[i][j];			// temp storage for writing to file (debug purpose)
			// apply scalar before calculating apical conductance
			pEar->pIHC_RP->fIHCciliaDispScaled[0] = pEar->fIHCrpResponse[i][j] * pEar->pIHC_RP->fC;
			
			// calculate the apical conductance
#if MATH_OPTIMISATION
			if (bMathOpt)
			{
				pEar->pIHC_RP->fExpUtoS0 = RTAP_exp(-pEar->pIHC_RP->fIHCciliaDispScaled[0] / pEar->pIHC_RP->fS0);
				pEar->pIHC_RP->fExpUtoS1 = RTAP_exp(-pEar->pIHC_RP->fIHCciliaDispScaled[0] / pEar->pIHC_RP->fS1);
			}
			else
#endif
			{
				pEar->pIHC_RP->fExpUtoS0 = exp(-pEar->pIHC_RP->fIHCciliaDispScaled[0] / pEar->pIHC_RP->fS0);
				pEar->pIHC_RP->fExpUtoS1 = exp(-pEar->pIHC_RP->fIHCciliaDispScaled[0] / pEar->pIHC_RP->fS1);
			}
			pEar->pIHC_RP->fGu[0] = (pEar->pIHC_RP->fGmax / (1 + (pEar->pIHC_RP->fExpUtoS0 * pEar->pIHC_RP->fExpU0oS0) * (1 + (pEar->pIHC_RP->fExpUtoS1 * pEar->pIHC_RP->fExpU1oS1)))) + pEar->pIHC_RP->fGa;

			// calculate the IHC receptor potential
			pEar->pIHC_RP->fInitialV[i] = pEar->pIHC_RP->fInitialV[i] + (-pEar->pIHC_RP->fGu[0] * (pEar->pIHC_RP->fInitialV[i] - pEar->pIHC_RP->fEt) - pEar->pIHC_RP->fGk * (pEar->pIHC_RP->fInitialV[i] - pEar->pIHC_RP->fEkp)) * pEar->pIHC_RP->fIHCrpVar1;
			pEar->fIHCrpResponse[i][j] = pEar->pIHC_RP->fInitialV[i];

			for (k=0; k<pEar->uiNumANfibreTypes; k++)			// uiNumANchannels = uiNumANfibreTypes * uiNumBFchannels
			{
				// initialise for AN sysnapse firing
				pEar->pAN_IHCsynapse->fANProbNotFiring[k][j] = 1.0f;

				// ----- IHC presynapse computation start -----
				// plug variables into formulae
#if MATH_OPTIMISATION
				if (bMathOpt)
					pEar->pIHC_presynapse->fmICaINF[0] = 1 / (1 + RTAP_exp(-pEar->pIHC_presynapse->fGamma * pEar->fIHCrpResponse[i][j]) / pEar->pIHC_presynapse->fBeta);
				else
#endif
					pEar->pIHC_presynapse->fmICaINF[0] = 1 / (1 + exp(-pEar->pIHC_presynapse->fGamma * pEar->fIHCrpResponse[i][j]) / pEar->pIHC_presynapse->fBeta);
				pEar->pIHC_presynapse->fmICaCurrent[i][k] = pEar->pIHC_presynapse->fmICaCurrent[i][k] + (pEar->pIHC_presynapse->fmICaINF[0] - pEar->pIHC_presynapse->fmICaCurrent[i][k]) * pEar->pIHC_presynapse->fIHCpresynapseVar1;
				// replace pow function with multiply f[0]tion (much faster - 0.5ms shaved off)
				pEar->pIHC_presynapse->fExponent[0] = pEar->pIHC_presynapse->fmICaCurrent[i][k] * pEar->pIHC_presynapse->fmICaCurrent[i][k] * pEar->pIHC_presynapse->fmICaCurrent[i][k];
				pEar->pIHC_presynapse->fICa[0] = pEar->pIHC_presynapse->fGmaxCa * pEar->pIHC_presynapse->fExponent[0] * (pEar->fIHCrpResponse[i][j] - pEar->pIHC_presynapse->fECa);
				pEar->pIHC_presynapse->fCaConcentration[i][k] = pEar->pIHC_presynapse->fCaConcentration[i][k] + (pEar->pIHC_presynapse->fICa[0] * pEar->fDt) - (pEar->pIHC_presynapse->fCaConcentration[i][k] * pEar->fDt / pEar->pIHC_presynapse->fTauCa[k]);
				pEar->pIHC_presynapse->fExponent[0] = -pEar->pIHC_presynapse->fCaConcentration[i][k];
//				for (m=0; m<pEar->pIHC_presynapse->fPower-1; m++)
//					pEar->pIHC_presynapse->fExponent[0] *= -pEar->pIHC_presynapse->fCaConcentration[k];
#if SINGLE_PRECISION
				pEar->fVesicleReleaseRate[k][j] = (float)(pEar->pIHC_presynapse->dwZ * pEar->pIHC_presynapse->fExponent[0] * pEar->pIHC_presynapse->fExponent[0] * pEar->pIHC_presynapse->fExponent[0]);
#elif DOUBLE_PRECISION
				pEar->fVesicleReleaseRate[k][j] = (double)(pEar->pIHC_presynapse->dwZ * pEar->pIHC_presynapse->fExponent[0] * pEar->pIHC_presynapse->fExponent[0] * pEar->pIHC_presynapse->fExponent[0]);
#endif

				// ----- AN probability spiking computation start -----
				pEar->pAN_IHCsynapse->fM_Pq[k] = pEar->pAN_IHCsynapse->fM - pEar->pAN_IHCsynapse->fProbAvailable[i][k];
				if (pEar->pAN_IHCsynapse->fM_Pq[k] < 0)
					pEar->pAN_IHCsynapse->fM_Pq[k] = 0;
				pEar->pAN_IHCsynapse->fProbReplenish[k] = pEar->pAN_IHCsynapse->fM_Pq[k] * pEar->pAN_IHCsynapse->fProbAN_ydt[k];
				pEar->pAN_IHCsynapse->fProbEjected[k] = pEar->pAN_IHCsynapse->fProbAvailable[i][k] * pEar->fVesicleReleaseRate[k][j] * pEar->fDt;
				pEar->pAN_IHCsynapse->fProbReprocessed[k] = pEar->pAN_IHCsynapse->fM_Pq[k] * pEar->pAN_IHCsynapse->fProbReprocess[i][k] * pEar->pAN_IHCsynapse->fProbAN_xdt[k];
				pEar->pAN_IHCsynapse->fReuptakeAndLost[k] = pEar->pAN_IHCsynapse->fProbAN_rdtldt[k] * pEar->pAN_IHCsynapse->fProbCleft[i][k];
				pEar->pAN_IHCsynapse->fReuptake[k] = pEar->pAN_IHCsynapse->fProbAN_rdt[k] * pEar->pAN_IHCsynapse->fProbCleft[i][k];
				pEar->pAN_IHCsynapse->fProbAvailable[i][k] = pEar->pAN_IHCsynapse->fProbAvailable[i][k] + pEar->pAN_IHCsynapse->fProbReplenish[k] - pEar->pAN_IHCsynapse->fProbEjected[k] + pEar->pAN_IHCsynapse->fProbReprocessed[k];
				if (pEar->pAN_IHCsynapse->fProbAvailable[i][k] < 0)
					pEar->pAN_IHCsynapse->fProbAvailable[i][k] = 0;		// cap min value to 0
				pEar->pAN_IHCsynapse->fProbCleft[i][k] = pEar->pAN_IHCsynapse->fProbCleft[i][k] + pEar->pAN_IHCsynapse->fProbEjected[k] - pEar->pAN_IHCsynapse->fReuptakeAndLost[k];
				pEar->pAN_IHCsynapse->fProbReprocess[i][k] = pEar->pAN_IHCsynapse->fProbReprocess[i][k] + pEar->pAN_IHCsynapse->fReuptake[k] - pEar->pAN_IHCsynapse->fProbReprocessed[k];

#if 0
				if ((i == 1) && (k == 1) && (j == 44))
				{
#if SINGLE_PRECISION
					float temp1, temp2, temp3;
#elif DOUBLE_PRECISION
					double temp1, temp2, temp3;
#endif

					temp1 = pEar->fANprobOutputRate[k][j];
					temp2 = pEar->pAN_IHCsynapse->fANProbNotFiring[k][j-1];
					temp3 = pEar->pAN_IHCsynapse->fPrevANprobOutputRate[i][k][0];
				}
#endif

				// Calculate the AN probability output rate and also apply refractory effect
				if ((!j) && (pEar->pAN_IHCsynapse->bFirstWindow[i][k]))
				{
					// for all starting values of input, prev value, i-1, will result in mem access violation so set fMin to 0
					pEar->fANprobOutputRate[k][j] = pEar->pAN_IHCsynapse->fProbEjected[k] / pEar->fDt;
				}
				// from 2nd window onwards, handle prev value of ANprob of not firing
				else if ((!j) && (!pEar->pAN_IHCsynapse->bFirstWindow[i][k]))
				{
					pEar->fANprobOutputRate[k][j] = pEar->pAN_IHCsynapse->fProbEjected[k] * pEar->pAN_IHCsynapse->fPrevANprobNotFiring[i][k] / pEar->fDt;
				}
				else
				{
					// get minimum value for ejected vesicles and prepare it to be stored as AN output (based on prev value of ANnotProbFire)
					pEar->fANprobOutputRate[k][j] = pEar->pAN_IHCsynapse->fProbEjected[k] * pEar->pAN_IHCsynapse->fANProbNotFiring[k][j-1] / pEar->fDt;
				}

				// Update the refractory matrix once the input index has moved past approx 50
				// This adds recent & removes distant probabilities so as to reduce the fraction of firing events during the refractory period
				if ((j > pEar->pAN_IHCsynapse->uiRefrac) || ((!pEar->pAN_IHCsynapse->bFirstWindow[i][k]) && (j > pEar->pAN_IHCsynapse->uiRefrac)))
				{
					// all other cases with the sample number above refrac variable boundary
					pEar->pAN_IHCsynapse->fANProbNotFiring[k][j] = pEar->pAN_IHCsynapse->fANProbNotFiring[k][j-1] * (1 - pEar->fANprobOutputRate[k][j] * pEar->fDt) / (1 - pEar->fANprobOutputRate[k][j-pEar->pAN_IHCsynapse->uiRefrac-1] * pEar->fDt);
				}
				else if ((!pEar->pAN_IHCsynapse->bFirstWindow[i][k]) && (!j))
				{
					// 2nd window onwards with the 1st sample relying on the prev window ANprobNotFiring param
					pEar->pAN_IHCsynapse->fANProbNotFiring[k][j] = pEar->pAN_IHCsynapse->fPrevANprobNotFiring[i][k] * (1 - pEar->fANprobOutputRate[k][j] * pEar->fDt) / (1 - pEar->pAN_IHCsynapse->fPrevANprobOutputRate[i][k][j] * pEar->fDt);
				}
				else if ((!pEar->pAN_IHCsynapse->bFirstWindow[i][k]) && (j > 0) && (j <= pEar->pAN_IHCsynapse->uiRefrac))
				{
					// 2nd window with samples below or equal to the refrac variable boundary
					pEar->pAN_IHCsynapse->fANProbNotFiring[k][j] = pEar->pAN_IHCsynapse->fANProbNotFiring[k][j-1] * (1 - pEar->fANprobOutputRate[k][j] * pEar->fDt) / (1 - pEar->pAN_IHCsynapse->fPrevANprobOutputRate[i][k][j] * pEar->fDt);
				}

				// save states
				if (j >= (pEar->uiAudioInputWinSize - pEar->pAN_IHCsynapse->uiRefrac - 1))
					pEar->pAN_IHCsynapse->fPrevANprobOutputRate[i][k][j-(pEar->uiAudioInputWinSize-pEar->pAN_IHCsynapse->uiRefrac-1)] = pEar->fANprobOutputRate[k][j];
				if (j == (pEar->uiAudioInputWinSize - 1))
					pEar->pAN_IHCsynapse->fPrevANprobNotFiring[i][k] = pEar->pAN_IHCsynapse->fANProbNotFiring[k][j];

				// peak detector
				if ((pEar->uiNumANfibreTypes == 1) || ((pEar->uiNumANfibreTypes == 2) && (k == 1)))
				{
					if (j)
					{
						if (pEar->fANprobOutputRate[k][j] > pEar->fANprobOutputRate[k][j-1])
						{
							uiRise++;			// increment Level-Rise counter
						}
						else if (pEar->fANprobOutputRate[k][j] < pEar->fANprobOutputRate[k][j-1])
						{
							if (uiRise)
							{
								// sum up peaks
								fPeakScore[i] += pEar->fANprobOutputRate[k][j-1];
								// track num of peaks encountered
								uiNumofPeaks++;
								// reset Level-Rise counter
								uiRise = 0;
							}
						}
					}
					else
					{
						// reset peak detector variables
						fPeakScore[i] = 0;
						uiNumofPeaks = 0;
						uiRise = 0;
					}
				}

				if (bScrollPlot)
				{
					if ((pEar->uiNumANfibreTypes == 1) || ((pEar->uiNumANfibreTypes == 2) && (k == 1)))
					{
						pEar->fAudSigDataDisAcc = fScrollAudImgBuff[j];
						pEar->fProcDataDispAcc = pEar->fANprobOutputRate[k][j];

						if (!siNumProcData2Display)
							uiImgSrcX = uiImgBuffWriteTrack[i];						// record the img buff start location where data begins to be dumped

						if (uiSubSamples == 0)
						{
							fAudioImgBuff[uiImgBuffWriteTrack[0]] = pEar->fAudSigDataDisAcc;
							fImgBuff[i][uiImgBuffWriteTrack[i]] = pEar->fProcDataDispAcc; // SCROLLNUMSAMP;
							siNumProcData2Display++;
							uiSubSamples = SCROLLNUMSAMP;
							pEar->fProcDataDispAcc = 0;

							// find local maxima & minima for 1st window frame
							if (fMaxPixDisplayVal < fImgBuff[i][uiImgBuffWriteTrack[i]])
								fMaxPixDisplayVal = fImgBuff[i][uiImgBuffWriteTrack[i]];
							if (fMinPixDisplayVal > fImgBuff[i][uiImgBuffWriteTrack[i]])
								fMinPixDisplayVal = fImgBuff[i][uiImgBuffWriteTrack[i]];

							RTAP_MaxMinSymmetry();

							// point to the next img buff offset
							uiImgBuffWriteTrack[i] = (uiImgBuffWriteTrack[i]++) % IMGTEMPSTORESIZE;
						}
						else
							uiSubSamples--;
					}
				}
				else
				{
					if ((bRefreshPlot || bAutoRefreshPlot) && (i == 0))
					{
						bErasePlot = true;			// ensure that previous plot is erased before plotting new one
						bProcRefreshPlot = true;
					}
					if ((dNumAlgoInvk == 0) || bProcRefreshPlot)
					{
						if ((pEar->uiNumANfibreTypes == 1) || ((pEar->uiNumANfibreTypes == 2) && (k == 1)))
						{
							// find local maxima & minima for 1st window frame
							if (fMaxPixDisplayVal < pEar->fANprobOutputRate[k][j])
								fMaxPixDisplayVal = pEar->fANprobOutputRate[k][j];
							if (fMinPixDisplayVal > pEar->fANprobOutputRate[k][j])
								fMinPixDisplayVal = pEar->fANprobOutputRate[k][j];

							RTAP_MaxMinSymmetry();

							pEar->fProcDataDispAcc += pEar->fANprobOutputRate[k][j];

							if (j == 0)
								uiImgBuffWriteTrack[i] = 0;

							if (!siNumProcData2Display)
								uiImgSrcX = uiImgBuffWriteTrack[i];						// record the img buff start location where data begins to be dumped

							// get average over "SampFreqTest" num of samples
							fImgBuff[i][uiImgBuffWriteTrack[i]] = pEar->fProcDataDispAcc;
							siNumProcData2Display++;									// size of processed data to display
							pEar->fProcDataDispAcc = 0;										// reset accumulator
							uiImgBuffWriteTrack[i] = (uiImgBuffWriteTrack[i]++) % IMGTEMPSTORESIZE;

							bProcRefreshPlot = true;
						}
					}
				}

				if (bRecordProcData && (i == 0))
					bRecordStatus = true;
				if (bRecordStatus && (!bRecordThreadEngaged) && (uiFunction2Rec == DRNL2ANsynapse_ST_OPT))
				{
					// CAUTION: This code does not cater for both LSR & HSR yet. Only 1 of it can be recorded for now ...
					// data to be dumped to binary file
					fProcData2Dump[i][k][j + pEar->uiRecBuffInc[i][k]] =  pEar->fANprobOutputRate[k][j];
				}
			}
		}
		// Compute mean peak score
		fMeanPeakScore[i] = fPeakScore[i] / uiNumofPeaks;

		pEar->pAN_IHCsynapse->bFirstWindow[i][0] = false;
		pEar->pAN_IHCsynapse->bFirstWindow[i][1] = false;
		if (bRecordStatus && (!bRecordThreadEngaged) && (uiFunction2Rec == DRNL2ANsynapse_ST_OPT))
		{
			pEar->uiRecBuffInc[i][0] += pEar->uiAudioInputWinSize;										// increment rec buff index by audio inp size
			if (pEar->uiNumANfibreTypes == 2)
				pEar->uiRecBuffInc[i][1] += pEar->uiAudioInputWinSize;
		}
	}

	// Acquire peak detector score
	for (j=0; j<3; j++)
	{
		for (i=m; i<pEar->uiNumBFchannels; i++)
		{
			if (fMeanPeakScore[j] < fMeanPeakScore[i])
			{
				temp = fMeanPeakScore[j];
				fMeanPeakScore[j] = fMeanPeakScore[i];
				fMeanPeakScore[i] = temp;
				uiMeanPeakBF[j] = i+1;
			}
		}
		fMaxPeak[j] = fMeanPeakScore[j];
		m++;
	}

	// call gfx thread to render plot
//	if (!bRenderPlotInProgress)
	{
		if ( bProcRefreshPlot )
		{
			bRefreshPlot = false;
			bProcRefreshPlot = false;
			bRefreshXaxis = true;		// update X-axis time labels
		}
		// increment counting semaphore
		dAlgoProcSem++;
		pthread_mutex_lock ( &DrawPlotMutex );
		pthread_cond_signal ( &cvDrawPlot );
		pthread_mutex_unlock ( &DrawPlotMutex );
	}

	// invoke record thread if conditions check out
	if (bRecordStatus && (!bRecordThreadEngaged) && (uiFunction2Rec == DRNL2ANsynapse_ST_OPT))
	{
		if (pEar->uiRecBuffInc[0][0] == pEar->uiNumSampToRec)
		{
			bRecordStatus = false;
			bRecordProcData = false;
			pEar->uiCurrBF2Dump = i;
//			pEar->uiRecBuffInc = 0;
			pEar->uiAudioInRecBuffTrack = 0;
			pthread_mutex_lock ( &Write2FileMutex[0] );
			pthread_cond_signal ( &cvFileWriteWaitonSignal[0] );
			pthread_mutex_unlock ( &Write2FileMutex[0] );
		}
	}

#if DRNL_WRITE2FILE
	BMfile.close();
#endif
#if DISPLAY_PROCTIME
	dAlgoEndTime[0] = Time::getMillisecondCounterHiRes();
	dAlgoTime = dAlgoEndTime[0] - dAlgoStartTime[0];
	dSummedAlgoTime += dAlgoTime;
	dNumAlgoInvk++;
#endif

	// flag DRNL() not in use
	bFunctionProcFlag = false;
}

void* RTAPcompute::StartRecThreads ( void* obj )
{
	reinterpret_cast<RTAPcompute*>(obj)->ProcDataRecordThread();

	return 0;
}

void* RTAPcompute::ProcDataRecordThread ()
{
	unsigned int i, j;
	FILE* pRTAPbinFile;
	bool bHeaderWritten = false;

#if RECORD_MAXVAL
	ofstream DumpFile;
	DumpFile.open ("Recorded_Data/ProcessedData.txt");

	((EarStruct *)pEar)->iFileWriterCounter++;
	int iFileWriterThread = ((EarStruct *)pEar)->iFileWriterCounter;
	cFileWriterThreaduiStatus[iFileWriterThread] = READY_TO_RUN;

	pthread_mutex_lock( &CallWriteFuncMutex );
	pthread_cond_wait ( &cvFileWriteWaitonRecBtn, &CallWriteFuncMutex );
	pthread_mutex_unlock( &CallWriteFuncMutex );

	DumpFile << "BF Channel (Hz):\t" << fAudioLvldBSPL << " dB" << endl;

	while (bRecordProcData  && bPlayPauseStatus/* & ((EarStruct *)pEar)->bComputationDone[iFileWriterThread]*/)
	{
		// wait for DRNL to signal that processed data packets are ready
		pthread_mutex_lock ( &Write2FileMutex[iFileWriterThread] );
		pthread_cond_wait ( &cvFileWriteWaitonSignal[iFileWriterThread], &Write2FileMutex[iFileWriterThread] );
		pthread_mutex_unlock ( &Write2FileMutex[iFileWriterThread] );

		cFileWriterThreaduiStatus[iFileWriterThread] = RUNNING;

		// log start time - file writer thread
		dWrite2FileStartTime = Time::getMillisecondCounterHiRes();

		// ensure that the right thread invoked to dump processed data
		((EarStruct *)pEar)->iFileWriterCounter++;
		iFileWriterThread = ((EarStruct *)pEar)->iFileWriterCounter;

		for (i=0; i<((EarStruct *)pEar)->uiNumBFchannels; i++)
			DumpFile << ((EarStruct *)pEar)->pDRNLstruct->fBFlist[i] << "\t" << ((EarStruct *)pEar)->fProcData2Sav1[i] <<
			"\t" << ((EarStruct *)pEar)->fProcData2Sav2[i] << "\t" << ((EarStruct *)pEar)->fProcData2Sav3[i] << endl;
#if 0
		for (i=0; i<((EarStruct *)pEar)->uiAudioInputWinSize; i++)
			DumpFile << ((EarStruct *)pEar)->fProcData2Sav1[i] << endl;
#endif

		((EarStruct *)pEar)->bComputationDone[0] = false;
		((EarStruct *)pEar)->bComputationDone[1] = false;

		// log end time - file writer thread
		dWrite2FileEndTime = Time::getMillisecondCounterHiRes();
	}

	DumpFile.close();

	cFileWriterThreaduiStatus[iFileWriterThread] = IDLE;
#elif USE_WRITE
	int fh;

	errno_t err = _sopen_s ( &fh, "Datadump.rtap", _O_CREAT | _O_RDWR, SH_DENYNO, _S_IREAD | _S_IWRITE );
	
	pthread_mutex_lock (&CallWriteFuncMutex);
	pthread_cond_wait (&cvFileWriteWaitonRecBtn, &CallWriteFuncMutex);
	pthread_mutex_unlock (&CallWriteFuncMutex);

	_write (fh, FileHeader, SIZEOFHEADER);

	while (bRecordProcData)
	{
		// wait for DRNL to signal that processed data packets are ready
		pthread_mutex_lock (&Write2FileMutex[0]);
		pthread_cond_wait (&cvFileWriteWaitonSignal[0], &Write2FileMutex[0]);
		pthread_mutex_unlock (&Write2FileMutex[0]);

		// log start time - file writer thread
		dWrite2FileStartTime = Time::getMillisecondCounterHiRes ();
		
		bRecordThreadEngaged = true;

		_write (fh, fProcData2Dump[((EarStruct *)pEar)->uiCurrBF2Dump], ((EarStruct *)pEar)->uiAudioInputWinSize);
//		_write (fh, FileData, ((EarStruct *)pEar)->uiAudioInputWinSize<<2);
#if 0
		for (i=0; i<pEar->uiAudioInputWinSize; i++)
		{
			_write (fh, &fProcData2Dump[((EarStruct *)pEar)->uiCurrBF2Dump][i], 4);
		}
#endif

		bRecordThreadEngaged = false;

		// log end time - file writer thread
		dWrite2FileEndTime = Time::getMillisecondCounterHiRes();
	}
		
	_close ( fh );
#elif USE_FWRITE
	pRTAPbinFile = fopen ("Datadump.brtap", "wb");

	// wait for 'Record' or 'Play+Record' button to be clicked
	pthread_mutex_lock (&CallWriteFuncMutex);
	pthread_cond_wait (&cvFileWriteWaitonRecBtn, &CallWriteFuncMutex);
	pthread_mutex_unlock (&CallWriteFuncMutex);

	// write binary file header into binary file first
	fwrite (FileHeader, 1, SIZEOFHEADER, pRTAPbinFile);
	fseek (pRTAPbinFile, SIZEOFHEADER, SEEK_SET);

	while (bRTAPrunning)
	{
		// wait for algorithm function to signal that has processed 1 time of data packets
		pthread_mutex_lock (&Write2FileMutex[0]);
		pthread_cond_wait (&cvFileWriteWaitonSignal[0], &Write2FileMutex[0]);
		pthread_mutex_unlock (&Write2FileMutex[0]);

		// log start time - file writer thread
		dWrite2FileStartTime = Time::getMillisecondCounterHiRes ();
		
		bRecordThreadEngaged = true;

		if (uiFunction2Rec != AUDIO_IN)
			fwrite (fAudioData2Dump, 4, ((EarStruct *)pEar)->uiNumSampToRec, pRTAPbinFile);

		for (i=0; i<((EarStruct *)pEar)->uiNumBFchannels; i++)
		{
			for (j=0; j<((EarStruct *)pEar)->uiNumANfibreTypes; j++)
			{
				fwrite (fProcData2Dump[i][j], 4, ((EarStruct *)pEar)->uiNumSampToRec, pRTAPbinFile);
				// reset incremental index to 0
				pEar->uiRecBuffInc[i][j] = 0;
			}
		}

		bRecordThreadEngaged = false;

		// log end time - file writer thread
		dWrite2FileEndTime = Time::getMillisecondCounterHiRes();
	}
	fclose (pRTAPbinFile);
#elif JUCE_FILELOGGER
	File fBinLog (File::getCurrentWorkingDirectory().getChildFile("RTAPdataLog.brtap"));
	
	// delete any existing binary file to avoid appending data at the end of file
	fBinLog.deleteFile();
	// create new data file
	fBinLog.create();
	FileOutputStream brtapWriteStream (fBinLog);

	if (brtapWriteStream.openedOk())
	{
		strcpy_s (cFileStatus, "Output stream created successfully");

		// wait for 'Record' or 'Play+Record' button to be clicked
		pthread_mutex_lock (&CallWriteFuncMutex);
		pthread_cond_wait (&cvFileWriteWaitonRecBtn, &CallWriteFuncMutex);
		pthread_mutex_unlock (&CallWriteFuncMutex);

		if (!bHeaderWritten)
		{
			brtapWriteStream.write (FileHeader, SIZEOFHEADER);
			brtapWriteStream.flush ();
			bHeaderWritten = true;
		}

		while (bRTAPrunning)
		{
			// wait for algorithm function to signal that has processed 1 time of data packets
			pthread_mutex_lock (&Write2FileMutex[0]);
			pthread_cond_wait (&cvFileWriteWaitonSignal[0], &Write2FileMutex[0]);
			pthread_mutex_unlock (&Write2FileMutex[0]);

			// log start time - file writer thread
			dWrite2FileStartTime = Time::getMillisecondCounterHiRes ();

			bRecordThreadEngaged = true;

			if (uiFunction2Rec != AUDIO_IN)
				brtapWriteStream.write (fAudioData2Dump, pEar->uiNumSampToRec);//((EarStruct *)pEar)->uiNumSampToRec);

			for (i=0; i<((EarStruct *)pEar)->uiNumBFchannels; i++)
			{
				for (j=0; j<((EarStruct *)pEar)->uiNumANfibreTypes; j++)
				{
					brtapWriteStream.write (fProcData2Dump[i][j], pEar->uiNumSampToRec);//((EarStruct *)pEar)->uiNumSampToRec);
						
					// reset incremental index to 0
					pEar->uiRecBuffInc[i][j] = 0;
				}
			}

			brtapWriteStream.flush ();

			bRecordThreadEngaged = false;

			// log end time - file writer thread
			dWrite2FileEndTime = Time::getMillisecondCounterHiRes();

		}
	}
	else	strcpy_s (cFileStatus, "Unable to create output stream");


#endif
	return 0;
}

#if SINGLE_PRECISION
void RTAPcompute::DRNL_brokenstick_nl ( float* fOutp, float* fInp, EarStruct* pEar )
#elif DOUBLE_PRECISION
void RTAPcompute::DRNL_brokenstick_nl ( double* fOutp, double* fInp, EarStruct* pEar )
#endif
{
	UINT i;
	SINT siSign;
#if SINGLE_PRECISION
	float fAbsInp, fExp;
#elif DOUBLE_PRECISION
	double fAbsInp, fExp;
#endif

	for (i=0; i<pEar->uiAudioInputWinSize; i++)
	{
		// find absolute of input values
//		fAbsInp = RTAP_abs ( fInp[i] );
		if (fInp[i] > 0)
			siSign = 1;
		else if (fInp[i] < 0)
			siSign = -1;
		else
			siSign = 0;

		fAbsInp = siSign * fInp[i];

		if ((fAbsInp) && (fAbsInp > pEar->pDRNLstruct->fCompThreshScaled))
		{
			// non-linear compression function of the DRNL
			fExp = log(pEar->pDRNLstruct->fDBSvar1 * fAbsInp);
#if MATH_OPTIMISATION
			if (bMathOpt)
				fOutp[i] = siSign * pEar->pDRNLstruct->fCompThreshBM * RTAP_exp(pEar->pDRNLstruct->fDRNLc * fExp);
			else
#endif
				fOutp[i] = siSign * pEar->pDRNLstruct->fCompThreshBM * exp(pEar->pDRNLstruct->fDRNLc * fExp);
		}
		else
			// linear function of the DRNL
			fOutp[i] = pEar->pDRNLstruct->fDRNLa * fInp[i];
	}
}

void RTAPcompute::GammatoneOpt ( strFilterParams* pEarFiltersParams, UINT n )
{
#if GAMMATONE_WRITE2FILE
	ofstream gammatonefile;
	gammatonefile.open("Debug_files/Gammatone_response.txt", ios::app);
#endif
#if GAMMATONE_MEASURE_PERFORMANCE
	clock_t tGammatoneStart, tGammatoneEnd;
	double dAudioDevIOCallbackProcTime;

	tGammatoneStart = clock();
#endif
#if SINGLE_PRECISION
	float* fGammatoneIOBuff;
#elif DOUBLE_PRECISION
	double* fGammatoneIOBuff;
#endif
	UINT uiOrder;

	for (uiOrder=0; uiOrder<n; uiOrder++)
	{
		pEarFiltersParams->iGammatonePassStatus++;

		// do the swap only after the 1st instance of the for loop completed
		if (uiOrder)
		{
			// use temporary pointer to hold 1 of the buffer contents as they are swapped
  			fGammatoneIOBuff = pEarFiltersParams->x;
			pEarFiltersParams->x = pEarFiltersParams->y;
			pEarFiltersParams->y = fGammatoneIOBuff;
			// only account for window continuity once 1st window is processed
			if (pEarFiltersParams->bFirstWindow == false)
				pEarFiltersParams->bLoopLock = true;
			else
				pEarFiltersParams->bLoopLock = false;
		}	

		// invoke the digital filter
		DFT2filterOpt (pEarFiltersParams);
#if GAMMATONE_WRITE2FILE
		UINT i;

		gammatonefile << "Call " << uiOrder << endl;
		for (i=0; i<pEarFiltersParams->uiAudioInputWinSize; i++)
		{
			gammatonefile << pEarFiltersParams->x[i] << "\t" << pEarFiltersParams->y[i] << endl;
		}
#endif

	}

#if GAMMATONE_MEASURE_PERFORMANCE
	tGammatoneEnd = clock();
	dAudioDevIOCallbackProcTime = ((double)(tGammatoneEnd - tGammatoneStart))/CLOCKS_PER_SEC;
	cout << "Gammatone filter processing time: " << dAudioDevIOCallbackProcTime << "sec" << endl;
#if GAMMATONE_WRITE2FILE
	gammatonefile << "Gammatone filter processing time: " << dAudioDevIOCallbackProcTime << "sec" << endl;
#endif
#endif
#if GAMMATONE_WRITE2FILE
	gammatonefile.close();
#endif
}

// direct form 2 transposed filter translated from Matlab
void RTAPcompute::DFT2filterOpt ( strFilterParams* pFilterParams )
{
	// sample indices
	UINT i, j, uiFirstLoopCap, uiLoopLockNumCap1, uiLoopLockNumCap2, uiLoopLockDenCap1, uiLoopLockDenCap2, uiCarryForward, uiZfCap;
#if SINGLE_PRECISION
	float temp;
#elif DOUBLE_PRECISION
	double temp;
#endif

	// cater for filter order > length(input) as this leads to memory access violation in the 1st loop
	// e.g. length(b) = 11, length(a) = 1, length(x) =10, then x[9] = valid, y[9] = valid, x[10] = INVALID!
	if ((pFilterParams->uiNumOrder > pFilterParams->uiAudioInputWinSize) || (pFilterParams->uiDenOrder > pFilterParams->uiAudioInputWinSize))
	{
		uiFirstLoopCap = pFilterParams->uiAudioInputWinSize;
		// check for high order filter loops
		if (pFilterParams->bLoopLock)
		{
			// ensure separate checks for num & den
			if (pFilterParams->uiNumOrder > pFilterParams->uiAudioInputWinSize)
				uiLoopLockNumCap2 = pFilterParams->uiAudioInputWinSize;
			else
				uiLoopLockNumCap2 = pFilterParams->uiNumOrder - 1;
			if (pFilterParams->uiDenOrder > pFilterParams->uiAudioInputWinSize)
				uiLoopLockDenCap2 = pFilterParams->uiAudioInputWinSize;
			else
				uiLoopLockDenCap2 = pFilterParams->uiDenOrder - 1;
		}
	}
	else if ((pFilterParams->uiNumOrder < pFilterParams->uiAudioInputWinSize) && (pFilterParams->uiNumOrder > pFilterParams->uiDenOrder))
	{
		uiFirstLoopCap = pFilterParams->uiNumOrder;
		uiLoopLockNumCap2 = pFilterParams->uiNumOrder - 1;
		uiLoopLockDenCap2 = pFilterParams->uiDenOrder - 1;
	}
	else if ((pFilterParams->uiDenOrder < pFilterParams->uiAudioInputWinSize) && (pFilterParams->uiDenOrder > pFilterParams->uiNumOrder))
	{
		uiFirstLoopCap = pFilterParams->uiDenOrder-1;
		uiLoopLockNumCap2 = pFilterParams->uiNumOrder - 1;
		uiLoopLockDenCap2 = pFilterParams->uiDenOrder - 1;
	}
	else
	{
		uiFirstLoopCap = pFilterParams->uiNumOrder;
		uiLoopLockNumCap2 = pFilterParams->uiNumOrder - 1;
		uiLoopLockDenCap2 = pFilterParams->uiDenOrder - 1;
	}

	// general variables for high order filter loops
	uiCarryForward = 0;
	uiLoopLockNumCap1 = 1;				// must be at least 1 to start off
	uiLoopLockDenCap1 = 0;				// must be 0 to cater for non return path for a[0]
	uiLoopLockNumCap2 = uiLoopLockDenCap2 = 0;

#if FILTER_WRITE2FILE
	ofstream filterfile;
	filterfile.open("Debug_files/Filter_response.txt", ios::app);
#endif

	// if there is no filter loop lock, continue with process startup for filter() as usual
	if (!pFilterParams->bLoopLock)
	{
		// Initial step is to perform y[0]=b[0]*x[0]. Loop will take care of y[1] onwards
		pFilterParams->y[0] = (pFilterParams->b[0] * pFilterParams->x[0] / pFilterParams->a[0]) + pFilterParams->zi[0];

		for (i=1; i<uiFirstLoopCap; i++)
		{
			pFilterParams->y[i] = 0.0;			// initialise y[i] to 0 1st
			if (i < pFilterParams->uiNumOrder)
				// use the increment, i to set the for loop boundary based on b nodes
				temp = i + 1;
			else
				// if i has increased to length(b) or more then cap upcoming for loop boundary to uiNumOrder
				temp = pFilterParams->uiNumOrder;

			for (j=0; j<temp; j++)
		   		// Handle the numerator 1str with eqn: y[i]=y[i]+b[j]*x[i-j];
				pFilterParams->y[i] = pFilterParams->y[i] + (pFilterParams->b[j] * pFilterParams->x[i-j]);

			if (i < pFilterParams->uiDenOrder)
				temp = i;
			else
				temp = pFilterParams->uiDenOrder - 1;

			for (j=0; j<temp; j++)
				pFilterParams->y[i] = pFilterParams->y[i] - (pFilterParams->a[j+1] * pFilterParams->y[i-j-1]);

			// make sure that the a[0] is taken into account
			pFilterParams->y[i] = pFilterParams->y[i] / pFilterParams->a[0];
		}
	}
	else
	{
		for (i=0; i<uiFirstLoopCap; i++)
		{
			pFilterParams->y[i]=0.0f;

			if (uiLoopLockNumCap1)
			{
				for (j=0; j<uiLoopLockNumCap1; j++)
				{
					pFilterParams->y[i] = pFilterParams->y[i] + (pFilterParams->b[j] * pFilterParams->x[i-j]);
					uiCarryForward = j;
				}
			}
			if (uiLoopLockNumCap1 < pFilterParams->uiNumOrder)
				uiLoopLockNumCap1++;

			if (uiLoopLockNumCap2)
			{
				for (j=0; j<uiLoopLockNumCap2; j++)
				{
					// apply window continuity option here
					if (pFilterParams->uiWinContinuity == 0)
					{
						// ensure that incremental flag is set accordingly for different passes of gammatone filter
						if ((pFilterParams->iGammatonePassStatus == 0) && (!pFilterParams->bFirstWindow))
							pFilterParams->y[i] = pFilterParams->y[i] + (pFilterParams->b[uiCarryForward+j+1] * pFilterParams->prevS[j]);
						else if ((pFilterParams->iGammatonePassStatus == 1) && (!pFilterParams->bFirstWindow))
							pFilterParams->y[i] = pFilterParams->y[i] + (pFilterParams->b[uiCarryForward+j+1] * pFilterParams->prevG2X[j]);
						else if ((pFilterParams->iGammatonePassStatus == 2) && (!pFilterParams->bFirstWindow))
							pFilterParams->y[i] = pFilterParams->y[i] + (pFilterParams->b[uiCarryForward+j+1] * pFilterParams->prevG3X[j]);
						else if ((pFilterParams->iGammatonePassStatus == 3) && (!pFilterParams->bFirstWindow))
							pFilterParams->y[i] = pFilterParams->y[i] + (pFilterParams->b[uiCarryForward+j+1] * pFilterParams->prevG4X[j]);
						else if ((pFilterParams->iGammatonePassStatus == 4) && (!pFilterParams->bFirstWindow))
							pFilterParams->y[i] = pFilterParams->y[i] + (pFilterParams->b[uiCarryForward+j+1] * pFilterParams->prevG5X[j]);
						else if ((pFilterParams->iGammatonePassStatus == 5) && (!pFilterParams->bFirstWindow))
							pFilterParams->y[i] = pFilterParams->y[i] + (pFilterParams->b[uiCarryForward+j+1] * pFilterParams->prevG6X[j]);
						else if ((pFilterParams->iGammatonePassStatus == 6) && (!pFilterParams->bFirstWindow))
							pFilterParams->y[i] = pFilterParams->y[i] + (pFilterParams->b[uiCarryForward+j+1] * pFilterParams->prevG7X[j]);
						else if ((pFilterParams->iGammatonePassStatus == 7) && (!pFilterParams->bFirstWindow))
							pFilterParams->y[i] = pFilterParams->y[i] + (pFilterParams->b[uiCarryForward+j+1] * pFilterParams->prevG8X[j]);
						else if ((pFilterParams->iGammatonePassStatus == 8) && (!pFilterParams->bFirstWindow))
							pFilterParams->y[i] = pFilterParams->y[i] + (pFilterParams->b[uiCarryForward+j+1] * pFilterParams->prevG9X[j]);
						else if ((pFilterParams->iGammatonePassStatus == 9) && (!pFilterParams->bFirstWindow))
							pFilterParams->y[i] = pFilterParams->y[i] + (pFilterParams->b[uiCarryForward+j+1] * pFilterParams->prevG10X[j]);
						else
							pFilterParams->y[i] = pFilterParams->y[i] + (pFilterParams->b[uiCarryForward+j+1] * pFilterParams->prevX[j]);
					}
					else if (pFilterParams->uiWinContinuity == 1)
						pFilterParams->y[i] = pFilterParams->y[i] + (pFilterParams->b[uiCarryForward+j+1] * pFilterParams->prevX[j]);
				}
				uiLoopLockNumCap2--;
			}

			if (uiLoopLockDenCap1)
			{
				for (j=0; j<uiLoopLockDenCap1;j++)
				{
					pFilterParams->y[i] = pFilterParams->y[i] - (pFilterParams->a[j+1] * pFilterParams->y[i-j-1]);
					uiCarryForward = j+1;
				}
			}
			if (uiLoopLockDenCap1 < pFilterParams->uiDenOrder-1)
				uiLoopLockDenCap1++;

			if (uiLoopLockDenCap2)
			{
				for (j=0; j<uiLoopLockDenCap2; j++)
				{
					// apply window continuity option here
					if (pFilterParams->uiWinContinuity == 0)
					{
						if ((pFilterParams->iGammatonePassStatus == 0) && (!pFilterParams->bFirstWindow))
							pFilterParams->y[i] = pFilterParams->y[i] - (pFilterParams->a[j+uiCarryForward+1] * pFilterParams->prevG1Y[j]);
						else if ((pFilterParams->iGammatonePassStatus == 1) && (!pFilterParams->bFirstWindow))
							pFilterParams->y[i] = pFilterParams->y[i] - (pFilterParams->a[j+uiCarryForward+1] * pFilterParams->prevG2Y[j]);
						else if ((pFilterParams->iGammatonePassStatus == 2) && (!pFilterParams->bFirstWindow))
							pFilterParams->y[i] = pFilterParams->y[i] - (pFilterParams->a[j+uiCarryForward+1] * pFilterParams->prevG3Y[j]);
						else if ((pFilterParams->iGammatonePassStatus == 3) && (!pFilterParams->bFirstWindow))
							pFilterParams->y[i] = pFilterParams->y[i] - (pFilterParams->a[j+uiCarryForward+1] * pFilterParams->prevG4Y[j]);
						else if ((pFilterParams->iGammatonePassStatus == 4) && (!pFilterParams->bFirstWindow))
							pFilterParams->y[i] = pFilterParams->y[i] - (pFilterParams->a[j+uiCarryForward+1] * pFilterParams->prevG5Y[j]);
						else if ((pFilterParams->iGammatonePassStatus == 5) && (!pFilterParams->bFirstWindow))
							pFilterParams->y[i] = pFilterParams->y[i] - (pFilterParams->a[j+uiCarryForward+1] * pFilterParams->prevG6Y[j]);
						else if ((pFilterParams->iGammatonePassStatus == 6) && (!pFilterParams->bFirstWindow))
							pFilterParams->y[i] = pFilterParams->y[i] - (pFilterParams->a[j+uiCarryForward+1] * pFilterParams->prevG7Y[j]);
						else if ((pFilterParams->iGammatonePassStatus == 7) && (!pFilterParams->bFirstWindow))
							pFilterParams->y[i] = pFilterParams->y[i] - (pFilterParams->a[j+uiCarryForward+1] * pFilterParams->prevG8Y[j]);
						else if ((pFilterParams->iGammatonePassStatus == 8) && (!pFilterParams->bFirstWindow))
							pFilterParams->y[i] = pFilterParams->y[i] - (pFilterParams->a[j+uiCarryForward+1] * pFilterParams->prevG9Y[j]);
						else if ((pFilterParams->iGammatonePassStatus == 9) && (!pFilterParams->bFirstWindow))
							pFilterParams->y[i] = pFilterParams->y[i] - (pFilterParams->a[j+uiCarryForward+1] * pFilterParams->prevG10Y[j]);
						else
							pFilterParams->y[i] = pFilterParams->y[i] - (pFilterParams->a[j+uiCarryForward+1] * pFilterParams->prevY[j]);
					}
					else if (pFilterParams->uiWinContinuity == 1)
						pFilterParams->y[i] = pFilterParams->y[i] - (pFilterParams->a[j+uiCarryForward+1] * pFilterParams->prevY[j]);
				}
			}
			if (uiLoopLockDenCap2)
				uiLoopLockDenCap2--;

			// ensure scaling by a[0] is done here
			pFilterParams->y[i] = pFilterParams->y[i] / pFilterParams->a[0];
		}
	}

	// end of initial part, now handle the rest of the array
	for (i=uiFirstLoopCap;i<pFilterParams->uiAudioInputWinSize;i++)
	{
		pFilterParams->y[i]=0.0f;
		for (j=0;j<pFilterParams->uiNumOrder;j++)
			pFilterParams->y[i] = pFilterParams->y[i] + (pFilterParams->b[j] * pFilterParams->x[i-j]);
		for (j=0;j<(pFilterParams->uiDenOrder-1);j++)
			pFilterParams->y[i] = pFilterParams->y[i] - (pFilterParams->a[j+1] * pFilterParams->y[i-j-1]);

		// yet again, ensure a[0] scale handled before moving on
		pFilterParams->y[i] = pFilterParams->y[i] / pFilterParams->a[0];
	}

	// store latter values of x & y for use in later invokation of filter() in a n-order filter() call (n>1)
	for (i=0; i<pFilterParams->uiNumOrder-1; i++)
	{
		// Ensure to check for condition where inp matrix size is smaller than filter order (rare)
		if (i < pFilterParams->uiAudioInputWinSize)
		{
			pFilterParams->prevX[i] = pFilterParams->x[pFilterParams->uiAudioInputWinSize - 1 - i];

			if (pFilterParams->iGammatonePassStatus == 0)
			{
				// save input for next G1 block
				pFilterParams->prevS[i] = pFilterParams->x[pFilterParams->uiAudioInputWinSize - 1 - i];
			}
			else if (pFilterParams->iGammatonePassStatus == 1)
			{
				// save current input for next G2 input block
				pFilterParams->prevG2X[i] = pFilterParams->x[pFilterParams->uiAudioInputWinSize - 1 - i];
			}
			else if (pFilterParams->iGammatonePassStatus == 2)
			{
				pFilterParams->prevG3X[i] = pFilterParams->x[pFilterParams->uiAudioInputWinSize - 1 - i];
			}
			else if (pFilterParams->iGammatonePassStatus == 3)
			{
				pFilterParams->prevG4X[i] = pFilterParams->x[pFilterParams->uiAudioInputWinSize - 1 - i];
			}
			else if (pFilterParams->iGammatonePassStatus == 4)
			{
				pFilterParams->prevG5X[i] =  pFilterParams->x[pFilterParams->uiAudioInputWinSize - 1 - i];
			}
			else if (pFilterParams->iGammatonePassStatus == 5)
			{
				pFilterParams->prevG6X[i] =  pFilterParams->x[pFilterParams->uiAudioInputWinSize - 1 - i];
			}
			else if (pFilterParams->iGammatonePassStatus == 6)
			{
				pFilterParams->prevG7X[i] =  pFilterParams->x[pFilterParams->uiAudioInputWinSize - 1 - i];
			}
			else if (pFilterParams->iGammatonePassStatus == 7)
			{
				pFilterParams->prevG8X[i] =  pFilterParams->x[pFilterParams->uiAudioInputWinSize - 1 - i];
			}
			else if (pFilterParams->iGammatonePassStatus == 8)
			{
				pFilterParams->prevG9X[i] =  pFilterParams->x[pFilterParams->uiAudioInputWinSize - 1 - i];
			}
			else if (pFilterParams->iGammatonePassStatus == 9)
			{
				pFilterParams->prevG10X[i] =  pFilterParams->x[pFilterParams->uiAudioInputWinSize - 1 - i];
			}
		}
	}
	for (i=0; i<pFilterParams->uiDenOrder-1; i++)
	{
		// Ensure to check for condition where inp matrix size is smaller than filter order (rare)
		if (i < pFilterParams->uiAudioInputWinSize)
		{
			pFilterParams->prevY[i] = pFilterParams->y[pFilterParams->uiAudioInputWinSize - 1 - i];

			if (pFilterParams->iGammatonePassStatus == 0)
			{
				pFilterParams->prevG1Y[i] = pFilterParams->y[pFilterParams->uiAudioInputWinSize - 1 - i];
			}
			else if (pFilterParams->iGammatonePassStatus == 1)
			{
				pFilterParams->prevG2Y[i] = pFilterParams->y[pFilterParams->uiAudioInputWinSize - 1 - i];
			}
			else if (pFilterParams->iGammatonePassStatus == 2)
			{
				pFilterParams->prevG3Y[i] = pFilterParams->y[pFilterParams->uiAudioInputWinSize - 1 - i];
			}
			else if (pFilterParams->iGammatonePassStatus == 3)
			{
				pFilterParams->prevG4Y[i] = pFilterParams->y[pFilterParams->uiAudioInputWinSize - 1 - i];
			}
			else if (pFilterParams->iGammatonePassStatus == 4)
			{
				pFilterParams->prevG5Y[i] = pFilterParams->y[pFilterParams->uiAudioInputWinSize - 1 - i];
			}
			else if (pFilterParams->iGammatonePassStatus == 5)
			{
				pFilterParams->prevG6Y[i] = pFilterParams->y[pFilterParams->uiAudioInputWinSize - 1 - i];
			}
			else if (pFilterParams->iGammatonePassStatus == 6)
			{
				pFilterParams->prevG7Y[i] = pFilterParams->y[pFilterParams->uiAudioInputWinSize - 1 - i];
			}
			else if (pFilterParams->iGammatonePassStatus == 7)
			{
				pFilterParams->prevG8Y[i] = pFilterParams->y[pFilterParams->uiAudioInputWinSize - 1 - i];
			}
			else if (pFilterParams->iGammatonePassStatus == 8)
			{
				pFilterParams->prevG9Y[i] = pFilterParams->y[pFilterParams->uiAudioInputWinSize - 1 - i];
			}
			else if (pFilterParams->iGammatonePassStatus == 9)
			{
				pFilterParams->prevG10Y[i] = pFilterParams->y[pFilterParams->uiAudioInputWinSize - 1 - i];
			}
		}
	}

#if FILTER_MEASURE_PERFORMANCE
	tFilterEnd = clock();
	dPlot2ScreenTime = ((double)(tFilterEnd - tFilterStart))/CLOCKS_PER_SEC;
	cout << "Filter processing time: " << dPlot2ScreenTime << "sec" << endl;
#if FILTER_WRITE2FILE
	filterfile << "Filter processing time: " << dPlot2ScreenTime << "sec" << endl;
#endif
#endif
#if FILTER_WRITE2FILE
	filterfile.close();
#endif
}

double* RTAPcompute::binomial_mult( int n, double *p )
{
    int i, j;
    double *a;

    a = (double *)calloc( 2 * n, sizeof(double) );
    if( a == NULL ) return( NULL );

    for( i = 0; i < n; ++i )
    {
	for( j = i; j > 0; --j )
	{
	    a[2*j] += p[2*i] * a[2*(j-1)] - p[2*i+1] * a[2*(j-1)+1];
	    a[2*j+1] += p[2*i] * a[2*(j-1)+1] + p[2*i+1] * a[2*(j-1)];
	}
	a[0] += p[2*i];
	a[1] += p[2*i+1];
    }
    return( a );
}

/**********************************************************************
  trinomial_mult - multiplies a series of trinomials together and returns
  the coefficients of the resulting polynomial.
  
  The multiplication has the following form:

  (x^2 + b[0]x + c[0])*(x^2 + b[1]x + c[1])*...*(x^2 + b[n-1]x + c[n-1])

  The b[i] and c[i] coefficients are assumed to be complex and are passed
  to the function as a pointers to arrays of doubles of length 2n. The real
  part of the coefficients are stored in the even numbered elements of the
  array and the imaginary parts are stored in the odd numbered elements.

  The resulting polynomial has the following form:
  
  x^2n + a[0]*x^2n-1 + a[1]*x^2n-2 + ... +a[2n-2]*x + a[2n-1]
  
  The a[i] coefficients can in general be complex but should in most cases
  turn out to be real. The a[i] coefficients are returned by the function as
  a pointer to an array of doubles of length 4n. The real and imaginary
  parts are stored, respectively, in the even and odd elements of the array.
  Storage for the array is allocated by the function and should be freed by
  the calling program when no longer needed.
  
  Function arguments:
  
  n  -  The number of trinomials to multiply
  b  -  Pointer to an array of doubles of length 2n.
  c  -  Pointer to an array of doubles of length 2n.
*/

double* RTAPcompute::trinomial_mult( int n, double *b, double *c )
{
    int i, j;
    double *a;

    a = (double *)calloc( 4 * n, sizeof(double) );
    if( a == NULL ) return( NULL );

    a[2] = c[0];
    a[3] = c[1];
    a[0] = b[0];
    a[1] = b[1];
  
    for( i = 1; i < n; ++i )
    {
	a[2*(2*i+1)]   += c[2*i]*a[2*(2*i-1)]   - c[2*i+1]*a[2*(2*i-1)+1];
	a[2*(2*i+1)+1] += c[2*i]*a[2*(2*i-1)+1] + c[2*i+1]*a[2*(2*i-1)];

	for( j = 2*i; j > 1; --j )
	{
	    a[2*j]   += b[2*i] * a[2*(j-1)]   - b[2*i+1] * a[2*(j-1)+1] + 
		c[2*i] * a[2*(j-2)]   - c[2*i+1] * a[2*(j-2)+1];
	    a[2*j+1] += b[2*i] * a[2*(j-1)+1] + b[2*i+1] * a[2*(j-1)] +
		c[2*i] * a[2*(j-2)+1] + c[2*i+1] * a[2*(j-2)];
	}

	a[2] += b[2*i] * a[0] - b[2*i+1] * a[1] + c[2*i];
	a[3] += b[2*i] * a[1] + b[2*i+1] * a[0] + c[2*i+1];
	a[0] += b[2*i];
	a[1] += b[2*i+1];
    }

    return( a );
}

/**********************************************************************
  dcof_bwlp - calculates the d coefficients for a butterworth lowpass 
  filter. The coefficients are returned as an array of doubles.

*/

double* RTAPcompute::dcof_bwlp( int n, double fcf )
{
    int k;            // loop variables
    double theta;     // PI * fcf / 2.0
    double st;        // sine of theta
    double ct;        // cosine of theta
    double parg;      // pole angle
    double sparg;     // sine of the pole angle
    double cparg;     // cosine of the pole angle
    double a;         // workspace variable
    double *rcof;     // binomial coefficients
    double *dcof;     // dk coefficients

    rcof = (double *)calloc( 2 * n, sizeof(double) );
    if( rcof == NULL ) return( NULL );

    theta = PI * fcf;
    st = sin(theta);
    ct = cos(theta);

    for( k = 0; k < n; ++k )
    {
	parg = PI * (double)(2*k+1)/(double)(2*n);
	sparg = sin(parg);
	cparg = cos(parg);
	a = 1.0 + st*sparg;
	rcof[2*k] = -ct/a;
	rcof[2*k+1] = -st*cparg/a;
    }

    dcof = binomial_mult( n, rcof );
    free( rcof );

    dcof[1] = dcof[0];
    dcof[0] = 1.0;
    for( k = 3; k <= n; ++k )
        dcof[k] = dcof[2*k-2];
    return( dcof );
}

/**********************************************************************
  dcof_bwhp - calculates the d coefficients for a butterworth highpass 
  filter. The coefficients are returned as an array of doubles.

*/

double* RTAPcompute::dcof_bwhp( int n, double fcf )
{
    return( dcof_bwlp( n, fcf ) );
}

/**********************************************************************
  dcof_bwbp - calculates the d coefficients for a butterworth bandpass 
  filter. The coefficients are returned as an array of doubles.

*/

double* RTAPcompute::dcof_bwbp( int n, double f1f, double f2f )
{
    int k;            // loop variables
    double theta;     // PI * (f2f - f1f) / 2.0
    double cp;        // cosine of phi
    double st;        // sine of theta
    double ct;        // cosine of theta
    double s2t;       // sine of 2*theta
    double c2t;       // cosine 0f 2*theta
    double *rcof;     // z^-2 coefficients
    double *tcof;     // z^-1 coefficients
    double *dcof;     // dk coefficients
    double parg;      // pole angle
    double sparg;     // sine of pole angle
    double cparg;     // cosine of pole angle
    double a;         // workspace variables

    cp = cos(PI * (f2f + f1f) / 2.0);
    theta = PI * (f2f - f1f) / 2.0;
    st = sin(theta);
    ct = cos(theta);
    s2t = 2.0*st*ct;        // sine of 2*theta
    c2t = 2.0*ct*ct - 1.0;  // cosine of 2*theta

    rcof = (double *)calloc( 2 * n, sizeof(double) );
    tcof = (double *)calloc( 2 * n, sizeof(double) );

    for( k = 0; k < n; ++k )
    {
	parg = PI * (double)(2*k+1)/(double)(2*n);
	sparg = sin(parg);
	cparg = cos(parg);
	a = 1.0 + s2t*sparg;
	rcof[2*k] = c2t/a;
	rcof[2*k+1] = s2t*cparg/a;
	tcof[2*k] = -2.0*cp*(ct+st*sparg)/a;
	tcof[2*k+1] = -2.0*cp*st*cparg/a;
    }

    dcof = trinomial_mult( n, tcof, rcof );
    free( tcof );
    free( rcof );

    dcof[1] = dcof[0];
    dcof[0] = 1.0;
    for( k = 3; k <= 2*n; ++k )
        dcof[k] = dcof[2*k-2];
    return( dcof );
}

/**********************************************************************
  dcof_bwbs - calculates the d coefficients for a butterworth bandstop 
  filter. The coefficients are returned as an array of doubles.

*/

double* RTAPcompute::dcof_bwbs( int n, double f1f, double f2f )
{
    int k;            // loop variables
    double theta;     // PI * (f2f - f1f) / 2.0
    double cp;        // cosine of phi
    double st;        // sine of theta
    double ct;        // cosine of theta
    double s2t;       // sine of 2*theta
    double c2t;       // cosine 0f 2*theta
    double *rcof;     // z^-2 coefficients
    double *tcof;     // z^-1 coefficients
    double *dcof;     // dk coefficients
    double parg;      // pole angle
    double sparg;     // sine of pole angle
    double cparg;     // cosine of pole angle
    double a;         // workspace variables

    cp = cos(PI * (f2f + f1f) / 2.0);
    theta = PI * (f2f - f1f) / 2.0;
    st = sin(theta);
    ct = cos(theta);
    s2t = 2.0*st*ct;        // sine of 2*theta
    c2t = 2.0*ct*ct - 1.0;  // cosine 0f 2*theta

    rcof = (double *)calloc( 2 * n, sizeof(double) );
    tcof = (double *)calloc( 2 * n, sizeof(double) );  

    for( k = 0; k < n; ++k )
    {
	parg = PI * (double)(2*k+1)/(double)(2*n);
	sparg = sin(parg);
	cparg = cos(parg);
	a = 1.0 + s2t*sparg;
	rcof[2*k] = c2t/a;
	rcof[2*k+1] = -s2t*cparg/a;
	tcof[2*k] = -2.0*cp*(ct+st*sparg)/a;
	tcof[2*k+1] = 2.0*cp*st*cparg/a;
    }

    dcof = trinomial_mult( n, tcof, rcof );
    free( tcof );
    free( rcof );

    dcof[1] = dcof[0];
    dcof[0] = 1.0;
    for( k = 3; k <= 2*n; ++k )
        dcof[k] = dcof[2*k-2];
    return( dcof );
}

/**********************************************************************
  ccof_bwlp - calculates the c coefficients for a butterworth lowpass 
  filter. The coefficients are returned as an array of integers.

*/

int* RTAPcompute::ccof_bwlp( int n )
{
    int *ccof;
    int m;
    int i;

    ccof = (int *)calloc( n+1, sizeof(int) );
    if( ccof == NULL ) return( NULL );

    ccof[0] = 1;
    ccof[1] = n;
    m = n/2;
    for( i=2; i <= m; ++i)
    {
        ccof[i] = (n-i+1)*ccof[i-1]/i;
        ccof[n-i]= ccof[i];
    }
    ccof[n-1] = n;
    ccof[n] = 1;

    return( ccof );
}

/**********************************************************************
  ccof_bwhp - calculates the c coefficients for a butterworth highpass 
  filter. The coefficients are returned as an array of integers.

*/

int* RTAPcompute::ccof_bwhp( int n )
{
    int *ccof;
    int i;

    ccof = ccof_bwlp( n );
    if( ccof == NULL ) return( NULL );

    for( i = 0; i <= n; ++i)
        if( i % 2 ) ccof[i] = -ccof[i];

    return( ccof );
}

/**********************************************************************
  ccof_bwbp - calculates the c coefficients for a butterworth bandpass 
  filter. The coefficients are returned as an array of integers.

*/

int* RTAPcompute::ccof_bwbp( int n )
{
    int *tcof;
    int *ccof;
    int i;

    ccof = (int *)calloc( 2*n+1, sizeof(int) );
    if( ccof == NULL ) return( NULL );

    tcof = ccof_bwhp(n);
    if( tcof == NULL ) return( NULL );

    for( i = 0; i < n; ++i)
    {
        ccof[2*i] = tcof[i];
        ccof[2*i+1] = 0.0;
    }
    ccof[2*n] = tcof[n];

    free( tcof );
    return( ccof );
}

/**********************************************************************
  ccof_bwbs - calculates the c coefficients for a butterworth bandstop 
  filter. The coefficients are returned as an array of integers.

*/

double* RTAPcompute::ccof_bwbs( int n, double f1f, double f2f )
{
    double alpha;
    double *ccof;
    int i, j;

    alpha = -2.0 * cos(PI * (f2f + f1f) / 2.0) / cos(PI * (f2f - f1f) / 2.0);

    ccof = (double *)calloc( 2*n+1, sizeof(double) );

    ccof[0] = 1.0;

    ccof[2] = 1.0;
    ccof[1] = alpha;
  
    for( i = 1; i < n; ++i )
    {
	ccof[2*i+2] += ccof[2*i];
	for( j = 2*i; j > 1; --j )
	    ccof[j+1] += alpha * ccof[j] + ccof[j-1];

	ccof[2] += alpha * ccof[1] + 1.0;
	ccof[1] += alpha;
    }

    return( ccof );
}

/**********************************************************************
  sf_bwlp - calculates the scaling factor for a butterworth lowpass filter.
  The scaling factor is what the c coefficients must be multiplied by so
  that the filter response has a maximum value of 1.

*/

double RTAPcompute::sf_bwlp( int n, double fcf )
{
    int m, k;         // loop variables
    double omega;     // PI * fcf
    double fomega;    // function of omega
    double parg0;     // zeroth pole angle
    double sf;        // scaling factor

    omega = PI * fcf;
    fomega = sin(omega);
    parg0 = PI / (double)(2*n);

    m = n / 2;
    sf = 1.0;
    for( k = 0; k < n/2; ++k )
        sf *= 1.0 + fomega * sin((double)(2*k+1)*parg0);

    fomega = sin(omega / 2.0);

    if( n % 2 ) sf *= fomega + cos(omega / 2.0);
    sf = pow( fomega, n ) / sf;

    return(sf);
}

/**********************************************************************
  sf_bwhp - calculates the scaling factor for a butterworth highpass filter.
  The scaling factor is what the c coefficients must be multiplied by so
  that the filter response has a maximum value of 1.

*/

double RTAPcompute::sf_bwhp( int n, double fcf )
{
    int m, k;         // loop variables
    double omega;     // PI * fcf
    double fomega;    // function of omega
    double parg0;     // zeroth pole angle
    double sf;        // scaling factor

    omega = PI * fcf;
    fomega = sin(omega);
    parg0 = PI / (double)(2*n);

    m = n / 2;
    sf = 1.0;
    for( k = 0; k < n/2; ++k )
        sf *= 1.0 + fomega * sin((double)(2*k+1)*parg0);

    fomega = cos(omega / 2.0);

    if( n % 2 ) sf *= fomega + sin(omega / 2.0);
    sf = pow( fomega, n ) / sf;

    return(sf);
}

/**********************************************************************
  sf_bwbp - calculates the scaling factor for a butterworth bandpass filter.
  The scaling factor is what the c coefficients must be multiplied by so
  that the filter response has a maximum value of 1.

*/

double RTAPcompute::sf_bwbp( int n, double f1f, double f2f )
{
    int k;            // loop variables
    double ctt;       // cotangent of theta
    double sfr, sfi;  // real and imaginary parts of the scaling factor
    double parg;      // pole angle
    double sparg;     // sine of pole angle
    double cparg;     // cosine of pole angle
    double a, b, c;   // workspace variables

    ctt = 1.0 / tan(PI * (f2f - f1f) / 2.0);
    sfr = 1.0;
    sfi = 0.0;

    for( k = 0; k < n; ++k )
    {
	parg = PI * (double)(2*k+1)/(double)(2*n);		// (2*k+1) => odd number
	sparg = ctt + sin(parg);
	cparg = cos(parg);
	a = (sfr + sfi)*(sparg - cparg);
	b = sfr * sparg;
	c = -sfi * cparg;
	sfr = b - c;
	sfi = a - b - c;
    }

    return( 1.0 / sfr );
}

/**********************************************************************
  sf_bwbs - calculates the scaling factor for a butterworth bandstop filter.
  The scaling factor is what the c coefficients must be multiplied by so
  that the filter response has a maximum value of 1.

*/

double RTAPcompute::sf_bwbs( int n, double f1f, double f2f )
{
    int k;            // loop variables
    double tt;        // tangent of theta
    double sfr, sfi;  // real and imaginary parts of the scaling factor
    double parg;      // pole angle
    double sparg;     // sine of pole angle
    double cparg;     // cosine of pole angle
    double a, b, c;   // workspace variables

    tt = tan(PI * (f2f - f1f) / 2.0);
    sfr = 1.0;
    sfi = 0.0;

    for( k = 0; k < n; ++k )
    {
	parg = PI * (double)(2*k+1)/(double)(2*n);
	sparg = tt + sin(parg);
	cparg = cos(parg);
	a = (sfr + sfi)*(sparg - cparg);
	b = sfr * sparg;
	c = -sfi * cparg;
	sfr = b - c;
	sfi = a - b - c;
    }

    return( 1.0 / sfr );
}
