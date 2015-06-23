
#ifndef __RTAPMASTER_H__
#define __RTAPMASTER_H__

#define AUWAVE_DEBUG	0

#include "../JuceDemoHeader.h"
#include "globalvars.h"
#include "nconventions.h"

class LiveAudioInput  : public Component,
						public AudioIODeviceCallback,
						public Timer
{
public:

    LiveAudioInput();
    ~LiveAudioInput();

    void paint (Graphics& g);
    void timerCallback();

    void audioDeviceAboutToStart (AudioIODevice* device);
    void audioDeviceStopped();

    void audioDeviceIOCallback (const float** inputChannelData, int numInputChannels,
                                float** outputChannelData, int numOutputChannels, int numSamples);

private:
    int nextSample, subSample, toneTracker, stepInvoked, stepBoundary;
	signed int sBuffQuadrant;
#if SINGLE_PRECISION
    float fRTaudioAccumulator;
#elif DOUBLE_PRECISION
	double fRTaudioAccumulator;
#endif
	UINT uiPrevPaintNextSample;

    LiveAudioInput (const LiveAudioInput&);
    LiveAudioInput& operator= (const LiveAudioInput&);
};

class RTAPmaster  : public Component
{
public:

    RTAPmaster ();
    ~RTAPmaster();

    void paint (Graphics& g);
    void resized();

	UINT uiBuffSize;

    juce_UseDebuggingNewOperator

private:
//    AudioDeviceManager deviceManager;
	AudioDeviceManager& deviceManager;

    TabbedComponent* tabbedComponent;

    // (prevent copy constructor and operator= being generated..)
    RTAPmaster (const RTAPmaster&);
    const RTAPmaster& operator= (const RTAPmaster&);
};


#endif
