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

#include "JuceDemoHeader.h"
#include <fcntl.h>


//==============================================================================
/**
*/
class IntroScreen  : public Component
{
public:
    IntroScreen()
        : linkButton ("https://code.google.com/p/rtap/", URL ("https://code.google.com/p/rtap/"))
    {
        setOpaque (true);

		RTAPimage = ImageFileFormat::loadFrom (BinaryData::RTAPscreen1_jpg, BinaryData::RTAPscreen1_jpgSize);
		addAndMakeVisible (versionLabel);
        addAndMakeVisible (linkButton);

		versionLabel.setColour (Label::textColourId, Colours::black);
        versionLabel.setText (String ("RTAPv1.4.3 built on {date}")
                                  .replace ("{date}",    String (__DATE__).replace ("  ", " ")),
                              dontSendNotification);

		linkButton.setColour (HyperlinkButton::textColourId, Colours::darkblue);
    }

    void paint (Graphics& g) override
    {
        g.fillAll (Colour::greyLevel (0.16f));

		// RTAP welcome image draw
		AffineTransform transform (AffineTransform::scale (0.88f, 0.92f));
		g.drawImageTransformed (RTAPimage, transform, false);
    }

    void resized() override
    {
        Rectangle<int> area (getLocalBounds().reduced (10).removeFromBottom (24));
        linkButton.setBounds (area.removeFromRight (getWidth() / 4));
        versionLabel.setBounds (area);
    }

private:
    Label versionLabel;
    HyperlinkButton linkButton;
    TooltipWindow tooltipWindow;
	
	Image RTAPimage;
	ImageComponent RTAPgfxWelcome;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (IntroScreen)
};


// This static object will register this demo type in a global list of demos..
static JuceDemoType<IntroScreen> demo ("00 Welcome!");
