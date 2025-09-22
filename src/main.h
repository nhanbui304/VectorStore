#ifndef __MAIN_H__
#define __MAIN_H__

#include <iostream>
#include <sstream>
#include <string>
#include <stdexcept>
#include <cmath>
#include "utils.h"

using namespace std;

class metric_error : public std::logic_error
{
public:
    metric_error() : std::logic_error("Invalid metric!") {}
    explicit metric_error(const std::string &what_arg) : std::logic_error(what_arg) {}
};

class invalid_k_value : public std::logic_error
{
public:
    invalid_k_value() : std::logic_error("Invalid k value!") {}
    explicit invalid_k_value(const std::string &what_arg) : std::logic_error(what_arg) {}
};

#endif // __MAIN_H__
