/*
  ==============================================================================

    TrackInfo.h
    Created: 23 Jul 2022 10:51:13pm
    Author:  pangj

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class TrackInfo {
 public:
  /**
   * \brief 
   *     Constructor. Takes in a File and initialises the other variables.
   */
  TrackInfo(File _file);

  File file;
  std::string trackTitle; // Title of the track
  std::string trackLength; // Length of the track

 private:
  /**
  * \brief
  *    Calculates the length of the track and returns it in minutes:seconds format
  *
  * \param trackFile
  *    The file to be used to get the length of
  *
  * \return
  *    Length of the track in minutes:seconds format
  */
  std::string getTrackLength(File file);
};
