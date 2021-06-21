#include<ros/ros.h>
#include<myworkcell_core/LocalizePart.h>
#include<tf/tf.h>
#include<moveit/move_group_interface/move_group_interface.h>
#include<moveit/planning_scene_interface/planning_scene_interface.h>


class ScanNPlan{
public:
    ScanNPlan(ros::NodeHandle& nh){
        vson_clnt = nh.serviceClient<myworkcell_core::LocalizePart>("localize_part");
    }

    void start(std::string& base_frame){
        myworkcell_core::LocalizePart srv;
        srv.request.base_frame = base_frame;

        moveit::planning_interface::MoveGroupInterface move_group("manipulator");
        move_group.setPoseReferenceFrame("base_frame");

        ROS_INFO_STREAM("Attempting to localize part in frame " << base_frame);

        if (!vson_clnt.call(srv)){
            ROS_ERROR("Could not localize part...");
            return;
        }
        else{
            move_target = srv.response.pose;
            ROS_INFO_STREAM("Part localized..." << srv.response);
            move_group.setPoseTarget(move_target);
            move_group.move();
        }
    }

private:
    ros::ServiceClient vson_clnt;
    geometry_msgs::Pose move_target;
};


int main(int argc, char *argv[])
{
    ros::init(argc, argv, "myworkcell_node");
    ros::AsyncSpinner async_spinner(1);
    async_spinner.start();
    ros::NodeHandle nh;
    

    ROS_INFO("Scan and plan started...");

    ros::NodeHandle prv_nh("~");
    std::string base_frame{""};
    prv_nh.param<std::string>("base_frame",base_frame, "world");

    ScanNPlan app(nh);
    ros::Duration(1).sleep();
    app.start(base_frame);
    ros::waitForShutdown();

    return 0;
}
