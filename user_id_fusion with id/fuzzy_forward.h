#pragma once
#include "fl/Headers.h"
#include <vector>
#include <iostream>

using namespace fl;
using namespace std;
class fuzzy_forward{
public:
	//*------------all of functions are call by reference
	//Initial the fuzzy rule, fuzzy rule is converted in this function
	void fuzzy_initial();	//砞﹚membership function跑计ぇ﹍て
	void fuzzy_rule();		//砞﹚membership functionのfuzzy rule
	//Defuzzier Initialize
	void defuzz_initial();	//秆家絢て
	void get_min_max_range(vector<double>&); //块: –membership function rangeぇ程の程
	//Output
	void fuzzy_output(double, double, double, double&); //fuzzy influence system ぇ璶function		//(input, input, input, output)
														//块: 羪场ō砰︾狝肅︹ぇだ计(Τ硂琌call by value)块:fusion score(call by reference)
private:
	Engine* engine;
	InputVariable* face;
	InputVariable* body;
	InputVariable* dress;
	OutputVariable* similarity;
	RuleBlock* ruleblock;

};