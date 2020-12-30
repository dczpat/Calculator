#include "Calculator.h"

int main()
{
	string expr; // cos(-sin(pi/2)+1)+((-8+3)/5*1)
	Calculator cal = Calculator();
	cout << "Please enter your expression:" << endl
		<< "(enter q to leave or r for records...)" << endl;
	while (true)
	{
		getline(cin, expr);
		cout << endl;
		if (expr == "q")
			break;
		if (expr == "r")
			cal.displayRecords();
		else
		{
			expr += '\n';
			cal.compute(expr);
		}
		cout << "Please enter your expression:" << endl
			<< "(enter q to leave or r for records...)" << endl;
	}

	return 0;
}