#ifndef MAINSTORAGE_H
#define MAINSTORAGE_H

#include <mutex>
#include <future>
using namespace std;

namespace Mainstorage {

static constexpr float pi = std::numbers::pi;

/*Singelton Main storage class, with public get functions for data and
firend class Task for updating data*/
class MainStorage
{
private:
	static MainStorage *instance;
	static std::mutex mutex_;

protected:
	MainStorage() {};
	~MainStorage() {}
	float x {-1};
	float y {-1};
	float z {-1};
	float roll {-1*pi};
	float pitch {-1*pi};
	float yaw {-1*pi};

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

	float calculate_angle (unsigned int t) {
		return pi*calculate_by_time(t);
	}
public:
	MainStorage(MainStorage &other) = delete;
	void operator=(const MainStorage &) = delete;
	static MainStorage *GetInstance();

	float get_x (void) const {
		return x;
	}
	float get_y (void) const {
		return y;
	}
	float get_z (void) const {
		return z;
	}
	float get_roll (void) const {
		return roll;
	}
	float get_pitch (void) const {
		return pitch;
	}
	float get_yaw (void) const {
		return yaw;
	}

	friend class Task;
};

MainStorage* MainStorage::instance{nullptr};
std::mutex MainStorage::mutex_;

MainStorage *MainStorage::GetInstance() {
	//thread protection
	std::lock_guard<std::mutex> lock(mutex_);
	if (!instance)
		instance = new MainStorage();
	return instance;
}

/*Based Task class that can update MainStorage and get calulations of
coordinates and angles as function of time */
class Task {

protected:
	static std::mutex  _lock;
	static void set_x(float x_i) {
		MainStorage::GetInstance()->x=x_i;
	};
	static void set_y(float y_i) {
		MainStorage::GetInstance()->y=y_i;
	};
	static void set_z(float z_i) {
		MainStorage::GetInstance()->z=z_i;
	};
	static void set_roll(float r) {
		MainStorage::GetInstance()->roll = r;
	};
	static void set_pitch(float p) {
		MainStorage::GetInstance()->pitch=p;
	};
	static void set_yaw(float y) {
		MainStorage::GetInstance()->yaw=y;
	};

	static float calc_coord (unsigned int delta) {
		return MainStorage::GetInstance()->calculate_by_time(delta);
	};
	static float calc_angle (unsigned int delta) {
		return MainStorage::GetInstance()->calculate_angle(delta);
	};
};

std::mutex  Task::_lock;
//Derived class Task10 that updates x, y in MainStorage and sending calculations of z & roll
class Task10 : public Task {
public:

	static tuple<float, float> StartProcessing(tuple<float, float> tup, unsigned int delta) {
		float x, y;
		tie (x,y) = tup;
		const lock_guard<std::mutex> lock(_lock);
		set_x(x);
		set_y(y);
		tuple<float, float> res = make_tuple(calc_coord(delta), calc_angle(delta));
		return res;
	}
};

//Derived class Task20 that updates z, yaw in MainStorage and sending calculations of x & pitch
class Task20 : public Task {
public:

	static tuple<float, float> StartProcessing(tuple<float, float> tup, unsigned int delta) {
		float z, yaw;
		tie (z,yaw) = tup;
		const lock_guard<std::mutex> lock(_lock);
		set_z(z);
		set_yaw(yaw);
		tuple<float, float> res = make_tuple(calc_coord(delta), calc_angle(delta));
		return res;
	}
};
//Derived class Task40 that updates roll, pitch in MainStorage and sending calculations of y & yaw
class Task40 : public Task {
public:
	static tuple<float, float> StartProcessing(tuple<float, float> tup, unsigned int delta) {
		float roll, pitch;
		tie (roll,pitch)  = tup;
		const lock_guard<std::mutex> lock(_lock);
		set_roll(roll);
		set_pitch(pitch);
		tuple<float, float> res = make_tuple(calc_coord(delta), calc_angle(delta));
		return res;
	}
};
}
#endif
