//
// Created by chenzheng on 2022/7/4.
//

#include <lcm/lcm-cpp.hpp>
#include <ros/ros.h>
#include <lcm_to_ros/LcmData.h>
#include <csignal>
#include "lcm_to_ros/state_estimator_lcmt.hpp"

lcm_to_ros::LcmData lcm_data;

class Handler {
 public:
  Handler() {};
  void state_estimator(const lcm::ReceiveBuffer *rbuf,
                       const std::string &chan,
                       const state_estimator_lcmt *msg) {
    lcm_data.p_x = msg->p[0];
    lcm_data.p_y = msg->p[1];
    lcm_data.p_z = msg->p[2];
    lcm_data.vWorld_x = msg->vWorld[0];
    lcm_data.vWorld_y = msg->vWorld[1];
    lcm_data.vWorld_z = msg->vWorld[2];
    lcm_data.vBody_x = msg->vBody[0];
    lcm_data.vBody_y = msg->vBody[1];
    lcm_data.vBody_z = msg->vBody[2];
    lcm_data.rpy_x = msg->rpy[0];
    lcm_data.rpy_y = msg->rpy[1];
    lcm_data.rpy_z = msg->rpy[2];
    lcm_data.omegaBody_x = msg->omegaBody[0];
    lcm_data.omegaBody_y = msg->omegaBody[1];
    lcm_data.omegaBody_z = msg->omegaBody[2];
    lcm_data.omegaWorld_x = msg->omegaWorld[0];
    lcm_data.omegaWorld_y = msg->omegaWorld[1];
    lcm_data.omegaWorld_z = msg->omegaWorld[2];
    lcm_data.quat_0 = msg->quat[0];
    lcm_data.quat_1 = msg->quat[1];
    lcm_data.quat_2 = msg->quat[2];
    lcm_data.quat_3 = msg->quat[3];
    lcm_data.p1_x = msg->pfoot[0];
    lcm_data.p1_y = msg->pfoot[1];
    lcm_data.p1_z = msg->pfoot[2];
    lcm_data.p2_x = msg->pfoot[3];
    lcm_data.p2_y = msg->pfoot[4];
    lcm_data.p2_z = msg->pfoot[5];
    lcm_data.p3_x = msg->pfoot[6];
    lcm_data.p3_y = msg->pfoot[7];
    lcm_data.p3_z = msg->pfoot[8];
    lcm_data.p4_x = msg->pfoot[9];
    lcm_data.p4_y = msg->pfoot[10];
    lcm_data.p4_z = msg->pfoot[11];
    lcm_data.pz_1 = msg->pz[0];
    lcm_data.pz_2 = msg->pz[1];
    lcm_data.pz_3 = msg->pz[2];
    lcm_data.pz_4 = msg->pz[3];
  }
};

int main(int argc, char **argv) {
  // ROS节点初始化
  ros::init(argc, argv, "lcm_to_ros");
  // 创建节点句柄
  ros::NodeHandle n;
  // 创建一个Publisher，发布名为/person_info的topic，消息类型为learning_topic::Person，队列长度10
  ros::Publisher lcm_data_pub = n.advertise<lcm_to_ros::LcmData>("/lcm_data", 10);
  // 设置循环的频率
  ros::Rate loop_rate(1000);
  signal(SIGINT, SIG_DFL);

  lcm::LCM state_estimator_lcm;

  if (!state_estimator_lcm.good())
    return 1;
  Handler handlerObject;
  state_estimator_lcm.subscribe("state_estimator_lcm", &Handler::state_estimator, &handlerObject);

  ROS_INFO("Start Conversion!!!");
  while (ros::ok()) {
    state_estimator_lcm.handle();
    lcm_data_pub.publish(lcm_data);
    loop_rate.sleep();
  }
  return 0;
}
