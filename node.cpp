#include <vector>
#include <iostream>
#include <map>
#include <algorithm>

#include "node.h"

/**
 * @brief Default constructor
 * 
 */
Node::Node() {
    this->feature = "";
    this->feature_index = -1;
    this->threshold1 = -1;
    this->threshold2 = -1;
    this->children = {};
    this->features = {};
    this->explored_features = {};
    this->dataset = {{}};
    this->isQuant = false;
    this->value = "";
    this->depth = 0;
}

/**
 * @brief Constructor for a new Node object
 * 
 * @param dataset - set of all datapoints in the node
 */
Node::Node(vector<string> features, vector<string> explored_features, vector<vector<string>> dataset, int depth) {
    this->feature = "";
    this->feature_index = -1;
    this->threshold1 = -1;
    this->threshold2 = -1;
    this->children = {};
    this->features = features;
    this->explored_features = explored_features;
    this->dataset = dataset;
    this->isQuant = false;
    this->value = "";
    this->depth = depth;
}

void Node::calculateValue() {
    string most_frequent_value = "";
    int most_frequent_value_counter = 0;

    map<string, int> valueCounter;

    for(int i = 0; i < static_cast<int>(this->dataset.size()); i++) {
        string currentValue = this->dataset[i].back(); // value of target feature

        if(valueCounter.find(currentValue) != valueCounter.end()) {
            valueCounter[currentValue] += 1;
        } else {
            valueCounter.insert(pair<string, int>(currentValue, 1));
        }
    }

    for(auto counter : valueCounter) { // <!!!!!!!!!> o que é que acontece em caso de empate?
        if(most_frequent_value_counter < counter.second) {
            most_frequent_value_counter = counter.second;
            most_frequent_value = counter.first;
        }      
    }

    this->value = most_frequent_value;
}

void Node::expand() {
    if(isQuant) { // quantitative attribute
        vector<vector<string>> left_branch;
        vector<vector<string>> middle_branch;
        vector<vector<string>> right_branch;
        for(auto datapoint : dataset) {
            if(stod(datapoint[feature_index]) <= threshold1) {
                left_branch.push_back(datapoint);
            } else if(stod(datapoint[feature_index]) <= threshold2) {
                middle_branch.push_back(datapoint);
            } else if(stod(datapoint[feature_index]) > threshold2) {
                right_branch.push_back(datapoint);
            } else cout << "algo tá muito mal" << endl;
        }

        Node left_child(features, explored_features, left_branch, depth + 1);
        children.push_back(left_child);

        Node middle_child(features, explored_features, middle_branch, depth + 1);
        children.push_back(middle_child);

        Node right_child(features, explored_features, right_branch, depth + 1);
        children.push_back(right_child);

    } else { // qualitative attribute
        vector<string> values;
        for(auto datapoint : dataset) {
            if(find(values.begin(), values.end(), datapoint[feature_index]) == values.end()) { // datapoint[feature_index] is not in values
                values.push_back(datapoint[feature_index]);
            }
        }
        for(string v : values) {
            vector<vector<string>> temp;
            for(auto datapoint : dataset) {
                if(datapoint[feature_index] == v) {
                    temp.push_back(datapoint);
                }
            }
            Node child(features, explored_features, temp, depth + 1);
            children.push_back(child);
        }
    }
}

// base case 1: all datapoints in the node have the same class value
bool Node::checkDatasetPositivity() {
    if(this->dataset.size() == 0) return false;

    string targetFeatureValue = this->dataset[0].back();

    for(int i = 1; i < static_cast<int>(this->dataset.size()); i++) {
        if(targetFeatureValue != this->dataset[i].back()) {
            return false;
        }
    }
    return true;
}

// base case 2: there are no datapoints in the node
bool Node::checkDatasetEmpty() {
    if(this->dataset.size() == 0) return true;
    return false;
}

string Node::findValue(vector<string> testpoint) {
    if(static_cast<int>(children.size()) == 0) return value;
    else {
        if(isQuant) { // quantitative feature
            if(stod(testpoint[feature_index]) <= threshold1) {
                return children[0].findValue(testpoint); // first child is <= threshold1
            } else if(stod(testpoint[feature_index]) <= threshold2) {
                return children[1].findValue(testpoint); // second child is <= threshold2
            } else {
                return children[2].findValue(testpoint);// third child is > threshold2
            }
        } else { // qualitative feature
            for(auto child : children) {
                if(testpoint[feature_index] == child.dataset[0][feature_index]) {
                    return child.findValue(testpoint);
                }
            }
        }
        return value; // current node has no children
    }
    return "algo correu mal";
}

void Node::printDatasetIds() {
    cout << endl << "------------------------------------------------------------------------------------------------------" << endl << endl;
    cout << "Dataset of <" << feature << "> (" << dataset.size() << "):" << endl;
    cout << "nr of children: " << children.size() << endl;
    for (auto datapoint : dataset) {
        cout << "    " << datapoint[0] << " - " << datapoint[datapoint.size()-1] << endl;
    }
}

void Node::printStructure(int tab_counter) {
    // print "class (counter)"
    if(static_cast<int>(children.size()) <= 0) {
        if((int)dataset.size() > 0) {
            cout << value << " (" << dataset.size() << ")" << endl;
        }
        return;
    }

    // print "<attribute>"
    cout << endl;
    for(int i = 0; i < tab_counter; i++) {
        cout << "    ";
    }
    cout << "<" << feature << ">" << endl;
    tab_counter++;

    // print "value:"
    for(int i = 0; i < static_cast<int>(children.size()); i++) {
        if(i != 0 || (int)children[0].dataset.size() > 0) {
            for(int i = 0; i < tab_counter; i++) {
                cout << "    ";
            }
        }

        if(isQuant) {
            if(threshold1 != -1) {
                if(i == 0) cout << "<=" << threshold1 << ": ";
                else if (i == 1) cout << threshold1 << "-" << threshold2 << ": ";
                else cout << ">" << threshold2 << ": ";
            } else {
                if(i == 0);
                else if (i == 1) cout << "<=" << threshold2 << ": ";
                else cout << ">" << threshold2 << ": ";
            }
        } else {
            cout << children[i].dataset[0][feature_index] << ": ";
        }

        children[i].printStructure(tab_counter + 1);
    }
}

int Node::printList(int counter, int target) {
    if(counter == target && (int)dataset.size() > 0) {
        printDatasetIds();
        return -1;
    }
    int child_nodes = 0;
    if((int)dataset.size() > 0) child_nodes++;
    for(int i = 0; i < (int)children.size(); i++) {
        if(counter > target) return -1;
        int temp = children[i].printList(counter+1, target);
        if(temp == -1) {
            return -1;
        }
        counter += temp;
        child_nodes +=temp;
    }
    return child_nodes;
}