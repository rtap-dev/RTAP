#include "RTAPbinaryConverter.h"

AuditoryPeripheryBinConverter::AuditoryPeripheryBinConverter(AudioDeviceManager& deviceManager_)
	:	deviceManager (deviceManager_),
		LoadRTAPbinFileBtn(0),
		SaveAsRTAPtxtFileBtn(0),
		ConvertRTAPbinFileBtn(0),
		BinFileStatus(0),
		TxtFileStatus(0),
		InputSineToneFreqStatus(0),
		FileSizeStatus(0),
		NumBFchannelStatus(0),
		NumANfibreTypeStatus(0),
		NumANchannelStatus(0),
		FunctionExecStatus(0),
		WindowSizeStatus(0),
		MinBFstatus(0),
		MaxBFstatus(0),
		InputTypeStatus(0),
		ANfibreTypeStatus(0),
		BinFileName(0),
		TextFileName(0),
		RunStatus(0),
		Instructions(0)
{
	// pDataBuff size is sizeof(float)*perBFsize + perBFsize
	// last param perBFsize is an allowance to cater for extra bytes to be read in if rogue bytes encountered
	pHeaderBuff = new char [54];
	cTxtFileStatus = new char [STRINGSIZE];
	cRunStatus = new char[STRINGSIZE];
	cBinFileStatus = new char [STRINGSIZE];

	// initialise file vairable here for display
	RTAP.FileLength = 0;
	err = -1;

	// -------- Binary file 'load' button start --------
	addAndMakeVisible (LoadRTAPbinFileBtn = new TextButton (String::empty));
    LoadRTAPbinFileBtn->setButtonText ("Load");
    LoadRTAPbinFileBtn->addListener (this);
    LoadRTAPbinFileBtn->setColour (TextButton::buttonColourId, Colours::bisque);
    LoadRTAPbinFileBtn->setColour (TextButton::textColourOffId, Colours::black);
	LoadRTAPbinFileBtn->setTooltip ("Select RTAP binary file to load");
	// -------- Binary file 'load' button end --------

	// -------- Processed data save to bin file button start --------
	addAndMakeVisible (ConvertRTAPbinFileBtn = new TextButton (String::empty));
    ConvertRTAPbinFileBtn->setButtonText ("Convert");
    ConvertRTAPbinFileBtn->addListener (this);
    ConvertRTAPbinFileBtn->setColour (TextButton::buttonColourId, Colours::bisque);
    ConvertRTAPbinFileBtn->setColour (TextButton::textColourOffId, Colours::black);
	ConvertRTAPbinFileBtn->setTooltip ("Convert the selected RTAP binary file");
	// -------- Processed data save to bin file button end --------

	// -------- Text file 'save as' button start --------
	addAndMakeVisible (SaveAsRTAPtxtFileBtn = new TextButton (String::empty));
    SaveAsRTAPtxtFileBtn->setButtonText ("Save As");
    SaveAsRTAPtxtFileBtn->addListener (this);
    SaveAsRTAPtxtFileBtn->setColour (TextButton::buttonColourId, Colours::bisque);
    SaveAsRTAPtxtFileBtn->setColour (TextButton::textColourOffId, Colours::black);
	SaveAsRTAPtxtFileBtn->setTooltip ("Set a different path location and text file name for the converted text data to be stored");
	// -------- Text file 'save as' button end --------

	// ------------ Binary file info display setup start ------------
	addAndMakeVisible (BinFileStatus = new Label (String::empty));
	BinFileStatus->setJustificationType (Justification::centredLeft);
	BinFileStatus->setEditable (false, false, false);
	BinFileStatus->setColour (TextEditor::textColourId, Colours::black);
	addAndMakeVisible (TxtFileStatus = new Label (String::empty));
	TxtFileStatus->setJustificationType (Justification::centredLeft);
	TxtFileStatus->setEditable (false, false, false);
	TxtFileStatus->setColour (TextEditor::textColourId, Colours::black);
	addAndMakeVisible (BinFileName = new Label (String::empty));
	BinFileName->setJustificationType (Justification::centredLeft);
	BinFileName->setEditable (false, false, false);
	BinFileName->setColour (TextEditor::textColourId, Colours::black);
	addAndMakeVisible (TextFileName = new Label (String::empty));
	TextFileName->setJustificationType (Justification::centredLeft);
	TextFileName->setEditable (false, false, false);
	TextFileName->setColour (TextEditor::textColourId, Colours::black);
	addAndMakeVisible (InputTypeStatus = new Label (String::empty));
	InputTypeStatus->setJustificationType (Justification::centredLeft);
	InputTypeStatus->setEditable (false, false, false);
	InputTypeStatus->setColour (TextEditor::textColourId, Colours::black);
	addAndMakeVisible (InputSineToneFreqStatus = new Label (String::empty));
	InputSineToneFreqStatus->setJustificationType (Justification::centredLeft);
	InputSineToneFreqStatus->setEditable (false, false, false);
	InputSineToneFreqStatus->setColour (TextEditor::textColourId, Colours::black);
	addAndMakeVisible (FileSizeStatus = new Label (String::empty));
	FileSizeStatus->setJustificationType (Justification::centredLeft);
	FileSizeStatus->setEditable (false, false, false);
	FileSizeStatus->setColour (TextEditor::textColourId, Colours::black);
	addAndMakeVisible (NumBFchannelStatus = new Label (String::empty));
	NumBFchannelStatus->setJustificationType (Justification::centredLeft);
	NumBFchannelStatus->setEditable (false, false, false);
	NumBFchannelStatus->setColour (TextEditor::textColourId, Colours::black);
	addAndMakeVisible (NumANfibreTypeStatus = new Label (String::empty));
	NumANfibreTypeStatus->setJustificationType (Justification::centredLeft);
	NumANfibreTypeStatus->setEditable (false, false, false);
	NumANfibreTypeStatus->setColour (TextEditor::textColourId, Colours::black);
	addAndMakeVisible (NumANchannelStatus = new Label (String::empty));
	NumANchannelStatus->setJustificationType (Justification::centredLeft);
	NumANchannelStatus->setEditable (false, false, false);
	NumANchannelStatus->setColour (TextEditor::textColourId, Colours::black);
	addAndMakeVisible (FunctionExecStatus = new Label (String::empty));
	FunctionExecStatus->setJustificationType (Justification::centredLeft);
	FunctionExecStatus->setEditable (false, false, false);
	FunctionExecStatus->setColour (TextEditor::textColourId, Colours::black);
	addAndMakeVisible (WindowSizeStatus = new Label (String::empty));
	WindowSizeStatus->setJustificationType (Justification::centredLeft);
	WindowSizeStatus->setEditable (false, false, false);
	WindowSizeStatus->setColour (TextEditor::textColourId, Colours::black);
	addAndMakeVisible (MinBFstatus = new Label (String::empty));
	MinBFstatus->setJustificationType (Justification::centredLeft);
	MinBFstatus->setEditable (false, false, false);
	MinBFstatus->setColour (TextEditor::textColourId, Colours::black);
	addAndMakeVisible (MaxBFstatus = new Label (String::empty));
	MaxBFstatus->setJustificationType (Justification::centredLeft);
	MaxBFstatus->setEditable (false, false, false);
	MaxBFstatus->setColour (TextEditor::textColourId, Colours::black);
	addAndMakeVisible (ANfibreTypeStatus = new Label (String::empty));
	ANfibreTypeStatus->setJustificationType (Justification::centredLeft);
	ANfibreTypeStatus->setEditable (false, false, false);
	ANfibreTypeStatus->setColour (TextEditor::textColourId, Colours::black);
	addAndMakeVisible (RunStatus = new Label (String::empty));
	RunStatus->setJustificationType (Justification::centredLeft);
	RunStatus->setEditable (false, false, false);
	RunStatus->setColour (TextEditor::textColourId, Colours::black);
	addAndMakeVisible (Instructions = new Label (String::empty));
	Instructions->setJustificationType (Justification::centredLeft);
	Instructions->setEditable (false, false, false);
	Instructions->setColour (TextEditor::textColourId, Colours::steelblue);
	// ------------ Binary file info display setup end ------------
	
	strncpy (ConversionProgressThread::cFunctionExecString, "No conversion done yet\n", STRINGSIZE);
	strncpy (cBinFileStatus, "Binary file not converted", STRINGSIZE);
	strncpy (cTxtFileStatus, "Data not dumped in text file", STRINGSIZE);
	strncpy (ConversionProgressThread::cInputType, "N/A\n", STRINGSIZE);
	strncpy (ConversionProgressThread::cANfibreType, "N/A\n", STRINGSIZE);
	strncpy (cRunStatus, "Converter idle", STRINGSIZE);
	BinFileNameString = "No binary file";
	TextFileNameString = "No text file";
	bSaveAsBtnClicked = false;
}

AuditoryPeripheryBinConverter::~AuditoryPeripheryBinConverter()
{
	delete cBinFileStatus;
	delete cRunStatus;
	delete pHeaderBuff;
	delete cTxtFileStatus;

	deleteAndZero (LoadRTAPbinFileBtn);
	deleteAndZero (ConvertRTAPbinFileBtn);
	deleteAndZero (SaveAsRTAPtxtFileBtn);
	deleteAndZero (BinFileStatus);
	deleteAndZero (TxtFileStatus);
	deleteAndZero (BinFileName);
	deleteAndZero (TextFileName);
	deleteAndZero (InputSineToneFreqStatus);
	deleteAndZero (FileSizeStatus);
	deleteAndZero (NumBFchannelStatus);
	deleteAndZero (NumANfibreTypeStatus);
	deleteAndZero (NumANchannelStatus);
	deleteAndZero (FunctionExecStatus);
	deleteAndZero (WindowSizeStatus);
	deleteAndZero (MinBFstatus);
	deleteAndZero (MaxBFstatus);
	deleteAndZero (InputTypeStatus);
	deleteAndZero (ANfibreTypeStatus);
	deleteAndZero (RunStatus);
	deleteAndZero (Instructions);
}

void AuditoryPeripheryBinConverter::paint (Graphics& g)
{
	g.fillAll (Colours::lightgrey);

	BinFileStatus->setText (String(cBinFileStatus), dontSendNotification);
	TxtFileStatus->setText (String(cTxtFileStatus), dontSendNotification);
	BinFileName->setText (BinFileNameString + " selected", dontSendNotification);
	TextFileName->setText (TextFileNameString + " selected", dontSendNotification);
	FileSizeStatus->setText ("Binary file size: " + String (ConversionProgressThread::RTAP.FileLength) + (" bytes"), dontSendNotification);
	if ((pHeaderBuff[4] == 0xFF) && (pHeaderBuff[5] == 0xFF) && (pHeaderBuff[6] == 0xFF) && (pHeaderBuff[7] == 0xFF))
		InputSineToneFreqStatus->setText ("No sine, pulse or sawtooth tone used", dontSendNotification);
	else
		InputSineToneFreqStatus->setText ("Input tone frequency: " + String (ConversionProgressThread::RTAP.uiToneFreq) + " Hz", dontSendNotification);
	NumBFchannelStatus->setText ("Num of BF channels: " + String (ConversionProgressThread::RTAP.uiNumBFchannels), dontSendNotification);
	NumANfibreTypeStatus->setText ("Num of AN fibre types: " + String (RTAP.uiNumANfibreTypes), dontSendNotification);
	NumANchannelStatus->setText ("Num of AN channels: " + String (RTAP.uiNumANchannels), dontSendNotification);
	WindowSizeStatus->setText ("Window size per BF channel: " + String (RTAP.uiProcDataSizePerBF), dontSendNotification);
	FunctionExecStatus->setText (String (ConversionProgressThread::cFunctionExecString), dontSendNotification);
	MinBFstatus->setText ("Minimum BF: " + String (RTAP.uiBFMin) + " Hz", dontSendNotification);
	MaxBFstatus->setText ("Maximum BF: " + String (RTAP.uiBFMax) + " Hz", dontSendNotification);
	InputTypeStatus->setText (ConversionProgressThread::cInputType, dontSendNotification);
	ANfibreTypeStatus->setText (ConversionProgressThread::cANfibreType, dontSendNotification);
	RunStatus->setText (cRunStatus, dontSendNotification);
	Instructions->setText ("First select & open a binary RTAP file by clicking on the 'Load' button.\nThen set the text file name by clicking 'Save As'.\nFinally click on the 'Convert' button to begin conversion.", dontSendNotification);
}

void AuditoryPeripheryBinConverter::resized()
{
	LoadRTAPbinFileBtn->setBounds (50, 100, 150, 35);
	ConvertRTAPbinFileBtn->setBounds (250, 100, 150, 35);
	SaveAsRTAPtxtFileBtn->setBounds (450, 100, 150, 35);
	RunStatus->setBounds (50, 180, 400, 24);
	BinFileStatus->setBounds (50, 200, 400, 24);
	TxtFileStatus->setBounds (50, 220, 400, 24);
	BinFileName->setBounds (50, 240, 800, 24);
	TextFileName->setBounds (50, 260, 800, 24);
	FileSizeStatus->setBounds (50, 280, 400, 24);
	InputTypeStatus->setBounds (50, 300, 400, 24);
	InputSineToneFreqStatus->setBounds (50, 320, 400, 24);
	NumBFchannelStatus->setBounds (50, 340, 400, 24);
	NumANfibreTypeStatus->setBounds (50, 360, 400, 24);
	NumANchannelStatus->setBounds (50, 380, 400, 24);
	FunctionExecStatus->setBounds (50, 400, 400, 24);
	WindowSizeStatus->setBounds (50, 420, 400, 24);
	MinBFstatus->setBounds (50, 440, 400, 24);
	MaxBFstatus->setBounds (50, 460, 400, 24);
	ANfibreTypeStatus->setBounds (50, 480, 400, 24);
	Instructions->setBounds (50, 20, 500, 72);
}

void AuditoryPeripheryBinConverter::buttonClicked (Button* buttonThatWasClicked)
{
	if 	(buttonThatWasClicked == LoadRTAPbinFileBtn)
	{
		strncpy (cRunStatus, "'Load' button clicked", STRINGSIZE);
		LoadBinRTAPfile ();
	}
	else if (buttonThatWasClicked == ConvertRTAPbinFileBtn)
	{
		// Convert only when the 'Save As' button is clicked
		if (bSaveAsBtnClicked)
		{
			strncpy (cRunStatus, "'Convert' button clicked", STRINGSIZE);
			if (runThread())
			{
				AlertWindow::showMessageBoxAsync (AlertWindow::WarningIcon, "RTAP binary file conversion in process", "RTAP binary file converted!");
			}
			else
			{
				AlertWindow::showMessageBoxAsync (AlertWindow::WarningIcon, "RTAP binary file conversion in process", "Unable to convert RTAP binary file!");
			}
		}
		else	strncpy (cRunStatus, "Unable to convert: Text file not specficied. Click on 'Save As' button", STRINGSIZE);

//		MainFunc();
	}
	else if (buttonThatWasClicked == SaveAsRTAPtxtFileBtn)
	{
		strncpy (cRunStatus, "'Save As' button clicked", STRINGSIZE);
		SaveAsRTAPtxtFile ();
		bSaveAsBtnClicked = true;
	}
}

void AuditoryPeripheryBinConverter::LoadBinRTAPfile ()
{
	FileChooser binFC ("Load binary RTAP file", File::getCurrentWorkingDirectory (), "*.brtap", true);
	
	if (binFC.browseForFileToOpen ())
	{
		RTAPbinFile = binFC.getResult ();
		BinFileNameString = RTAPbinFile.getFileName ();

		if (RTAPbinFile.existsAsFile ())
		{
			strncpy (cBinFileStatus, "Binary file opened", STRINGSIZE);

			RTAP.FileLength = RTAPbinFile.getSize ();

			// read header first
			if (brtapReadStream = RTAPbinFile.createInputStream ())
			{
				brtapReadStream->read (pHeaderBuff, SIZEOFHEADER);

				// Acquire binary rtap file details for display
				FormatRTAPheader (pHeaderBuff);
			}
		}
		else	strncpy (cBinFileStatus, "Binary file not opened", STRINGSIZE);
	}
}

void AuditoryPeripheryBinConverter::SaveAsRTAPtxtFile ()
{
	FileChooser txtFC ("Save text RTAP file as", File::getCurrentWorkingDirectory (), ".txt", true);

	if (txtFC.browseForFileToSave (true))
	{
		RTAPtxtFile = txtFC.getResult ();
		// Dump RTAP header ID for starters to create the text file
		ProxyBuff.clear ();
		ProxyBuff = "Converted from ";
		ProxyBuff.append (BinFileNameString, BinFileNameString.length ());
		ProxyBuff.append ("\n", 3);
		RTAPtxtFile.appendText (ProxyBuff, false, false);

		if (RTAPtxtFile.existsAsFile())
		{
			strncpy (cTxtFileStatus, "Text file created", STRINGSIZE);
		}
		else strncpy (cTxtFileStatus, "Unable to create text file", STRINGSIZE);
	}
}
#if 0
void AuditoryPeripheryBinConverter::ConvertBinRTAPfile ()
{
	bExit = false;
	long dataSize = RTAP.FileLength - SIZEOFHEADER;
	uiFPoffset = 0;
	RTAP.uiByteOffset = 0;
	RTAP.uiStartPoint = 0;
	RTAP.uiCurrBF = 0;
	RTAP.uiCurrANfibreType = 0;
	nbytes = 5000;

	RTAP_logspace (RTAP.fBFlist, (float) RTAP.uiBFMin, (float) RTAP.uiBFMax, RTAP.uiNumBFchannels);
	WriteHeaderToTextFile ();

	// pDataBuff size is sizeof(float)*perBFsize + perBFsize
	// last param perBFsize is an allowance to cater for extra bytes to be read in if rogue bytes encountered
	pDataBuff = new char [(RTAP.uiProcDataSizePerBF<<2) + RTAP.uiProcDataSizePerBF];

	FPdataSize = RTAP.FileLength - SIZEOFHEADER;

	for (i=0; i<dataSize; i+=(RTAP.uiProcDataSizePerBF << 2))
	{
		// check if file size has become smaller than default size - > smaller segment indicates end of file approaching
		if ((RTAP.FileLength - i) < (RTAP.uiProcDataSizePerBF << 2))
		{
			RTAP.uiProcDataSizePerBF = ((RTAP.FileLength - i) >> 2);
//			printf("No. of samples in last segment of RTAP binary file: %d\n", RTAP.uiProcDataSizePerBF);
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
	}

	delete pDataBuff;
}

void AuditoryPeripheryBinConverter::MainFunc()
{
	bExit = false;

	uiFPoffset = 0;
	RTAP.uiByteOffset = 0;
	RTAP.uiStartPoint = 0;
	RTAP.uiCurrBF = 0;
	RTAP.uiCurrANfibreType = 0;
	nbytes = 5000;

	err = fopen_s (&pRTAPbinFile, "bin/Datadump.brtap", "rb");

//	if (err == 0)
	{
		strncpy (cBinFileStatus, "bin\Datadump.brtap opened", STRINGSIZE);
		fseek (pRTAPbinFile, 0, SEEK_END);
		RTAP.FileLength = ftell (pRTAPbinFile);
		rewind (pRTAPbinFile);							// position file pointer to the front of the file
	
		fread (pHeaderBuff, 1, SIZEOFHEADER, pRTAPbinFile);

		fclose (pRTAPbinFile);
	}
//	else	strncpy (cBinFileStatus, "bin\Datadump.brtap not opened", STRINGSIZE);
#if 0
	FormatRTAPheader (pHeaderBuff);

	RTAP_logspace (RTAP.fBFlist, (float) RTAP.uiBFMin, (float) RTAP.uiBFMax, RTAP.uiNumBFchannels);
	WriteHeaderToTextFile ();

	// pDataBuff size is sizeof(float)*perBFsize + perBFsize
	// last param perBFsize is an allowance to cater for extra bytes to be read in if rogue bytes encountered
	pDataBuff = new char [(RTAP.uiProcDataSizePerBF<<2) + RTAP.uiProcDataSizePerBF];

	FPdataSize = RTAP.FileLength - SIZEOFHEADER;

	for (i=0; i<(RTAP.FileLength-SIZEOFHEADER); i+=(RTAP.uiProcDataSizePerBF << 2))
	{
		// check if file size has become smaller than default size - > smaller segment indicates end of file approaching
		if ((RTAP.FileLength - i) < (RTAP.uiProcDataSizePerBF << 2))
		{
			RTAP.uiProcDataSizePerBF = ((RTAP.FileLength - i) >> 2);
//			printf("No. of samples in last segment of RTAP binary file: %d\n", RTAP.uiProcDataSizePerBF);
			bExit = true;
		}
		// read size of float * BF channel size = 4 * (normally) 2560 (based on 22.05 kHz)
		uiBytesToRead = RTAP.uiProcDataSizePerBF<<2;

		fread (pDataBuff, sizeof(float), RTAP.uiProcDataSizePerBF, pRTAPbinFile);

		// reset char buff pointer
		uiCharBuffOffset = 0;
		// set the end point for char buff
		RTAP.uiCharBuffEndPoint = RTAP.uiProcDataSizePerBF - 1;

		for (j=0; j<RTAP.uiProcDataSizePerBF; j++)
		{
			memcpy ((char*) &RTAP.fProcData[j], &pDataBuff[uiCharBuffOffset], 4);

			ProxyBuff = fAbs(RTAP.fProcData[j]);
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
	}
#endif
	
//	delete pDataBuff;
}
#endif
void AuditoryPeripheryBinConverter::FormatRTAPheader (char* pHeader)
{
	RTAP.pFileType[0] = pHeader[0];
	RTAP.pFileType[1] = pHeader[1];
	RTAP.pFileType[2] = pHeader[2];
	RTAP.pFileType[3] = pHeader[3];
	
	memcpy ((char*) &RTAP.cAudioSrc, pHeader+4, 1);
	memcpy ((char*) &RTAP.uiToneFreq, pHeader+5, 4);
	memcpy ((char*) &RTAP.uiNumBFchannels, pHeader+9, 4);
	memcpy ((char*) &RTAP.uiNumANfibreTypes, pHeader+13, 4);
	memcpy ((char*) &RTAP.uiNumANchannels, pHeader+17, 4);
	memcpy ((char*) &RTAP.uiFunctionRunning, pHeader+21, 4);
	memcpy ((char*) &RTAP.uiProcDataSizePerBF, pHeader+25, 4);
	memcpy ((char*) &RTAP.uiBFMin, pHeader+29, 4);
	memcpy ((char*) &RTAP.uiBFMax, pHeader+33, 4);
	memcpy ((char*) &RTAP.iANfibreType, pHeader+37, 4);

	switch (RTAP.cAudioSrc)
	{
		case AUDIO_SYSTEM:
			strncpy (ConversionProgressThread::cInputType, "System audio input", STRINGSIZE);
			break;
		case AUDIO_SINE:
			strncpy (ConversionProgressThread::cInputType, "Sine input", STRINGSIZE);
			break;
		case AUDIO_SQUARE:
			strncpy (ConversionProgressThread::cInputType, "Squeare input", STRINGSIZE);
			break;
		case AUDIO_SAWTOOTH:
			strncpy (ConversionProgressThread::cInputType, "Sawtooth input", STRINGSIZE);
			break;
		default:
			strncpy (ConversionProgressThread::cInputType, "Error reading audio input parameter", STRINGSIZE);
			break;
	}
	switch (RTAP.uiFunctionRunning)
	{
		case AUDIO_IN:
			strncpy (ConversionProgressThread::cFunctionExecString, "Auditory stimulus input data", STRINGSIZE);
			break;
		case EER_OUT:
			strncpy (ConversionProgressThread::cFunctionExecString, "Acoustical stimulus pressure (Pascal)", STRINGSIZE);
			break;
		case TM_DISP:
			strncpy (ConversionProgressThread::cFunctionExecString, "Tympanic membrane displacement (metres)", STRINGSIZE);
			break;
		case STAPES_DISP:
			strncpy (ConversionProgressThread::cFunctionExecString, "Stapes displacement (metres)", STRINGSIZE);
			break;
		case DRNL_ST:
			strncpy (ConversionProgressThread::cFunctionExecString, "Single threaded BM displacement (metres)", STRINGSIZE);
			break;
		case DRNL2IHCRP_ST:
			strncpy (ConversionProgressThread::cFunctionExecString, "Single threaded IHC receptor potential (Volts)", STRINGSIZE);
			break;
		case DRNL2IHCpresynapse_ST:
			strncpy (ConversionProgressThread::cFunctionExecString, "Single threaded IHC neurotransmitter release (Number of neurotransmitter release)", STRINGSIZE);
			if (RTAP.iANfibreType == 1)	strncpy (ConversionProgressThread::cANfibreType, "AN fibre type: LSR", STRINGSIZE);
			else if (RTAP.iANfibreType == 2) strncpy (ConversionProgressThread::cANfibreType, "AN fibre type: HSR", STRINGSIZE);
			break;
		case DRNL2ANsynapse_ST:
			strncpy (ConversionProgressThread::cFunctionExecString, "Single threaded AN spiking (Number of spikes)", STRINGSIZE);
			if (RTAP.iANfibreType == 1)	strncpy (ConversionProgressThread::cANfibreType, "AN fibre type: LSR", STRINGSIZE);
			else if (RTAP.iANfibreType == 2) strncpy (ConversionProgressThread::cANfibreType, "AN fibre type: HSR", STRINGSIZE);
			break;
		case DRNL_ST_OPT:
			strncpy (ConversionProgressThread::cFunctionExecString, "Single threaded optimised BM displacement (metres)", STRINGSIZE);
			break;
		case DRNL2IHCRP_ST_OPT:
			strncpy (ConversionProgressThread::cFunctionExecString, "Single threaded optimised IHC receptor potential (Volts)", STRINGSIZE);
			break;
		case DRNL2IHCpresynapse_ST_OPT:
			strncpy (ConversionProgressThread::cFunctionExecString, "Single threaded optimised IHC neurotransmitter release (Number of neurotransmitter release)", STRINGSIZE);
			if (RTAP.iANfibreType == 1)	strncpy (ConversionProgressThread::cANfibreType, "AN fibre type: LSR", STRINGSIZE);
			else if (RTAP.iANfibreType == 2) strncpy (ConversionProgressThread::cANfibreType, "AN fibre type: HSR", STRINGSIZE);
			break;
		case DRNL2ANsynapse_ST_OPT:
			strncpy (ConversionProgressThread::cFunctionExecString, "Single threaded optimised AN spiking (Number of spikes)", STRINGSIZE);
			if (RTAP.iANfibreType == 1)	strncpy (ConversionProgressThread::cANfibreType, "AN fibre type: LSR", STRINGSIZE);
			else if (RTAP.iANfibreType == 2) strncpy (ConversionProgressThread::cANfibreType, "AN fibre type: HSR", STRINGSIZE);
			break;
		case DRNL_MT:
			strncpy (ConversionProgressThread::cFunctionExecString, "Multi-threaded BM displacement (metres)", STRINGSIZE);
			break;
		case DRNL2IHCRP_MT:
			strncpy (ConversionProgressThread::cFunctionExecString, "Multi-threaded IHC receptor potential (Volts)", STRINGSIZE);
			break;
		case DRNL2IHCpresynapse_MT:
			strncpy (ConversionProgressThread::cFunctionExecString, "Multi-threaded IHC neurotransmitter (Number of neurotransmitter release)", STRINGSIZE);
			if (RTAP.iANfibreType == 1)	strncpy (ConversionProgressThread::cANfibreType, "AN fibre type: LSR", STRINGSIZE);
			else if (RTAP.iANfibreType == 2) strncpy (ConversionProgressThread::cANfibreType, "AN fibre type: HSR", STRINGSIZE);
			break;
		case DRNL2ANsynapse_MT:
			strncpy (ConversionProgressThread::cFunctionExecString, "Multi-threaded AN spiking (Number of spikes)", STRINGSIZE);
			if (RTAP.iANfibreType == 1)	strncpy (ConversionProgressThread::cANfibreType, "AN fibre type: LSR", STRINGSIZE);
			else if (RTAP.iANfibreType == 2) strncpy (ConversionProgressThread::cANfibreType, "AN fibre type: HSR", STRINGSIZE);
			break;
		default:
			strncpy (ConversionProgressThread::cFunctionExecString, "Invalid function ID", STRINGSIZE);
			break;
	}
}
#if 0
void AuditoryPeripheryBinConverter::WriteHeaderToTextFile ()
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
	ProxyBuff.append (cInputType, 15);
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

void AuditoryPeripheryBinConverter::FormatFPdata (char* pFPdata)
{
	int i, temp;
	
	for (i=0; i<RTAP.uiProcDataSizePerBF; i++)
	{
		temp = i<<2;
		memcpy ( (char*) &RTAP.fProcData[i], &pFPdata[temp+RTAP.uiByteOffset], 4 );

		if (!RTAP.fProcData[i])
		{
			if (pFPdata[temp] || pFPdata[temp+1] || pFPdata[temp+2] || pFPdata[temp+3])
			{
				RTAP.uiByteOffset++;
				memcpy ( (char*) &RTAP.fProcData[i], &pFPdata[temp+RTAP.uiByteOffset], 4 );
			}
		}
	}
}

void AuditoryPeripheryBinConverter::WriteFPdataToTextFile (unsigned int uiSet)
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
//		fprintf (pTxtFile, \n");
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
//		fprintf (pTxtFile, "BF Channel %d: %f Hz\n", RTAP.uiCurrBF, RTAP.fBFlist[RTAP.uiCurrBF-1]);
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
		//fprintf (pTxtFile, "%.15f\n", RTAP.fProcData[i]);
		ProxyBuff.clear ();
		ProxyBuff.append (String(RTAP.fProcData[i]), 17);
		ProxyBuff.append ("\n", 3);
		RTAPtxtFile.appendText (ProxyBuff, false, false);
	}
#endif
}

float AuditoryPeripheryBinConverter::fAbs (float fNum)
{
	if (fNum < 0)
		fNum = -fNum;			// ensure all numbers are +ve

	return (fNum);
}

void AuditoryPeripheryBinConverter::RTAP_logspace (float* fNum, float fXlower, float fXupper, unsigned int uiNumPoints)
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
#endif