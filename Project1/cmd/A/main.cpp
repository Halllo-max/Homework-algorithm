#include "common/SeqList.h"
#include "common/Tools.h"
#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include <cmath>
#include <fstream>
#include <filesystem>

// 最大括号数量
const int MAX_PARENTHESES = 100;

// 多项式项结构体
struct PolyTerm {
    double coeff;
    int power;
};

// 比较幂次大小
bool compareTerms(const PolyTerm& a, const PolyTerm& b) {
    return a.power < b.power;
}

// 解析多项式输入
void ParsePolynomial(const std::string& input, SeqList<double>& coeffs, SeqList<int>& powers) {
    std::istringstream iss(input);
    std::string token;
    
    while (iss >> token) {
        // 判断系数正负
        if (token == "+") continue;
        bool negative = false;
        if (token[0] == '-') {
            negative = true;
            token = token.substr(1);
        }
        
        size_t x_pos = token.find('x');
        double coeff;
        int power = 0;
        
        if (x_pos == std::string::npos) {
            // 常数项
            coeff = ToDouble(token);
            power = 0;
        } else {
            // 处理系数
            if (x_pos == 0) {
                coeff = 1.0; // 针对幂次为0的特殊判断
            } else {
                coeff = ToDouble(token.substr(0, x_pos));
            }
            
            // 处理指数
            if (x_pos == token.length() - 1) {
                power = 1;
            } else if (token[x_pos + 1] == '^') {
                power = std::stoi(token.substr(x_pos + 2));
            } else {
                power = 1;
            }
        }
        
        if (negative) coeff = -coeff; // 转换负系数
        
        coeffs.Push_back(coeff);
        powers.Push_back(power);
    }
}

// 合并同类项
void Combine(SeqList<double>& coeffs, SeqList<int>& powers) {
    // 转换为数组
    PolyTerm terms[100];
    int termCount = coeffs.Length();
    for (int i = 0; i < termCount; ++i) {
        terms[i].coeff = coeffs.Get(i);
        terms[i].power = powers.Get(i);
    }
    
    // 按幂次排序
    std::sort(terms, terms + termCount, compareTerms);
    
    // 合并同类项
    int newTermCount = 0;
    for (int i = 0; i < termCount; ) {
        double sum = terms[i].coeff;
        int j = i + 1;
        while (j < termCount && terms[j].power == terms[i].power) {
            sum += terms[j].coeff;
            j++;
        }
        
        // 防止 double 计算存在误差
        if (fabs(sum) > 1e-10) {  // 忽略极小系数
            terms[newTermCount].coeff = sum;
            terms[newTermCount].power = terms[i].power;
            newTermCount++;
        }
        
        i = j;
    }
    
    // 更新回SeqList
    coeffs.Clear();
    powers.Clear();
    for (int i = 0; i < newTermCount; ++i) {
        coeffs.Push_back(terms[i].coeff);
        powers.Push_back(terms[i].power);
    }
}

// 打印结果
void PrintRes(SeqList<double> &coeffs, SeqList<int> &powers) {
    // 结果为0
    if (coeffs.Length() == 0) {
        std::cout << "0" << std::endl;
        return;
    }
    
    bool firstTerm = true;
    for (int i = 0; i < coeffs.Length(); ++i) {
        double coeff = coeffs.Get(i);
        int power = powers.Get(i);

        // 防止 double 计算存在误差
        if (fabs(coeff) < 1e-10) continue;  // 忽略0系数项
        
        // 处理符号
        if (!firstTerm) {
            if (coeff > 0) std::cout << " + ";
            else std::cout << " - ";
        } else {
            if (coeff < 0) std::cout << "-";
        }
        
        // 处理系数
        double absCoeff = fabs(coeff);
        if (absCoeff != 1.0 || power == 0) {
            if (absCoeff == floor(absCoeff)) {
                std::cout << static_cast<int>(absCoeff);
            } else {
                std::cout << absCoeff;
            }
        }
        
        // 处理变量部分
        if (power > 0) {
            std::cout << "x";
            if (power > 1) {
                std::cout << "^" << power;
            }
        }
        
        firstTerm = false;
    }
    
    if (firstTerm) std::cout << "0";
    std::cout << std::endl;
}

// 多项式加法
void PolyAdd(const SeqList<double>& coeffs1, const SeqList<int>& powers1,
             const SeqList<double>& coeffs2, const SeqList<int>& powers2,
             SeqList<double>& resCoeffs, SeqList<int>& resPowers) {
    // 合并两个多项式
    for (int i = 0; i < coeffs1.Length(); ++i) {
        resCoeffs.Push_back(coeffs1.Get(i));
        resPowers.Push_back(powers1.Get(i));
    }
    for (int i = 0; i < coeffs2.Length(); ++i) {
        resCoeffs.Push_back(coeffs2.Get(i));
        resPowers.Push_back(powers2.Get(i));
    }
    
    // 合并同类项
    Combine(resCoeffs, resPowers);
}

// 多项式减法
void PolySub(const SeqList<double>& coeffs1, const SeqList<int>& powers1,
             const SeqList<double>& coeffs2, const SeqList<int>& powers2,
             SeqList<double>& resCoeffs, SeqList<int>& resPowers) {
    // 合并两个多项式，第二个多项式取反
    for (int i = 0; i < coeffs1.Length(); ++i) {
        resCoeffs.Push_back(coeffs1.Get(i));
        resPowers.Push_back(powers1.Get(i));
    }
    for (int i = 0; i < coeffs2.Length(); ++i) {
        resCoeffs.Push_back(-coeffs2.Get(i));
        resPowers.Push_back(powers2.Get(i));
    }
    
    // 合并同类项
    Combine(resCoeffs, resPowers);
}

// 多项式乘法
void PolyMul(const SeqList<double>& coeffs1, const SeqList<int>& powers1,
             const SeqList<double>& coeffs2, const SeqList<int>& powers2,
             SeqList<double>& resCoeffs, SeqList<int>& resPowers) {
    // 执行乘法
    for (int i = 0; i < coeffs1.Length(); ++i) {
        for (int j = 0; j < coeffs2.Length(); ++j) {
            double coeff = coeffs1.Get(i) * coeffs2.Get(j);
            int power = powers1.Get(i) + powers2.Get(j);
            
            resCoeffs.Push_back(coeff);
            resPowers.Push_back(power);
        }
    }
    
    // 合并同类项
    Combine(resCoeffs, resPowers);
}

// 多项式求导
void PolyDerivative(const SeqList<double>& coeffs, const SeqList<int>& powers,
                    SeqList<double>& derivCoeffs, SeqList<int>& derivPowers) {
    for (int i = 0; i < coeffs.Length(); ++i) {
        double coeff = coeffs.Get(i);
        int power = powers.Get(i);
        
        if (power > 0) {
            derivCoeffs.Push_back(coeff * power);
            derivPowers.Push_back(power - 1);
        }
    }
}

// 多项式求值
double PolyEvaluate(const SeqList<double>& coeffs, const SeqList<int>& powers, double x) {
    double result = 0.0;
    for (int i = 0; i < coeffs.Length(); ++i) {
        double coeff = coeffs.Get(i);
        int power = powers.Get(i);
        result += coeff * pow(x, power);
    }
    return result;
}

int main() {
    // 硬编码使用与源码文件同目录下的 test_cases.txt
    std::filesystem::path sourcePath = __FILE__;
    std::filesystem::path filePath = sourcePath.parent_path() / "test_cases.txt";

    if (!std::filesystem::exists(filePath)) {
        std::cerr << "当前工作目录: " << std::filesystem::current_path() << std::endl;
        return 1;
    }

    std::ifstream testFile(filePath);
    if (!testFile.is_open()) {
        std::cerr << "无法打开测试文件: '" << filePath << "'。请检查文件权限。" << std::endl;
        return 1;
    }

    std::string line;
    int testCase = 1;
    
    while (std::getline(testFile, line)) {
        // 跳过空行和注释行(以#开头)
        if (line.empty() || line[0] == '#') continue;
        
        std::cout << "\n=== 测试用例 " << testCase++ << " ===" << std::endl;
        
        // 解析测试数据格式: "多项式1|多项式2|x值"
        size_t delimiterPos1 = line.find('|');
        if (delimiterPos1 == std::string::npos) {
            std::cerr << "测试数据格式错误，应为'多项式1|多项式2|x值'" << std::endl;
            continue;
        }
        
        size_t delimiterPos2 = line.find('|', delimiterPos1 + 1);
        if (delimiterPos2 == std::string::npos) {
            std::cerr << "测试数据格式错误，应为'多项式1|多项式2|x值'" << std::endl;
            continue;
        }
        
        std::string poly1 = line.substr(0, delimiterPos1);
        std::string poly2 = line.substr(delimiterPos1 + 1, delimiterPos2 - delimiterPos1 - 1);
        std::string xValueStr = line.substr(delimiterPos2 + 1);
        
        // 去除前后空格
        poly1.erase(0, poly1.find_first_not_of(" \t"));
        poly1.erase(poly1.find_last_not_of(" \t") + 1);
        poly2.erase(0, poly2.find_first_not_of(" \t"));
        poly2.erase(poly2.find_last_not_of(" \t") + 1);
        xValueStr.erase(0, xValueStr.find_first_not_of(" \t"));
        xValueStr.erase(xValueStr.find_last_not_of(" \t") + 1);
        
        double xValue = ToDouble(xValueStr);
        
        SeqList<double> coeffs1, coeffs2;
        SeqList<int> powers1, powers2;
        
        std::cout << "多项式1: " << poly1 << std::endl;
        ParsePolynomial(poly1, coeffs1, powers1);
        Combine(coeffs1, powers1);
        
        std::cout << "多项式2: " << poly2 << std::endl;
        ParsePolynomial(poly2, coeffs2, powers2);
        Combine(coeffs2, powers2);
        
        // 计算结果
        SeqList<double> addCoeffs, subCoeffs, mulCoeffs, derivCoeffs1, derivCoeffs2;
        SeqList<int> addPowers, subPowers, mulPowers, derivPowers1, derivPowers2;
        
        PolyAdd(coeffs1, powers1, coeffs2, powers2, addCoeffs, addPowers);
        PolySub(coeffs1, powers1, coeffs2, powers2, subCoeffs, subPowers);
        PolyMul(coeffs1, powers1, coeffs2, powers2, mulCoeffs, mulPowers);
        PolyDerivative(coeffs1, powers1, derivCoeffs1, derivPowers1);
        PolyDerivative(coeffs2, powers2, derivCoeffs2, derivPowers2);
        
        // 输出结果
        std::cout << "加法结果: ";
        PrintRes(addCoeffs, addPowers);
        
        std::cout << "减法结果: ";
        PrintRes(subCoeffs, subPowers);
        
        std::cout << "乘法结果: ";
        PrintRes(mulCoeffs, mulPowers);
        
        std::cout << "多项式1的导数: ";
        PrintRes(derivCoeffs1, derivPowers1);
        
        std::cout << "多项式2的导数: ";
        PrintRes(derivCoeffs2, derivPowers2);
        
        // 计算并输出多项式在x处的值
        std::cout << "多项式1在x=" << xValue << "处的值: " 
                  << PolyEvaluate(coeffs1, powers1, xValue) << std::endl;
        std::cout << "多项式2在x=" << xValue << "处的值: " 
                  << PolyEvaluate(coeffs2, powers2, xValue) << std::endl;
    }
    
    testFile.close();
    return 0;
}