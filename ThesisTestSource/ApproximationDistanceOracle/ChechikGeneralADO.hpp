//
// Created by loenberg on 12/21/20.
//

#ifndef THESISTESTSOURCE_CHECHIKGENERALADO_HPP
#define THESISTESTSOURCE_CHECHIKGENERALADO_HPP

#include "../BaseClasses/DistanceOracle.hpp"

class ChechikGeneralADO: public DistanceOracle{
public:
    double GetDistance(int u, int v) override {
        return 0;
    }

    int GetSize() override {
        return 0;
    }

};

#endif //THESISTESTSOURCE_CHECHIKGENERALADO_HPP
