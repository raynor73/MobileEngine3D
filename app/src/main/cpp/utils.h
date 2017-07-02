#ifndef UTILS_H
#define UTILS_H


#include <string>
#include <sstream>

using namespace std;

namespace Utils
{

float toRadians(float);

template <typename T> inline string toString(T value)
{
	stringstream ss;
	ss << value;
	return ss.str();
}

}

#endif // UTILS_H
