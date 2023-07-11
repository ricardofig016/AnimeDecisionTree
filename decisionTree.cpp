#include <iostream>
#include <vector>
#include <map>
#include <cmath>
#include <algorithm>
#include <regex>

#include "decisionTree.h"

using namespace std;

// default constructor for the DecisionTree class
DecisionTree::DecisionTree() {
    this->max_depth = 0;
    this->features = {};
    this->target_feature = "";
    this->root = Node();
}

/**
 * @brief Constructor for a new Decision Tree object
 * 
 * @param dataset - set of datapoints of the root node
 * @param max_depth - max depth the tree will expand to
 * @param features - features of the datapoints
 */
DecisionTree::DecisionTree(vector<vector<string>> dataset, int max_depth, vector<string> features) {
    this->max_depth = max_depth;
    this->features = features;
    this->target_feature = features.back(); // last element of features
    this->root = Node(features, {}, dataset, 1);
}

// recursive function to expand the tree

Node DecisionTree::ID3(Node &node, string father_value) {
    // determine base cases
    if(node.checkDatasetPositivity()) { // base case 1 - todos os datapoints têm o mesmo valor de target feature
        node.value = node.dataset[0].back();
        return node;
    } if(node.checkDatasetEmpty()) { // base case 2 - não há datapoints, dataset vazio
        node.value = father_value;
        return node;
    } if(features.size() == node.explored_features.size()+2) { // base case 3 - não há mais features, subarvore concluida
        node.calculateValue();
        return node;
    } if(node.depth > max_depth) { // base case 4 - altura máxima alcançada
        node.calculateValue();
        return node;
    }

    // choose feature to split the tree (using information gain formula)
    int best_feature_index = bestInformationGain(node);

    // calculate which points to split a numerical feature (using thresholds)
    if(isQuant(node.dataset, best_feature_index)) {
        // find threshold1
        double best_threshold1 = bestThreshold1(node.dataset, best_feature_index);

        // find threshold2
        double best_threshold2 = bestThreshold2(node.dataset, best_feature_index, best_threshold1);

        // sort both thresholds
        if(best_threshold1 > best_threshold2) {
            double temp = best_threshold1;
            best_threshold1 = best_threshold2;
            best_threshold2 = temp;
        }

        node.isQuant = true;
        node.threshold1 = best_threshold1;
        node.threshold2 = best_threshold2;
    }

    // rename and expand the node and populate it with parents' datapoints that obey the nodes feature

    node.feature_index = best_feature_index;
    node.feature = features[best_feature_index];
    node.calculateValue();
    //cout << endl << "expanded feature: " << node.feature << endl;
    //cout << "threshold: " << node.threshold << endl;
    node.explored_features.push_back(node.feature);
    node.expand();

    // do the same for created nodes
    for(int i = 0; i < static_cast<int>(node.children.size()); i++) {
        node.children[i] = ID3(node.children[i], node.value);
    }

    return node;
}

double DecisionTree::entropy(vector<vector<string>> dataset) {
    int total_data_points = dataset.size();

    map<string, int> target_feature_count;
    for (const auto& datapoint : dataset) {
        string target_feature_value = datapoint.back();
        if (target_feature_count.find(target_feature_value) != target_feature_count.end()) {
            target_feature_count[target_feature_value]++;
        } else {
            target_feature_count[target_feature_value] = 1;
        }
    }

    double entropy = 0.0;
    for (const auto& [target_feature_value, count] : target_feature_count) {
        double probability = static_cast<double>(count) / total_data_points;
        entropy -= probability * log2(probability);
    }
    return entropy;
}

double DecisionTree::entalpy(vector<vector<string>> dataset, int feature_index) {
    int total_data_points = dataset.size();

    map<string, int> feature_count;
    for(const auto& datapoint : dataset) {
        string feature_value = datapoint[feature_index];
        if (feature_count.find(feature_value) != feature_count.end()) {
            feature_count[feature_value]++;
        } else {
            feature_count[feature_value] = 1;
        }
    }

    double entalpy = 0.0;
    for (const auto& [feature_value, count] : feature_count) {
        double probability = static_cast<double>(count) / total_data_points;
        vector<vector<string>> temp;
        for(auto datapoint : dataset) {
            if(datapoint[feature_index] == feature_value) {
                temp.push_back(datapoint);
            }
        }
        entalpy += probability  * entropy(temp);
    }
    return entalpy;
}

double DecisionTree::entalpyNum1(vector<vector<string>> dataset, int feature_index, double threshold) {
    int total_data_points = dataset.size();

    map<string, int> feature_count;
    feature_count["left_branch"] = 0;
    feature_count["right_branch"] = 0;

    for(const auto& datapoint : dataset) {
        double feature_value = stod(datapoint[feature_index]);
        if (feature_value <= threshold) {
            feature_count["left_branch"]++;
        } else {
            feature_count["right_branch"]++;
        }
    }

    double entalpy = 0.0;
    for (const auto& [branch, count] : feature_count) {
        double probability = static_cast<double>(count) / total_data_points;
        vector<vector<string>> temp;
        if(branch == "left_branch") {
            for(auto datapoint : dataset) {
                if(stod(datapoint[feature_index]) <= threshold) {
                    temp.push_back(datapoint);
                }
            }
        } else { // right branch
            for(auto datapoint : dataset) {
                if(stod(datapoint[feature_index]) > threshold) {
                    temp.push_back(datapoint);
                }
            }
        }
        entalpy += probability  * entropy(temp);
    }
    return entalpy;
}

double DecisionTree::entalpyNum2(vector<vector<string>> dataset, int feature_index, double th1, double th2) {
    int total_data_points = dataset.size();

    map<string, int> feature_count;
    feature_count["left_branch"] = 0;
    feature_count["middle_branch"] = 0;
    feature_count["right_branch"] = 0;

    for(const auto& datapoint : dataset) {
        double feature_value = stod(datapoint[feature_index]);
        if(feature_value <= th1) {
            feature_count["left_branch"]++;
        } else if(feature_value <= th2) {
            feature_count["middle_branch"]++;
        } else {
            feature_count["right_branch"]++;
        }
    }

    double entalpy = 0.0;
    for (const auto& [branch, count] : feature_count) {
        double probability = static_cast<double>(count) / total_data_points;
        vector<vector<string>> temp;
        if(branch == "left_branch") {
            for(auto datapoint : dataset) {
                if(stod(datapoint[feature_index]) <= th1) {
                    temp.push_back(datapoint);
                }
            }
        } else if(branch == "middle_branch") {
            for(auto datapoint : dataset) {
                if(stod(datapoint[feature_index]) > th1 && stod(datapoint[feature_index]) <= th2) {
                    temp.push_back(datapoint);
                }
            }
        } else { // right branch
            for(auto datapoint : dataset) {
                if(stod(datapoint[feature_index]) > th2) {
                    temp.push_back(datapoint);
                }
            }
        }
        entalpy += probability  * entropy(temp);
    }
    return entalpy;
}

double DecisionTree::bestThreshold1(vector<vector<string>> dataset, int feature_index) {
    double best_threshold = -1;
    double best_threshold_IG = 0;
    double current_threshold_IG; // calculated IG of the current threshold
    for(int i = 0; i < static_cast<int>(dataset.size()); i++) {
        double threshold = stod(dataset[i][feature_index]);
        current_threshold_IG = entropy(dataset) - entalpyNum1(dataset, feature_index, threshold);
        if(current_threshold_IG > best_threshold_IG) {
            best_threshold_IG = current_threshold_IG;
            best_threshold = threshold;
        }
    }
    return best_threshold;
}

double DecisionTree::bestThreshold2(vector<vector<string>> dataset, int feature_index, double threshold1) {
    // divide the dataset on threshold1
    vector<vector<string>> left_dataset;
    vector<vector<string>> right_dataset;
    for(auto datapoint : dataset) {
        if(stod(datapoint[feature_index]) <= threshold1) {
            left_dataset.push_back(datapoint);
        } else if(stod(datapoint[feature_index]) > threshold1) {
            right_dataset.push_back(datapoint);
        }
    }

    // choose biggest dataset
    vector<vector<string>> chosen_dataset;
    if(left_dataset.size() > right_dataset.size()) {
        for(auto datapoint : left_dataset) {
            chosen_dataset.push_back(datapoint);
        }
    } else {
        for(auto datapoint : right_dataset) {
            chosen_dataset.push_back(datapoint);
        }
    }

    // find threshold2
    double best_threshold = -1;
    double best_threshold_IG = 0;
    double current_threshold_IG; // calculated IG of the current threshold

    // search chosen dataset
    for(int i = 0; i < static_cast<int>(chosen_dataset.size()); i++) {
        double threshold = stod(chosen_dataset[i][feature_index]);
        current_threshold_IG = entropy(chosen_dataset) - entalpyNum1(chosen_dataset, feature_index, threshold);
        if(current_threshold_IG > best_threshold_IG) {
            best_threshold_IG = current_threshold_IG;
            best_threshold = threshold;
        }
    }

    return best_threshold;
}

/**
 * @brief calculates what is the feature with the best information gain
 * 
 * @param node - node that needs expanding
 * @return int - index of the best feature
 */
int DecisionTree::bestInformationGain(Node node) {
    double bestIG = 0.0;
    int best_feature_index = 0;
    double parent_entalpy = entropy(node.dataset);

    for(int feature_index = 1; feature_index < static_cast<int>(features.size()-1); feature_index++) {
        // check if the feature has already been explored
        if(find(node.explored_features.begin(), node.explored_features.end(), features[feature_index]) != node.explored_features.end()) {
            continue;
        }

        double currentIG = 0.0;
        if(isQuant(node.dataset, feature_index)) { // the feature contains quantitative values
            double best_threshold1 = bestThreshold1(node.dataset, feature_index);
            double best_threshold2 = bestThreshold2(node.dataset, feature_index, best_threshold1);

            double current_thresholds_IG; // calculated IG of the current thresholds
            current_thresholds_IG = parent_entalpy - entalpyNum2(node.dataset, feature_index, best_threshold1, best_threshold2);
            if(current_thresholds_IG > currentIG) {
                currentIG = current_thresholds_IG;
            }
        } else { // the feature contains qualitative values
            currentIG = parent_entalpy - entalpy(node.dataset, feature_index);
        }

        // cout << endl << features[feature_index] << ": " << endl;
        // cout << "ig: " << currentIG << endl;

        if(currentIG > bestIG) {
            bestIG = currentIG;
            best_feature_index = feature_index;
        }
    }

    return best_feature_index;
}

bool DecisionTree::isQuant(vector<vector<string>> dataset, int feature_index) {
    static const regex float_regex("^[+-]?[0-9]*\\.?[0-9]+$");
    /*
    '^' - matches the beggining of the string
    '[+-]?' - '[+-]' matches either '+' or '-', and '?' makes it opcional
    '[0-9]*' - '[0-9]' matches any digit between 0 and 9, and '*' matches 0 or more occurences of '[0-9]'
    '\\.?' - '\\.' matches a dot character ('.') and '?' makes it opcional
    '[0-9]+' - '[0-9]' matches any digit between 0 and 9, and '+' matches 1 or more occurences of '[0-9]'
    '$' - matches the end of the string
    */
    return regex_match(dataset[0][feature_index], float_regex);
}
