/*
  ==============================================================================

    DeckGUI.cpp
    Created: 17 Jul 2022 10:52:03am
    Author:  pangj

  ==============================================================================
*/

#include <JuceHeader.h>
#include "DeckGUI.h"

//==============================================================================
DeckGUI::DeckGUI(DJAudioPlayer* _player,
                 AudioFormatManager& formatManagerToUse,
                 AudioThumbnailCache& cacheToUse, 
                 QueueComponent* _queueComponent, 
                 bool _isDeck1)
               : player(_player),
                 waveformdisplay(formatManagerToUse, cacheToUse), 
                 queueComponent(_queueComponent),
                 isLoaded(false), isLooping(false), 
                 isDeck1(_isDeck1)
{
  // make buttons. sliders, and labels visible
  addAndMakeVisible(playpauseButton);
  addAndMakeVisible(resetButton);
  addAndMakeVisible(loadButton);
  addAndMakeVisible(twoTimesButton);
  addAndMakeVisible(muteButton);
  addAndMakeVisible(skipFrontButton);
  addAndMakeVisible(skipBackButton);
  addAndMakeVisible(loopButton);
  addAndMakeVisible(volSlider);
  addAndMakeVisible(speedSlider);
  addAndMakeVisible(posSlider);
  addAndMakeVisible(volumeLabel);
  addAndMakeVisible(speedLabel);
  addAndMakeVisible(name);
  addAndMakeVisible(length);
  addAndMakeVisible(movingLength);
  addAndMakeVisible(waveformdisplay);

  // add listeners ("this" is a pointer to each object)
  playpauseButton.addListener(this);
  resetButton.addListener(this);
  loadButton.addListener(this);
  twoTimesButton.addListener(this);
  muteButton.addListener(this);
  skipFrontButton.addListener(this);
  skipBackButton.addListener(this);
  loopButton.addListener(this);
  volSlider.addListener(this);
  speedSlider.addListener(this);
  posSlider.addListener(this);

  // Volume Slider
  volSlider.setRange(0.0, 1.0, 0.1);
  volSlider.setValue(0.5); // initial value of 0.5 
  volSlider.setSliderStyle(Slider::SliderStyle::LinearBarVertical); // set the slider style to LinearBarVertical
  volSlider.setTextBoxStyle(Slider::NoTextBox, false, 0, 0); // remove text box inside the slider
  volSlider.setPopupDisplayEnabled(true, true, this); // a pop-up showing the current value

  // Text label for volume slider
  volumeLabel.setText("Vol", dontSendNotification);
  volumeLabel.attachToComponent(&volSlider, false); // display above the slider

  // Speed Slider 
  speedSlider.setRange(0.1, 2.0, 0.1); // program crashes when speed slider hits 0. So the range starts from 0.1
  speedSlider.setValue(1.0); // initial value of 1.0
  speedSlider.setSliderStyle(Slider::SliderStyle::LinearVertical); // set the slider style to LinearVertical
  speedSlider.setTextBoxStyle(Slider::NoTextBox, false, 0, 0); // remove text box
  speedSlider.setPopupDisplayEnabled(true, true, this); // a pop-up showing the current value
  speedSlider.setTextValueSuffix("x"); // add a suffix to the pop-up value

  // Text label for speed slider
  speedLabel.setText("Spd", dontSendNotification);
  speedLabel.attachToComponent(&speedSlider, false); // display text above the speed slider

  // Position slider
  posSlider.setRange(0.0, 1.0);
  posSlider.setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0); // no text box

  // set to flip toggle state when button is pressed
  muteButton.setClickingTogglesState(true);
  twoTimesButton.setClickingTogglesState(true);
  loopButton.setClickingTogglesState(true);

  // set interval to every 500 millisecond 
  startTimer(500);
}

DeckGUI::~DeckGUI() {
  stopTimer();
}

/* Drawing of the component */
void DeckGUI::paint(Graphics& g) {
  g.fillAll(Colour{ 0xFF000000 }); // background colour
  g.setColour(Colour{ 0xFFFFFFFF });
  g.drawRect(getLocalBounds(), 1); // draw an outline around the component
  g.setFont(14.0f);

  // set colour of the buttons
  playpauseButton.setLookAndFeel(&LookAndFeel_V1);
  playpauseButton.setColour(ComboBox::outlineColourId, Colour(0xFFFFFFFF)); // button outline colour
  playpauseButton.setColour(TextButton::buttonColourId, Colour(0xFF317A00)); // button background colour
  playpauseButton.setColour(TextButton::textColourOffId, Colour(0xFFFFFFFF)); // button text colour

  resetButton.setLookAndFeel(&LookAndFeel_V3);
  resetButton.setColour(TextButton::buttonColourId, Colour(0xFF1B1B1E));
  resetButton.setColour(TextButton::textColourOffId, Colour(0xFFDE1738));

  loadButton.setLookAndFeel(&LookAndFeel_V3);
  loadButton.setColour(TextButton::buttonColourId, Colour(0xFF1B1B1E));
  loadButton.setColour(TextButton::textColourOffId, Colour(0xFF7F96FF));
  
  loopButton.setLookAndFeel(&LookAndFeel_V3);
  loopButton.setColour(TextButton::buttonColourId, Colour(0xFF1B1B1E));
  loopButton.setColour(TextButton::textColourOffId, Colour(0xFFADA8B6));
  
  skipBackButton.setLookAndFeel(&LookAndFeel_V1);
  skipBackButton.setColour(ComboBox::outlineColourId, Colour(0xFF000000));
  skipBackButton.setColour(TextButton::buttonColourId, Colour(0xFFCDCDCD));
  skipBackButton.setColour(TextButton::textColourOffId, Colour(0xFF000000));
  
  skipFrontButton.setLookAndFeel(&LookAndFeel_V1);
  skipFrontButton.setColour(ComboBox::outlineColourId, Colour(0xFF000000));
  skipFrontButton.setColour(TextButton::buttonColourId, Colour(0xFFCDCDCD));
  skipFrontButton.setColour(TextButton::textColourOffId, Colour(0xFF000000));
  
  twoTimesButton.setLookAndFeel(&LookAndFeel_V1);
  muteButton.setLookAndFeel(&LookAndFeel_V1);

  name.setColour(Label::textColourId, Colour(0xFF000000));
  name.setColour(Label::backgroundColourId, Colour(0xFFFFFFFF));
  name.setFont(18.0f);
  name.setJustificationType(Justification::centred);

  length.setColour(Label::textColourId, Colour(0xFF63372C));
  length.setFont(16.0f);
  length.setJustificationType(Justification::centred);

  // set colour of slider track and thumb 
  getLookAndFeel().setColour(Slider::thumbColourId, Colour(0xFFE3170A));
  getLookAndFeel().setColour(Slider::trackColourId, Colour(0xFFFFFFFF));

  // display track name and length if a track is loaded into Deck
  if (isLoaded) {
    name.setText(TrackName, dontSendNotification);
    length.setText(movingTrackLength + " | " + TrackLength, dontSendNotification);
    length.setColour(Label::backgroundColourId, Colour(0xFFFFFFFF));
  }

  else {
    name.setText("No Track Loaded", dontSendNotification);
  }
}

/* Sets the size of each object in the component */
void DeckGUI::resized() {
  double rowH = getHeight() / 8;
  double colW = getWidth() / 3;

  // change the components position and size (int x, int y, int width, int height)
  waveformdisplay.setBounds(5, 5, getWidth() - 10, rowH * 2 - 10);
  posSlider.setBounds(5, rowH, getWidth() - 10, rowH * 2);

  name.setBounds(getWidth() / 4 - 25, rowH * 2.5, getWidth() / 2 + 50, rowH);
  length.setBounds(getWidth() / 4 - 25, rowH * 3.5, getWidth() / 2 + 50, rowH);

  // << PLAY >> 
  playpauseButton.setBounds(colW + 10, rowH * 4.9, colW - 20, rowH - 2);
  skipBackButton.setBounds(playpauseButton.getX() - (colW / 3) - 3, rowH * 5, colW / 3, rowH - 10);
  skipFrontButton.setBounds(playpauseButton.getRight() + 3, rowH * 5, colW / 3, rowH - 10);

  // DECK 1
  if (isDeck1) {
    volSlider.setBounds(getRight() - (getWidth() / 13) - 10, rowH * 2.9, getWidth() / 13, rowH * 3 + 10);
    speedSlider.setBounds(getY() + 10, rowH * 3, getWidth() / 13, rowH * 3 + 10);

    // [RESET | LOOP | LOAD]
    resetButton.setBounds(10, rowH * 6.9, colW - 60, rowH - 2);
    loopButton.setBounds(resetButton.getRight() + 10, rowH * 6.9, colW - 60, rowH - 2);
    loadButton.setBounds(loopButton.getRight() + 10, rowH * 6.9, colW - 60, rowH - 2);
  }
  // DECK 2
  else {
    volSlider.setBounds(getY() + 10, rowH * 2.9, getWidth() / 13, rowH * 3 + 10);
    speedSlider.setBounds(getX() - (getWidth() / 13) - 10, rowH * 3, getWidth() / 13, rowH * 3 + 10);

    // [ LOAD | LOOP | RESET ]
    resetButton.setBounds(colW * 2 + 50, rowH * 6.9, colW - 60, rowH - 2);
    loopButton.setBounds(resetButton.getX() - resetButton.getWidth() - 10, rowH * 6.9, colW - 60, rowH - 2);
    loadButton.setBounds(loopButton.getX() - loopButton.getWidth() - 10, rowH * 6.9, colW - 60, rowH - 2);
  }

  muteButton.setBounds(volSlider.getX() - 5, volSlider.getBottom() + 3, getWidth() / 10, rowH / 2);
  twoTimesButton.setBounds(speedSlider.getX() - 5, speedSlider.getBottom() - 5, getWidth() / 10, rowH / 2);
}

/* Determine what action to take when a button is clicked */
void DeckGUI::buttonClicked(Button* button) {
  // if the playpause button is clicked  
  if (button == &playpauseButton) {
    // if the play button is clicked, play the track and set the button text to "PAUSE"
    if (!player->isPlaying) {
      DBG("play button pressed");
      player->start();
      playpauseButton.setButtonText("PAUSE");
    }

    // if the "PAUSE" button is clicked, stop the track and set the button text to "PLAY"
    else {
      DBG("pause button pressed");
      player->stop();
      playpauseButton.setButtonText("PLAY");
    }
  }

  // if the reset button is clicked    
  if (button == &resetButton) {
    DBG("reset button pressed");

    // if track was playing when reset button was clicked,
    // immediately play the track after resetting the track
    if (player->isPlaying) {
      player->setPosition(0);
      player->start();
    }

    // if track was not playing when reset button was clicked,
    // keep the track paused after resetting
    else {
      player->setPosition(0);
      player->stop();
    }
  }

  // if the load button is clicked
  if (button == &loadButton) {
    DBG("load button pressed");
    FileChooser chooser{ "Select a file to play..." };

    if (chooser.browseForFileToOpen()) {
      player->loadURL(URL{ chooser.getResult() });
      waveformdisplay.loadURL(URL{ chooser.getResult() });
      setNameAndLength(File{ chooser.getResult() });
    }
  }

  // if the mute button is clicked
  if (button == &muteButton) {
    DBG("mute button pressed");

    // if mute button is 'On' 
    if (muteButton.getToggleState()) {
      player->setGain(0); // set volume to 0
      muteButton.setButtonText("unmute");
    }

    else {
      muteButton.setButtonText("mute");
      player->setGain(volSlider.getValue());
    }
  }

  // if the 2x button is clicked
  if (button == &twoTimesButton) {
    DBG("2x button pressed");
    // set speed to 2 if the twotimes button is 'On'
    if (twoTimesButton.getToggleState()) {
      player->setSpeed(2);
      twoTimesButton.setButtonText(speedSlider.getTextFromValue(speedSlider.getValue())); // set text to the speed slider value
    }

    // set twoTimesButton text back to "2.0x"
    else {
      twoTimesButton.setButtonText("2.0x");
      player->setSpeed(speedSlider.getValue());
    }
  }

  // if the loop button is clicked
  if (button == &loopButton) {
    DBG("loopButton pressed");

    // if loop button is 'On'
    if (loopButton.getToggleState()) {
      isLooping = true;
      loopButton.setButtonText("STOP LOOP");
    }

    else {
      isLooping = false;
      loopButton.setButtonText("LOOP");
    }
  }

  // if >> button is clicked
  if (button == &skipFrontButton) {
    DBG("skipFrontButton pressed");

    // set new position to +5 secs
    player->setPosition(player->getPosition() + 5.0);
  }

  // if << button is clicked
  if (button == &skipBackButton) {
    DBG("skipBackButton pressed");

    // get new position (-5 secs of current pos)
    double pos{ player->getPosition() - 5.0 };

    // set new position if it is not lesser than 0
    if (!pos <= 0) {
      player->setPosition(pos);
    }
  }
}

/* Determine what action to take when the value of a slider is changed */
void DeckGUI::sliderValueChanged(Slider* slider) {
  // if slider moved and mute button is 'off'
  if ((slider == &volSlider) && (muteButton.getToggleState() == false)) {
    player->setGain(slider->getValue());
  }

  // if slider moved and 2x button is 'off'
  if ((slider == &speedSlider) && (twoTimesButton.getToggleState() == false)) {
    player->setSpeed(slider->getValue());
  }

  if (slider == &posSlider) {
    player->setPositionRelative(slider->getValue());
  }
}

/* Determines whether the component is interested in the set of files being dragged in */
bool DeckGUI::isInterestedInFileDrag(const StringArray& /*files*/) {
  return true;
}

/* Processing of the files dropped onto this component */
void DeckGUI::filesDropped(const StringArray& files, int /*x*/, int /*y*/) {
  // only drop 1 file at a time
  if (files.size() == 1) {
    player->loadURL(URL{ File{files[0]} });
    waveformdisplay.loadURL(URL{ File{files[0]} });
    
    setNameAndLength(files[0]);
  }
}

/* Callback routine that gets called according to the interval set */
void DeckGUI::timerCallback() {
  // update waveformdisplay, posSlider, and movingTrackLength
  waveformdisplay.setPositionRelative(player->getPositionRelative());
  posSlider.setValue(player->getPositionRelative());
  movingTrackLength = lengthInString(player->getPosition());

  // replay the track if the loop button is 'on' when the track reaches the end
  if (isLooping) {
    if (player->getPosition() >= player->getLengthInSeconds()) {
      player->setPosition(0);
      player->start();
    }
  }

  else {
    // play songs from the queue when the player reaches end of track
    if (player->getPosition() >= player->getLengthInSeconds()) {
      if (queueComponent->playQueueButton.getToggleState() && queueComponent->queuedTracks.size() > 0) {

        // the first item in queuedTracks vector
        File file = queueComponent->queuedTracks[0].file;
        player->loadURL(URL{ file });
        waveformdisplay.loadURL(URL{ file });
        setNameAndLength(file);

        player->setPosition(0);
        player->start();
        playpauseButton.setButtonText("PAUSE");

        // erase the first item in the vector (queuedTracks[0])
        queueComponent->queuedTracks.erase(queueComponent->queuedTracks.begin());
        queueComponent->queueTable.updateContent();
      }
    }
  }
}

/* Check whether the component is interested in the type of object being dragged in */
bool DeckGUI::isInterestedInDragSource(const SourceDetails& /*dragSourceDetails*/) {
  return true;
}

/* Processes the item dropped in */
void DeckGUI::itemDropped(const SourceDetails& dragSourceDetails) {
  URL trackURL = URL{ dragSourceDetails.description.toString() };

  player->loadURL(trackURL);
  waveformdisplay.loadURL(trackURL);

  setNameAndLength(File{ trackURL.getLocalFile() });
}

/* Gets the name and length of the file passed in to display in the deck */
void DeckGUI::setNameAndLength(File file) {
  TrackName = (File{ file }.getFileNameWithoutExtension());
  TrackLength = lengthInString(player->getLengthInSeconds());
  isLoaded = true;
  repaint();
}

/* Formats the time passed in into minutes:seconds format
   used in DeckGUI::setNameAndLength() to display track length */
String DeckGUI::lengthInString(double time) {
  int totalTime = (int)time;

  String mins{ totalTime / 60 };
  String secs{ totalTime % 60 };

  // if mins is 1 digit, add a leading zero
  if (mins.length() == 1)
    mins = "0" + mins;

  // if secs is 1 digit, add a leading zero
  if (secs.length() == 1)
    secs = "0" + secs;

  return mins + ':' + secs;;
}
