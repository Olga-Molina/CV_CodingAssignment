/******************************************************************************


*******************************************************************************/


#include <thread>
#include "mainstorage.h"

using namespace std;

int main()
{
    // Create threads
    std::thread t10(thread10);
    std::thread t20(thread20);
    std::thread t40(thread40);

    // Join threads (infinite loop in this case)
    t10.join();
    t20.join();
    t40.join();
    
    return 0;
}
