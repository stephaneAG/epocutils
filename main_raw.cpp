/* main.cpp - epocutils test file [ RESEARCH EDITION ]
*  This file is a test program to implement the Epoc SDK with the help of the 'epocutils' helper library ( will be built as a shared library later on ... )
*
*  by StephaneAG - 2013
*/

/* include the necessary */
//#include <iostream> // already included in the 'epocutils' helper lib - nb: it may be a good thing to add it here instead of inside the lib for portability ( ? ... )
#include <unistd.h> // necessary for 'usleep(microseconds)'
/* include our helpers */
#include "epocutils_alt.hpp" // the epocutils header file containing both the code for the 'raw EEG data' access & the data after Emotiv algorithms processing
//#include <StephaneAG/epocutils.h> // the future location of the header file

/* -------- wip implm: will be moved to 'epocutils::' as soon as possible ( .. ) -------- */
EE_DataChannel_t targetChannelList[] = {
  ED_COUNTER,
  ED_AF3, ED_F7, ED_F3, ED_FC5, ED_T7,
  ED_P7, ED_O1, ED_O2, ED_P8, ED_T8,
  ED_FC6, ED_F4, ED_F8, ED_AF4,
  ED_GYROX, ED_GYROY, ED_TIMESTAMP,
  ED_FUNC_ID, ED_FUNC_VALUE, ED_MARKER, ED_SYNC_SIGNAL
};

const char* channelNames[] = { 
  "COUNTER",
  "AF3","F7","F3","FC5","T7",
  "P7","O1","O2","P8","T8",
  "FC6","F4","F8","AF4",
  "GYROX","GYROY","TIMESTAMP",
  "FUNC_ID","FUNC_VALUE","MARKER","SYNC_SIGNAL" 
};

/* -------------------------------------------------------------------------------------- */



/* the main */
int main(int agrc, char *argv[])
{
  // init code for the program ( SDL stuff, ... )
  // [ EMOTIV EPOC ] init code for the Epoc part of the program - using 'epocutils'
  EmoEngineEventHandle eEvent = epocutils::createEventHandle(); // create an 'EmoEngineEventHandle' using the 'epocutils' helper
  EmoStateHandle       eState = epocutils::createStateHandle(); // create an 'EmoStateHandle' using the 'epocutils' helper
  DataHandle           hData; // = epocutils::createDataHandle(); // RESEARCH EDITION - create a 'DataHandle' using the 'epocutils' helper
  //EE_DataChannel_t     targetChannelList[] = epocutils::createDataChannelsList(); // RESEARCH EDITION - create a 'EE_DataCHannelList' array using the 'epocutils' helper
  //const char*          channelNames = epocutils::createDataChannelNames(); // RESEARCH EDITION - create a 'const char*' map using the 'epocutils' helper
  unsigned int         userID = 0; // create a user ID
  int                  epoc_state = 0; // create a int variable that'll hold the return codes from the EmoEngine ( EDK_OK, etc .. )
  bool                 connected = false; // create a boolean that'll be used to gather data from the Epoc headset if true ( must be set to 'true' to handle new events with 'epocutils::handleEvents()' )
  float                secs = 1; // [ RESEARCH EDITION ] set the var that 'll be sued to set the 'BufferSizeInSec' value
  bool                 dataAcquisitionEnabled = false; // [ RESEARCH EDITION ] 'll probably be moved inside one of the 'epocutils' lib fcns ( ... )
  bool                 dataHandleIsSet = false;
  bool                 bufferSizeIsSet = false;
  //epocutils::EpocHeadset_t billy; // [ DEV EDITION] create a new 'EpocHeadset_t' struct that we'll update with the latest data using the 'epocutils::handleEvents()' function
  epocutils::EpocHeadsetRaw_t cartman; // [ RESEARCH EDITION ] create a new 'EpocHeadsetRaw_t' struct that we'll update with the latest data using the 'epocutils::handleEvents()' function

  //epocutils::initializeEpocHeadsetStruct(userID, billy); // init the EpocHeadset_t struct // later, there 'll be a fcn that 'll provide directly an initialized struct ( ... )
  epocutils::initializeEpocHeadsetStruct(userID, cartman); // init the EpocHeadsetRaw_t struct // note that the name of the function used is the same, only its signature is changing ( ... )

  // [ EMOTIV EPOC ]connection code for the Epoc part of the program - using 'epocutils'
  int epoc_conn_retval = epocutils::connect(connected); // try to connect to the Epoc headset
  if ( epoc_conn_retval != 0 )
  {
    std::cout << "Failed to connect to the Epoc headset." << std::endl;
    std::cout << "Program says: I'm of none use if no Epoc headset if connected ! -> Exiting." << std::endl;
    //exit 1; // for certain programs that are pretty useless without the Epoc headset ( for games, we can still use the mouse & keyboard or some Arduino-made joystick ;p )
  } else
  {
    std::cout << "Connected to the Epoc headset." << std::endl;
  }

  // ---- As I'm not currently "doing it" the way it's achieved in 'EEGLogger.cpp', adding a little delay before trying to set the data acquisition to true ( > enough time for the 'user_added' event to pop )
  //std::cout << "Sleeping for 30000 microseconds .."<< std::endl;
  //usleep(30000); // I guess sleeping makes the buffer full of stuff for some time ( maybe that's why we get wayyy more than 4 samples at at time ( like every 1 second ) & why the counter nevers starts at 0 )
  //std::cout << "Awake !"<< std::endl;
  // [ EMOTIV EPOC - RESEARCH EDITION ] data acquisition enabling code for the Epoc part of the program - using 'epocutils'
  //int epoc_dataAccess_retval = epocutils::enableDataAcquisition(connected, dataAcquisitionEnabled, userID); // nb: we could ( and 'll ) use the same variable twice ( the one handling the returned int value )
  //if ( epoc_dataAccess_retval != 0 )
  //{
  //  std::cout << "Failed to enable raw EEG data acquisition on the Epoc headset." << std::endl;
  //  std::cout << "Program says: If you can't access raw EEG data with your library, you may either have the EDK Dev Edition or missing stuff from your Edk.h header file ..." << std::endl;
  //} else
  //{
  //  std::cout << "Raw EEG data acquisition enabled on the Epoc headset." << std::endl;
  //}

  // [ EMOTIV EPOC - RESEARCH EDITION ] buffer size setup code for the Epoc part of the program - using 'epocutils'
  //epocutils::setBufferSizeInSeconds(connected, dataAcquisitionEnabled, secs); // set the buffer size in seconds - Note: bools 'connected' & 'dataAcquisitionEnabled' both have to be true to use this fcn

  while (1) // dummy infinite loop for now ( ... )
  {
    // handle the SDL events & other stuff like that
    // Nb: if we need to update the position or parameters of stuff, we'll do so just after getting stuff from the Epoc headset
    
    // handle fresh data from the Epoc headset if it is connected
    // [ DEV EDITION - access data after it has been processed by the Emotiv algorithms ]
    //epocutils::handleEvents(connected, epoc_state, eEvent, eState, userID, billy); // the latest data from the Epoc headset 'll update the struct member values
    // [ RESEARCH EDITION - access data right after hardware pre-processing ( mainly amplifying I suppose [but still, I gotta DIGG ! ;p ] ) ]
    // THE ONE USED BEFORE "ALT" IMPLM
    //epocutils::handleEvents(connected, dataAcquisitionEnabled, epoc_state, eEvent, eState, hData, userID, cartman); // the latest data from the Epoc headset 'll update the struct member values
    //epocutils::handleEvents(connected, dataAcquisitionEnabled, epoc_state, eEvent, eState, hData, userID, cartman, targetChannelList, channelNames);
    // THE CURRENTLY WIP IMPLM "ALT" VERSION
    epocutils::handleEvents(connected, dataAcquisitionEnabled, dataHandleIsSet, bufferSizeIsSet, epoc_state, eEvent, eState, hData, secs, userID, cartman, targetChannelList, channelNames);

    // if new data is available from the Epoc headset
    //if ( billy.newDataToRead )
    if ( cartman.newDataToRead )
    {
      std::cout << "Hello World Raw EEG data ?! "<< std::endl;
      //std::cout << "billy time: " << billy.time << " UserID: " << billy.userID << std::endl; / etc .. -> here, we can access all the data from the Epoc headset VIA the EpocHeadset_t structure
      //billy.newDataToRead = false; // set the 'newDataToRead' bool to false to prevent unnecessary polling ( ... ) // -> YES, I'll write a fcn do do that ( ... )
      cartman.newDataToRead = false; // same as above ( ... )
    }
    // or
    // epocutils::epocEventsHandler(epoc_conn_state, eEvent, eState, userID, callbackFunction); // not yet ( ... )
    // render ( updated ) SDL stuff & other things like that
  }

  // [ EMOTIV EPOC ] free all the resources used by the Epoc headset
  //epocutils::disconnect(connected, eState, eEvent); // DEV EDITION - disconnect from the Epoc headset & free all the resources involved (bool to 'false', EmoEngine resources freed ( gone ^^! ) ) 
  epocutils::disconnect(connected, dataAcquisitionEnabled, eState, eEvent, hData); // RESEARCH EDITION  - nearly the same as above

  return 0;
}
