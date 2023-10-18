#include "dsets.h"
#include <iostream>

void DisjointSets::addelements(int num){
    //add n unconnected root nodes to the end of the vector
    while(num > 0){
        disj.push_back(-1);
        num--;
    }
}
int DisjointSets::find(int elem){
    //find the representative element of the set that contains the element
    //path coompression
    if(disj[elem] < 0) return elem;
    else return find(disj[elem]);
}
void DisjointSets::setunion(int a, int b){
    //merge two disjoint set, update the size and parent pointer
    //union by size
    a = find(a);
    b = find(b);
    int ns = disj[a] + disj[b];
    if(disj[a] < disj[b]){
        disj[b] = a;
        disj[a] = ns;
    }else{
        disj[a] = b;
        disj[b] = ns;
    }
} 
int DisjointSets::size(int elem){
    //return the size of the set that contains the element
    return -1*disj[find(elem)];
}
