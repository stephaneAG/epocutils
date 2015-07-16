This is the repo of the "epocutils" C/C++ helper dedicated to the Epoc headset.
Any updates on this project will be published here in the future.

Nb: the instance of 'EpocHeadset_t' used in 'main.cpp' is called 'Billy' in reference to Stan & his gran'pa ( South Park ) ;p

Note: next [ BIG ] update soon !

Enjoy !

-- update of 14/09/2015 --

Visit the dedicated Github page for more infos & the following ( present here to benefit from github syntax highlight )

##### Usage

```c++
// main.cpp
#include "epocutils.hpp" // include the 'epocutils' helpers
// ( ... )
int main(int agrc, char *argv[])
{
  // init code for the program ( SDL stuff, ... )
  EmoEngineEventHandle eEvent = epocutils::createEventHandle();
  EmoStateHandle       eState = epocutils::createStateHandle();
  unsigned int         userID = 0;
  int                  epoc_state = 0;
  bool                 connected = false;

  epocutils::EpocHeadset_t myEpocHeadset;
  epocutils::initializeEpocHeadsetStruct(userID, myEpocHeadset);
  // ( ... )
  int epoc_conn_retval = epocutils::connect(connected);
  if ( epoc_conn_retval != 0 )
  {
    std::cout << "Failed to connect to the Epoc headset." << std::endl;
  }
  else
  {
    std::cout << "Connected to the Epoc headset." << std::endl;
  }

  // ( ... )
  while (1) // dummy infinite loop
  {
    epocutils::handleEvents(connected, epoc_state, eEvent, eState, userID, myEpocHeadset);
    if ( myEpocHeadset.newDataToRead )
    {
      // ( ... )
      myEpocHeadset.newDataToRead = false;
    }
  }
  epocutils::disconnect(connected, eState, eEvent);

  return 0;
}
```

##### Compiling

```bash
# example of compiling & linking against the Emotiv library
g++ -Wall -o epoc_test.out main.cpp epocutils.hpp epocutils.cpp edk.h edkErrorCode.h EmoStateDLL.h -ledk -ledk_utils_linux
```
