#ifndef __RTAPSETINPUTPARAMS_H__
#define __RTAPSETINPUTPARAMS_H__

#include "../JuceDemoHeader.h"
#include "RTAPmaster.h"

class RTAPsetInputParams  :	public Component,
							public ButtonListener,
							public ComboBoxListener,
							public SliderListener
{
public:
    //==============================================================================
    RTAPsetInputParams (AudioDeviceManager& deviceManager_);
    ~RTAPsetInputParams();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    //[/UserMethods]

    void paint (Graphics& g);
    void resized();
    void buttonClicked (Button* buttonThatWasClicked);
	void comboBoxChanged (ComboBox* comboBoxThatHasChanged);
	void sliderValueChanged(Slider* sliderThatWasMoved);

    //==============================================================================

private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    AudioDeviceManager& deviceManager;
    //[/UserVariables]

    //==============================================================================
    GroupComponent* GraphTypeGroup;
	Label* AudioInputLevelLabel;
	Slider* AudioInputLevelSlider;
	Label* SamplingFreqLabel;
	ComboBox* SamplingFreqComboBox;
	Label* InputSineBoostLabel;
	Slider* InputSineBoostSlider;
	Label* InputSineFreqLabel;
	Slider* InputSineFreqSlider;
	ToggleButton* AudioDSound;
	ToggleButton* AudioSrcSine;
	ToggleButton* AudioSrcSquare;
	ToggleButton* AudioSrcSaw;
	ToggleButton* StaticDisplayBtn;
	ToggleButton* AutoRefreshDisplayBtn;
	ToggleButton* ScrollDisplayBtn;
	ToggleButton* ScatterDispTogBtn;
	ToggleButton* LineDispTogBtn;
	ToggleButton* SpectroDispTogBtn;
	//==============================================================================
    // (prevent copy constructor and operator= being generated..)
    RTAPsetInputParams (const RTAPsetInputParams&);
    const RTAPsetInputParams& operator= (const RTAPsetInputParams&);
};

#endif