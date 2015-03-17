/*
  ==============================================================================

   This file is part of the JUCE library - "Jules' Utility Class Extensions"
   Copyright 2004-12 by Raw Material Software Ltd.

  ------------------------------------------------------------------------------

   JUCE can be redistributed and/or modified under the terms of the GNU General
   Public License (Version 2), as published by the Free Software Foundation.
   A copy of the license is included in the JUCE distribution, or can be found
   online at www.gnu.org/licenses.

   JUCE is distributed in the hope that it will be useful, but WITHOUT ANY
   WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
   A PARTICULAR PURPOSE.  See the GNU General Public License for more details.

  ------------------------------------------------------------------------------

   To release a closed-source product which uses JUCE, commercial licenses are
   available: visit www.rawmaterialsoftware.com/juce for more information.

  ==============================================================================
*/

#include "../JuceDemoHeader.h"
#include "globalvars.h"


static String getRTAPinfo()
{
	static char Display[60], Waveform[60];
    String RTAPdata;

	if (cWaveformType == SCATTER_PLOT)
		strcpy_s (Waveform, "Scatter plot");
	else if (cWaveformType == LINE_PLOT)
		strcpy_s (Waveform, "Line plot");
	else if (cWaveformType == SPECTROGRAM_PLOT)
		strcpy_s (Waveform, "Spectrogram plot");

	if (cDisplayMode == STATIC_DISPLAY)
		strcpy_s (Display, "Static display");
	else if (cDisplayMode == AUTOREFRESH_DISPLAY)
		strcpy_s (Display, "Auto-refresh display");
	else if (cDisplayMode == SCROLL_DISPLAY)
		strcpy_s (Display, "Scroll display");

    RTAPdata
      << "RTAP SETTINGS INFORMATION...\n"

	  << "\nGeneral\n"
	  << "RTAP runtime priority:.......................................................................... " <<
	  cProcessPriority << "\n"
	  << "Audio source:................................................................................... " <<
	  cAudioSourceName << "\n"
	  << "Math optimisation:.............................................................................. " <<
	  cMathOpt << "\n"
	  << "Algorithm function invoked:..................................................................... " <<
	  cFunctionName << "\n"

	  << "\nInput\n"
	  << "Input audio level:.............................................................................. " <<
	  fAudioLvldBSPL << " dB SPL\n"
	  << "Input tone frequency:........................................................................... " <<
	  iSineFreq << " Hz\n"
	  << "Input tone amplitude:........................................................................... " <<
	  fSineInpAmpAdj << " \n"

	  << "\nBasilar membrane (BM)\n"
	  << "Number of best frequency (BF) channels set:..................................................... " <<
	  dDisplayNumBFchannels << "\n"
	  << "Minimum best frequency (BF) range:.............................................................. " <<
	  fMinBFlist << " Hz\n"
	  << "Maximum best frequency (BF) range:.............................................................. " <<
	  fMaxBFlist << " Hz\n"
	  << "Compression threshold:.......................................................................... " <<
	  fComprThreshBMdB << " dB\n"
	  << "Gammatone filter order:......................................................................... " <<
	  iGammatoneFiltOrder << "\n"

	  << "\nInner hair cell (IHC) presynapse region\n"
	  << "Number of auditory nerve (AN) fibre types set:.................................................. " <<
	  dDisplayNumANfibreTypes << "\n"
	  << "Number of auditory nerve (AN) channels set:..................................................... " <<
	  dDisplayNumANchannels << "\n"
	  << "AN fibre type processed:........................................................................ " <<
	  cANfibreSelect << "\n"

	  << "\nDisplay\n"
	  << "Waveform plot type:............................................................................. " <<
	  Waveform << "\n"
	  << "Waveform display mode:.......................................................................... " <<
	  Display << "\n"
	  << "Per window boundary display:.................................................................... " <<
	  cWindowBoundaryDisp << "\n"
	  << "Peak amplitude display:......................................................................... " <<
	  cPeakAmpDisp << "\n"

	  << "\nRTAP RUNTIME INFORMATION...\n\n"

	  << "Number of times audio device IO callback (for mic in & audio speaker loopback) function invoked: " << 
	  dNumGetAuSampInvk << "\n"
	  << "Average runtime of audio device IO callback function per invokation:............................ " << 
	  dSummedAudioDevIOtime / dNumGetAuSampInvk << " ms\n" 
	  << "Number of times outer & middle ear (OME) function invoked:...................................... " << 
	  dNumOMEinvk << "\n"
	  << "Average runtime of outer & middle ear (OME) function per invokation:............................ " << 
	  dSummedOMEtime / dNumOMEinvk << " ms\n"
	  << "Number of times algorithm function invoked:..................................................... " <<
	  dNumAlgoInvk << "\n"
	  << "Average runtime of algorithm per invokation:.................................................... " <<
	  dSummedAlgoTime / dNumAlgoInvk << " ms\n"
	  << "Number of times wave image buffer render function invoked:...................................... " <<
	  dNumPlotRenderInvk << "\n"
	  << "Average runtime of wave image buffer render function:........................................... " <<
	  dSummedPlotRenderTime/dNumPlotRenderInvk << " ms\n";

    DBG (RTAPdata);
    return RTAPdata;
}

class RTAPinfo  : public Component
{
public:
    RTAPinfo()
    {
        addAndMakeVisible (resultsBox);
        resultsBox.setReadOnly (true);
        resultsBox.setMultiLine (true);
        resultsBox.setColour (TextEditor::backgroundColourId, Colours::transparentBlack);
        resultsBox.setFont (Font (Font::getDefaultMonospacedFontName(), 12.0f, Font::plain));
        resultsBox.setText (getRTAPinfo());
    }

    void paint (Graphics& g) override
    {
        g.fillAll (Colour::greyLevel (0.93f));
    }

    void resized() override
    {
        resultsBox.setBounds (getLocalBounds().reduced (8));
    }

private:
    TextEditor resultsBox;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (RTAPinfo);
};

// This static object will register this demo type in a global list of demos..
static JuceDemoType<RTAPinfo> demo ("04 RTAP Info");
