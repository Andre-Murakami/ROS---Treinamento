#include "ros/ros.h"

//Mensagens
#include "std_msgs/Float64.h"


class Counter {

    public: 
        Counter(ros::NodeHandle *nh) {
            count = 0;
            publish_interval = 1;

            num_sub = nh->subscribe("/number", 10 , &Counter::numberCallback, this);
            count_pub = nh->advertise<std_msgs::Float64>("/current_count", 10);
            timer_pub = nh->createTimer(ros::Duration(publish_interval), &Counter::timerCallback, this);
        }

        void numberCallback(const std_msgs::Float64 &msg){
            count = count + msg.data;
            ROS_INFO("Contagem atual %f ", count);
        }

        void timerCallback(const ros::TimerEvent &event) {
            std_msgs::Float64 msg;
            msg.data =count;
            count_pub.publish(msg);
        }



    private:
        double count;
        double publish_interval;
        ros::Subscriber num_sub;
        ros::Publisher count_pub;
        ros::Timer timer_pub;

};

int main(int argc, char **argv ) {


    ros:: init(argc, argv, "counter_node"); //inicializando o nó
    ros::NodeHandle nh;
    Counter counter = Counter(&nh);
    ros::spin();

    return 0;
}
