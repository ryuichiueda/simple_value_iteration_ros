#!/usr/bin/env python

import sys,os
import rospy,roslib
from simple_value_iteration_ros.srv import *

def p(f):
    n = 0
    for line in f:
        print '\t' + line.rstrip() + '\t',
        n = n + 1
        if n%3 == 0:
            print ''

    print ''

def do_value_iteration(t_num,init):
    try:
        s = rospy.ServiceProxy('do_sweeps',DoSweeps)
        d = roslib.packages.get_pkg_dir('simple_value_iteration_ros')
        # change this file to example_state_trans_prob if you want to try the probabilistic version
        res = s(work_dir='',
            state_transition_file=d+'/scripts/example_state_trans',
            thread_num=t_num,initialize=init)

    except rospy.ServiceException, e:
        print "Service call failed: %s"%e

    return res.delta

def print_values():
    try:
        s = rospy.ServiceProxy('flush_data',FlushData)

        v_file = '/tmp/tmp' + str(os.getpid()) + '.value'
        s(work_dir='',filename=v_file,type="values")
    
    except rospy.ServiceException, e:
        print "Service call failed: %s"%e

    with open(v_file,'r') as f:
        print "values:"
        p(f)

    os.remove(v_file)

def print_policy():
    try:
        s = rospy.ServiceProxy('flush_data',FlushData)

        p_file = '/tmp/tmp' + str(os.getpid()) + '.policy'
        s(work_dir='',filename=p_file,type="policy")
    
    except rospy.ServiceException, e:
        print "Service call failed: %s"%e

    with open(p_file,'r') as f:
        print "policy:"
        p(f)

    os.remove(p_file)
    	
if __name__ == '__main__':
    rospy.wait_for_service('do_sweeps')
    rospy.wait_for_service('flush_data')

    n = 1
    print "### sweep %d ###" % n
    delta = do_value_iteration(1,True)
    print_values()
    print_policy()
    n = n + 1

    while delta > 0.001:
        print "### sweep %d ###" % n
        delta = do_value_iteration(2,False)
        print_values()
        print_policy()
        n = n + 1
    
