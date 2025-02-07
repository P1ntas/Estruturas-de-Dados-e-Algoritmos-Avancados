#include "kdtree.h"
#include <limits>
#include <utility>
#include <queue>
#include <functional>
#include <cmath>


KDNode::KDNode(Point p) : point(std::move(p)), left(nullptr), right(nullptr) {}

KDTree::KDTree(int dims) : dimensions(dims), root(nullptr) {}

KDTree::KDTree(int dims, std::vector<Point> points) : dimensions(dims), root(nullptr) {
    std::vector<Point> queue = points;
    int dimension = 0;

    root = buildTree(queue, dimension);
}

KDTree::~KDTree() {
    clear(root);
}

void KDTree::clear(KDNode* node) {
    if (node) {
        clear(node->left);
        clear(node->right);
        delete node;
    }
}

KDNode* KDTree::insertUtil(KDNode* node, Point point, int depth) const {
    if (node == nullptr) {
        return new KDNode(std::move(point));
    }

    int dim = depth % dimensions;

    if (point.coords[dim] < node->point.coords[dim]) {
        node->left = insertUtil(node->left, std::move(point), depth + 1);
    } else {
        node->right = insertUtil(node->right, std::move(point), depth + 1);
    }

    return node;
}

KDNode* KDTree::buildTree(std::vector<Point> points, int currentDimension) {
    if (points.empty()) return nullptr;

    int dim = currentDimension % dimensions;
    std::sort(points.begin(), points.end(), [dim](const Point& a, const Point& b) {
        return a.coords[dim] < b.coords[dim];
    });

    int medianIndex = points.size() / 2;
    KDNode* node = new KDNode(points[medianIndex]);
    node->left = buildTree(std::vector<Point>(points.begin(), points.begin() + medianIndex), currentDimension + 1);
    node->right = buildTree(std::vector<Point>(points.begin() + medianIndex + 1, points.end()), currentDimension + 1);

    return node;
}

KDNode* KDTree::findNearestNeighborUtil(KDNode* node, Point target, KDNode* best, double& bestDist, int depth) {
    if (node == nullptr)
        return best;

    double dist = distance(node->point, target);
    if (dist < bestDist) {
        bestDist = dist;
        best = node;
    }

    int dim = depth % dimensions;
    KDNode* nearerSubtree = (target.coords[dim] < node->point.coords[dim]) ? node->left : node->right;
    KDNode* furtherSubtree = (target.coords[dim] < node->point.coords[dim]) ? node->right : node->left;

    best = findNearestNeighborUtil(nearerSubtree, target, best, bestDist, depth + 1);

    if (std::abs(target.coords[dim] - node->point.coords[dim]) < bestDist)
        best = findNearestNeighborUtil(furtherSubtree, target, best, bestDist, depth + 1);

    return best;
}

double KDTree::distance(const Point& p1, const Point& p2) const {
    double dist = 0.0;
    for (int i = 0; i < dimensions; ++i)
        dist += (p1.coords[i] - p2.coords[i]) * (p1.coords[i] - p2.coords[i]);
    return std::sqrt(dist);
}

void KDTree::insert(Point point) {
    root = insertUtil(root, std::move(point), 0);
}

Point KDTree::findNearestNeighbor(Point target) {
    KDNode* nearest = nullptr;
    double bestDist = std::numeric_limits<double>::max();
    nearest = findNearestNeighborUtil(root, std::move(target), nearest, bestDist, 0);
    return nearest->point;
}

std::vector<Point> KDTree::findKNearestNeighbors(Point target, int k) const {
    std::priority_queue<std::pair<double, KDNode*>> pq;

    // Helper function to compare distances for priority queue
    auto compareDistances = [&](KDNode* a, KDNode* b) {
        return distance(a->point, target) < distance(b->point, target);
    };

    // Helper function to insert into priority queue and keep size <= k
    auto insertIntoQueue = [&](KDNode* node) {
        double dist = distance(node->point, target);
        pq.push({dist, node});
        if (pq.size() > k) pq.pop(); // Keep size <= k
    };

    // Recursive function to traverse the tree and find nearest neighbors
    std::function<void(KDNode*, int)> traverse = [&](KDNode* node, int depth) {
        if (node == nullptr) return;

        int dim = depth % dimensions;
        KDNode* nearerSubtree = (target.coords[dim] < node->point.coords[dim]) ? node->left : node->right;
        KDNode* furtherSubtree = (target.coords[dim] < node->point.coords[dim]) ? node->right : node->left;

        // Insert current node into priority queue
        insertIntoQueue(node);

        // Traverse nearer subtree
        traverse(nearerSubtree, depth + 1);

        // Check if we need to traverse further subtree
        if (std::abs(target.coords[dim] - node->point.coords[dim]) < pq.top().first || pq.size() < k)
            traverse(furtherSubtree, depth + 1);
    };

    // Start traversal from root
    traverse(root, 0);

    // Extract k nearest neighbors from priority queue
    std::vector<Point> nearestNeighbors;
    while (!pq.empty()) {
        nearestNeighbors.push_back(pq.top().second->point);
        pq.pop();
    }

    // Reverse the vector to get neighbors in ascending order of distance
    std::reverse(nearestNeighbors.begin(), nearestNeighbors.end());

    return nearestNeighbors;
}

int KDTree::getDimensions() const
{
    return dimensions;
}
