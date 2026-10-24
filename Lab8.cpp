#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;

class numAsString {
	public:
		string number;
		char sign;
		int decimalPosition;
};

numAsString padNumBack(numAsString num, int zerosToAdd) {
	for(int i = 0; i < zerosToAdd; i++) {
		num.number += '0';
	}
	return num;
}

numAsString padNumFront(numAsString num, int zerosToAdd) {
	for(int i = 0; i < zerosToAdd; i++) {
		num.number = '0' + num.number;
	}
	num.decimalPosition += zerosToAdd;
	return num;
}

numAsString prepareNumber(string num) {
	numAsString fixedNum;
	char sign;
	size_t decimalPosition = num.find('.');
	if(decimalPosition == string::npos) {
		decimalPosition = num.length();
	} else {
		num.erase(decimalPosition, 1);
	}
	if(num[0] == '-') {
		sign = '-';
		num.erase(0, 1);
		decimalPosition--;
	} else {
		sign = '+';
		if(num[0] == '+') {
			num.erase(0, 1);
			decimalPosition--;
		}
	}
	fixedNum.number = num;
	fixedNum.sign = sign;
	fixedNum.decimalPosition = decimalPosition;
	return fixedNum;
}

string subtractNumbers(string num1, string num2);

string addNumbers(string num1, string num2) {
	numAsString fixedNum1 = prepareNumber(num1);
	numAsString fixedNum2 = prepareNumber(num2);

	if(fixedNum1.sign != fixedNum2.sign) {
		if(fixedNum1.sign == '-') {
			num1.erase(0, 1);
			num1 = "-" + num1;
		}
		if(fixedNum2.sign == '-') {
			num2.erase(0, 1);
		} else {
			num2 = "-" + num2;
		}
		return subtractNumbers(num1, num2);
	}

	int decimals1 = fixedNum1.number.length() - fixedNum1.decimalPosition;
	int decimals2 = fixedNum2.number.length() - fixedNum2.decimalPosition;

	if(decimals1 < decimals2) {
		fixedNum1 = padNumBack(fixedNum1, decimals2 - decimals1);
	} else {
		fixedNum2 = padNumBack(fixedNum2, decimals1 - decimals2);
	}

	if(fixedNum1.number.length() < fixedNum2.number.length()) {
		fixedNum1 = padNumFront(fixedNum1, fixedNum2.number.length() - fixedNum1.number.length());
	} else {
		fixedNum2 = padNumFront(fixedNum2, fixedNum1.number.length() - fixedNum2.number.length());
	}

	int sum = 0;
	string result = "";
	int carry = 0;

	for(int i = fixedNum1.number.length(); i > 0; i--) {
		sum = (fixedNum1.number[i - 1] - '0') + (fixedNum2.number[i - 1] - '0') + carry;
		result += char('0' + sum % 10);
		carry = sum / 10;
	}

	if(carry > 0) {
		result += char('0' + carry);
	}

	int left = 0;
	int right = result.length() - 1;

	while(left < right) {
		char temp = result[left];
		result[left] = result[right];
		result[right] = temp;
		left++;
		right--;
	}

	result.insert(fixedNum1.decimalPosition, ".");

	if(fixedNum1.sign == '-') {
		result = "-" + result;
	}

	return result;
}

string subtractNumbers(string num1, string num2) {
	numAsString fixedNum1 = prepareNumber(num1);
	numAsString fixedNum2 = prepareNumber(num2);

	if(fixedNum1.sign != fixedNum2.sign) {
		if(fixedNum2.sign == '-') {
			num2.erase(0, 1);
		} else {
			num2 = "-" + num2;
		}
		return addNumbers(num1, num2);
	}

	int decimals1 = fixedNum1.number.length() - fixedNum1.decimalPosition;
	int decimals2 = fixedNum2.number.length() - fixedNum2.decimalPosition;

	if(decimals1 < decimals2) {
		fixedNum1 = padNumBack(fixedNum1, decimals2 - decimals1);
	} else {
		fixedNum2 = padNumBack(fixedNum2, decimals1 - decimals2);
	}

	if(fixedNum1.number.length() < fixedNum2.number.length()) {
		fixedNum1 = padNumFront(fixedNum1, fixedNum2.number.length() - fixedNum1.number.length());
	} else {
		fixedNum2 = padNumFront(fixedNum2, fixedNum1.number.length() - fixedNum2.number.length());
	}

	string result = "";
	int borrow = 0;

	for(int i = fixedNum1.number.length(); i > 0; i--) {
		int digit1 = fixedNum1.number[i - 1] - '0' - borrow;
		int digit2 = fixedNum2.number[i - 1] - '0';
		borrow = 0;

		if(digit1 < digit2) {
			digit1 += 10;
			borrow = 1;
		}

		result += char('0' + digit1 - digit2);
	}

	while(result.length() > 1 && result.back() == '0') {
		result.pop_back();
	}

	int left = 0;
	int right = result.length() - 1;

	while(left < right) {
		char temp = result[left];
		result[left] = result[right];
		result[right] = temp;
		left++;
		right--;
	}

	result.insert(fixedNum1.decimalPosition, ".");

	if(fixedNum1.sign == '-') {
		result = "-" + result;
	}

	return result;
}

bool isValidNumber(string num) {
	int numOfDecimals = 0;
	for(int i = 0; i < num.length(); i++) {
		char c = num[i];
		if(!(c >= '0' && c <= '9')) {
			if(c == '.') {
				if(i == 0) {
					return false;
				}
				numOfDecimals++;
			} else if(!(i == 0 && (c == '-' || c == '+'))) {
				return false;
			}
		}
	}
	if(numOfDecimals <= 1) {
		return true;
	}
	return false;
}

void calculateFromFile(string filename) {
	ifstream file(filename);
	string line;
	int linenum = 1;
	while(getline(file, line)) {
		stringstream input(line);
		string a, b, extra;
		if(!(input >> a >> b)) {
			cout << "line" << linenum << ": ERROR: expected exactly two numbers" << endl;
		} else if(input >> extra) {
			cout << "line" << linenum << ": ERROR: expected exactly two numbers" << endl;
		} else if(isValidNumber(a) && isValidNumber(b)) {
			cout << "line" << linenum << ": " << a << " + " << b << " = " << addNumbers(a, b) << endl;
		} else if(isValidNumber(a)) {
			cout << "line" << linenum << ": ERROR: invalid number \"" << b << "\"" << endl;
		} else {
			cout << "line" << linenum << ": ERROR: invalid number \"" << a << "\"" << endl;
		}
		linenum++;
	}
	file.close();
}

int main() {
	string filename;
	cout << "Enter name of file: ";
	cin >> filename;
	calculateFromFile(filename);
	return 0;
}

