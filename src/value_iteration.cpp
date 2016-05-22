#include "ros/ros.h"
#include "std_msgs/String.h"
#include "std_msgs/Float64.h"
#include "simple_value_iteration_ros/DoSweeps.h"
#include "simple_value_iteration_ros/FlushData.h"
#include <iostream>
#include <vector>
#include "StateTrans.h"
#include <thread>
using namespace std;

void worker(int start_pos,StateTrans *st);
bool do_sweeps(simple_value_iteration_ros::DoSweeps::Request &req,
	simple_value_iteration_ros::DoSweeps::Response &res);
bool flush_data(simple_value_iteration_ros::FlushData::Request &req,
	simple_value_iteration_ros::FlushData::Response &res);
void die(string reason);

void worker(int start_pos,StateTrans *st) {
	//no need of exclusive control due to the property of value functions
	st->valueIteration(start_pos);
}

void die(string reason){
	ROS_INFO(reason.c_str());
	exit(1);
}

StateTrans st;

bool flush_data(simple_value_iteration_ros::FlushData::Request &req,
		simple_value_iteration_ros::FlushData::Response &res)
{
	string v_filename = req.work_dir + '/' + req.filename + ".value";
	if(!st.printValues(v_filename))
		die("value file write error");

	string p_filename = req.work_dir + '/' + req.filename + ".policy";
	if(!st.printActions(p_filename))
		die("policy file write error");

	res.file = req.work_dir + '/' + req.filename;
	return true;
}

bool do_sweeps(simple_value_iteration_ros::DoSweeps::Request &req,
		simple_value_iteration_ros::DoSweeps::Response &res)
{
	//reading data from file
	if(req.initialize){
		st.init();
		string filename = req.work_dir + '/' + req.state_transition_file;
		if(! st.readStateTransFile(filename.c_str()))
			die("state_trans file error");
	}

	//execution with n threads
	int worker_num = req.thread_num;
	if(worker_num <= 0)
		worker_num = 1;

	vector<thread> th;
	for(int n=0;n<worker_num;n++){
		unsigned long start_pos = (unsigned long)
			(double(st.getStateNum())/worker_num*n);

		th.push_back(thread(worker,start_pos,&st));
	}

	//waiting
	for(int n=0;n<worker_num;n++){
		th[n].join();
	}

	res.delta = st.getDelta();

	return true;
}

int main(int argc, char **argv)
{
	ros::init(argc, argv, "value_iteration");
	ros::NodeHandle n;

	ros::ServiceServer f = n.advertiseService("flush_data", flush_data);

	ros::ServiceServer s = n.advertiseService("do_sweeps", do_sweeps);
	ROS_INFO("Prepared the node");

	ros::spin();

	return 0;
}

