#include "Stopper.h"
#include "geometry_msgs/Twist.h"

#include<stdlib.h>
#include<time.h>
#define random(x) (rand()%x)

Stopper::Stopper()
{
    keepMoving = true;
    keepMoving0=true;
    getRandom = false;
    // Advertise a new publisher for the robot~s velocity command topic
    commandPub = node.advertise<geometry_msgs::Twist>("/cmd_vel", 10);
    // Subscribe to the simulated robot~s laser scan topic
    laserSub = node.subscribe("/scan", 1, &Stopper::scanCallback, this);
}
// Send a velocity command
void Stopper::moveForward() 
{
    geometry_msgs::Twist msg; 
    msg.linear.x = FORWARD_SPEED;
    ROS_INFO("SPEED : % f", msg.linear.x);
    commandPub.publish(msg);
}

void Stopper::turnCorner()
{
    static geometry_msgs::Twist msg;
     
    if(keepMoving0==1 && keepMoving==0)
    getRandom = 1;
    else
    getRandom = 0;

    if(getRandom)   
    {
        //angular_velocity = (random(100)-50)/25.0;
        angular_velocity = 1.5708;
        msg.angular.z = angular_velocity;
        //zaman kavramı, odometri
        //is angle 90???
    }
    ROS_INFO("Angular velocity : % f", msg.angular.z);
    commandPub.publish(msg);
}

// Process the incoming laser scan message
void Stopper::scanCallback(const sensor_msgs::LaserScan::ConstPtr &scan)

{
    keepMoving0 = keepMoving;
    bool isObstacleInFront = false;
    keepMoving = true;
    static geometry_msgs::Twist msg;

    //solun sağın taranması??
    int minIndex = 360 + floor((MIN_SCAN_ANGLE - scan->angle_min) / scan->angle_increment);
    int maxIndex = floor((MAX_SCAN_ANGLE - scan->angle_min) / scan->angle_increment);

    std::vector<std::pair<float, float>> scaled_ranges;

    //indeksler arası kontrol.
    for (int i = minIndex; i < 360; i++)
        scaled_ranges.push_back(std::make_pair(i, scan->ranges[i]));
    for (int i = 0; i <= maxIndex; i++)
        scaled_ranges.push_back(std::make_pair(i, scan->ranges[i]));
    for (auto value : scaled_ranges)
    {
        if (value.second < MIN_DIST_FROM_OBSTACLE)
        {
            keepMoving = false;
            //taranılan açılarda engel var ise dur.
            msg.linear.x = 0;
            ROS_INFO("Turn a corner!");
            break;
        }
    }
}


void Stopper::startMoving()
{
    srand((int)time(0));

    ros::Rate rate(10);
    ROS_INFO("Start moving");
    // Keep spinning loop until user presses Ctrl+C or the robot got too close to an obstacle
    while (ros::ok())
    {
        // Need to call this function often to allow ROS to process incoming messages
        ros::spinOnce();
        
        if (false == keepMoving)
            turnCorner();
        else
            moveForward();
        
        rate.sleep();
    }
}

