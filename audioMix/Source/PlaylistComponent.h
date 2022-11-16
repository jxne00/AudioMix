/*
  ==============================================================================

    PlaylistComponent.h
    Created: 19 Jul 2022 8:16:39pm
    Author:  pangj

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include <vector>
#include <string>
#include <fstream>
#include "TrackInfo.h"
#include "DeckGUI.h"
#include "QueueComponent.h"


//==============================================================================
/*
*/
class PlaylistComponent : public Component,
                          public TableListBoxModel,
                          public Button::Listener,
                          public FileDragAndDropTarget,
                          public TextEditor::Listener,
                          public DragAndDropContainer
{
public:
  /**
   * \brief
   *    Constructor.
   */
  PlaylistComponent(DeckGUI* _deckGUI1, DeckGUI* _deckGUI2, QueueComponent* _queue);

  /**
   * \brief
   *    Destructor.
   */
  ~PlaylistComponent() override;

  /**
   * \brief
   *    Drawing of the component.
   *
   * \param g
   *    Graphic reference used to do carry out drawing operations
   */
  void paint(juce::Graphics& g) override;

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

  /**
   * \brief
   *    Allows adding of multiple files into the library through the FileChooser.
   *    This function is called when the "Add to Playlist" button is clicked.
   */
  void addToLibrary();

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
  bool isInterestedInFileDrag(const StringArray&) override;

  /**
   * \brief
   *    Processing of the files dropped onto this component.
   *
   * \param files
   *    The set of (absolute) pathnames of the files that is dragged
   */
  void filesDropped(const StringArray& files, int, int) override;

  /**
   * \brief
   *    Remove the track that matches the id passed in.
   *
   * \param id
   *    The ID of the row of track selected
   */
  void removeTrack(int id);

  /**
   * \brief
   *    Remove all tracks from the library.
   */
  void removeAllTracks();

  /**
   * \brief
   *    Search for tracks within the library.
   *    This function is called whenever there is a text change in the search box.
   *
   * \param keyword
   *    The keyword entered into the search box.
   */
  void search(String keyword);

  /**
   * \brief
   *    Persist the library by storing storing the file path of all tracks added into the library in a .txt file.
   *    This function is called in the destructor.
   */
  void saveLibrary();

  /**
   * \brief
   *    Loads all track infomation by reading from the .txt file created in saveLibrary() function.
   *    This function is called in the constructor.
   */
  void addSavedLibrary();

  /**
   * \brief
   *    To allow any track added in the library to be dragged and dropped into deckGUI1 or deckGUI2.
   *
   * \param selectedRow
   *    Index of the row selected
   *
   * \return var
   *    The URL of the row of track dragged
   */
  var getDragSourceDescription(const SparseSet<int>& selectedRow);


private:

  // TableListBox displaying track information
  TableListBox tableComponent;

  // vector for storing and retrieving track information
  std::vector<TrackInfo> trackInfo;

  // vector for storing tracks matching search keyword
  std::vector<TrackInfo> searchResult;

  // button to load tracks into the playlist component
  ImageButton loadToLibrary;

  // button to add selected row to queue
  ImageButton addToQueue;

  // button to remove all tracks from the library
  TextButton removeAll{ "Remove All" };

  // to load tracks into deckGUI1 & deckGUI2
  DeckGUI* deckGUI1;
  DeckGUI* deckGUI2;

  // search box to search for a track
  TextEditor searchBox;

  LookAndFeel_V3 LookAndFeel_V3;
 
  QueueComponent* queueComponent;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PlaylistComponent)
};
