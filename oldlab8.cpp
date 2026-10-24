#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
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
	fixedNum.sign = '+';
	size_t decimalPosition = num.find('.');
	if(decimalPosition == string::npos) {
		decimalPosition = num.length();
	} else {
		num.erase(decimalPosition, 1);
	}
	if(!num.empty() && (num[0] == '-' || num[0] == '+')) {
		fixedNum.sign = num[0];
		num.erase(0, 1);
		if(decimalPosition > 0) {
			decimalPosition--;
		}
	}
	fixedNum.number = num;
	fixedNum.decimalPosition = static_cast<int>(decimalPosition);
	return fixedNum;
}

string normalizeResult(string result, int decimalPosition, char sign) {
	while(result.length() > 1 && result[0] == '0' && decimalPosition > 1) {
		result.erase(0, 1);
		decimalPosition--;
	}
	if(result.find_first_not_of('0') == string::npos) {
		return "0";
	}
	if(decimalPosition <= 0) {
		result.insert(0, static_cast<size_t>(-decimalPosition + 1), '0');
		decimalPosition = 1;
	}
	if(decimalPosition >= static_cast<int>(result.length())) {
		result.append(static_cast<size_t>(decimalPosition - result.length()), '0');
	} else {
		result.insert(static_cast<size_t>(decimalPosition), ".");
	}
	while(result.find('.') != string::npos && result.back() == '0') {
		result.pop_back();
	}
	if(!result.empty() && result.back() == '.') {
		result.pop_back();
	}
	if(result.empty()) {
		return "0";
	}
	if(result.find_first_not_of("0.") == string::npos) {
		return "0";
	}
	if(sign == '-') {
		result.insert(0, "-");
	}
	return result;
}

int compareNumbers(const string& num1, const string& num2) {
	string a = num1;
	string b = num2;
	size_t dotA = a.find('.');
	size_t dotB = b.find('.');
	int decimalsA = dotA == string::npos ? 0 : static_cast<int>(a.length() - dotA - 1);
	int decimalsB = dotB == string::npos ? 0 : static_cast<int>(b.length() - dotB - 1);
	if(dotA != string::npos) {
		a.erase(dotA, 1);
	}
	if(dotB != string::npos) {
		b.erase(dotB, 1);
	}
	while(decimalsA < decimalsB) {
		a += '0';
		decimalsA++;
	}
	while(decimalsB < decimalsA) {
		b += '0';
		decimalsB++;
	}
	size_t firstA = a.find_first_not_of('0');
	size_t firstB = b.find_first_not_of('0');
	a = firstA == string::npos ? "0" : a.substr(firstA);
	b = firstB == string::npos ? "0" : b.substr(firstB);
	if(a.length() < b.length()) {
		return -1;
	}
	if(a.length() > b.length()) {
		return 1;
	}
	if(a < b) {
		return -1;
	}
	if(a > b) {
		return 1;
	}
	return 0;
}

string addNumbers(string num1, string num2);

string subtractNumbers(string num1, string num2) {
	numAsString fixedNum1 = prepareNumber(num1);
	numAsString fixedNum2 = prepareNumber(num2);
	int decimals1 = static_cast<int>(fixedNum1.number.length()) - fixedNum1.decimalPosition;
	int decimals2 = static_cast<int>(fixedNum2.number.length()) - fixedNum2.decimalPosition;
	if(fixedNum1.sign != fixedNum2.sign) {
		if(fixedNum2.sign == '-') {
			fixedNum2.sign = '+';
		} else {
			fixedNum2.sign = '-';
		}
		return addNumbers(num1, fixedNum2.sign == '-' ? "-" + fixedNum2.number : fixedNum2.number);
	}
	int comparison = compareNumbers(num1, num2);
	if(comparison == 0) {
		return "0";
	}
	bool negative = fixedNum1.sign == '-';
	if(comparison < 0) {
		swap(fixedNum1, fixedNum2);
		negative = !negative;
	}
	if(decimals1 < decimals2) {
		fixedNum1 = padNumBack(fixedNum1, decimals2 - decimals1);
	} else {
		fixedNum2 = padNumBack(fixedNum2, decimals1 - decimals2);
	}
	if(fixedNum1.number.length() < fixedNum2.number.length()) {
		fixedNum1 = padNumFront(fixedNum1, static_cast<int>(fixedNum2.number.length() - fixedNum1.number.length()));
	} else {
		fixedNum2 = padNumFront(fixedNum2, static_cast<int>(fixedNum1.number.length() - fixedNum2.number.length()));
	}
	string result;
	int borrow = 0;
	for(int i = static_cast<int>(fixedNum1.number.length()) - 1; i >= 0; i--) {
		int digit1 = fixedNum1.number[i] - '0' - borrow;
		int digit2 = fixedNum2.number[i] - '0';
		borrow = 0;
		if(digit1 < digit2) {
			digit1 += 10;
			borrow = 1;
		}
		result += char('0' + digit1 - digit2);
	}
	int left = 0;
	int right = static_cast<int>(result.length()) - 1;
	while(left < right) {
		swap(result[left], result[right]);
		left++;
		right--;
	}
	int decimalPosition = fixedNum1.decimalPosition;
	while(result.length() > 1 && result[0] == '0' && decimalPosition > 1) {
		result.erase(0, 1);
		decimalPosition--;
	}
	while(result.length() > 1 && result.back() == '0' && static_cast<int>(result.length()) > decimalPosition) {
		result.pop_back();
	}
	if(result.find_first_not_of('0') == string::npos) {
		return "0";
	}
	if(decimalPosition <= 0) {
		result.insert(0, static_cast<size_t>(1 - decimalPosition), '0');
		decimalPosition = 1;
	}
	if(decimalPosition >= static_cast<int>(result.length())) {
		result.append(static_cast<size_t>(decimalPosition - result.length()), '0');
	} else {
		result.insert(static_cast<size_t>(decimalPosition), ".");
	}
	while(result.find('.') != string::npos && result.back() == '0') {
		result.pop_back();
	}
	if(!result.empty() && result.back() == '.') {
		result.pop_back();
	}
	if(negative) {
		result.insert(0, "-");
	}
	return result;
}

string addNumbers(string num1, string num2) {
	numAsString fixedNum1 = prepareNumber(num1);
	numAsString fixedNum2 = prepareNumber(num2);
	int decimals1 = static_cast<int>(fixedNum1.number.length()) - fixedNum1.decimalPosition;
	int decimals2 = static_cast<int>(fixedNum2.number.length()) - fixedNum2.decimalPosition;
	if(fixedNum1.sign != fixedNum2.sign) {
		string positive1 = fixedNum1.number;
		string positive2 = fixedNum2.number;
		if(fixedNum1.sign == '-') {
			positive1.insert(0, "-");
		}
		if(fixedNum2.sign == '-') {
			positive2.insert(0, "-");
		}
		return subtractNumbers(positive1, fixedNum1.sign == '-' ? positive2 : positive2);
	}
	if(decimals1 < decimals2) {
		fixedNum1 = padNumBack(fixedNum1, decimals2 - decimals1);
	} else {
		fixedNum2 = padNumBack(fixedNum2, decimals1 - decimals2);
	}
	if(fixedNum1.number.length() < fixedNum2.number.length()) {
		fixedNum1 = padNumFront(fixedNum1, static_cast<int>(fixedNum2.number.length() - fixedNum1.number.length()));
	} else {
		fixedNum2 = padNumFront(fixedNum2, static_cast<int>(fixedNum1.number.length() - fixedNum2.number.length()));
	}
	string result;
	int carry = 0;
	for(int i = static_cast<int>(fixedNum1.number.length()) - 1; i >= 0; i--) {
		int sum = fixedNum1.number[i] - '0' + fixedNum2.number[i] - '0' + carry;
		result += char('0' + sum % 10);
		carry = sum / 10;
	}
	if(carry > 0) {
		result += char('0' + carry);
	}
	int left = 0;
	int right = static_cast<int>(result.length()) - 1;
	while(left < right) {
		swap(result[left], result[right]);
		left++;
		right--;
	}
	int decimalPosition = fixedNum1.decimalPosition;
	if(carry > 0) {
		decimalPosition++;
	}
	while(result.length() > 1 && result[0] == '0' && decimalPosition > 1) {
		result.erase(0, 1);
		decimalPosition--;
	}
	if(decimalPosition <= 0) {
		result.insert(0, static_cast<size_t>(1 - decimalPosition), '0');
		decimalPosition = 1;
	}
	if(decimalPosition >= static_cast<int>(result.length())) {
		result.append(static_cast<size_t>(decimalPosition - result.length()), '0');
	} else {
		result.insert(static_cast<size_t>(decimalPosition), ".");
	}
	while(result.find('.') != string::npos && result.back() == '0') {
		result.pop_back();
	}
	if(!result.empty() && result.back() == '.') {
		result.pop_back();
	}
	if(result.find_first_not_of("0.") == string::npos) {
		return "0";
	}
	if(fixedNum1.sign == '-') {
		result.insert(0, "-");
	}
	return result;
}

bool isValidNumber(string num) {
	if(num.empty()) {
		return false;
	}
	size_t i = 0;
	if(num[i] == '+' || num[i] == '-') {
		i++;
	}
	bool hasDigit = false;
	bool hasDecimal = false;
	for(; i < num.length(); i++) {
		char c = num[i];
		if(c >= '0' && c <= '9') {
			hasDigit = true;
		} else if(c == '.' && !hasDecimal) {
			hasDecimal = true;
		} else {
			return false;
		}
	}
	return hasDigit;
}

void calculateFromFile(string filename) {
	ifstream file(filename);
	if(!file) {
		cout << "ERROR: could not open file \"" << filename << "\"" << endl;
		return;
	}
	string line;
	int linenum = 1;
	while(getline(file, line)) {
		stringstream input(line);
		string a, b, extra;
		if(!(input >> a >> b) || (input >> extra)) {
			cout << "line" << linenum << ": ERROR: expected exactly two numbers" << endl;
		} else if(!isValidNumber(a)) {
			cout << "line" << linenum << ": ERROR: invalid number \"" << a << "\"" << endl;
		} else if(!isValidNumber(b)) {
			cout << "line" << linenum << ": ERROR: invalid number \"" << b << "\"" << endl;
		} else {
			cout << "line" << linenum << ": " << a << " + " << b << " = " << addNumbers(a, b) << endl;
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
