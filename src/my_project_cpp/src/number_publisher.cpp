#include "ros/ros.h"

#include "std_msgs/Float64.h"
#include "std_srvs/Empty.h"


class NumberPublisher{                             // Atualizar

    public:
        NumberPublisher(ros::NodeHandle *nh){      // Atualizar
            //Inicializar Variaveis
            number = 1.2;
            publish_interval = 1;
            reset_interval = 7;

            num_pub = nh->advertise<std_msgs::Float64>("/number", 10);
            timer_pub = nh->createTimer(ros::Duration(publish_interval), &NumberPublisher::timerCallback, this);
            timer_reset = nh->createTimer(ros::Duration(reset_interval), &NumberPublisher::timerResetCallback, this);
            client_reset = nh->serviceClient<std_srvs::Empty>("reset_counter");
        }

        void timerCallback(const ros::TimerEvent &event){
                std_msgs::Float64 msg;
                msg.data = number;
                num_pub.publish(msg);
        }

        void timerResetCallback(const ros::TimerEvent &event){
                std_srvs::Empty srv;
                ROS_INFO("solicitacao de resett da contagem");
                client_reset.call(srv);
        }

    private:
        double number;
        double publish_interval;
        double reset_interval;
        ros::Publisher num_pub;
        ros::Timer timer_pub;
        ros::ServiceClient client_reset;
        ros::Timer timer_reset;
};


int main(int argc, char **argv)
{
    ros::init(argc, argv, "number_publisher");     // Atualizar
    ros::NodeHandle nh;
    NumberPublisher node_name = NumberPublisher(&nh);     // Atualizar
    
    ros::spin();
    return 0;
}