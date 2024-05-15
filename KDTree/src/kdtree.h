//
// Created by janto on 15/05/2024.
//

#ifndef KDTREE_KDTREE_H
#define KDTREE_KDTREE_H

#include <iostream>
#include <vector>
#include <cmath>

struct Point {
    std::vector<double> coords;

    explicit Point(std::vector<double> coordinates);
};

class KDNode {
public:
    Point point;
    KDNode* left;
    KDNode* right;

    explicit KDNode(Point p);
};

class KDTree {
private:
    int dimensions;
    KDNode* root;

    KDNode* insertUtil(KDNode* node, Point point, int depth) const;
    KDNode* findNearestNeighborUtil(KDNode* node, Point target, KDNode* best, double& bestDist, int depth);
    [[nodiscard]] double distance(Point p1, Point p2) const;

public:
    explicit KDTree(int dims);

    void insert(Point point);
    Point findNearestNeighbor(Point target);
};

#endif //KDTREE_KDTREE_H
