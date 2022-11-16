#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent() {
  // set the size of the component
  setSize(800, 600);

  // Some platforms require permissions to open input channels so request that here
  if (RuntimePermissions::isRequired(RuntimePermissions::recordAudio)
    && !RuntimePermissions::isGranted(RuntimePermissions::recordAudio))
  {
    RuntimePermissions::request(RuntimePermissions::recordAudio,
      [&](bool granted) { setAudioChannels(granted ? 2 : 0, 2); });
  }
  else {
    // Specify the number of input and output channels that we want to open
    setAudioChannels(0, 2);
  }

  // Make deckGUI1 & deckGUI2 visible
  addAndMakeVisible(deckGUI1);
  addAndMakeVisible(deckGUI2);

  addAndMakeVisible(playlistComponent);
  addAndMakeVisible(queueComponent);
  addAndMakeVisible(crossfader);

  formatManager.registerBasicFormats();
}

MainComponent::~MainComponent() {
  // This shuts down the audio device and clears the audio source.
  shutdownAudio();
}

//==============================================================================
/* Prepare the audio source for playing. */
void MainComponent::prepareToPlay(int samplesPerBlockExpected, double sampleRate) {
  // This function will be called when the audio device is started, or when
  // its settings (i.e. sample rate, block size, etc) are changed.

  // add player1 and player2 as an input source to mixer
  mixerSource.addInputSource(&player1, false);
  mixerSource.addInputSource(&player2, false);

  // call prepareToPlay() on each input source
  player1.prepareToPlay(samplesPerBlockExpected, sampleRate);
  player2.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

/* Called repeatedly to fetch subsequent blocks of audio data. */
void MainComponent::getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill) {
  mixerSource.getNextAudioBlock(bufferToFill);
}

/* Release of resources that are no longer needed once playback stops. */
void MainComponent::releaseResources() {
  // This will be called when the audio device stops, or when it is being restarted due to a setting change.

  // free up the resources
  player1.releaseResources();
  player2.releaseResources();
  mixerSource.removeAllInputs();
  mixerSource.releaseResources();
}

//==============================================================================
/*  Drawing of the component. */
void MainComponent::paint(Graphics& g) {
  g.setFont(20.0f);
  g.setColour(Colours::white);
  g.fillAll(Colour{ 0xff000000 });

  deckGUI1.setColour(Slider::thumbColourId, Colour(0xff2A0C4E));
  deckGUI1.setColour(Slider::trackColourId, Colour(0xffffffff));
}

/* Sets the size of each object in the component. */
void MainComponent::resized() {
  deckGUI1.setBounds(0, 0, getWidth() / 2, getHeight() / 2);
  deckGUI2.setBounds(getWidth() / 2, 0, getWidth() / 2, getHeight() / 2);

  crossfader.setBounds(getWidth() / 3, (deckGUI1.getHeight() / 8) * 7, getWidth() / 3, deckGUI1.getHeight() / 8);

  playlistComponent.setBounds(0, getHeight() / 2, (getWidth() / 4) * 3 , getHeight() / 2);
  queueComponent.setBounds(playlistComponent.getRight(), getHeight() / 2, getWidth() / 4, getHeight() / 2);
}
