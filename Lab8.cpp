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

numAsString prepareNumber(string num) {
	numAsString result;
	result.sign = '+';
	if(num[0] == '+' || num[0] == '-') {
		result.sign = num[0];
		num.erase(0, 1);
	}
	size_t decimal = num.find('.');
	result.decimalPosition = decimal == string::npos ? num.length() : static_cast<int>(decimal);
	if(decimal != string::npos) {
		num.erase(decimal, 1);
	}
	result.number = num;
	return result;
}

bool isValidNumber(const string& num) {
	if(num.empty()) {
		return false;
	}
	size_t start = 0;
	if(num[0] == '+' || num[0] == '-') {
		start = 1;
	}
	if(start == num.length()) {
		return false;
	}
	size_t decimal = string::npos;
	for(size_t i = start; i < num.length(); i++) {
		if(num[i] == '.') {
			if(decimal != string::npos || i == start || i == num.length() - 1) {
				return false;
			}
			decimal = i;
		} else if(num[i] < '0' || num[i] > '9') {
			return false;
		}
	}
	return true;
}

string normalizeDigits(string digits) {
	size_t first = digits.find_first_not_of('0');
	if(first == string::npos) {
		return "0";
	}
	return digits.substr(first);
}

string formatResult(string digits, int decimalPosition, char sign) {
	if(digits.find_first_not_of('0') == string::npos) {
		return "0";
	}
	while(decimalPosition < 0) {
		digits.insert(digits.begin(), '0');
		decimalPosition++;
	}
	if(decimalPosition == 0) {
		digits.insert(0, "0.");
	} else if(decimalPosition < static_cast<int>(digits.length())) {
		digits.insert(static_cast<size_t>(decimalPosition), ".");
	} else if(decimalPosition > static_cast<int>(digits.length())) {
		digits.append(static_cast<size_t>(decimalPosition - digits.length()), '0');
	}
	size_t dot = digits.find('.');
	if(dot != string::npos) {
		while(digits.back() == '0') {
			digits.pop_back();
		}
		if(digits.back() == '.') {
			digits.pop_back();
		}
		size_t integerEnd = digits.find('.');
		if(integerEnd == string::npos) {
			integerEnd = digits.length();
		}
		size_t first = digits.find_first_not_of('0');
		if(first != string::npos && first < integerEnd) {
			digits.erase(0, first);
		}
	} else {
		size_t first = digits.find_first_not_of('0');
		if(first != string::npos) {
			digits.erase(0, first);
		}
	}
	if(digits.empty() || digits.find_first_not_of("0.") == string::npos) {
		return "0";
	}
	if(sign == '-') {
		digits.insert(digits.begin(), '-');
	}
	return digits;
}

int compareMagnitude(string a, string b) {
	numAsString x = prepareNumber(a);
	numAsString y = prepareNumber(b);
	int dx = static_cast<int>(x.number.length()) - x.decimalPosition;
	int dy = static_cast<int>(y.number.length()) - y.decimalPosition;
	if(dx < dy) {
		x = numAsString{x.number, x.sign, x.decimalPosition};
		x.number.append(static_cast<size_t>(dy - dx), '0');
	} else if(dy < dx) {
		y.number.append(static_cast<size_t>(dx - dy), '0');
	}
	x.number = normalizeDigits(x.number);
	y.number = normalizeDigits(y.number);
	if(x.number.length() < y.number.length()) {
		return -1;
	}
	if(x.number.length() > y.number.length()) {
		return 1;
	}
	if(x.number < y.number) {
		return -1;
	}
	if(x.number > y.number) {
		return 1;
	}
	return 0;
}

string addMagnitude(string a, string b, int& decimalPosition) {
	numAsString x = prepareNumber(a);
	numAsString y = prepareNumber(b);
	int decimalsX = static_cast<int>(x.number.length()) - x.decimalPosition;
	int decimalsY = static_cast<int>(y.number.length()) - y.decimalPosition;
	int decimals = max(decimalsX, decimalsY);
	x.number.append(static_cast<size_t>(decimals - decimalsX), '0');
	y.number.append(static_cast<size_t>(decimals - decimalsY), '0');
	decimalPosition = static_cast<int>(x.number.length()) - decimals;
	if(x.number.length() < y.number.length()) {
		x.number.insert(0, y.number.length() - x.number.length(), '0');
	} else if(y.number.length() < x.number.length()) {
		y.number.insert(0, x.number.length() - y.number.length(), '0');
	}
	string result;
	int carry = 0;
	for(int i = static_cast<int>(x.number.length()) - 1; i >= 0; i--) {
		int sum = x.number[i] - '0' + y.number[i] - '0' + carry;
		result.push_back(static_cast<char>('0' + sum % 10));
		carry = sum / 10;
	}
	if(carry) {
		result.push_back(static_cast<char>('0' + carry));
		decimalPosition++;
	}
	int left = 0;
	int right = static_cast<int>(result.length()) - 1;
	while (left < right) {
		char temp = result[left];
		result[left] = result[right];
		result[right] = temp;
		left++;
		right--;
	}
	return result;
}

string subtractMagnitude(string a, string b, int& decimalPosition) {
	numAsString x = prepareNumber(a);
	numAsString y = prepareNumber(b);
	int decimalsX = static_cast<int>(x.number.length()) - x.decimalPosition;
	int decimalsY = static_cast<int>(y.number.length()) - y.decimalPosition;
	int decimals = max(decimalsX, decimalsY);
	x.number.append(static_cast<size_t>(decimals - decimalsX), '0');
	y.number.append(static_cast<size_t>(decimals - decimalsY), '0');
	decimalPosition = static_cast<int>(x.number.length()) - decimals;
	if(x.number.length() < y.number.length()) {
		x.number.insert(0, y.number.length() - x.number.length(), '0');
	} else if(y.number.length() < x.number.length()) {
		y.number.insert(0, x.number.length() - y.number.length(), '0');
	}
	string result;
	int borrow = 0;
	for(int i = static_cast<int>(x.number.length()) - 1; i >= 0; i--) {
		int digit = x.number[i] - '0' - borrow - (y.number[i] - '0');
		borrow = digit < 0;
		if(borrow) {
			digit += 10;
		}
		result.push_back(static_cast<char>('0' + digit));
	}
	int left = 0;
	int right = static_cast<int>(result.length()) - 1;
	while (left < right) {
		char temp = result[left];
		result[left] = result[right];
		result[right] = temp;
		left++;
		right--;
	}	
	size_t first = result.find_first_not_of('0');
	if(first == string::npos) {
		return "0";
	}
	result.erase(0, first);
	decimalPosition -= static_cast<int>(first);
	return result;
}

string addNumbers(string num1, string num2) {
	numAsString x = prepareNumber(num1);
	numAsString y = prepareNumber(num2);
	int decimalPosition = 0;
	string result;
	char sign = '+';
	if(x.sign == y.sign) {
		result = addMagnitude(num1, num2, decimalPosition);
		sign = x.sign;
	} else {
		int comparison = compareMagnitude(num1, num2);
		if(comparison == 0) {
			return "0";
		}
		if(comparison > 0) {
			result = subtractMagnitude(num1, num2, decimalPosition);
			sign = x.sign;
		} else {
			result = subtractMagnitude(num2, num1, decimalPosition);
			sign = y.sign;
		}
	}
	return formatResult(result, decimalPosition, sign);
}

string subtractNumbers(string num1, string num2) {
	if(!num2.empty() && num2[0] == '-') {
		num2[0] = '+';
	} else if(!num2.empty() && num2[0] == '+') {
		num2[0] = '-';
	} else {
		num2.insert(num2.begin(), '-');
	}
	return addNumbers(num1, num2);
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
			cout << "line " << linenum << ": ERROR: expected exactly two numbers" << endl;
		} else if(!isValidNumber(a) && !isValidNumber(b)) {
			cout << "line " << linenum << ": ERROR: invalid number \"" << a << "\" \"" << b << "\"" << endl;
		} else if(!isValidNumber(a)) {
			cout << "line " << linenum << ": ERROR: invalid number \"" << a << "\"" << endl;
		} else if(!isValidNumber(b)) {
			cout << "line " << linenum << ": ERROR: invalid number \"" << b << "\"" << endl;
		} else {
			cout << "line " << linenum << ": " << a << " + " << b << " = " << addNumbers(a, b) << endl;
		}
		linenum++;
	}
}

int main() {
	string filename;
	cout << "Enter name of file: ";
	cin >> filename;
	calculateFromFile(filename);
	return 0;
}

