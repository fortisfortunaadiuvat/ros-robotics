#include <ros/ros.h>
#include <move_base_msgs/MoveBaseAction.h>
#include <actionlib/client/simple_action_client.h>

bool moveToGoal(double xGoal, double yGoal,double orientation);
char choose();

double x_A = 6.3123;
double y_A = -4.4526;
double yaw_A = -0.872665;

double x_B= 0.8100;
double y_B= 1.6451;
double yaw_B = 1.5708;

double x_C= -6.9616;
double y_C = -3.1483;
double yaw_C = -0.872665;

bool goalReached ;

 int main(int argc, char** argv){
   ros::init(argc, argv, "simple_navigation_goals");
   ros::NodeHandle n;
   ros::spinOnce();

   while(true){
     goalReached = moveToGoal(x_A, y_A,yaw_A);
     goalReached = moveToGoal(x_B, y_B,yaw_B);
     goalReached = moveToGoal(x_C, y_C,yaw_C);

   }
   return 0;
}

bool moveToGoal(double xGoal, double yGoal, double orientation){
   actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> ac("move_base", true);

   while(!ac.waitForServer(ros::Duration(5.0))){
      ROS_INFO("Waiting for the move_base action server to come up");
   }

   move_base_msgs::MoveBaseGoal goal;

   goal.target_pose.header.frame_id = "map";
   goal.target_pose.header.stamp = ros::Time::now();

   goal.target_pose.pose.position.x =  xGoal;
   goal.target_pose.pose.position.y =  yGoal;
   goal.target_pose.pose.position.z =  0.0;
   goal.target_pose.pose.orientation.x = 0.0;
   goal.target_pose.pose.orientation.y = 0.0;
   goal.target_pose.pose.orientation.z = orientation;
   goal.target_pose.pose.orientation.w = 1.0;

   ROS_INFO("Sending goal location ...");
   ac.sendGoal(goal);

   ac.waitForResult();

   if(ac.getState() == actionlib::SimpleClientGoalState::SUCCEEDED){
      ROS_INFO("You have reached the destination");
      return true;
   }
   else{
      ROS_INFO("The robot failed to reach the destination");
      return false;
   }
}
