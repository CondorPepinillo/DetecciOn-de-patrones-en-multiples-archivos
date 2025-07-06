#include <iostream>
#include <string>
#include <vector>

using namespace std;


vector<string> split(const string& str, char delimiter) {
        vector<string> tokens;
        istringstream iss(str);
        string token;

        while (getline(iss, token, delimiter)) {
            tokens.push_back(token);
        }
    return tokens;
    };