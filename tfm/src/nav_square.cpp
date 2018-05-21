/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "ros/ros.h"
#include "geometry_msgs/Twist.h"
#include <tf/transform_datatypes.h>
#include "nav_msgs/Odometry.h"

ros::Publisher velocity_publisher;
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
    ros::init(argc, argv, "nav_square_node");
    ros::NodeHandle n;
    
    double speed;
    double distance;
    bool isForward;
    
    double angular_speed;
    double angle;
    bool clockwise;
    
    velocity_publisher = n.advertise<geometry_msgs::Twist>("mobile_base/commands/velocity", 10);
    pose_subscriber = n.subscribe("odom", 100, poseCallback);
    
    ros::Rate loop_rate(100);
    
	move(0.15, 1, 1);
	rotate(0.5, PI/2.0, 1);
	move(0.15, 1, 1);
	rotate(0.5, PI/2.0, 1);
	move(0.15, 1, 1);
	rotate(0.5, PI/2.0, 1);
	move(0.15, 1, 1);
	rotate(0.5, PI/2.0, 1);
    
    //ros::spin();
    
    return 0;
    
}

void move(double speed, double distance, bool isForward)
{
    geometry_msgs::Twist vel_msg;
    double x_start = turtlebot_pose.position.x;
    double y_start = turtlebot_pose.position.y;

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
    double current_distance = 0;
    ros::Rate loop_rate(100);
    do
    {
        velocity_publisher.publish(vel_msg);
        ros::spinOnce();
        loop_rate.sleep();        
        current_distance = getDistance(turtlebot_pose.position.x, turtlebot_pose.position.y, x_start, y_start);
    }while(current_distance < distance);
    
    vel_msg.linear.x = 0;
    velocity_publisher.publish(vel_msg);

}

void rotate(double angular_speed, double angle, bool clockwise)
{
    geometry_msgs::Twist vel_msg;
    tf::Quaternion q(turtlebot_pose.orientation.x, turtlebot_pose.orientation.y, turtlebot_pose.orientation.z, 				turtlebot_pose.orientation.w);

    tf::Matrix3x3 m(q);
    double roll, pitch, yaw;
    m.getRPY(roll, pitch, yaw);

    double last_angle = yaw;
    double turn_angle = 0.0;

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

    ros::Rate loop_rate(100);
    
    do
    {
    velocity_publisher.publish(vel_msg);
    ros::spinOnce();
    loop_rate.sleep();

    tf::Quaternion q(turtlebot_pose.orientation.x, turtlebot_pose.orientation.y, turtlebot_pose.orientation.z, 		turtlebot_pose.orientation.w);

    tf::Matrix3x3 m(q);
    m.getRPY(roll, pitch, yaw);    
    double delta_angle = abs(yaw - last_angle);

    turn_angle += delta_angle;
    last_angle = yaw;
    cout << "Angulo girado: "<< turn_angle << endl;
    }
    while(turn_angle /*+ degrees2radians(1.0)*/ < angle);

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

