//Author: Kuok Chon Kit
//Date: 2016/08/11
//Program Name: Multimodal fusion
//Function: fusion the three modalities
//Lib used: fuzzylite-3.16
#include "fuzzy_forward.h"

using namespace fl;

void fuzzy_forward::fuzzy_initial(){	//initalize
	engine = new Engine;	//這變數意為整個fuzzy influence system
	face = new InputVariable;	//定義face
	body = new InputVariable;	//定義body
	dress = new InputVariable;	//定義dress
	similarity = new OutputVariable;	//定義fuzzy output
	ruleblock = new RuleBlock;	
}

void fuzzy_forward::fuzzy_rule(){
	//Set up the membership function
    face->setName("face");  //Set the membership function of face modality
    face->setRange(0.000, 1.000); //Set the range
    face->addTerm(new Trapezoid("HIGH", 0.000, 0.000, 0.45, 0.6)); //Set up the membership function
	face->addTerm(new Triangle("MEDIUM", 0.45, 0.6, 0.75));
    face->addTerm(new Trapezoid("LOW", 0.6, 0.75, 1.0, 1.0));
    engine->addInputVariable(face);

	//Set up the membership function
    body->setName("body"); //Set the membership function of body modality
    body->setRange(0.000, 1.000); //Set the range
    body->addTerm(new Trapezoid("HIGH", 0.5, 0.75, 1.0, 1.0)); //Set up the membership function
	body->addTerm(new Triangle("MEDIUM", 0.25, 0.5, 0.75));
    body->addTerm(new Trapezoid("LOW", 0.00, 0.00, 0.25, 0.5));
    engine->addInputVariable(body);

	//Set up the membership function
    dress->setName("dress"); //Set the membership function of dress modality
    dress->setRange(0.000, 1.000); //Set the range
    dress->addTerm(new Trapezoid("HIGH", 0.5, 0.75, 1.0, 1.0)); //Set up the membership function
	dress->addTerm(new Triangle("MEDIUM", 0.25, 0.5, 0.75));
    dress->addTerm(new Trapezoid("LOW", 0.00, 0.00, 0.25, 0.5));
    engine->addInputVariable(dress);

	//Set up the membership function
    similarity->setName("similarity"); //Set the membership function of fusion part
    similarity->setRange(-0.25, 1.25); //Set the range
    similarity->setDefaultValue(fl::nan);
	similarity->addTerm(new Triangle("VERY_LOW", -0.25, 0.25)); //Set up the membership function
    similarity->addTerm(new Triangle("LOW", 0.000, 0.500)); 
	similarity->addTerm(new Triangle("MEDIUM", 0.25, 0.75));
    similarity->addTerm(new Triangle("HIGH", 0.500, 1.00));
    similarity->addTerm(new Triangle("VERY_HIGH", 0.75, 1.250));
    engine->addOutputVariable(similarity);

	//Set up the fuzzy rule
    ruleblock->addRule(Rule::parse("if face is HIGH and body is HIGH and dress is HIGH then similarity is VERY_HIGH", engine));
	ruleblock->addRule(Rule::parse("if face is HIGH and body is HIGH and dress is MEDIUM then similarity is HIGH", engine));
	ruleblock->addRule(Rule::parse("if face is MEDIUM and body is HIGH and dress is HIGH then similarity is HIGH", engine));
	ruleblock->addRule(Rule::parse("if face is HIGH and body is MEDIUM and dress is HIGH then similarity is HIGH", engine));
	ruleblock->addRule(Rule::parse("if face is HIGH and body is MEDIUM and dress is MEDIUM then similarity is MEDIUM", engine));
	ruleblock->addRule(Rule::parse("if face is MEDIUM and body is HIGH and dress is MEDIUM then similarity is MEDIUM", engine));
	ruleblock->addRule(Rule::parse("if face is MEDIUM and body is MEDIUM and dress is HIGH then similarity is MEDIUM", engine));
	ruleblock->addRule(Rule::parse("if face is MEDIUM and body is MEDIUM and dress is MEDIUM then similarity is MEDIUM", engine));
	ruleblock->addRule(Rule::parse("if face is MEDIUM and body is MEDIUM and dress is LOW then similarity is MEDIUM", engine));
	ruleblock->addRule(Rule::parse("if face is LOW and body is MEDIUM and dress is MEDIUM then similarity is MEDIUM", engine));
	ruleblock->addRule(Rule::parse("if face is MEDIUM and body is LOW and dress is MEDIUM then similarity is MEDIUM", engine));
	ruleblock->addRule(Rule::parse("if face is MEDIUM and body is LOW and dress is LOW then similarity is LOW", engine));
	ruleblock->addRule(Rule::parse("if face is LOW and body is MEDIUM and dress is LOW then similarity is LOW", engine));
	ruleblock->addRule(Rule::parse("if face is LOW and body is LOW and dress is MEDIUM then similarity is LOW", engine));
	ruleblock->addRule(Rule::parse("if face is HIGH and body is LOW and dress is LOW then similarity is LOW", engine));
	ruleblock->addRule(Rule::parse("if face is LOW and body is HIGH and dress is LOW then similarity is LOW", engine));
	ruleblock->addRule(Rule::parse("if face is LOW and body is LOW and dress is HIGH then similarity is LOW", engine));
	ruleblock->addRule(Rule::parse("if face is LOW and body is HIGH and dress is HIGH then similarity is HIGH", engine));
	ruleblock->addRule(Rule::parse("if face is HIGH and body is LOW and dress is HIGH then similarity is HIGH", engine));
	ruleblock->addRule(Rule::parse("if face is HIGH and body is HIGH and dress is LOW then similarity is HIGH", engine));
	ruleblock->addRule(Rule::parse("if face is LOW and body is MEDIUM and dress is HIGH then similarity is MEDIUM", engine));
	ruleblock->addRule(Rule::parse("if face is HIGH and body is LOW and dress is MEDIUM then similarity is MEDIUM", engine));
	ruleblock->addRule(Rule::parse("if face is MEDIUM and body is LOW and dress is HIGH then similarity is MEDIUM", engine));
	ruleblock->addRule(Rule::parse("if face is LOW and body is HIGH and dress is MEDIUM then similarity is MEDIUM", engine));
	ruleblock->addRule(Rule::parse("if face is MEDIUM and body is LOW and dress is HIGH then similarity is MEDIUM", engine));
	ruleblock->addRule(Rule::parse("if face is HIGH and body is MEDIUM and dress is LOW then similarity is MEDIUM", engine));
	ruleblock->addRule(Rule::parse("if face is LOW and body is LOW and dress is LOW then similarity is VERY_LOW", engine));
    engine->addRuleBlock(ruleblock);	//把以上的fuzzy rule輸入至engine內

}

//This function is used for call back the min,max value of range and range of the fuzzy set
void fuzzy_forward::defuzz_initial(){
	engine->configure("Minimum", "Maximum","AlgebraicProduct", "AlgebraicSum", "Centroid"); //定義 min-max system, defuzzier
	//engine->configure("Minimum", "","AlgebraicProduct", "AlgebraicSum", "Centroid");
}

//this function is no used 沒有實際用途
//void fuzzy_forward::get_min_max_range(vector<double>& min_max_range){
//	min_max_range.clear();
//	min_max_range.push_back(face->getMinimum()); //0
//	min_max_range.push_back(face->getMaximum()); //1
//	min_max_range.push_back(body->getMinimum());
//	min_max_range.push_back(body->getMaximum());
//	min_max_range.push_back(dress->getMinimum());
//	min_max_range.push_back(dress->getMaximum());
//	min_max_range.push_back(face->range());
//	min_max_range.push_back(body->range());
//	min_max_range.push_back(dress->range()); //8
//}

//this function is used for (input->output)
void fuzzy_forward::fuzzy_output(double face_mark, double body_mark, double dress_mark, double& similarity_output){
	face->setInputValue(face_mark); //input the measured distance of face modality
	body->setInputValue(body_mark);  //input the measured distance of body modality
	dress->setInputValue(dress_mark); // //input the measured distance of dress modality
	engine->process(); //run the fuzzy influence system

	similarity_output = similarity->getOutputValue(); //the output of the fuzzy system
}