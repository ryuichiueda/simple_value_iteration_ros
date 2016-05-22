#include "ros/ros.h"
#include "std_msgs/String.h"
#include "simple_value_iteration_ros/DoSweeps.h"
#include <iostream>
#include <vector>
#include "StateTrans.h"
#include <thread>
using namespace std;

int sweep(int argc, char const* argv[]);
void worker(int start_pos,StateTrans *st);

bool do_sweeps(simple_value_iteration_ros::DoSweeps::Request &req,
	simple_value_iteration_ros::DoSweeps::Response &res);
void die(string reason);

void die(string reason){
	ROS_INFO(reason.c_str());
	exit(1);
}

bool do_sweeps(simple_value_iteration_ros::DoSweeps::Request &req,
		simple_value_iteration_ros::DoSweeps::Response &res)
{
	StateTrans st;
	//reading data from file
	if(! st.readStateTransFile(req.state_transition_file.c_str()))
		die("state_trans file error");

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

	st.printValues();
/*   res.sum = req.a + req.b;
   8   ROS_INFO("request: x=%ld, y=%ld", (long int)req.a, (long int)req.b);
   9   ROS_INFO("sending back response: [%ld]", (long int)res.sum);
  10   return true;
*/
	return true;
}

void worker(int start_pos,StateTrans *st) {
	//no need of exclusive control due to the property of value functions
	st->valueIteration(start_pos);
}

int main(int argc, char **argv)
{
	ros::init(argc, argv, "value_iteration");
	ros::NodeHandle n;
	ros::ServiceServer service = n.advertiseService("do_sweeps", do_sweeps);
	ROS_INFO("Prepared the node");
	ros::spin();

	return 0;
}



int sweep(int argc, char const* argv[])
{
	//handling of options
	int worker_num = 1;
	int filename_pos = 1;
	if(argc < 2 || argc == 3 || argc > 4){
	//	usage();
	}
	else if(argc == 4){
		filename_pos = 3;
		worker_num = atoi(argv[2]);

		if(worker_num <= 0)
			die("Invalid Thread Num");
	}

	StateTrans st;
	//reading data from file
	if(! st.readStateTransFile(argv[filename_pos]))
		die("state_trans file error");

	//execution with n threads
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

	st.printValues();

	if(st.isConverged()){
		st.printActions();
	}
	exit(0);
}
