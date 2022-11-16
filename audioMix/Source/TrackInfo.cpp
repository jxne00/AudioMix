/*
  ==============================================================================

    TrackInfo.cpp
    Created: 23 Jul 2022 10:51:13pm
    Author:  pangj

  ==============================================================================
*/

#include "TrackInfo.h"

/* Initialises the other variables with the file passed in */
TrackInfo::TrackInfo(File _file)
                   : file(_file),
                     trackTitle(_file.getFileNameWithoutExtension().toStdString()), // file name without the extension
                     trackLength(getTrackLength(_file)) // length of the track in minutes:seconds format
{
}

/* Calculates the length of the track and returns it in minutes:seconds format */
std::string TrackInfo::getTrackLength(File file) {
  AudioFormatManager formatManager;
  formatManager.registerBasicFormats();

  // create temporary reader for the file
  AudioFormatReader* reader = formatManager.createReaderFor(file);

  if (reader) {
    // get the length(in seconds) of the track (lengthInSamples/sampleRate)
    int lengthInSecs = reader->lengthInSamples / reader->sampleRate;
    delete reader; // delete the reader after using

    std::string mins = std::to_string(lengthInSecs / 60); // minutes in string 
    std::string secs = std::to_string(lengthInSecs % 60); // seconds in string (the remainder)

    // if secs is single digit, add a leading zero
    if (secs.length() == 1)
      secs = "0" + secs;

    // if mins is single digit, add a leading zero
    if (mins.length() == 1)
      mins = "0" + mins;

    // return in minutes:seconds format
    return mins + ":" + secs;
  }
  else return "";
}
