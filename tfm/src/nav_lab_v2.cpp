/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "ros/ros.h"
#include "geometry_msgs/Twist.h"
#include "geometry_msgs/PoseStamped.h"
#include <tf/transform_datatypes.h>
#include "nav_msgs/Odometry.h"

ros::Publisher velocity_publisher;
ros::Publisher goal_publisher;
ros::Subscriber pose_subscriber;
geometry_msgs::Pose turtlebot_pose;

using namespace std;

const double PI = 3.14159265359;

//method to move the robot straight
void move(double speed, double distance, bool isForward);
void rotate(double angular_speed, double angle, bool clockwise);
double degrees2radians(double angle_in_degrees);
void poseCallback(const nav_msgs::Odometry::ConstPtr& pose_message);
double getDistance(double x1, double y1, double x2, double y2);

int main(int argc, char **argv)
{
    
    //Initiate new ROS node named "talker"
    ros::init(argc, argv, "nav_lab_v2");
    ros::NodeHandle n;

    int opt = 0;
    
    double angle;
    
    velocity_publisher = n.advertise<geometry_msgs::Twist>("mobile_base/commands/velocity", 10);
    goal_publisher = n.advertise<geometry_msgs::PoseStamped>("move_base_simple/goal", 10);
    pose_subscriber = n.subscribe("odom", 100, poseCallback);
    
    ros::Rate loop_rate(100);
    	
	//cout << "ME MUEVO" << endl;
	move(0.1, 0.3, 0);
	//cout << "GIRO"<< endl;
	rotate(0.5, 2*PI, 1);

	geometry_msgs::PoseStamped pose;
	geometry_msgs::Twist vel_msg;

	while(opt != 2)
	{
    		cout << "Elija una opción:" << endl << "(1) Navegar hacia una posición." << endl << "(2) Salir." << endl;
		cin >> opt;
		switch(opt)
		{
			case 1:
			{
				system("clear");
				pose.header.stamp = ros::Time::now();
				pose.header.frame_id = "map";
				cout << "Coordenada x: ";
				cin >> pose.pose.position.x;
				cout << "Coordenada y: ";
				cin >> pose.pose.position.y;
				cout << "Orientación en z (grados): ";
				cin >> angle;
				pose.pose.orientation.w = degrees2radians(angle);
				
				pose.pose.position.z = 0.0;
				pose.pose.orientation.x = 0.0;
				pose.pose.orientation.y = 0.0;
				pose.pose.orientation.z = 0.0;
				
				goal_publisher.publish(pose); 
        			ros::spinOnce();
        			loop_rate.sleep();
		
				break;
			}
			case 2:
				break;
			default:
			{
				system("clear");
				break;
			}
		}

	}
    //ros::spin();
    
    pose.pose.position.x = turtlebot_pose.position.x;
    pose.pose.position.y = turtlebot_pose.position.y;
    goal_publisher.publish(pose);	

    vel_msg.linear.x = 0;
    velocity_publisher.publish(vel_msg);		

    ros::shutdown();
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

void poseCallback(const nav_msgs::Odometry::ConstPtr & pose_message)
{
    turtlebot_pose.position.x = pose_message->pose.pose.position.x;
    turtlebot_pose.position.y = pose_message->pose.pose.position.y;
    turtlebot_pose.orientation = pose_message->pose.pose.orientation;
}


double getDistance(double x1, double y1, double x2, double y2)
{
    return sqrt(pow((x1-x2),2)+pow((y1-y2),2));
}

