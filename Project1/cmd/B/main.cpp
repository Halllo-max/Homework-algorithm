#include "common/Stack.h"
#include "common/Tools.h"
#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include <cctype>
#include <filesystem>
#include <fstream>
#include <stdexcept>

const int MAXTOKEN = 1e5 + 10;

int level(std::string c) {
    if (c == "+" || c == "-") {
        return 1;
    } else if (c == "*" || c == "/") {
        return 2;
    } else {
        return 0;
    }
}

double cal(std::string c, double a, double b) {
    if (c == "+") return a + b;
    if (c == "-") return a - b;
    if (c == "*") return a * b;
    if (c == "/") {
        if (b == 0) {
            throw std::runtime_error("除零错误");
        }
        return a / b;
    }
    return 0;
}

int Spilt(std::string &s, std::string tokens[], int maxTokens) {
    int cnt = 0;
    int i = 0;
    while (i < s.length() && cnt < maxTokens) {
        if (isspace(s[i])) {
            i++;
            continue;
        }

        if (isdigit(s[i]) || (s[i] == '.' && i + 1 < s.length() && isdigit(s[i + 1]))) {
            std::string num;
            bool isFloat = false;
            while (i < s.length() && (isdigit(s[i]) || (s[i] == '.' && !isFloat))) {
                if (s[i] == '.') isFloat = true;
                num += s[i];
                i++;
            }
            tokens[cnt++] = num;
            continue;
        }

        if (s[i] == '+' || s[i] == '-' || s[i] == '*' || s[i] == '/' ||
            s[i] == '(' || s[i] == ')') {
            tokens[cnt++] = std::string(1, s[i]);
            i++;
        } else {
            throw std::runtime_error("无效字符: " + std::string(1, s[i]));
        }
    }
    return cnt;
}

double EvaluateExpression(const std::string &input) {
    Stack<std::string> op_stack;
    Stack<double> num_stack;
    int maxTokens = input.length();
    std::string tokens[MAXTOKEN];

    int cnt = Spilt(const_cast<std::string&>(input), tokens, maxTokens);

    for (int i = 0; i < cnt; i++) {
        std::string c = tokens[i];
        if (c == "+" || c == "-" || c == "*" || c == "/") {
            while (!op_stack.isEmpty() && level(op_stack.Top()) >= level(c) && op_stack.Top() != "(") {
                std::string c1 = op_stack.Top(); op_stack.Pop();
                if (num_stack.Length() < 2) {
                    throw std::runtime_error("表达式语法错误: 操作数不足");
                }
                double n1 = num_stack.Top(); num_stack.Pop();
                double n2 = num_stack.Top(); num_stack.Pop();
                double n3 = cal(c1, n2, n1);
                num_stack.Push(n3);
            }
            op_stack.Push(c);
        } else if (c == "(") {
            op_stack.Push(c);
        } else if (c == ")") {
            while (!op_stack.isEmpty() && op_stack.Top() != "(") {
                std::string c1 = op_stack.Top(); op_stack.Pop();
                if (num_stack.Length() < 2) {
                    throw std::runtime_error("表达式语法错误: 操作数不足");
                }
                double n1 = num_stack.Top(); num_stack.Pop();
                double n2 = num_stack.Top(); num_stack.Pop();
                double n3 = cal(c1, n2, n1);
                num_stack.Push(n3);
            }
            if (op_stack.isEmpty()) {
                throw std::runtime_error("括号不匹配: 缺少左括号");
            }
            op_stack.Pop();
        } else {
            try {
                num_stack.Push(std::stod(c));
            } catch (const std::invalid_argument&) {
                throw std::runtime_error("无效数字: " + c);
            } catch (const std::out_of_range&) {
                throw std::runtime_error("数字超出范围: " + c);
            }
        }
    }

    while (!op_stack.isEmpty()) {
        std::string c1 = op_stack.Top(); op_stack.Pop();
        if (num_stack.Length() < 2) {
            throw std::runtime_error("表达式语法错误: 操作数不足");
        }
        double n1 = num_stack.Top(); num_stack.Pop();
        double n2 = num_stack.Top(); num_stack.Pop();
        double n3 = cal(c1, n2, n1);
        num_stack.Push(n3);
    }

    if (num_stack.isEmpty()) {
        throw std::runtime_error("表达式为空");
    }

    return num_stack.Top();
}

int main() {
    std::filesystem::path sourcePath = __FILE__;
    std::filesystem::path filePath = sourcePath.parent_path() / "test_cases.txt";

    if (!std::filesystem::exists(filePath)) {
        std::cerr << "错误：测试文件不存在: '" << filePath << "'" << std::endl;
        std::cerr << "当前工作目录: " << std::filesystem::current_path() << std::endl;
        return 1;
    }

    std::ifstream testFile(filePath);
    if (!testFile.is_open()) {
        std::cerr << "无法打开测试文件: '" << filePath << "'。请检查文件权限。" << std::endl;
        return 1;
    }

    std::string line;
    int caseNo = 1;
    while (std::getline(testFile, line)) {
        if (line.empty() || line[0] == '#') continue;
        
        std::cout << "\n=== 测试用例 " << caseNo++ << " ===" << std::endl;
        std::cout << "表达式: " << line << std::endl;
        
        try {
            double res = EvaluateExpression(line);
            std::cout << "结果: " << res << std::endl;
        } catch (const std::exception& e) {
            std::cerr << "错误: " << e.what() << std::endl;
        }
    }

    testFile.close();
    return 0;
}