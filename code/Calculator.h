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

//��׺���ʽ�е�Ԫ�ؿ�����������������
//�ʽ���һ��ͳһ��Ԫ������
struct exprElem
{
	elemType et;//��ʶ����
	char opt;
	double opd;
	exprElem(elemType et, char c, double d) :
		et(et), opt(c), opd(d) {}
};

const double pi = 3.1415926;

class Calculator
{
private:
	vector<pair<string, double>> records; //��������¼�����5��
	string infixExpr;//��ǰ�������׺���ʽ
	vector<exprElem> postfixExpr;//ת����õ��ĺ�׺���ʽ
	char prev, cur;//ǰһ������ǰɨ����ַ�
	double result;//������
	bool checkSyntax();//�����ʽ�Ϸ���
	bool in2postExpr();//��׺ת��׺���ʽ
	bool checkParen();//�������ƥ�䣬���Ƚ���
	double computePostfixExpr();//�����׺���ʽ
	unsigned int findErr();//�����ѷ��ֵĴ�����ԭʼ���ʽ�е�λ��
	void displayErr(unsigned int pos);//����չʾ����λ��
public:
	Calculator();//��������¼
	~Calculator();//���������¼
	void compute(string expr);//�������õļ���ӿ�
	void displayRecords();//�������õĲ鿴��¼�ӿ�
};