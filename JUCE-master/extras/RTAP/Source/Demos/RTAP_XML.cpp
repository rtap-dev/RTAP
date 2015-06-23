/*

*/

#include "../JuceDemoHeader.h"
#include "globalvars.h"

//==============================================================================
class RTAP_XML   :	public Component,
					private Button::Listener,
					private CodeDocument::Listener
{
public:
    /** The type of database to parse. */
    enum Type
    {
        xml
    };

    RTAP_XML()
        : codeDocumentComponent (codeDocument, nullptr),
		GenerateXMLbutton (0),
		SaveXMLbutton (0)
    {
        setOpaque (true);

		// Generate and display RTAP parameters in XML format
		addAndMakeVisible (GenerateXMLbutton = new TextButton ());
		GenerateXMLbutton->setButtonText ("Generate");
		GenerateXMLbutton->setTooltip ("Generate & display XML script from RTAP parameters");
		GenerateXMLbutton->addListener (this);

		// Load XML from file button
		addAndMakeVisible (LoadXMLbutton = new TextButton ());
		LoadXMLbutton->setButtonText ("Load");
		LoadXMLbutton->setTooltip ("Load RTAP parameters from XML file");
		LoadXMLbutton->addListener (this);

		// Save XML to file button
		addAndMakeVisible (SaveXMLbutton = new TextButton ());
		SaveXMLbutton->setButtonText ("Save");
		SaveXMLbutton->setTooltip ("Save RTAP parameters as an XML file");
		SaveXMLbutton->addListener (this);

		// Load RTAP params from XML file

        addAndMakeVisible (codeDocumentComponent);
        codeDocument.addListener (this);

        addAndMakeVisible (errorMessage);
        errorMessage.setReadOnly (true);
        errorMessage.setMultiLine (true);
        errorMessage.setCaretVisible (false);
        errorMessage.setColour (TextEditor::outlineColourId, Colours::transparentWhite);
        errorMessage.setColour (TextEditor::shadowColourId, Colours::transparentWhite);
    }

    ~RTAP_XML()
    {
    }

    void paint (Graphics& g) override
    {
        fillBrushedAluminiumBackground (g);
    }

    void resized() override
    {
        Rectangle<int> area (getLocalBounds());
		GenerateXMLbutton->setBounds (area.removeFromTop (36).removeFromLeft (70).reduced (8));
		LoadXMLbutton->setBounds (90, 8, 60, 20);
		SaveXMLbutton->setBounds (180, 8, 60, 20);
        codeDocumentComponent.setBounds (area.removeFromTop(area.getHeight()).reduced (8));
        errorMessage.setBounds (codeDocumentComponent.getBounds());
    }

private:
	TextButton* GenerateXMLbutton;
	TextButton* LoadXMLbutton;
	TextButton* SaveXMLbutton;
    CodeDocument codeDocument;
    CodeEditorComponent codeDocumentComponent;
//    TreeView resultsTree;

    ScopedPointer<TreeViewItem> rootItem;
    ScopedPointer<XmlElement> parsedXml;
    TextEditor errorMessage;

    void rebuildTree()
    {
        ScopedPointer<XmlElement> openness;

        if (rootItem != nullptr)
            openness = rootItem->getOpennessState();

        createNewRootNode();

        if (openness != nullptr && rootItem != nullptr)
            rootItem->restoreOpennessState (*openness);
    }

    void createNewRootNode()
    {
        // clear the current tree
        rootItem = nullptr;

		rebuildXml();

        // if we have a valid TreeViewItem hide any old error messages and set our TreeView to use it
        if (rootItem != nullptr)
            errorMessage.clear();

        errorMessage.setVisible (! errorMessage.isEmpty());
    }

    /** Parses the editors contects as XML. */
    TreeViewItem* rebuildXml()
    {
        parsedXml = nullptr;

        XmlDocument doc (codeDocument.getAllContent());
        parsedXml = doc.getDocumentElement();

        if (parsedXml == nullptr)
        {
            String error (doc.getLastParseError());

            if (error.isEmpty())
                error = "Unknown error";

            errorMessage.setText ("Error parsing XML: " + error, dontSendNotification);

            return nullptr;
        }

        return 0;
    }

    void buttonClicked (Button* buttonThatWasClicked) override
    {
		if (buttonThatWasClicked == GenerateXMLbutton)
		{
			GenerateXML ();
		}
		else if (buttonThatWasClicked == LoadXMLbutton)
		{
			LoadXML();
		}
		else if (buttonThatWasClicked == SaveXMLbutton)
		{
			SaveXML ();
		}
    }

	void GenerateXML ()
	{
		// create an outer node called RTAP
		XmlElement RTAPparams ("RTAP");
		// RTAP parameters parsed as XML strings
		XmlElement* Configuration = new XmlElement ("Configuration");
		XmlElement* Comment1 = new XmlElement ("Comment");
		XmlElement* Comment2 = new XmlElement ("Comment");
		XmlElement* Comment3 = new XmlElement ("Comment");
		XmlElement* Comment4 = new XmlElement ("Comment");
		XmlElement* General = new XmlElement ("General");
		Comment1->setAttribute ("RTAPpriority ", "              // RTAP Process Priority - Normal = 0x20, Above Normal = 0x8000, High = 0x80, Real-time = 0x100");
		Comment2->setAttribute ("AudioSource ", "               // Audio source - System audio (Mic in or speaker loopback) = 0, Sine input = 1, Square_pulse input = 2, Sawtooth input = 3");
		Comment3->setAttribute ("AlgorithmToRun ", "            // Algorithm to run - DRNL = 0x09, DRNL-to-IHCRP = 0x0A, DRNL-to-IHCRP = 0x0B, DRNL-to-AN = 0x0C");
		Comment4->setAttribute ("MathOptimisation ", "          // Math optimisation boolean flag - Enable = 1, Disable = 0");
		General->setAttribute ("RTAPpriority", iRTAPpriority);
		General->setAttribute ("AudioSource", cAudioSource);
		General->setAttribute ("AlgorithmToRun", iFunctionToRun);
		General->setAttribute ("MathOptimisation", bMathOpt);
		XmlElement* Comment5 = new XmlElement ("Comment");
		XmlElement* Comment6 = new XmlElement ("Comment");
		XmlElement* Comment7 = new XmlElement ("Comment");
		XmlElement* InputSignal = new XmlElement ("Input");
		Comment5->setAttribute ("Level ", "                     // Audio level (dB SPL) - Min value = 5, Max value = 90, Interval = 1");
		Comment6->setAttribute ("SineFrequency ", "             // Sine tone frequency (Hz) - Min value = 100, Max value = 6000, Interval = 50");
		Comment7->setAttribute ("SineAmplitude ", "             // Sine amplitude - Min value = 0.0, Max value = 1.0, Interval = 0.1");
		InputSignal->setAttribute ("Level", fAudioLvldBSPL);							// Audio input level (dB SPL) setting
		InputSignal->setAttribute ("SineFrequency", iSineFreq);							// Sine tone generator frequency setting
		InputSignal->setAttribute ("SineAmplitude", fSineInpAmpAdj);					// Sine tone generator amplitude setting
		XmlElement* Comment8 = new XmlElement ("Comment");
		XmlElement* Comment9 = new XmlElement ("Comment");
		XmlElement* Comment10 = new XmlElement ("Comment");
		XmlElement* Comment11 = new XmlElement ("Comment");
		XmlElement* Comment12 = new XmlElement ("Comment");
		XmlElement* BMconfig = new XmlElement ("BM");
		Comment8->setAttribute ("NumberOfBFchannels ", "        // Number of BF channels - Min value = 1, Max value = 300, Interval = 1");
		Comment9->setAttribute ("MinBF ", "                     // Minimum best frequency (Hz) - Min value = 200, Max value = 6000, Interval = 1");
		Comment10->setAttribute ("MaxBF ", "                     // Maximum best frequency (Hz) - Min value = 6000, Max value = 11025, Interval = 1");
		Comment11->setAttribute ("BMcompressionThreshold ", "    // Basilar membrane compression threshold (dB) - Min value = -20, Max value = 90, Interval = 1");
		Comment12->setAttribute ("GammatoneFilterOrder ", "      // Gammatone filter order - Min value = 1, Max value = 5, Interval = 1 --");
		BMconfig->setAttribute ("NumberOfBFchannels", dDisplayNumBFchannels);			// Number of BF channels setting
		BMconfig->setAttribute ("MinBF", fMinBFlist);									// min best frequency (Hz) setting
		BMconfig->setAttribute ("MaxBF", fMaxBFlist);									// max best frequency (Hz) setting
		BMconfig->setAttribute ("BMcompressionThreshold", fComprThreshBMdB);			// BM compression threshold (dB)
		BMconfig->setAttribute ("GammatoneFilterOrder", iGammatoneFiltOrder);			// Gammatone filter order
		XmlElement* Comment13 = new XmlElement ("Comment");
		XmlElement* Comment14 = new XmlElement ("Comment");
		XmlElement* Comment15 = new XmlElement ("Comment");
		XmlElement* Comment16 = new XmlElement ("Comment");
		XmlElement* DRNL = new XmlElement ("DRNL");
		Comment13->setAttribute ("a ", "                       // DRNL a - Min value = 1, Max value = 100000, Interval = 1");
		Comment14->setAttribute ("b ", "                       // DRNL b - Min value = 0.0, Max value = 1.0, Interval = 0.0001");
		Comment15->setAttribute ("c ", "                       // DRNL c - Min value = 0.0, Max value = 1.0, Interval = 0.1");
		Comment16->setAttribute ("LinearGain ", "              // Linear pathway gain - Min value = -20, Max value = 90, Interval = 1");
		DRNL->setAttribute ("a", fDRNLa);
		DRNL->setAttribute ("b", fDRNLb);
		DRNL->setAttribute ("c", fDRNLc);
		DRNL->setAttribute ("LinearGain", fLinGain);
		XmlElement* Comment17 = new XmlElement ("Comment");
		XmlElement* Comment18 = new XmlElement ("Comment");
		XmlElement* NRR = new XmlElement ("NRR");
		Comment17->setAttribute ("NumberOfANfibreTypes ", "		// Number of AN fibre types - Min value = 1, Max value = 2, Interval = 1");
		Comment18->setAttribute ("ANfibreSelect ", "			// AN fibre select (only when NumberOfANfibreTypes = 1) - LSR = 1, HSR = 2");
		NRR->setAttribute ("NumberOfANfibreTypes", dDisplayNumANfibreTypes);
		NRR->setAttribute ("ANfibreSelect", iANfibreType);
		XmlElement* Comment19 = new XmlElement ("Comment");
		XmlElement* Comment20 = new XmlElement ("Comment");
		XmlElement* Comment21 = new XmlElement ("Comment");
		XmlElement* Comment22 = new XmlElement ("Comment");
		XmlElement* Display = new XmlElement ("Display");
		Comment19->setAttribute ("WaveformType ", "              // RTAP waveform display - Scatter plot = 1, Line plot = 2, Spectrogram plot = 2");
		Comment20->setAttribute ("DisplayMode ", "               // RTAP display mode - Static display = 1, Auto refresh = 2, Scroll = 3");
		Comment21->setAttribute ("PerWindowBoundaryDisplay ", "  // Per window segment display- Enable = 1, Disable= 0");
		Comment22->setAttribute ("PeakAmplitudeDisplay ", "      // Peak amplitude display - Enable = 1, Disable= 0 ");
		Display->setAttribute ("WaveformType", cWaveformType);							// 1: Scatter, 2: Line, 3: Spectrogram
		Display->setAttribute ("DisplayMode", cDisplayMode);							// 1: Static, 2: Auto refresh 3: Scroll
		Display->setAttribute ("PerWindowBoundaryDisplay", bWindowBoundaryDisp);		// Window boundary display toggle
		Display->setAttribute ("PeakAmplitudeDisplay", bPeakAmpDisp);					// Peak amplitude display indication among BF channels

		BMconfig->addChildElement (Comment13);
		BMconfig->addChildElement (Comment14);
		BMconfig->addChildElement (Comment15);
		BMconfig->addChildElement (Comment16);
		BMconfig->addChildElement (DRNL);
		Configuration->addChildElement (Comment1);
		Configuration->addChildElement (Comment2);
		Configuration->addChildElement (Comment3);
		Configuration->addChildElement (Comment4);
		Configuration->addChildElement (General);
		Configuration->addChildElement (Comment5);
		Configuration->addChildElement (Comment6);
		Configuration->addChildElement (Comment7);
		Configuration->addChildElement (InputSignal);
		Configuration->addChildElement (Comment8);
		Configuration->addChildElement (Comment9);
		Configuration->addChildElement (Comment10);
		Configuration->addChildElement (Comment11);
		Configuration->addChildElement (Comment12);
		Configuration->addChildElement (BMconfig);
		Configuration->addChildElement (Comment17);
		Configuration->addChildElement (NRR);
		Configuration->addChildElement (Comment18);
		Configuration->addChildElement (Comment19);
		Configuration->addChildElement (Comment20);
		Configuration->addChildElement (Comment21);
		Configuration->addChildElement (Display);
		RTAPparams.addChildElement (Configuration);

		codeDocument.replaceAllContent (RTAPparams.createDocument (String::empty));
	}

	void LoadXML ()
	{
		FileChooser fc ("Load XML file", File::getCurrentWorkingDirectory (), "*.xml", true);

		if (fc.browseForFileToOpen ())
		{
			File fLoadFile = fc.getResult();

			if (fLoadFile.existsAsFile ())
			{
				TranslateXML(XmlDocument::parse (fLoadFile));
				codeDocument.replaceAllContent (fLoadFile.loadFileAsString());
				// set flag ot indicate RTAP parameters updated from xml
				// this flag can be reset when anyone of RTAP input sources i.e. button, combo box, slider is updated.
				bXmlUpdate = true;
			}
		}
	}

	void SaveXML ()
	{
		FileChooser fc ("Save XML file", File::getCurrentWorkingDirectory (), "*.xml", true);

		if (fc.browseForFileToSave (true))
		{
			File fSaveFile = fc.getResult ();

			if (fSaveFile.existsAsFile ())
				fSaveFile.replaceWithText (codeDocument.getAllContent (), false, false);
			else
				fSaveFile.appendText (codeDocument.getAllContent (), false, false);
		}
	}

	void TranslateXML (const XmlElement* const xml)
	{
		// Ensure that XML loaded has RTAP tag
		if (xml !=  nullptr && xml->hasTagName ("RTAP"))
		{
			forEachXmlChildElement (*xml, config)
			{
				// Next ensure that Configuration tag is present after RTAP
				if (config->hasTagName ("Configuration"))
				{
					forEachXmlChildElement (*config, e)
					{
						// get RTAP general settings
						if (e->hasTagName ("General"))
						{
							iRTAPpriority = e->getIntAttribute ("RTAPpriority");
							cAudioSource = (char) e->getIntAttribute ("AudioSource");
							if (cAudioSource == AUDIO_SYSTEM) strcpy_s (cAudioSourceName, "System audio (Mic-in or Speaker loopback");
							else if (cAudioSource == AUDIO_SINE) strcpy_s (cAudioSourceName, "Sine tone");
							else if (cAudioSource == AUDIO_SQUARE) strcpy_s (cAudioSourceName, "Square pulse tone");
							else if (cAudioSource == AUDIO_SAWTOOTH) strcpy_s (cAudioSourceName, "Sawtooth tone");
							iFunctionToRun = e->getIntAttribute ("AlgorithmToRun");
							bMathOpt = e->getIntAttribute ("MathOptimisation");
						}
						// get RTAP input parameters
						else if (e->hasTagName ("Input"))
						{
//							fSamplingRate = (float) e->getDoubleAttribute ("SamplingRate");
							fAudioLvldBSPL = (float) e->getDoubleAttribute ("Level");
							// Range check & cap for audio level from 5dB SPL to 90dB SPL
							fAudioLvldBSPL = RangeCap (fAudioLvldBSPL, 5.f, 90.f);
							iSineFreq = e->getIntAttribute ("SineFrequency");
							iSineFreq = RangeCap (iSineFreq, 100, 6000);
							fSineInpAmpAdj = (float) e->getDoubleAttribute ("SineAmplitude");
							fSineInpAmpAdj = RangeCap (fSineInpAmpAdj, 0.0f, 1.0f);
						}
						// get RTAP best frequency (BF) parameters
						else if (e->hasTagName ("BM"))
						{
							uiNumofBFchannels = (UINT) e->getIntAttribute ("NumberOfBFchannels");
							uiNumofBFchannels = RangeCap (uiNumofBFchannels, 1, NUMOFBFCHANNELS);
							fMinBFlist = (float) e->getDoubleAttribute ("MinBF");
							fMinBFlist = RangeCap (fMinBFlist, 200.f, 6000.f);
							fMaxBFlist = (float) e->getDoubleAttribute ("MaxBF");
							fMaxBFlist = RangeCap (fMaxBFlist, 6000.f, 11025.f);
							fComprThreshBMdB = (float) e->getDoubleAttribute ("BMcompressionThreshold");
							fComprThreshBMdB = RangeCap (fComprThreshBMdB, -20.f, 90.f);
							iGammatoneFiltOrder = e->getIntAttribute ("GammatoneFilterOrder");
							iGammatoneFiltOrder = RangeCap(iGammatoneFiltOrder, 1, 5);
							forEachXmlChildElement (*e, DRNL)
							{
								if (DRNL->hasTagName ("DRNL"))
								{
									fDRNLa = (float) DRNL->getDoubleAttribute ("a");
									fDRNLa = RangeCap (fDRNLa, 0.f, 10000.f);
									fDRNLb = (float) DRNL->getDoubleAttribute ("b");
									fDRNLb = RangeCap (fDRNLb, 0.f, 1.f);
									fDRNLc = (float) DRNL->getDoubleAttribute ("c");
									fDRNLc = RangeCap (fDRNLc, 0.f, 1.f);
									fLinGain = (float) DRNL->getDoubleAttribute ("LinearGain");
									fLinGain = RangeCap (fLinGain, 200.f, 1000.f);
								}
							}
						}
						else if (e->hasTagName ("NRR"))
						{
							uiNumofANfibreTypes = e->getIntAttribute ("NumberOfANfibreTypes");
							uiNumofANfibreTypes = RangeCap (uiNumofANfibreTypes, 1, 2);
							iANfibreType = e->getIntAttribute ("ANfibreSelect");
							iANfibreType = RangeCap (iANfibreType, 1, 2);
						}
						else if (e->hasTagName ("Display"))
						{
							cWaveformType = (char) e->getIntAttribute ("WaveformType");
							cDisplayMode = (char) e->getIntAttribute ("DisplayMode");
							if (cDisplayMode == STATIC_DISPLAY)
							{
								bScrollPlot = false;
								bAutoRefreshPlot = false;
							}
							else if (cDisplayMode == AUTOREFRESH_DISPLAY)
							{
								bAutoRefreshPlot = true;
							}
							else if (cDisplayMode == SCROLL_DISPLAY)
							{
								bScrollPlot = true;
								bAutoRefreshPlot = false;
							}
							bWindowBoundaryDisp = e->getBoolAttribute ("PerWindowBoundaryDisplay");
							bPeakAmpDisp = e->getBoolAttribute ("PeakAmplitudeDisplay");
						}
					}
				}
			}
		}
	}

	// overloaded RangeCap function
	int RangeCap (int iVar2chk, int iMin, int iMax)
	{
		if (iVar2chk < iMin) return iMin;
		else if (iVar2chk > iMax) return iMax;
		else return iVar2chk;
	}
	float RangeCap (float fVar2chk, float fMin, float fMax)
	{
		if (fVar2chk < fMin) return fMin;
		else if (fVar2chk > fMax) return fMax;
		else return fVar2chk;
	}

    void codeDocumentTextInserted (const String&, int) override     { rebuildTree(); }
    void codeDocumentTextDeleted (int, int) override                { rebuildTree(); }

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (RTAP_XML)
};


// This static object will register this demo type in a global list of demos..
static JuceDemoType<RTAP_XML> demo ("40 RTAP XML");
