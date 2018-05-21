#include "tfm/Funciones_Tfm.h"
#include "ros/ros.h"
#include "geometry_msgs/Twist.h"
#include "tfm/Clases_Tfm.h"
//#include "tfm/Clases_Tfm.cpp"
#include <math.h>
#include <string>
#include <cstdlib>
#include <iostream>
#include <fstream>

using namespace std;

const double PI = 3.14159265359;

void move(double speed, double distance, bool isForward)
{
    ros::Publisher velocity_publisher;
    ros::NodeHandle n2;

    velocity_publisher = n2.advertise<geometry_msgs::Twist>("mobile_base/commands/velocity", 10);
    geometry_msgs::Twist vel_msg;
    //distance = speed * time
    
    //set a random linear velocity in the x-axis
    if(isForward)
        vel_msg.linear.x = abs(speed);
    else
        vel_msg.linear.x = -abs(speed);
    vel_msg.linear.y = 0;
    vel_msg.linear.z = 0;
    
    //set a random angular velocity in the y-axis
    vel_msg.angular.x = 0;
    vel_msg.angular.y = 0;
    vel_msg.angular.z = 0;
    
    //t0: current time
    double t0 = ros::Time::now().toSec();
    double current_distance = 0;
    ros::Rate loop_rate(100);
    do
    {
        velocity_publisher.publish(vel_msg);
        double t1 = ros::Time::now().toSec();
        current_distance = speed *(t1-t0);
        ros::spinOnce();
        loop_rate.sleep();        
    }while(current_distance < distance);
    
    vel_msg.linear.x = 0;
    velocity_publisher.publish(vel_msg);
}

void rotate(double angular_speed, double angle, bool clockwise)
{
    ros::Publisher velocity_publisher;
    ros::NodeHandle n2;

    velocity_publisher = n2.advertise<geometry_msgs::Twist>("mobile_base/commands/velocity", 10);
        geometry_msgs::Twist vel_msg;
    //set a random linear velocity in the x-axis
    vel_msg.linear.x = 0;
    vel_msg.linear.y = 0;
    vel_msg.linear.z = 0;
    //set a random angular velocity in the y-axis
    vel_msg.angular.x = 0;
    vel_msg.angular.y = 0;
    
    if(clockwise)
        vel_msg.angular.z = -abs(angular_speed);
    else
        vel_msg.angular.z = abs(angular_speed);
    
    double current_angle = 0.0;
    double t0 = ros::Time::now().toSec();
    ros::Rate loop_rate(100);
    
    do
    {
    velocity_publisher.publish(vel_msg);
    double t1 = ros::Time::now().toSec();
    current_angle = angular_speed * (t1 -t0);
    ros::spinOnce();
    loop_rate.sleep();
    }
    while(current_angle<angle);

    vel_msg.angular.z = 0;
    velocity_publisher.publish(vel_msg);  
}

double degrees2radians(double angle_in_degrees)
{
    return angle_in_degrees * PI / 180.0;
}

double radians2degrees(double angle_in_radians)
{
    return angle_in_radians * 180.0 / PI;
}

double getDistance(double x1, double y1, double x2, double y2)
{
    return sqrt(pow((x1-x2),2)+pow((y1-y2),2));
}

void localization()
{
	//cout << "ME MUEVO" << endl;
	move(0.1, 0.3, 0);
	//cout << "GIRO"<< endl;
	rotate(0.5, 4*PI, 1);
}

void save_pose(std::string name, double x, double y, double angle)
{
	std::string file;
	
	cout << "Nombre del archivo: " << endl;
	cin >> file;
	file = "/home/jan/catkin_ws/src/tfm/points/" + file + ".txt";
	ofstream MyFile;
	MyFile.open(file.c_str(), ios::app);
	
	if(MyFile.is_open())
	{
		MyFile << name << " " << x << " " << y << " " << angle << endl;
	}
	else
	{
		cout << "Error: Alga ha ido mal. El archivo no se ha abierto correctamente." << endl;
	}
	MyFile.close();
}

int get_lines(std::string name)
{
	int n=0;
	std:string dato;
	name = "/home/jan/catkin_ws/src/tfm/points/" + name + ".txt";
	ifstream MyFile;
	MyFile.open(name.c_str());

	if(MyFile.is_open())
	{
		while(getline(MyFile,dato))
                {
                    n++;
                }
	}
	else
	{
		cout << "Error: Algo ha ido mal. El archivo no se ha abierto correctamente." << endl;
	}
	MyFile.close();
	return n;
}

local_point get_point(int n, std::string name)
{
	local_point point;
	std::string linea, dato, ID;
	int i=0;
	int j=0;
	std::string x_s, y_s, angle_s;
	double x_n, y_n, angle_n;

	name = "/home/jan/catkin_ws/src/tfm/points/" + name + ".txt";
	ifstream MyFile;
	MyFile.open(name.c_str());

	if(MyFile.is_open())
	{
		while(i<n)
		{
			getline(MyFile,linea);
			i++;
		}

		for(i=0 ; i<linea.size(); i++)
		{
			if (linea[i] != ' ' && j==0)
			{
				ID = ID + linea[i];
			}
			if (linea[i] != ' ' && j==1)
			{
				x_s = x_s + linea[i];
			}
			if (linea[i] != ' ' && j==2)
			{
				y_s = y_s + linea[i];
			}
			if (linea[i] != ' ' && j==3)
			{
				angle_s = angle_s + linea[i];
			}

			if (linea[i] == ' ') j++;
		}
		
		x_n = atof(x_s.c_str());
		y_n = atof(y_s.c_str());
		angle_n = atof(angle_s.c_str());

		point.setID(ID);
		point.setPose(x_n, y_n, angle_n);
	}
	else
	{
		cout << "Error: Algo ha ido mal. El archivo no se ha abierto correctamente." << endl;
	}
	MyFile.close();
	return point;
}
