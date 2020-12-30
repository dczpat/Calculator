#pragma once
#include <iostream>
#include <vector>
#include <stack>
#include <algorithm>
#include <string>
#include <sstream>
#include <set>
#include <map>
#include <cmath>
#include <regex>
#include <Windows.h>
#include <fstream>
#include <conio.h>
#include <iomanip>
using namespace std;

enum elemType { OPERAND, OPERATOR };
typedef double (*pf1)(double);
typedef double (*pf2)(double, double);

//后缀表达式中的元素可能是运算符或操作数
//故建立一个统一的元素类型
struct exprElem
{
	elemType et;//标识类型
	char opt;
	double opd;
	exprElem(elemType et, char c, double d) :
		et(et), opt(c), opd(d) {}
};

const double pi = 3.1415926;

class Calculator
{
private:
	vector<pair<string, double>> records; //最近计算记录，最多5条
	string infixExpr;//当前输入的中缀表达式
	vector<exprElem> postfixExpr;//转换后得到的后缀表达式
	char prev, cur;//前一个、当前扫描的字符
	double result;//运算结果
	bool checkSyntax();//检查表达式合法性
	bool in2postExpr();//中缀转后缀表达式
	bool checkParen();//检查括号匹配，最先进行
	double computePostfixExpr();//计算后缀表达式
	unsigned int findErr();//查找已发现的错误在原始表达式中的位置
	void displayErr(unsigned int pos);//高亮展示错误位置
public:
	Calculator();//导入计算记录
	~Calculator();//导出计算记录
	void compute(string expr);//供外界调用的计算接口
	void displayRecords();//供外界调用的查看记录接口
};