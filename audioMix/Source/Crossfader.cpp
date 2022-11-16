/*
  ==============================================================================

    Crossfader.cpp
    Created: 1 Aug 2022 12:03:27pm
    Author:  pangj

  ==============================================================================
*/

#include <JuceHeader.h>
#include "Crossfader.h"

//==============================================================================
Crossfader::Crossfader(DeckGUI* _deckGUI1, 
                       DeckGUI* _deckGUI2) 
                     : deckGUI1(_deckGUI1), 
                       deckGUI2(_deckGUI2)
{
  addAndMakeVisible(crossfadeSlider);
  crossfadeSlider.addListener(this);

  crossfadeSlider.setRange(0.0, 1.0, 0.1); // range from 0 to 1
  crossfadeSlider.setValue(0.5); // initial value of 0.5
  crossfadeSlider.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
  crossfadeSlider.setPopupDisplayEnabled(true, true, this); // pop-up to show the current value
}

Crossfader::~Crossfader() {
}

/* Drawing of the component */
void Crossfader::paint (Graphics& g) {
  // set colour of slider track and thumb 
  v1.setColour(Slider::thumbColourId, Colour(0xFFE3170A));
  v1.setColour(Slider::trackColourId, Colour(0xFF000000));
  
  crossfadeSlider.setLookAndFeel(&v1);

  g.setColour(Colour(0xFF000000));
  g.fillAll(Colour(0xFFFFFFFF));
  g.drawRect(getLocalBounds(), 1.0f);
}

/* Sets the size of objects in the component */
void Crossfader::resized() {
  crossfadeSlider.setBounds(0, 0, getWidth(), getHeight());
}

/* Determine what action to take when the value of a slider is changed */
void Crossfader::sliderValueChanged(Slider* slider) {
  if (slider == &crossfadeSlider) {
    // 0.5 is the neutral value between the 2 players
    // if crossfadeSlider value is lesser than 0.5, deckGUI1 should get louder
    if (slider->getValue() < 0.5) {
      deckGUI1->player->setGain(1 - slider->getValue());
      deckGUI2->player->setGain(slider->getValue());
    }

    // if crossfadeSlider value is more than 0.5, deckGUI2 should get louder
    else if (slider->getValue() > 0.5) {
      deckGUI2->player->setGain(slider->getValue());
      deckGUI1->player->setGain(1 - slider->getValue());
    }
  }
}
