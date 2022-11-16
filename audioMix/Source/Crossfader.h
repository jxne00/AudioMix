/*
  ==============================================================================

    Crossfader.h
    Created: 1 Aug 2022 12:03:27pm
    Author:  pangj

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "DeckGUI.h"

//==============================================================================
/*
*/
class Crossfader  : public Component, 
                    public Slider::Listener
{
public:
    /**
     * \brief 
     *     Constructor.
     */
    Crossfader(DeckGUI* _deckGUI1, DeckGUI* _deckGUI2);

    /**
     * \brief 
     *     Destructor.
     */
    ~Crossfader() override;

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
     *    Sets the size of objects in the component.
     */
    void resized() override;

    /**
     * \brief
     *    Determine what action to take when the value of a slider is changed.
     *
     * \param slider
     *    A pointer associated with the slider moved
     */
    void sliderValueChanged(Slider* slider) override;

private:
    // corssfade slider for varying the volume between deck 1 and 2
    Slider crossfadeSlider;

    // instances of DECKGUI class
    DeckGUI* deckGUI1;
    DeckGUI* deckGUI2;

    // for using LookAndFeel_V1
    LookAndFeel_V1 v1;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Crossfader)
};
