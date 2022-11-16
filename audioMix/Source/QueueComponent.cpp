/*
  ==============================================================================

    QueueComponent.cpp
    Created: 8 Aug 2022 10:07:04pm
    Author:  pangj

  ==============================================================================
*/

#include <JuceHeader.h>
#include "QueueComponent.h"

//==============================================================================
QueueComponent::QueueComponent() {

  // table headers
  queueTable.getHeader().addColumn("Queued Tracks", 1, 170);
  queueTable.getHeader().addColumn("", 2, 30);

  queueTable.setModel(this);

  addAndMakeVisible(queueTable);
  addAndMakeVisible(clearQueue);
  addAndMakeVisible(playQueueButton);

  clearQueue.addListener(this);
  playQueueButton.addListener(this);

  // change toggle state whenever button is clicked
  playQueueButton.setClickingTogglesState(true);
  
  // set colour when toggle is 'On'
  playQueueButton.setColour(TextButton::buttonOnColourId, Colour(0xFF63768D));
}

QueueComponent::~QueueComponent() {
}

/* Drawing of component */
void QueueComponent::paint (Graphics& g) {
    g.fillAll (Colour(0xFF000000)); // fill background
    queueTable.setColour(ListBox::backgroundColourId, Colour(0xFF000000)); // set table background colour
    clearQueue.setColour(TextButton::buttonColourId, Colour(0xFF6E0D25)); // set button background colour
    playQueueButton.setColour(TextButton::buttonColourId, Colour(0xFF000000));
}

/* setting the size of each object in the component */
void QueueComponent::resized() {
  double rowH = getHeight() / 10;
  double colW = getWidth() - 10;

  queueTable.setBounds(0, 0, getWidth(), rowH * 9);
  clearQueue.setBounds(5, rowH * 9, colW / 2 - 4, rowH - 5);
  playQueueButton.setBounds(clearQueue.getRight() + 4, rowH * 9, colW / 2, rowH - 5);
}

/* returns the number of items in the TableListBox */
int QueueComponent::getNumRows() {
  //repaint();
  return queuedTracks.size();
}

/* set the background color of the TableListBox cells */
void QueueComponent::paintRowBackground(Graphics& g, int rowNumber, int width, int height, bool rowIsSelected) {
  g.fillAll(Colour(0xFFF2E5D7));
}

/* drawing of infomation inside each cell */
void QueueComponent::paintCell(Graphics& g, int rowNumber, int columnId, int width, int height, bool rowIsSelected) {
  if (rowNumber < getNumRows()) {
    if (columnId == 1) {
      g.drawText(queuedTracks[rowNumber].trackTitle, 2, 0, width - 4, height, Justification::centredLeft, true);
    }
  }
}

/* Create a cutom component that is added into a cell */
Component* QueueComponent::refreshComponentForCell(int rowNumber, int columnId, bool isRowSelected, Component* existingComponentToUpdate) {
  if (columnId == 2) {
    if (existingComponentToUpdate == nullptr) {
      TextButton* btn = new TextButton{ "x" };
      String id{ std::to_string(rowNumber) };

      btn->setComponentID(id);
      btn->addListener(this);
      existingComponentToUpdate = btn;
    }
  }
  return existingComponentToUpdate;
}

/* Determine what action to take when a button is clicked */
void QueueComponent::buttonClicked(Button* button) {
  // clear the queue if "Clear Queue" button clicked
  if (button == &clearQueue) {
    DBG("QueueComponent::buttonClicked - clear queue button clicked");
    if (!queuedTracks.empty()) {
      queuedTracks.clear();
      queueTable.updateContent();
    }
  }

  else if (button == &playQueueButton) {
    DBG("QueueComponent::buttonClicked - playQueue button clicked");
    const char* text = playQueueButton.getToggleState() ? "Playing Queue" : "Play Queue";
    playQueueButton.setButtonText(text);
  }

  else {
    // get the component's ID as an int
    int id = stoi(button->getComponentID().toStdString());

    // delete the item stored in queuedTracks[id]
    if (button->getButtonText() == "x") {
      DBG("QueueComponent::buttonClicked - delete button clicked");
      queuedTracks.erase(queuedTracks.begin() + id);
      queueTable.updateContent();
    }
  }
}

