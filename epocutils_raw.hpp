/*  epocutils.h - namespaced utility for the Emotiv Epoc SDK ( Dev Edition )
*
*   This utility provides two easy ways to communicate with the Epoc headset
*   
*   Currently supports the EDK Dev & Research ( raw EEG data ) Edition
* 
*   supposed to be used:  1 -> as a utility class ( include header file in necessary .cpp file )
*                         2 -> as a utility class, embedded in a library that is available system wide ( .so file )
*
*   note: I own 2 Epoc headsets | a SDK Dev edition & I am able to get raw EEG data with it ( hint: all you need is the right 'dek.h' file to make your "Dev" edition a "Research" one ( ... ) )
*
*  
*   by StephaneAG - 2013
*/

/* includes the necessary standard stuff */
#include <iostream>
#include <cstdio> // necessary for 'printf()'
#include <cstdlib> // necessary for 'exit(1)'
#include <map> // necessary for the 'channelNames' map

/* include the Emotiv-proprietary stuff */
#include "edk.h"
#include "edkErrorCode.h"
#include "EmoStateDLL.h"

/* we define what this header file makes available to file that include it */

#ifndef epocutils_h
#define epocutils_h

namespace epocutils
{

  /* RESEARCH EDITION - the target Data Channels List */
  /*
  EE_DataChannel_t targetChannelList[] = {
      ED_COUNTER,
      ED_AF3, ED_F7, ED_F3, ED_FC5, ED_T7, 
      ED_P7, ED_O1, ED_O2, ED_P8, ED_T8, 
      ED_FC6, ED_F4, ED_F8, ED_AF4,
      ED_GYROX, ED_GYROY, ED_TIMESTAMP, 
      ED_FUNC_ID, ED_FUNC_VALUE, ED_MARKER, ED_SYNC_SIGNAL
  };
  */

  /* RESEARCH EDITION - the channels names */
  /*
  const char* channelNames[] = { "COUNTER","AF3","F7","F3","FC5","T7","P7","O1","O2","P8","T8","FC6","F4","F8","AF4","GYROX","GYROY","TIMESTAMP","FUNC_ID","FUNC_VALUE","MARKER","SYNC_SIGNAL" };
  */

  /* RESEARCH EDITION - the 'EpocHeadsetRaw_t' structure to hold all the stuff we're interested in */
  struct EpocHeadsetRaw_t
  {
    // bool to prevent unnecessary polling
    bool newDataToRead;
    // headset counter / Timestamp
    int counter;
    int timestamp;
    // Gyroscope
    int gyrox;
    int gyroy;
    // Fcn ID / value
    int func_id;
    int func_value;
    // Marker / SyncSignal
    int marker;
    int sync_signal;
    // the different channels available on the Epoc headset device
    float af3;
    float f7;
    float f3;
    float fc5;
    float t7;
    float p7;
    float o1;
    float o2;
    float p8;
    float t8;
    float fc6;
    float f4;
    float f8;
    float af4;
  };


  /* DEV EDITION - the 'EpocHeadset_t' structure to hold all the stuff we're interested in */
  struct EpocHeadset_t
  {
    // bool to prevent unnecessary polling
    bool newDataToRead;

    // -- Epoc 'original' values -- // R: " #include <typeinfo> & then  std::cout << typeid( <the-var-name> ).name() << std::endl; ", but still not that easy to use ( ...) 
    float time; // aka, returned a 'f' from the comment above in comments
    unsigned int userID;
    int wirelessSignalStatus; // R: do a 'static_cast<int>(ES_GetWIrelessSignalStatus(eState))' as else the type will be 'EE_SignalStrength_t' ( enum )
    // Expressiv suite
    int isBlinking;
    int isWinkingLeft;
    int isWinkingRight;
    int isLookingLeft;
    int isLookingRight;
        // Digg the doc & ask Emotiv for the 'IsEyesOpen', 'IsLookingUp' & 'IsLookingDown' fcn ( ... )
    float eyebrow; // R: " expressivStates[ EXP_EYEBROW ] " 
    float furrow;
    float smile;
    float clench;
    float smirkLeft;
    float smirkRight;
    float laugh;
    // Affectiv suite
    float shortTermExcitement;
    float longTermExcitement;
    float engagementBoredom;
          // Digg the doc & ask Emotiv for the 'Meditation' & 'Frustration'
    // Cognitiv suite
    int cogntivAction;
    float cogntiviActionConfidence;

  };


  /* connect to the Epoc headset */
  int connect(bool& connected);

  /* disconnect from the Epoc headset ( WARNING: this function WILL NOT free the 'EmoStateHandle' neither the 'EmoEngineEventHandle' ! ) */
  void disconnect(bool& connected);

  /* disconnect from the Epoc headset AND clean up */
  void disconnect(bool& connected, EmoStateHandle& eState, EmoEngineEventHandle& eEvent);

  /* RESEARCH EDITION - disconnect from the Epoc headset AND clean up */
  void disconnect(bool& connected,bool& dataAcquisitionEnabled, EmoStateHandle& eState, EmoEngineEventHandle& eEvent, DataHandle& hData);

  /* RESEARCH EDITION - enable the data acquisition for the user */
  int enableDataAcquisition(bool& connected, bool& dataAcquisitionEnabled, unsigned int& userID);

  /* RESEARCH EDITION - set the buffer size in seconds */
  void setBufferSizeInSeconds(bool& connected, bool& dataAcquisitionEnabled, float& bufferSizeInSeconds);
  
  /* create an 'EmoEngineEventHandle' */
  EmoEngineEventHandle createEventHandle();

  /* create an 'EmoStateHandle' */
  EmoStateHandle createStateHandle();

  /* RESEARCH EDITION - create a 'DataHandle' */
  DataHandle createDataHandle();

  /* RESEARCH EDITION - create a 'EE_DataChannel_t' targetChannelList */
  //EE_DataChannel_t createDataChannelsList();

  /* RESEARCH EDITION - create a 'const char*' channelNames */
  //const char* createDataChannelNames();

  /* DEV EDITION - initialize the struct members ( I actually don't know if it's actually required, but it's always nice to init parameters before setting values to them anyway ( ... ) */
  void initializeEpocHeadsetStruct(unsigned int& userID, EpocHeadset_t& epocheadset);
  /* RESEARCH EDITION - initialize the struct members */
  void initializeEpocHeadsetStruct(unsigned int& userID, EpocHeadsetRaw_t& epocheadset);

  /* TO DO - 'createInitializedEpocHeadsetStruct' & 'createInitializedEpocHeadsetRawStruct' */
  
  /* DEV EDITION - handle fresh data from the Epoc headset, if connected, & update the passed 'EpocHeadset_struct' structure with that data */
  void handleEvents(bool& connected, int& epoc_state, EmoEngineEventHandle& eEvent, EmoStateHandle& eState, unsigned int& userID, EpocHeadset_t& epocheadset);
  /* RESEARCH EDITION - handle fresh data from the Epoc headset, if connected & data acquisition enabled, & update the passed 'EpocHeadsetRaw_struct' structure with that data */
  // the one used until wip the alt one:
  void handleEvents(bool& connected, bool& dataAcquisitionEnabled, int& epoc_state, EmoEngineEventHandle& eEvent, EmoStateHandle& eState, DataHandle& hData, unsigned int& userID, EpocHeadsetRaw_t& epocheadset);
   //void handleEvents(bool& connected, bool& dataAcquisitionEnabled, int& epoc_state, EmoEngineEventHandle& eEvent, EmoStateHandle& eState, DataHandle& hData, unsigned int& userID, EpocHeadsetRaw_t& epocheadset, EE_DataChannel_t& targetChannelList, const char*& channelNames );
  //void handleEvents(bool& connected, bool& dataAcquisitionEnabled, bool& dataHandleIsSet, bool& bufferSizeIsSet, int& epoc_state, EmoEngineEventHandle& eEvent, EmoStateHandle& eState, DataHandle& hData, float& bufferSize, unsigned int& userID, EpocHeadsetRaw_t& epocheadset, EE_DataChannel_t& targetChannelList, const char*& channelNames);
  void handleEvents(bool& connected, bool& dataAcquisitionEnabled, bool& dataHandleIsSet, bool& bufferSizeIsSet, int& epoc_state, EmoEngineEventHandle& eEvent, EmoStateHandle& eState, DataHandle& hData, float& bufferSize, unsigned int& userID, EpocHeadsetRaw_t& epocheadset, EE_DataChannel_t targetChannelList[], const char* channelNames[]);


  /* TO DO - even just to know how to do it - ~same fcn as above but accept a 'fcn&' as last argument, to wich it 'd pass a struct/class when done processing new events ( if any ) */

}

#endif // epocutils_h
