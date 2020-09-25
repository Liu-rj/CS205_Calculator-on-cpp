#include <iostream>
#include <string>
#include <cmath>
#include <vector>
#include <sstream>

using namespace std;

bool isNum(string s);

double multiply(double double1, double double2);

vector<string> judge(string str, vector<string> elements);

bool priorThan(string a, string b);

double stringToNum(string str);

int main() {
    cout << "Please write your calculation below: " << endl;
    cout << "(input * to exit)" << endl;
    string str;
    cin >> str;
    while (str != "*") {
        vector<string> elements;
        vector<string> str1;
        vector<string> str2;
        vector<double> nums;
        elements = judge(str, elements);
        //判断用户输入并进行数据初始化操作，分解数据并赋值给elements
        while (elements.empty()) {
            cin >> str;
            elements = judge(str, elements);
        }
        //将中缀表达式转换为逆波兰表达式
        for (int i = 0; i < elements.size(); ++i) {
            if (isNum(elements.at(i))) {
                //如果为数字，则按顺序存储在str1当中
                str1.push_back(elements.at(i));
            } else {
                if (str2.empty() || priorThan(elements.at(i), str2[str2.size() - 1]) || str2[str2.size() - 1] == "(") {
                    //如果不为数字，先判断其与str2头元素的优先级大小，如果优先级低，则顺序排列，若优先级高，则反序输出str2中的元素，然后将其存入
                    str2.push_back(elements.at(i));
                } else {
                    long times = 0;
                    for (long j = str2.size() - 1; j >= 0; --j) {
                        if (priorThan(str2.at(j), elements.at(i)) && str2.at(j) != "(" && str2.at(j) != ")") {
                            str1.push_back(str2.at(j));
                            str2.pop_back();
                        } else {
                            if ((str2.at(j) == ")" || times != 0) && str2.at(j) != "(") {
                                if (str2.at(j) == ")") {
                                    str2.pop_back();
                                    times++;
                                } else {
                                    str1.push_back(str2.at(j));
                                }
                            } else if (str2.at(j) == "(") {
                                times--;
                                str2.pop_back();
                            }
                        }
                    }
                    str2.push_back(elements.at(i));
                }
            }
        }
        //完成逆波兰表达式的最后一步，将str2剩下的元素倒序赋值给str1
        if (!str2.empty()) {
            for (long i = str2.size() - 1; i >= 0; i--) {
                if (str2.at(i) != "(" && str2.at(i) != ")") {
                    str1.push_back(str2.at(i));
                }
            }
        }
        cout << "Reverse Polish Notation: ";
        for (string string1 : str1) {
            cout << string1 << " ";
        }
        cout << endl;
        //开始计算，遍历str1
        for (int i = 0; i < str1.size(); ++i) {
            double temp = 0;
            if (isNum(str1.at(i))) {
                nums.push_back(stringToNum(str1.at(i)));
            } else {
                //检索到运算符，在这里对nums的头两个元素做运算
                if (str1.at(i) == "+") {
                    temp = nums.at(nums.size() - 1) + nums.at(nums.size() - 2);
                    nums.pop_back();
                    nums.pop_back();
                    nums.push_back(temp);
                } else if (str1.at(i) == "-") {
                    temp = nums.at(nums.size() - 1) - nums.at(nums.size() - 2);
                    nums.pop_back();
                    nums.pop_back();
                    nums.push_back(temp);
                } else if (str1.at(i) == "*") {
                    temp = nums.at(nums.size() - 1) * nums.at(nums.size() - 2);
                    //temp = multiply(nums.at(nums.size() - 1), nums.at(nums.size() - 2));
                    nums.pop_back();
                    nums.pop_back();
                    nums.push_back(temp);
                } else if (str1.at(i) == "/") {
                    temp = nums.at(nums.size() - 2) / nums.at(nums.size() - 1);
                    nums.pop_back();
                    nums.pop_back();
                    nums.push_back(temp);
                } else if (str1.at(i) == "^") {
                    temp = pow(nums.at(nums.size() - 2), nums.at(nums.size() - 1));
                    nums.pop_back();
                    nums.pop_back();
                    nums.push_back(temp);
                } else if (str1.at(i) == "%") {
                    temp = fmod(nums.at(nums.size() - 2), nums.at(nums.size() - 1));
                    nums.pop_back();
                    nums.pop_back();
                    nums.push_back(temp);
                }
            }
        }
        cout << "result: " << nums.at(0) << endl;
        cin >> str;
    }
}

double stringToNum(string str) {
    istringstream iss(str);
    double num;
    iss >> num;
    return num;
}

vector<string> judge(string str, vector<string> elements) {
    char element;
    for (int i = 0; i < str.size(); i++) {
        element = str[i];
        if (element == '+' || element == '-' || element == '*' || element == '/' || element == '^' || element == '%' ||
            element == '(' || element == ')' || element == '.' || (element >= 48 && element <= 57)) {
            if ((element == '-' || element == '+') && str[i - 1] == '(') {
                elements.emplace_back("0");
                elements.push_back(str.substr(i, 1));
            } else if ((element >= 48 && element <= 57) || element == '.') {
                if (i == str.size() - 1) {
                    elements.push_back(str.substr(i, 1));
                    break;
                } else {
                    for (int j = i + 1; j < str.size(); ++j) {
                        if (j == str.size() - 1) {
                            elements.push_back(str.substr(i, str.size() - i));
                            i = j;
                        } else if ((str[j] < 48 || str[j] > 57) && str[j] != '.') {
                            elements.push_back(str.substr(i, j - i));
                            i = j - 1;
                            break;
                        }
                    }
                }
            } else {
                elements.push_back(str.substr(i, 1));
            }
        } else {
            cout << "wrong input!" << endl << "Please input again: " << endl;
            elements.clear();
        }
    }
    return elements;
}

bool isNum(string s) {
    stringstream sin(s);
    double t;
    char p;
    if (!(sin >> t))
        //sin>>t表示把sin转换成double的变量（其实对于int和float型的都会接收），如果转换成功，则值为非0，如果转换不成功就返回为0
        return false;
    if (sin >> p)
        //检测错误输入中，数字加字符串的输入形式（例如：34.f）
        return false;
    else
        return true;
}

bool priorThan(string a, string b) {
    if (a == "(" || a == ")") {
        return true;
    } else if (a == "^" && b != "(" && b != ")") {
        return true;
    } else if ((a == "*" || a == "/" || a == "%") && b != "^" && b != "(" && b != ")") {
        return true;
    } else if ((a == "+" || a == "-") && (b == "+" || b == "-")) {
        return true;
    } else {
        return false;
    }
}

double multiply(double double1, double double2) {
    string a = to_string(double1);
    string b = to_string(double2);
    int *arr1 = new int[a.size()]();
    int *arr2 = new int[b.size()]();
    int *result = new int[a.size() + b.size()]{};
    string str;
    for (int i = 0; i < a.size(); i++) {
        if (a[i] < '0' || a[i] > '9') {
            cout << "your input is wrong!";
            return 0;
        }
        arr1[i] = a[i] - '0';
    }
    for (int i = 0; i < b.size(); i++) {
        if (b[i] < '0' || b[i] > '9') {
            cout << "your input is wrong!";
            return 0;
        }
        arr2[i] = b[i] - '0';
    }
    for (int i = a.size() - 1; i >= 0; i--) {
        for (int j = b.size() - 1; j >= 0; j--) {
            result[a.size() - 1 - i + b.size() - 1 - j] += (arr1[i] * arr2[j]) % 10;
            if (result[a.size() - 1 - i + b.size() - 1 - j] >= 10) {
                result[a.size() - 1 - i + b.size() - j] += result[a.size() - 1 - i + b.size() - 1 - j] / 10;
                result[a.size() - 1 - i + b.size() - 1 - j] = result[a.size() - 1 - i + b.size() - 1 - j] % 10;
            }
            result[a.size() - 1 - i + b.size() - j] += (arr1[i] * arr2[j]) / 10;
            if (result[a.size() - 1 - i + b.size() - j] >= 10) {
                result[a.size() - i + b.size() - j] += result[a.size() - 1 - i + b.size() - j] / 10;
                result[a.size() - 1 - i + b.size() - j] = result[a.size() - 1 - i + b.size() - j] % 10;
            }
        }
    }
    if (result[a.size() + b.size() - 1] != 0) {
        for (int i = a.size() + b.size() - 1; i >= 0; i--) {
            str.append(to_string(result[i]));

        }
    } else {
        for (int i = a.size() + b.size() - 2; i >= 0; i--) {
            str.append(to_string(result[i]));
        }
    }
    return stringToNum(str);
}