/*
  ==============================================================================

    PlaylistComponent.cpp
    Created: 19 Jul 2022 8:16:39pm
    Author:  pangj

  ==============================================================================
*/

#include <JuceHeader.h>
#include "PlaylistComponent.h"

//==============================================================================
PlaylistComponent::PlaylistComponent(DeckGUI* _deckGUI1,
                                     DeckGUI* _deckGUI2, 
                                     QueueComponent* _queue)
                                   : deckGUI1(_deckGUI1),
                                     deckGUI2(_deckGUI2),
                                     queueComponent(_queue)
{
  // load image from image file
  Image loadIcon = ImageCache::getFromMemory(BinaryData::load_icon_png, BinaryData::load_icon_pngSize);
  Image queueIcon = ImageCache::getFromMemory(BinaryData::addToQueue_Logo_png, BinaryData::addToQueue_Logo_pngSize);

  // set drawing of image in different states
  loadToLibrary.setImages(true, true, true,
                          loadIcon, 1.0f, Colours::transparentBlack,  // normal 
                          loadIcon, 1.0f, Colour(0xFFBBBBBF),         // on mouse over
                          loadIcon, 0.5f, Colours::transparentBlack); // when clicked

  addToQueue.setImages(true, true, true,
                       queueIcon, 1.0f, Colours::transparentBlack,
                       queueIcon, 1.0f, Colour(0xFFBBBBBF),
                       queueIcon, 0.5f, Colours::transparentBlack);

  // set the header of the TableListBox
  tableComponent.getHeader().addColumn("Track Title", 1, 250);
  tableComponent.getHeader().addColumn("Length", 2, 150);
  tableComponent.getHeader().addColumn("PLAY IN", 3, 75);
  tableComponent.getHeader().addColumn("PLAY IN", 4, 75);
  tableComponent.getHeader().addColumn("", 5, 40);

  tableComponent.setModel(this);

  // make visible
  addAndMakeVisible(tableComponent);
  addAndMakeVisible(loadToLibrary);
  addAndMakeVisible(addToQueue);
  addAndMakeVisible(removeAll);
  addAndMakeVisible(searchBox);

  // add listeners
  loadToLibrary.addListener(this);
  addToQueue.addListener(this);
  removeAll.addListener(this);
  searchBox.addListener(this);

  searchBox.setTextToShowWhenEmpty("Enter Search Keyword", Colour(0xffffffff));
  searchBox.setJustification(Justification::centred);

  // lambda expression for calling search function on every text change in the search box
  searchBox.onTextChange = [this] {
    search(searchBox.getText());
  };

  // call function to restore library
  addSavedLibrary();
}

PlaylistComponent::~PlaylistComponent() {
  // save tracks that are currently in the library whenever desctructor is called
  saveLibrary();
}

/* Drawing of component */
void PlaylistComponent::paint(juce::Graphics& g) {

  g.setColour(Colour(0xffA8F9FF)); // table outline colour
  g.drawRect(getLocalBounds(), 1); // draw an outline around the component

  g.setColour(Colours::white);
  g.setFont(15.0f);

  // set the colour of the tableComponent and buttons 
  tableComponent.setColour(ListBox::backgroundColourId, Colour(0xff000000));
  tableComponent.setLookAndFeel(&LookAndFeel_V3);

  removeAll.setColour(TextButton::buttonColourId, Colour(0xff6E0D25));
  searchBox.setColour(TextEditor::outlineColourId, Colour(0xffffffff));

  // display number of tracks added
  if (searchBox.isEmpty()) {
    g.drawText(String{ trackInfo.size() } + " TRACKS ADDED",
               searchBox.getRight() + 20, 5, (getWidth() / 3) - 50, getHeight() / 10,
               Justification::centredLeft,
               true);
  }
  else { // display number of search results
    g.drawText(String{ searchResult.size() } + " RESULTS FOUND",
               searchBox.getRight() + 20, 5, (getWidth() / 3) - 50, getHeight() / 10,
               Justification::centredLeft,
               true);
  }

}

/* Sets the size of each object in the component */
void PlaylistComponent::resized() {
  double rowH = getHeight() / 10;
  double colW = getWidth() / 3;

  searchBox.setBounds(5, 5, colW, rowH);
  removeAll.setBounds(colW * 1.9, 5, colW / 2, rowH);
  addToQueue.setBounds(colW * 2.5, 5, colW / 4, rowH);
  loadToLibrary.setBounds(addToQueue.getRight(), 5, colW / 4, rowH);

  tableComponent.setBounds(5, 40, getWidth() - 10, getHeight() - rowH);
}

/* Counts the number of items in the TableListBox */
int PlaylistComponent::getNumRows() { 
  repaint();
  // return size of trackInfo vector if empty, searchResult vector otherwise
  return searchBox.isEmpty() ? trackInfo.size() : searchResult.size();
}

/* Set the background color of the TableListBox */
void PlaylistComponent::paintRowBackground(Graphics& g, int rowNumber, int /*width*/, int /*height*/, bool rowIsSelected) {
  if (rowIsSelected) {
    g.fillAll(Colours::indianred);
  }

  // paint different colour for odd and even rows
  else {
    (rowNumber % 2 == 0) ? g.fillAll(Colour(0xFF000000)) : g.fillAll(Colour(0xFF191919));
  }
}

/* Draws the infomation inside each cell */
void PlaylistComponent::paintCell(Graphics& g, int rowNumber, int columnId, int width, int height, bool rowIsSelected) {
  g.setColour(Colour(0xFFFFFFFF)); // set colour of text when drawn
  
  if (rowNumber < getNumRows()) {
    if (columnId == 1) { // display track titles
      if (searchBox.isEmpty()) {
         g.drawText(trackInfo[rowNumber].trackTitle, 2, 0, width - 4, height, Justification::centredLeft, true);
      }
      else { // display seach results if search box is not empty
        g.drawText(searchResult[rowNumber].trackTitle, 2, 0, width - 4, height, Justification::centredLeft, true);
      } 
    }

    if (columnId == 2) { // display track lengths
      if (searchBox.isEmpty()) {
        g.drawText(trackInfo[rowNumber].trackLength, 2, 0, width - 4, height, Justification::centredLeft, true);
      }
      else { // display seach results if search box is not empty
        g.drawText(searchResult[rowNumber].trackLength, 2, 0, width - 4, height, Justification::centredLeft, true);
      }
    }
  }
}

/* Create or updates a cutom component that is added into a cell */
Component* PlaylistComponent::refreshComponentForCell(int rowNumber,
                                                      int columnId,
                                                      bool isRowSelected,
                                                      Component* existingComponentToUpdate)
{
  if (columnId == 3) {
    if (existingComponentToUpdate == nullptr) {
      TextButton* btn = new TextButton{ "DECK 1" };
      String id{ std::to_string(rowNumber) };
      // set a unique id onto the button
      btn->setComponentID(id);
      btn->addListener(this);
      existingComponentToUpdate = btn;

      btn->setColour(TextButton::buttonColourId, Colour(0xFF989FCE));
    }
  }

  if (columnId == 4) {
    if (existingComponentToUpdate == nullptr) {
      TextButton* btn = new TextButton{ "DECK 2" };
      String id{ std::to_string(rowNumber) };

      btn->setComponentID(id);
      btn->addListener(this);
      existingComponentToUpdate = btn;

      btn->setColour(TextButton::buttonColourId, Colour(0xFFF4D1AE));
    }
  }

  if (columnId == 5) {
    if (existingComponentToUpdate == nullptr) {
      TextButton* btn = new TextButton{ "X" };
      String id{ std::to_string(rowNumber) };

      btn->setComponentID(id);
      btn->addListener(this);
      existingComponentToUpdate = btn;

      btn->setColour(TextButton::buttonColourId, Colour(0xFF8F0002));
    }
  }

  return existingComponentToUpdate;
}

/* Determine what action to take when a button is clicked */
void PlaylistComponent::buttonClicked(Button* button) {
  if (button == &loadToLibrary) {
    DBG("PlaylistComponent::buttonClicked loadToPlaylist button clicked");
    addToLibrary();
  }

  else if (button == &removeAll) {
    DBG("PlaylistComponent::buttonClicked removeAll button clicked");
    removeAllTracks();
  }

  else if (button == &addToQueue) {
    DBG("PlaylistComponent::buttonClicked addToQueue button clicked");
    
    // if at least 1 row is selected
    if (tableComponent.getNumSelectedRows() > 0) {
      int selectedRow = tableComponent.getSelectedRow(); // get id of selected row

      // add the file of the selected row into queuedTracks vector
      queueComponent->queuedTracks.push_back(trackInfo[selectedRow].file);
      queueComponent->queueTable.updateContent();
    }
  }

  else {
    // id of the row where button clicked 
    int id = stoi(button->getComponentID().toStdString());

    // load track into Deck 1 player if "DECK 1" button is pressed
    if (button->getButtonText() == "DECK 1") {
      DBG("PlaylistComponent::buttonClicked DECK 1 button clicked");

      if (searchBox.isEmpty()) {
        deckGUI1->player->loadURL(URL{ trackInfo[id].file });
        deckGUI1->waveformdisplay.loadURL(URL{ trackInfo[id].file });
        deckGUI1->waveformdisplay.setPositionRelative(deckGUI1->player->getPositionRelative());

        deckGUI1->setNameAndLength(File{ URL(URL{ trackInfo[id].file }).getLocalFile() });
      }
      else {
        deckGUI1->player->loadURL(URL{ searchResult[id].file });
        deckGUI1->waveformdisplay.loadURL(URL{ searchResult[id].file });
        deckGUI1->waveformdisplay.setPositionRelative(deckGUI1->player->getPositionRelative());

        deckGUI1->setNameAndLength(File{ URL(URL{ searchResult[id].file }).getLocalFile() });
      }

    }

    // load track into Deck 2 player if "DECK 2" button is pressed
    if (button->getButtonText() == "DECK 2") {
      DBG("PlaylistComponent::buttonClicked DECK 2 button clicked");

      if (searchBox.isEmpty()) {
        deckGUI2->player->loadURL(URL{ trackInfo[id].file });
        deckGUI2->waveformdisplay.loadURL(URL{ trackInfo[id].file });
        deckGUI2->waveformdisplay.setPositionRelative(deckGUI2->player->getPositionRelative());

        deckGUI2->setNameAndLength(File{ URL{ trackInfo[id].file }.getLocalFile() });
      }
      else {
        deckGUI2->player->loadURL(URL{ searchResult[id].file });
        deckGUI2->waveformdisplay.loadURL(URL{ searchResult[id].file });
        deckGUI2->waveformdisplay.setPositionRelative(deckGUI2->player->getPositionRelative());

        deckGUI2->setNameAndLength(File{ URL{ searchResult[id].file }.getLocalFile() });
      }
    }

    // remove the selected track from library if the "X" button is pressed
    if (button->getButtonText() == "X") {
      if (!trackInfo.empty())
        removeTrack(id);
    }
  }
}

/*  Allows adding of multiple files into the library 
    (called when "Add to Playlist" button is clicked) */
void PlaylistComponent::addToLibrary() {
  FileChooser chooser{ "Select file(s) to add into library..." };

  // able to select one or multiple files
  if (chooser.browseForMultipleFilesToOpen()) {
    for (File& file : chooser.getResults()) {
      
      bool exists = false; // true = track exists, false = track does not exist

      // compare name of chosen file with the names stored in trackTitle to determine if it already exists
      if (!trackInfo.empty()) {
        for (TrackInfo& track : trackInfo) {
          if (track.file == file) 
            exists = true;
        }
      }

      // add file into trackInfo if the file does not already exist
      if (!exists) {
        trackInfo.push_back(file);
      }

      // display message box if track name already exists in library
      else {
        AlertWindow::showMessageBox(AlertWindow::AlertIconType::InfoIcon,
                                    "Warning:", // title
                                    "The track '" + file.getFileNameWithoutExtension() + "' has already been added.", // message
                                    "OK", // button text
                                    nullptr);
      }

      // refresh the list to update it with the newly added row
      tableComponent.updateContent();
    }
  }
}

/* Determines whether the component is interested in the set of files being dragged in */
bool PlaylistComponent::isInterestedInFileDrag(const StringArray&) {
  return true;
}

/* Processing of the files dropped onto this component */
void PlaylistComponent::filesDropped(const StringArray& files, int, int) {
  // one or more files droppped
  for (String file : files) {
    File droppedFile{ file };

    // track does not exist = false, already exists = true
    bool exists = false;

    // compare name of chosen file with the names stored in trackTitle to determine if it already exists
    if (!trackInfo.empty()) {
      for (TrackInfo& line : trackInfo) {
        if (line.file == droppedFile)
          exists = true;
      }
    }

    // add track name and length into respective vectors if track does not already exist
    if (!exists) {
      trackInfo.push_back(droppedFile);
    }

    // diisplay message box if track name already exists in library
    else {
      AlertWindow::showMessageBox(AlertWindow::AlertIconType::InfoIcon,
                                  "Warning!", // title
                                  "The track '" + droppedFile.getFileNameWithoutExtension() + "' has already been added.", // message
                                  "OK", // button text
                                  nullptr);
    }
  }

  // refresh the list to update with the new rows
  tableComponent.updateContent();
}

/* Remove the track that matches the id passed in */
void PlaylistComponent::removeTrack(int id) {
  // pop_back if it is the last item in the vector
  // otherwise, remove item from the vector at index of [id]
  if (searchBox.isEmpty()) {
    id == trackInfo.size() ? trackInfo.pop_back() : trackInfo.erase(trackInfo.begin() + id);
  }

  // if searchbox is not empty, remove the track from both searchResult and trackInfo vector
  else {
    auto i = 0;
    for (TrackInfo& title : trackInfo) {
      if (title.trackTitle == searchResult[id].trackTitle) {
        i == trackInfo.size() ? trackInfo.pop_back() : trackInfo.erase(trackInfo.begin() + i);
      } 
      i++;
    }
    id == searchResult.size() ? searchResult.pop_back() : searchResult.erase(searchResult.begin() + id);
  }

  tableComponent.updateContent();
}

/* Remove all tracks from the library */
void PlaylistComponent::removeAllTracks() {
  // if the search box is empty
  if (searchBox.isEmpty()) {
    // clear the trackInfo vector to remove all items
    if (!trackInfo.empty()) {
      trackInfo.clear();
      tableComponent.updateContent();
    }
  }
  else {
    // if library is displaying search results
    if (!searchResult.empty()) {
      for (TrackInfo& result : searchResult) {
        // delete all the tracks in searchResult from the trackInfo vector
        for (int i = 0; i < trackInfo.size(); i++) {
          if (result.trackTitle == trackInfo[i].trackTitle) {
            i == trackInfo.size() ? trackInfo.pop_back() : trackInfo.erase(trackInfo.begin() + i);
          }
        }
      }
      searchResult.clear();
      tableComponent.updateContent();
    }
  }
}

/* Search for tracks within the library */
void PlaylistComponent::search(String keyword) {
  // clear vector before adding new search results
  searchResult.clear();

  // compare keyword to track titles of all added tracks to find matches
  if (keyword.isNotEmpty()) {
    for (TrackInfo& track : trackInfo) {
      if (String{ track.trackTitle }.containsIgnoreCase(keyword)) {
        searchResult.push_back(track.file);
      }
    }
  }
  tableComponent.updateContent();
}

/* Persist the library by storing the file path in a .txt file */
void PlaylistComponent::saveLibrary() {
  // create a .txt file to store information of tracks added into library 
  std::ofstream playlist("library.txt");

  // store the full file path into the .txt file
  for (TrackInfo& track : trackInfo) {
    playlist << track.file.getFullPathName() << '\n';
  }
}

/* Loads track by reading from the .txt file created in saveLibrary() function */
void PlaylistComponent::addSavedLibrary() {
  // open the .txt file containing saved tracks
  std::ifstream library{ "library.txt" };

  if (library.is_open()) {
    std::string line;

    while (getline(library, line, '\n')) {
      File file { line };
      trackInfo.push_back(file);
    }
  }

  tableComponent.updateContent();
  // close the .txt file
  library.close();
}

/* To allow any track added in the library to be dragged and dropped into deckGUI1 or deckGUI2 */
var PlaylistComponent::getDragSourceDescription(const SparseSet<int>& selectedRow) {
  String details;

  // return the URL of the row that is dragged (in String)
  // use trackInfo vector if searchBox is empty and searchResult vector otherwise
  URL row = searchBox.isEmpty() ? URL{ trackInfo[selectedRow[0]].file } : URL{ searchResult[selectedRow[0]].file };
  details << row.toString(false) << " ";

  return details;
}
