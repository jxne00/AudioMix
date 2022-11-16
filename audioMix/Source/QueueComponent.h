/*
  ==============================================================================

    QueueComponent.h
    Created: 8 Aug 2022 10:07:04pm
    Author:  pangj

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "TrackInfo.h"

//==============================================================================
/*
*/
class QueueComponent  : public Component,
                        public TableListBoxModel,
                        public Button::Listener
{
public:
    /**
     * \brief 
     *     Constructor.
     */
    QueueComponent();

    /**
     * \brief 
     *     Destructor.
     */
    ~QueueComponent() override;

    /**
     * \brief
     *    Drawing of the component.
     *
     * \param g
     *    Graphic reference used to do carry out drawing operations
     */
    void paint (juce::Graphics&) override;

    /**
     * \brief
     *    Sets the size of each object in the component.
     */
    void resized() override;

    /**
     * \brief
     *    Counts the number of items in the TableListBox.
     *
     * \return int
     *    The number of items in the TableListBox
     */
    int getNumRows() override;

    /**
     * \brief
     *    Set the background color of the TableListBox.
     *
     * \param rowNumber
     *    Row number of each row
     * \param width
     *    Unused parameter
     * \param height
     *    Unused parameter
     * \param rowIsSelected
     *    States whether the row is selected. true if selected, false otherwise
     */
    void paintRowBackground(Graphics& g, 
                            int rowNumber,
                            int width,
                            int height,
                            bool rowIsSelected) override;

    /**
     * \brief
     *    Draws the infomation inside each cell.
     *
     * \param g
     *    Graphic reference used to do carry out drawing operations
     * \param rowNumber
     *    The row number of each row
     * \param columnId
     *    ID of each column
     * \param width
     *    Width of the cell
     * \param height
     *    Height of the cell
     * \param rowIsSelected
     *    States whether the row is selected. true if selected, false otherwise
     */
    void paintCell(Graphics& g, 
                   int rowNumber,
                   int columnId,
                   int width,
                   int height,
                   bool rowIsSelected) override;

    /**
     * \brief
     *    Create or ppdates a cutom component that is added into a cell.
     *
     * \param rowNumber
     *    Row number of component
     * \param columnId
     *    Column ID of component
     * \param isRowSelected
     *    States if a row is selected
     * \param existingComponentToUpdate
     *    A pointer to the component
     *
     * \return
     *    Pointer to the Component created or updated
     */
    Component* refreshComponentForCell(int rowNumber,
                                       int columnId,
                                       bool isRowSelected,
                                       Component* existingComponentToUpdate) override;

    /**
     * \brief
     *    Determine what action to take when a button is clicked.
     *
     * \param button
     *    A pointer associated with the button clicked
     */
    void buttonClicked(Button* button) override;

private:
  // TableListBox displaying songs added to queue
  TableListBox queueTable;

  // vector storing information of queued tracks
  std::vector<TrackInfo> queuedTracks;

  // button to clear the queue
  TextButton clearQueue{ "Clear Queue" };
  
  // button to play queued songs in deck
  TextButton playQueueButton{ "Play Queue" };

  // PlaylistComponent and DeckGUI added as friend class 
  // so that they can access private members of QueueComponent
  friend class PlaylistComponent;
  friend class DeckGUI;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (QueueComponent)
};
