#!/bin/bash 

tmp=/tmp/$$

dir=$(dirname $0)
cp $dir/test/state_trans /tmp/tmp.simple_value_iteration_ros

rosrun simple_value_iteration_ros value_iteration &

sleep 3

#############################################
### the first sweep (number of thread: 1) ###
#############################################

# sweep
rosservice call /do_sweeps "work_dir: '/tmp/'
state_transition_file: 'tmp.simple_value_iteration_ros'
thread_num: 1
initialize: true" | tee $tmp-res

# writing out of the value function
rosservice call /flush_data "work_dir: '/tmp/'
filename: 'result'"
echo '* values'
cat /tmp/result.value

##############################################
### following sweeps (number of thread: 8) ###
##############################################
DELTA=$(awk '{print $2}' $tmp-res)
while [ "$DELTA" != '0.0' ] ; do
	rosservice call /do_sweeps "thread_num: 8
initialize: false" | tee $tmp-res

	DELTA=$(awk '{print $2}' $tmp-res)

	# writing out of the value function
	rosservice call /flush_data "work_dir: '/tmp/'
filename: 'result'"
	echo '* values'
	cat /tmp/result.value
done 

##########################
### showing the policy ###
##########################
rosservice call /flush_data "work_dir: '/tmp/'
filename: 'result'"
echo '* policy'
cat /tmp/result.policy
