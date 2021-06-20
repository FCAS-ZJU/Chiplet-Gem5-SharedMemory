/*
 * Copyright (c) 2010-2012, 2015, 2017 ARM Limited
 * Copyright (c) 2020 Barkhausen Institut
 * All rights reserved
 *
 * The license below extends only to copyright in the software and shall
 * not be construed as granting a license to any other intellectual
 * property including but not limited to intellectual property relating
 * to a hardware implementation of the functionality of the software
 * licensed hereunder.  You may use the software subject to the license
 * terms below provided that you ensure that this notice is replicated
 * unmodified and in its entirety in all distributions of the software,
 * modified or unmodified, in source code or in binary form.
 *
 * Copyright (c) 2011 Advanced Micro Devices, Inc.
 * Copyright (c) 2003-2006 The Regents of The University of Michigan
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met: redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer;
 * redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution;
 * neither the name of the copyright holders nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "sim/pseudo_inst.hh"

#include <fcntl.h>
#include <unistd.h>

#include <array>
#include <cerrno>
#include <fstream>
#include <string>
#include <vector>

#include "base/debug.hh"
#include "base/output.hh"
#include "config/the_isa.hh"
#include "cpu/base.hh"
#include "cpu/quiesce_event.hh"
#include "cpu/thread_context.hh"
#include "debug/Loader.hh"
#include "debug/Quiesce.hh"
#include "debug/WorkItems.hh"
#include "dev/net/dist_iface.hh"
#include "kern/kernel_stats.hh"
#include "params/BaseCPU.hh"
#include "sim/full_system.hh"
#include "sim/process.hh"
#include "sim/serialize.hh"
#include "sim/sim_events.hh"
#include "sim/sim_exit.hh"
#include "sim/stat_control.hh"
#include "sim/stats.hh"
#include "sim/system.hh"
#include "sim/vptr.hh"

using namespace std;
using namespace Stats;

namespace PseudoInst
{

/**
 * Unique keys to retrieve various params by the initParam pseudo inst.
 *
 * @note Each key may be at most 16 characters (because we use
 * two 64-bit registers to pass in the key to the initparam function).
 */
namespace InitParamKey
{

/**
 *  The default key (empty string)
 */
const std::string DEFAULT = "";
/**
 *  Unique key for "rank" param (distributed gem5 runs)
 */
const std::string DIST_RANK = "dist-rank";
/**
 *  Unique key for "size" param (distributed gem5 runs)
 */
const std::string DIST_SIZE = "dist-size";

} // namespace InitParamKey

static inline void
panicFsOnlyPseudoInst(const char *name)
{
    panic("Pseudo inst \"%s\" is only available in Full System mode.");
}


void gadia_call(ThreadContext *tc, uint64_t srcCoreNum, uint64_t toStart, uint64_t data, uint64_t isCompleted){
		// now we modify the program to change the behaviour so that it can send the message to the router
		// argument as follows
		// we now assume that the srcCore, desCore, whether to complete the task(boolean)
		// the output is assume to be src, destination, data
	uint64_t data_clear = isCompleted;	

	if(data_clear == 2){

		//my lucky number!
    		int fd = 32256;
        	struct flock lock = {};
        	lock.l_type = F_WRLCK;
        	lock.l_whence = SEEK_SET;
        	lock.l_start = 0;
        	lock.l_len = 0;
        	lock.l_pid = getpid();
		fcntl(fd, F_SETLKW, &lock);



          	ofstream outfile;
          	outfile.open("semaphore", std::ios::app);
          	outfile << 1 << endl;
          	outfile.close();


		lock.l_type = F_UNLCK;
                fcntl(fd, F_SETLKW, &lock);

	  	return;

	}


    if(isCompleted == 1){
   
	// modify the file to declare the completeness
	//string name;
	//name	+= (unsigned int)srcCoreNum;
	ofstream outfile;
        outfile.open("semaphore", std::ios::app);
        outfile << 0 << endl;
        outfile.close();
		return;
   } 
		

	  ofstream outfile;
          outfile.open("data", std::ios::app);
          outfile << data << endl;
          outfile.close();

}


static int gadia_counter = 0;
static bool isEOF = false;
uint64_t gadia_receive(ThreadContext* tc, uint64_t srcCore){

    // first we check whether the caculation completed

	/*
    ifstream infile;
    infile.open("cmplt", std::ios::in);
    int b;
    infile >> b;
    if(b == 1){
        // it shows caculation completed
	// then we return -3
	infile.close();
	return (uint64_t)-3;

    }

    infile.close();
*/
   /*
    std::string basicName = "semaphore";
    basicName += srcCore + '0';
    const int length = basicName.length();
    char a[length];

    for(int i = 0; i < length;i++){

	a[i] = basicName[i];
    }*/    
   
    int fd;

    if(srcCore != -1){
	fd = open("semaphore", O_RDONLY);
	struct flock lock = {};
    	lock.l_type = F_WRLCK;
    	lock.l_whence = SEEK_SET;
    	lock.l_start = 0;
    	lock.l_len = 0;

    	lock.l_pid = getpid();

    	char buffer[256] = {'5', '6'};

    	fcntl(fd, F_SETLKW, &lock);

    	//size_t len = 0;
    	ssize_t size = read(fd, buffer, 256);


    	// the size is 2* totalChiplet + 2
    	if(size == (2 * srcCore + 2) || (size == 2*(2*srcCore) + 2)){
       		 // the data is clear
       	 	close(fd);
       	 	lock.l_type = F_UNLCK;
       	 	fcntl(fd, F_SETLKW, &lock);
       	 	return (uint64_t) -4;


    	 }else{

            std::cout<< "needed is " << 2*srcCore + 2 << " or " << 4*srcCore + 2 << " but " << size << std::endl;
    	 }	


	close(fd);
        lock.l_type = F_UNLCK;
        fcntl(fd, F_SETLKW, &lock);

	// the barrier is waitting
	
	return (uint64_t)-6;

    }else{
	//fd = open("semaphore", O_RDONLY);

    }

    //int fd = open(a, O_RDWR);
   

    /*
    struct flock lock = {};
    lock.l_type = F_WRLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start = 0;
    lock.l_len = 0;

    lock.l_pid = getpid();

    char buffer[256] = {'5', '6'};

    fcntl(fd, F_SETLKW, &lock);

    //size_t len = 0;
    ssize_t size = read(fd, buffer, 256);
    // 1 indicates data is not updated
    // then we return -2 
    if(size < 0)
	std::cout << "size < 0, thus it failed" << std::endl;
    if(0){

    }*/
    if(1){
	ifstream infile("data",ios::in);
	string data;
	for(int i = 0;;i++){
	string rubbish;
	if(!infile){
		gadia_counter = 0;
		infile.close();
		printf ("file open failed!\n the counter is: %d", gadia_counter);
		return -1;
	}
	getline(infile, rubbish);

	if(infile.eof()){
                if(isEOF){
                	std::cout << "EOF" << std::endl;
                	//close(fd);
                	//lock.l_type = F_UNLCK;
                	//fcntl(fd, F_SETLKW, &lock);
                	// to declare end here is
			infile.close();
        		return uint64_t(-1);
		}else{
			data = rubbish;
        	        int a = atoi(data.c_str());
        	        gadia_counter++;
        	        infile.close();
			isEOF = true;
	                return a;
		
		}

        }


	if(i == gadia_counter){
		isEOF = false;
		data = rubbish;
		int a = atoi(data.c_str());
		gadia_counter++;
		infile.close();
		return a;
	}


	}

	infile.close();
        
    }
    
}

void
arm(ThreadContext *tc)
{
    DPRINTF(PseudoInst, "PseudoInst::arm()\n");
    if (!FullSystem)
        panicFsOnlyPseudoInst("arm");

    if (tc->getKernelStats())
        tc->getKernelStats()->arm();
}

void
quiesce(ThreadContext *tc)
{
    DPRINTF(PseudoInst, "PseudoInst::quiesce()\n");
    tc->quiesce();
}

void
quiesceSkip(ThreadContext *tc)
{
    DPRINTF(PseudoInst, "PseudoInst::quiesceSkip()\n");
    tc->quiesceTick(tc->getCpuPtr()->nextCycle() + 1);
}

void
quiesceNs(ThreadContext *tc, uint64_t ns)
{
    DPRINTF(PseudoInst, "PseudoInst::quiesceNs(%i)\n", ns);
    tc->quiesceTick(curTick() + SimClock::Int::ns * ns);
}

void
quiesceCycles(ThreadContext *tc, uint64_t cycles)
{
    DPRINTF(PseudoInst, "PseudoInst::quiesceCycles(%i)\n", cycles);
    tc->quiesceTick(tc->getCpuPtr()->clockEdge(Cycles(cycles)));
}

uint64_t
quiesceTime(ThreadContext *tc)
{
    DPRINTF(PseudoInst, "PseudoInst::quiesceTime()\n");

    return (tc->readLastActivate() - tc->readLastSuspend()) /
        SimClock::Int::ns;
}

uint64_t
rpns(ThreadContext *tc)
{
    DPRINTF(PseudoInst, "PseudoInst::rpns()\n");
    return curTick() / SimClock::Int::ns;
}

void
wakeCPU(ThreadContext *tc, uint64_t cpuid)
{
    DPRINTF(PseudoInst, "PseudoInst::wakeCPU(%i)\n", cpuid);
    System *sys = tc->getSystemPtr();

    if (sys->numContexts() <= cpuid) {
        warn("PseudoInst::wakeCPU(%i), cpuid greater than number of contexts"
             "(%i)\n",cpuid, sys->numContexts());
        return;
    }

    ThreadContext *other_tc = sys->threadContexts[cpuid];
    if (other_tc->status() == ThreadContext::Suspended)
        other_tc->activate();
}

void
m5exit(ThreadContext *tc, Tick delay)
{
    DPRINTF(PseudoInst, "PseudoInst::m5exit(%i)\n", delay);
    if (DistIface::readyToExit(delay)) {
        Tick when = curTick() + delay * SimClock::Int::ns;
        exitSimLoop("m5_exit instruction encountered", 0, when, 0, true);
    }
}

void
m5fail(ThreadContext *tc, Tick delay, uint64_t code)
{
    DPRINTF(PseudoInst, "PseudoInst::m5fail(%i, %i)\n", delay, code);
    Tick when = curTick() + delay * SimClock::Int::ns;
    exitSimLoop("m5_fail instruction encountered", code, when, 0, true);
}

void
loadsymbol(ThreadContext *tc)
{
    DPRINTF(PseudoInst, "PseudoInst::loadsymbol()\n");
    if (!FullSystem)
        panicFsOnlyPseudoInst("loadsymbol");

    const string &filename = tc->getCpuPtr()->system->params()->symbolfile;
    if (filename.empty()) {
        return;
    }

    std::string buffer;
    ifstream file(filename.c_str());

    if (!file)
        fatal("file error: Can't open symbol table file %s\n", filename);

    while (!file.eof()) {
        getline(file, buffer);

        if (buffer.empty())
            continue;

        string::size_type idx = buffer.find(' ');
        if (idx == string::npos)
            continue;

        string address = "0x" + buffer.substr(0, idx);
        eat_white(address);
        if (address.empty())
            continue;

        // Skip over letter and space
        string symbol = buffer.substr(idx + 3);
        eat_white(symbol);
        if (symbol.empty())
            continue;

        Addr addr;
        if (!to_number(address, addr))
            continue;

        if (!tc->getSystemPtr()->workload->insertSymbol(addr, symbol))
            continue;


        DPRINTF(Loader, "Loaded symbol: %s @ %#llx\n", symbol, addr);
    }
    file.close();
}

void
addsymbol(ThreadContext *tc, Addr addr, Addr symbolAddr)
{
    DPRINTF(PseudoInst, "PseudoInst::addsymbol(0x%x, 0x%x)\n",
            addr, symbolAddr);
    if (!FullSystem)
        panicFsOnlyPseudoInst("addSymbol");

    std::string symbol;
    tc->getVirtProxy().readString(symbol, symbolAddr);

    DPRINTF(Loader, "Loaded symbol: %s @ %#llx\n", symbol, addr);

    tc->getSystemPtr()->workload->insertSymbol(addr, symbol);
    Loader::debugSymbolTable->insert(addr, symbol);
}

uint64_t
initParam(ThreadContext *tc, uint64_t key_str1, uint64_t key_str2)
{
    DPRINTF(PseudoInst, "PseudoInst::initParam() key:%s%s\n", (char *)&key_str1,
            (char *)&key_str2);
    if (!FullSystem) {
        panicFsOnlyPseudoInst("initParam");
        return 0;
    }

    // The key parameter string is passed in via two 64-bit registers. We copy
    // out the characters from the 64-bit integer variables here, and
    // concatenate them in the key character buffer
    const int len = 2 * sizeof(uint64_t) + 1;
    char key[len];
    memset(key, '\0', len);

    std::array<uint64_t, 2> key_regs = {{ key_str1, key_str2 }};
    key_regs = letoh(key_regs);
    memcpy(key, key_regs.data(), sizeof(key_regs));

    // Check key parameter to figure out what to return.
    const std::string key_str(key);
    if (key == InitParamKey::DEFAULT)
        return tc->getCpuPtr()->system->init_param;
    else if (key == InitParamKey::DIST_RANK)
        return DistIface::rankParam();
    else if (key == InitParamKey::DIST_SIZE)
        return DistIface::sizeParam();
    else
        panic("Unknown key for initparam pseudo instruction:\"%s\"", key_str);
}


void
resetstats(ThreadContext *tc, Tick delay, Tick period)
{
    DPRINTF(PseudoInst, "PseudoInst::resetstats(%i, %i)\n", delay, period);
    if (!tc->getCpuPtr()->params()->do_statistics_insts)
        return;


    Tick when = curTick() + delay * SimClock::Int::ns;
    Tick repeat = period * SimClock::Int::ns;

    Stats::schedStatEvent(false, true, when, repeat);
}

void
dumpstats(ThreadContext *tc, Tick delay, Tick period)
{
    DPRINTF(PseudoInst, "PseudoInst::dumpstats(%i, %i)\n", delay, period);
    if (!tc->getCpuPtr()->params()->do_statistics_insts)
        return;


    Tick when = curTick() + delay * SimClock::Int::ns;
    Tick repeat = period * SimClock::Int::ns;

    Stats::schedStatEvent(true, false, when, repeat);
}

void
dumpresetstats(ThreadContext *tc, Tick delay, Tick period)
{
    DPRINTF(PseudoInst, "PseudoInst::dumpresetstats(%i, %i)\n", delay, period);
    if (!tc->getCpuPtr()->params()->do_statistics_insts)
        return;


    Tick when = curTick() + delay * SimClock::Int::ns;
    Tick repeat = period * SimClock::Int::ns;

    Stats::schedStatEvent(true, true, when, repeat);
}

void
m5checkpoint(ThreadContext *tc, Tick delay, Tick period)
{
    DPRINTF(PseudoInst, "PseudoInst::m5checkpoint(%i, %i)\n", delay, period);
    if (!tc->getCpuPtr()->params()->do_checkpoint_insts)
        return;

    if (DistIface::readyToCkpt(delay, period)) {
        Tick when = curTick() + delay * SimClock::Int::ns;
        Tick repeat = period * SimClock::Int::ns;
        exitSimLoop("checkpoint", 0, when, repeat);
    }
}

uint64_t
readfile(ThreadContext *tc, Addr vaddr, uint64_t len, uint64_t offset)
{
    DPRINTF(PseudoInst, "PseudoInst::readfile(0x%x, 0x%x, 0x%x)\n",
            vaddr, len, offset);
    if (!FullSystem) {
        panicFsOnlyPseudoInst("readfile");
        return 0;
    }

    const string &file = tc->getSystemPtr()->params()->readfile;
    if (file.empty()) {
        return ULL(0);
    }

    uint64_t result = 0;

    int fd = ::open(file.c_str(), O_RDONLY, 0);
    if (fd < 0)
        panic("could not open file %s\n", file);

    if (::lseek(fd, offset, SEEK_SET) < 0)
        panic("could not seek: %s", strerror(errno));

    char *buf = new char[len];
    char *p = buf;
    while (len > 0) {
        int bytes = ::read(fd, p, len);
        if (bytes <= 0)
            break;

        p += bytes;
        result += bytes;
        len -= bytes;
    }

    close(fd);
    tc->getVirtProxy().writeBlob(vaddr, buf, result);
    delete [] buf;
    return result;
}

uint64_t
writefile(ThreadContext *tc, Addr vaddr, uint64_t len, uint64_t offset,
            Addr filename_addr)
{
    DPRINTF(PseudoInst, "PseudoInst::writefile(0x%x, 0x%x, 0x%x, 0x%x)\n",
            vaddr, len, offset, filename_addr);

    // copy out target filename
    std::string filename;
    tc->getVirtProxy().readString(filename, filename_addr);

    OutputStream *out;
    if (offset == 0) {
        // create a new file (truncate)
        out = simout.create(filename, true, true);
    } else {
        // do not truncate file if offset is non-zero
        // (ios::in flag is required as well to keep the existing data
        //  intact, otherwise existing data will be zeroed out.)
        out = simout.open(filename, ios::in | ios::out | ios::binary, true);
    }

    ostream *os(out->stream());
    if (!os)
        panic("could not open file %s\n", filename);

    if (offset != 0) {
        // seek to offset
        os->seekp(offset);
    }

    // copy out data and write to file
    char *buf = new char[len];
    tc->getVirtProxy().readBlob(vaddr, buf, len);
    os->write(buf, len);
    if (os->fail() || os->bad())
        panic("Error while doing writefile!\n");

    simout.close(out);

    delete [] buf;

    return len;
}

void
debugbreak(ThreadContext *tc)
{
    DPRINTF(PseudoInst, "PseudoInst::debugbreak()\n");
    Debug::breakpoint();
}

void
switchcpu(ThreadContext *tc)
{
    DPRINTF(PseudoInst, "PseudoInst::switchcpu()\n");
    exitSimLoop("switchcpu");
}

/*
 * This function is executed when the simulation is executing the syscall
 * handler in System Emulation mode.
 */
void
m5Syscall(ThreadContext *tc)
{
    DPRINTF(PseudoInst, "PseudoInst::m5Syscall()\n");
    Fault fault;
    tc->syscall(&fault);
}

void
togglesync(ThreadContext *tc)
{
    DPRINTF(PseudoInst, "PseudoInst::togglesync()\n");
    DistIface::toggleSync(tc);
}

//
// This function is executed when annotated work items begin.  Depending on
// what the user specified at the command line, the simulation may exit and/or
// take a checkpoint when a certain work item begins.
//
void
workbegin(ThreadContext *tc, uint64_t workid, uint64_t threadid)
{
    DPRINTF(PseudoInst, "PseudoInst::workbegin(%i, %i)\n", workid, threadid);
    System *sys = tc->getSystemPtr();
    const System::Params *params = sys->params();

    if (params->exit_on_work_items) {
        exitSimLoop("workbegin", static_cast<int>(workid));
        return;
    }

    DPRINTF(WorkItems, "Work Begin workid: %d, threadid %d\n", workid,
            threadid);
    tc->getCpuPtr()->workItemBegin();
    sys->workItemBegin(threadid, workid);

    //
    // If specified, determine if this is the specific work item the user
    // identified
    //
    if (params->work_item_id == -1 || params->work_item_id == workid) {

        uint64_t systemWorkBeginCount = sys->incWorkItemsBegin();
        int cpuId = tc->getCpuPtr()->cpuId();

        if (params->work_cpus_ckpt_count != 0 &&
            sys->markWorkItem(cpuId) >= params->work_cpus_ckpt_count) {
            //
            // If active cpus equals checkpoint count, create checkpoint
            //
            exitSimLoop("checkpoint");
        }

        if (systemWorkBeginCount == params->work_begin_ckpt_count) {
            //
            // Note: the string specified as the cause of the exit event must
            // exactly equal "checkpoint" inorder to create a checkpoint
            //
            exitSimLoop("checkpoint");
        }

        if (systemWorkBeginCount == params->work_begin_exit_count) {
            //
            // If a certain number of work items started, exit simulation
            //
            exitSimLoop("work started count reach");
        }

        if (cpuId == params->work_begin_cpu_id_exit) {
            //
            // If work started on the cpu id specified, exit simulation
            //
            exitSimLoop("work started on specific cpu");
        }
    }
}

//
// This function is executed when annotated work items end.  Depending on
// what the user specified at the command line, the simulation may exit and/or
// take a checkpoint when a certain work item ends.
//
void
workend(ThreadContext *tc, uint64_t workid, uint64_t threadid)
{
    DPRINTF(PseudoInst, "PseudoInst::workend(%i, %i)\n", workid, threadid);
    System *sys = tc->getSystemPtr();
    const System::Params *params = sys->params();

    if (params->exit_on_work_items) {
        exitSimLoop("workend", static_cast<int>(workid));
        return;
    }

    DPRINTF(WorkItems, "Work End workid: %d, threadid %d\n", workid, threadid);
    tc->getCpuPtr()->workItemEnd();
    sys->workItemEnd(threadid, workid);

    //
    // If specified, determine if this is the specific work item the user
    // identified
    //
    if (params->work_item_id == -1 || params->work_item_id == workid) {

        uint64_t systemWorkEndCount = sys->incWorkItemsEnd();
        int cpuId = tc->getCpuPtr()->cpuId();

        if (params->work_cpus_ckpt_count != 0 &&
            sys->markWorkItem(cpuId) >= params->work_cpus_ckpt_count) {
            //
            // If active cpus equals checkpoint count, create checkpoint
            //
            exitSimLoop("checkpoint");
        }

        if (params->work_end_ckpt_count != 0 &&
            systemWorkEndCount == params->work_end_ckpt_count) {
            //
            // If total work items completed equals checkpoint count, create
            // checkpoint
            //
            exitSimLoop("checkpoint");
        }

        if (params->work_end_exit_count != 0 &&
            systemWorkEndCount == params->work_end_exit_count) {
            //
            // If total work items completed equals exit count, exit simulation
            //
            exitSimLoop("work items exit count reached");
        }
    }
}

} // namespace PseudoInst
