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
    const int context_save_restore = std::stoi(argv[5]);
    const int isr_time = std::stoi(argv[6]);
    int activity_time = 0;

    /******************************************************************/

    //parse each line of the input trace file
    while(std::getline(input_file, trace)) {
        auto [activity, duration_intr] = parse_trace(trace);

        /******************ADD YOUR SIMULATION CODE HERE*************************/

        if (activity == "CPU"){
            execution += std::to_string(totalTime) + ", " + std::to_string(duration_intr) + ", " + "CPU burst" + "\n"; 
            totalTime += duration_intr;
        }
        else if (activity == "SYSCALL")
        {
                auto [intr_seq, new_time] = intr_boilerplate(totalTime, duration_intr, context_save_restore, vectors);
                execution += intr_seq;
                totalTime = new_time;
                // Add ISR and device driver steps
                activity_time = delays[duration_intr];
                totalTime += isr_time;
                activity_time -= isr_time;
                execution += std::to_string(totalTime) + ", "+ std::to_string(isr_time) + ", transfer data from device to memory\n";
                totalTime += isr_time;
                activity_time -= isr_time;
                execution += std::to_string(totalTime) + ", "+ std::to_string(isr_time) + ", SYSCALL: Run the ISR\n";
                
                if (activity_time > 0){
                    execution += std::to_string(totalTime) + ", "+ std::to_string(activity_time) + ", Check for errors\n";
                    totalTime += activity_time;
                    activity_time = 0;
                }
                
                execution += std::to_string(totalTime) + ", 1, IRET\n";
                totalTime += 1;

        }
        else if (activity == "END_IO")
        {
                auto [intr_seq, new_time] = intr_boilerplate(totalTime, duration_intr, context_save_restore, vectors);
                execution += intr_seq;  
                totalTime = new_time;
                // Add ISR and device driver steps
                activity_time = delays[duration_intr];
                if (activity_time > isr_time){
                    execution += std::to_string(totalTime) + ", " + std::to_string(isr_time) + ", ENDIO: run the ISR (device driver)\n";
                    totalTime += isr_time;
                    activity_time -= isr_time;
                }
                if (activity_time > 0){
                    execution += std::to_string(totalTime) + ", "+ std::to_string(activity_time) + ", Check Device Status\n";
                    totalTime += activity_time;
                    activity_time = 0;
                }
                execution += std::to_string(totalTime) + ", 1, IRET\n";
                totalTime += 1;
        }

        /************************************************************************/

    }

    input_file.close();

    write_output(execution, argv[4]);

    return 0;
}
