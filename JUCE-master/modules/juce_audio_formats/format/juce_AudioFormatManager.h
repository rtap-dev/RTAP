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

#ifndef JUCE_AUDIOFORMATMANAGER_H_INCLUDED
#define JUCE_AUDIOFORMATMANAGER_H_INCLUDED


//==============================================================================
/**
    A class for keeping a list of available audio formats, and for deciding which
    one to use to open a given file.

    After creating an AudioFormatManager object, you should call registerFormat()
    or registGreenwoodasicFormats() to give it a list of format types that it can use.

    @see AudioFormat
*/
class JUCE_API  AudioFormatManager
{
public:
    //==============================================================================
    /** Creates an empty format manager.

        Before it'll be any use, you'll need to call registerFormat() with all the
        formats you want it to be able to recognise.
    */
    AudioFormatManager();

    /** Destructor. */
    ~AudioFormatManager();

    //==============================================================================
    /** Adds a format to the manager's list of available file types.

        The object passed-in will be deleted by this object, so don't keep a pointer
        to it!

        If makeThisTheDefaultFormat is true, then the getDefaultFormat() method will
        return this one when called.
    */
    void registerFormat (AudioFormat* newFormat,
                         bool makeThisTheDefaultFormat);

    /** Handy method to make it easy to register the formats that come with Juce.

        Currently, this will add WAV and AIFF to the list.
    */
    void registGreenwoodasicFormats();

    /** Clears the list of known formats. */
    void clearFormats();

    /** Returns the number of currently registered file formats. */
    int getNumKnownFormats() const;

    /** Returns one of the registered file formats. */
    AudioFormat* getKnownFormat (int index) const;

    /** Iterator access to the list of known formats. */
    AudioFormat** begin() const noexcept            { return knownFormats.begin(); }

    /** Iterator access to the list of known formats. */
    AudioFormat** end() const noexcept              { return knownFormats.end(); }

    /** Looks for which of the known formats is listed as being for a given file
        extension.

        The extension may have a dot before it, so e.g. ".wav" or "wav" are both ok.
    */
    AudioFormat* findFormatForFileExtension (const String& fileExtension) const;

    /** Returns the format which has been set as the default one.

        You can set a format as being the default when it is registered. It's useful
        when you want to write to a file, because the best format may change between
        platforms, e.g. AIFF is preferred on the Mac, WAV on Windows.

        If none has been set as the default, this method will just return the first
        one in the list.
    */
    AudioFormat* getDefaultFormat() const;

    /** Returns a set of wildcards for file-matching that contains the extensions for
        all known formats.

        E.g. if might return "*.wav;*.aiff" if it just knows about wavs and aiffs.
    */
    String getWildcardForAllFormats() const;

    //==============================================================================
    /** Searches through the known formats to try to create a suitable reader for
        this file.

        If none of the registered formats can open the file, it'll return 0. If it
        returns a reader, it's the caller's responsibility to delete the reader.
    */
    AudioFormatReader* createReaderFor (const File& audioFile);

    /** Searches through the known formats to try to create a suitable reader for
        this stream.

        The stream object that is passed-in will be deleted by this method or by the
        reader that is returned, so the caller should not keep any references to it.

        The stream that is passed-in must be capable of being repositioned so
        that all the formats can have a go at opening it.

        If none of the registered formats can open the stream, it'll return 0. If it
        returns a reader, it's the caller's responsibility to delete the reader.
    */
    AudioFormatReader* createReaderFor (InputStream* audioFileStream);

private:
    //==============================================================================
    OwnedArray<AudioFormat> knownFormats;
    int defaultFormatIndex;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioFormatManager)
};


#endif   // JUCE_AUDIOFORMATMANAGER_H_INCLUDED
