#pragma once
#include <vector>
#include <cmath>
#include <algorithm>
#include <string>

namespace quant{

class pathpayoff {
public:
    virtual ~pathpayoff() = default;
    virtual double calculate(const std::vector<double>& path, double T) const = 0;
    virtual std::string name() const =0;
};

class asianpayoff: public pathpayoff {  
public:
    enum class Type { Call, Put};

    asianpayoff(double strike, Type type, bool arithmetic = true)
        : strike_(strike), type_(type), arithmetic_(arithmetic) {}
    
    double calculate(const std::vector<double>& path, double T) const override {
        double avg;
        if (arithmetic_) {
            double sum=0.0;
            for (double s : path) sum+=s;
            avg = sum /path.size();
        } else {
            double log_sum= 0.0;
            for (double s: path) log_sum += std::log(s);
            avg= std::exp(log_sum/ path.size());
        }

        double payoff= (type_ == Type::Call) ? std::max(avg- strike_, 0.0)
            : std::max(strike_ -avg, 0.0);
        
        return payoff;  
    }

    std::string name() const override {
        return (arithmetic_? "Arithmetic": "Geometric") + 
            std::string(type_== Type::Call ? "Asian Call": "Asian Put");
    }

private:
    double strike_;
    Type type_;
    bool arithmetic_;
};

};