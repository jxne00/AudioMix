/*
  ==============================================================================

    WaveformDisplay.cpp
    Created: 20 Jul 2022 9:34:37pm
    Author:  pangj

  ==============================================================================
*/

#include <JuceHeader.h>
#include "WaveformDisplay.h"

//==============================================================================
WaveformDisplay::WaveformDisplay(AudioFormatManager& formatManagerToUse,
                                 AudioThumbnailCache& cacheToUse)
                               : audioThumb(1000, formatManagerToUse, cacheToUse),
                                 fileLoaded(false),
                                 position(0)
{
  audioThumb.addChangeListener(this);
}

WaveformDisplay::~WaveformDisplay() {
}

/* Drawing of the component */
void WaveformDisplay::paint(juce::Graphics& g) {
  g.fillAll(Colour{ 0xFF580C1F }); // background
  g.setColour(Colour{ 0xFF9AD2CB }); // set outline colour
  g.drawRect(getLocalBounds(), 1); // draw an outline around the component

  g.setColour(Colour{ 0xFFF2E5D7 }); // set text and waveform colour

  if (fileLoaded) {
      // draw the waveform
      audioThumb.drawChannel(g,
                             getLocalBounds(), // how much of g to draw
                             0, // start time 
                             audioThumb.getTotalLength(),  // end time (end of the track)
                             0, // channel 0
                             1.0f); // vertical zoom factor

    // playhead rectangle outline
    g.setColour(Colours::red);
    g.drawRect(position * getWidth(), 0, getWidth() / 28, getHeight(), 2);
  }

  else {
    g.setFont(15.0f); // font size
    g.setColour(Colours::white);
    g.drawText("Waveform not loaded", getLocalBounds(), Justification::centred, true);   // draw some placeholder text
  }
}

/* For receiving the callback */
void WaveformDisplay::changeListenerCallback(ChangeBroadcaster* source) {
  repaint();
}

/* Allows the WaveformDisplay to be told to load a file */
void WaveformDisplay::loadURL(URL audioURL) {
  audioThumb.clear();
  fileLoaded = audioThumb.setSource(new URLInputSource(audioURL));

  if (fileLoaded) {
    DBG("WaveformDisplay::loadURL " << audioURL.toString(true) << " loaded");
    repaint(); // waveform drawn only after calling repaint
  }
  else {
    DBG("WaveformDisplay::loadURL not loaded");
  }
}

/* Set the relative position of the playhead */
void WaveformDisplay::setPositionRelative(double pos) {
  // update whenever the positiion is changed 
  if (pos != position && pos > 0) {
    position = pos;
    repaint();
  }
}
