#pragma once
#include <cmath>
#include <algorithm>

namespace quant {

/*
heston model
dS=(r-q)S dt+ sqrt(v) S dW1
dv=κ(θ-V) dt+ xi sqrt(v) dW2
dW1 *dW2 = rho dt
*/

struct hestonparams{
    double r; //risk-free rate
    double q; //dividend-yield
    double vθ; //initial variance
    double κ; //mean reversion=spread
    double θ; //long-run variance
    double xi;//vol of vol
    double rho;// correlation
};

class hestonmodel {
public:
    explicit hestonmodel(const hestonparams& params) : params_(params) {
        L22_= std::sqrt(1.0 -params_.rho* params_.rho);
    }

    void step(double& S, double& v, double dt, double Z1, double Z2) const {
        double W1 =Z1;
        double W2 = params_.rho* Z1 + L22_ *Z2;

        double sqrt_dt= std::sqrt(dt);
        double v_pos = std::max(v, 0.0);
        double sqrt_v= std::sqrt(v_pos);

        S=S * std::exp((params_.r - params_.q - 0.5* v_pos)* dt+sqrt_v * sqrt_dt * W1);

        v = v+ params_.κ *(params_.θ -v_pos) * dt
            + params_.xi *sqrt_v *sqrt_dt *W2
            + 0.25* params_.xi * params_.xi *dt* (W2*W2 - 1.0);
    }

    const hestonparams& params() {return params_; }
private:
    hestonparams params_;
    double L22_;
};

}