#include "tfm/Clases_Tfm.h"
#include "ros/ros.h"
#include "geometry_msgs/PoseStamped.h"
#include "tfm/Funciones_Tfm.h"
#include <string>
#include <iostream>
#include <geometry_msgs/Twist.h>

using namespace std;
void local_point::setID(std::string x)
{
	ID = x;
}

void local_point::setPose(double x, double y, double angle)
{
	pose.header.stamp = ros::Time::now();
	pose.header.frame_id = "map";

	pose.pose.position.x = x;
	pose.pose.position.y = y;
	pose.pose.position.z = 0.0;
	pose.pose.orientation.x = 0.0;
	pose.pose.orientation.y = 0.0;
	pose.pose.orientation.z = sin(0.5*degrees2radians(angle));
	pose.pose.orientation.w = cos(0.5*degrees2radians(angle));
}

geometry_msgs::PoseStamped local_point::getPose()
{
	return pose;
}

std::string local_point::getID()
{
	return ID;
}

