#ifndef MAINSTORAGE_H
#define MAINSTORAGE_H

#include <iostream>
#include <mutex>
#include <future>
#include <condition_variable>
#include <queue>
using namespace std;
using namespace std::chrono; 
using std::chrono::operator""ms;


//get current time as a strt time of application
const  auto clock_start = std::chrono::high_resolution_clock::now();
bool flag = true;

static constexpr float pi = std::numbers::pi;

enum fields {x, y, z, roll, pitch, yaw};
/*Singelton Main storage class, with public get functions for data and
update functions for updating data*/
class MainStorage
{
private:
	static MainStorage *instance;
	static mutex mutex_;

protected:
	MainStorage() {};
	~MainStorage() {}
	float x {-1};
	float y {-1};
	float z {-1};
	float roll {-1*pi};
	float pitch {-1*pi};
	float yaw {-1*pi};

public:
	MainStorage(MainStorage &other) = delete;
	void operator=(const MainStorage &) = delete;
	static MainStorage *getInstance();

	float get_x (void) const {return x;}
	float get_y (void) const {return y;}
	float get_z (void) const {return z;}
	float get_roll (void) const {return roll;}
	float get_pitch (void) const {return pitch;}
	float get_yaw (void) const {return yaw;}
	
	float calculate_by_time (unsigned int t)
	{
		float res;
		int d = t%100;
		float delta = d;

		if (delta < 50)
			res = -1 + delta/25;
		else
			res = 1 - (delta-50)/25;
				return res;
	}
	
	float calculate_angle (unsigned int t) 
	{
		return pi*calculate_by_time(t);
	}
	
	void updateField (int threadId, enum fields f, float val)
	{
	    lock_guard<mutex> lock(mutex_);
	    switch (f) 
	    {
	    case fields::x:
	        if (threadId != 10) //can raise excepteion here
	            break;
	        x = val;
	        break;
	    case fields::y:
	        if (threadId != 10) //can raise excepteion here
	            break;
	        y = val;
	        break;
	    case fields::z:
	        if (threadId != 20) //can raise excepteion here
	            break;
	        z = val;
	        break;
	    case fields::roll:
	        if (threadId != 40) //can raise excepteion here
	            break;
	        roll = val;
	        break;
	    case fields::pitch:
	        if (threadId != 40) //can raise excepteion here
	            break;
	        pitch = val;
	        break;
	    case fields::yaw:
	        if (threadId != 20) //can raise excepteion here
	            break;
	        yaw = val;
	        break;
	    }
	    //DEBUG: Just a print for debug purposes
	    //cout << "Recieved field: " << f <<" with value: "<<val<<" ,from thread: "<<threadId<<endl;
	}
};

MainStorage* MainStorage::instance{nullptr};
std::mutex MainStorage::mutex_;

MainStorage *MainStorage::getInstance() 
{
	//thread protection
	lock_guard<mutex> lock(mutex_);
	if (!instance)
		instance = new MainStorage();

	return instance;
}

struct Message 
{
    int senderId;
    float content;
    enum fields f;
};

queue<Message> queue_x, queue_y, queue_z, queue_roll, queue_pitch, queue_yaw;
mutex mtx_x, mtx_y, mtx_z, mtx_roll, mtx_pitch, mtx_yaw; // Mutex for each queue


// Function to send a message to a specific queue
void sendMessage(queue<Message>& queue, mutex& mtx, int senderId, float content, enum fields f) 
{
    std::lock_guard<std::mutex> lock(mtx);
    queue.push({senderId, content, f});
}

// Function to receive messages and update singleton data
void receiveMessages(queue<Message>& queue, mutex& mtx) 
{
    unique_lock<mutex> lock(mtx);
    while (!queue.empty()) {
        Message msg = queue.front();
        queue.pop();
        
        MainStorage::getInstance()->updateField(msg.senderId, msg.f, msg.content);
    }
}

// Thread 10: Wakes every 10 ms, sends to Thread 20 (20 ms) and Thread 40 (40 ms)
void thread10() {
    int counter = 0; // Counts 10 ms intervals
    while (flag) {
        this_thread::sleep_for(10ms); // Wake every 10 ms
        counter += 10;
        auto clock_now = std::chrono::high_resolution_clock::now();
        unsigned int delta = (std::chrono::duration_cast<std::chrono::milliseconds>(clock_now - clock_start)).count();
        //DEBUG: Just for a program to stop in test run, should be commented
        flag = (delta > 1000) ? false:true;
        
        // Every 20 ms, send to Thread 20
        if (counter % 20 == 0) {
            sendMessage(queue_z, mtx_z, 10, MainStorage::getInstance()->calculate_by_time(delta), fields::z);
        }

        // Every 40 ms, send to Thread 40
        if (counter % 40 == 0) {
            sendMessage(queue_roll, mtx_roll, 10, MainStorage::getInstance()->calculate_angle(delta), fields::roll);
        }

        // Process incoming messages
        receiveMessages(queue_x, mtx_x);
        receiveMessages(queue_y, mtx_y);
    }
}

// Thread 20: Wakes every 20 ms, sends to Thread 10 (20 ms) and Thread 40 (40 ms)
void thread20() {
    int counter = 0; // Counts 20 ms intervals
    while (flag) {
        this_thread::sleep_for(20ms); // Wake every 20 ms
        counter += 20;
        auto clock_now = std::chrono::high_resolution_clock::now();
        unsigned int delta = (std::chrono::duration_cast<std::chrono::milliseconds>(clock_now - clock_start)).count();

        // Every 20 ms, send to Thread 10
        sendMessage(queue_y, mtx_y, 40, MainStorage::getInstance()->calculate_by_time(delta), fields::y);
        
        // Every 40 ms, send to Thread 40
        if (counter % 40 == 0) 
        {
            sendMessage(queue_yaw, mtx_yaw, 40, MainStorage::getInstance()->calculate_angle(delta), fields::yaw);
        }

        // Process incoming messages
        receiveMessages(queue_roll, mtx_roll);
        receiveMessages(queue_pitch, mtx_pitch);
    }
}

// Thread 40: Wakes every 40 ms, sends to Thread 10  and Thread 20 
void thread40() {
    while (flag) {
        this_thread::sleep_for(40ms); // Wake every 40 ms
        auto clock_now = std::chrono::high_resolution_clock::now();
        unsigned int delta = (std::chrono::duration_cast<std::chrono::milliseconds>(clock_now - clock_start)).count();

        // Send to Thread 10
        sendMessage(queue_x, mtx_x, 20, MainStorage::getInstance()->calculate_by_time(delta), fields::x);
        

        // Send to Thread 20
        sendMessage(queue_pitch, mtx_pitch, 20, MainStorage::getInstance()->calculate_angle(delta), fields::pitch);

        // Process incoming messages
        receiveMessages(queue_z, mtx_z);
        receiveMessages(queue_yaw, mtx_yaw);
    }
}

#endif
