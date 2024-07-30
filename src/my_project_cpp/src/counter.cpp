#include "ros/ros.h"

//Mensagens
#include "std_msgs/Float64.h"
#include "std_srvs/Empty.h"
#include "my_project_msgs/CounterHistory.h"
#include "my_project_msgs/CheckNumber.h"

class Counter {

    public: 
        Counter(ros::NodeHandle *nh) {
            count = 0;
            publish_interval = 1;

            if (nh->getParam("initial_count", count)){
                ROS_INFO("Contagem inicial em %f", count);
            } else {
                ROS_INFO("Contagem inicial nao definido");
            }
            nh->param<double>("pub_rate", publish_interval, 1);

            if (nh->hasParam("pub_rate") || nh->hasParam("initial_count")) {
                ROS_WARN("Parametros customizados");
                nh->setParam("custom_param", true);
            } else {
                nh->setParam("custom_param", false);
            }

            if (nh->hasParam("delete_param")) {
                ROS_ERROR("Deletando Parametros");
                nh->deleteParam("pub_rate");
                nh->deleteParam("initial_count");
            }

            last_count = count;
            cycles = 0;

            num_sub = nh->subscribe("/number", 10 , &Counter::numberCallback, this);
            count_pub = nh->advertise<std_msgs::Float64>("/current_count", 10);
            timer_pub = nh->createTimer(ros::Duration(publish_interval), &Counter::timerCallback, this);
            reset_srv = nh->advertiseService("/reset_counter", &Counter::resetSrvCallback, this);
            history_pub = nh->advertise<my_project_msgs::CounterHistory>("/history_counter", 10);
            check_greater_srv = nh->advertiseService("/check_greater", &Counter::checkGreaterSrvCallback, this);
        }

         

        void numberCallback(const std_msgs::Float64 &msg){
            cycles++;
            last_count = count;
            count = count + msg.data;
            ROS_INFO("Contagem atual %f ", count);
        }

        void timerCallback(const ros::TimerEvent &event) {
            std_msgs::Float64 msg;
            msg.data =count;
            count_pub.publish(msg);

            my_project_msgs::CounterHistory history_msg;
            history_msg.current_value = count;
            history_msg.last_value = last_count;
            history_msg.cycles = cycles;
            history_pub.publish(history_msg);
        }

        bool resetSrvCallback(std_srvs::Empty::Request &req, std_srvs::Empty::Response &res){
            count = 0;
            ROS_INFO("Resentando a contagem.");    
            return true;
        }

        bool checkGreaterSrvCallback(my_project_msgs::CheckNumber::Request &req, my_project_msgs::CheckNumber::Response &res){
            if( req.number > count) {
                res.result = true;
            } 
            else {
                res.result = false;
            }
            ROS_WARN("Verifcando de eh amior que contagem.");    
            return true;
        }

    private:
        double count;
        double publish_interval;
        double last_count;
        int cycles;

        ros::Publisher history_pub;
        ros::Subscriber num_sub;
        ros::Publisher count_pub;
        ros::Timer timer_pub;
        ros::ServiceServer reset_srv;
        ros::ServiceServer check_greater_srv;
};

int main(int argc, char **argv ) {


    ros:: init(argc, argv, "counter_node"); //inicializando o nó
    ros::NodeHandle nh;
    Counter counter = Counter(&nh);
    ros::spin();

    return 0;
};
