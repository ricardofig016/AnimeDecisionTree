#ifndef DECISIONTREE_H
#define DECISIONTREE_H

#include <iostream>
#include <vector>

#include "node.h"

using namespace std;

class DecisionTree {
public:
    int max_depth; // max depth the tree will expand to
    vector<string> features; // features of the datapoints
    string target_feature; // class attribute
    Node root; // root node of the tree

    Node ID3(Node &node, string father_value);
    double entropy(vector<vector<string>> dataset);
    double entalpy(vector<vector<string>> dataset, int feature_index);
    double entalpyNum1(vector<vector<string>> dataset, int feature_index, double threshold);
    double entalpyNum2(vector<vector<string>> dataset, int feature_index, double th1, double th2);
    double bestThreshold1(vector<vector<string>> dataset, int feature_index);
    double bestThreshold2(vector<vector<string>> dataset, int feature_index, double threshold1);
    int bestInformationGain(Node node);
    bool isQuant(vector<vector<string>> dataset, int feature_index);

    DecisionTree();
    DecisionTree(vector<vector<string>> dataset, int max_depth, vector<string> features);
};

#endif