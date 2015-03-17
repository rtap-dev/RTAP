#include <stdlib.h>
#include "RTAPmainGUI.h"

class SettingProgressStatus : public ThreadWithProgressWindow
{
public:
	SettingProgressStatus() : ThreadWithProgressWindow ("Configuring RTAP", true, true)
	{
		setStatusMessage("Preparing");
	}
	void run()	override
	{
		setProgress (-1.0);
		setStatusMessage ("Running RTAP configuration");
		wait(400);

		setProgress (uiRTAPsetStatus / dRTAPsetThingsToDo);
		switch (uiRTAPsetStatus)
		{
			case MemFreed:
				setStatusMessage ("Memory freed ...");
				wait(200);
				break;
			case MemReallocated:
				setStatusMessage ("Memory reallocated ...");
				wait(200);
				break;
			case APparamsSet:
				setStatusMessage ("RTAP parameters set ...");
				wait(200);
				break;
			case APalgoSet:
				setStatusMessage ("RTAP algorithms initialised ...");
				wait(200);
				break;
			case ReinitialiseCompleted:
				setStatusMessage ("RTAP reinitialisating ...");
				wait(200);
				break;
		}
		wait(400);
	}
};

RTAPmainGUI::RTAPmainGUI (AudioDeviceManager& deviceManager_)
    : deviceManager (deviceManager_),
      liveAudioDisplayComp (0),
	  JUCEliveDisplay(0),
#if TEST_AUDIODEVICESETUP
	  SetAudioDevice(0),
	  GetAudioDevice(0),
	  SamplingFreqSlider(0),
#endif
	  PlayPauseButton(0),
	  ProcDataRecordButton(0),
	  SetParamsButton(0),
	  PlayRecButton(0),
	  RefreshPlotButton(0),
#if DOUBLE_PRECISION_BUTTON
	  DoublePrecisionBtn(0),
#endif
	  APfunction2RunComboBox(0),
	  APfunction2RecComboBox(0),
	  RTAPpriorityComboBox(0),
	  WindowBoundaryDispBtn(0),
	  PeakAmpDispBtn(0),
	  GetAudSampProcTime(0),
	  OMEProcTime(0),
	  ProcThread1Status(0),
	  ProcThread2Status(0),
	  FileWriterThread1Status(0),
	  FileWriterThread2Status(0),
	  FileWriterThreadProcTime(0),
	  PixDrawThread1Status(0),
	  PixDrawThread2Status(0),
	  ProcessPriority(0),
	  AlgoProcTimeThread1(0),
	  AlgoProcTimeThread2(0),
	  AudioDevIOCallbackProcTime(0),
	  PlotRenderThreadTime(0),
	  Plot2ScreenTime(0),
	  MaxPeak1(0),
	  MaxPeak2(0),
	  MaxPeak3(0)
#if 0
	  SampInPixSlider(0)
#endif
{
    addAndMakeVisible (liveAudioDisplayComp = new LiveAudioInput ());

	// -------- Processed data save to bin file button start --------
	addAndMakeVisible (ProcDataRecordButton = new TextButton (String::empty));
    ProcDataRecordButton->setButtonText ("Record");
    ProcDataRecordButton->addListener (this);
    ProcDataRecordButton->setColour (TextButton::buttonColourId, Colours::bisque);
    ProcDataRecordButton->setColour (TextButton::textColourOffId, Colours::black);
	// -------- Processed data save to bin file button end --------

	// -------- set auditory periphery (AP) system parameters button start --------
	addAndMakeVisible (SetParamsButton = new TextButton (String::empty));
	SetParamsButton->setButtonText ("Set");
	SetParamsButton->addListener (this);
	SetParamsButton->setColour (TextButton::buttonColourId, Colours::bisque);
	SetParamsButton->setColour (TextButton::textColourOffId, Colours::black);
	// -------- set auditory periphery (AP) system parameters button end --------

	// -------- 'Play + Rec' button start --------
	addAndMakeVisible (PlayRecButton = new TextButton (String::empty));
	PlayRecButton->setButtonText ("Play + Rec");
	PlayRecButton->addListener (this);
	PlayRecButton->setColour (TextButton::buttonColourId, Colours::bisque);
	PlayRecButton->setColour (TextButton::textColourOffId, Colours::black);
	// -------- 'Play + Pause' button end --------

	// -------- 'Refresh Plot' button start --------
	addAndMakeVisible (RefreshPlotButton = new TextButton (String::empty));
	RefreshPlotButton->setButtonText ("Refresh Plot");
	RefreshPlotButton->addListener (this);
	RefreshPlotButton->setColour (TextButton::buttonColourId, Colours::bisque);
	RefreshPlotButton->setColour (TextButton::textColourOffId, Colours::black);
	// --------- 'Display Plot' button end ---------

	addAndMakeVisible(JUCEliveDisplay = new RTAPgfx());

#if TEST_AUDIODEVICESETUP
	SetAudioDevice = new AudioDeviceManager::AudioDeviceSetup();
	GetAudioDevice = new AudioDeviceManager::AudioDeviceSetup();
#endif

#if DOUBLE_PRECISION_BUTTON
	// -------- Double precision toggle button start --------
	addAndMakeVisible ( DoublePrecisionBtn = new ToggleButton(String::empty) );
	DoublePrecisionBtn->setButtonText ( T("Double precision") );
	DoublePrecisionBtn->setTooltip ( "64-bit computation" );
	DoublePrecisionBtn->addListener ( this );
	DoublePrecisionBtn->setClickingTogglesState ( true );
	DoublePrecisionBtn->setToggleState ( false, false );
	// -------- Double precision toggle button end ----------
#endif

	// -------- Play/Pause Button display start --------
	DrawablePath normal, over, down, disabled;
	
	Path p;
	p.addTriangle (50,50,60,55,50,60);
	normal.setPath (p);
	normal.setFill (Colours::black);
	p.clear ();
	p.addTriangle (50,50,60,55,50,60);
	over.setPath (p);						// cursor over button
	over.setFill (Colours::khaki);			// triangle colour
	over.setStrokeFill (Colours::khaki);	// button outline colour
	over.setStrokeThickness (5.0f);
	p.clear();
	p.addTriangle (50,50,60,55,50,60);
	down.setPath (p);						// button pressed
	down.setFill (Colours::blue);
	down.setStrokeFill (Colours::blue);
	down.setStrokeThickness (5.0f);
	p.clear ();
	p.addTriangle (40,50,60,55,50,60);
	disabled.setPath (p);
	disabled.setFill (Colours::grey);
	disabled.setStrokeFill (Colours::grey);
	disabled.setStrokeThickness (5.0f);

	PlayPauseButton = new DrawableButton ( "Play/Pause", DrawableButton::ImageOnButtonBackground );
	PlayPauseButton->setImages ( &normal, &over, &down, &disabled );
	PlayPauseButton->setClickingTogglesState ( true );
	PlayPauseButton->setColour (TextEditor::backgroundColourId, Colour (0x32ffffff));

	addAndMakeVisible ( PlayPauseButton );
//	PlayPauseButton->setColour(TextButton::buttonColourId, Colours::goldenrod);		// set button colour
//	PlayPauseButton->setColour(TextButton::textColourOffId, Colours::black);	// set text colour
	PlayPauseButton->addListener ( this );
	// -------- Play/Pause Button display end --------

	// -------- Auditory periphery function combo box selection computation start --------
	addAndMakeVisible (APfunction2RunLabel = new Label (String::empty, "Function to compute"));
	APfunction2RunLabel->setJustificationType (Justification::left);
	APfunction2RunLabel->setEditable (false, false, false);
	APfunction2RunLabel->setColour (TextEditor::textColourId, Colours::black);
	addAndMakeVisible (APfunction2RunComboBox = new ComboBox ("Function to Run"));
	APfunction2RunComboBox->setEditableText (false);
	APfunction2RunComboBox->setJustificationType (Justification::centred);

	APfunction2RunComboBox->addItem ("BM displacement", DRNL_ST_OPT);
	APfunction2RunComboBox->addItem ("IHC receptor potential", DRNL2IHCRP_ST_OPT);
	APfunction2RunComboBox->addItem ("IHC vesicle release", DRNL2IHCpresynapse_ST_OPT);
	APfunction2RunComboBox->addItem ("AN spiking", DRNL2ANsynapse_ST_OPT);
#if 0
	APfunction2RunComboBox->addItem ( "BM displacement (MT)", DRNL_MT );
	APfunction2RunComboBox->addItem ( "IHC receptor potential (MT)", DRNL2IHCRP_MT );
	APfunction2RunComboBox->addItem ( "IHC vesicle release (MT)", DRNL2IHCpresynapse_MT );
	APfunction2RunComboBox->addItem ( "AN spiking (MT)", DRNL2ANsynapse_MT );
#endif
	APfunction2RunComboBox->addListener (this);
	// -------- Auditory periphery function combo box selection computation end --------

	// -------- Auditory periphery function combo box selection record start --------
	addAndMakeVisible ( APfunction2RecLabel = new Label (String::empty, "Function to record"));
	APfunction2RecLabel->setJustificationType (Justification::left);
	APfunction2RecLabel->setEditable (false, false, false);
	APfunction2RecLabel->setColour (TextEditor::textColourId, Colours::black);
	addAndMakeVisible (APfunction2RecComboBox = new ComboBox ("Function to Rec"));
	APfunction2RecComboBox->setEditableText (false);
	APfunction2RecComboBox->setJustificationType (Justification::centred);
	// only display OME function to record. For other functions, tie in with func to run
	APfunction2RecComboBox->addItem ("Audio input", AUDIO_IN);
	APfunction2RecComboBox->addItem ("Acoustic pressure", EER_OUT);
	APfunction2RecComboBox->addItem ("TM displacement", TM_DISP);
	APfunction2RecComboBox->addItem ("Stapes displacement", STAPES_DISP);
#if 0
	APfunction2RecComboBox->addItem ("BM displacement", DRNL_ST_OPT);
//	APfunction2RecComboBox->addItem ("BM displacement (ST)", DRNL_ST);
	APfunction2RecComboBox->addItem ("IHC receptor potential", DRNL2IHCRP_ST_OPT);
	APfunction2RecComboBox->addItem ("IHC vesicle release", DRNL2IHCpresynapse_ST_OPT);
	APfunction2RecComboBox->addItem ("AN spiking", DRNL2ANsynapse_ST_OPT);

	APfunction2RecComboBox->addItem ("BM displacement (MT)", DRNL_MT);
	APfunction2RecComboBox->addItem ("IHC receptor potential (MT)", DRNL2IHCRP_MT);
	APfunction2RecComboBox->addItem ("IHC vesicle release (MT)", DRNL2IHCpresynapse_MT);
	APfunction2RecComboBox->addItem ("AN spiking (MT)", DRNL2ANsynapse_MT);
#endif
	APfunction2RecComboBox->addListener (this);
	// -------- Auditory periphery function combo box selection record end --------

	// -------- Auditory periphery function combo box selection record start --------
	addAndMakeVisible (RTAPpriorityLabel = new Label (String::empty, "RTAP Priority"));
	RTAPpriorityLabel->setJustificationType (Justification::left);
	RTAPpriorityLabel->setEditable (false, false, false);
	RTAPpriorityLabel->setColour (TextEditor::textColourId, Colours::black);
	addAndMakeVisible (RTAPpriorityComboBox = new ComboBox ("RTAPpriority"));
	RTAPpriorityComboBox->setEditableText (false);
	RTAPpriorityComboBox->setJustificationType (Justification::centred);
	RTAPpriorityComboBox->addItem ("REAL-TIME", REAL_TIME);
	RTAPpriorityComboBox->addItem ("HIGH", HIGH);
	RTAPpriorityComboBox->addItem ("ABOVE_NORMAL", ABOVE_NORMAL);
	RTAPpriorityComboBox->addItem ("NORMAL", NORMAL);
	RTAPpriorityComboBox->addListener (this);
	// -------- Auditory periphery function combo box selection record end --------

#if TEST_AUDIODEVICESETUP
	// -------- Sampling frequency adjustment slider display start --------
	addAndMakeVisible(SamplingFreqLabel = new Label (String::empty,T ("Sampling frequency")));
	SamplingFreqLabel->setJustificationType (Justification::centred);
	SamplingFreqLabel->setEditable (false, false, false);
	SamplingFreqLabel->setColour (TextEditor::textColourId, Colours::black);
	addAndMakeVisible (SamplingFreqSlider = new Slider(String::empty));
	SamplingFreqSlider->setSliderStyle (Slider::LinearHorizontal);
	SamplingFreqSlider->setTextBoxStyle (Slider::TextBoxLeft, false, 80, 20);
	SamplingFreqSlider->setRange (8000.f, 22050.f, 50.f);
	SamplingFreqSlider->addListener (this);
	// -------- Sampling frequency adjustment slider display end --------
#endif

	// -------- Window boundary display toggle button start --------
	addAndMakeVisible (WindowBoundaryDispBtn = new ToggleButton(String::empty));
	WindowBoundaryDispBtn->setButtonText ("Window boundary display");
	WindowBoundaryDispBtn->setTooltip ("Draw window boundaries on the processed signal display (for scatter & line plots only)");
	WindowBoundaryDispBtn->addListener (this);
	WindowBoundaryDispBtn->setClickingTogglesState (true);
	WindowBoundaryDispBtn->setToggleState (false, false);
	// -------- Window boundary display toggle button end ----------

	// -------- Peak BF channel(s) display toggle button start --------
	addAndMakeVisible (PeakAmpDispBtn = new ToggleButton(String::empty));
	PeakAmpDispBtn->setButtonText ("Peak BF display");
	PeakAmpDispBtn->setTooltip ("Draw window boundaries on the BF signal with largest amplitude (for scatter & line plots only)");
	PeakAmpDispBtn->addListener (this);
	PeakAmpDispBtn->setClickingTogglesState (true);
	PeakAmpDispBtn->setToggleState (false, false);
	// -------- Peak BF channel(s) display toggle button end ----------

#if DISPLAY_PROCTIME
	// -------- Function processing time display labels start --------
	addAndMakeVisible (GetAudSampProcTime = new Label (String::empty, "Get aud samp: "));
	GetAudSampProcTime->setJustificationType (Justification::centredLeft);
	GetAudSampProcTime->setEditable (false, false, false);
	GetAudSampProcTime->setColour (TextEditor::textColourId, Colours::black);

	addAndMakeVisible (OMEProcTime = new Label (String::empty, "OME: "));
	OMEProcTime->setJustificationType (Justification::centredLeft);
	OMEProcTime->setEditable (false, false, false);
	OMEProcTime->setColour (TextEditor::textColourId, Colours::black);

	addAndMakeVisible (ProcessPriority = new Label (String::empty, "Process priority: "));
	ProcessPriority->setJustificationType (Justification::centredLeft);
	ProcessPriority->setEditable (false, false, false);
	ProcessPriority->setColour (TextEditor::textColourId, Colours::black);
	addAndMakeVisible (ProcThread1Status = new Label (String::empty, "AP thread 1 idle"));
	ProcThread1Status->setJustificationType (Justification::centredLeft);
	ProcThread1Status->setEditable (false, false, false);
	ProcThread1Status->setColour (TextEditor::textColourId, Colours::black);
	addAndMakeVisible (ProcThread2Status = new Label (String::empty, "AP thread 2 idle"));
	ProcThread2Status->setJustificationType (Justification::centredLeft);
	ProcThread2Status->setEditable (false, false, false);
	ProcThread2Status->setColour (TextEditor::textColourId, Colours::black);
	addAndMakeVisible (FileWriterThread1Status = new Label (String::empty, "File writer thread 1 idle"));
	FileWriterThread1Status->setJustificationType (Justification::centredLeft);
	FileWriterThread1Status->setEditable (false, false, false);
	FileWriterThread1Status->setColour (TextEditor::textColourId, Colours::black);
	addAndMakeVisible (FileWriterThread2Status = new Label (String::empty, "File writer thread 2 idle"));
	FileWriterThread2Status->setJustificationType (Justification::centredLeft);
	FileWriterThread2Status->setEditable (false, false, false);
	FileWriterThread2Status->setColour (TextEditor::textColourId, Colours::black);
	addAndMakeVisible (PixDrawThread1Status = new Label (String::empty, "Pix draw thread 1 idle"));
	PixDrawThread1Status->setJustificationType (Justification::centredLeft);
	PixDrawThread1Status->setEditable (false, false, false);
	PixDrawThread1Status->setColour (TextEditor::textColourId, Colours::black);
	addAndMakeVisible (PixDrawThread2Status = new Label (String::empty, "Pix draw thread 2 idle" ));
	PixDrawThread2Status->setJustificationType (Justification::centredLeft);
	PixDrawThread2Status->setEditable (false, false, false);
	PixDrawThread2Status->setColour (TextEditor::textColourId, Colours::black);

	addAndMakeVisible (AlgoProcTimeThread1 = new Label (String::empty, ""));
	AlgoProcTimeThread1->setJustificationType (Justification::centredLeft);
	AlgoProcTimeThread1->setEditable (false, false, false);
	AlgoProcTimeThread1->setColour (TextEditor::textColourId, Colours::black);
	addAndMakeVisible (AlgoProcTimeThread2 = new Label (String::empty, ""));
	AlgoProcTimeThread2->setJustificationType (Justification::centredLeft);
	AlgoProcTimeThread2->setEditable (false, false, false);
	AlgoProcTimeThread2->setColour (TextEditor::textColourId, Colours::black);

	addAndMakeVisible (AudioDevIOCallbackProcTime = new Label (String::empty, "Gammatone: "));
	AudioDevIOCallbackProcTime->setJustificationType (Justification::centredLeft);
	AudioDevIOCallbackProcTime->setEditable (false, false, false);
	AudioDevIOCallbackProcTime->setColour (TextEditor::textColourId, Colours::black);

	addAndMakeVisible (PlotRenderThreadTime = new Label (String::empty, "Plot pixel time: "));
	PlotRenderThreadTime->setJustificationType (Justification::centredLeft);
	PlotRenderThreadTime->setEditable (false, false, false);
	PlotRenderThreadTime->setColour (TextEditor::textColourId, Colours::black);
	addAndMakeVisible (Plot2ScreenTime = new Label (String::empty, "Plot draw time: "));
	Plot2ScreenTime->setJustificationType (Justification::centredLeft);
	Plot2ScreenTime->setEditable (false, false, false);
	Plot2ScreenTime->setColour (TextEditor::textColourId, Colours::black);
	addAndMakeVisible (FileWriterThreadProcTime = new Label (String::empty, "File writer thread: "));
	FileWriterThreadProcTime->setJustificationType (Justification::centredLeft);
	FileWriterThreadProcTime->setEditable (false, false, false);
	FileWriterThreadProcTime->setColour (TextEditor::textColourId, Colours::black);
	// -------- Function processing time display labels end --------
#endif
	addAndMakeVisible (MaxPeak1 = new Label (String::empty));
	MaxPeak1->setJustificationType (Justification::centredLeft);
	MaxPeak1->setEditable (false, false, false);
	MaxPeak1->setColour (TextEditor::textColourId, Colours::black);
	addAndMakeVisible (MaxPeak2 = new Label (String::empty));
	MaxPeak2->setJustificationType (Justification::centredLeft);
	MaxPeak2->setEditable (false, false, false);
	MaxPeak2->setColour (TextEditor::textColourId, Colours::black);
	addAndMakeVisible (MaxPeak3 = new Label (String::empty));
	MaxPeak3->setJustificationType (Justification::centredLeft);
	MaxPeak3->setEditable (false, false, false);
	MaxPeak3->setColour (TextEditor::textColourId, Colours::black);

	// set default settings if there is no RTAP parameters update from XML
	if (!bXmlUpdate)
	{
		RTAPpriorityComboBox->setSelectedId (NORMAL);
		// set the default multi channel combo box to 1 channel
		APfunction2RunComboBox->setSelectedId (DRNL_ST_OPT);		// set default function to single-threaded DRNL
		APfunction2RecComboBox->setSelectedId (DRNL_ST_OPT);
	}
	// if there is an XML loading, then update RTAP parameters accordingly
	else
	{
		RTAPpriorityComboBox->setSelectedId (iRTAPpriority);
		APfunction2RunComboBox->setSelectedId (iFunctionToRun);
		APfunction2RecComboBox->setSelectedId (DRNL_ST_OPT);
		// set respective toggle buttons to XML settings
		if (bWindowBoundaryDisp)
			WindowBoundaryDispBtn->setToggleState (true, false);
		if (bPeakAmpDisp)
			PeakAmpDispBtn->setToggleState (true, false);
	}

#if TEST_AUDIODEVICESETUP
	SamplingFreqSlider->setValue (22050.f, true, false );
#endif

    deviceManager.addAudioCallback (liveAudioDisplayComp);
}

RTAPmainGUI::~RTAPmainGUI()
{
    deviceManager.removeAudioCallback (liveAudioDisplayComp);

	deleteAndZero (liveAudioDisplayComp);
	deleteAndZero (SetParamsButton);
	deleteAndZero (PlayRecButton);
	deleteAndZero (ProcDataRecordButton);
	deleteAndZero (RefreshPlotButton);
	deleteAndZero (JUCEliveDisplay);
#if TEST_AUDIODEVICESETUP
	deleteAndZero (SetAudioDevice);
	deleteAndZero (GetAudioDevice);
	deleteAndZero (SamplingFreqLabel);
	deleteAndZero (SamplingFreqSlider);
#endif
	deleteAndZero (PlayPauseButton);
//	deleteAndZero (ImpulseResponse);
#if DOUBLE_PRECISION_BUTTON
	deleteAndZero (DoublePrecisionBtn);
#endif
	deleteAndZero (APfunction2RunLabel);
	deleteAndZero (APfunction2RunComboBox);
	deleteAndZero (APfunction2RecLabel);
	deleteAndZero (APfunction2RecComboBox);
	deleteAndZero (RTAPpriorityLabel);
	deleteAndZero (RTAPpriorityComboBox);
	deleteAndZero (WindowBoundaryDispBtn);
	deleteAndZero (PeakAmpDispBtn);
#if DISPLAY_PROCTIME
	deleteAndZero (GetAudSampProcTime);
	deleteAndZero (OMEProcTime);
	deleteAndZero (ProcThread1Status);
	deleteAndZero (ProcThread2Status);
	deleteAndZero (FileWriterThread1Status);
	deleteAndZero (FileWriterThread2Status);
	deleteAndZero (PixDrawThread1Status);
	deleteAndZero (PixDrawThread2Status);
	deleteAndZero (ProcessPriority);
	deleteAndZero (AlgoProcTimeThread1);
	deleteAndZero (AlgoProcTimeThread2);
	deleteAndZero (FileWriterThreadProcTime);
	deleteAndZero (AudioDevIOCallbackProcTime);
	deleteAndZero (PlotRenderThreadTime);
	deleteAndZero (Plot2ScreenTime);
#endif
	deleteAndZero (MaxPeak1);
	deleteAndZero (MaxPeak2);
	deleteAndZero (MaxPeak3);
}

//==============================================================================
void RTAPmainGUI::paint (Graphics& g)
{
    g.fillAll (Colours::lightgrey);
#if DISPLAY_PROCTIME
#if _DEBUG || VAR_FB
	GetAudSampProcTime->setText ("uiDestX = " + String (uiGlobalShow1) + " , uiImgBuffReadTrack =  " + String (uiGlobalShow5) + " ) ", dontSendNotification);
	OMEProcTime->setText ("uiDisplayDestX = " + String (uiGlobalShow2) + " , uiImgSrcX =  " + String (uiGlobalShow6) + " ) ", dontSendNotification);
	AlgoProcTimeThread1->setText ("siNumProcData2Display = " + String (uiGlobalShow3), dontSendNotification);
	PlotRenderThreadTime->setText ("uiScreenStartRender = " + String (uiGlobalShow4), dontSendNotification);
#else
	ProcessPriority->setText ("Process priority: " + String (cProcessPriority) + String (iProcessPriority), dontSendNotification);
	GetAudSampProcTime->setText ("Get audio data: " + String (dGetAuSampTime) + " ms, " + String (dSummedGetAuSamp/dNumGetAuSampInvk) + " ms", dontSendNotification);
	OMEProcTime->setText ("Outer & Middle ear: " + String (dOMEtime) + " ms, " + String (dSummedOMEtime/dNumOMEinvk) + " ms", dontSendNotification);
	AlgoProcTimeThread1->setText (String (cFunctionName) + String (dAlgoTime) + " ms, " + String (dSummedAlgoTime/dNumAlgoInvk) + " ms", dontSendNotification);
	// for multi-threaded application print 
	if (iFunctionToRun >= DRNL_MT)
		AlgoProcTimeThread2->setText (String (cFunctionName) + String (dAlgoEndTime[1] - dAlgoStartTime[1]) + " ms", dontSendNotification);
	AudioDevIOCallbackProcTime->setText ("Audio In: " + String (dAudioDevIOstartTime - dPrevAudioDevIOstartTime)/* + " ms, " + String (dSummedAudioDevIOtime/dNumGetAuSampInvk)*/ + " ms", dontSendNotification);
	PlotRenderThreadTime->setText ("Plot render: " + String (dPlotRenderTime) + " ms, " + String (dSummedPlotRenderTime/dNumPlotRenderInvk) + " ms", dontSendNotification);
	Plot2ScreenTime->setText ("Plot display: " + String (dGfxPaintFuncTime) + " ms, " + String (dGfxPaintTime) + "ms", dontSendNotification);
//	FileWriterThreadProcTime->setText ("File writer: " + String (dWrite2FileEndTime - dWrite2FileStartTime) + " ms", dontSendNotification);
	FileWriterThreadProcTime->setText (String (cFileStatus), dontSendNotification);
#endif

	MaxPeak1->setText (" Max peak: BF # " + String (uiMeanPeakBF[0]) + " - " + String (fMaxPeak[0]), dontSendNotification);
	MaxPeak2->setText ("x2 = " + String (uiGlobalShow3) + " y2 = " + String(uiGlobalShow4), dontSendNotification);//(" fMaxPixDisplayVal: BF # " + String (fMaxPixDisplayVal) + " " + String (uiGlobalShow6), dontSendNotification);
	MaxPeak3->setText ("x1 = " + String (uiGlobalShow5) + " y1 = " + String(uiGlobalShow6), dontSendNotification);//(" fMinPixDisplayVal: BF # " + String (fMinPixDisplayVal), dontSendNotification);

	// Display auditory periphery thread status:
	// Thread 0
	if (cProcThreadStatus[0] == RUNNING)
		ProcThread1Status->setText ( "Thread 1 running", dontSendNotification );
	else if (cProcThreadStatus[0] == READY_TO_RUN)
		ProcThread1Status->setText ( "Thread 1 ready to run", dontSendNotification );
	else
		ProcThread1Status->setText ( "Thread 1 idle", dontSendNotification );
	// Thread 1
	if (cProcThreadStatus[1] == RUNNING)
		ProcThread2Status->setText ( "Thread 2 running", dontSendNotification );
	else if (cProcThreadStatus[1] == READY_TO_RUN)
		ProcThread2Status->setText ( "Thread 2 ready to run", dontSendNotification );
	else
		ProcThread2Status->setText ( "Thread 2 idle", dontSendNotification );
	
	// Display file writer thread status:
	// Thread 0
	if (cFileWriterThreadStatus[0] == RUNNING)
		FileWriterThread1Status->setText ( "File writer thread 1 running", dontSendNotification );
	else if (cFileWriterThreadStatus[0] == READY_TO_RUN)
		FileWriterThread1Status->setText ( "File writer thread 1 ready to run", dontSendNotification );
	else
		FileWriterThread1Status->setText ( "File writer thread 1 idle", dontSendNotification );
	// Thread 1
	if (cFileWriterThreadStatus[1] == RUNNING)
		FileWriterThread2Status->setText ( "File writer thread 2 running", dontSendNotification );
	else if (cFileWriterThreadStatus[1] == READY_TO_RUN)
		FileWriterThread2Status->setText ( "File writer thread 2 ready to run", dontSendNotification );
	else
		FileWriterThread2Status->setText ( "File writer thread 2 idle", dontSendNotification );

#endif
}

void RTAPmainGUI::resized()
{
	// setBounds (x, y, width, height);
//    liveAudioDisplayComp->setBounds (8, 8, getWidth() - 16, 64);
	JUCEliveDisplay->setBounds (getWidth() * 0.005f, getHeight() * 0.01f, getWidth() * 0.99f, getHeight() * 0.76f);
	APfunction2RecComboBox->setBounds (proportionOfWidth (0.01f), proportionOfHeight (0.87f), 130, 20);
	APfunction2RecLabel->setBounds (proportionOfWidth (0.01f), proportionOfHeight (0.83f), 150, 35);
	APfunction2RunComboBox->setBounds (proportionOfWidth (0.13f), proportionOfHeight (0.87f), 170, 20);
	APfunction2RunLabel->setBounds (proportionOfWidth (0.13f), proportionOfHeight (0.83f), 150, 35);
	RTAPpriorityComboBox->setBounds (proportionOfWidth (0.28f), proportionOfHeight (0.87f), 130, 20);
	RTAPpriorityLabel->setBounds (proportionOfWidth (0.28f), proportionOfHeight (0.83f), 150, 35);
	PlayPauseButton->setBounds (proportionOfWidth (0.01f), proportionOfHeight (0.78f), 40, 40);
	SetParamsButton->setBounds (proportionOfWidth (0.06f), proportionOfHeight (0.78f), 60, 40);
	PlayRecButton->setBounds (proportionOfWidth (0.13f), proportionOfHeight (0.78f), 60, 40);
	ProcDataRecordButton->setBounds (proportionOfWidth (0.20f), proportionOfHeight (0.78f), 60, 40);
	RefreshPlotButton->setBounds (proportionOfWidth (0.27f), proportionOfHeight (0.78f), 60, 40);
	WindowBoundaryDispBtn->setBounds (20, 620, 150, 24);
	PeakAmpDispBtn->setBounds (20, 640, 150 ,24);
#if TEST_AUDIODEVICESETUP
	SamplingFreqLabel->setBounds ( proportionOfWidth ( 0.29f ), proportionOfHeight ( 0.89f ), 150, 40 );
	SamplingFreqSlider->setBounds ( proportionOfWidth ( 0.29f ), proportionOfHeight ( 0.93f ), 180, 20 );
#endif
#if DOUBLE_PRECISION_BUTTON
	DoublePrecisionBtn->setBounds ( proportionOfWidth ( 0.29f ), proportionOfHeight ( 0.96f ), 150 , 24 );
#endif

#if DISPLAY_PROCTIME
	ProcessPriority->setBounds (proportionOfWidth (0.40f), proportionOfHeight (0.78f), proportionOfWidth (0.4303f ), 24);
	GetAudSampProcTime->setBounds (proportionOfWidth (0.40f), proportionOfHeight (0.81f), proportionOfWidth (0.4303f), 24);
	OMEProcTime->setBounds (proportionOfWidth (0.40f), proportionOfHeight (0.84f), proportionOfWidth (0.4303f), 24);
	AlgoProcTimeThread1->setBounds (proportionOfWidth (0.40f), proportionOfHeight (0.87f), proportionOfWidth (0.4303f), 24);
//	AlgoProcTimeThread2->setBounds (proportionOfWidth (0.25f), 280, proportionOfWidth (0.4303f), 24);
	PlotRenderThreadTime->setBounds (proportionOfWidth (0.40f), proportionOfHeight (0.90f), proportionOfWidth (0.4303f), 24);
	Plot2ScreenTime->setBounds (proportionOfWidth (0.40f), proportionOfHeight (0.93f), proportionOfWidth (0.4303f), 24);
	FileWriterThreadProcTime->setBounds (proportionOfWidth (0.40f), proportionOfHeight (0.96f), proportionOfWidth (0.4303f), 24);
	ProcThread1Status->setBounds (proportionOfWidth (0.77f), proportionOfHeight (0.87f), proportionOfWidth (0.4303f), 24);
//	ProcThread2Status->setBounds (proportionOfWidth (0.70f), proportionOfHeight (0.81f), proportionOfWidth (0.4303f), 24);
	FileWriterThread1Status->setBounds (proportionOfWidth (0.77f), proportionOfHeight (0.90f), proportionOfWidth (0.4303f), 24);
//	FileWriterThread2Status->setBounds (proportionOfWidth (0.70f), proportionOfHeight (0.87f), proportionOfWidth (0.4303f), 24);
	PixDrawThread1Status->setBounds (proportionOfWidth (0.77f), proportionOfHeight (0.93f), proportionOfWidth (0.4303f), 24);
//	PixDrawThread2Status->setBounds (proportionOfWidth (0.70f), proportionOfHeight (0.93f), proportionOfWidth (0.4303f), 24);
	AudioDevIOCallbackProcTime->setBounds (proportionOfWidth (0.77f), proportionOfHeight (0.96f), proportionOfWidth (0.4303f), 24);
#endif
	MaxPeak1->setBounds (proportionOfWidth (0.77f), proportionOfHeight (0.78f), proportionOfWidth (0.4303f), 24);
	MaxPeak2->setBounds (proportionOfWidth (0.77f), proportionOfHeight (0.81f), proportionOfWidth (0.4303f), 24);
	MaxPeak3->setBounds (proportionOfWidth (0.77f), proportionOfHeight (0.84f), proportionOfWidth (0.4303f), 24);
}

void RTAPmainGUI::buttonClicked (Button* buttonThatWasClicked)
{
	bXmlUpdate = false;
    if (buttonThatWasClicked == PlayPauseButton)
	{
		bPlayPauseStatus ^= true;								// alter Play/Pause status between 0 & 1
		uiRTAPplayStart = Time::getMillisecondCounter();		// get time to track processing time
	}
	else if (buttonThatWasClicked == SetParamsButton)			// SET button
	{
		// Update progress bar status thread
		SettingProgressStatus ProgressStatus;
		if (ProgressStatus.runThread())
		{
			AlertWindow::showMessageBoxAsync (AlertWindow::WarningIcon, "RTAP configuration in progress", "RTAP configured to new setting");
		}
		else
		{
			AlertWindow::showMessageBoxAsync (AlertWindow::WarningIcon, "RTAP configuration in progress", "Unable to setup RTAP to new configurations");
		}

		//update RTAP boolean flag & other relevant variables to restore coordinates to origin in wave display
		bRTAPsetBtnDispSync = true;

		// flip the Play/Pause button to "off" if "on"
		if (bPlayPauseStatus)
			PlayPauseButton->setToggleState (false, true);
		JUCEliveDisplay->FreeResources();						// de-allocate any existing mem
		JUCEliveDisplay->Reinitialise();						// re-initialise mem with new system parameter settings
	}
	else if (buttonThatWasClicked == ProcDataRecordButton)
	{
		// only start the record process only record thread is not recording or not about to record
		if (!bRecordThreadEngaged || !bRecordProcData)
			bRecordProcData = true;
	}
	else if (buttonThatWasClicked == PlayRecButton)
	{
		// only start the play and record process only record thread is not recording or not about to record
		if (!bRecordThreadEngaged || !bRecordProcData)
		{
			bPlayPauseStatus = true;
			bRecordProcData = true;
		}
	}
	else if (buttonThatWasClicked == RefreshPlotButton)
	{
		bRefreshPlot = true;
	}
	else if (buttonThatWasClicked == WindowBoundaryDispBtn)
	{
		bWindowBoundaryDisp ^= true;
	}
	else if (buttonThatWasClicked == PeakAmpDispBtn)
	{
		bPeakAmpDisp ^= true;
	}
#if DOUBLE_PRECISION_BUTTON
	else if (buttonThatWasClicked == DoublePrecisionBtn)
	{
		bDoublePrecision ^= true;
	}
#endif
}

void RTAPmainGUI::comboBoxChanged (ComboBox* comboBoxThatHasChanged)
{
	bXmlUpdate = false;

	if (comboBoxThatHasChanged == APfunction2RunComboBox)
	{
		iFunctionToRun = APfunction2RunComboBox->getSelectedId ();
	}

	if (comboBoxThatHasChanged == RTAPpriorityComboBox)
	{
		iRTAPpriority = RTAPpriorityComboBox->getSelectedId();
	}

	if (comboBoxThatHasChanged == APfunction2RecComboBox)
	{
		if (APfunction2RecComboBox->getSelectedId ())
			uiFunction2Rec = APfunction2RecComboBox->getSelectedId ();
	}
}

void RTAPmainGUI::sliderValueChanged (Slider* sliderThatWasMoved)
{
#if TEST_AUDIODEVICESETUP
	if ( sliderThatWasMoved == SamplingFreqSlider )
	{
		SampFreqTest = SamplingFreqSlider->getValue();
		SetAudioDevice->sampleRate = 22050;//SampFreqTest;
//		SetAudioDevice->bufferSize = 1280;
		deviceManager.initialise(1, 1, 0, true, String::empty, SetAudioDevice);
//		deviceManager.setAudioDeviceSetup(*SetAudioDevice, true);

		deviceManager.getAudioDeviceSetup(*GetAudioDevice);
		fsGet = GetAudioDevice->sampleRate;
		buffSize = GetAudioDevice->bufferSize;
		inputChannel = GetAudioDevice->inputChannels;
		outputChannel = GetAudioDevice->outputChannels;
	}
#endif
}

void RTAPmainGUI::visibilityChanged()
{
    //[UserCode_visibilityChanged] -- Add your code here...
//   recorder->stop();
//    recordButton->setButtonText ("Record");
    //[/UserCode_visibilityChanged]
}

