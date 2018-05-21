#ifndef CLASES_TFM_H
#define CLASES_TFM_H

#include <ros/ros.h>
#include "geometry_msgs/PoseStamped.h"
#include <string>
#include <iostream>
#include <geometry_msgs/Twist.h>

using namespace std;

class local_point
{
	public:
		void setID(std::string s);
		void setPose(double x, double y, double angle);
		std::string getID();
		geometry_msgs::PoseStamped getPose();
	private:
		std::string ID;
		geometry_msgs::PoseStamped pose;
		
	protected:
};

#endif // CLASES_TFM_H
