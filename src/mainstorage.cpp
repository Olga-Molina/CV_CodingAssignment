/******************************************************************************


*******************************************************************************/


#include <chrono>
#include <iostream>
#include <thread>
#include <future>
#include "mainstorage.h"

using namespace std;
using namespace std::chrono; 
using namespace Mainstorage;

//get current time as a strt time of application
const auto clock_start = std::chrono::high_resolution_clock::now();

auto clock_now = clock_start;
unsigned int delta = 0;
bool flag = true;
using std::chrono::operator""ms;


int main()
{
    float x = -1, y = -1, z = -1, roll = -1*pi, pitch = -1*pi, yaw = -1*pi;
    int count = 0;

    
    while (flag)
    {
        this_thread::sleep_for(10ms); // sleep fo 10ms
        //get current time and calculate time difference from a start as unsigned int in ms
        clock_now = std::chrono::high_resolution_clock::now();
        delta = (std::chrono::duration_cast<std::chrono::milliseconds>(clock_now - clock_start)).count();
        //Call 10ms task and take its results
        future<tuple<float, float>> asyncTask1 = async(&Task10::StartProcessing,  make_tuple(x,y), delta);
        tie (z,roll) = asyncTask1.get();
        //If it is an even loop iteration call 20ms task and take its results
        if (count%2 ==1)
        {
            future<tuple<float, float>> asyncTask2 = async(&Task20::StartProcessing,  make_tuple(z,yaw), delta);
            tie(x, pitch) = asyncTask2.get();
        }
        //If it is a 4th multiplier loop iteration call 40 ms task and take it results
        if (count ==3)
        {
            future<tuple<float, float>> asyncTask3 = async(&Task40::StartProcessing,  make_tuple(roll, pitch), delta);
            tie(y, yaw) = asyncTask3.get();
        }
        //Increase loop caunter and take it modulo 4
        count = (++count)%4;
       
       //Just a print to see results
       // cout << "x is: " << x <<" , y is: "<<y<<" ,z is: "<<z<<" ,roll is: "<< roll<<" ,pitch is: "<<pitch<<" ,yaw is: "<< yaw <<" ,delta is: "<< delta <<endl;
        
        
        //Just for a program to stop in test run
        if (delta > 1000)
        {
            flag = false;
        }
    }
    return 0;
}
