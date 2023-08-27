#include <vector>
#include <string>
#include "filesystem"

using namespace std;

#ifndef __STRING_UTILS_H__
#define __STRING_UTILS_H__

class StringUtils
{
public:
    StringUtils();
    vector<string> split(string str, char segment);
    vector<string> split(string str, string delimiter);
    string removeWhiteSpaces(string str);
    int include(vector<string> vector, string str);
    string join(vector<string> vector, string token);
    string pathToString(filesystem::path path);
    string replaceAllstring(string str, const string& from, const string& to);
    long long dateToInt(string date);
};

#endif