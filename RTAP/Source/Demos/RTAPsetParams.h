#ifndef __RTAPSETPARAMS_H__
#define __RTAPSETPARAMS_H__

#include "../JuceDemoHeader.h"
#include "RTAPmaster.h"

class RTAPsetParams  :	public Component,
						public ButtonListener,
						public ComboBoxListener,
						public SliderListener
{
public:
    //==============================================================================
    RTAPsetParams (AudioDeviceManager& deviceManager_);
    ~RTAPsetParams();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    //[/UserMethods]

    void paint (Graphics& g);
    void resized();
    void buttonClicked (Button* buttonThatWasClicked);
	void comboBoxChanged (ComboBox* comboBoxThatHasChanged);
	void sliderValueChanged(Slider* sliderThatWasMoved);

    //==============================================================================
    juce_UseDebuggingNewOperator

private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    AudioDeviceManager& deviceManager;
    //[/UserVariables]

    //==============================================================================
    Label* MultiBFchannelLabel;
	ComboBox* MultiBFchannelComboBox;
	Label* NumANfibreTypesLabel;
	ComboBox* NumANfibreTypesComboBox;
	Label* ANfibreTypesLabel;
	ComboBox* ANfibreTypesComboBox;
	GroupComponent* DRNLgroup;
	Label* GammatoneFiltOrderLabel;
	Slider* GammatoneFiltOrderSlider;
	Label* BFlistMinLabel;
	Slider* BFlistMinSlider;
	Label* BFlistMaxLabel;
	Slider* BFlistMaxSlider;
	Label* ComprThreshBMdBLabel;
	Slider* ComprThreshBMdBSlider;
	Label* DRNLaLabel;
	Slider* DRNLaSlider;
	Label* DRNLbLabel;
	Slider* DRNLbSlider;
	Label* DRNLcLabel;
	Slider* DRNLcSlider;
	Label* LinGainLabel;
	Slider* LinGainSlider;
    TextEditor* testResultsBox;
	ToggleButton* MathOptimisationBtn;

    //==============================================================================
    // (prevent copy constructor and operator= being generated..)
    RTAPsetParams (const RTAPsetParams&);
    const RTAPsetParams& operator= (const RTAPsetParams&);
};


#endif
