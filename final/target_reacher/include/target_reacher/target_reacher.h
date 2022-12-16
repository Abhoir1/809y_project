#pragma once

#include <rclcpp/rclcpp.hpp>
#include <geometry_msgs/msg/twist.hpp>
#include <string>
#include "bot_controller/bot_controller.h"
#include "ros2_aruco_interfaces/msg/aruco_markers.hpp"
#include "tf2_ros/static_transform_broadcaster.h"

// timer
class TargetReacher : public rclcpp::Node
{
public:
    TargetReacher(std::shared_ptr<BotController> const &bot_controller) : Node("target_reacher")
    {

        m_bot_controller = bot_controller;
        bool i = false;
        
        auto goal_x = this->declare_parameter<double>("aruco_target.x");
        auto goal_y = this->declare_parameter<double>("aruco_target.y");

        auto frame_id = this->declare_parameter<std::string>("final_destination.frame_id");

        auto aruco_0_x = this->declare_parameter<double>("final_destination.aruco_0.x");
        auto aruco_0_y = this->declare_parameter<double>("final_destination.aruco_0.y");

        auto aruco_1_x = this->declare_parameter<double>("final_destination.aruco_1.x");
        auto aruco_1_y = this->declare_parameter<double>("final_destination.aruco_1.y");

        auto aruco_2_x = this->declare_parameter<double>("final_destination.aruco_2.x");
        auto aruco_2_y = this->declare_parameter<double>("final_destination.aruco_2.y");

        auto aruco_3_x = this->declare_parameter<double>("final_destination.aruco_3.x");
        auto aruco_3_y = this->declare_parameter<double>("final_destination.aruco_3.y");

        m_bot_controller->set_goal(goal_x,goal_y);

        cmd_vel_publisher = this->create_publisher<geometry_msgs::msg::Twist>("/robot1/cmd_vel", 10);

        goal_reached_subscriber = this->create_publisher<std_msgs::msg::Bool>("/goal_reached", 10, std::bind(&TargetReacher::timer_callback, this, std::placeholders::_1));

        aruco_subscriber = this->create_subscription<ros2_aruco_interfaces::msg::ArucoMarkers>("/aruco_markers", 10, std::bind(&TargetReacher::cb1, this, std::placeholders::_1));

        final_destination_broadcaster =
            std::make_shared<tf2_ros::StaticTransformBroadcaster>(this);

        m_timer = this->create_wall_timer(1s, std::bind(&TargetReacher::cb2, this));
        tf_buffer = std::make_unique<tf2_ros::Buffer>(this->get_clock());
        tf_listener = std::make_shared<tf2_ros::TransformListener>(*tf_buffer);


    }

private:
    // attributes
    std::shared_ptr<BotController> m_bot_controller;
    rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr cmd_vel_publisher;
    rclcpp::Subscription<std_msgs::msg::Bool>::SharedPtr goal_reached_subscriber;
    rclcpp::Subscription<ros2_aruco_interfaces::msg::ArucoMarkers>::SharedPtr aruco_subscriber;
    
    std::shared_ptr<tf2_ros::StaticTransformBroadcaster> final_destination_broadcaster;
    std::shared_ptr<tf2_ros::TransformListener> tf_listener;
    rclcpp::TimerBase::SharedPtr m_timer;

    std::unique_ptr<tf2_ros::Buffer> tf_buffer;
    std::string frame_id;
    bool i;
    double aruco_0_x;
    double aruco_0_y;

    double aruco_1_x;
    double aruco_1_y;

    double aruco_2_x;
    double aruco_2_y;

    double aruco_3_x;
    double aruco_3_y;

    void final_destination(int k);

    void cb1(const std::shared_ptr<ros2_aruco_interfaces::msg::ArucoMarkers> aruco);

    //callback for goal reached topic
    void timer_callback(const std::shared_ptr<std_msgs::msg::Bool> msg);

    void cb2();
};