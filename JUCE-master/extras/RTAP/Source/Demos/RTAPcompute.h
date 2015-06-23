#ifndef __RTAPCOMPUTE_H__
#define __RTAPCOMPUTE_H__

#include "nconventions.h"
#include <complex>
#include <math.h>
#include <stdlib.h>
#include "globalvars.h"
#include <process.h>

#define PI							3.14159265359f
#define NUMOFCOLOURBANDS			31
#define IMGBREADTH					168
#define NUMWIN2REC					2

// debug pre-processor directives
#define BROKENSTICK_DEBUG			0
#define BROKENSTICK_VARVIEW			5	// change this to any number until uiSizeofInp to view variable values
#define TEST_FILTER					0
#define TEST_GAMMATONE				0
#define DRNL_WRITE2FILE				0
#define OME_INPUT_CHECK				0
#define EER_WRITE2FILE				0
#define TMDISP_WRITE2FILE			0
#define STAPESDISP_WRITE2FILE		0
#define OME_WRITE2FILE				0
#define GAMMATONE_WRITE2FILE		0
#define FILTER_DEBUG				0
#define FILTER_LOOP1_DEBUG			0
#define FILTER_LOOP2_DEBUG			0
#define FILTER_ZF_CHECK				0
#define FILTER_ZFLOOP1_DEBUG		0
#define FILTER_ZFLOOP2_DEBUG		0
#define FILTER_WRITE2FILE			0
#define SPECTROWAVE_DEBUG			0

class RTAPcompute
{
public:
	RTAPcompute ();
	~RTAPcompute ();
	void FreeResources ();
	void Reinitialise ();
	void LabelYaxis (float* fBFlist);
	void LabelXaxis ();
	void getAudioSamples ();
	void setOMEparams (EarStruct*);
	void setButterworthCoeff (EarStruct*);
	void setDRNLparams (EarStruct*);
	void setIHCparams (EarStruct*);
	void setANparams (EarStruct*);
	void OMEprocess (EarStruct*);

	void DRNL_SingleThreadedOpt (EarStruct*);
	void DRNL_to_IHC_RP_SingleThreadedOpt (EarStruct*);
	void DRNL_to_IHCpresyanpse_SingleThreadedOpt (EarStruct*);
	void DRNL_to_AN_SingleThreadedOpt (EarStruct*);

	EarStruct* pEar;
	strFilterParams* pFilterParams;
	double dPlot2ScreenTime;

private:
	// POSIX thread declarations
	// value expected to be returned from a pointer to a void
	static void* StartAlgoThread (void*);
	static void* StartRecThreads (void*);
	void* ProcessFunction ();
	void* ProcDataRecordThread ( );

#if SINGLE_PRECISION
	void DRNL_brokenstick_nl (float* fOutp, float* fInp, EarStruct* pEar);
#elif DOUBLE_PRECISION
	void DRNL_brokenstick_nl (double* fOutp, double* fInp, EarStruct* pEar);
#endif
	void gammatone (strFilterParams*, UINT);
	void GammatoneOpt (strFilterParams*, UINT);
	void DFT2filterOpt (strFilterParams*);
	void RTAP_MaxMinSymmetry ();

#if SINGLE_PRECISION
	UINT RTAP_round (float fX);
	void RTAP_logspace (float* fNum, float fXlower, float fXupper, UINT uiNumPoints);
	float RTAP_exp (float x);
	float RTAP_abs (float x);
	int RTAP_sign (float fX);
#elif DOUBLE_PRECISION
	UINT RTAP_round (double fX);
	void RTAP_logspace (double* fNum, double fXlower, double fXupper, UINT uiNumPoints);
	double RTAP_exp (double x);
	double RTAP_abs (double x);
	int RTAP_sign (double fX);
#endif
	// Butterworth filter functions to compute coefficients
	double *binomial_mult(int n, double *p);
	double *trinomial_mult(int n, double *b, double *c);

	double *dcof_bwlp(int n, double fcf);
	double *dcof_bwhp(int n, double fcf);
	double *dcof_bwbp(int n, double f1f, double f2f);
	double *dcof_bwbs(int n, double f1f, double f2f);

	int *ccof_bwlp(int n);
	int *ccof_bwhp(int n);
	int *ccof_bwbp(int n);
	double *ccof_bwbs(int n, double f1f, double f2f);

	double sf_bwlp(int n, double fcf);
	double sf_bwhp(int n, double fcf);
	double sf_bwbp(int n, double f1f, double f2f);
	double sf_bwbs(int n, double f1f, double f2f);

	AuditoryPeriphery2Process eAuditoryPeriphery2Process;
#if SINGLE_PRECISION
	float fAudioData2Dump[1280*NUMWIN2REC];							// separate audio data dump buffer (complement the proc data dumping -> reserved for future implementation)
	float fProcData2Dump[NUMOFBFCHANNELS][NUMOFFIBERCHANNELS][1280*NUMWIN2REC];
	float fAudioInput[1280];
#elif DOUBLE_PRECISION
	double fAudioData2Dump[1280*NUMWIN2REC];							// separate audio data dump buffer (complement the proc data dumping -> reserved for future implementation)
	double fProcData2Dump[NUMOFBFCHANNELS][NUMOFFIBERCHANNELS][1280*NUMWIN2REC];
	double fAudioInput[1280];
#endif
	UINT uiSubSamples;
	UINT uiOffset;
	char* FileHeader;
	char cProcThreaduiStatus[2];
	char cFileWriterThreaduiStatus[2];
};

#endif