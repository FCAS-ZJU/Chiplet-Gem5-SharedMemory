#include "learning_gem5/part2/hello_object.hh"


#include <iostream>
#include "base/logging.hh"
#include "debug/HelloExample.hh"

HelloObject::HelloObject(HelloObjectParams *params) :
    SimObject(params),
    // This is a C++ lambda. When the event is triggered, it will call the
    // processEvent() function. (this must be captured)
    event([this]{ processEvent(); }, name() + ".event"),
    goodbye(params->goodbye_object),
    // Note: This is not needed as you can *always* reference this->name()
    myName(params->name),
    latency(params->time_to_wait),
    timesLeft(params->number_of_fires)
{
    DPRINTF(HelloExample, "Created the hello object\n");
    panic_if(!goodbye, "HelloObject must have a non-null GoodbyeObject");
}

void
HelloObject::processEvent()
{
    timesLeft--;
    DPRINTF(HelloExample, "Process %d\n",timesLeft);

    if(timesLeft<=0){
        DPRINTF(HelloExample,"Finish!\n");
        goodbye->sayGoodbye(myName);
    }else{
        schedule(event,curTick()+latency);
    }
}

void
HelloObject::startup()
{
    schedule(event,latency);
}

HelloObject*
HelloObjectParams::create()
{
    return new HelloObject(this);
}

