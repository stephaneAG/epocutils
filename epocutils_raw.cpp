/*  epocutils.cpp - implementation file */

/* we include the corresponding header file */
#include "epocutils_alt.hpp"

/* other includes will be below */
//#include <iostream> // included in .h

/* we define the functions declared in the corresponding header file in the current implementation file */

/* connect to the Epoc headset */
int epocutils::connect(bool& connected)
{
  if ( EE_EngineConnect() == EDK_OK )
  {
    connected = true; // update the Epoc headset connection status
    std::cout << "epocutils:: Now connected to the Epoc headset." << std::endl; // inform that we're successfully connected
    return 0; // return 0 as no errors occured
  } else {
    std::cout << "epocutils:: An error occured while trying to connect to the Epoc headset." << std::endl; // inform that an error occured
    return 1; // return 1 as an error occured
  }
}

/* disconnect from the Epoc headset ( WARNING: this function WILL NOT free the 'EmoStateHandle' neither the 'EmoEngineEventHandle' ! ) */
void epocutils::disconnect(bool& connected)
{
  EE_EngineDisconnect(); // we disconnect from the EmoEngine
  connected = false; // we update the headset connection status ( a 'connected = false' will prevent new readings from the EmoEngine using 'epocutils::handleEvents()' )
  std::cout << "epocutils:: Now disconnected from the Epoc headset." << std::endl; // inform that an error occured
}

/* disconnect from the Epoc headset AND clean up */
void epocutils::disconnect(bool& connected, EmoStateHandle& eState, EmoEngineEventHandle& eEvent)
{
  EE_EngineDisconnect(); // we disconnect from the EmoEngine
  connected = false; // we update the headset connection status ( a 'connected = false' will prevent new readings from the EmoEngine using 'epocutils::handleEvents()' )
  std::cout << "epocutils:: Now disconnected from the Epoc headset." << std::endl; // inform that the connection with the Epoc headset has been closed
  EE_EmoStateFree(eState); // free the 'EmoStateHandle' instance
  std::cout << "epocutils:: EmoStateHandle resources freed." << std::endl; // inform that the EmoStateHandle instance has been freed
  EE_EmoEngineEventFree(eEvent); // free the 'EmoEngineEventHandle' instance
  std::cout << "epocutils:: EmoEngineEventHandle resources freed." << std::endl; // inform that the EmoEngineEventHandle instance has been freed
}

/* RESEARCH EDITION - disconnect from the Epoc headset AND clean up */
void epocutils::disconnect(bool& connected,bool& dataAcquisitionEnabled, EmoStateHandle& eState, EmoEngineEventHandle& eEvent, DataHandle& hData)
{
  EE_EngineDisconnect(); // we disconnect from the EmoEngine
  connected = false; // we update the headset connection status ( a 'connected = false' will prevent new readings from the EmoEngine using 'epocutils::handleEvents()' )
  dataAcquisitionEnabled = false; // nearly the same as above ( ... )
  std::cout << "epocutils:: Now disconnected from the Epoc headset." << std::endl; // inform that the connection with the Epoc headset has been closed
  EE_DataFree(hData); // free the 'DataHandle' instance ( aka free the memory thtat it previously referenced )
  std::cout << "epocutils:: DataHandle resources freed." << std::endl; // inform that the DataHandle instance has been freed
  EE_EmoStateFree(eState); // free the 'EmoStateHandle' instance
  std::cout << "epocutils:: EmoStateHandle resources freed." << std::endl; // inform that the EmoStateHandle instance has been freed
  EE_EmoEngineEventFree(eEvent); // free the 'EmoEngineEventHandle' instance
  std::cout << "epocutils:: EmoEngineEventHandle resources freed." << std::endl; // inform that the EmoEngineEventHandle instance has been freed

}

/* RESEARCH EDITION - enable the data acquisition for the user */
int epocutils::enableDataAcquisition(bool& connected, bool& dataAcquisitionEnabled, unsigned int& userID)
{
  if (connected) // we must be already connected to the Epoc headset to enable the  raw EEG data acquisition
  {
    if ( EE_DataAcquisitionEnable(userID, true) == EDK_OK )
    {
      dataAcquisitionEnabled = true; // update the data acquisition enabling status
      std::cout << "epocutils:: Data acquisition has been successfully enabled for the current user: ready to collect raw EEG data" << std::endl;
      return 0; // return 0 as no errors occured
    } else {
      std::cout << "epocutils:: Error while enabling the raw EEG data acquisition for the current user." << std::endl;
      return 1; // return 1 as an error occured
    }
  }
  else
  {
    return 1; // return 1 as not connected
  }
}

/* RESEARCH EDITION - set the buffer size in seconds */
void epocutils::setBufferSizeInSeconds(bool& connected, bool& dataAcquisitionEnabled, float& bufferSizeInSeconds)
{
  if (connected && dataAcquisitionEnabled)
  {
    if ( EE_DataSetBufferSizeInSec(bufferSizeInSeconds) == EDK_OK )
    {
      std::cout << "epocutil:: buffer size in seconds correctly set." << std::endl;
      printf("epocutils:: Buffer size set to %f seconds \n", bufferSizeInSeconds);
    } else
    {
      std::cout << "epocutil:: Error while setting the buffer size in seconds." << std::endl;
    }
  }
}


/* create an 'EmoEngineEventHandle' */
EmoEngineEventHandle epocutils::createEventHandle()
{
  return EE_EmoEngineEventCreate(); // simply return what the original fcn returns, 'wrapping' it
}

 /* create an 'EmoStateHandle' */
EmoStateHandle epocutils::createStateHandle()
{
  return EE_EmoStateCreate(); // simply return what the original fcn returns, 'wrapping' it
}

/* RESEARCH EDITION - create a 'DataHandle' */
DataHandle epocutils::createDataHandle()
{
  return EE_DataCreate(); // simply return what the original fcn returns, 'wrapping' it
}

/* RESEARCH EDITION - create a 'EE_DataChannel_t' targetChannelList */
//EE_DataChannel_t epocutils::createDataChannelsList()
//{
// return EE_DataChannel_t epocutils::targetChannelList[];
//}

/* RESEARCH EDITION - create a 'const char*' channelNames */
//const char* epocutils::createDataChannelNames()
//{
//  return const char* epocutils::channelNames[];
//}

/*  initialize the struct members */
/* RESEARCH EDITION - 'raw EEG data version' - we access the channels of the Epoc headset directly after hardware-only processing ( no Emotiv algorithms-specific processing occured ) */
void epocutils::initializeEpocHeadsetStruct(unsigned int& userID, EpocHeadsetRaw_t& epocheadset)
{
  // bool to prevent unnecessary polling
  epocheadset.newDataToRead = false;
  // headset counter / Timestamp
  epocheadset.counter = 0;
  epocheadset.timestamp = 0;;
  // Gyroscope
  epocheadset.gyrox = 0;
  epocheadset.gyroy = 0;
  // Fcn ID / value
  epocheadset.func_id = 0;
  epocheadset.func_value = 0;
  // Marker / SyncSignal
  epocheadset.marker = 0;
  epocheadset.sync_signal = 0;
  // the different channels available on the Epoc headset device
  epocheadset.af3 = 0.0f;
  epocheadset.f7 = 0.0f;
  epocheadset.f3 = 0.0f;
  epocheadset.fc5 = 0.0f;
  epocheadset.t7 = 0.0f;
  epocheadset.p7 = 0.0f;
  epocheadset.o1 = 0.0f;
  epocheadset.o2 = 0.0f;
  epocheadset.p8 = 0.0f;
  epocheadset.t8 = 0.0f;
  epocheadset.fc6 = 0.0f;
  epocheadset.f4 = 0.0f;
  epocheadset.f8 = 0.0f;
  epocheadset.af4 = 0.0f;
}

/* DEV EDITION  - 'processed data version' - we access the data of the Epoc headset after it's been processed by the Emotiv algorithms ( no raw EEG data access ) */
void epocutils::initializeEpocHeadsetStruct(unsigned int& userID, epocutils::EpocHeadset_t& epocheadset)
{
  // we init the bool that we'll use to know if we have unread data from the Epoc headset 
  epocheadset.newDataToRead = false;
  // we initialize the EpocHeadset struct with all its member parameters to 0, except the userID wich equals the one passed as argument
  epocheadset.time = 0.0f;
  epocheadset.userID = userID;
  epocheadset.wirelessSignalStatus = 0;
  // Expressiv suite
  epocheadset.isBlinking = 0;
  epocheadset.isWinkingLeft = 0;
  epocheadset.isWinkingRight = 0;
  epocheadset.isLookingLeft = 0;
  epocheadset.isLookingRight = 0;
  epocheadset.eyebrow = 0.0f;
  epocheadset.furrow = 0.0f;
  epocheadset.smile = 0.0f;
  epocheadset.clench = 0.0f;
  epocheadset.smirkLeft = 0.0f;
  epocheadset.smirkRight = 0.0f;
  epocheadset.laugh = 0.0f;
  // Affectiv suite
  epocheadset.shortTermExcitement = 0.0f;
  epocheadset.longTermExcitement = 0.0f;
  epocheadset.engagementBoredom = 0.0f;
  // Cognitiv suite
  epocheadset.cogntivAction = 0;
  epocheadset.cogntiviActionConfidence = 0; // close, but not the same as 'power' ( YES, I DISAGREE with Emotiv's words on this ( ... )
  
  std::cout << "epocutils:: Epoc headset struct initialized." << std::endl; // inform that the EpocHeadset struct has been initialized
}

/* DEV EDITION - handle fresh data from the Epoc headset, if connected, & update the passed 'EpocHeadset_struct' structure with that data */
void epocutils::handleEvents(bool& connected, int& epoc_state, EmoEngineEventHandle& eEvent, EmoStateHandle& eState, unsigned int& userID, epocutils::EpocHeadset_t& epocheadset)
{
  if ( connected )
  {
    epoc_state = EE_EngineGetNextEvent(eEvent); // get the latest EmoEngine events ( aka 'stuff from Epoc' )
    
    if (epoc_state == EDK_OK)
    {
      EE_Event_t eventType = EE_EmoEngineEventGetType(eEvent);
      EE_EmoEngineEventGetUserId(eEvent, &userID);

      // Log the EmoState if it has been updated
      if (eventType == EE_EmoStateUpdated)
      {
	std::cout << "epocutils:: New data from the Epoc headset." << std::endl; // inform that new data is available
        EE_EmoEngineEventGetEmoState(eEvent, eState);
	const float timestamp = ES_GetTimeFromStart(eState);
	std::cout<<"epocutils:: Timestamp: " << timestamp <<" New EmoState from user: " << userID << std::endl;

	// write data from the EmoEngine to the EpocHeadset struct ... or just log it to stdout for the moment ?
	epocheadset.time = ES_GetTimeFromStart(eState);
	epocheadset.userID = userID;
	epocheadset.wirelessSignalStatus = static_cast<int>(ES_GetWirelessSignalStatus(eState));
	
	// Expressiv suite
	epocheadset.isBlinking = ES_ExpressivIsBlink(eState);
	epocheadset.isWinkingLeft = ES_ExpressivIsLeftWink(eState);
	epocheadset.isWinkingRight = ES_ExpressivIsRightWink(eState);
	epocheadset.isLookingLeft = ES_ExpressivIsLookingLeft(eState);
	epocheadset.isLookingRight = ES_ExpressivIsLookingRight(eState);

	std::map<EE_ExpressivAlgo_t, float> expressivStates;
	EE_ExpressivAlgo_t upperFaceAction = ES_ExpressivGetUpperFaceAction(eState);
	float upperFacePower = ES_ExpressivGetUpperFaceActionPower(eState);
	EE_ExpressivAlgo_t lowerFaceAction = ES_ExpressivGetLowerFaceAction(eState);
	float lowerFacePower = ES_ExpressivGetLowerFaceActionPower(eState);

	expressivStates[ upperFaceAction ] = upperFacePower;
	expressivStates[ lowerFaceAction ] = lowerFacePower;

	epocheadset.eyebrow = expressivStates[ EXP_EYEBROW ];
	epocheadset.furrow = expressivStates[ EXP_FURROW ];
	epocheadset.smile = expressivStates[ EXP_SMILE ];
	epocheadset.clench = expressivStates[ EXP_CLENCH ];
	epocheadset.smirkLeft = expressivStates[ EXP_SMIRK_LEFT ];
	epocheadset.smirkRight = expressivStates[ EXP_SMIRK_RIGHT ];
	epocheadset.laugh = expressivStates[ EXP_LAUGH ];

	// Affective suite
	epocheadset.shortTermExcitement = ES_AffectivGetExcitementShortTermScore(eState);
	epocheadset.longTermExcitement = ES_AffectivGetExcitementLongTermScore(eState);
	epocheadset.engagementBoredom = ES_AffectivGetEngagementBoredomScore(eState);

	// Cognitiv suite
	epocheadset.cogntivAction = static_cast<int>(ES_CognitivGetCurrentAction(eState));
	epocheadset.cogntiviActionConfidence = ES_CognitivGetCurrentActionPower(eState);
	
	epocheadset.newDataToRead = true; // we update our boolean ot indicate that data is yet to be read

      }

    } else if (epoc_state != EDK_NO_EVENT)
    {
      std::cout << "epocutils:: No new data from the Epoc headset:" << std::endl; // inform that no new data is available
      std::cout << "epocutils:: Internal error in Emotiv Engine or other return value not (yet) handled !" << std::endl;
    }
  }  
}

/* RESEARCH EDITION - handle fresh data from the Epoc headset, if connected & data acquisition enabled, & update the passed 'EpocHeadsetRaw_struct' structure with that data */
void epocutils::handleEvents(bool& connected, bool& dataAcquisitionEnabled, int& epoc_state, EmoEngineEventHandle& eEvent, EmoStateHandle& eState, DataHandle& hData, unsigned int& userID, EpocHeadsetRaw_t& epocheadset)
//void handleEvents(bool& connected, bool& dataAcquisitionEnabled, int& epoc_state, EmoEngineEventHandle& eEvent, EmoStateHandle& eState, DataHandle& hData, unsigned int& userID, EpocHeadsetRaw_t& epocheadset, EE_DataChannel_t& targetChannelList, const char*& channelNames )
{
  if ( connected && dataAcquisitionEnabled )
  {
    epoc_state = EE_EngineGetNextEvent(eEvent); // get the latest EmoEngine events ( aka 'stuff from Epoc' ) // Nb: here, we only check that the return status is OK, and then do stuff with the raw EEG data
    if (epoc_state == EDK_OK)
    {
      // handle stuff
      
      // check the return code of the Data Update Handle
      if ( EE_DataUpdateHandle(0, hData) != EDK_OK )
      {
        std::cout << "epocutils:: Couldn't setup Data Update Handle"<< std::endl;
	exit(1); //return 1; //break; // for the moment ( "for the moment" <- ? I guess I'll remember why I left that comment later ( ... )
      }

      unsigned int nSamplesTaken=0;
      if ( EE_DataGetNumberOfSample(hData,&nSamplesTaken) != EDK_OK )
      {
        std::cout << "Error getting the Number of samples" << std::endl;
	exit(1); //return 1; //break; // for the moment ( "for the moment" <- ? I guess I'll remember why I left that comment later ( ... )
      }

      if (nSamplesTaken != 0)
      {
        std::cout << "epocutils:: --Data updated: number of samples taken:" << nSamplesTaken << std::endl;
	double* ddata = new double[nSamplesTaken];
	for (int sampleIdx=0 ; sampleIdx < (int)nSamplesTaken ; ++sampleIdx)
	{
	  /*
	  std::cout << "epocutils:: ---------------- NEW RAW EEG DATA BEGIN : SAMPLE INDEX: "<< sampleIdx << "----------------" << std::endl;
	  //for (int i = 0 ; i < sizeof(epocutils::targetChannelList)/sizeof(EE_DataChannel_t) ; i++)
	  for (int i = 0 ; i < sizeof(targetChannelList)/sizeof(EE_DataChannel_t) ; i++)
	  {
	    EE_DataGet(hData, targetChannelList[i], ddata, nSamplesTaken);
	    //EE_DataGet(hData, epocutils::targetChannelList[i], ddata, nSamplesTaken);
	    std::cout << channelNames[i] << " : " << (float)ddata[sampleIdx] << "\t";
	    //std::cout << epocutils::channelNames[i] << " : " << (float)ddata[sampleIdx] << "\t";
	  }
	  std::cout << "epocutils:: ---------------- NEW RAW EEG DATA END ----------------------------------------------------" << std::endl;
	  */
	}
	delete[] ddata;
      }
    }
    else if (epoc_state == EDK_CANNOT_ACQUIRE_DATA)
    {
      std::cout << "epocutils:: Unable to acquire raw EEG data"<< std::endl;
    }
    else if (epoc_state != EDK_NO_EVENT)
    {
      std::cout << "epocutils:: No new data from the Epoc headset:" << std::endl; // inform that no new data is available
      std::cout << "epocutils:: Internal error in Emotiv Engine or other return value not (yet) handled !" << std::endl;
    }
  }  
}

// alternative WIP implm of the above function ( -> R: getting wayyy more than 4 samples per second ... ( ..) )
//void epocutils::handleEvents(bool& connected, bool& dataAcquisitionEnabled, bool& dataHandleIsSet, bool& bufferSizeIsSet, int& epoc_state, EmoEngineEventHandle& eEvent, EmoStateHandle& eState, DataHandle& hData, float& bufferSize, unsigned int& userID, EpocHeadsetRaw_t& epocheadset, EE_DataChannel_t& targetChannelList, const char*& channelNames)
void epocutils::handleEvents(bool& connected, bool& dataAcquisitionEnabled, bool& dataHandleIsSet, bool& bufferSizeIsSet, int& epoc_state, EmoEngineEventHandle& eEvent, EmoStateHandle& eState, DataHandle& hData, float& bufferSize, unsigned int& userID, EpocHeadsetRaw_t& epocheadset, EE_DataChannel_t targetChannelList[], const char* channelNames[])
{
  if (connected)
  {
    // set the DataHandle if not previously set
    if (!dataHandleIsSet)
    {
      hData = EE_DataCreate();
      dataHandleIsSet = true;
    }
    // set the buffer size if not previously set
    if (!bufferSizeIsSet)
    {  
      EE_DataSetBufferSizeInSec(bufferSize);
      bufferSizeIsSet = true;
    }
    
    // handle the Epoc eEvents
    epoc_state = EE_EngineGetNextEvent(eEvent);
    if (epoc_state == EDK_OK)
    {
      EE_Event_t eventType = EE_EmoEngineEventGetType(eEvent);
      EE_EmoEngineEventGetUserId(eEvent, &userID);
      if (eventType == EE_UserAdded)
      {
        std::cout << "epocutils:: User Added" << std::endl;
	if ( EE_DataAcquisitionEnable(userID,true) == EDK_OK)
	{
	  std::cout << "epocutils:: raw EEG data acquisition successfully enabled." << std::endl;
	  dataAcquisitionEnabled = true;
	}
	else
	{
		std::cout << "epocutils:: Couldn't enable raw EEG data acquisition." << std::endl;
	}
      } 
      else 
      {
        std::cout << "epocutils:: Other event than 'EE_UserAdded' " << std::endl;
      }
    }

    // handle the raw EEG data if data acquisition is enabled
    if(dataAcquisitionEnabled)
    {
      if ( EE_DataUpdateHandle(0, hData) != EDK_OK )
      {
	std::cout << "epocutils:: Couldn't setup Data Update Handle"<< std::endl;
        //break;
      }

      unsigned int nSamplesTaken=0;
      if ( EE_DataGetNumberOfSample(hData,&nSamplesTaken) != EDK_OK )
      {
        std::cout << "epocutils:: Couldn't get the Number Of Samples" << std::endl;
	//break;
      }

      if (nSamplesTaken != 0)
      {
        std::cout << std::endl << std::endl << "epocutils:: Data updated -> number of samples taken:" << nSamplesTaken << std::endl;
	double* ddata = new double[nSamplesTaken];
	for (int sampleIdx= 0 ; sampleIdx < (int)nSamplesTaken ; ++sampleIdx)
	{
	  std::cout << "epocutils:: NEW RAW EEG DATA BEGIN -> SAMPLE INDEX: "<< sampleIdx << std::endl;
	  /* -------- TESTING BUG -------- */
          /*  Flo, si tu passes par la, ... ;p */

	  /*
	  int targetChannelList_size = sizeof(targetChannelList);
	  int dataChannel_size = sizeof(EE_DataChannel_t);
	  int targetChannelList_elem_size = sizeof(targetChannelList[0]);
	  std::cout << "epocutils:: targetChannelList_size equals: " << targetChannelList_size << std::endl;
	  std::cout << "epocutils:: dataChannel_size equals: " << dataChannel_size << std::endl;
	  std::cout << "epocutils:: targetChannelList / EE_DataChannel_t ratio equals: " << targetChannelList_size / dataChannel_size << std::endl;
	  std::cout << "epocutils:: targetChannelList_elem_size equals: " << targetChannelList_elem_size << std::endl;
	  std::cout << "epocutils:: targetChannelList / targetChannelList[0] ratio equals: " << targetChannelList_size / targetChannelList_elem_size << std::endl;

	  int channelNames_size = sizeof(channelNames);
	  int channelNames_elem_size = sizeof(channelNames[0]);
	  std::cout << "epocutils:: channelNames_size equals: " << channelNames_size << std::endl;
	  std::cout << "epocutils:: channelNames_elem_size equals: " << channelNames_elem_size << std::endl;
	  std::cout << "epocutils:: channelNames ratio equals: " << channelNames_size / channelNames_elem_size<< std::endl;

	  //for (int i = 0; i < targetChannelList_size/dataChannel_size; i++)
	  //for (int i = 0 ; i < (int)sizeof(targetChannelList)/sizeof(EE_DataChannel_t) ; i++)
	  //for (int i = 0 ; i < sizeof(targetChannelList)/sizeof(EE_DataChannel_t) ; i++) // ORIGINAL CODE THAT WORKED OUTSIDE OF FCN IN MAIN HELP: don't know [yet] why it only logs the 2 first values only ( ?! )
          //for (int i = 0 ; i < sizeof(targetChannelList)/sizeof(targetChannelList[0]) ; i++)
	  */
	  
	  /* -------- END TESTING BUG  -------- */
	  // at least, I know the number of channels ;p
	  for (int i = 0; i < 22; i++) // harcoded one to find the why the above code has some problem somewhere ( .. ) // WORKS ( ! ) -> & nb: no more warning message during compiling ( .. )
	  {
	    EE_DataGet(hData, targetChannelList[i], ddata, nSamplesTaken);
	    std::cout << channelNames[i] << " : " << (float)ddata[sampleIdx] << "\t";
	  }

	  std::cout << std::endl << "epocutils:: NEW RAW EEG DATA END" << std::endl << std::endl;
	}
	delete[] ddata;

      } // " if (nSamplesTaken != 0) "


    } // " if(dataAcquisitionEnabled) "
  }
}
