#pragma once
#include <vector>
#include <string>
#include <cmath>
#include <algorithm>
namespace quant {

class pathpayoff {
public:
    virtual ~pathpayoff() = default;
    virtual double calculate(const std::vector<double>& path, double T) const =0;
    virtual std::string name() const =0;
};


};