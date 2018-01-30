#include "timer.h"

using namespace std::chrono;

void Timer::start() {
	_start_time = steady_clock::now();
}

std::string Timer::elapsed() {
	_end_time = steady_clock::now();
	std::string elapsed_string;
	
	if (_time_mode == "seconds") {
		seconds elapsed_time = duration_cast<seconds>(_end_time - _start_time);
		elapsed_string = std::to_string(elapsed_time.count()) + " seconds";
	} else if (_time_mode == "minutes") {
		minutes elapsed_time = duration_cast<minutes>(_end_time - _start_time);
		elapsed_string = std::to_string(elapsed_time.count()) + " minutes";
	} else if (_time_mode == "hours") {
		hours elapsed_time = duration_cast<hours>(_end_time - _start_time);
		elapsed_string = std::to_string(elapsed_time.count()) + " hours";
	}

	return elapsed_string;
}

void Timer::mode(std::string mode) {
	_time_mode = mode;
}

void Timer::print_elapsed() {
    std::cout << "Time elapsed: " << this->elapsed() << std::endl;
}
