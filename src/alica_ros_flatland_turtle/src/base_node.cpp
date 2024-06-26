#include <chrono>
#include <fstream>
#include <ros/ros.h>
#include <signal.h>
#include <sstream>
#include <thread>

#include <alica_ros_flatland/base.hpp>
#include <flatland_msgs/DeleteModel.h>

void killMyTurtle(const std::string& name, ros::NodeHandle& privNh)
{
    ros::ServiceClient killClient = privNh.serviceClient<flatland_msgs::DeleteModel>("/delete_model");
    flatland_msgs::DeleteModel killSrv;
    killSrv.request.name = name;
    if (killClient.waitForExistence() && killClient.call(killSrv)) {
        ROS_INFO_STREAM(name << " was killed");
    } else {
        ROS_INFO_STREAM(name << " does not exist or failed to kill " << name);
    }
}

int main(int argc, char** argv)
{
    ros::init(argc, argv, "alica_turtle_base_node");
    ROS_INFO("Started Turtle Base Node.");
    std::string name, roleset, masterPlan, turtlesimRosPath, turtlesimLibPath, placeholderMappingFilePath;
    int agentId;

    ros::NodeHandle nh, privNh("~");
    privNh.getParam("name", name);
    privNh.getParam("roleset", roleset);
    privNh.getParam("master_plan", masterPlan);
    privNh.getParam("placeholder_mapping", placeholderMappingFilePath);
    privNh.getParam("turtlesim_ros_path", turtlesimRosPath);
    privNh.getParam("turtlesim_lib_path", turtlesimLibPath);
    privNh.getParam("agent_id", agentId);

    ROS_INFO_STREAM("HostName    : " << name);
    ROS_INFO_STREAM("Roleset     : " << (roleset.empty() ? "Default" : roleset));
    ROS_INFO_STREAM("Master Plan : " << masterPlan);
    ROS_INFO_STREAM("Mapping     : " << placeholderMappingFilePath);
    ROS_INFO_STREAM("ROS Turtlesim lib path  : " << turtlesimRosPath);
    ROS_INFO_STREAM("Base Turtlesim lib path  : " << turtlesimLibPath);
    ROS_INFO_STREAM("Agent ID    : " << agentId);

    if (masterPlan.size() == 0) {
        ROS_ERROR_STREAM("Master plan or roleset location is not available");
        return 0;
    }

    // kill turtle if same name turtle already exist
    killMyTurtle(name, privNh);

    std::optional<std::string> maybeMapping;
    if (!placeholderMappingFilePath.empty()) {
        std::ifstream mappingFileStream(placeholderMappingFilePath);
        if (mappingFileStream) {
            std::ostringstream ss;
            ss << mappingFileStream.rdbuf();
            maybeMapping.emplace(ss.str());
        } else {
            ROS_ERROR_STREAM("Could not read the placeholder mapping file");
            return 1;
        }
    }
    turtlesim::Base base(nh, privNh, name, agentId, roleset, masterPlan, {turtlesimRosPath, turtlesimLibPath}, maybeMapping);

    ROS_INFO("Starting ALICA turtle Base.......");
    base.start();

    while (ros::ok()) {
        ros::Rate(10).sleep();
    }

    // Ensure turtle is no longer visible
    killMyTurtle(name, privNh);
    return 0;
}
