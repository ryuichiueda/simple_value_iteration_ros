# simple_value_iteration_ros

A simple value iteration tool that works on ROS

# how to use

We show an example with a sample script: ./scripts/sample.py.
This sample solves a problem shown below with a `state_transition_file,'
which is shown in ./scripts/example_state_trans.

1. launch the server

    ```
    $ rosrun simple_value_iteration_ros value_iteration 
    [ INFO] [1463924689.001910579]: Prepared the node
    ```

2. launch the sample script

In the case of this example, two sweeps are executed. In the second sweep, 
two threads are invoked. How to call services and how to use the multi-thread
mode are shown in the sample python code.

    ```
    $ rosrun simple_value_iteration_ros sample.py 
    ### sweep 0 ###
    values:
    	0 70368744177664		1 1000		2 0	
    	3 70368744177664		4 2000		5 1000	
    	6 70368744177664		7 3000		8 2000	
    
    policy:
    	0 null		1 right		2 null	
    	3 null		4 up		5 up	
    	6 null		7 up		8 up	
    
    ### sweep 1 ###
    values:
    	0 2000		1 1000		2 0	
    	3 3000		4 2000		5 1000	
    	6 4000		7 3000		8 2000	
    
    policy:
    	0 right		1 right		2 null	
    	3 up		4 up		5 up	
    	6 up		7 up		8 up	
    ```

# the sample problem

In sample.py executed above solve the following problem.

We assume the following tile world.

    |-----------------------|
    |       |       |       |
    |state 0|state 1|state 2|
    |       |       |       |
    |-----------------------|
    |       |       |       |
    |state 3|state 4|state 5|
    |       |       |       |
    |-------|-------|-------|
    |       |       |       |
    |state 6|state 7|state 8|
    |       |       |       |
    |-------|-------|-------|

At first, a robot exists one of the state. The task is that the robot goes to
the state 2 from any initial state with minimum number of steps.

The robot can choose
`left', `right', `up', and `down'
so as to change its state.
When an action is done,
the robot receives value 1
as a cost.

The optimal value function
will be calculated from this
definition.

    |-----------------------|
    |       |       |       |
    |   2   |   1   |   0   |
    |       |       |       |
    |-----------------------|
    |       |       |       |
    |   3   |   2   |   1   |
    |       |       |       |
    |-------|-------|-------|
    |       |       |       |
    |   4   |   3   |   2   |
    |       |       |       |
    |-------|-------|-------|


# format of a state_transition_file

A state_transition_file is composed of three sections that start with `%%...'
In this stage, we cannot change the order of the sections.

    %%metadata%%
    statenum 9
    actions left right up down
    
    %%state transitions%%
    state 6 action right
    	state 7 prob. 1 cost 1000
    state 6 action up
    	state 3 prob. 1 cost 1000
    
    state 7 action right
    	state 8 prob. 1 cost 1000

    ....

    state 1 action down
    	state 4 prob. 1 cost 1000
    state 1 action right
    	state 2 prob. 1 cost 1000

    %%final states%%
    state 2 value 0

There is another example state_transition_file `example_state_trans_prob'
in the directory. In this file, state transitions are defined stochastically.
