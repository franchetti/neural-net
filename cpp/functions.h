#pragma once

#include <vector>
#include <math.h>
#include <iostream>
#include <memory>
#include <string>
#include <map>
#include <algorithm>
#include <chrono>
#include <random>
#include <SDL.h>
#include <iomanip>
#include <ctime>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <fstream>
#include <Windows.h>
#include <math.h>
#include <thread>
#include <mutex>
using namespace std;

double distance(SDL_Point p1, SDL_Point p2) {
	return sqrt(pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2));
}
double sigmoid(double input) {
	return 1 / (1 + exp(-input));
}
double to_rad(double degrees) {
	return (degrees * 3.14) / 180;
}
double newmap(double n, double startmin, double startmax, double endmin, double endmax) {

	double v = (n - startmin) * (endmax - endmin) / (startmax - startmin) + endmin;

	if (v <= endmax && v >= endmin) {
		return v;
	}
	else if (v > endmax) {
		return endmax;
	}
	else if (v < endmin) {
		return endmin;
	}
}
double angular_coefficient(SDL_Point p1, SDL_Point p2) {
	if (p2.x - p1.x == 0)return 0;
	return (p2.y - p1.y) / (p2.x - p1.x);
}
bool point_inside_rect(SDL_Point p, SDL_Rect rect) {
	if (p.x >= rect.x && p.x <= rect.x + rect.w &&
		p.y >= rect.y && p.y <= rect.y + rect.h)
		return true;
	else
		return false;
}
bool do_intersect(SDL_Point p1, SDL_Point p2, SDL_Point p3, SDL_Point p4)
{
	float ax = p2.x - p1.x;     // direction of line a
	float ay = p2.y - p1.y;     // ax and ay as above

	float bx = p3.x - p4.x;     // direction of line b, reversed
	float by = p3.y - p4.y;     // really -by and -by as above

	float dx = p3.x - p1.x;   // right-hand side
	float dy = p3.y - p1.y;

	float det = ax * by - ay * bx;

	if (det == 0) return false;

	float r = (dx * by - dy * bx) / det;
	float s = (ax * dy - ay * dx) / det;

	return !(r < 0 || r > 1 || s < 0 || s > 1);
}
float intersection(SDL_Point p1, SDL_Point p2, SDL_Point p3, SDL_Point p4) {
	// Store the values for fast access and easy
	// equations-to-code conversion
	float x1 = p1.x, x2 = p2.x, x3 = p3.x, x4 = p4.x;
	float y1 = p1.y, y2 = p2.y, y3 = p3.y, y4 = p4.y;

	float d = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4);
	// If d is zero, there is no intersection
	if (d == 0) return 0;

	// Get the x and y
	float pre = (x1*y2 - y1*x2), post = (x3*y4 - y3*x4);
	float x = (pre * (x3 - x4) - (x1 - x2) * post) / d;
	float y = (pre * (y3 - y4) - (y1 - y2) * post) / d;

	// Check if the x and y coordinates are within both lines
	if (x < min(x1, x2) || x > max(x1, x2) ||
		x < min(x3, x4) || x > max(x3, x4)) return NULL;
	if (y < min(y1, y2) || y > max(y1, y2) ||
		y < min(y3, y4) || y > max(y3, y4)) return NULL;

	// Return the point of intersection
	SDL_Point ret;
	ret.x = x;
	ret.y = y;
	return 1 - newmap(distance(ret, p1), 0, distance(p2, p1), 0, 1);
}
string remove_zero(string str)
{


	bool found = str.find('.');
	if (found == std::string::npos)return str;

	// Count trailing zeros
	int i = str.length() - 1;
	while ((str[i] == '0' || str[i] == '.') && i>0) {

		if (str[i] == '.') {
			str.erase(str.begin() + i);
			break;
		}
		// The erase function removes i characters
		// from given index (0 here)
		str.erase(str.begin() + i);
		i--;
	}

	return str;
}

class Timer {
public:
	std::chrono::time_point<std::chrono::steady_clock> start;
	Timer() {
		start = std::chrono::steady_clock::now();
	}
	~Timer() {
		auto duration = std::chrono::duration_cast<std::chrono::milliseconds>
			(std::chrono::steady_clock::now() - start);
		cout << "Time taken: " << duration.count() << "ms\n";
	}
	auto get_time() {
		auto duration = std::chrono::duration_cast<std::chrono::milliseconds>
			(std::chrono::steady_clock::now() - start);
		return  duration.count();
	}
};

class Random {
public:
	static double random_value(int min, int max) {
		random_device rd;  //Will be used to obtain a seed for the random number engine
		mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
		uniform_real_distribution<> dis(min, max);
		return dis(gen);
	}
};
