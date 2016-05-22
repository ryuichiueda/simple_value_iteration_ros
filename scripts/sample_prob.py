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

def get_values():
    try:
        s = rospy.ServiceProxy('flush_data',FlushData)

        v_file = '/tmp/tmp' + str(os.getpid()) + '.value'
        s(work_dir='',filename=v_file,type="values")
    
    except rospy.ServiceException, e:
        print "Service call failed: %s"%e

    with open(v_file,'r') as f:
        print "values:"
        for line in f: print '\t' + line,

    os.remove(v_file)

def get_policy():
    try:
        s = rospy.ServiceProxy('flush_data',FlushData)

        p_file = '/tmp/tmp' + str(os.getpid()) + '.policy'
        s(work_dir='',filename=p_file,type="policy")
    
    except rospy.ServiceException, e:
        print "Service call failed: %s"%e

    os.remove(p_file)
    	
if __name__ == '__main__':
    rospy.wait_for_service('do_sweeps')
    rospy.wait_for_service('flush_data')

    n = 0
    print "### sweep %d ###" % n
    delta = do_value_iteration(2,True)
    get_values()
    get_policy()
    n = n + 1

    while delta > 0.001:
        print "### sweep %d ###" % n
        delta = do_value_iteration(8,False)
        get_values()
        get_policy()
        n = n + 1
    
