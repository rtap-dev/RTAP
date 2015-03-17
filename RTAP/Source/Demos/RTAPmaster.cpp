
#include "RTAPmaster.h"
#include "RTAPsetInputParams.h"
#include "RTAPsetParams.h"
#include "RTAPmainGUI.h"
#include "RTAPbinaryConverter.h"

LiveAudioInput::LiveAudioInput()
{
    nextSample = subSample = stepInvoked = 0;
    fRTaudioAccumulator = 0;
	toneTracker = 0;
    zeromem (fAudioSamples2Display, sizeof (fAudioSamples2Display));
    setOpaque (true);
	sBuffQuadrant = -1;

	// Sets the timing interval in milliseconds
    startTimer (1000 / 50); // use a timer to keep repainting this component
}

LiveAudioInput::~LiveAudioInput()
{
}

void LiveAudioInput::paint (Graphics& g)
{
#if AUWAVE_DEBUG
	char *xbuff, *ybuff, *midYbuff, *sampleNumbuff, *nextSampbuff, *sampBuff1, *sampBuff2;
	char *Heightbuff, *Widthbuff, *numElementsArraybuff, *sampleRatebuff, *AuInNumSampBuff;
	char *APbuffTestShow;
#if 0
	xbuff = new char [50];
	ybuff = new char [50];
	midYbuff = new char [50];
	sampleNumbuff = new char [50];
#endif
	nextSampbuff = new char [50];
	AuInNumSampBuff = new char [50];
	sampleRatebuff = new char [50];
	APbuffTestShow = new char [50];
#if 0
	sampBuff1 = new char [50];
	sampBuff2 = new char [50];
	Heightbuff = new char [50];
	Widthbuff = new char [50];
	numElementsArraybuff = new char [50];
#endif
#endif

	// fills the background colour of audio display
	g.fillAll (Colours::white);

	// audio signal colour
	g.setColour (Colours::darkgreen);

#if SINGLE_PRECISION
    const float midY = getHeight() * 0.5f;
#elif DOUBLE_PRECISION
	const double midY = getHeight() * 0.5f;
#endif
	int x;
	int sampleNum = (nextSample + BUFFERGENSIZE - 1);
#if SINGLE_PRECISION
	float fNextSampleSizeY, fCurrYPixelDisp;
#elif DOUBLE_PRECISION
	double fNextSampleSizeY, fCurrYPixelDisp;
#endif

	x = jmin (getWidth(), numElementsInArray(fAudioSamples2Display));

    for (x; --x >= 0;)
    {
		// hi-res audio waveform display
		if (x > 600)
			fNextSampleSizeY = (midY - (midY * fAudioHiResSamples[sampleNum-- % numElementsInArray(fAudioHiResSamples)]))-midY;
		else
			// acquire y-coord for next sample
			fNextSampleSizeY = (midY - (midY * fAudioSamples2Display[sampleNum-- % numElementsInArray(fAudioSamples2Display)]))-midY;

		// update current sample
		fCurrYPixelDisp = fNextYPixel;
		// offset the next sample for display
		fNextYPixel = midY + fNextSampleSizeY;
		// draw a line between the 2 points
		g.drawLine(x, fCurrYPixelDisp, x - 1, fNextYPixel, 1);
    }

#if AUWAVE_DEBUG
//	g.drawFittedText(itoa(x,xbuff,10), -200, 10, getWidth()-20, getHeight()/3, Justification::centred, 1);
//	g.drawFittedText(itoa(midY, midYbuff, 10), -100, 10, getWidth()-20, getHeight()/3, Justification::centred, 1);
//	g.drawFittedText(itoa(sampleNum,sampleNumbuff,10), -50, 10, getWidth()-20, getHeight()/3, Justification::centred, 1);
//	g.drawFittedText(itoa(nextSample-uiPrevPaintNextSample,nextSampbuff,10), 0, 10, getWidth()-20, getHeight()/3, Justification::centred, 1);
//	g.drawFittedText(itoa(uiIncomingNumSamples,AuInNumSampBuff,10), 50, 10, getWidth()-20, getHeight()/3, Justification::centred, 1);
//	g.drawFittedText(itoa(fNextSampleSizeY,sampBuff2,10), 100, 10, getWidth()-20, getHeight()/3, Justification::centred, 1);
//	g.drawFittedText(itoa(getHeight(),Heightbuff,10), 150, 10, getWidth()-20, getHeight()/3, Justification::centred, 1);
//	g.drawFittedText(itoa(getWidth(),Widthbuff,10), 200, 10, getWidth()-20, getHeight()/3, Justification::centred, 1);
//	g.drawFittedText(itoa(numElementsInArray(fAudioSamples2Display),numElementsArraybuff,10), 250, 10, getWidth()-20, getHeight()/3, Justification::centred, 1);
//	g.drawFittedText(itoa(fSampleRate,sampleRatebuff,10), 300, 10, getWidth()-20, getHeight()/3, Justification::centred, 1);

#if 0
	delete [] xbuff;
	delete [] midYbuff;
	delete [] sampleNumbuff;
#endif
	delete [] nextSampbuff;
	delete [] AuInNumSampBuff;
	delete [] sampleRatebuff;
	delete [] APbuffTestShow;
#if 0
	delete [] sampBuff1;
	delete [] sampBuff2;
	delete [] Heightbuff;
	delete [] Widthbuff;
	delete [] numElementsArraybuff;
#endif

	uiPrevPaintNextSample = nextSample;
#endif
}

void LiveAudioInput::timerCallback()
{
    repaint();
}

void LiveAudioInput::audioDeviceAboutToStart (AudioIODevice* device)
{
    zeromem (fAudioSamples2Display, sizeof (fAudioSamples2Display));
	fSampleRate = device->getCurrentSampleRate();
}

void LiveAudioInput::audioDeviceStopped()
{
    zeromem (fAudioSamples2Display, sizeof (fAudioSamples2Display));
}

void LiveAudioInput::audioDeviceIOCallback (const float** inputChannelData, int numInputChannels,
                                                       float** outputChannelData, int numOutputChannels, int numSamples)
{
#if DISPLAY_PROCTIME
	dPrevAudioDevIOstartTime = dAudioDevIOstartTime;
	dAudioDevIOstartTime = Time::getMillisecondCounterHiRes();
#endif

	int skip = siSamp2Skip;
	int chan = 0;
	unsigned int i;

	// keep track of number of audio packets being acquired
	if (bPlayPauseStatus)
		uiAudioPackets2BeProcessed++;

	for (i=0; i<numSamples; ++i)
    {
//        for (int chan = 0; chan < numInputChannels; ++chan)
        {
            // mic in audio
			if (cAudioSource == AUDIO_SYSTEM)
			{
				if (inputChannelData[chan] != 0)
				{
#if 0
					// put conditions just in case incoming audio data is outside -1 to 1 range
					if (inputChannelData[chan][i] > 1)
						fRTaudioAccumulator += 1;							// place a boundary for values > 1 i.e. = 1
					else if (inputChannelData[chan][i] < -1)
						fRTaudioAccumulator += -1;							// place a boundary for values < -1 i.e. = -1
					else
						// fabsf calculates the absolute value and rounds it to the nearest whole number
						fRTaudioAccumulator += inputChannelData[chan][i];

					fRTaudioAccumulator = 0;
#endif

					if (bPlayPauseStatus)
					{
						if (!skip)
						{
							fAudioHiResSamples[(uiHiResAudioWriteTracker++) % HIRESBUFFSIZE] = inputChannelData[chan][i];
							// reset the audio sample skip parameter once audio acquired
							skip = siSamp2Skip;
						}
						else
							skip--;			// decrement till 0 b4 capturing 1 audio sample

						uiNumAuPcktRx++;		// increment to indicate 1 window of audio data received
					}
				}
			}
		}
    }
	
	pthread_mutex_lock ( &ComputeAlgoMutex );
	pthread_cond_signal ( &cvComputeAlgo );
	pthread_mutex_unlock ( &ComputeAlgoMutex );

	uiIncomingNumSamples = numSamples;

    // We need to clear the output buffers, in case they're full of junk..
    for (int i = 0; i < numOutputChannels; ++i)
        if (outputChannelData[i] != 0)
		{
#if SINGLE_PRECISION
            zeromem (outputChannelData[i], sizeof (float) * numSamples);
#elif DOUBLE_PRECISION
			zeromem (outputChannelData[i], sizeof (float) * numSamples);
#endif
		}

#if DISPLAY_PROCTIME
		if (bPlayPauseStatus)
		{
			dAudioDevIOendTime = Time::getMillisecondCounterHiRes();
			dAudioDevIOtime = dAudioDevIOendTime - dPrevAudioDevIOstartTime;
			dSummedAudioDevIOtime += dAudioDevIOtime;
			dNumGetAuSampInvk++;
			dNumSamples = numSamples;
		}
#endif
}

RTAPmaster::RTAPmaster ()
	: tabbedComponent (0),
	deviceManager (MainAppWindow::getSharedAudioDeviceManager())
{

    addAndMakeVisible (tabbedComponent = new TabbedComponent (TabbedButtonBar::TabsAtTop));
    tabbedComponent->setTabBarDepth (30);

	// add tabs here ...
#if JUCE_DISPLAY
    tabbedComponent->addTab ("RTAP Main" , Colours::lightgrey, new RTAPmainGUI (deviceManager), true);
#elif OPENGL_DISPLAY
//	tabbedComponent->addTab ("OpenGL display" ), Colours::lightgrey, new AuditoryPeripheryOpenGLmain (deviceManager), true);
#endif

	tabbedComponent->addTab ("Input Params" , Colours::lightgrey, new RTAPsetInputParams (deviceManager), true);
	tabbedComponent->addTab ("Algorithm Params" , Colours::lightgrey, new RTAPsetParams (deviceManager), true);
	tabbedComponent->addTab ("Binary Converter" , Colours::lightgrey, new AuditoryPeripheryBinConverter (deviceManager), true);

    tabbedComponent->setCurrentTabIndex (0);

    setSize (300, 200);
}

RTAPmaster::~RTAPmaster()
{
    deleteAndZero (tabbedComponent);
}

void RTAPmaster::paint (Graphics& g)
{
    g.fillAll (Colours::white);
}

void RTAPmaster::resized()
{
    tabbedComponent->setBounds (0, 0, getWidth() - 0, getHeight() - 0);
}

static JuceDemoType <RTAPmaster> demo ("04 RTAP");
