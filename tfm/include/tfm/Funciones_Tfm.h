#ifndef FUNCIONES_TFM_H
#define FUNCIONES_TFM_H

#include "ros/ros.h"
#include "geometry_msgs/Twist.h"
#include <math.h>
#include <string>
#include <iostream>
#include <tfm/Clases_Tfm.h>
//#include "tfm/Clases_Tfm.cpp"

void move(double speed, double distance, bool isForward);

void rotate(double angular_speed, double angle, bool clockwise);

double degrees2radians(double angle_in_degrees);

double radians2degrees(double angle_in_radians);

double getDistance(double x1, double y1, double x2, double y2);

void localization();

void save_pose(std::string name, double x, double y, double angle);

int get_lines(std::string name);

local_point get_point(int n, std::string name);
    
#endif /* FUNCIONES_TFM_H */
