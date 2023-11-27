#include "GraphMatrix.hpp"
#include <iostream>
#include <set>
#include <vector>
#include <utility> // para pair
using namespace std;

int main() {

    cout << "------ Matriz de adjacencia ------ " << endl;
    GraphMatrix g(4);

    g.print();

    cout << "--------------------------------"<< endl;

    g.add_edge(0,1,10);
    g.add_edge(1,2,1);
    g.add_edge(2,3,2);
    g.add_edge(3,1,3);


    g.print();
    cout << "--------------------------------"<< endl;
    // cout << "---------------- aaa ----------------"<< endl;
    // GraphMatrix cg = g;
    g.edmonds_algorithm(0);
    //g.print();
    return 0;
}