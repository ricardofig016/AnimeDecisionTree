#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cstring>
#include <regex>
#include <map>
#include <iomanip>
#include <typeinfo>
#include <algorithm>

#include "node.h"
#include "decisionTree.h"

using namespace std;

void mean(vector<string> column)
{
    int size = 0;
    double total = 0;
    for (string val_str : column)
    {
        size++;
        double val = stod(val_str);
        total += val;
    }

    double mean = total / size;
    cout << fixed << setprecision(2) << "total: " << total << endl;
    cout << fixed << setprecision(2) << "mean: " << mean << endl;
}

void median(vector<string> column_str)
{
    int size = (int)column_str.size();
    double median;

    vector<double> column;
    for (string val_str : column_str)
    {
        column.push_back(stod(val_str));
    }
    sort(column.begin(), column.end());

    if (size % 2 == 0)
    {
        median = (column[size / 2 - 1] + column[size / 2]) / 2;
    }
    else
    {
        median = column[size / 2];
    }

    cout << fixed << setprecision(2) << "median: " << median << endl;
}

void min_max(vector<string> column)
{
    double min_val = INT32_MAX, max_val = 0;
    for (string val_str : column)
    {
        double val = stod(val_str);
        max_val = max(max_val, val);
        min_val = min(min_val, val);
    }

    cout << fixed << setprecision(2) << "min: " << min_val << endl;
    cout << fixed << setprecision(2) << "max: " << max_val << endl;
}

void freq_distribution(vector<string> column)
{
    int max_size = 0;
    map<string, int> freq;
    for (string val : column)
    {
        freq[val]++;
        max_size = max(max_size, (int)val.size() + (int)to_string(freq[val]).size() + 2);
    }

    multimap<int, string> sorted_freq;
    for (auto pair : freq)
    {
        sorted_freq.insert(make_pair(pair.second, pair.first));
    }

    for (auto pair : sorted_freq)
    {
        string val_out = pair.second + '(' + to_string(pair.first) + ')';
        cout << left << setw(max_size) << setfill(' ') << val_out << ": ";

        int freq_percent = (pair.first * 50) / (int)column.size();
        for (int i = 0; i < freq_percent; i++)
        {
            cout << "*";
        }
        cout << endl;
    }
}

bool is_quant(vector<string> column)
{
    static const regex float_regex("^[+-]?[0-9]*\\.?[0-9]+$");
    /*
    '^' - matches the beggining of the string
    '[+-]?' - '[+-]' matches either '+' or '-', and '?' makes it opcional
    '[0-9]*' - '[0-9]' matches any digit between 0 and 9, and '*' matches 0 or more occurences of '[0-9]'
    '\\.?' - '\\.' matches a dot character ('.') and '?' makes it opcional
    '[0-9]+' - '[0-9]' matches any digit between 0 and 9, and '+' matches 1 or more occurences of '[0-9]'
    '$' - matches the end of the string
    */
    return regex_match(column[0], float_regex);
}

void calc_statistics(vector<string> features, vector<vector<string>> dataset, string column_name)
{
    int feature = 0;
    for (int i = 0; i < (int)features.size(); i++)
    {
        if (features[i] == column_name)
        {
            feature = i;
            break;
        }
        if (i == (int)features.size() - 1)
        {
            cout << "error: column " << column_name << " not found" << endl;
            return;
        }
    }

    vector<string> column;
    for (vector<string> val : dataset)
    {
        column.push_back(val[feature]);
    }

    cout << "Statistics of " << features[feature] << ':' << endl;
    if (is_quant(column))
    {
        mean(column);
        median(column);
        min_max(column);
    }
    else
    {
        freq_distribution(column);
    }
    cout << endl;
}

int main(int argc, char **argv)
{
    // Declare variables for storing the data
    vector<vector<string>> dataset;
    vector<string> features;

    // Open the CSV data_file for reading
    string data_file_name = "dataset.csv";
    for (int i = 1; i < argc; i++)
    {
        if (strcmp(argv[i], "-ds") == 0)
        {
            data_file_name = argv[i + 1];
            break;
        }
    }
    ifstream data_file(data_file_name);

    string line;
    getline(data_file, line);
    stringstream ss(line);

    // Loop through each column of the line
    string value;
    while (getline(ss, value, ','))
    {
        features.push_back(value);
    }

    // Loop through each line of the data_file
    while (getline(data_file, line))
    {
        // Declare a stringstream to parse the line
        stringstream ss(line);
        vector<string> row;

        // Loop through each column of the line
        string value;
        while (getline(ss, value, ','))
        {
            // Add the value to the row vector
            row.push_back(value);
        }

        // Add the row vector to the dataset vector
        dataset.push_back(row);
    }

    data_file.close();

    // Declare variables for storing the tests
    vector<vector<string>> testset;

    // Open the CSV test_file for reading
    string test_file_name = "testset.csv";
    for (int i = 1; i < argc; i++)
    {
        if (strcmp(argv[i], "-ts") == 0)
        {
            test_file_name = argv[i + 1];
            break;
        }
    }
    ifstream test_file(test_file_name);

    string line2;
    getline(test_file, line2);
    stringstream ss2(line2);

    // Loop through each column of the line
    string value2;
    while (getline(ss2, value2, ',')) // Ignore first line of the file (line of feature names)

        // Loop through each line2 of the test_file
        while (getline(test_file, line2))
        {
            // Declare a stringstream to parse the line
            stringstream ss2(line2);
            vector<string> row;

            // Loop through each column of the line
            string value2;
            while (getline(ss2, value2, ','))
            {
                // Add the value to the row vector
                row.push_back(value2);
            }

            // Add the row vector to the testset vector
            testset.push_back(row);
        }

    test_file.close();

    // column statistics
    for (int i = 1; i < argc; i++)
    {
        if (strcmp(argv[i], "-s") == 0)
        {
            if (strcmp(argv[i + 1], "--all") == 0)
            {
                for (int i = 1; i < (int)features.size(); i++)
                {
                    string feature_index = features[i];
                    calc_statistics(features, dataset, feature_index);
                }
                break;
            }

            string column = argv[i + 1];
            calc_statistics(features, dataset, column);
        }
    }

    // input max depth
    int max_depth = __INT_MAX__;
    for (int i = 1; i < argc; i++)
    {
        if (strcmp(argv[i], "-d") == 0)
        {
            max_depth = stoi(argv[i + 1]);
            break;
        }
    }

    // Create new decision tree
    DecisionTree dt(dataset, max_depth, features);

    // Create nodes of the tree following the ID3 algorithm
    dt.root = dt.ID3(dt.root, "-");

    // Print the structure of the tree
    dt.root.printStructure(0);

    // Print the class values for the test cases of the testset
    cout << endl
         << "------------------------------------------------------------------------------------------------------" << endl
         << endl;
    cout << "Testset (" << testset.size() << "):" << endl;
    for (auto testpoint : testset)
    {
        cout << "    " << testpoint[0] << " - " << dt.root.findValue(testpoint) << endl;
    }

    // Print the datapoint names of a node
    for (int i = 1; i < argc; i++)
    {
        if (strcmp(argv[i], "-n") == 0)
        {
            if (dt.root.printList(1, stoi(argv[i + 1])) != -1)
            {
                cout << endl
                     << "------------------------------------------------------------------------------------------------------" << endl;
                cout << endl
                     << "not enough nodes" << endl;
            }
        }
    }

    return 0;
}