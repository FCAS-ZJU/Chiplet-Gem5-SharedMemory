#include "learning_gem5/part2/goodbye_object.hh"

#include "debug/HelloExample.hh"
#include "sim/sim_exit.hh"

GoodbyeObject::GoodbyeObject(GoodbyeObjectParams *params) :
    SimObject(params), event([this]{ processEvent(); }, name() + ".event"),
    bandwidth(params->write_bandwidth), bufferSize(params->buffer_size),
    buffer(nullptr), bufferUsed(0)
{
    buffer = new char[bufferSize]();
    DPRINTF(HelloExample, "Created the goodbye object\n");
}

GoodbyeObject::~GoodbyeObject()
{
    delete[] buffer;
}

void
GoodbyeObject::processEvent()
{
    DPRINTF(HelloExample, "Processing the event!\n");

    // Actually do the "work" of the event
    fillBuffer();
}

void
GoodbyeObject::sayGoodbye(std::string other_name)
{
    DPRINTF(HelloExample, "Saying goodbye to %s\n", other_name);

    message = "Goodbye " + other_name + "!! ";

    // Kick off the the first buffer fill. If it can't fill the whole buffer
    // because of a limited bandwidth, then this function will schedule another
    // event to finish the fill
    fillBuffer();
}

void
GoodbyeObject::fillBuffer()
{
    // There better be a message
    assert(message.length() > 0);

    // Copy from the message to the buffer per byte.
    int bytes_copied = 0;
    for (auto it = message.begin();
         it < message.end() && bufferUsed < bufferSize - 1;
         it++, bufferUsed++, bytes_copied++) {
        // Copy the character into the buffer
        buffer[bufferUsed] = *it;
    }

    if (bufferUsed < bufferSize - 1) {
        // Wait for the next copy for as long as it would have taken
        DPRINTF(HelloExample, "Scheduling another fillBuffer in %d ticks\n",
                bandwidth * bytes_copied);
        schedule(event, curTick() + bandwidth * bytes_copied);
    } else {
        DPRINTF(HelloExample, "Goodbye done copying!\n");
        // Be sure to take into account the time for the last bytes
        exitSimLoop(buffer, 0, curTick() + bandwidth * bytes_copied);
    }
}

GoodbyeObject*
GoodbyeObjectParams::create()
{
    return new GoodbyeObject(this);
}
