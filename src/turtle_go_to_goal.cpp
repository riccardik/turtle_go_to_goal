#include "ros/ros.h"
#include<math.h>

#include <geometry_msgs/Twist.h>
#include <turtlesim/Pose.h>

#include <sstream>

using namespace std;

float x_real = 0;
float y_real = 0;


float EuclidianDistance(float x_g, float y_g, float x_r, float y_r)
{
  return sqrt(pow((x_g - x_r),2) + pow((x_g - x_r),2));
}

//% Manage here the callback

void getPositionCallback(const turtlesim::Pose& msg){
   //Using the callback function just for subscribing
   //Subscribing the message and storing it in 'linx' and 'angZ'
  x_real = msg.x;
  y_real = msg.y;
}


int main(int argc, char **argv)
{
  ros::init(argc, argv, "turtle_go_to_goal");
  ros::NodeHandle n;
  float x_goal = 0;
  float y_goal = 0;

  cout << "Insert X and Y coordinates that the robot should reach" <<endl;
  cin >> x_goal >> y_goal;

  ros::Rate loop_rate(30);
  int counter = 0;
  //rostopic echo  /turtle1/pose

  //Manage here the publisher and the subscriber
  ros::Subscriber sub = n.subscribe("/turtle1/pose", 1000, getPositionCallback);
  ros::Publisher pub = n.advertise<geometry_msgs::Twist>("/turtle1/cmd_vel", 1000);

  //subscribe to the topic pose
  //rostopic liste
  // rostopic info /turtle1/cmd_vel
  //rosmsg show to get how a message works

  while (ros::ok())
  //rostopic echo  /turtle1/pose
  {
    // Send here the velocity command to the robot
    geometry_msgs::Twist msg;
    msg.linear.x=2;
    msg.angular.z = 1;


    


    //publish message
    pub.publish(msg);
    loop_rate.sleep();
    ros::spinOnce();

    // Exit conditions
    counter++;
    if(counter>=900)
    {
      break;
    }
    if(EuclidianDistance(x_goal,y_goal,x_real,y_real)<0.5)
    {
      break;
    }
  }
  cout << "The process has finished in " << counter << " iterations"<<endl;
  cout << "The position error is " << EuclidianDistance(x_goal,y_goal,x_real,y_real) <<endl;
}
