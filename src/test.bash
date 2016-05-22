#!/bin/bash -xv

rosrun simple_value_iteration_ros value_iteration &

sleep 3

rosservice call /do_sweeps "work_dir: '/tmp/'
state_transition_file: '/home/ueda/catkin_ws/src/simple_value_iteration_ros/src/test/state_trans'
thread_num: 1
initialize: true" 

for i in seq 1 10 ; do
rosservice call /do_sweeps "work_dir: '/tmp/'
state_transition_file: '/home/ueda/catkin_ws/src/simple_value_iteration_ros/src/test/state_trans'
thread_num: 1
initialize: false" 
done 
