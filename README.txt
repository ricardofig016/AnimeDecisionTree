---------------------------------------------------------------------------------------------------

compile command

g++ main.cpp decisionTree.cpp node.cpp -o program

---------------------------------------------------------------------------------------------------

run command

./program

---------------------------------------------------------------------------------------------------

command options

specify dataset file name           "-ds file"      (default datastet.csv)      *csv file
specify testset file name           "-ts file"      (default testset.csv)       *csv file
request stats of a column of ds     "-s column"     (default none)              *can be set to --all
specify the max depth of the tree   "-d depth"      (default infinite)          *root is depth=0
request the dataset of a node       "-n node"       (default none)              *uses dfs 

---------------------------------------------------------------------------------------------------

run command (example with options)
./program -ds ricardo_data.csv -ts ricardo_test.csv -s --all -d 4 -n 33

---------------------------------------------------------------------------------------------------
