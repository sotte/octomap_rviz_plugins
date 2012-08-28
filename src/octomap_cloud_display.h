/*
 * Copyright (c) 2009, Willow Garage, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the Willow Garage, Inc. nor the names of its
 *       contributors may be used to endorse or promote products derived from
 *       this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef RVIZ_OCTOMAP_CLOUD_DISPLAY_H
#define RVIZ_OCTOMAP_CLOUD_DISPLAY_H


#include <boost/shared_ptr.hpp>
#include <boost/thread/mutex.hpp>

#include <ros/ros.h>
#include <message_filters/subscriber.h>
#include <tf/message_filter.h>
#include <ros/spinner.h>
#include <ros/callback_queue.h>

#include <image_geometry/pinhole_camera_model.h>

#include "rviz/properties/bool_property.h"
#include "rviz/properties/ros_topic_property.h"
#include "rviz/properties/enum_property.h"
#include "rviz/properties/float_property.h"
#include "rviz/properties/int_property.h"

#include <rviz/display.h>

#include "rviz/ogre_helpers/point_cloud.h"

#if ROS_VERSION_MINIMUM(1,8,0) // test for Fuerte (newer PCL)
  #include <octomap_msgs/OctomapBinary.h>
  #include <octomap_msgs/GetOctomap.h>
  #include <octomap_msgs/BoundingBoxQuery.h>
#else
  #include <octomap_ros/OctomapBinary.h>
  #include <octomap_ros/GetOctomap.h>
  #include <octomap_ros/ClearBBXRegion.h>
#endif

#include <octomap_ros/OctomapROS.h>
#include <octomap/OcTreeKey.h>

using namespace rviz;

namespace octomap_rviz_plugin
{

/**
 * \class OctomapCloudDisplay
 *
 */
class OctomapCloudDisplay : public rviz::Display
{
Q_OBJECT
public:
  OctomapCloudDisplay();
  virtual ~OctomapCloudDisplay();

  // Initialize
  virtual void onInitialize();

  // Overrides from Display
  virtual void update(float wall_dt, float ros_dt);
  virtual void reset();

private Q_SLOTS:
  // rviz property slots
  void updateQueueSize();
  void updateTopic();
  void updateTreeDepth();


protected:
  void incomingMessageCallback(const octomap_msgs::OctomapBinaryConstPtr& msg);

  void setColor( double z_pos, double min_z, double max_z, double color_factor, PointCloud::Point& point);

  // overrides from Display
  virtual void onEnable();
  virtual void onDisable();

  virtual void fixedFrameChanged();

  void subscribe();
  void unsubscribe();

  void clear();

  typedef std::vector<PointCloud::Point> VPoint;
  typedef std::vector<VPoint> VVPoint;

  // point buffer
  VVPoint newPoints_;
  VVPoint pointBuf_;
  double boxSizes_[16];
  bool newPointsReceived_;

  uint32_t messages_received_;

  boost::mutex mutex_;

  // ROS stuff
  message_filters::Subscriber<octomap_msgs::OctomapBinary> sub_;
  tf::MessageFilter<octomap_msgs::OctomapBinary>* tf_filter_;

  // Ogre-rviz point clouds
  Ogre::SceneNode* pointcloud_scene_node_;
  PointCloud* cloud_[16];

  // Plugin properties
  IntProperty* queue_size_property_;
  u_int32_t queue_size_;

  RosTopicProperty* octomap_topic_property_;

  IntProperty* tree_depth_property_;
  unsigned int treeDepth_;

  double colorFactor_;
};


} // namespace octomap_rviz_plugin

#endif //RVIZ_OCTOMAP_CLOUD_DISPLAY_H
