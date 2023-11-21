#include "GraphList.hpp"
#include "GraphMatrix.hpp"
#include <iostream>
#include <vector>
#include <utility> // para pair
using namespace std;

int main() {

    cout << "------ Matriz de adjacencia ------ " << endl;
    GraphMatrix g(4);

    g.add_edge(0,1,1);
    g.add_edge(1,2,1);
    g.add_edge(2,3,1);
    g.add_edge(3,1,1);
    
    g.print();

    vector<int> gc = g.findCycles();
    cout << "--------------------------------"<< endl;
    for(int i : gc){
        cout << i << " ";
    }
    cout << endl;

    cout << "--------------------------------"<< endl;
    GraphMatrix cg = g;
    cg.contract(gc);

    g.print();
    cout << "--------------------------------"<< endl;
    cg.print();

    return 0;
}