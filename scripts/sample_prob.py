#!/usr/bin/env python

import sys,os
import rospy,roslib
from simple_value_iteration_ros.srv import *

def do_value_iteration(t_num,init):
    try:
        s = rospy.ServiceProxy('do_sweeps',DoSweeps)
        d = roslib.packages.get_pkg_dir('simple_value_iteration_ros')
        res = s(work_dir='', state_transition_file=d+'/scripts/example_state_trans_prob',thread_num=t_num,initialize=init)

    except rospy.ServiceException, e:
        print "Service call failed: %s"%e

    return res.delta

def get_the_result():
    try:
        # get the result
        suffix = 'tmp' + str(os.getpid())
        s = rospy.ServiceProxy('flush_data',FlushData)
        delta = s(work_dir='/tmp',filename=suffix)

        v_file = '/tmp/' + suffix + '.value'
        p_file = '/tmp/' + suffix + '.policy'
    
    except rospy.ServiceException, e:
        print "Service call failed: %s"%e

    with open(v_file,'r') as f:
        print "values:"
        for line in f: print '\t' + line,

    with open(p_file,'r') as f:
        print "policy:"
        for line in f: print '\t' + line,

    os.remove(v_file)
    os.remove(p_file)
    	
if __name__ == '__main__':
    rospy.wait_for_service('do_sweeps')
    rospy.wait_for_service('flush_data')

    n = 0
    print "### sweep %d ###" % n
    delta = do_value_iteration(2,True)
    get_the_result()
    n = n + 1

    while delta > 0.001:
        print "### sweep %d ###" % n
        delta = do_value_iteration(8,False)
        get_the_result()
        n = n + 1
    
