#!/usr/bin/env python3

import rospy
from std_msgs.msg import Float64

class NumberPublisher:
    def __init__(self):
        self.number = 3.1
        self.publish_interval = 1.5

        self.num_pub = rospy.Publisher("/number", Float64, queue_size=10)
        self.timer_pub = rospy.Timer(rospy.Duration(self.publish_interval), self.timerCallback)

    def timerCallback(self, event):
        msg = Float64()
        msg.data = self.number
        self.num_pub.publish(msg)

if __name__ == '__main__':
    try:
        rospy.init_node("number_publisher_node")
        NumberPublisher()
        rospy.spin()
    except rospy.ROSInterruptException:
        pass
