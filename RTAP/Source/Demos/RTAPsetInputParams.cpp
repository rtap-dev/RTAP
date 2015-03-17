
#include "RTAPsetInputParams.h"

RTAPsetInputParams::RTAPsetInputParams (AudioDeviceManager& deviceManager_)
	: deviceManager (deviceManager_),
	AudioInputLevelSlider (0),
	SamplingFreqComboBox (0),
	InputSineBoostSlider (0),
	InputSineFreqSlider (0),
	AudioDSound (0),
	AudioSrcSine (0),
	AudioSrcSquare (0),
	AudioSrcSaw (0),
	StaticDisplayBtn (0),
	AutoRefreshDisplayBtn (0),
	ScrollDisplayBtn (0),
	ScatterDispTogBtn (0),
	LineDispTogBtn (0),
	SpectroDispTogBtn (0)
{
	addAndMakeVisible (GraphTypeGroup = new GroupComponent ("GraphType", "Graph Type"));

	// -------- Sampling freq combo box display start --------
	addAndMakeVisible (SamplingFreqLabel = new Label(String::empty, "Sampling Freq (Hz)"));
	SamplingFreqLabel->setJustificationType (Justification::centred);
	SamplingFreqLabel->setEditable (false, false, false);
	SamplingFreqLabel->setColour (TextEditor::textColourId, Colours::black);
	addAndMakeVisible (SamplingFreqComboBox = new ComboBox ("Sampling Freq:"));
	SamplingFreqComboBox->setEditableText (false);
	SamplingFreqComboBox->setJustificationType (Justification::centred);
	SamplingFreqComboBox->addItem ("22,050 Hz", 22050);
#if MULTI_SAMPLINGFREQ
	SamplingFreqComboBox->addItem ("14,700 Hz", 14700);
	SamplingFreqComboBox->addItem ("11,025 Hz", 11025);
	SamplingFreqComboBox->addItem ("8,820 Hz", 8820);
#endif
	SamplingFreqComboBox->setTooltip ("Sampling freq for audio stream");
	SamplingFreqComboBox->addListener (this);
	// -------- Sampling freq combo box display end --------

	// -------- Audio input dB SPL gain slider display start --------
	addAndMakeVisible (AudioInputLevelLabel = new Label (String::empty, "Audio input level (dB SPL)"));
	AudioInputLevelLabel->setJustificationType (Justification::left);
	AudioInputLevelLabel->setEditable (false, false, false);
	AudioInputLevelLabel->setColour (TextEditor::textColourId, Colours::black);
	addAndMakeVisible (AudioInputLevelSlider = new Slider (String::empty));
	AudioInputLevelSlider->setSliderStyle (Slider::LinearHorizontal);
	AudioInputLevelSlider->setTextBoxStyle (Slider::TextBoxLeft, false, 80, 20);
	AudioInputLevelSlider->setRange (5, 90, 1);
	AudioInputLevelSlider->addListener (this);
	// -------- Audio input dB SPL gain slider display end --------

	// -------- Input sine freq adjustment slider display start --------
	addAndMakeVisible (InputSineFreqLabel = new Label (String::empty, "Sine frequency"));
	InputSineFreqLabel->setJustificationType (Justification::centred);
	InputSineFreqLabel->setEditable (false, false, false);
	InputSineFreqLabel->setColour (TextEditor::textColourId, Colours::black);
	addAndMakeVisible (InputSineFreqSlider = new Slider (String::empty));
	InputSineFreqSlider->setSliderStyle (Slider::LinearHorizontal);
	InputSineFreqSlider->setTextBoxStyle (Slider::TextBoxLeft, false, 80, 20);
	InputSineFreqSlider->setRange (100, 6000, 50);
	InputSineFreqSlider->setTooltip ("Input sine wave frequency");
	InputSineFreqSlider->addListener (this);
	// -------- Input sine freq adjustment slider display end --------

	// -------- Input sine amplitude boost slider display start --------
	addAndMakeVisible (InputSineBoostLabel = new Label (String::empty, "Sine amplitude"));
	InputSineBoostLabel->setJustificationType (Justification::centred);
	InputSineBoostLabel->setEditable (false, false, false);
	InputSineBoostLabel->setColour (TextEditor::textColourId, Colours::black);
	addAndMakeVisible (InputSineBoostSlider = new Slider(String::empty));
	InputSineBoostSlider->setSliderStyle (Slider::LinearHorizontal);
	InputSineBoostSlider->setTextBoxStyle (Slider::TextBoxLeft, false, 80, 20);
	InputSineBoostSlider->setRange (0, 1.0, 0.1);
	InputSineBoostSlider->setTooltip ("Input sine amplitude in 0.1 increment (0.0 - 1.0");
	InputSineBoostSlider->addListener (this);
	// -------- Input sine amplitude boost slider display end --------

	// -------- Audio source toggle buttons start --------
	addAndMakeVisible (AudioDSound = new ToggleButton(String::empty));
	AudioDSound->setButtonText("System level audio source");
	AudioDSound->setRadioGroupId (3008);
	AudioDSound->setTooltip ("Mic In & Speaker Out audio option. To set, click on 'Audio Settings' on the left");
	AudioDSound->addListener (this);
	AudioDSound->setToggleState (true, false);

	addAndMakeVisible (AudioSrcSine= new ToggleButton(String::empty));
	AudioSrcSine->setButtonText ("Sine wave audio source");
	AudioSrcSine->setRadioGroupId (3008);
	AudioSrcSine->setTooltip ("Sine tone audio stimulus");
	AudioSrcSine->addListener (this);

	addAndMakeVisible (AudioSrcSquare= new ToggleButton(String::empty));
	AudioSrcSquare->setButtonText ("Square wave audio source");
	AudioSrcSquare->setRadioGroupId (3008);
	AudioSrcSquare->setTooltip ("Pulse tone audio stimulus");
	AudioSrcSquare->addListener (this);

	addAndMakeVisible (AudioSrcSaw= new ToggleButton(String::empty));
	AudioSrcSaw->setButtonText ("Sawtooth wave audio source");
	AudioSrcSaw->setRadioGroupId (3008);
	AudioSrcSaw->setTooltip ("Sawtooth audio stimulus");
	AudioSrcSaw->addListener (this);
	// -------- Audio source toggle buttons end --------

	// -------- Display plot type toggle buttons start --------
	addAndMakeVisible (StaticDisplayBtn = new ToggleButton (String::empty));
	StaticDisplayBtn->setButtonText ("Static display");
	StaticDisplayBtn->setRadioGroupId (3024);
	StaticDisplayBtn->setTooltip ("Static display of either scatter, line or spectrogram plot");
	StaticDisplayBtn->addListener (this);
	StaticDisplayBtn->setToggleState (true, false);

	addAndMakeVisible (AutoRefreshDisplayBtn = new ToggleButton (String::empty));
	AutoRefreshDisplayBtn->setButtonText ("Auto refresh display");
	AutoRefreshDisplayBtn->setRadioGroupId (3024);
	AutoRefreshDisplayBtn->setTooltip ("Automatically refreshes scatter, line & spectrogram plots");
	AutoRefreshDisplayBtn->addListener (this);

	addAndMakeVisible (ScrollDisplayBtn = new ToggleButton (String::empty));
	ScrollDisplayBtn->setButtonText ("Scroll display - only for scatter & line plots");
	ScrollDisplayBtn->setRadioGroupId (3024);
	ScrollDisplayBtn->setTooltip ("Scroll display only for scatter & line plots");
	ScrollDisplayBtn->addListener (this);
	// -------- Display plot type toggle buttons end --------

	// -------- Display plot toggle buttons start --------
	addAndMakeVisible (ScatterDispTogBtn = new ToggleButton(String::empty));
	ScatterDispTogBtn->setButtonText ("Scatter plot display");
	ScatterDispTogBtn->setRadioGroupId (3016);
	ScatterDispTogBtn->setTooltip ("ERB scaled scatter plots");
	ScatterDispTogBtn->addListener (this);
	ScatterDispTogBtn->setToggleState (true, false);

	addAndMakeVisible (LineDispTogBtn = new ToggleButton(String::empty));
	LineDispTogBtn->setButtonText ("Line plot display");
	LineDispTogBtn->setRadioGroupId (3016);
	LineDispTogBtn->setTooltip ("ERB scaled line plots");
	LineDispTogBtn->addListener (this);

	addAndMakeVisible (SpectroDispTogBtn = new ToggleButton(String::empty));
	SpectroDispTogBtn->setButtonText ("Spectrogram plot display");
	SpectroDispTogBtn->setRadioGroupId (3016);
	SpectroDispTogBtn->setTooltip ("Multi-coloured spectral plots");
	SpectroDispTogBtn->addListener (this);
	// -------- Display plot toggle buttons end --------

	// set default values here
	// if no RTAP parameters update from XML loading then set default value
	if (!bXmlUpdate)
	{
		AudioInputLevelSlider->setValue (50, dontSendNotification);		// 50dB SPL
		SamplingFreqComboBox->setSelectedId (22050);					// fs = 22.05kHz
		InputSineBoostSlider->setValue (1.0f, dontSendNotification);
		InputSineFreqSlider->setValue (500, dontSendNotification);
	}
	// if RTAP parameters loaded from XML file, then update accordingly
	else
	{
		AudioInputLevelSlider->setValue (fAudioLvldBSPL, dontSendNotification);
		SamplingFreqComboBox->setSelectedId (22050);
		InputSineBoostSlider->setValue (fSineInpAmpAdj, dontSendNotification);
		InputSineFreqSlider->setValue (iSineFreq, dontSendNotification);
		
		if (cAudioSource == AUDIO_SYSTEM) AudioDSound->setToggleState (true, false);
		else if (cAudioSource == AUDIO_SINE) AudioSrcSine->setToggleState (true, false);
		else if (cAudioSource == AUDIO_SQUARE) AudioSrcSquare->setToggleState (true, false);
		else if (cAudioSource == AUDIO_SAWTOOTH) AudioSrcSaw->setToggleState (true, false);

		// set respective toggle button to be checked based on XML settings
		switch (cDisplayMode)
		{
			case STATIC_DISPLAY: StaticDisplayBtn->setToggleState (true, false); break;
			case AUTOREFRESH_DISPLAY: AutoRefreshDisplayBtn->setToggleState (true, false); break;
			case SCROLL_DISPLAY: ScrollDisplayBtn->setToggleState (true, false); break;
		}
		switch (cWaveformType)
		{
			case SCATTER_PLOT: ScatterDispTogBtn->setToggleState (true, false);
			case LINE_PLOT: LineDispTogBtn->setToggleState (true, false);
			case SPECTROGRAM_PLOT: SpectroDispTogBtn->setToggleState (true, false);
		}
	}
}

RTAPsetInputParams::~RTAPsetInputParams()
{
	deleteAndZero (GraphTypeGroup);
	deleteAndZero (AudioInputLevelSlider);
	deleteAndZero (AudioInputLevelLabel);
	deleteAndZero (SamplingFreqLabel);
	deleteAndZero (SamplingFreqComboBox);
	deleteAndZero (InputSineBoostLabel);
	deleteAndZero (InputSineBoostSlider);
	deleteAndZero (InputSineFreqLabel);
	deleteAndZero (InputSineFreqSlider);
	deleteAndZero (AudioDSound);
	deleteAndZero (AudioSrcSine);
	deleteAndZero (AudioSrcSquare);
	deleteAndZero (AudioSrcSaw);
	deleteAndZero (StaticDisplayBtn);
	deleteAndZero (AutoRefreshDisplayBtn);
	deleteAndZero (ScrollDisplayBtn);
	deleteAndZero (ScatterDispTogBtn);
	deleteAndZero (LineDispTogBtn);
	deleteAndZero (SpectroDispTogBtn);
}

void RTAPsetInputParams::paint (Graphics& g)
{
    g.fillAll (Colours::lightgrey);
}

void RTAPsetInputParams::resized()
{
	GraphTypeGroup->setBounds (proportionOfWidth (0.0007f), proportionOfHeight (0.6f), proportionOfWidth (0.28f), proportionOfHeight (0.3f));//10, 420, 170, 160);
	SamplingFreqLabel->setBounds (proportionOfWidth (0.001f), proportionOfHeight (0.0f), proportionOfWidth (0.12f), proportionOfHeight (0.04f));//8, 0, 150, 35);
	SamplingFreqComboBox->setBounds (proportionOfWidth (0.003f), proportionOfHeight (0.04f), proportionOfWidth (0.08f), proportionOfHeight (0.03f));//(12, 30, 100, 20);
	AudioInputLevelLabel->setBounds (proportionOfWidth (0.118f), proportionOfHeight (0.0f), proportionOfWidth (0.12f), proportionOfHeight (0.04f));//(162, 0, 150, 35);
	AudioInputLevelSlider->setBounds (proportionOfWidth (0.12f), proportionOfHeight (0.01f), proportionOfWidth (0.12f), proportionOfHeight (0.10f));//(166, 30, 150, 20);
	InputSineBoostLabel->setBounds (proportionOfWidth (0.255f), proportionOfHeight (0.0f), proportionOfWidth (0.12f), proportionOfHeight (0.04f));
	InputSineBoostSlider->setBounds (proportionOfWidth (0.26f), proportionOfHeight (0.01f), proportionOfWidth (0.12f), proportionOfHeight (0.10f));
	InputSineFreqLabel->setBounds (proportionOfWidth (0.39f), proportionOfHeight (0.0f), proportionOfWidth (0.12f), proportionOfHeight (0.04f));
	InputSineFreqSlider->setBounds (proportionOfWidth (0.40f), proportionOfHeight (0.01f), proportionOfWidth (0.12f), proportionOfHeight (0.10f));
	AudioDSound->setBounds (15, 300, 200, 24);
	AudioSrcSine->setBounds (15, 320, 200, 24);
	AudioSrcSquare->setBounds (15, 340, 200, 24);
	AudioSrcSaw->setBounds (15, 360, 200, 24);
	// leave height as constant so as to point cursor exactly on toggle button icon - 
	// if not toggle button is highlighted only when cursor points else where
	StaticDisplayBtn->setBounds (proportionOfWidth (0.01f), proportionOfHeight (0.62f), proportionOfWidth (0.25f), 25);
	AutoRefreshDisplayBtn->setBounds (proportionOfWidth (0.01f), proportionOfHeight (0.65f), proportionOfWidth (0.25f), 25);
	ScrollDisplayBtn->setBounds (proportionOfWidth (0.01f), proportionOfHeight (0.68f), proportionOfWidth (0.25f), 25);
	ScatterDispTogBtn->setBounds (proportionOfWidth (0.01f), proportionOfHeight (0.74f), proportionOfWidth (0.25f), 24);
	LineDispTogBtn->setBounds (proportionOfWidth (0.01f), proportionOfHeight (0.77f), proportionOfWidth (0.25f), 24);
	SpectroDispTogBtn->setBounds (proportionOfWidth (0.01f), proportionOfHeight (0.80f), proportionOfWidth (0.25f), 24);
}

void RTAPsetInputParams::buttonClicked (Button* buttonThatWasClicked)
{
	bXmlUpdate = false;

	if (buttonThatWasClicked == AudioDSound)			cAudioSource = AUDIO_SYSTEM;
	else if (buttonThatWasClicked == AudioSrcSine)		cAudioSource = AUDIO_SINE;
	else if (buttonThatWasClicked == AudioSrcSquare)	cAudioSource = AUDIO_SQUARE;
	else if (buttonThatWasClicked == AudioSrcSaw)		cAudioSource = AUDIO_SAWTOOTH;
	else if (buttonThatWasClicked == StaticDisplayBtn)
	{
		bScrollPlot = false;
		bAutoRefreshPlot = false;
		cDisplayMode = STATIC_DISPLAY;
	}
	else if (buttonThatWasClicked == AutoRefreshDisplayBtn)
	{
		bAutoRefreshPlot = true;
		cDisplayMode = AUTOREFRESH_DISPLAY;
	}
	else if (buttonThatWasClicked == ScrollDisplayBtn)
	{
		bScrollPlot = true;
		bAutoRefreshPlot = false;
		cDisplayMode = SCROLL_DISPLAY;
	}
	else if (buttonThatWasClicked == ScatterDispTogBtn)	cWaveformType = SCATTER_PLOT;
	else if (buttonThatWasClicked == LineDispTogBtn)	cWaveformType = LINE_PLOT;
	else if (buttonThatWasClicked == SpectroDispTogBtn)	cWaveformType = SPECTROGRAM_PLOT;
}

void RTAPsetInputParams::comboBoxChanged (ComboBox* comboBoxThatHasChanged)
{
	bXmlUpdate = false;

	if (comboBoxThatHasChanged == SamplingFreqComboBox)
	{
#if SINGLE_PRECISION
		fSamplingRate = (float) SamplingFreqComboBox->getSelectedId ();
#elif DOUBLE_PRECISION
		fSamplingRate = (float) SamplingFreqComboBox->getSelectedId ();
#endif
	}
}

void RTAPsetInputParams::sliderValueChanged (Slider* sliderThatWasMoved)
{
	bXmlUpdate = false;

	if (sliderThatWasMoved == AudioInputLevelSlider)
	{
		fAudioLvldBSPL = AudioInputLevelSlider->getValue ();
	}
	else if (sliderThatWasMoved == InputSineBoostSlider)
	{
		fSineInpAmpAdj = InputSineBoostSlider->getValue ();
	}
	else if (sliderThatWasMoved == InputSineFreqSlider)
	{
		iSineFreq = InputSineFreqSlider->getValue ();
	}
}
