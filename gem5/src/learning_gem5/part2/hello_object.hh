#ifndef __LEARNING_GEM5_HELLO_OBJECT_HH__
#define __LEARNING_GEM5_HELLO_OBJECT_HH__

#include <string>

#include "learning_gem5/part2/goodbye_object.hh"
#include "params/HelloObject.hh"
#include "sim/sim_object.hh"



class HelloObject : public SimObject
{
  private:
    void processEvent();

    EventFunctionWrapper event;

    GoodbyeObject* goodbye;

    const std::string myName;

    const Tick latency;

    int timesLeft;

  public:
    HelloObject(HelloObjectParams *p);

    void startup();
};


#endif // __LEARNING_GEM5_HELLO_OBJECT_HH__
