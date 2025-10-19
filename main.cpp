/*
 * INSTRUCTION:
 *     This is a C++ starting code for Homework Group1.
 *     When you finish the development, download this file and submit it to Canvas
 *     according to the submission instructions.
 *
 *     Please DO NOT change the name of the main procedure "main()"
 */

// Finish the head comment with Abstract, Name, and Date.
/*
 * Abstract: This program uses infix to postfix notation using a shunting yard algorithm
 *           and uses a stack to perform operations on math expressions. These expressions
 *           are some that can be entered on a scientific calculator (e.g., (6+4)^2*1.5+4)
 * AI tools utilized: ChatGPT was used to generate the functions for tokenization and to
 *                    complete the shunting yard algorithm.
 *
 * Name: Mathew Johann, Kayla Perez
 * Date: 10/18/2025
 */

#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <cctype>
#include <cmath>
using namespace std;

// Define token types
enum class TokenType {
    Number,
    Operator,
    LeftParen,
    RightParen
};

// Token structure
struct Token {
    TokenType type;
    string value;
};

int precedence(string op) {
    // check for precedence
    if (op == "(") {
        return 3;
    } else if (op == "^") {
        return 2;
    } else if (op == "*" || op == "/") {
        return 1;
    } else {
        return 0;
    }
}

// Tokenizer function
vector<Token> tokenizeExpression(const string& expr) {
    vector<Token> tokens;
    size_t i = 0;

    while (i < expr.length()) {
        char ch = expr[i];

        // Check for a negative number (unary minus)
        bool isNegativeNumber = false;
        if (ch == '-') {
            // At start OR after '(' or another operator
            if (tokens.empty() || 
                tokens.back().type == TokenType::LeftParen || 
                tokens.back().type == TokenType::Operator) {
                
                // Peek ahead to see if it's a number
                if (i + 1 < expr.length() && (std::isdigit(expr[i + 1]) || expr[i + 1] == '.')) {
                    isNegativeNumber = true;
                }
            }
        }

        // Handle number (possibly negative or decimal)
        if (isdigit(ch) || ch == '.' || isNegativeNumber) {
            string number;
            bool hasDecimal = false;

            if (isNegativeNumber) {
                number += '-';
                ++i;
                ch = expr[i];
            }

            while (i < expr.length() && (isdigit(expr[i]) || expr[i] == '.')) {
                if (expr[i] == '.') {
                    if (hasDecimal) break;  // Only one decimal allowed
                    hasDecimal = true;
                }
                number += expr[i++];
            }

            tokens.push_back({TokenType::Number, number});
        }

        // Handle operators
        else if (ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '^') {
            tokens.push_back({TokenType::Operator, std::string(1, ch)});
            ++i;
        }

        // Parentheses
        else if (ch == '(') {
            tokens.push_back({TokenType::LeftParen, "("});
            ++i;
        } else if (ch == ')') {
            tokens.push_back({TokenType::RightParen, ")"});
            ++i;
        }

        // Invalid character
        else {
            cerr << "Invalid character: '" << ch << "'\n";
            ++i;  // Skip invalid
        }
    }

    return tokens;
}

int main() {
    string expression;
    vector<string> tokens; // represent tokenization
    vector<string> middleRoute; // use a middle route for the shunting yard algorithm
    vector<string> finalResult; // output the final result in order of tokenization
    vector<string> stack; // store numbers in a stack; the last number is the result of the expression
    cout << "Enter a math expression: ";
    cin >> expression; // read the expression
    vector<Token> tokenSequence = tokenizeExpression(expression);

    for (const auto& token : tokenSequence) {
        tokens.push_back(token.value);
    }
    cout << "Infix:";
    for (int i = 0; i < tokens.size(); i++) {
        cout << " " << tokens[i];
    }
    cout << endl;

    // I created this loop myself. It reads the entire tokenization.
    while (!tokens.empty()) {
        if (tokens[0] == "+" || tokens[0] == "-" || tokens[0] == "*" || tokens[0] == "/" || tokens[0] == "^" || tokens[0] == "(") {
            while (!middleRoute.empty() && middleRoute.back() != "(" && precedence(middleRoute.back()) >= precedence(tokens[0])) {
                finalResult.push_back(middleRoute.back());
                middleRoute.pop_back();
            }
            middleRoute.push_back(tokens[0]);
            tokens.erase(tokens.begin());
        } else if (tokens[0] == ")") {
            while (middleRoute.back() != "(") {
                finalResult.push_back(middleRoute.back());
                middleRoute.pop_back();
            }
            middleRoute.pop_back();
            tokens.erase(tokens.begin());
        } else {
            finalResult.push_back(tokens[0]);
            tokens.erase(tokens.begin());
        }
    }
    while (!middleRoute.empty()) {
        finalResult.push_back(middleRoute.back());
        middleRoute.pop_back();
    }
    cout << "Postfix:";
    for (int i = 0; i < finalResult.size(); i++) {
        cout << " " << finalResult[i];
    }
    cout << endl;
    while (!finalResult.empty()) {
        stack.push_back(finalResult[0]);
        finalResult.erase(finalResult.begin());
        if (stack.back() == "+") {
            double temp = stod(stack[stack.size() - 3]) + stod(stack[stack.size() - 2]);
            for (int i = 0; i < 3; i++) {
                stack.pop_back();
            }
            stringstream ss;
            ss << temp;
            stack.push_back(ss.str());
        } else if (stack.back() == "-") {
            double temp = stod(stack[stack.size() - 3]) - stod(stack[stack.size() - 2]);
            for (int i = 0; i < 3; i++) {
                stack.pop_back();
            }
            stringstream ss;
            ss << temp;
            stack.push_back(ss.str());
        } else if (stack.back() == "*") {
            double temp = stod(stack[stack.size() - 3]) * stod(stack[stack.size() - 2]);
            for (int i = 0; i < 3; i++) {
                stack.pop_back();
            }
            stringstream ss;
            ss << temp;
            stack.push_back(ss.str());
        } else if (stack.back() == "/") {
            double temp = stod(stack[stack.size() - 3]) / stod(stack[stack.size() - 2]);
            for (int i = 0; i < 3; i++) {
                stack.pop_back();
            }
            stringstream ss;
            ss << temp;
            stack.push_back(ss.str());
        } else if (stack.back() == "^") {
            double temp = pow(stod(stack[stack.size() - 3]), stod(stack[stack.size() - 2]));
            for (int i = 0; i < 3; i++) {
                stack.pop_back();
            }
            stringstream ss;
            ss << temp;
            stack.push_back(ss.str());
        }
    }
    cout << "Result from expression: " << stack[0] << endl;
    

    return 0;
}
