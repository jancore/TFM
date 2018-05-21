#include "ros/ros.h"
#include "geometry_msgs/Twist.h"
#include <tf/transform_datatypes.h>
#include "nav_msgs/Odometry.h"

ros::Publisher velocity_publisher;
ros::Subscriber pose_subscriber;
geometry_msgs::Pose turtlebot_pose;

using namespace std;

const double PI = 3.14159265359;

void move(double speed, double distance, bool isForward);
void rotate(double angular_speed, double angle, bool clockwise);
double degrees2radians(double angle_in_degrees);
void poseCallback(const nav_msgs::Odometry::ConstPtr& pose_message);
double getDistance(double x1, double y1, double x2, double y2);

int main(int argc, char **argv)
{
    ros::init(argc, argv, "odom_out_and_back_node");
    ros::NodeHandle n;
    
    double speed;
    double distance;
    bool isForward;
    
    double angular_speed;
    double angle;
    bool clockwise;
    int i=0;
    
    //velocity_publisher = n.advertise<geometry_msgs::Twist>("cmd_vel", 10);
    velocity_publisher = n.advertise<geometry_msgs::Twist>("mobile_base/commands/velocity", 10);
    pose_subscriber = n.subscribe("odom", 1000, poseCallback);
    
    ros::Rate loop_rate(100);
    
	while(i!=4)
	{	
		if(i==0)
		{
			cout << "ME MUEVO" << endl;
			move(0.10, 1, 1);
		}
		else if(i==1)
		{
			cout << "GIRO"<< endl;
			rotate(0.25, PI, 1);
		}
		else if(i==2)
		{
			cout << "ME MUEVO"<< endl;
			move(0.10, 1, 1);
		}
		else
		{
			cout << "GIRO"<< endl;
			rotate(0.25, PI, 1);
		}
		i++;
		ros::spinOnce();
		loop_rate.sleep();
	}
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
	current_distance = getDistance(turtlebot_pose.position.x, turtlebot_pose.position.y, x_start, y_start);
	ros::spinOnce();
        loop_rate.sleep();
    }while(current_distance < distance);
    
    vel_msg.linear.x = 0;
    velocity_publisher.publish(vel_msg);

}

void rotate(double angular_speed, double angle, bool clockwise)
{
    geometry_msgs::Twist vel_msg;
    tf::Quaternion q(turtlebot_pose.orientation.x, turtlebot_pose.orientation.y, turtlebot_pose.orientation.z, turtlebot_pose.orientation.w);

    tf::Matrix3x3 m(q);
    double roll, pitch, yaw;
    m.getRPY(roll, pitch, yaw);

    //double yaw = 2*atan2(turtlebot_pose.orientation.z, turtlebot_pose.orientation.w);
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

    tf::Quaternion q(turtlebot_pose.orientation.x, turtlebot_pose.orientation.y, turtlebot_pose.orientation.z, turtlebot_pose.orientation.w);

    tf::Matrix3x3 m(q);
    m.getRPY(roll, pitch, yaw);
   
    //yaw =  2*atan2(turtlebot_pose.orientation.z, turtlebot_pose.orientation.w);    
    double delta_angle = abs(yaw - last_angle);

    turn_angle += delta_angle;
    last_angle = yaw;
    cout << "Angulo girado: "<< turn_angle << endl;
    ros::spinOnce();
    loop_rate.sleep();
    }
    while(turn_angle < angle);

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

