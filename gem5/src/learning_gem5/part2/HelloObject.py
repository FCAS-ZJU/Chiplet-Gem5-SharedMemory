from m5.params import *
from m5.SimObject import SimObject

class HelloObject(SimObject):
    #It is not required that the type be the same as the name of the class, but it is convention. 
    type = 'HelloObject'
    cxx_header = "learning_gem5/part2/hello_object.hh"

    time_to_wait = Param.Latency("Time before firing the event")
    number_of_fires = Param.Int(1, "Number of times to fire the event before "
                                   "goodbye")

    goodbye_object = Param.GoodbyeObject("A goodbye object")


class GoodbyeObject(SimObject):
    type = 'GoodbyeObject'
    cxx_header = "learning_gem5/part2/goodbye_object.hh"

    buffer_size = Param.MemorySize('1kB',
                                   "Size of buffer to fill with goodbye")
    write_bandwidth = Param.MemoryBandwidth('100MB/s', "Bandwidth to fill "
                                            "the buffer")

    

    
