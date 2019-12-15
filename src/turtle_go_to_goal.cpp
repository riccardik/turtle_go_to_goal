#include "ros/ros.h"
#include<math.h>

#include <geometry_msgs/Twist.h>
#include <turtlesim/Pose.h>

#include <sstream>

using namespace std;

float x_real = 0;
float y_real = 0;
float x_goal = 0;
float y_goal = 0;
float theta_real = 0;
int aligned=0;
float velx=0;
float vely=0;


float EuclidianDistance(float x_g, float y_g, float x_r, float y_r)
{
  return sqrt(pow((x_g - x_r),2) + pow((y_g - y_r),2));
}

float ecd(float x_g, float y_g, float x_r, float y_r)
{
  return sqrt(pow((x_g - x_r),2)*pow((y_g - y_r),2));
}

//% Manage here the callback

void getPositionCallback(const turtlesim::Pose& msg){
   //Using the callback function just for subscribing
   //Subscribing the message and storing it in 'linx' and 'angZ'
  x_real = msg.x;
  y_real = msg.y;
  theta_real = msg.theta;

}

float checkTheta1()
{
  float theta;
  theta=acos((-x_real+x_goal)/EuclidianDistance(x_goal,y_goal,x_real,y_real));
  return theta;
}

float checkTheta2()
{
  float theta;
  theta=asin((y_real-y_goal)/EuclidianDistance(x_goal,y_goal,x_real,y_real));
  return theta;
}


int main(int argc, char **argv)
{
  ros::init(argc, argv, "turtle_go_to_goal");
  ros::NodeHandle n;


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


  //calculate tetatarget
  float tetatarget=0;

  tetatarget=acos((x_goal*x_real+y_real*y_goal)/ecd(x_goal,y_goal,x_real,y_real));

  //tetatarget=atan2(y_goal,x_goal) - atan2(y_real,x_real);

  cout<<tetatarget<<"\t"<<theta_real<<endl;
  if ((x_goal*y_real-x_real*y_goal)<0)
    tetatarget=-tetatarget;

tetatarget=3.14/4;



  while (ros::ok())
  //rostopic echo  /turtle1/pose
  {
    // Send here the velocity command to the robot
    geometry_msgs::Twist msg;

    /*if (((checkTheta2()<theta_real+0.1 && checkTheta2()>theta_real-0.1) )&& aligned==0 && (checkTheta1()<theta_real+0.1 && checkTheta1()>theta_real-0.1))
    //if (atan2(y_goal-y_real,x_goal-x_real)!=theta_real)
    //if (sqrt(pow(x_goal-x_real, 2))!=sqrt(pow(y_goal-y_real, 2)))
    {

      msg.angular.z = 1;
    }
    else
      {
        msg.linear.x=-1;
        msg.linear.y=-1;
        msg.angular.z = 0;
        aligned=1;
      }*/




    if (aligned!=1)
    {
      //if (-checkTheta1()>theta_real+0.1 || -checkTheta1()<theta_real-0.1)
      //if (!((checkTheta2()<theta_real+0.1 && checkTheta2()>theta_real-0.1) && (checkTheta1()<theta_real+0.1 && checkTheta1()>theta_real-0.1)))
      if (!(tetatarget-0.05<theta_real && tetatarget+0.05>theta_real))
      //if (!(tetatarget-0.05<theta_real && tetatarget+0.05>theta_real) && !((EuclidianDistance(x_goal,y_goal,x_real,y_real)*cos(theta_real)<0 && EuclidianDistance(x_goal,y_goal,x_real,y_real)*sin(theta_real)>0)||(EuclidianDistance(x_goal,y_goal,x_real,y_real)*cos(theta_real)>0 && EuclidianDistance(x_goal,y_goal,x_real,y_real)*sin(theta_real)<0)) )
      //if (atan2(y_goal-y_real,x_goal-x_real)!=theta_real)
      //if (sqrt(pow(x_goal-x_real, 2))!=sqrt(pow(y_goal-y_real, 2)))
      {

        msg.angular.z = 1;
        if (x_real<x_goal)
          velx=cos(tetatarget);

        else
          velx=-cos(tetatarget);
        if (y_real<y_goal)
          vely=sin(tetatarget);
        else
          vely=-sin(tetatarget);
      }

      else
        aligned=1;


    }
    else
      {
        msg.linear.x=1;
        msg.linear.y=1;
        msg.angular.z = 0;
        aligned=1;
      }




      /*if(EuclidianDistance(x_goal,y_goal,x_real,y_real)<0.5)
      {  msg.linear.x=0;
        msg.linear.y=0;
      }*/



    //  cout<<checkTheta1()<<"\t"<<theta_real<<"\t"<<checkTheta2()<<"\t"<<aligned<<endl;
    cout<<tetatarget<<"\t"<<theta_real<<"\t"<<aligned<<endl;

    //cout<<x_real<<"\t"<<y_real<<"\t"<<theta_real<<endl;
      //cout<<x_real<<"\t"<<y_real<<"\t"<<x_goal<<"\t"<<y_goal<<endl;

  //  cout<<atan2(y_real-y_goal, x_real-x_goal)<<"\t"<<atan2(y_goal,x_goal)<<theta_real<<endl;

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
  aligned=0;
  tetatarget=0;
}


//rosrun turtle_go_to_goal turtlego
