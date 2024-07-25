#include "ros/ros.h"

#include "std_msgs/Float64.h"

class NumberPublisher{                             // Atualizar

    public:
        NumberPublisher(ros::NodeHandle *nh){      // Atualizar
            //Inicializar Variaveis
            number = 1.2;
            publish_interval = 1;
            num_pub = nh->advertise<std_msgs::Float64>("/number", 10);
            timer_pub = nh->createTimer(ros::Duration(publish_interval), &NumberPublisher::timerCallback, this);

        }

        void timerCallback(const ros::TimerEvent &event){
                std_msgs::Float64 msg;
                msg.data = number;
                num_pub.publish(msg);
        }

    private:
        double number;
        double publish_interval;
        ros::Publisher num_pub;
        ros::Timer timer_pub;

};


int main(int argc, char **argv)
{
    ros::init(argc, argv, "number_publisher");     // Atualizar
    ros::NodeHandle nh;
    NumberPublisher node_name = NumberPublisher(&nh);     // Atualizar
    
    ros::spin();
    return 0;
}