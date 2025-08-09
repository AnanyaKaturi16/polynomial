#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <sstream>
#include "json/json.h" 

using namespace std;
using namespace Json;

long long convertToDecimal(const string& value, int base) {
    long long decimal = 0;
    long long power = 1;
    for (int i = value.size() - 1; i >= 0; --i) {
        char c = value[i];
        int digit;
        if (c >= '0' && c <= '9') {
            digit = c - '0';
        } else if (c >= 'a' && c <= 'z') {
            digit = c - 'a' + 10;
        } else if (c >= 'A' && c <= 'Z') {
            digit = c - 'A' + 10;
        } else {
            throw runtime_error("Invalid character in value");
        }
        if (digit >= base) {
            throw runtime_error("Invalid value for the given base");
        }
        decimal += digit * power;
        power *= base;
    }
    return decimal;
}

int main() {
    ifstream file("input.json");
    stringstream buffer;
    buffer << file.rdbuf();
    file.close();

    Value root;
    Reader reader;
    reader.parse(buffer.str(), root);

    int n = root["keys"]["n"].asInt();
    int k = root["keys"]["k"].asInt();

    vector<pair<long long, long long>> points;
    for (int i = 1; i <= n; ++i) {
        string key = to_string(i);
        int base = root[key]["base"].asInt();
        string value = root[key]["value"].asString();
        long long x = stoll(key);
        long long y = convertToDecimal(value, base);
        points.emplace_back(x, y);
    }
    long long c = 0;
    for (int i = 0; i < k; ++i) {
        long long numerator = 1;
        long long denominator = 1;
        for (int j = 0; j < k; ++j) {
            if (i != j) {
                numerator *= 0 - points[j].first;
                denominator *= points[i].first - points[j].first;
            }
        }
        c += points[i].second * numerator / denominator;
    }

    cout << c << endl;

    return 0;
}
