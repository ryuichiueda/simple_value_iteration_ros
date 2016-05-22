#ifndef STATE_TRANS_H_
#define STATE_TRANS_H_

#include <iostream>
#include <sstream>
#include <cstdlib>
#include <vector>
#include "State.h"
using namespace std;

class StateTrans{
private:
	unsigned long m_state_num;
	vector<string> m_actions;
	vector<State> m_states;

	//functions for initialization
	bool setStateNum(const string &str);
	bool setAction(const string &action);
	bool setValue(unsigned long s,unsigned long v);
	bool setStateTrans(unsigned long s,int a,unsigned long s_to,double p,unsigned long cost);
	//functions for file reading
	bool parseHeader(string &line);
	bool parseStateTrans(string &line);
	bool parseFinalStates(string &line);
	bool tokenizer(string &line,vector<string> &words);

	double m_delta;
public:
	StateTrans();
	~StateTrans();
	//void status(void);

	void init(void);

	unsigned int getActionIndex(string &line);
	State *getState(unsigned long index);
	unsigned long getStateNum(void);
	//functions for execution
	bool readStateTransFile(const char *filename);
	bool valueIteration(unsigned long start_pos);

	//output
	bool printValues(string filename);
	bool printActions(string filename);

	double getDelta(void){return m_delta;}
};

#endif
