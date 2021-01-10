//
// Created by loenberg on 12/19/20.
//

#ifndef THESISSOURCE_DISTANCEORACLE_HPP
#define THESISSOURCE_DISTANCEORACLE_HPP

class DistanceOracle {
public:
    virtual double GetDistance(int u, int v);
    virtual int GetSize();
};

#endif //THESISSOURCE_DISTANCEORACLE_HPP
