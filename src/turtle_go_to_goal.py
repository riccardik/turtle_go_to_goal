#!/usr/bin/env python

import rospy, math

x_real = 0
y_real = 0

def EuclidianDistance(x_g,y_g,x_r,y_r):
  return math.sqrt(math.pow((x_g-x_r),2) + math.pow((y_g-y_r),2))

# Manage the callback

#


def controller():
  rate = rospy.Rate(30)
  
  print("Insert the coordinates that the robot should reach")
  x_goal = input("Insert X")
  y_goal = input("Insert y")
  
  counter = 0
  
  # Manage here the publisher and the subscriber
  
  #
  
  while not rospy.is_shutdown():
    # Send here the velocity command to the robot
    
    #
    
    counter = counter+1
    if counter >= 900:
      break
    if EuclidianDistance(x_goal,y_goal,x_real,y_real)<0.5:
      break
  print("The process has finished in " + str(counter) + " iterations")
  print("The position error is " + str(EuclidianDistance(x_goal,y_goal,x_real,y_real)))

if __name__ == '__main__':
    try:
        controller()
    except rospy.ROSInterruptException:
        pass
