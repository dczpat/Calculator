#include "Calculator.h"

map<char, int> isp = {
	{'+', 3}, {'-', 3}, {'*', 5}, {'/', 5}, {'s', 6}, {'c', 6}, {'t', 6}, {'_', 6}, {'(', 1}, {')', 8}, {'\n', 0} };

map<char, int> icp = {
	{'+', 2}, {'-', 2}, {'*', 4}, {'/', 4}, {'s', 7}, {'c', 7}, {'t', 7}, {'_', 7}, {'(', 8}, {')', 1}, {'\n', 0} };

double _add(const double x, const double y)
{
	return x + y;
}

double _minus(const double x, const double y)
{
	return x - y;
}

double _multiply(const double x, const double y)
{
	return x * y;
}

double _divide(const double x, const double y)
{
	return x / y;
}

double _opposite(const double x)
{
	return -x;
}

double _sin(const double x)
{
	return sin(x);
}

double _cos(const double x)
{
	return cos(x);
}

double _tan(const double x)
{
	return tan(x);
}

map<char, pf1> op1 = {
	{'_', _opposite}, {'s', _sin}, {'c', _cos}, {'t', _tan} };

map<char, pf2> op2 = {
	{'+', _add}, {'-', _minus}, {'*', _multiply}, {'/', _divide} };

string& replace_all_distinct(string& str, const string& old_value, const string& new_value)
{
	for (string::size_type pos(0); pos != string::npos; pos += new_value.length())
	{
		if ((pos = str.find(old_value, pos)) != string::npos)
			str.replace(pos, old_value.length(), new_value);
		else
			break;
	}
	return str;
}

bool istrig(const char ch)
{
	return ch == 's' || ch == 'c' || ch == 't';
}

bool isopt(const char ch)
{
	return ch == '+' || ch == '-' || ch == '*' || ch == '/';
}

void redFont()
{
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	//red and intensified
	SetConsoleTextAttribute(hOut, FOREGROUND_RED | FOREGROUND_INTENSITY);
}

void normalFont()
{
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	//white again
	SetConsoleTextAttribute(hOut, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
}

unsigned int Calculator::findErr()
{
	string p;
	unsigned int pos;
	if (prev == 's')
		p = "sin";
	else if (prev == 'c')
		p = "cos";
	else if (prev == 't')
		p = "tan";
	else
		p = prev;
	p += cur;

	if (prev == '\n')
		pos = 0;
	else if ((pos = infixExpr.find(p)) != string::npos)
	{
		if (prev == 's' || prev == 'c' || prev == 't')
			pos += 3;
		else
			pos++;
	}
	//pi
	else
		pos = infixExpr.find("pi" + cur) + 2;

	return pos;
}

bool Calculator::checkSyntax()
{
	bool ret = true;
	//check the end
	if (cur == '\n')
	{
		if (prev != ')' && !isdigit(prev))
		{
			cout << "No \'" << prev << "\' at the end of the expression!" << endl;
			displayErr(infixExpr.length() - 2);
			return false;
		}
	}

	//illegal symbol
	if (!isdigit(cur) && isp.find(cur) == isp.end())
	{
		cout << "Illegal symbol: \'" << cur << '\'' << endl;
		ret = false;
	}

	//illegal match from here
	else if (prev == '\n' || prev == '(' || istrig(prev))
	{
		if (!isdigit(cur) && !istrig(cur) && cur != '(' && cur != '_')
		{
			cout << "No \'" << cur << "\' ";
			if (prev == '\n')
				cout << "at the beginning of the expression!";
			else if (prev == '(')
				cout << "after the left parenthesis!";
			else
				cout << "after trigonometric functions!";
			cout << endl;
			ret = false;
		}
	}
	else if (isopt(prev) || prev == '_')
	{
		if (!isdigit(cur) && !istrig(cur) && cur != '(')
		{
			cout << "No \'" << cur << "\' ";
			if (prev == '_')
				cout << "after the minus sign!";
			else
				cout << "after operators!";
			cout << endl;
			ret = false;
		}
	}
	else if (isdigit(prev) || prev == ')')
	{
		if (!isopt(cur) && cur != ')' && cur != '\n')
		{
			if (!isdigit(prev))
				cout << "No \'" << cur << "\' ";
			else
				cout << "No digit ";

			if (prev == ')')
				cout << "after the right parenthesis!";
			else
				cout << "after digits!";
			cout << endl;
			ret = false;
		}
	}

	if (!ret)
		displayErr(findErr());
	return ret;
}

//将中缀表达式转化为后缀表达式
bool Calculator::in2postExpr()
{
	string expr = infixExpr;
	stack<char> operators;
	expr = replace_all_distinct(expr, "pi", "3.1415926");
	expr = replace_all_distinct(expr, "sin", "s");
	expr = replace_all_distinct(expr, "cos", "c");
	expr = replace_all_distinct(expr, "tan", "t");
	stringstream ss;
	exprElem ee(OPERAND, '\0', 0.0);
	operators.push(prev);

	int i = 0;
	while (i < expr.length() && !operators.empty())
	{
		cur = expr[i];
		//ignore normal spaces
		if (cur == ' ') 
		{
			i++;
			continue;
		}
		if (cur == '-' && (prev == '\n' || prev == '(' || istrig(prev)))
			cur = '_';
		if (!checkSyntax())
			return false;

		//digit
		if (isdigit(cur))
		{
			prev = cur;
			int cnt = 0; //counter for '.'
			string val;
			double operand;
			while (i < expr.length() && (isdigit(expr[i]) || expr[i] == '.'))
			{
				if (expr[i] == '.')
				{
					cnt++;
					if (cnt > 1)
					{
						cout << "Redundant decimal point!" << endl;
						cout << infixExpr << endl;
						return false;
					}
					if (!isdigit(expr[i + 1]))
					{
						cout << "Missing digits after the decimal point!" << endl;
						cout << infixExpr << endl;
						return false;
					}
				}
				val += expr[i];
				//always make prev the last digit coming
				prev = expr[i];
				i++;
			}
			while (expr[i] == ' ')
				i++;
			if (isdigit(expr[i]) || expr[i] == '.')
			{
				cout << "Redundant space between digits!" << endl;
				cur = expr[i];
				prev = ' ';
				displayErr(findErr());
				return false;
			}
			ss << val;
			ss >> operand;
			ss.clear();
			ee = exprElem(OPERAND, '\0', operand);
			postfixExpr.push_back(ee);
		}

		//operator
		else
		{
			prev = operators.top();
			if (isp[prev] < icp[cur])
			{
				operators.push(cur);
				i++;
			}
			else if (isp[prev] == icp[cur])
			{
				operators.pop();
				if (prev == '(')
					i++;
			}
			else
			{
				while (isp[prev] > icp[cur])
				{
					operators.pop();
					ee = exprElem(OPERATOR, prev, 0.0);
					postfixExpr.push_back(ee);
					prev = operators.top();
				}
				if (isp[prev] == icp[cur])
					operators.pop();
				else
					operators.push(cur);
				i++;
			}
			prev = cur;
		}
	}
	if (operators.empty())
		return true;
	else
		return false;
}

//检查括号匹配
bool Calculator::checkParen()
{
	stack<int> leftIndex;
	for (int i = 0; i < infixExpr.length(); ++i)
	{
		if (infixExpr[i] == '(')
			leftIndex.push(i);
		else if (infixExpr[i] == ')')
		{
			//redundant ')'
			if (leftIndex.empty())
			{
				cout << "Redundant right parentheses!" << endl;
				displayErr(i);
				return false;
			}
			leftIndex.pop();
		}
		else//ignore upper or lower case
		{
			infixExpr[i]=tolower(infixExpr[i]);
		}
	}
	//redundant '('
	if (!leftIndex.empty())
	{
		cout << "Redundant left parentheses!" << endl;
		displayErr(infixExpr.find_last_of('('));
		return false;
	}
	return true;
}

void Calculator::displayErr(unsigned int pos)
{
	//retype the expression and red the erroneous part
	for (int i = 0; i < infixExpr.length(); ++i)
	{
		if (i == pos)
		{
			redFont();
			cout << infixExpr[i];
			normalFont();
		}
		else
			cout << infixExpr[i];
	}
	cout << endl;
}

double Calculator::computePostfixExpr()
{
	stack<double> operands;
	double val, x, y;
	if (postfixExpr.size() == 1 && postfixExpr[0].et == OPERAND)
	{
		return postfixExpr[0].opd;
	}
	for (const auto& elem : postfixExpr)
	{
		if (elem.et == OPERAND)
			operands.push(elem.opd);
		else
		{
			const char& ch = elem.opt;
			if (ch == '_' || ch == 's' || ch == 'c' || ch == 't')
			{
				x = operands.top();
				operands.pop();
				val = op1[ch](x);
			}
			else
			{
				x = operands.top();
				operands.pop();
				y = operands.top();
				operands.pop();
				val = op2[ch](y, x);
			}
			operands.push(val);
		}
	}
	return val;
}

void Calculator::compute(string expr)
{
	infixExpr = expr;
	postfixExpr.clear();
	prev = '\n';
	if (checkParen() && in2postExpr())
	{
		result = computePostfixExpr();
		records.push_back(make_pair(infixExpr, result));
		if (records.size() > 5)
			records.erase(records.begin());
		cout << "The result is: "
			<< fixed << setprecision(8) << result << endl << endl;
	}
}

Calculator::Calculator()
{
	string expr;
	double result;
	fstream fin;
	fin.open("records.txt", ios::in);
	while (fin >> expr >> result)
		records.push_back(make_pair(expr + '\n', result));
	fin.close();
}

Calculator::~Calculator()
{
	fstream fout;
	fout.open("records.txt", ios::out);
	for (auto& r : records)
		fout << r.first << r.second << endl;
	fout.close();
}

void Calculator::displayRecords()
{
	int pos = records.size();
	if (pos == 0)
	{
		cout << "No record available!" << endl << endl;
		return;
	}

	cout << "Please check your computation records using UP/DOWN Arrow:" << endl
		<< "(Press ESC to leave...)" << endl << endl;

	while (true)
	{
		// Esc
		if (_getch() == 27)
		{
			cout << endl << endl;
			return;
		}
		switch (_getch())
		{
		case 72: // ↑
			pos = (pos - 1 + records.size()) % records.size();
			break;
		case 80: // ↓
			pos = (pos + 1 + records.size()) % records.size();
			break;
		default:
			pos = (pos + records.size()) % records.size();
			break;
		}
		cout << "expression: " << records[pos].first
			<< "result:" << fixed << setprecision(8) << records[pos].second
			<< endl << endl;
	}
}