#pragma once
#include <vector>
#include <string>
#include <unordered_map>
#include <queue>
#include <stack>
#include <set>

using namespace std;


struct Vertex{



    //dont need Date, Date used to distinguish the many graph variables

    string name;
    float price;
    float volume; //need volume?


    Vertex(){name = ""; price = 0; volume = 0;}

    Vertex(string _name, float _price, float _volume){
        name = _name; price = _price; volume = _volume;
    }

};


class Graph{


    //// Hidden attribute: Graphs are further distinguished by either all organic/non organic vertices
    ///  Attribute actually stored as a character at the end of key to a hashMap: either 'C' or 'O'


public:

    /*
     * Key is name of region, Value is the adjList, Pair of: destination name, then edge weight
     * Pair->first must be Vertex in order to be consistent with index 0: source region vertex info
     * In all other cases the Vertex will only have the destination name contained
     *
     */
    unordered_map< string, vector< pair<Vertex, int>>> adjList;


    int dateCount;

    /*
    * from a valid date, Exp: 11/04/15  , 1st time we read this date, date count = 0,
    * second time we read the date, date count = 1, meaning : 11/ 05 / 15,
    * this way we can get dates that are not actually part of the data set,
    * also a dateCount is much simpler than creating and verifying that a valid date exists
    */


    // Graph( int _dateCount );

    void InsertEdge(string &from, Vertex &to, int distance);

    string bfsPrice( const string& source, float min, float max );

    vector<string> bfsVolume( const string& source, float min, float max );

    string DfsPrice( const string& source, float min, float max );

    vector<string> DfsVolume( const string& source, float min, float max );




};


//Graph::Graph(int _dateCount){dateCount = _dateCount;}

void Graph::InsertEdge(string &from, Vertex &to, int distance) {


    //hashing for adjList is done based on name

    pair<Vertex, int> edge;
    edge.first = to;
    edge.second = distance;

    adjList[from].push_back(edge);


}

string Graph::bfsPrice(const string& source, float min, float max) {

    vector<string> priceRangeList;

    float compare = 0;

    float minPrice = 0;
    string minPriceName;

    //set and queue will store names of cities/region visited

    set<string> visited;
    queue<string> BFSqueue;

    visited.insert(source);
    BFSqueue.push(source);

    //Vpair dummy; //will be used to search and get "key" attribute data from the adjList

    while(!BFSqueue.empty()){

        string current = BFSqueue.front();
        BFSqueue.pop();

        //find should (always?) result in valid iterator (ok not always)
        //constant access of hash search
        auto iter = adjList.find(current);

        //don't do anything if current is not found in the adjList
        if(iter != adjList.end()){

            compare = iter->second[0].first.price;

            if(current == source){
                minPrice = compare;
                minPriceName = current;
            }

            else if(compare < minPrice){
                minPrice = compare;
                minPriceName = current;
            }
            else{
                //do nothing, minPrice not updated
            }

            //second is the vector of pairs, get index[0] of pairs = source data, get first = Vertex
            if( compare < max && compare > min )
                priceRangeList.push_back(iter->first);



            vector<pair<Vertex, int>> neighbors = adjList[current];
            for(int i = 1; i < neighbors.size(); i++){

                if(visited.count(neighbors[i].first.name) == 0){
                    visited.insert(neighbors[i].first.name);
                    BFSqueue.push(neighbors[i].first.name);
                }

            }


        }


    }

    return minPriceName;

}

vector<string> Graph::bfsVolume(const string& source, float min, float max) {

    ////Everything the same as bfsPrice, except search for volume range instead

    vector<string> volumeRangeList;

    float compare = 0;

    //set and queue will store names of cities/region visited

    set<string> visited;
    queue<string> BFSqueue;

    visited.insert(source);
    BFSqueue.push(source);

    //Vpair dummy; //will be used to search and get "key" attribute data from the adjList

    while(!BFSqueue.empty()){

        string current = BFSqueue.front();
        BFSqueue.pop();

        //find should (always?) result in valid iterator (ok not always)
        //constant access of hash search
        auto iter = adjList.find(current);

        //don't do anything if current is not found in the adjList
        if(iter != adjList.end()){

            compare = iter->second[0].first.price;

            ////maybe chnge to get max, now is getting min volume
            if( compare < max && compare > min )
                volumeRangeList.push_back(iter->first);



            vector<pair<Vertex, int>> neighbors = adjList[current];
            for(int i = 1; i < neighbors.size(); i++){

                if(visited.count(neighbors[i].first.name) == 0){
                    visited.insert(neighbors[i].first.name);
                    BFSqueue.push(neighbors[i].first.name);
                }

            }


        }

    }

    return volumeRangeList;

}

string Graph::DfsPrice(const string &source, float min, float max) {

    ////same as Bfs, but with stack instead

    vector<string> priceRangeList;

    float compare = 0;

    float minPrice = 0;
    string minPriceName;

    //set and queue will store names of cities/region visited

    set<string> visited;
    queue<string> dfsStack;

    visited.insert(source);
    dfsStack.push(source);

    //Vpair dummy; //will be used to search and get "key" attribute data from the adjList

    while(!dfsStack.empty()){

        string current = dfsStack.front();
        dfsStack.pop();

        //find should (always?) result in valid iterator (ok not always)
        //constant access of hash search
        auto iter = adjList.find(current);

        //don't do anything if current is not found in the adjList
        if(iter != adjList.end()){

            compare = iter->second[0].first.price;

            if(current == source){
                minPrice = compare;
                minPriceName = current;
            }

            else if(compare < minPrice){
                minPrice = compare;
                minPriceName = current;
            }
            else{
                //do nothing, minPrice not updated
            }

            //second is the vector of pairs, get index[0] of pairs = source data, get first = Vertex
            if( compare < max && compare > min )
                priceRangeList.push_back(iter->first);



            vector<pair<Vertex, int>> neighbors = adjList[current];
            for(int i = 1; i < neighbors.size(); i++){

                if(visited.count(neighbors[i].first.name) == 0){
                    visited.insert(neighbors[i].first.name);
                    dfsStack.push(neighbors[i].first.name);
                }

            }


        }


    }

    return minPriceName;


}

vector<string> Graph::DfsVolume(const string &source, float min, float max) {

    ////same as Bfs, but with stack instead
    vector<string> volumeRangeList;

    float compare = 0;

    //set and queue will store names of cities/region visited

    set<string> visited;
    queue<string> dfsStack;

    visited.emplace(source);
    dfsStack.push(source);

    //Vpair dummy; //will be used to search and get "key" attribute data from the adjList

    while(!dfsStack.empty()){

        string current = dfsStack.front();
        dfsStack.pop();

        //find should (always?) result in valid iterator (ok not always)
        //constant access of hash search
        auto iter = adjList.find(current);

        //don't do anything if current is not found in the adjList
        if(iter != adjList.end()){

            compare = iter->second[0].first.price;

            //second is the vector of pairs, get index[0] of pairs = source data, get first = Vertex
            if( compare < max && compare > min )
                volumeRangeList.push_back(iter->first);



            vector<pair<Vertex, int>> neighbors = adjList[current];
            for(int i = 1; i < neighbors.size(); i++){

                if(visited.count(neighbors[i].first.name) == 0){
                    visited.insert(neighbors[i].first.name);
                    dfsStack.push(neighbors[i].first.name);
                }

            }


        }


    }

    return volumeRangeList;


}



/*int Graph::getDateCount() const {
    return dateCount;
}

void Graph::setDateCount(int _dateCount) {
    Graph::dateCount = _dateCount;
}*/
