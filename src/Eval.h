//==========Eval.h===========

#ifndef EVAL_INCLUDED
#define EVAL_INCLUDED

//=================================
// included dependencies
#include <string>
#include <stdlib.h>
#include <sstream>
#include <vector>
//=================================




class GetArg
{
private:
    std::string temp1;
public:
    std::string GetArg0(std::string, std::string, std::string);
    std::string GetArg1(std::string, std::string, std::string, int, std::string);
};


class Eval
{
private:
    int temp1, temp3;
    std::string temp2;
public:
    Eval(std::string);
    std::string POWER(std::string);
    std::string DIVIDE(std::string);
    std::string MULTIPLY(std::string);
    std::string ADD(std::string);
    std::string SUBTRACT(std::string);
    std::string ADDORSUBTRACT(std::string);
    std::string MULTIPLYORDIVIDE(std::string);
    std::string operation;

    bool BEval(std::string);
    std::string ISEQUAL(std::string);
    std::string ISNOTEQUAL(std::string);
    std::string ISGREATER(std::string);
    std::string ISSMALLER(std::string);
    std::string ISGREATERE(std::string);
    std::string ISSMALLERE(std::string);
    std::string AND(std::string);
    std::string OR(std::string);

};

float Evaluate(std::string strvalue);

int iEvaluate(std::string strvalue);

std::string stringify(int whatever);
std::string stringify(float whatever);
std::string stringify(double whatever);

bool BEvaluate(std::string strvalue);

std::string Evaluayh(std::string strvalue);


#endif  //EVAL_INCLUDED





