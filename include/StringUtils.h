#include <vector>
#include <string>
using namespace std;

#ifndef __STRING_UTILS_H__
#define __STRING_UTILS_H__

class StringUtils
{
public:
    StringUtils();
    vector<string> split(string str, char segment);
    string removeWhiteSpaces(string str);
    int include(vector<string> vector, string str);
};

#endif