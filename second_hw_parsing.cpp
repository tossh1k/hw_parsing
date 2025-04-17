#include <iostream>
#include <string>
#include <sstream>

void start_handler()
{
    std::cout << "start" << std::endl;
}

void end_handler()
{
    std::cout << "end" << std::endl;
}

void digit_token_handler(uint64_t token)
{
    std::cout << "digit token: [" << token << "]" << std::endl;
}

void string_token_handler(const std::string& token)
{
    std::cout << "string token: [" << token << "]" << std::endl;
}

class TokenParser
{
private:
    void(*start_callback)();
    void(*end_callback)();
    void(*digit_callback)(uint64_t);
    void(*str_callback)(const std::string&);
public:
    TokenParser() : start_callback(nullptr), end_callback(nullptr), digit_callback(nullptr), str_callback(nullptr) {}
    ~TokenParser() {
        if (end_callback) end_callback();
    }

    void SetStartCallback(void (*fun)()) {
        start_callback = fun;
    }

    void SetEndCallback(void (*fun)()) {
        end_callback = fun;
    }

    void SetDigitTokenCallback(void (*fun)(uint64_t)) {
        digit_callback = fun;
    }

    void SetStringTokenCallback(void (*fun)(const std::string&)) {
        str_callback = fun;
    }

    bool isNum(const std::string& s) {
        for (char ch : s) if (ch > '9' || ch < '0') return false;
        return true;
    }

    void Parse(const std::string& input) {

        if (start_callback) {
            start_callback();
            start_callback = nullptr;
        }

        std::istringstream stream(input);
        std::string token;
        
        while (stream >> token) {
            if (isNum(token)) {
                try {
                    uint64_t value = std::stoull(token);
                    if (digit_callback) digit_callback(value);
                }
                catch (...) {
                    if (str_callback) str_callback(token);
                }
            } else if (str_callback) str_callback(token);
        }
    }
};

int main()
{   
    std::string flag; std::cin >> flag;
    TokenParser parser;
    
    if (flag[0] == '1') parser.SetStartCallback(start_handler);
    if (flag[1] == '1') parser.SetEndCallback(end_handler);
    if (flag[2] == '1') parser.SetDigitTokenCallback(digit_token_handler);
    if (flag[3] == '1') parser.SetStringTokenCallback(string_token_handler);
  


    std::string line;
    while (std::getline(std::cin, line, '\n')) {
        parser.Parse(line);
    }

    return 0;
}