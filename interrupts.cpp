/**
 *
 * @file interrupts.cpp
 * @author Sasisekhar Govind
 *
 */

#include<interrupts.hpp>

int main(int argc, char** argv) {

    //vectors is a C++ std::vector of strings that contain the address of the ISR
    //delays  is a C++ std::vector of ints that contain the delays of each device
    //the index of these elemens is the device number, starting from 0
    auto [vectors, delays] = parse_args(argc, argv);
    std::ifstream input_file(argv[1]);

    std::string trace;      //!< string to store single line of trace file
    std::string execution;  //!< string to accumulate the execution output

    /******************ADD YOUR VARIABLES HERE*************************/

    int totalTime = 0;
    const int context_save_restore = 10;
    const int isr_time = 40;

    auto log_activity = [&](int d, std::string a){
        execution += std::to_string(totalTime) + ", " + std::to_string(d) + ", " + a + "\n"; 
        totalTime += d;
    };
    /******************************************************************/

    //parse each line of the input trace file
    while(std::getline(input_file, trace)) {
        auto [activity, duration_intr] = parse_trace(trace);

        /******************ADD YOUR SIMULATION CODE HERE*************************/

        if (activity == "CPU"){
            log_activity(duration_intr, "CPU BURST");
        }
        else if (activity == "SYSCALL")
        {
            log_activity(1, "switch to kernel mode");
            log_activity(context_save_restore, "context saved");
            log_activity(1, "find vector " + std::to_string(duration_intr) + " in memory position " + std::to_string(duration_intr));
            log_activity(isr_time, "obtain ISR address");
            log_activity(24, "call device driver");
        }
        else if (activity == "END_IO")
        {
            log_activity(1, "switch to kernel mode");
            log_activity(context_save_restore, "context saved");
            log_activity(1, "find vector " + std::to_string(duration_intr) + " in memory position " + std::to_string(duration_intr));
            log_activity(35, "store information in memory");
        }
        
        /************************************************************************/

    }

    input_file.close();

    write_output(execution);

    return 0;
}
