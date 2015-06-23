
#include "RTAPsetParams.h"

RTAPsetParams::RTAPsetParams (AudioDeviceManager& deviceManager_)
    : deviceManager (deviceManager_),
	  MultiBFchannelComboBox (0),
	  NumANfibreTypesComboBox (0),
	  ANfibreTypesComboBox (0),
	  GammatoneFiltOrderSlider(0),
	  BFlistMinSlider(0),
	  BFlistMaxSlider(0),
	  ComprThreshBMdBSlider(0),
	  DRNLaSlider(0),
	  DRNLbSlider(0),
	  DRNLcSlider(0),
	  LinGainSlider(0),
	  MathOptimisationBtn(0)
{
	int i;

	// -------- Multiple BF channel drop down menu selection display start --------
	addAndMakeVisible (MultiBFchannelLabel = new Label ( String::empty, "Number of BF channels"));
	MultiBFchannelLabel->setJustificationType (Justification::left);
	MultiBFchannelLabel->setEditable (false, false, false);
	MultiBFchannelLabel->setColour (TextEditor::textColourId, Colours::black);
	addAndMakeVisible (MultiBFchannelComboBox = new ComboBox (String::empty));
	MultiBFchannelComboBox->setEditableText (true);
	MultiBFchannelComboBox->setJustificationType (Justification::centred);

	for (i=1; i<=NUMOFBFCHANNELS; i++)
		MultiBFchannelComboBox->addItem (String(i) + " BF channels", i);
	MultiBFchannelComboBox->addListener (this);
	// -------- Multiple BF channel drop down menu selection display end --------
	
	// -------- Number of AN fibre type combo box display start --------
	addAndMakeVisible (NumANfibreTypesLabel = new Label (String::empty, "Number of auditory nerve (AN) fibre types"));
	NumANfibreTypesLabel->setJustificationType (Justification::centred);
	NumANfibreTypesLabel->setEditable (false, false, false);
	NumANfibreTypesLabel->setColour (TextEditor::textColourId, Colours::black);
	addAndMakeVisible (NumANfibreTypesComboBox = new ComboBox ("NumANfibreTypes:"));
	NumANfibreTypesComboBox->setEditableText (false);
	NumANfibreTypesComboBox->setJustificationType (Justification::centred);
	NumANfibreTypesComboBox->addItem ("1", 1);
	NumANfibreTypesComboBox->addItem ("2", 2);
	NumANfibreTypesComboBox->addListener (this);
	// -------- Number of AN fibre type combo box display end --------

	// -------- AN fibre type combo box display start --------
	addAndMakeVisible (ANfibreTypesLabel = new Label (String::empty, "Auditory nerve (AN) fibre types"));
	ANfibreTypesLabel->setJustificationType (Justification::centred);
	ANfibreTypesLabel->setEditable (false, false, false);
	ANfibreTypesLabel->setColour (TextEditor::textColourId, Colours::black);
	addAndMakeVisible (ANfibreTypesComboBox = new ComboBox ("ANfibreTypes"));
	ANfibreTypesComboBox->setEditableText (false);
	ANfibreTypesComboBox->setJustificationType (Justification::centred);
	ANfibreTypesComboBox->addItem ("LSR", 1);
	ANfibreTypesComboBox->addItem ("HSR", 2);
	ANfibreTypesComboBox->addListener (this);
	// -------- AN fibre type combo box display end --------

	addAndMakeVisible (DRNLgroup = new GroupComponent ("DRNLparams", "Basilar Membrane (DRNL)"));

	// -------- Gammatone filter order select slider display start --------
	addAndMakeVisible (GammatoneFiltOrderLabel = new Label (String::empty, "Gammatone filter order"));
	GammatoneFiltOrderLabel->setJustificationType (Justification::left);
	GammatoneFiltOrderLabel->setEditable (false, false, false);
	GammatoneFiltOrderLabel->setColour (TextEditor::textColourId, Colours::black);
	addAndMakeVisible (GammatoneFiltOrderSlider = new Slider (String::empty));
	GammatoneFiltOrderSlider->setSliderStyle (Slider::LinearHorizontal);
	GammatoneFiltOrderSlider->setTextBoxStyle (Slider::TextBoxLeft, false, 80, 20);
	GammatoneFiltOrderSlider->setRange (1, 5, 1);
	GammatoneFiltOrderSlider->addListener (this);
	// -------- Gammatone filter order select slider display end --------

	// -------- Min BF list slider display start --------
	addAndMakeVisible (BFlistMinLabel = new Label (String::empty, "Min BF channel freq (Hz)"));
	BFlistMinLabel->setJustificationType (Justification::left);
	BFlistMinLabel->setEditable (false, false, false);
	BFlistMinLabel->setColour (TextEditor::textColourId, Colours::black);
	addAndMakeVisible (BFlistMinSlider = new Slider (String::empty));
	BFlistMinSlider->setSliderStyle (Slider::LinearHorizontal);
	BFlistMinSlider->setTextBoxStyle (Slider::TextBoxLeft, false, 80, 20);
	BFlistMinSlider->setRange (200, 6000, 1);
	BFlistMinSlider->addListener (this);
	// -------- Min BF list slider display end --------

	// -------- Max BF list slider display start --------
	addAndMakeVisible (BFlistMaxLabel = new Label (String::empty, "Max BF channel freq (Hz)"));
	BFlistMaxLabel->setJustificationType (Justification::left);
	BFlistMaxLabel->setEditable (false, false, false);
	BFlistMaxLabel->setColour (TextEditor::textColourId, Colours::black);
	addAndMakeVisible (BFlistMaxSlider = new Slider (String::empty));
	BFlistMaxSlider->setSliderStyle (Slider::LinearHorizontal);
	BFlistMaxSlider->setTextBoxStyle (Slider::TextBoxLeft, false, 80, 20);
	BFlistMaxSlider->setRange (6000, 11025, 1);
	BFlistMaxSlider->addListener (this);
	// -------- Max BF list slider display end --------

	// -------- BM compression threshold (dB) slider display start --------
	addAndMakeVisible (ComprThreshBMdBLabel = new Label (String::empty, "BM compression threshold (dB)"));
	ComprThreshBMdBLabel->setJustificationType (Justification::left);
	ComprThreshBMdBLabel->setEditable (false, false, false);
	ComprThreshBMdBLabel->setColour (TextEditor::textColourId, Colours::black);
	addAndMakeVisible (ComprThreshBMdBSlider = new Slider (String::empty));
	ComprThreshBMdBSlider->setSliderStyle (Slider::LinearHorizontal);
	ComprThreshBMdBSlider->setTextBoxStyle (Slider::TextBoxLeft, false, 80, 20);
	ComprThreshBMdBSlider->setRange (-20, 90, 1);
	ComprThreshBMdBSlider->addListener (this);
	// -------- BM compression thrshold (dB) slider display end --------

	// -------- DRNLa slider display start --------
	addAndMakeVisible (DRNLaLabel = new Label (String::empty, "a" ));
	DRNLaLabel->setJustificationType (Justification::left);
	DRNLaLabel->setEditable (false, false, false);
	DRNLaLabel->setColour (TextEditor::textColourId, Colours::black);
	addAndMakeVisible (DRNLaSlider = new Slider (String::empty));
	DRNLaSlider->setSliderStyle (Slider::LinearHorizontal);
	DRNLaSlider->setTextBoxStyle (Slider::TextBoxLeft, false, 80, 20);
	DRNLaSlider->setRange (0, 1e5, 1);
	DRNLaSlider->addListener (this);
	// -------- DRNLa slider display end --------

	// -------- DRNLb slider display start --------
	addAndMakeVisible (DRNLbLabel = new Label (String::empty, "b" ));
	DRNLbLabel->setJustificationType (Justification::left);
	DRNLbLabel->setEditable (false, false, false);
	DRNLbLabel->setColour (TextEditor::textColourId, Colours::black);
	addAndMakeVisible (DRNLbSlider = new Slider (String::empty));
	DRNLbSlider->setSliderStyle (Slider::LinearHorizontal);
	DRNLbSlider->setTextBoxStyle (Slider::TextBoxLeft, false, 80, 20);
	DRNLbSlider->setRange (0, 1, 1e-4);
	DRNLbSlider->addListener (this);
	// -------- DRNLb slider display end --------

	// -------- DRNLc slider display start --------
	addAndMakeVisible (DRNLcLabel = new Label (String::empty, "c" ));
	DRNLcLabel->setJustificationType (Justification::left);
	DRNLcLabel->setEditable (false, false, false);
	DRNLcLabel->setColour (TextEditor::textColourId, Colours::black);
	addAndMakeVisible (DRNLcSlider = new Slider (String::empty));
	DRNLcSlider->setSliderStyle (Slider::LinearHorizontal);
	DRNLcSlider->setTextBoxStyle (Slider::TextBoxLeft, false, 80, 20);
	DRNLcSlider->setRange (0, 1, 0.1);
	DRNLcSlider->addListener (this);
	// -------- DRNLc slider display end --------

	// -------- Linear gain slider display start --------
	addAndMakeVisible (LinGainLabel = new Label (String::empty, "Linear Pathway Gain"));
	LinGainLabel->setJustificationType (Justification::left);
	LinGainLabel->setEditable (false, false, false);
	LinGainLabel->setColour (TextEditor::textColourId, Colours::black);
	addAndMakeVisible (LinGainSlider = new Slider (String::empty));
	LinGainSlider->setSliderStyle (Slider::LinearHorizontal);
	LinGainSlider->setTextBoxStyle (Slider::TextBoxLeft, false, 80, 20);
	LinGainSlider->setRange (200, 1000, 1);
	LinGainSlider->addListener (this);
	// -------- DRNLc slider display end --------

	// -------- Math optimisation toggle button start --------
	addAndMakeVisible (MathOptimisationBtn = new ToggleButton(String::empty));
	MathOptimisationBtn->setButtonText ("Math optimisation");
	MathOptimisationBtn->setTooltip ("Algorithm processing speedup");
	MathOptimisationBtn->addListener (this);
	MathOptimisationBtn->setClickingTogglesState (true);
	MathOptimisationBtn->setToggleState (false, false);
	// -------- Math optimisation toggle button end ----------

    setSize (600, 400);

	// set default values here
	// if no RTAP parameters update from XML loading then set default value
	if (!bXmlUpdate)
	{
		MultiBFchannelComboBox->setSelectedId (1);						// 1 BF channel
		NumANfibreTypesComboBox->setSelectedId (1);						// 1 AN fibre type
		ANfibreTypesComboBox->setSelectedId (1);						// LSR AN fibre type
		GammatoneFiltOrderSlider->setValue (3, dontSendNotification);	// Default gammatone filter order = 3
		BFlistMinSlider->setValue (250, dontSendNotification);			// Min BF = 250Hz
		BFlistMaxSlider->setValue (6000, dontSendNotification);			// Max BF = 4000Hz
		ComprThreshBMdBSlider->setValue (10, dontSendNotification);		// 10dB SPL
		DRNLaSlider->setValue (5e4, dontSendNotification);				// 50,000
		DRNLbSlider->setValue (3e-4, dontSendNotification);				// 0.0003
		DRNLcSlider->setValue (0.2, dontSendNotification);				// 0.2
		LinGainSlider->setValue (500, dontSendNotification);			// 500
	}
	// if RTAP parameters loaded from XML file, then update accordingly
	else
	{
		MultiBFchannelComboBox->setSelectedId (uiNumofBFchannels);
		NumANfibreTypesComboBox->setSelectedId (uiNumofANfibreTypes);
		ANfibreTypesComboBox->setSelectedId (iANfibreType);
		GammatoneFiltOrderSlider->setValue (iGammatoneFiltOrder, dontSendNotification);
		BFlistMinSlider->setValue (fMinBFlist, dontSendNotification);
		BFlistMaxSlider->setValue (fMaxBFlist, dontSendNotification);
		ComprThreshBMdBSlider->setValue (fComprThreshBMdB, dontSendNotification);
		DRNLaSlider->setValue (fDRNLa, dontSendNotification);
		DRNLbSlider->setValue (fDRNLb, dontSendNotification);
		DRNLcSlider->setValue (fDRNLc, dontSendNotification);
		LinGainSlider->setValue (fLinGain, dontSendNotification);

		// Deal with radio button setting with XML settings
		if (bMathOpt)
			MathOptimisationBtn->setToggleState (true, false);
	}
}

RTAPsetParams::~RTAPsetParams()
{
	deleteAndZero (MultiBFchannelLabel);
	deleteAndZero (MultiBFchannelComboBox);
	deleteAndZero (NumANfibreTypesLabel);
	deleteAndZero (NumANfibreTypesComboBox);
	deleteAndZero (ANfibreTypesLabel);
	deleteAndZero (ANfibreTypesComboBox);
	deleteAndZero (DRNLgroup);
	deleteAndZero (GammatoneFiltOrderLabel);
	deleteAndZero (GammatoneFiltOrderSlider);
	deleteAndZero (BFlistMinLabel);
	deleteAndZero (BFlistMinSlider);
	deleteAndZero (BFlistMaxLabel);
	deleteAndZero (BFlistMaxSlider);
	deleteAndZero (ComprThreshBMdBLabel);
	deleteAndZero (ComprThreshBMdBSlider);
	deleteAndZero (DRNLaLabel);
	deleteAndZero (DRNLaSlider);
	deleteAndZero (DRNLbLabel);
	deleteAndZero (DRNLbSlider);
	deleteAndZero (DRNLcLabel);
	deleteAndZero (DRNLcSlider);
	deleteAndZero (LinGainLabel);
	deleteAndZero (LinGainSlider);
	deleteAndZero (MathOptimisationBtn);
}

void RTAPsetParams::paint (Graphics& g)
{
    g.fillAll (Colours::lightgrey);
}

void RTAPsetParams::resized()
{
	MultiBFchannelLabel->setBounds (310, 50, 150, 35);
	MultiBFchannelComboBox->setBounds (314, 80, 150, 20);
	NumANfibreTypesLabel->setBounds (610, 50, 150, 35);
	NumANfibreTypesComboBox->setBounds (614, 80, 150, 35);
	ANfibreTypesLabel->setBounds (610, 110, 150, 35);
	ANfibreTypesComboBox->setBounds (614, 140, 150, 35);
	DRNLgroup->setBounds (10, 55, 200, 500);
	GammatoneFiltOrderLabel->setBounds (25, 70, 150, 35);
	GammatoneFiltOrderSlider->setBounds (28, 100, 150, 20);
	BFlistMinLabel->setBounds (25, 120, 150, 35);
	BFlistMinSlider->setBounds (28, 150, 150, 20);
	BFlistMaxLabel->setBounds (25, 170, 150, 35);
	BFlistMaxSlider->setBounds (28, 200, 150, 20);
	ComprThreshBMdBLabel->setBounds (25, 220, 150, 35);
	ComprThreshBMdBSlider->setBounds (28, 250, 150, 20);
	DRNLaLabel->setBounds (25, 270, 150, 35);
	DRNLaSlider->setBounds (28, 300, 150, 20);
	DRNLbLabel->setBounds (25, 320, 150, 35);
	DRNLbSlider->setBounds (28, 350, 150, 20);
	DRNLcLabel->setBounds (25, 370, 150, 35);
	DRNLcSlider->setBounds (28, 400, 150, 20);
	LinGainLabel->setBounds (25, 420, 150, 35);
	LinGainSlider->setBounds (28, 450, 150, 20);
	MathOptimisationBtn->setBounds (25, 500, 150, 24);
}

void RTAPsetParams::buttonClicked (Button* buttonThatWasClicked)
{
	bXmlUpdate = false;

	if (buttonThatWasClicked == MathOptimisationBtn)
	{
		bMathOpt ^= true;
	}
}

void RTAPsetParams::comboBoxChanged (ComboBox* comboBoxThatHasChanged)
{
	int i, temp=0;
	String combotext;

	if (comboBoxThatHasChanged == MultiBFchannelComboBox)
	{
		combotext = MultiBFchannelComboBox->getText();

		// check to see if another number greater than 30 channels is used
		if (isdigit (combotext[0]))
			uiNumofBFchannels = combotext.getIntValue ();

		for (i=0; i<uiNumofBFchannels; i++)
			temp += i;
		uiBFChannelOffset = temp;
	}
	else if (comboBoxThatHasChanged == NumANfibreTypesComboBox)
	{
		uiNumofANfibreTypes = NumANfibreTypesComboBox->getSelectedId ();
	}
	else if (comboBoxThatHasChanged == ANfibreTypesComboBox)
	{
		iANfibreType = ANfibreTypesComboBox->getSelectedId ();
	}
}

void RTAPsetParams::sliderValueChanged(Slider* sliderThatWasMoved)
{
	bXmlUpdate = false;

	if (sliderThatWasMoved == GammatoneFiltOrderSlider)
	{
		iGammatoneFiltOrder = (int) GammatoneFiltOrderSlider->getValue();
	}
	else if (sliderThatWasMoved == BFlistMinSlider)
	{
		fMinBFlist = BFlistMinSlider->getValue();
	}
	else if (sliderThatWasMoved == BFlistMaxSlider)
	{
		fMaxBFlist = BFlistMaxSlider->getValue();
	}
	else if (sliderThatWasMoved == ComprThreshBMdBSlider)
	{
		fComprThreshBMdB = ComprThreshBMdBSlider->getValue();
	}
	else if (sliderThatWasMoved == DRNLaSlider)
	{
		fDRNLa = DRNLaSlider->getValue();
	}
	else if (sliderThatWasMoved == DRNLbSlider)
	{
		fDRNLb = DRNLbSlider->getValue();
	}
	else if (sliderThatWasMoved == DRNLcSlider)
	{
		fDRNLc = DRNLcSlider->getValue();
	}
	else if (sliderThatWasMoved == LinGainSlider)
	{
		fLinGain = LinGainSlider->getValue();
	}
}
