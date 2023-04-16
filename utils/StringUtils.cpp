#include "StringUtils.h"

#include <string>
#include <vector>
#include <sstream>
#include <algorithm>

using namespace std;

StringUtils::StringUtils() {}

vector<string> StringUtils::split(string myString, char segment) {
    vector<string> splitedString;
    stringstream ss(myString);
    string item;
    while (getline(ss, item, segment))
    {
        splitedString.push_back(item);
    }
    return splitedString;
}

string StringUtils::removeWhiteSpaces(string str) {
    str.erase(remove(str.begin(), str.end(), ' '), str.end());
    str.erase(remove(str.begin(), str.end(), '\n'), str.end());
    str.erase(remove(str.begin(), str.end(), '\t'), str.end());
    return str;
}

int StringUtils::include(vector<string> vector, string str) {
    for (int i = 0; i < vector.size(); i++)
    {
        if (vector[i] == str)
        {
            return i;
        }
    }
    return -1;
}