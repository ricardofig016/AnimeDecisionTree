---------------------------------------------------------------------------------------------------

Description

the program first creates a decision tree based on the ID3 algorithm from a .csv file
the structure of the tree will be displayed on the terminal
the program will then take another .csv file and make a decision on the contents based on the tree
the user can:
    - specify the name of the files used to make and test the tree
    - request statistics (frequency distibution, total, mean, median, min, max) from a selected 
    column on a .csv file
    - specify the maximum depth the tree will grow to
    - request the information of a selected node in the tree

---------------------------------------------------------------------------------------------------

Compile Command

g++ main.cpp decisionTree.cpp node.cpp -o program.exe

---------------------------------------------------------------------------------------------------

Run Command

./program.exe

---------------------------------------------------------------------------------------------------

Command Options

specify dataset file name           "-ds file"      (default datastet.csv)      *csv file
specify testset file name           "-ts file"      (default testset.csv)       *csv file
request stats of a column of ds     "-s column"     (default none)              *can be set to --all
specify the max depth of the tree   "-d depth"      (default infinite)          *root is depth=0
request the dataset of a node       "-n node"       (default none)              *uses dfs 

example:
./program.exe -ds ricardo_data.csv -ts ricardo_test.csv -s format -d 4 -n 33

*order of options does not matter
*no option is required

---------------------------------------------------------------------------------------------------
