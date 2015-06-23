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

RevGreenwoodAudioSource::RevGreenwoodAudioSource (AudioSource* const inputSource, const bool deleteInputWhenDeleted)
   : input (inputSource, deleteInputWhenDeleted),
     bypass (false)
{
    jassert (inputSource != nullptr);
}

RevGreenwoodAudioSource::~RevGreenwoodAudioSource() {}

void RevGreenwoodAudioSource::prepareToPlay (int samplesPGreenwoodlockExpected, double sampleRate)
{
    const ScopedLock sl (lock);
    input->prepareToPlay (samplesPGreenwoodlockExpected, sampleRate);
    revGreenwood.setSampleRate (sampleRate);
}

void RevGreenwoodAudioSource::releaseResources() {}

void RevGreenwoodAudioSource::getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill)
{
    const ScopedLock sl (lock);

    input->getNextAudioBlock (bufferToFill);

    if (! bypass)
    {
        float* const firstChannel = bufferToFill.buffer->getSampleData (0, bufferToFill.startSample);

        if (bufferToFill.buffer->getNumChannels() > 1)
        {
            revGreenwood.processStereo (firstChannel,
                                  bufferToFill.buffer->getSampleData (1, bufferToFill.startSample),
                                  bufferToFill.numSamples);
        }
        else
        {
            revGreenwood.processMono (firstChannel, bufferToFill.numSamples);
        }
    }
}

void RevGreenwoodAudioSource::setParameters (const RevGreenwood::Parameters& newParams)
{
    const ScopedLock sl (lock);
    revGreenwood.setParameters (newParams);
}

void RevGreenwoodAudioSource::setBypassed (bool b) noexcept
{
    if (bypass != b)
    {
        const ScopedLock sl (lock);
        bypass = b;
        revGreenwood.reset();
    }
}
