/*
  ==============================================================================

    DJAudioPlayer.cpp
    Created: 16 Jul 2022 10:47:59am
    Author:  pangj

  ==============================================================================
*/

#include "DJAudioPlayer.h"

DJAudioPlayer::DJAudioPlayer(AudioFormatManager& _formatManager)
                           : formatManager(_formatManager), 
                             isPlaying(false)
{
}

DJAudioPlayer::~DJAudioPlayer() {
}

/* Prepare the audio source for playing */
void DJAudioPlayer::prepareToPlay(int samplesPerBlockExpected, double sampleRate) {
  transportSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
  resampleSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

/* Called repeatedly to fetch subsequent blocks of audio data */
void DJAudioPlayer::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) {
  resampleSource.getNextAudioBlock(bufferToFill);
}

/* Release of resources that are no longer needed once playback stops */
void DJAudioPlayer::releaseResources() {
  transportSource.releaseResources();
  resampleSource.releaseResources();
}

/* Allows the DJAudioPlayer to be told to load a file */
void DJAudioPlayer::loadURL(URL audioURL) {
  // convert the audioURL into an audio input stream 
  // and pass the input stream into AudioFormatManager to create a reader
  auto* reader = formatManager.createReaderFor(audioURL.createInputStream(false));

  // check if the reader is created successfully
  if (reader != nullptr) { // if successful
    // Create an AudioFormatReaderSource - take numbers out of audio file and wraps up with the audio life cycle so we can use it as an audio source
    std::unique_ptr<AudioFormatReaderSource> newSource(new AudioFormatReaderSource(reader, true));
    // Pass the AudioFormatReaderSource into the transport source
    transportSource.setSource(newSource.get(), 0, nullptr, reader->sampleRate);

    DBG("DJAudioPlayer::loadURL loaded");

    // if anything goes wrong this will exit out of the function and clear up the memory
    // otherwise pass the pointer to the class scope variable
    readerSource.reset(newSource.release());
  }
}

/* Set the volume control */
void DJAudioPlayer::setGain(double gain) {
  if (gain < 0 || gain > 1.0) {
    DBG("DJAudioPlayer::setGain - gain should be between 0 and 1");
  }
  else {
    transportSource.setGain(gain);
  }
}

/* Set the speed control */
void DJAudioPlayer::setSpeed(double ratio) {
  if (ratio < 0 || ratio > 2.0) {
    DBG("DJAudioPlayer::setSpeed - ratio should be between 0 and 2");
  }
  else {
    resampleSource.setResamplingRatio(ratio);
  }
}

/* Set the position control */
void DJAudioPlayer::setPosition(double posInSecs) {
  transportSource.setPosition(posInSecs);
}

/* Set the position such that it matches the length of the audio file */
void DJAudioPlayer::setPositionRelative(double pos) {
  if (pos < 0 || pos > 1.0) {
    DBG("DJAudioPlayer::setPositionRelative - pos should be between 0 and 1");
  }

  else {
    double posInSecs = transportSource.getLengthInSeconds() * pos;
    setPosition(posInSecs);
  }
}

/* Start playing the track */
void DJAudioPlayer::start() {
  transportSource.start();
  isPlaying = true; // set true if the track starts playing
}

/* Stop playing the track */
void DJAudioPlayer::stop() {
  transportSource.stop();
  isPlaying = false; // set to false if the track stops playing
}

/* Get the relative position of the playhead */
double DJAudioPlayer::getPositionRelative() {
  if (transportSource.getLengthInSeconds() == 0)
    return 0;

  return transportSource.getCurrentPosition() / transportSource.getLengthInSeconds();
}

/* Get the current position of the playhead */
double DJAudioPlayer::getPosition() {
  return transportSource.getCurrentPosition();
}

/* Get the length of file in seconds */
double DJAudioPlayer::getLengthInSeconds() {
  return transportSource.getLengthInSeconds();
}
