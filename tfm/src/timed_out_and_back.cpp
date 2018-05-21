/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "ros/ros.h"
#include "geometry_msgs/Twist.h"
#include "turtlesim/Pose.h"

ros::Publisher velocity_publisher;
ros::Publisher turtlebot_velocity;	//publicador para el topic de velocidad del turtlebot.
ros::Subscriber pose_subscriber;
turtlesim::Pose turtlesim_pose;

using namespace std;

const double PI = 3.14159265359;

//method to move the robot straight
void move(double speed, double distance, bool isForward);
void rotate(double angular_speed, double angle, bool clockwise);
double degrees2radians(double angle_in_degrees);
double getDistance(double x1, double y1, double x2, double y2);

int main(int argc, char **argv)
{
    
    //Initiate new ROS node named "talker"
    ros::init(argc, argv, "timed_out_and_back_node");
    ros::NodeHandle n;
    
    double speed;
    double distance;
    bool isForward;
    
    double angular_speed;
    double angle;
    bool clockwise;
    
    velocity_publisher = n.advertise<geometry_msgs::Twist>("/cmd_vel", 10);
    turtlebot_velocity = n.advertise<geometry_msgs::Twist>("/mobile_base/commands/velocity", 10);
	
	move(0.15, 1, 1);
	rotate(0.5, PI, 1);
	move(0.15, 1, 1);
	rotate(0.5, PI, 1);

    //ros::Rate loop_rate(0.5);    
    
    //ros::spin();
    
    return 0;
    
}

void move(double speed, double distance, bool isForward)
{
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
	turtlebot_velocity.publish(vel_msg);	//Publicar velocidad tanto para el simulador como para el Turtlebot real.
        double t1 = ros::Time::now().toSec();
        current_distance = speed *(t1-t0);
        //ros::spinOnce();
        //loop_rate.sleep();        
    }while(current_distance < distance);
    
    vel_msg.linear.x = 0;
    velocity_publisher.publish(vel_msg);
    turtlebot_velocity.publish(vel_msg);
}

void rotate(double angular_speed, double angle, bool clockwise)
{
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
    turtlebot_velocity.publish(vel_msg);	//Publicar velocidad tanto para el simulador como para el Turtlebot real.
    double t1 = ros::Time::now().toSec();
    current_angle = angular_speed * (t1 -t0);
    ros::spinOnce();
    loop_rate.sleep();
    }
    while(current_angle<angle);

    vel_msg.angular.z = 0;
    velocity_publisher.publish(vel_msg);
    turtlebot_velocity.publish(vel_msg);        
}

double degrees2radians(double angle_in_degrees)
{
    return angle_in_degrees * PI / 180.0;
}

double getDistance(double x1, double y1, double x2, double y2)
{
    return sqrt(pow((x1-x2),2)+pow((y1-y2),2));
}

