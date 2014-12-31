#include <ros/ros.h>
#include <visualization_msgs/Marker.h>
#include <ardrone_autonomy/Navdata.h>
#include <iomanip> 

ros::Publisher marker_pub;
int count = 0;

// A callback function. Executed each time a new pose message arrives .
void poseMessageReceived ( const ardrone_autonomy::Navdata& msg) {
	ROS_INFO_STREAM("poseMessageReceived: " << count);
	count++;
	if(msg.tags_count > 0) 	
	{
		ROS_INFO_STREAM( "Tags Count = " << msg.tags_count << " X = " << msg.tags_xc[0] << ", Y = " << msg.tags_yc[0]);
	
	    visualization_msgs::Marker marker;
	    // Set the frame ID and timestamp.  See the TF tutorials for information on these.
	    marker.header.frame_id = "ardrone_base_frontcam";
	    marker.header.stamp = ros::Time::now();
	 
	    // Set the namespace and id for this marker.  This serves to create a unique ID
	    // Any marker sent with the same namespace and id will overwrite the old one
	    marker.ns = "basic_shapes";
	    marker.id = 0;
	 
	    // Set the marker type.  Initially this is CUBE, and cycles between that and SPHERE, ARROW, and CYLINDER
		marker.type = visualization_msgs::Marker::CUBE;
	 
	    // Set the marker action.  Options are ADD, DELETE, and new in ROS Indigo: 3 (DELETEALL)
	    marker.action = visualization_msgs::Marker::ADD;

		// Set the pose of the marker.  This is a full 6DOF pose relative to the frame/time specified in the header
		marker.pose.position.x = msg.tags_xc[0] / 10;
		marker.pose.position.y = msg.tags_yc[0] / 10;
		marker.pose.position.z = 0;
		marker.pose.orientation.x = 0.0;
		marker.pose.orientation.y = 0.0;
		marker.pose.orientation.z = 0.0;
		marker.pose.orientation.w = 1.0;
  
		// Set the scale of the marker -- 1x1x1 here means 1m on a side
		marker.scale.x = 0.5;
		marker.scale.y = 0.5;
		marker.scale.z = 0.5;
  
		// Set the color -- be sure to set alpha to something non-zero!
		marker.color.r = 0.0f;
		marker.color.g = 1.0f;
		marker.color.b = 0.0f;
		marker.color.a = 1.0;
  
		marker.lifetime = ros::Duration();
  
		// Publish the marker
		ROS_INFO_STREAM( "Publish visualization_marker");
		marker_pub.publish(marker);	
	}
}

int main(int argc, char **argv) 
{
	ros::init(argc, argv, "ardrone_followme");
	ros::NodeHandle nh;
	ROS_INFO_STREAM("Servus miteinand! Jetzt geht's los...");
		
	// Create a subscriber object .
	ros::Subscriber sub = nh.subscribe( "ardrone/navdata" , 1000 , &poseMessageReceived ) ;
	marker_pub = nh.advertise<visualization_msgs::Marker>("visualization_marker", 5, true);		
	// Let ROS take over.
 	ros::spin();
}
