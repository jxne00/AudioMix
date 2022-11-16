#pragma once

#include <JuceHeader.h>
#include "DJAudioPlayer.h"
#include "DeckGUI.h"
#include "PlaylistComponent.h"
#include "QueueComponent.h"
#include "Crossfader.h"


//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent : public AudioAppComponent {
public:
  //==============================================================================
  /**
   * \brief 
   *     Constructor.
   */
  MainComponent();

  /**
   * \brief 
   *     Destructor.
   */
  ~MainComponent() override;

  //==============================================================================
  /**
   * \brief
   *     Prepare the audio source for playing.
   *
   * \param samplesPerBlockExpected
   *    Number of samples to be supplied by the source
   * \param sampleRate
   *    The sample rate at which the output will be used
   */
  void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;

  /**
   * \brief
   *    Called repeatedly to fetch subsequent blocks of audio data.
   *
   * \param bufferToFill
   *    The destination buffer to fill with audio data
   */
  void getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill) override;

  /**
   * \brief
   *     Release of resources that are no longer needed once playback stops.
   */
  void releaseResources() override;

  //==============================================================================
  /**
   * \brief
   *    Drawing of the component.
   *
   * \param g
   *    Graphic reference used to do carry out drawing operations
   */
  void paint(Graphics& g) override;

  /**
   * \brief
   *    Sets the size of each object in the component.
   */
  void resized() override;


private:
  //==============================================================================
  // Your private member variables go here...

  FileChooser fChooser{ "Select a file..." };

  AudioFormatManager formatManager;

  // store up to 30 files in the cache at any one time
  AudioThumbnailCache thumbCache{ 30 };

  // addding a DJAudioPlayer object as a data member
  DJAudioPlayer player1{ formatManager };
  DJAudioPlayer player2{ formatManager };

  DeckGUI deckGUI1{ &player1, formatManager, thumbCache, &queueComponent, true };
  DeckGUI deckGUI2{ &player2, formatManager, thumbCache, &queueComponent, false };

  MixerAudioSource mixerSource;
  QueueComponent queueComponent;

  PlaylistComponent playlistComponent{ &deckGUI1, &deckGUI2, &queueComponent };

  Crossfader crossfader{ &deckGUI1, &deckGUI2 };

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)
};
