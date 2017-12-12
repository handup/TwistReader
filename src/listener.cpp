#include "ros/ros.h"
#include "geometry_msgs/Twist.h"
#include <vector>
#include <ecl/time.hpp>
#include <functional>

using namespace std;

  vector<geometry_msgs::Twist> vectorOfTwists;
  ros::Time begin;

  bool isMoving(geometry_msgs::Twist twist){
    // linear and angular speeds may be different from zero
    return (twist.linear.x || twist.linear.y || twist.linear.z ||
           twist.angular.x || twist.angular.y || twist.angular.z);

  }

  void chatterCallback(const geometry_msgs::Twist msg) {

    bool moving = isMoving(msg);
    if(moving){
      vectorOfTwists.push_back(msg);
    }
    else
    {
      geometry_msgs::Twist sum;

      for (std::vector<geometry_msgs::Twist>::iterator attack = vectorOfTwists.begin();
                attack != vectorOfTwists.end();
                ++attack)
          {
            geometry_msgs::Twist current = *attack;
            sum.linear.x += current.linear.x;
            sum.linear.y += current.linear.y;
            sum.linear.z += current.linear.z;
            sum.angular.x += current.angular.x;
            sum.angular.y += current.angular.y;
            sum.angular.z += current.angular.z;
          }
      if(!vectorOfTwists.size()){

      geometry_msgs::Twist mean;
      int size = vectorOfTwists.size();

      mean.linear.x = sum.linear.x/size;
      mean.linear.y = sum.linear.y/size;
      mean.linear.z = sum.linear.z/size;
      mean.angular.x = sum.angular.x/size;
      mean.angular.y = sum.angular.y/size;
      mean.angular.z = sum.angular.z/size;

      ROS_INFO("Average Linear x ,y , z: \n%4.2f, %4.2f, %4.2f \n",
                mean.linear.x, mean.linear.y, mean.linear.z);
      ROS_INFO("Average Angular x ,y , z: \n%4.2f, %4.2f, %4.2f \n",
                mean.angular.x, mean.angular.y, mean.angular.z);
      ROS_INFO("Sum Linear x ,y , z: \n%4.2f, %4.2f, %4.2f \n",
                sum.linear.x, sum.linear.y, sum.linear.z);
      ROS_INFO("Sum Angular x ,y , z: \n%4.2f, %4.2f, %4.2f \n",
                sum.angular.x, sum.angular.y, sum.angular.z);
      //ROS_INFO(begin - ros::Time::now());
      begin = ros::Time::now();
      vectorOfTwists.clear();
              }
    }
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "listener");
  ros::Time::init();

  begin = ros::Time::now();
  /**
   * NodeHandle is the main access point to communications with the ROS system.
   * The first NodeHandle constructed will fully initialize this node, and the last
   * NodeHandle destructed will close down the node.
   */
  ros::NodeHandle n;

  ros::Subscriber sub = n.subscribe("/mobile_base_controller/cmd_vel", 1000, chatterCallback);

  /**
   * ros::spin() will enter a loop, pumping callbacks.  With this version, all
   * callbacks will be called from within this thread (the main one).  ros::spin()
   * will exit when Ctrl-C is pressed, or the node is shutdown by the master.
   */
  ros::spin();

  return 0;
}
