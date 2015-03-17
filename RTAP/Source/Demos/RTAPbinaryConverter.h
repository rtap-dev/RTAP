#ifndef __AUDITORYPERIPHERYBINCONVERTER_H__
#define __AUDITORYPERIPHERYBINCONVERTER_H__

#include "../JuceDemoHeader.h"
#include "RTAPmaster.h"
#include <sys\stat.h>
#include <fcntl.h>
#include <io.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <share.h>
#include <string.h>
#include "globalvars.h"

#define C_CONVENTIONS			0

#define BYTESPERBF				1280*4		// sizeof(float) * 1280 FP sample (based on fs = 22.05kHz)
#define CHECK_FP				0
#define FILE_READ				1
#define CHECK_SINGLE_HEX_FILE	0
#define ARBITRARY_THRESHOLD		1e-10
#define USE_READ				0
#define USE_FREAD				1
#define LSR						1
#define HSR						2

#define AUDIO_IN					1
#define EER_OUT						2
#define TM_DISP						3
#define STAPES_DISP					4
#define DRNL_ST						5
#define DRNL2IHCRP_ST				6
#define DRNL2IHCpresynapse_ST		7
#define DRNL2ANsynapse_ST			8
#define DRNL_ST_OPT					9
#define DRNL2IHCRP_ST_OPT			10
#define DRNL2IHCpresynapse_ST_OPT	11
#define DRNL2ANsynapse_ST_OPT		12
#define DRNL_MT						13
#define DRNL2IHCRP_MT				14
#define DRNL2IHCpresynapse_MT		15
#define DRNL2ANsynapse_MT			16
#define STRINGSIZE					256
#define MAXWINSIZE					5120

typedef struct {
	long FileLength;
	float* fProcData;
	float* fAbsFPstep;
	float* fBFlist;
	unsigned int readcount;
	unsigned int uiNumBFchannels;
	unsigned int uiNumANfibreTypes;
	unsigned int uiNumANchannels;
	unsigned int uiFunctionRunning;
	unsigned int uiProcDataSizePerBF;
	unsigned int uiByteOffset;
	unsigned int uiStartPoint;
	unsigned int uiCharBuffEndPoint;
	unsigned int uiBFMin;
	unsigned int uiBFMax;
	unsigned int uiCurrBF;
	unsigned int uiCurrANfibreType;
	unsigned int uiToneFreq;
	int iANfibreType;
	char* pFileType;
	char cAudioSrc;
}RTAPstruct;

class ConversionProgressThread : public ThreadWithProgressWindow
{
public:
	ConversionProgressThread () : ThreadWithProgressWindow ("RTAP binary file conversion to text", true, true)
	{
		setStatusMessage ("Initialising ...");

		// Allocate memory
		RTAP.pFileType = new char [4];
		RTAP.fProcData = new float [1280];
		RTAP.fAbsFPstep = new float [3];
		RTAP.fBFlist = new float [100];				// based on observed data

		cFunctionExecString = new char [STRINGSIZE];
		cInputType = new char [STRINGSIZE];
		cANfibreType = new char[STRINGSIZE];
		pDataBuff = new char[MAXWINSIZE];

		// initialise file vairable here for display
		RTAP.FileLength = 0;

		bExitStatusThread = false;
	}
	~ConversionProgressThread ()
	{
		delete pDataBuff;
		delete cFunctionExecString;
		delete cInputType;
		delete cANfibreType;
		delete RTAP.fBFlist;
		delete RTAP.fAbsFPstep;
		delete RTAP.fProcData;
		delete RTAP.pFileType;
	}
	void ConvertBinRTAPfile ()
	{
		bExit = false;
		long dataSize = RTAP.FileLength - SIZEOFHEADER;
		uiFPoffset = 0;
		RTAP.uiByteOffset = 0;
		RTAP.uiStartPoint = 0;
		RTAP.uiCurrBF = 0;
		RTAP.uiCurrANfibreType = 0;
		nbytes = 5000;
		long i, j;
		double dStatus = 0.f;

		setStatusMessage ("Writing RTAP header data ...");
		RTAP_logspace (RTAP.fBFlist, (float) RTAP.uiBFMin, (float) RTAP.uiBFMax, RTAP.uiNumBFchannels);
		WriteHeaderToTextFile ();

		FPdataSize = RTAP.FileLength - SIZEOFHEADER;

		setStatusMessage ("Writing RTAP data ...");
		for (i=0; i<dataSize; i+=(RTAP.uiProcDataSizePerBF << 2))
		{
			// check if file size has become smaller than default size - > smaller segment indicates end of file approaching
			if ((RTAP.FileLength - i) < (RTAP.uiProcDataSizePerBF << 2))
			{
				RTAP.uiProcDataSizePerBF = ((RTAP.FileLength - i) >> 2);
				bExit = true;
			}
			// read size of float * BF channel size = 4 * (normally) 2560 (based on 22.05 kHz)
			uiBytesToRead = RTAP.uiProcDataSizePerBF<<2;

			brtapReadStream->read (pDataBuff, uiBytesToRead);//RTAP.uiProcDataSizePerBF);

			// reset char buff pointer
			uiCharBuffOffset = 0;
			// set the end point for char buff
			RTAP.uiCharBuffEndPoint = RTAP.uiProcDataSizePerBF - 1;

			for (j=0; j<RTAP.uiProcDataSizePerBF; j++)
			{
				memcpy ((char*) &RTAP.fProcData[j], &pDataBuff[uiCharBuffOffset], 4);

				proxy = fAbs(RTAP.fProcData[j]);
				// check for very tiny values here

				uiCharBuffOffset += 4;
			}

			WriteFPdataToTextFile (i);

			if (bExit)
				break;

			if (((i/(RTAP.uiProcDataSizePerBF<<2)) % 2) == 1)
			{
				RTAP.uiCurrANfibreType = (RTAP.uiCurrANfibreType++) % (RTAP.uiNumANfibreTypes);
				if (RTAP.uiCurrANfibreType == LSR)
					RTAP.uiCurrBF = (RTAP.uiCurrBF++) % (RTAP.uiNumBFchannels);
			}
			dStatus = (double) i/dataSize;
			setProgress (dStatus);
		}
	}
	void WriteHeaderToTextFile ()
	{	
#if C_CONVENTIONS
		fopen_s (&pTxtFile, "ProcessedData.txt","w");

		fprintf (pTxtFile, "%c%c%c%c Computed Data\n", RTAP.pFileType[0], RTAP.pFileType[1], RTAP.pFileType[2], RTAP.pFileType[3]);
		fprintf (pTxtFile, "%s\n", cInputType);
		fprintf (pTxtFile, "Tone freq: %d Hz\n", RTAP.uiToneFreq);
		fprintf (pTxtFile, "Number of BF channels: %d\n", RTAP.uiNumBFchannels);
		fprintf (pTxtFile, "Number of AN fibre types: %d\n", RTAP.uiNumANfibreTypes);
		fprintf (pTxtFile, "Number of AN channels: %d\n", RTAP.uiNumANchannels);
		fprintf (pTxtFile, "%s\n", cFunctionExecString);
		fprintf (pTxtFile, "Size of processed data per BF channel: %d\n", RTAP.uiProcDataSizePerBF);
		fprintf (pTxtFile, "Min BF: %d Hz\n", RTAP.uiBFMin);
		fprintf (pTxtFile, "Max BF: %d Hz\n", RTAP.uiBFMax);
		fprintf (pTxtFile, "%s \n", cANfibreType);

		fclose (pTxtFile);
#else
		ProxyBuff.clear ();
		ProxyBuff = "Input type: ";
		ProxyBuff.append (cInputType, 15);;
		ProxyBuff.append ("\n", 5);
		RTAPtxtFile.appendText (ProxyBuff, false, false);
		ProxyBuff.clear ();
		ProxyBuff = "Tone frequency: ";
		if (RTAP.cAudioSrc != AUDIO_SYSTEM)
		{
			ProxyBuff.append (String(RTAP.uiToneFreq), 6);
			ProxyBuff.append (" Hz\n", 5);
		}
		else
			ProxyBuff.append ("N/A\n", 5);
		RTAPtxtFile.appendText (ProxyBuff, false, false);
		ProxyBuff.clear ();
		ProxyBuff = "Number of best frequency (BF) channels: ";
		ProxyBuff.append (String(RTAP.uiNumBFchannels), 4);
		ProxyBuff.append ("\n", 3);
		RTAPtxtFile.appendText (ProxyBuff, false, false);
		ProxyBuff.clear ();
		ProxyBuff = "Number of AN fibre types: ";
		ProxyBuff.append (String(RTAP.uiNumANfibreTypes), 2);
		ProxyBuff.append ("\n", 3);
		RTAPtxtFile.appendText (ProxyBuff, false, false);
		ProxyBuff.clear ();
		ProxyBuff = "Number of AN channels: ";
		ProxyBuff.append (String(RTAP.uiNumANchannels), 4);
		ProxyBuff.append ("\n", 3);
		RTAPtxtFile.appendText (ProxyBuff, false, false);
		ProxyBuff.clear ();
		ProxyBuff = cFunctionExecString;
		ProxyBuff.append ("\n", 3);
		RTAPtxtFile.appendText (ProxyBuff, false, false);
		ProxyBuff.clear ();
		ProxyBuff = "Window size: ";
		ProxyBuff.append (String(RTAP.uiProcDataSizePerBF), 5);
		ProxyBuff.append ("\n", 3);
		RTAPtxtFile.appendText (ProxyBuff, false, false);
		ProxyBuff.clear ();
		ProxyBuff = "Minimum BF: ";
		ProxyBuff.append (String(RTAP.uiBFMin), 6);
		ProxyBuff.append (" Hz", 3);
		ProxyBuff.append ("\n", 3);
		RTAPtxtFile.appendText (ProxyBuff, false, false);
		ProxyBuff.clear ();
		ProxyBuff = "Maximum BF: ";
		ProxyBuff.append (String(RTAP.uiBFMax), 6);
		ProxyBuff.append (" Hz", 3);
		ProxyBuff.append ("\n", 3);
		RTAPtxtFile.appendText (ProxyBuff, false, false);
		ProxyBuff.clear ();
		ProxyBuff = "AN fibre type: ";
		ProxyBuff.append (cANfibreType, 7);
		ProxyBuff.append ("\n", 3);
		RTAPtxtFile.appendText (ProxyBuff, false, false);
#endif
	}
	void WriteFPdataToTextFile (unsigned int uiSet)
	{
		int i;
	
#if C_CONVENTIONS
		fopen_s (&pTxtFile, "ProcessedData.txt","a+");

		if (uiSet == 0)
		{
			fprintf (pTxtFile, "Audio Input Data \n");
		}
		else if (uiSet == (RTAP.uiProcDataSizePerBF << 2))
		{
			// also valid for uiSet = 1 but don't need to show
		}
		else if (((uiSet / (RTAP.uiProcDataSizePerBF << 2)) % 2) == 0)
		{
			fprintf (pTxtFile, "BF Channel %d: %f Hz\n", RTAP.uiCurrBF, RTAP.fBFlist[RTAP.uiCurrBF-1]);
			if ((RTAP.uiFunctionRunning == DRNL2IHCpresynapse_ST) || (RTAP.uiFunctionRunning == DRNL2ANsynapse_ST) ||
				(RTAP.uiFunctionRunning == DRNL2IHCpresynapse_ST_OPT) || (RTAP.uiFunctionRunning == DRNL2ANsynapse_ST_OPT) ||
				(RTAP.uiFunctionRunning == DRNL2IHCpresynapse_MT) || (RTAP.uiFunctionRunning == DRNL2ANsynapse_MT))
			{
				if (RTAP.uiCurrANfibreType == LSR)
					fprintf (pTxtFile, "AN Fibre Type: Low Spontaneous Rate (LSR)\n");
				else if (RTAP.uiCurrANfibreType == HSR)
					fprintf (pTxtFile, "AN Fibre Type: High Spontaneous Rate (HSR)\n");
			}
		}
		for (i=0; i<RTAP.uiProcDataSizePerBF; i++)
		{
			fprintf (pTxtFile, "%.15f\n", RTAP.fProcData[i]);
		}

		fclose (pTxtFile);
#else
		if (uiSet == 0)
		{
			ProxyBuff.clear ();
			ProxyBuff = "Audio Input Data";
			ProxyBuff.append ("\n", 3);
			RTAPtxtFile.appendText (ProxyBuff, false, false);
		}
		else if (uiSet == (RTAP.uiProcDataSizePerBF << 2))
		{
			// also valid for uiSet = 1 but don't need to show
		}
		else if (((uiSet / (RTAP.uiProcDataSizePerBF << 2)) % 2) == 0)
		{
			ProxyBuff.clear ();
			ProxyBuff = "BF Channel ";
			ProxyBuff.append (String(RTAP.uiCurrBF), 4);
			ProxyBuff.append (String(": "), 3);
			ProxyBuff.append (String(RTAP.fBFlist[RTAP.uiCurrBF-1]), 4);
			ProxyBuff.append (String(" Hz"), 4);
			ProxyBuff.append (String("\n"), 4);
			RTAPtxtFile.appendText (ProxyBuff, false, false);
			if ((RTAP.uiFunctionRunning == DRNL2IHCpresynapse_ST) || (RTAP.uiFunctionRunning == DRNL2ANsynapse_ST) ||
				(RTAP.uiFunctionRunning == DRNL2IHCpresynapse_ST_OPT) || (RTAP.uiFunctionRunning == DRNL2ANsynapse_ST_OPT) ||
				(RTAP.uiFunctionRunning == DRNL2IHCpresynapse_MT) || (RTAP.uiFunctionRunning == DRNL2ANsynapse_MT))
			{
				ProxyBuff.clear ();
				if (RTAP.uiCurrANfibreType == LSR)
				{
					RTAPtxtFile.appendText ("AN Fibre Type: Low Spontaneous Rate (LSR)");
				}
				else if (RTAP.uiCurrANfibreType == HSR)
				{
					RTAPtxtFile.appendText ("AN Fibre Type: High Spontaneous Rate (HSR)");
				}

				RTAPtxtFile.appendText ("\n");
			}
		}
		for (i=0; i<RTAP.uiProcDataSizePerBF; i++)
		{
			ProxyBuff.clear ();
			ProxyBuff.append (String(RTAP.fProcData[i]), 17);
			ProxyBuff.append ("\n", 3);
			RTAPtxtFile.appendText (ProxyBuff, false, false);
		}
#endif
	}
	float fAbs (float fNum)
	{
		if (fNum < 0)
			fNum = -fNum;			// ensure all numbers are +ve

		return (fNum);
	}
	void RTAP_logspace (float* fNum, float fXlower, float fXupper, unsigned int uiNumPoints)
	{
		float fStep;
		unsigned int i;

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
	void run()	override
	{
			setProgress (-1.0);
			setStatusMessage ("Running RTAP binary converter");
			ConvertBinRTAPfile ();
	}
	
	String ProxyBuff;

	RTAPstruct RTAP;

	File RTAPbinFile, RTAPtxtFile;
	FileInputStream* brtapReadStream;
	long FPdataSize;
	float proxy;
	UINT uiFPoffset;
	UINT uiBytesToRead;
	UINT uiCharBuffOffset;
	UINT nbytes;
	char cFunctionToRun;
	enum eFunctionToRun
	{
		eLoadRTAPbinFile,
		eSaveAsRTAPtxtFile,
		eConvertBinToTxt
	};
	char* pDataBuff;
	char* cInputType;
	char* cFunctionExecString;
	char *cANfibreType;
	bool bExitStatusThread;
	bool bExit;
};

class AuditoryPeripheryBinConverter	:	public Component,
										public ButtonListener,
										public ConversionProgressThread
{
public:
	AuditoryPeripheryBinConverter (AudioDeviceManager& deviceManager_);
	~AuditoryPeripheryBinConverter ();

	void paint (Graphics& g);
	void resized ();
	void buttonClicked (Button* buttonThatWasClicked);

private:
	AudioDeviceManager& deviceManager;

	TextButton* LoadRTAPbinFileBtn;
	TextButton* SaveAsRTAPtxtFileBtn;
	TextButton* ConvertRTAPbinFileBtn;

	void LoadBinRTAPfile ();
	void SaveAsRTAPtxtFile ();
	void ConvertBinRTAPfile ();
	void MainFunc ();
	float fAbs (float fNum);
	void FormatRTAPheader (char* pHeader);
	void WriteHeaderToTextFile ();
	void FormatFPdata (char* pFPdata);
	void WriteFPdataToTextFile (unsigned int uiSet);
	void RTAP_logspace (float* fNum, float fXlower, float fXupper, unsigned int uiNumPoints);

	String BinFileNameString;
	String TextFileNameString;

	// Variables
	FILE *pRTAPbinFile, *pTxtFile;
	long i, BytesRead;
	float proxy;						// old function usage
	char* pHeaderBuff;
	char* cBinFileStatus;
	char* cTxtFileStatus;
	char* cRunStatus;
	errno_t err;
	int fh;
	unsigned int num, j;
	bool bSaveAsBtnClicked;

	// Vairables for bin file info display
	Label* BinFileStatus;
	Label* TxtFileStatus;
	Label* InputSineToneFreqStatus;
	Label* FileSizeStatus;
	Label* NumBFchannelStatus;
	Label* NumANfibreTypeStatus;
	Label* NumANchannelStatus;
	Label* FunctionExecStatus;
	Label* WindowSizeStatus;
	Label* MinBFstatus;
	Label* MaxBFstatus;
	Label* InputTypeStatus;
	Label* ANfibreTypeStatus;
	Label* RunStatus;
	Label* BinFileName;
	Label* TextFileName;
	Label* Instructions;

	AuditoryPeripheryBinConverter (const AuditoryPeripheryBinConverter&);
	AuditoryPeripheryBinConverter& operator= (const AuditoryPeripheryBinConverter&);
};

#endif