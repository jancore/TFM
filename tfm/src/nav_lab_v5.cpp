/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
#include <stdlib.h> 
#include <string>
#include <iostream>
#include <math.h>
#include "ros/ros.h"
#include "geometry_msgs/Twist.h"
#include "geometry_msgs/PoseStamped.h"
#include "geometry_msgs/PoseWithCovarianceStamped.h"
#include "nav_msgs/Odometry.h"
#include "tfm/Clases_Tfm.h"
#include "tfm/Clases_Tfm.cpp"
#include "tfm/Funciones_Tfm.h"
#include "tfm/Funciones_Tfm.cpp"

ros::Publisher velocity_publisher;
ros::Publisher goal_publisher;
ros::Subscriber pose_subscriber;
geometry_msgs::Pose turtlebot_pose;
void poseCallback(const geometry_msgs::PoseWithCovarianceStamped::ConstPtr & pose_message);

using namespace std;

//const int N = 6;

int main(int argc, char **argv)
{
    
    //Initiate new ROS node named "talker"
    ros::init(argc, argv, "nav_lab_v5");
    ros::NodeHandle n;

    velocity_publisher = n.advertise<geometry_msgs::Twist>("mobile_base/commands/velocity", 10);
    goal_publisher = n.advertise<geometry_msgs::PoseStamped>("move_base_simple/goal", 10);
    pose_subscriber = n.subscribe("amcl_pose", 100, poseCallback);

    int opt = 0;
    int i;
    int j=0;
    int N;
    double angle;
    char c;
    bool check =  false;
    std::string name;
    local_point *points;
    
    ros::Rate loop_rate(100);

	localization();

	geometry_msgs::PoseStamped pose;
	geometry_msgs::Twist vel_msg;

	pose.header.stamp = ros::Time::now();
	pose.header.frame_id = "map";

	while(opt != 5)
	{
		system("clear");
    		cout << "Elija una opción:" << endl << "(1) Navegar hacia una posición." << endl << "(2) Navegar a un sitio predefinido." <<  endl <<"(3) Teleoperación." << endl << "(4) Cargar puntos." << endl <<"(5) Salir." << "Número de puntos: " << j << endl;
		cin >> opt;
		switch(opt)
		{
			case 1:
			{
				system("clear");
				cout << "Coordenada x: ";
				cin >> pose.pose.position.x;
				cout << "Coordenada y: ";
				cin >> pose.pose.position.y;
				cout << "Orientación en z (grados): ";
				cin >> angle;
				pose.pose.orientation.z = sin(0.5*degrees2radians(angle));
				pose.pose.orientation.w = cos(0.5*degrees2radians(angle));
								
				pose.pose.position.z = 0.0;
				pose.pose.orientation.x = 0.0;
				pose.pose.orientation.y = 0.0;
				
				
				goal_publisher.publish(pose); 
        			ros::spinOnce();
        			loop_rate.sleep();
		
				break;
			}
			case 2:
			{
				if(check)
				{
					while(opt != N)
					{
						system("clear");
						cout << "Elija una opción:" << endl;
						for(i=0; i<N; i++)
						{
							cout << "(" << i+1 << ") " << points[i].getID() << "." << endl;
						}
						cout << "(" << i+1 << ") Atrás." << endl;
						cin >> opt;
						--opt;
						if(opt >= 0 && opt < N)
							goal_publisher.publish(points[opt].getPose());
						ros::spinOnce();
						loop_rate.sleep();
					}
				}
				else
				{
					cout << "No hay puntos cargados" << endl;
					system("pause");
				}
				break;
					
			}
			case 3:
			{	
				system("xterm -e sh -c \"roslaunch tfm joystick.launch; bash\"");
				do
				{	
					system("clear");
					cout << "-¿Guardar posición? (g)." << endl;
					cout << "-¿Salir? (s)." << endl;
					cin >> c;
					switch(c)
					{
						case 'g':
						{
							cout << "Nombre de la posición: " << endl;
							cin >> name;
							save_pose(name, turtlebot_pose.position.x, turtlebot_pose.position.y, radians2degrees(2.0*asin(turtlebot_pose.orientation.z)));
							break;
						}
						case 's':
						{
							break;
						}
						default:
						{
							system("clear");
							break;
						}
					}
				}
				while(c != 's');
				//system("exit");
				break;
			}
			case 4:
			{								
				cout << "Nombre del archivo: " << endl;
				cin >> name;				
				N =  get_lines(name);

				points = new local_point[N];

				for(i=0; i<N; i++)
				{
					points[i] = get_point((i+1), name);
				}
				check = true;
				break;	
			}
			case 5:
				break;
			default:
			{
				system("clear");
				break;
			}
		}

	}
    //ros::spin();
    
    pose.pose = turtlebot_pose;
    goal_publisher.publish(pose);	

    vel_msg.linear.x = 0;
    velocity_publisher.publish(vel_msg);		

    ros::shutdown();
    delete [] points;
    return 0;
}

void poseCallback(const geometry_msgs::PoseWithCovarianceStamped::ConstPtr & pose_message)
{
    turtlebot_pose.position.x = pose_message->pose.pose.position.x;
    turtlebot_pose.position.y = pose_message->pose.pose.position.y;
    turtlebot_pose.orientation = pose_message->pose.pose.orientation;
}
