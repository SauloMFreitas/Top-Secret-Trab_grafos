#include <fstream>
#include <sstream>

#include "include/graph.hpp"

using namespace std;

const string REFERENCE = "https://github.com/prokls/edmonds-branching-algorithm/blob/master/haskell/edmonds.hs";

const string FILE_NAME = "graph3.txt";

vector<string> read_lines() {
    vector<string> lines;
    string line;
    ifstream input_file(FILE_NAME);

    if(!input_file.is_open()) {
        cerr << "Error: Unable to open file `" << FILE_NAME << "`" << endl;
        exit(EXIT_FAILURE);
    }

    while(getline(input_file, line)) lines.push_back(line);

    input_file.close();
    return lines;
}

int main() {
    Graph* g = new Graph(read_lines());
    cout << g->str() << endl;

    Graph arb = Graph::edmonds(*g);
    cout << arb.str() << endl;
    
    delete g;
    return 0;
}
