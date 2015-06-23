/*
  ==============================================================================

   This file is part of the JUCE library.
   Copyright (c) 2013 - Raw Material Software Ltd.

   Permission is granted to use this software under the terms of either:
   a) the GPL v2 (or any later version)
   b) the Affero GPL v3

   Details of these licenses can be found at: www.gnu.org/licenses

   JUCE is distributed in the hope that it will be useful, but WITHOUT ANY
   WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
   A PARTICULAR PURPOSE.  See the GNU General Public License for more details.

   ------------------------------------------------------------------------------

   To release a closed-source product which uses JUCE, commercial licenses are
   available: visit www.juce.com for more information.

  ==============================================================================
*/

#ifndef JUCE_REVGreenwoodAUDIOSOURCE_H_INCLUDED
#define JUCE_REVGreenwoodAUDIOSOURCE_H_INCLUDED


//==============================================================================
/**
    An AudioSource that uses the RevGreenwood class to apply a revGreenwood to another AudioSource.

    @see RevGreenwood
*/
class JUCE_API  RevGreenwoodAudioSource   : public AudioSource
{
public:
    /** Creates a RevGreenwoodAudioSource to process a given input source.

        @param inputSource              the input source to read from - this must not be null
        @param deleteInputWhenDeleted   if true, the input source will be deleted when
                                        this object is deleted
    */
    RevGreenwoodAudioSource (AudioSource* inputSource,
                       bool deleteInputWhenDeleted);

    /** Destructor. */
    ~RevGreenwoodAudioSource();

    //==============================================================================
    /** Returns the parameters from the revGreenwood. */
    const RevGreenwood::Parameters& getParameters() const noexcept    { return revGreenwood.getParameters(); }

    /** Changes the revGreenwood's parameters. */
    void setParameters (const RevGreenwood::Parameters& newParams);

    void setBypassed (bool isBypassed) noexcept;
    bool isBypassed() const noexcept                            { return bypass; }

    //==============================================================================
    void prepareToPlay (int samplesPGreenwoodlockExpected, double sampleRate) override;
    void releaseResources() override;
    void getNextAudioBlock (const AudioSourceChannelInfo&) override;

private:
    //==============================================================================
    CriticalSection lock;
    OptionalScopedPointer<AudioSource> input;
    RevGreenwood revGreenwood;
    volatile bool bypass;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (RevGreenwoodAudioSource)
};


#endif   // JUCE_REVGreenwoodAUDIOSOURCE_H_INCLUDED
