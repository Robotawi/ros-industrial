#include <ros/ros.h>
#include <fake_ar_publisher/ARMarker.h>
#include <myworkcell_core/LocalizePart.h>

class Localizer{
public:

Localizer(ros::NodeHandle& nh){
  sub_ = nh.subscribe<fake_ar_publisher::ARMarker>("ar_pose_marker", 1, &Localizer::vision_callback, this);

  srv_ = nh.advertiseService("localize_part", &Localizer::localize_part, this);
}

void vision_callback(const fake_ar_publisher::ARMarkerConstPtr& msg){
  armrk_ = msg;
  // ROS_INFO_STREAM(armrk_->pose.pose.position);
}

bool localize_part(myworkcell_core::LocalizePart::Request& req, myworkcell_core::LocalizePart::Response& res){
  fake_ar_publisher::ARMarkerConstPtr p = armrk_;
  if (!p) return false;

  res.pose = p->pose.pose;
  return true;
}

private:
  ros::Subscriber sub_;
  fake_ar_publisher::ARMarkerConstPtr armrk_;
  ros::ServiceServer srv_;
};

int main(int argc, char *argv[])
{
   // This must be called before anything else ROS-related
  ros::init(argc, argv, "vision_node");

  // Create a ROS node handle
  ros::NodeHandle nh;
  Localizer loclz(nh);
  ROS_INFO("Vision node started...");
  ros::spin();
  return 0;
}
