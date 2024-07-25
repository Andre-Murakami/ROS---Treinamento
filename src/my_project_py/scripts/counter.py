#!/usr/bin/env python3

import rospy

from std_msgs.msg import Float64

class Counter:

        def __init__(self):
            self.count = 0
            self.publish_interval = 3


            self.count = 0
            self.num_sub = rospy.Subscriber("/number", Float64, self.numberCallback, queue_size = 10)
            self.count_pub = rospy.Publisher("/current_count", Float64, queue_size=10)
            self.timer_pub = rospy.Timer(rospy.Duration(self.publish_interval), self.timerCallback)

        def numberCallback(self, msg):
            self.count = self.count + msg.data
            rospy.loginfo("Contagem Atual " + str(self.count))

        def timerCallback(self, event):
            msg = Float64()
            msg.data = self.count
            self.count_pub.publish(msg)


if __name__ == '__main__':
    try:
        rospy.init_node("counter_node")
        Counter()
        rospy.spin()
    except rospy.ROSInterruptException:
        pass


