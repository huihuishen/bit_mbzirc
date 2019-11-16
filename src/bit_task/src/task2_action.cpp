#include "ros/ros.h"
#include "std_msgs/String.h"

#include <sstream>

#include <actionlib/client/simple_action_client.h>
#include "bit_motion/pickputAction.h"    

#include <actionlib/server/simple_action_server.h>
#include "bit_plan/buildingAction.h"


#define TASK_GET 0
#define TASK_BUILD 1

typedef actionlib::SimpleActionClient<bit_motion::pickputAction> Client;
typedef actionlib::SimpleActionServer<bit_plan::buildingAction> Server;

class PickPutActionClient
{
public:
    PickPutActionClient(const std::string client_name, bool flag = true):client(client_name, flag) // 使用初始化列表来初始化client
    {
        ClientName = client_name;
    }

    // 客户端开始
    void Start()
    {
        if (!client.isServerConnected())
        {
            // 等待服务器初始化完成
            ROS_INFO_STREAM("Waiting for action server: ["<<ClientName<<"] to start!");
            client.waitForServer();
            ROS_INFO_STREAM("Action server: ["<<ClientName<<"] start!");
        }        
    }

    void sendGoal(bit_motion::pickputGoal goal, double Timeout = 10.0)
    {
        // 发送目标至服务器
        client.sendGoal(goal,
                        boost::bind(&PickPutActionClient::done_cb, this, _1, _2),
                        boost::bind(&PickPutActionClient::active_cb, this),
                        boost::bind(&PickPutActionClient::feedback_cb, this, _1));
        
        // 等待完成，超时时间为10s
        if(client.waitForResult(ros::Duration(Timeout)))    // 如果目标完成
        {
            ROS_INFO_STREAM("Task: ["<<ClientName<<"] finished within the time");
        }
        else
        {
            ROS_INFO_STREAM("Task: ["<<ClientName<<"] failed, cancel Goal");
            client.cancelGoal();
        }
        ROS_INFO("Current State: %s\n", client.getState().toString().c_str());
    }

private:
    Client client;
    std::string ClientName;      // action名称

private:
    /* action完成时的回调函数，一次性  */
    void done_cb(const actionlib::SimpleClientGoalState& state, const bit_motion::pickputResultConstPtr& result)
    {
        ROS_INFO("DONE:%d", result->finish_state);
    }
    /* action启动时的回调函数，一次性  */
    void active_cb()
    {
        ROS_INFO("ACTIVE");
    }
    /* action收到反馈时的回调函数 */
    void feedback_cb(const bit_motion::pickputFeedbackConstPtr& feedback)
    {
        ROS_INFO_STREAM("THE STATE NOM IS: "<<  feedback -> move_rightnow);
    }
};


class BuildingActionServer
{
    public:
        BuildingActionServer(ros::NodeHandle nh, const std::string server_name, bool flag = false):
                        server(nh,server_name,boost::bind(&BuildingActionServer::execute_cb, this, _1),flag)
        {
            server.registerPreemptCallback(boost::bind(&BuildingActionServer::preempt_cb, this));
        }

        void Start()
        {
            server.start();   
        }

        /*
        *建筑 action
        */
        void execute_cb(const bit_plan::buildingGoalConstPtr& goal)
        {
            bit_plan::buildingFeedback feedback;    /* 创建一个feedback对象 */

            ROS_INFO("The goal brick num is: %d", goal->goal_task.Num);

            static PickPutActionClient Task2Client("pickputAction", true);   // 连接取砖动作服务器
            Task2Client.Start();

            /* 判断是否有砖堆信息与放置处信息 */
            if (true)   // 如果有砖堆信息
            {
                feedback.task_feedback = "The brick and building position exist";
                server.publishFeedback(feedback);
            }
            else        // 如果没有砖堆信息
            {
                feedback.task_feedback = "Looking for the position of brick and building";
                server.publishFeedback(feedback);
                // 找砖和墙位置，
                // 然后在建筑位置进行global costmap的设置，放置车辆开过砖墙区域  

                ros::Duration(5).sleep();       // 临时占位     换成action

                feedback.task_feedback = "The position of brick and building has been found";
                server.publishFeedback(feedback);
            }

            // 循环每个预设砖块信息
            for (size_t count = 0; count < goal->goal_task.Num; count++)
            {
                // 根据 goal->goal_task.bricks[count].type 移动至相应颜色砖块处
                ROS_INFO_STREAM("Move to the brick type: "<<goal->goal_task.bricks[count].type);
                
                ros::Duration(3).sleep();       // 临时占位     换成 service

                // 将砖块搬运至车上
                pick_goal.goal_brick = goal->goal_task.bricks[count];  // 将队列砖块取出发送给取砖程序
                pick_goal.task = TASK_GET;

                Task2Client.sendGoal(pick_goal, 100);   // 发送目标 timeout 100s

                ROS_INFO("Picked %ld brick of %d", count+1, goal->goal_task.Num);
            }
            
            // 移动到观察建筑处，调用检测砖堆状态
            ROS_INFO("the building state is well");

            ros::Duration(3).sleep();       // 临时占位     换成 service


            // 移动至砖块建筑处
            ROS_INFO("move to the building place");

            ros::Duration(3).sleep();       // 临时占位     换成 service

            // 循环
            // 移动局部位置，需要使得车辆前进线对准目标位置
            // 放砖
            for (size_t count = 0; count < goal->goal_task.Num; count++)
            {
                // 计算出欲放置砖块相对于目前的位置
                // 移动

                ros::Duration(3).sleep();       // 临时占位     换成 service

                // 将砖块放置在建筑物处
                pick_goal.goal_brick = goal->goal_task.bricks[count];  // 将队列砖块取出发送给取砖程序
                pick_goal.task = TASK_BUILD;

                Task2Client.sendGoal(pick_goal, 100);   // 发送目标 timeout 100s

                ROS_INFO("Put %ld brick of %d", count+1, goal->goal_task.Num);

            }

            ROS_INFO("COUNT DONE");

            server.setSucceeded();   /* 发送result */
        }

    private:
        Server server;
        /* 创建搬运action目标 */
        bit_motion::pickputGoal pick_goal;   
        
        
        // 中断回调函数
        void preempt_cb()
        {
            if(server.isActive())
            {
                server.setPreempted(); // 强制中断
            }
        }
};


int main(int argc, char *argv[])
{
    ros::init(argc, argv, "task2_action");
    ros::NodeHandle nh;

    BuildingActionServer UGVServer(nh, "ugv_building", false);
    UGVServer.Start();

    ros::spin();

    return 0;
}