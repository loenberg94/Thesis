//
// Created by loenberg on 2/2/21.
//

#ifndef THESISTESTSOURCE_TYPES_HPP
#define THESISTESTSOURCE_TYPES_HPP


struct vertex_distance{
    int v;
    double distance;
};

struct vertex_distance_distance_comparer{
    bool operator()(const vertex_distance& vd1, const vertex_distance& vd2){
        return vd1.distance < vd2.distance;
    }
};

#endif //THESISTESTSOURCE_TYPES_HPP
