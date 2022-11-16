/*
  ==============================================================================

    DeckGUI.h
    Created: 17 Jul 2022 10:52:03am
    Author:  pangj

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include <iterator>
#include "DJAudioPlayer.h"
#include "WaveformDisplay.h"
#include "QueueComponent.h"

//==============================================================================
/*
*/
class DeckGUI : public juce::Component,
                public Button::Listener,
                public Slider::Listener,
                public FileDragAndDropTarget,
                public Timer,
                public DragAndDropTarget
{
public:
  /**
   * \brief 
   *     Constructor.
   */
  DeckGUI(DJAudioPlayer* player,
          AudioFormatManager& formatManagerToUse,
          AudioThumbnailCache& cacheToUse, QueueComponent* _queueComponent, bool differentiate);

  /**
   * \brief 
   *     Destructor.
   */
  ~DeckGUI();

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
   *    Sets the size of each object in the component.
   */
  void resized() override;

  /**
   * \brief
   *    Determine what action to take when a button is clicked.
   *
   * \param button
   *    A pointer associated with the button clicked
   */
  void buttonClicked(Button*) override;

  /**
   * \brief
   *    Determine what action to take when the value of a slider is changed.
   *
   * \param slider
   *    A pointer associated with the slider moved
   */
  void sliderValueChanged(Slider* slider) override;

  /**
   * \brief
   *    Determines whether the component is interested in the set of files being dragged in.
   *
   * \param files
   *    The set of (absolute) pathnames of the files that is dragged
   *
   * \return
   *    Returns true
   */
  bool isInterestedInFileDrag(const StringArray& files) override;

  /**
   * \brief
   *    Processing of the files dropped onto this component.
   *
   * \param files
   *    The set of (absolute) pathnames of the files that is dragged
   */
  void filesDropped(const StringArray& files, int x, int y) override;

  /**
   * \brief 
   *     Callback routine that gets called according to the interval set.
   */
  void timerCallback() override;

  /**
   * \brief
   *    Check whether the component is interested in the type of object being dragged in.
   *
   * \param dragSourceDetails
   *    Contains details about the item dragged in (passed in from PlaylistComponent::getDragSourceDescription)
   *
   * \return
   *    Returns true
   */
  bool isInterestedInDragSource(const SourceDetails& dragSourceDetails);

  /**
   * \brief
   *    Processes the item dropped in.
   *
   * \param dragSourceDetails
   *    Contains details about the item dragged in (passed in from PlaylistComponent::getDragSourceDescription)
   */
  void itemDropped(const SourceDetails& dragSourceDetails);

  /**
   * \brief 
   *     Gets the name and length of the file passed in.
   * 
   * \param file
   *     The file that is to be used for getting the name and length
   */
  void setNameAndLength(File file);

  /**
   * \brief
   *    Formats the time into minutes:seconds format.
   *
   * \param time
   *    The time in datatype double that is to be formatted
   */
  String lengthInString(double time);

private:
  // Buttons
  TextButton playpauseButton{ "PLAY" };
  TextButton resetButton{ "RESET" };
  TextButton loadButton{ "LOAD" };
  TextButton twoTimesButton{ "2.0x" };
  TextButton muteButton{ "mute" };
  TextButton skipFrontButton{ ">>" };
  TextButton skipBackButton{ "<<" };
  TextButton loopButton{ "LOOP" };

  // Sliders
  Slider volSlider;
  Slider speedSlider;
  Slider posSlider;
  Slider balanceSlider;

  // Labels
  Label volumeLabel;
  Label speedLabel;

  // Stores track infomation to be displayed
  String TrackName;
  String TrackLength;
  Label name;
  Label length;

  // stores the current play position of the track
  String movingTrackLength;
  Label movingLength;

  FileChooser fChooser{ "Select a file" };

  DJAudioPlayer* player;
  WaveformDisplay waveformdisplay;
  QueueComponent* queueComponent;

  // determines whether a track is loaded into the Deck
  bool isLoaded;

  // determines whether the loop button is 'on' or 'off'
  bool isLooping;

  // PlaylistComponent and Crossfader declared as a friend class 
  // so that DeckGUI's private members can be accessed.
  friend class PlaylistComponent;
  friend class Crossfader;

  // for using different versions of LookAndFeel
  LookAndFeel_V1 LookAndFeel_V1;
  LookAndFeel_V3 LookAndFeel_V3;

  // for differentiating between deck 1 and 2
  // true = deck 1, false = deck 2
  bool isDeck1;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DeckGUI)
};

