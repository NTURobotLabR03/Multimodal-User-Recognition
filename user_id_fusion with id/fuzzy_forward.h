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
	void fuzzy_initial();	//�]�wmembership function�ܼƤ���l��
	void fuzzy_rule();		//�]�w�Umembership function�A�H��fuzzy rule
	//Defuzzier Initialize
	void defuzz_initial();	//�Ѽҽk��
	void get_min_max_range(vector<double>&); //��X: �C��membership function range���̤j�ȥH�γ̤p��
	//Output
	void fuzzy_output(double, double, double, double&); //fuzzy influence system ���D�nfunction		//(input, input, input, output)
														//��J: �y���B����B��A�C�⤧����(�u���o�T�ӬOcall by value)�A��X:fusion score(call by reference)
private:
	Engine* engine;
	InputVariable* face;
	InputVariable* body;
	InputVariable* dress;
	OutputVariable* similarity;
	RuleBlock* ruleblock;

};