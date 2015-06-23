/*
  ==============================================================================

  This is an automatically generated file created by the Jucer!

  Creation date:  13 Nov 2009 3:52:50 pm

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Jucer version: 1.12

  ------------------------------------------------------------------------------

  The Jucer is part of the JUCE library - "Jules' Utility Class Extensions"
  Copyright 2004-6 by Raw Material Software ltd.

  ==============================================================================
*/

#ifndef __RTAPMAINGUI_H__
#define __RTAPMAINGUI_H__

//[Headers]     -- You can add your own extra header files here --
#include "../JuceDemoHeader.h"
#include "RTAPmaster.h"
#include "RTAPgfx.h"
#include "globalvars.h"

//class AudioRecorder;
//[/Headers]

//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Jucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class RTAPmainGUI  :	public Component,
						public ButtonListener,
						public ComboBoxListener,
						public SliderListener
{
public:
    //==============================================================================
    RTAPmainGUI (AudioDeviceManager& deviceManager_);
    ~RTAPmainGUI();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    //[/UserMethods]

    void paint ( Graphics& g );
    void resized();
    void buttonClicked ( Button* buttonThatWasClicked );
	void comboBoxChanged ( ComboBox* comboBoxThatHasChanged );
	void sliderValueChanged ( Slider* sliderThatWasMoved );
    void visibilityChanged ();


    //==============================================================================
    juce_UseDebuggingNewOperator

private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    AudioDeviceManager& deviceManager;
//    ScopedPointer<AudioRecorder> recorder;
    //[/UserVariables]
	
    //==============================================================================
    LiveAudioInput* liveAudioDisplayComp;
	RTAPgfx* JUCEliveDisplay;
#if TEST_AUDIODEVICESETUP
	AudioDeviceManager::AudioDeviceSetup* SetAudioDevice;
	AudioDeviceManager::AudioDeviceSetup* GetAudioDevice;
	Label* SamplingFreqLabel;
	Slider* SamplingFreqSlider;
#endif
    Label* explanationLabel;
#if DOUBLE_PRECISION_BUTTON
	ToggleButton* DoublePrecisionBtn;
#endif
	TextButton* ProcDataRecordButton;
	TextButton* SetParamsButton;
	TextButton* PlayRecButton;
	TextButton* RefreshPlotButton;
	DrawableButton* PlayPauseButton;
	Label* APfunction2RunLabel;
	ComboBox* APfunction2RunComboBox;
	Label* APfunction2RecLabel;
	ComboBox* APfunction2RecComboBox;
	Label* RTAPpriorityLabel;
	ComboBox* RTAPpriorityComboBox;
	ToggleButton* WindowBoundaryDispBtn;
	ToggleButton* PeakAmpDispBtn;
	// Processing time labels for display
	Label* GetAudSampProcTime;
	Label* OMEProcTime;
	Label* ProcThread1Status;
	Label* ProcThread2Status;
	Label* FileWriterThread1Status;
	Label* FileWriterThread2Status;
	Label* PixDrawThread1Status;
	Label* PixDrawThread2Status;
	Label* ProcessPriority;
	Label* AlgoProcTimeThread1;
	Label* AlgoProcTimeThread2;
	Label* FileWriterThreadProcTime;
	Label* AudioDevIOCallbackProcTime;
	Label* PlotRenderThreadTime;
	Label* Plot2ScreenTime;
	Label* MaxPeak1;
	Label* MaxPeak2;
	Label* MaxPeak3;

	RTAPpriority eRTAPpriority;
	AuditoryPeriphery2Process eAuditoryPeriphery2Process;

#if TEST_AUDIODEVICESETUP
	double fsGet;
	UINT buffSize;
	BigInteger inputChannel;
	BigInteger outputChannel;
#endif

    RTAPmainGUI (const RTAPmainGUI&);
    const RTAPmainGUI& operator= (const RTAPmainGUI&);
};


#endif
