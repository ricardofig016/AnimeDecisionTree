#ifndef NODE_H
#define NODE_H

#include <iostream>
#include <vector>
#include <string>

using namespace std;

class Node {
public:
    string feature; // feature that will divide the dataset of the node
    int feature_index; // index in which the feature is apresented in the dataset
    double threshold1; // best value for the division of a numerical feature
    double threshold2;
    vector<Node> children; // vector of child nodes
    vector<string> features; // features of the datapoints
    vector<string> explored_features; // features that have already been split
    vector<vector<string>> dataset; // set of all datapoints in the node
    bool isQuant; // true if the attribute is quantitative, false if qualitative
    string value; // class value of the node
    int depth; // depth of the node in the decision tree

    Node();
    Node(vector<string> features, vector<string> explored_features, vector<vector<string>> dataset, int depth);
    
    void calculateValue(); // calculate the value of the node based on voting majority
    void expand(); // creates children for the node
    bool checkDatasetPositivity(); // base case 1 - todos os datapoints têm o mesmo valor de target feature
    bool checkDatasetEmpty(); // base case 2 - não há datapoints, dataset vazio
    string findValue(vector<string> testpoint);
    void printDatasetIds(); // print all the datapoints in the dataset to the console
    void printStructure(int tab_counter);
    int printList(int counter, int target);
};

#endif