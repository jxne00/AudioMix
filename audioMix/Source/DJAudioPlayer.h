/*
  ==============================================================================

    DJAudioPlayer.h
    Created: 16 Jul 2022 10:47:59am
    Author:  pangj

  ==============================================================================
*/

#pragma once
#include "../JuceLibraryCode/JuceHeader.h"

class DJAudioPlayer : public AudioSource {
public:

  /**
   * \brief
   *    Constructor.
   */
  DJAudioPlayer(AudioFormatManager& _formatManager);

  /**
   * \brief
   *    Destructor.
   */
  ~DJAudioPlayer();
  
  /**
   * \brief 
   *    Prepare the audio source for playing.
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

  /**
   * \brief
   *    Allows the DJAudioPlayer to be told to load a file.
   *
   * \param audioURL
   *    The URL to be loaded
   */
  void loadURL(URL audioURL);

  /**
   * \brief
   *    Set the volume control.
   *
   * \param gain
   *    The new gain amount
   */
  void setGain(double gain);

  /**
   * \brief
   *    Set the speed control.
   *
   * \param ratio
   *    The new resampling ratio (speed)
   */
  void setSpeed(double ratio);

  /**
   * \brief
   *    Set the position control (of the audio file).
   *
   * \param posInSecs
   *    The new playback position (in seconds)
   */
  void setPosition(double posInSecs);

  /**
   * \brief
   *    Set the position such that it matches the length of the audio file.
   *
   * \param pos
   *    The updated position to be set to
   */
  void setPositionRelative(double pos);

  /**
   * \brief
   *    Start playing the track.
   */
  void start();

  /**
   * \brief
   *    Stop playing the track.
   */
  void stop();

  /**
   * \brief
   *    Get the relative position of the playhead.
   *
   * \return
   *    The relative position of the playhead
   */
  double getPositionRelative();

  /**
   * \brief 
   *     Get the current position of the playhead.
   * 
   * \return
   *     The position of the playhead (in seconds)
   */
  double getPosition();

  /**
   * \brief 
   *     Get the length of file in seconds.
   * 
   * \return
   *    The length of the file in seconds
   */
  double getLengthInSeconds();

  /**
   * \brief
   *  variable for determining if the track is currently playing
   *  value is set to "true" if track is playing, false otherwise
   */
  bool isPlaying;

private:
  AudioFormatManager& formatManager;
  std::unique_ptr<AudioFormatReaderSource> readerSource;
  AudioTransportSource transportSource;
  ResamplingAudioSource resampleSource{ &transportSource, false, 2 };
};