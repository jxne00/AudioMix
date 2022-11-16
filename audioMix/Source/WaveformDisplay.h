/*
  ==============================================================================

    WaveformDisplay.h
    Created: 20 Jul 2022 9:34:37pm
    Author:  pangj

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class WaveformDisplay : public juce::Component,
                        public ChangeListener
{
public:
  /**
   * \brief 
   *     Constructor.
   */
  WaveformDisplay(AudioFormatManager& formatManagerToUse,
                  AudioThumbnailCache& cacheToUse);

  /**
   * \brief 
   *     Destructor.
   */
  ~WaveformDisplay();

  /**
   * \brief
   *    Drawing of the component.
   *
   * \param g
   *    Graphic reference used to do carry out drawing operations
   */
  void paint(juce::Graphics&) override;

  /**
   * \brief 
   *     For receiving the callback.
   * 
   * \param source
   *     The ChangeBroadcaster that triggered the callback
   */
  void changeListenerCallback(ChangeBroadcaster* source) override;

  /**
   * \brief
   *    Allows the WaveformDisplay to be told to load a file.
   *
   * \param audioURL
   *    The URL to be loaded
   */
  void loadURL(URL audioURL);

  /**
   * \brief 
   *     Set the relative position of the playhead.
   * 
   * \param pos
   *     The updated position to be set to
   */
  void setPositionRelative(double pos);

private:
  AudioThumbnail audioThumb;

  // determines if a file has been loaded
  bool fileLoaded;

  // stores the position of the playhead
  double position;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(WaveformDisplay)
};
