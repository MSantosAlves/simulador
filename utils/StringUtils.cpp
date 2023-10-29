#include "StringUtils.h"

#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <iostream>
#include <cstdint>
#include <ctime>
#include <iomanip>

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


vector<string> StringUtils::split(string str, string delimiter) {
    vector<string> tokens;
    size_t start = 0, end = 0;

    while ((end = str.find(delimiter, start)) != string::npos) {
        tokens.push_back(str.substr(start, end - start));
        start = end + delimiter.length();
    }
    tokens.push_back(str.substr(start));

    return tokens;
}

string StringUtils::removeWhiteSpaces(string str) {
    str.erase(remove(str.begin(), str.end(), ' '), str.end());
    str.erase(remove(str.begin(), str.end(), '\n'), str.end());
    str.erase(remove(str.begin(), str.end(), '\t'), str.end());
    return str;
}

vector<string> StringUtils::removeWhiteSpacesFromSplitedString(vector<string> splitedString)
{
    for (int i = 0; i < splitedString.size(); i++) {
        splitedString[i] = removeWhiteSpaces(splitedString[i]);
    }
    return splitedString;
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

string StringUtils::join(vector<string> vector, string token) {
    string result = "";
    for (int i = 0; i < vector.size(); i++) {
        result += vector[i];
        if (i < vector.size() - 1) {
            result += token;
        }
    }
    return result;
}

string StringUtils::pathToString(filesystem::path path) {
    string aux = split(path.generic_string(), "/out")[0];
    vector<string> splitedPathString = split(aux, "/");
    string sysFileChar = "/";// (_WIN64 || _WIN32) ? "\\" : "/";
    string pathString = join(splitedPathString, sysFileChar);

    return pathString;
}

string StringUtils::replaceAllstring(string str, const string& from, const string& to) {
    size_t start_pos = 0;
    while ((start_pos = str.find(from, start_pos)) != string::npos) {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length();
    }
    return str;
}

long long StringUtils::dateToInt(string date) {
    date.erase(remove(date.begin(), date.end(), '.'), date.end());
    date.erase(remove(date.begin(), date.end(), ':'), date.end());
    long long result = stoll(date);
   
	return result;
}

string StringUtils::intToDate(long long date) {

    string inputStr = to_string(date);

    if (inputStr.length() < 12) {
        inputStr = "0" + inputStr;
    }

    string hours = inputStr.substr(0, 2);
    string minutes = inputStr.substr(2, 2);
    string seconds = inputStr.substr(4, 2);
    string microseconds = inputStr.substr(6);

    string formattedDate = hours + ":" + minutes + ":" + seconds + "." + microseconds;
    
    return formattedDate;
}