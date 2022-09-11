#include <iostream>
#include <fstream>
#include <SFML/Graphics.hpp>
#include "textureManager.h"
#include "textBox.h"
#include "button.h"
#include "avocado.h"
#include "spriteButton.h"
#include "Graph.h"
#include "edge.h"
#include <set>
#include <map>
#include <unordered_map>
#include <iomanip>
#include <sstream>
#include <math.h>
#include <cmath>
#include <vector>
#include <unordered_map>
#include <iostream>
#include <fstream>
#include <string>
#define earthRadiusKm 6371.0
using namespace std;

vector <string> decodeLine(string line);
void writeAvocadoPositions(unordered_map<string, avocado>& avocados);
void connectAvocados(unordered_map<string, avocado>& avos, unordered_map<string, Graph>& graph);

//HAVERSINE FUNCTION PROVIDED BY MrTJ on Stackoverflow
//used to calculate the distance between two cities (via distance formula on a sphere (HAVERSINE))
//https://stackoverflow.com/questions/10198985/calculating-the-distance-between-2-latitudes-and-longitudes-that-are-saved-in-a

// This function converts decimal degrees to radians
double deg2rad(double deg) {
    return (deg * M_PI / 180);
}

//  This function converts radians to decimal degrees
double rad2deg(double rad) {
    return (rad * 180 / M_PI);
}

/**
 * Returns the distance between two points on the Earth.
 * Direct translation from http://en.wikipedia.org/wiki/Haversine_formula
 * @param lat1d Latitude of the first point in degrees
 * @param lon1d Longitude of the first point in degrees
 * @param lat2d Latitude of the second point in degrees
 * @param lon2d Longitude of the second point in degrees
 * @return The distance between the two points in kilometers
 */

double distanceEarth(double lat1d, double lon1d, double lat2d, double lon2d) {
    double lat1r, lon1r, lat2r, lon2r, u, v;
    lat1r = deg2rad(lat1d);
    lon1r = deg2rad(lon1d);
    lat2r = deg2rad(lat2d);
    lon2r = deg2rad(lon2d);
    u = sin((lat2r - lat1r)/2);
    v = sin((lon2r - lon1r)/2);
    return 2.0 * earthRadiusKm * asin(sqrt(u * u + cos(lat1r) * cos(lat2r) * v * v));
}

void ReadCityLookUP(unordered_map<string, int> &_cityLook){

    //used for distance lookup table
    int index = 0;

    //key
    string tempCity;

    string cityListFile = "UniqueCities.txt";

    ifstream inCSV;  //re-use ifstream obj name

    inCSV.open(cityListFile);


    if(inCSV.is_open()){

        while(!inCSV.eof()){

            getline(inCSV, tempCity);


            _cityLook[tempCity] = index;
            index++;


        }
    }
}

vector<string> decodeLine(string line) {
    vector<string> returnVector;
    int oldCommaIndex = 0;
    int newCommaIndex = -1;
    char currentChar;
    for (long unsigned int i = 0; i < line.size(); ++i) {
        currentChar = line.at(i);
        if (currentChar == ',') {
            newCommaIndex = i;
            returnVector.push_back(line.substr(oldCommaIndex, newCommaIndex - oldCommaIndex));
            oldCommaIndex = newCommaIndex + 1;
        }
        if (i == line.size() - 1) {
            newCommaIndex = i;
            returnVector.push_back(line.substr(oldCommaIndex, newCommaIndex - oldCommaIndex + 1));
        }
    }
    return returnVector;
}

void writeAvocadoPositions(unordered_map<string, avocado>& avocados) {
    ofstream fileOut;
    fileOut.open("csvs/avocadoPositions.csv");
    for (auto iter = avocados.begin(); iter != avocados.end(); ++iter) {
        sf::Vector2f avocadoPosition = iter->second.getPosition();
        string avocadoX = to_string(avocadoPosition.x);
        string avocadoY = to_string(avocadoPosition.y);
        string fileOutLine = "";
        fileOutLine += iter->second.getName();
        fileOutLine += ",";
        fileOutLine += avocadoX;
        fileOutLine += ",";
        fileOutLine += avocadoY;
        fileOut << fileOutLine << endl;
    }
    fileOut.close();
}

void connectAvocados(unordered_map<string, avocado>& avos, unordered_map<string, Graph>& graph, vector<edge>& edges) {
    string fullKey = "2017-06-25C6";
    auto aList = graph[fullKey].adjList;
    //iterate through adjList to map avocados to vertices
    for (auto iter = aList.begin(); iter != aList.end(); ++iter) {
        string vertexName = iter->first;
        int avoIndex = -1;
        if (vertexName != "Midsouth" && vertexName != "West" && vertexName != "TotalUS" && vertexName != "Northeast" && vertexName != "Southeast" && vertexName != "SouthCentral" && vertexName != "Plains") {
            //we need to find the corresponding avocado
            if (avos.find(vertexName) != avos.end()) {
                //we have found the avocado
            }
//            for (int i = 0; i < avos.size(); ++i) {
//                if (avos[i].getName() == vertexName) {
//                    //we have found the avocado
//                    avoIndex = i;
//                    break;
//                }
//            }
            for (int i = 2; i < iter->second.size(); ++i) {
                //we are now accessing every neighbor (index = 0 is source)
                //call the add neighbor
                //pass the xPos and the yPos
                //call the add edge function
                string neighborVertexName = iter->second[i].first.name;
                int avoNeighborIndex = -1;
                if (vertexName != "Midsouth" && vertexName != "West" && vertexName != "TotalUS" && vertexName != "Northeast" && vertexName != "Southeast" && vertexName != "SouthCentral" && vertexName != "Plains") {
                    if (neighborVertexName != "Midsouth" && neighborVertexName != "West" && neighborVertexName != "TotalUS" && neighborVertexName != "Northeast" && neighborVertexName != "Southeast" && neighborVertexName != "SouthCentral" && neighborVertexName != "Plains") {
                        sf::Vector2f pos = sf::Vector2f(avos[neighborVertexName].getPosition());
                        avos[vertexName].addNeighbor(pos);
                        edge ed(avos[vertexName].getPosition(), pos);
                        edges.push_back(ed);
                    }
                }
            }
        }
    }
}

int main()
{
    //int vertexCount = 0;
    //START OF GRAPH FUNCTIONALITY
    //creat look-up city index for the distance look-up table
    unordered_map<string, int> cityLookUp;
    int cityIndex;
    int distanceTolerance = 800;

    string tempCityName;
    int tempEdgeDistance;

    ////Start of distance table fill in

    ReadCityLookUP(cityLookUp);

    vector<vector<double>> coordinates;
    string FileName = "Distance.csv";


    string location, lati, longi;
    double lat = 0;
    double lon = 0;
    ifstream CSVcoord;


    CSVcoord.open(FileName);

    if(CSVcoord.is_open()){

        while(!CSVcoord.eof()){
            getline(CSVcoord, location, ',');

            getline(CSVcoord, lati, ',');
            lat = stof(lati);


            getline(CSVcoord, longi);
            lon = stof(longi);

            vector<double> coords;
            coords.push_back(lat);
            coords.push_back(lon);
            //create a vector with lat, long coordinate pair and add it to the vector

            coordinates.push_back(coords);


        }

    }

    int distLookUp[54][54];
    for(int i = 0; i < 54; i++){
        for(int j = 0; j < 54; j++){
            double lat1 = coordinates[i][0];
            double long1 = coordinates[i][1];
            double lat2 = coordinates[j][0];
            double long2 = coordinates[j][1];
            distLookUp[i][j] = int (distanceEarth(lat1, long1, lat2, long2));    //call upon our function to calculate the distance
        }
    }

    ////End of distance table calculation


    //this is where we store the graphs objects into "layers" based on date
    unordered_map<string, Graph> graphCollection;


    string csvFileName = "Augmented_avocado_Compress.csv";
    ifstream inCSV;

    string date, price, volume, type , region;

    Vertex tempVertex;      //used for index  0 of adjList, represents source data
    Vertex dummyVertex;    // used for other indices of adjList, only using Vertex for consistency with index 0

    string charConv = "C";
    string charOrganic = "O";

    int startingDate;


    //// testing variables:

    int rowsRead = 0;



    inCSV.open(csvFileName);
    //Begin reading in data

    if(inCSV.is_open()){

        getline(inCSV, date);  //get rid of header


        while(!inCSV.eof()){

            getline(inCSV, date, ',');

            getline(inCSV, price, ',');

            getline(inCSV, volume, ',');

            getline(inCSV, type, ',');

            getline(inCSV, region);

            //region.pop_back();  // remove line break character


            tempVertex.volume= stof(volume);
            tempVertex.name = region;
            tempVertex.price =stof(price);


            //now we know which index(row) to match Dist-lookup column
            cityIndex = cityLookUp[region];


            //will not traverse the columns contagiously: use iterator
            int column;

            if(type == "organic"){date.append(charOrganic);}//add 'O' to date, make date(key) dependent on type
            else{date.append(charConv);}


            startingDate = graphCollection[date].dateCount;

//            if (region == "Detroit" && startingDate == 2) {
//                cout << "pog" << endl;
//            }

            if(startingDate < 7){

                if(startingDate != 0){date.append( to_string(startingDate) );}


                /*  if adjList @ index "region" is empty, add itself to index 0
               *  index 0 stores the source vertex infro like: price, volume, distance = 0 bec it is itself
               *  then add its adj: this process only done once per layer, per region
               */

                if(graphCollection[date].adjList[region].empty()){


                    graphCollection[date].InsertEdge(region, tempVertex,0);
                    //vertexCount++;

                    ////remember: cityLookUp is helper lookup for dist table look up

                    //secondary use of cityLookUp, now using to iterate through columns (same ordering as rows)
                    for(auto iter = cityLookUp.begin(); iter != cityLookUp.end(); iter++){

                        column = iter->second;

                        if(distLookUp[cityIndex][column] < distanceTolerance){
                            //capture cities within 600 miles

                            //need to create dummy Vertex (only use name attribute)
                            dummyVertex.name = iter->first;

                            tempEdgeDistance = distLookUp[cityIndex][column];

                            graphCollection[date].InsertEdge(region,dummyVertex,tempEdgeDistance);

                        }

                    }

                    if(graphCollection[date].adjList.size() == 54){

                        //just added last vertex to graph to fill it:
                        if(startingDate != 0){date.pop_back() ;}
                        graphCollection[date].dateCount++;

                    }

                }   ////INSERT EDGE

                    // else current layer is still not filled, but current region already added
                else{

                    //cycle through remaining days ahead to find potential room for region
                    //startingDate = graphCollection[date].dateCount;

                    if(startingDate != 0){date.pop_back() ;}

                    for(int i = startingDate; i < 7; i++){

                        date.append(to_string(i));
                        if(graphCollection[date].adjList[region].empty()){


                            graphCollection[date].InsertEdge(region, tempVertex,0);
                            //vertexCount++;

                            ////remember: cityLookUp is helper lookup for dist table look up

                            //secondary use of cityLookUp, now using to iterate through columns (same ordering as rows)
                            for(auto iter = cityLookUp.begin(); iter != cityLookUp.end(); iter++){

                                column = iter->second;

                                if(distLookUp[cityIndex][column] < distanceTolerance){
                                    //capture cities within 600 miles

                                    //need to create dummy Vertex (only use name attribute)
                                    dummyVertex.name = iter->first;

                                    tempEdgeDistance = distLookUp[cityIndex][column];

                                    graphCollection[date].InsertEdge(region,dummyVertex,tempEdgeDistance);

                                }

                            }

                            break; // don't want to add same data in multiple layers

                            /*if(graphCollection[date].adjList.size() == 54){

                                //just added last vertex to graph to fill it:

                                date.pop_back();

                                graphCollection[date].dateCount++;

                            }*/

                        }  ////INSERT EDGE
                        else{
                            // this layer also has region filled, correct date for next iteration
                            date.pop_back();
                        }
                    }
                }
            }
            rowsRead++;  //just analysis
        }
    }
    //END OF GRAPH FUNCTIONALITY


    //tests the graphCollection
//    cout << graphCollection.size() << endl;
//    cout << "size of adj list @: 2018-01-28C6 " << graphCollection["2018-01-28C6"].adjList.size() << endl;
//    cout << graphCollection["2018-01-28C6"].adjList["Detroit"].size() << endl;
//    cout << graphCollection["2018-01-28C6"].adjList["GrandRapids"].size() << endl;
//    cout << vertexCount << endl;




    vector<edge> edges;
    //load the names of all the cities
    string fileLine = "";
    vector<string> fileContent;
    ifstream file;
    file.open("csvs/avocadoPositions.csv");
    if (file.is_open()) {
        while (getline(file, fileLine)) {
            fileContent.push_back(fileLine);
        }
    }
    else {
        cout << "Improper Load" << endl;
    }

    vector<string> cityNames;
    vector<sf::Vector2f> avocadoPositions;
    for (int i = 0; i < fileContent.size(); ++i) {
        vector<string> tempVector = decodeLine(fileContent[i]);
        cityNames.push_back(tempVector[0]);
        float avoPosX = stof(tempVector[1]);
        float avoPosY = stof(tempVector[2]);
        sf::Vector2f avoPos = sf::Vector2f(avoPosX, avoPosY);
        avocadoPositions.push_back(avoPos);
    }

    file.close();
    float width = 1200;
    float height = 800;
    sf::RenderWindow window(sf::VideoMode((int) width, (int)height), "FR E SH VOCA DO");

    //setup maps
    vector<string> mapColors;
    int currentMapIndex = 0;
    mapColors.push_back("Green");
    mapColors.push_back("Red");
    mapColors.push_back("Orange");
    mapColors.push_back("Purple");
    vector<sf::Sprite> maps;
    for (int i = 0; i < mapColors.size(); ++i) {
        sf::Sprite map;
        string mapName = "pics/usaMap" + mapColors[i] + ".png";
        map.setTexture(TextureManager::GetTexture(mapName));
        map.setOrigin(map.getLocalBounds().width / 2, map.getLocalBounds().height / 2);
        map.setPosition(window.getSize().x / 2.0 - 150, window.getSize().y / 2.0 + 100);
        map.scale(.48, .48);
        maps.push_back(map);
    }
    sf::RectangleShape mapBackground(sf::Vector2f(window.getSize().x, window.getSize().y));
    mapBackground.setFillColor(sf::Color(63, 72, 204));

    sf::Sprite smilingAvocado;
    smilingAvocado.setTexture(TextureManager::GetTexture("pics/smilingAvocado.png"));
    smilingAvocado.setOrigin(smilingAvocado.getLocalBounds().width / 2, smilingAvocado.getLocalBounds().height / 2);
    smilingAvocado.setPosition(1100, 650);
    smilingAvocado.scale(.15, .15);
    bool smileyBig = false;

    sf::Sprite scaredAvocado;
    scaredAvocado.setTexture(TextureManager::GetTexture("pics/scaredAvocado3.png"));
    scaredAvocado.setOrigin(scaredAvocado.getLocalBounds().width / 2, scaredAvocado.getLocalBounds().height / 2);
    scaredAvocado.setPosition(1100, 650);
    scaredAvocado.scale(.15, .15);
    bool scaredBig = false;
    bool isScared = false;
    int scaredCount = 0;

    //setup multiple avocados
    //make an avocado class where each avocado has a selected and unselected version
    unordered_map<string, avocado> avocados;
    int numAvocados = 54;
    //float avoX = width / numAvocados;
    for (int i = 0; i < numAvocados; ++i) {
        avocado avo(cityNames[i]);
        //avo.setPosition(sf::Vector2f(10 * i + window.getSize().x / 10, 5 * i + window.getSize().y / 10));
        avo.setPosition(sf::Vector2f(avocadoPositions[i].x, avocadoPositions[i].y));
        avocados[cityNames[i]] = avo;
    }

    //add the neighbors
    connectAvocados(avocados, graphCollection, edges);

    //make textbox by making a textBox object and setting up a white rectangle
    float searchX = 30;
    float searchY = 30;
    sf::Font font;
    font.loadFromFile("fonts/Roboto-Regular.ttf");
    textBox search(15, sf::Color::Black, false);
    search.setFont(font);
    search.setPosition(searchX, searchY);
    search.setLimit(true, 21);
    search.setGrayText("Enter Location");
    string userInput = "";

    //make all the year select buttons
    vector<button> yearButtons;
    int numYearButtons = 4;
    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < 2; ++j) {
            button yearButton("", 5, 12, sf::Color::White, sf::Color::Black);
            yearButton.setPosition(sf::Vector2f(55 + 80 * j, 80 + 40 * i));
            yearButton.setFont(font);
            yearButton.setHoverColor(sf::Color::Red);
            yearButtons.push_back(yearButton);
        }
    }
    yearButtons[0].setSelected(true);


    //set up the year texts
    vector<sf::Text> yearTexts;
    int numYears = 4;
    int yearTextIterations = 0;
    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < 2; ++j) {
            sf::Text yearText;
            yearText.setPosition(sf::Vector2f(70 + 80 * j, 77 + 40 * i));
            yearText.setFont(font);
            yearText.setFillColor(sf::Color::Black);
            yearText.setCharacterSize(12);
            yearText.setString(to_string(2015 + yearTextIterations));
            yearTexts.push_back(yearText);
            yearTextIterations += 1;
        }
    }

    //setup the month buttons
    vector<button> monthButtons;
    int numMonthButtons = 12;
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 4; ++j) {
            button monthButton("", 5, 12, sf::Color::White, sf::Color::Black);
            monthButton.setPosition(sf::Vector2f(280 + 80 * j, 30 + 60 * i));
            monthButton.setFont(font);
            monthButton.setHoverColor(sf::Color::Red);
            monthButtons.push_back(monthButton);
        }
    }
    monthButtons[0].setSelected(true);


    //set up the month texts
    vector<string> monthNames;
    monthNames.push_back("January");
    monthNames.push_back("February");
    monthNames.push_back("March");
    monthNames.push_back("April");
    monthNames.push_back("May");
    monthNames.push_back("June");
    monthNames.push_back("July");
    monthNames.push_back("August");
    monthNames.push_back("September");
    monthNames.push_back("October");
    monthNames.push_back("November");
    monthNames.push_back("December");

    vector<sf::Text> monthTexts;
    int numMonths = 12;
    int monthTextIterations = 0;
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 4; ++j) {
            sf::Text monthText;
            monthText.setPosition(sf::Vector2f(295 + 80 * j, 27 + 60 * i));
            monthText.setFont(font);
            monthText.setFillColor(sf::Color::Black);
            monthText.setCharacterSize(12);
            monthText.setString(monthNames[monthTextIterations]);
            monthTexts.push_back(monthText);
            monthTextIterations += 1;
        }
    }

    //make all the tens select buttons
    vector<button> tensButtons;
    int numTensButtons = 4;
    for (int i = 0; i < 4; ++i) {
        button tensButton("", 5, 12, sf::Color::White, sf::Color::Black);
        tensButton.setPosition(sf::Vector2f(630, 15 + 18 * i));
        tensButton.setFont(font);
        tensButton.setHoverColor(sf::Color::Red);
        tensButtons.push_back(tensButton);

    }
    tensButtons[0].setSelected(true);


    //set up the tens texts
    vector<sf::Text> tensTexts;
    int numTens = 4;
    int tensTextIterations = 0;
    for (int i = 0; i < 4; ++i) {
        sf::Text tensText;
        tensText.setPosition(sf::Vector2f(645, 12 + 18 * i));
        tensText.setFont(font);
        tensText.setFillColor(sf::Color::Black);
        tensText.setCharacterSize(12);
        tensText.setString(to_string(tensTextIterations));
        tensTexts.push_back(tensText);
        tensTextIterations += 1;
    }

    //make all the ones select buttons
    vector<button> onesButtons;
    int numOnesButtons = 10;
    for (int i = 0; i < 10; ++i) {
        button onesButton("", 5, 12, sf::Color::White, sf::Color::Black);
        onesButton.setPosition(sf::Vector2f(665, 15 + 18 * i));
        onesButton.setFont(font);
        onesButton.setHoverColor(sf::Color::Red);
        onesButtons.push_back(onesButton);

    }
    onesButtons[1].setSelected(true);


    //set up the ones texts
    vector<sf::Text> onesTexts;
    int numOnes = 10;
    int onesTextIterations = 0;
    for (int i = 0; i < 10; ++i) {
        sf::Text onesText;
        onesText.setPosition(sf::Vector2f(680, 12 + 18 * i));
        onesText.setFont(font);
        onesText.setFillColor(sf::Color::Black);
        onesText.setCharacterSize(12);
        onesText.setString(to_string(onesTextIterations));
        onesTexts.push_back(onesText);
        onesTextIterations += 1;
    }

    //make all the offset select buttons
    vector<button> offsetButtons;
    int numOffsetButtons = 7;
    for (int i = 0; i < 7; ++i) {
        button offsetButton("", 5, 12, sf::Color::White, sf::Color::Black);
        offsetButton.setPosition(sf::Vector2f(700, 15 + 18 * i));
        offsetButton.setFont(font);
        offsetButton.setHoverColor(sf::Color::Red);
        offsetButtons.push_back(offsetButton);

    }
    offsetButtons[0].setSelected(true);


    //set up the offset texts
    vector<sf::Text> offsetTexts;
    int numOffsets = 7;
    int offsetTextIterations = 0;
    for (int i = 0; i < 7; ++i) {
        sf::Text offsetText;
        offsetText.setPosition(sf::Vector2f(715, 12 + 18 * i));
        offsetText.setFont(font);
        offsetText.setFillColor(sf::Color::Black);
        offsetText.setCharacterSize(12);
        offsetText.setString(to_string(offsetTextIterations));
        offsetTexts.push_back(offsetText);
        offsetTextIterations += 1;
    }

    //make all the isOrganic select buttons
    vector<button> isOrganicButtons;
    int numIsOrganicButtons = 2;
    for (int i = 0; i < 2; ++i) {
        button isOrganicButton("", 5, 12, sf::Color::White, sf::Color::Black);
        isOrganicButton.setPosition(sf::Vector2f(630, 159 + 18 * i));
        isOrganicButton.setFont(font);
        isOrganicButton.setHoverColor(sf::Color::Red);
        isOrganicButtons.push_back(isOrganicButton);

    }
    isOrganicButtons[0].setSelected(true);

    vector<string> avocadoTypes;
    avocadoTypes.push_back("C");
    avocadoTypes.push_back("O");
    int avocadoTypesIndex = 0;



    //set the isOrganic texts
    vector<sf::Text> isOrganicTexts;
    int numIsOrganics = 2;
    int isOrganicTextIterations = 0;
    for (int i = 0; i < 2; ++i) {
        sf::Text isOrganicText;
        isOrganicText.setPosition(sf::Vector2f(645, 156 + 18 * i));
        isOrganicText.setFont(font);
        isOrganicText.setFillColor(sf::Color::Black);
        isOrganicText.setCharacterSize(12);
        isOrganicText.setString(avocadoTypes[avocadoTypesIndex]);
        isOrganicTexts.push_back(isOrganicText);
        isOrganicTextIterations += 1;
        avocadoTypesIndex += 1;
    }

    //make all the isBFS select buttons
    vector<button> isBFSButtons;
    int numIsBFSButtons = 2;
    for (int i = 0; i < 2; ++i) {
        button isBFSButton("", 5, 12, sf::Color::White, sf::Color::Black);
        isBFSButton.setPosition(sf::Vector2f(70 + 50 * i, 170));
        isBFSButton.setFont(font);
        isBFSButton.setHoverColor(sf::Color::Red);
        isBFSButtons.push_back(isBFSButton);

    }
    isBFSButtons[0].setSelected(true);

    //set the isBFS texts
    vector<string> searchNames;
    searchNames.push_back("BFS");
    searchNames.push_back("DFS");
    vector<sf::Text> isBFSTexts;
    int numIsBFSs = 2;
    int isBFSTextIterations = 0;
    for (int i = 0; i < 2; ++i) {
        sf::Text isBFSText;
        isBFSText.setPosition(sf::Vector2f(85 + 50 * i, 167));
        isBFSText.setFont(font);
        isBFSText.setFillColor(sf::Color::Black);
        isBFSText.setCharacterSize(12);
        isBFSText.setString(searchNames[i]);
        isBFSTexts.push_back(isBFSText);
        isBFSTextIterations += 1;
    }

    //set up the region name texts
    vector<string> regionNames;
    regionNames.push_back("TotalUS");
    regionNames.push_back("West");
    regionNames.push_back("Southeast");
    regionNames.push_back("SouthCentral");
    regionNames.push_back("Northeast");
    regionNames.push_back("Midsouth");
    regionNames.push_back("Plains");

    vector<sf::Text> regionTexts;
    int numRegions = 7;
    int regionTextIterations = 0;
    for (int i = 0; i < 7; ++i) {
        sf::Text regionText;
        regionText.setPosition(sf::Vector2f(900, 465 + 42 * i));
        regionText.setFont(font);
        regionText.setFillColor(sf::Color::Black);
        regionText.setCharacterSize(12);
        regionText.setString(regionNames[i]);
        regionTexts.push_back(regionText);
        regionTextIterations += 1;
    }

    //set up the dates search button
    spriteButton searchButton("pics/searchButton.png", "pics/searchButtonHovered.png");
    searchButton.setPosition(sf::Vector2f(780, 90));
    searchButton.scale(.4, .4);

    //set up the "search" frame
    //this include "Search for:"
    //the current date
    //and the search word on the button
    sf::Text dateSearchText;
    dateSearchText.setFont(font);
    dateSearchText.setCharacterSize(20);
    dateSearchText.setPosition(815, 15);
    dateSearchText.setFillColor(sf::Color::Black);
    dateSearchText.setString("Set Date To");

    sf::Text searchText;
    searchText.setFont(font);
    searchText.setCharacterSize(20);
    searchText.setPosition(827, 143);
    searchText.setFillColor(sf::Color::Black);
    searchText.setString("EXECUTE");


    //setup the user panel
    sf::RectangleShape userPanel(sf::Vector2f(window.getSize().x, 200));
    userPanel.setPosition(0, 0);
    userPanel.setFillColor(sf::Color(100, 100, 100));
    sf::RectangleShape userPanelBorder(sf::Vector2f(window.getSize().x, 2));
    userPanelBorder.setPosition(0, userPanel.getSize().y);
    userPanelBorder.setFillColor(sf::Color::Black);
    sf::RectangleShape dividerA(sf::Vector2f(2 , 200));
    dividerA.setPosition(250, 0);
    dividerA.setFillColor(sf::Color::Black);
    sf::RectangleShape dividerB(sf::Vector2f(2, 200));
    dividerB.setPosition(610, 0);
    dividerB.setFillColor(sf::Color::Black);
    sf::RectangleShape dividerC(sf::Vector2f(2, 200));
    dividerC.setPosition(740, 0);
    dividerC.setFillColor(sf::Color::Black);
    sf::RectangleShape dividerD(sf::Vector2f(250, 2));
    dividerD.setPosition(0, 150);
    dividerD.setFillColor(sf::Color::Black);


    //setup the description panel on the right side
    sf::RectangleShape descriptionPanel(sf::Vector2f(200, window.getSize().y));
    descriptionPanel.setPosition(window.getSize().x - 200, 0);
    descriptionPanel.setFillColor(sf::Color(100, 100, 100));
    sf::RectangleShape descriptionPanelBorder(sf::Vector2f(2, window.getSize().y));
    descriptionPanelBorder.setPosition(window.getSize().x - descriptionPanel.getSize().x, 0);
    descriptionPanelBorder.setFillColor(sf::Color::Black);

    string selectedCity = "";

    //raw, instant input initialized
    string userYearInput = "2015";
    string userMonthInput = "January";
    string userMonthInputNum = "01";
    string userTensInput = "1";
    string userOnesInput = "1";
    string userOffsetInput = "";
    string userDayInput = userTensInput + userOnesInput;
    string userDateInput = userMonthInput + " " + userDayInput + ", " + userYearInput;
    string userIsOrganicInput = "C";
    string userIsBFSInput = "BFS";

    //first index of the string is yyyy-mm-ddCO
    string userInputKey = userYearInput + "-" + userMonthInputNum + "-" + userTensInput + userOnesInput + userIsOrganicInput + userOffsetInput;

    //last input information
    string lastUserYearInput = "2015";
    string lastUserMonthInput = "January";
    string lastUserMonthInputNum = "01";
    string lastUserTensInput = "1";
    string lastUserOnesInput = "1";
    string lastUserDayInput = userTensInput + userOnesInput;
    string lastUserDateInput = userMonthInput + " " + userDayInput + ", " + userYearInput;
    string lastUserIsOrganicInput = "C";
    string lastUserOffsetInput = "";
    string lastUserIsBFSInput = "BFS";

    string lastUserInputKey = userYearInput + "-" + userMonthInputNum + "-" + userTensInput + userOnesInput + userIsOrganicInput + userOffsetInput;

    sf::Text dateText;
    dateText.setFont(font);
    dateText.setCharacterSize(25);
    dateText.setPosition(780, 45);
    dateText.setFillColor(sf::Color::Black);
    dateText.setString(userDateInput);

    sf::Text offsetText;
    offsetText.setFont(font);
    offsetText.setCharacterSize(12);
    offsetText.setPosition(750, 85);
    offsetText.setFillColor(sf::Color::Black);
    offsetText.setString("Offset: " + userOffsetInput);

    sf::Text isOrganicText;
    isOrganicText.setFont(font);
    isOrganicText.setCharacterSize(12);
    isOrganicText.setPosition(750, 105);
    isOrganicText.setFillColor(sf::Color::Black);
    isOrganicText.setString(userIsOrganicInput);
    if (isOrganicText.getString() == "C") {
        isOrganicText.setString("Conventional");
    }
    else {
        isOrganicText.setString("Organic");
    }

    sf::Text invalidText;
    invalidText.setFont(font);
    invalidText.setCharacterSize(18);
    invalidText.setPosition(860, 85);
    invalidText.setFillColor(sf::Color::Red);
    invalidText.setString("(Invalid Input!)");

    sf::Text lastDateText;
    lastDateText.setFont(font);
    lastDateText.setCharacterSize(18);
    lastDateText.setPosition(730, 202);
    lastDateText.setFillColor(sf::Color::Black);
    lastDateText.setString("Date Displayed: " + lastUserDateInput);

    string cityHighlighted = "";
    string lastCityHighlighted = "";
    cityHighlighted = graphCollection[userInputKey].bfsPrice("Detroit", .32, 4.56);
    if (!cityHighlighted.empty()) {
        avocados[cityHighlighted].setHighlighted(true);
        lastCityHighlighted = cityHighlighted;
        //cout << "we made it in here" << endl;
    }

    //make all the text objects for the selected avocado
    //display the name, display the price, and display the total volume
    sf::Text regionText;
    regionText.setFont(font);
    regionText.setCharacterSize(14);
    regionText.setPosition(1010, 70);
    regionText.setFillColor(sf::Color::Black);
    regionText.setString("Region:");

    sf::Text priceText;
    priceText.setFont(font);
    priceText.setCharacterSize(14);
    priceText.setPosition(1010, 250);
    priceText.setFillColor(sf::Color::Black);
    priceText.setString("Average Price:");

    sf::Text volumeText;
    volumeText.setFont(font);
    volumeText.setCharacterSize(14);
    volumeText.setPosition(1010, 430);
    volumeText.setFillColor(sf::Color::Black);
    volumeText.setString("Units Sold:");

    sf::Text selectedCityText;
    selectedCityText.setFont(font);
    selectedCityText.setCharacterSize(18);
    selectedCityText.setPosition(1010, 100);
    selectedCityText.setFillColor(sf::Color::Black);
    selectedCityText.setString(selectedCity);

    sf::Text selectedCityPriceText;
    selectedCityPriceText.setFont(font);
    selectedCityPriceText.setCharacterSize(18);
    selectedCityPriceText.setPosition(1010, 280);
    selectedCityPriceText.setFillColor(sf::Color::Black);
    selectedCityPriceText.setString("");

    sf::Text selectedCityVolumeText;
    selectedCityVolumeText.setFont(font);
    selectedCityVolumeText.setCharacterSize(18);
    selectedCityVolumeText.setPosition(1010, 460);
    selectedCityVolumeText.setFillColor(sf::Color::Black);
    selectedCityVolumeText.setString("");

    //to test neighbor function
//    edge testEdge(sf::Vector2f(220, 270), sf::Vector2f(146, 298));
//    edge testEdge2(sf::Vector2f(146, 298), sf::Vector2f(220, 270));
//    cout << avocados[13].getName() << endl;
//    cout << avocados[13].getPosition().x << endl;
//    for (int i = 0; i < avocados[13].getNeighbors().size(); ++i) {
//        cout << avocados[13].getNeighbors()[i].x << endl;
//    }
//    cout << edges.size() << endl;
//    for (int i = 0; i < edges.size(); ++i) {
//        cout << edges[i].getFrom().x << " " << edges[i].getFrom().y << " " << edges[i].getTo().x << " " << edges[i].getTo().y << endl;
//    }

    while (window.isOpen())
    {
        string currentInput = search.getCurrentString();

        //Handle all user events
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed) {
                //for positioning the avocados within the application
                //writeAvocadoPositions(avocados);
                window.close();
            }
            else if (event.type == sf::Event::TextEntered) {
                string newInput = search.typedOn(event);
                if (userInput == newInput) {
                    //do nothing
                }
                else {
                    //we must have pressed enter
                    //modify what we compare the search to and also make it non case sensitive preferably
                    int selectedIndex = -1;
                    if (avocados.find(newInput) != avocados.end()) {
                        avocados[newInput].setSelected(true);
                        selectedCity = avocados[newInput].getName();
                        cout << selectedCity << endl;
                    }
//                    for (int i = 0; i < avocados.size(); ++i) {
//                        if (newInput == avocados[i].getName()) {
//                            //can only have one avocado selected at a time
//                            avocados[i].setSelected(true);
//                            selectedCity = avocados[i].getName();
//                            cout << selectedCity << endl;
//                            selectedIndex = i;
//                        }
//                    }
                    //will turn off the previous selected avocado hopefully
                    for (auto iter = avocados.begin(); iter != avocados.end(); ++iter) {
                        if (iter->second.isSelected()) {
                            if (iter->first != newInput) {
                                iter->second.setSelected(false);
                            }
//                            if (selectedIndex != -1) {
//                                if (i != selectedIndex) {
//                                    avocados[i].setSelected(false);
//                                }
//                            }
                        }
                    }
                    userInput = newInput;
                }
            }
            else if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    sf::Vector2i position = sf::Mouse::getPosition(window);

                    search.clickedOn(event, window, position);

                    if (smilingAvocado.getGlobalBounds().contains(position.x, position.y)) {
                        isScared = true;
                    }
                    //chagne the condition to look at whether or not the current key exists
                    if (searchButton.isMouseOver(window)) {
                        if (graphCollection.find(userInputKey) == graphCollection.end()) {
                            //do nothing
                        }
                        else {
                            if (userInputKey != lastUserInputKey) {
                                currentMapIndex += 1;
                                //do the bfs or dfs, and set the resulting avocado to highlighted
                                if (userIsBFSInput == "BFS") {
                                    cityHighlighted = graphCollection[userInputKey].bfsPrice("Detroit", 0.32, 4.56);
                                    if (cityHighlighted != "") {
                                        if (cityHighlighted != lastCityHighlighted) {
                                            avocados[cityHighlighted].setHighlighted(true);
                                            avocados[lastCityHighlighted].setHighlighted(false);
                                            lastCityHighlighted = cityHighlighted;
                                        }
                                    }
                                }
                                else {
                                    cityHighlighted = graphCollection[userInputKey].DfsPrice("Detroit", 0.32, 4.56);
                                    if (cityHighlighted != "") {
                                        if (cityHighlighted != lastCityHighlighted) {
                                            avocados[cityHighlighted].setHighlighted(true);
                                            avocados[lastCityHighlighted].setHighlighted(false);
                                            lastCityHighlighted = cityHighlighted;
                                        }
                                    }
                                }
//                                for (auto iter = avocados.begin(); iter != avocados.end(); ++iter) {
//                                    if (iter->second.isHighlighted()) {
//                                        if (iter->first != cityHighlighted) {
//                                            avocados[iter->first].setHighlighted(false);
//                                        }
//                                    }
//                                }


                                //update all of the "last" variables
                                lastUserYearInput = userYearInput;
                                lastUserMonthInput = userMonthInput;
                                lastUserMonthInputNum = userMonthInputNum;
                                lastUserTensInput = userTensInput;
                                lastUserOnesInput = userOnesInput;
                                lastUserDayInput = userDayInput;
                                lastUserDateInput = userDateInput;
                                lastUserIsOrganicInput = userIsOrganicInput;
                                lastUserOffsetInput = userOffsetInput;
                                lastUserIsBFSInput = userIsBFSInput;
                                lastUserInputKey = userInputKey;
                            }
                        }
                    }

                    int yearButtonSelectedIndex = -1;
                    for (int i = 0; i < numYearButtons; ++i) {
                        if (yearButtons[i].isMouseOver(window)) {
                            yearButtons[i].clickedOn();
                            userYearInput = to_string(2015 + i);
                            yearButtonSelectedIndex = i;
                        }
                    }

                    //will turn off the previous selected yearButton
                    for (int i = 0; i < numYearButtons; ++i) {
                        if (yearButtons[i].isSelected()) {
                            if (yearButtonSelectedIndex != -1) {
                                if (i != yearButtonSelectedIndex) {
                                    yearButtons[i].setSelected(false);
                                }
                            }
                        }
                    }

                    int monthButtonSelectedIndex = -1;
                    for (int i = 0; i < numMonthButtons; ++i) {
                        if (monthButtons[i].isMouseOver(window)) {
                            monthButtons[i].clickedOn();
                            userMonthInput = monthTexts[i].getString();
                            monthButtonSelectedIndex = i;
                        }
                    }

                    //will turn off the previous selected monthButton
                    for (int i = 0; i < numMonthButtons; ++i) {
                        if (monthButtons[i].isSelected()) {
                            if (monthButtonSelectedIndex != -1) {
                                if (i != monthButtonSelectedIndex) {
                                    monthButtons[i].setSelected(false);
                                }
                            }
                        }
                    }

                    int tensButtonSelectedIndex = -1;
                    for (int i = 0; i < numTensButtons; ++i) {
                        if (tensButtons[i].isMouseOver(window)) {
                            tensButtons[i].clickedOn();
                            userTensInput = tensTexts[i].getString();
//                            if (userTensInput == "0") {
//                                userTensInput = "";
//                            }
                            tensButtonSelectedIndex = i;
                        }
                    }

                    //will turn off the previous selected tensButton
                    for (int i = 0; i < numTensButtons; ++i) {
                        if (tensButtons[i].isSelected()) {
                            if (tensButtonSelectedIndex != -1) {
                                if (i != tensButtonSelectedIndex) {
                                    tensButtons[i].setSelected(false);
                                }
                            }
                        }
                    }

                    int onesButtonSelectedIndex = -1;
                    for (int i = 0; i < numOnesButtons; ++i) {
                        if (onesButtons[i].isMouseOver(window)) {
                            onesButtons[i].clickedOn();
                            userOnesInput = onesTexts[i].getString();
                            onesButtonSelectedIndex = i;
                        }
                    }

                    //will turn off the previous selected onesButton
                    for (int i = 0; i < numOnesButtons; ++i) {
                        if (onesButtons[i].isSelected()) {
                            if (onesButtonSelectedIndex != -1) {
                                if (i != onesButtonSelectedIndex) {
                                    onesButtons[i].setSelected(false);
                                }
                            }
                        }
                    }

                    int isOrganicButtonSelectedIndex = -1;
                    for (int i = 0; i < numIsOrganicButtons; ++i) {
                        if (isOrganicButtons[i].isMouseOver(window)) {
                            isOrganicButtons[i].clickedOn();
                            userIsOrganicInput = isOrganicTexts[i].getString();
                            isOrganicButtonSelectedIndex = i;
                        }
                    }

                    //will turn off the previous selected onesButton
                    for (int i = 0; i < numIsOrganicButtons; ++i) {
                        if (isOrganicButtons[i].isSelected()) {
                            if (isOrganicButtonSelectedIndex != -1) {
                                if (i != isOrganicButtonSelectedIndex) {
                                    isOrganicButtons[i].setSelected(false);
                                }
                            }
                        }
                    }

                    int isBFSButtonSelectedIndex = -1;
                    for (int i = 0; i < numIsBFSButtons; ++i) {
                        if (isBFSButtons[i].isMouseOver(window)) {
                            isBFSButtons[i].clickedOn();
                            userIsBFSInput = isBFSTexts[i].getString();
                            isBFSButtonSelectedIndex = i;
                        }
                    }

                    //will turn off the previous selected isBFSButton
                    for (int i = 0; i < numIsBFSButtons; ++i) {
                        if (isBFSButtons[i].isSelected()) {
                            if (isBFSButtonSelectedIndex != -1) {
                                if (i != isBFSButtonSelectedIndex) {
                                    isBFSButtons[i].setSelected(false);
                                }
                            }
                        }
                    }

                    int offsetButtonSelectedIndex = -1;
                    for (int i = 0; i < numOffsetButtons; ++i) {
                        if (offsetButtons[i].isMouseOver(window)) {
                            offsetButtons[i].clickedOn();
                            userOffsetInput = offsetTexts[i].getString();
                            if (userOffsetInput == "0") {
                                userOffsetInput = "";
                            }
                            offsetButtonSelectedIndex = i;
                        }
                    }

                    //will turn off the previous selected onesButton
                    for (int i = 0; i < numOffsetButtons; ++i) {
                        if (offsetButtons[i].isSelected()) {
                            if (offsetButtonSelectedIndex != -1) {
                                if (i != offsetButtonSelectedIndex) {
                                    offsetButtons[i].setSelected(false);
                                }
                            }
                        }
                    }

                    int selectedIndex = -1;
                    //string selectedAvo = "";
                    for (auto iter = avocados.begin(); iter != avocados.end(); ++iter) {
                        if (iter->second.isMouseOver(window)) {
                            //can only have one avocado selected at a time
                            iter->second.clickedOn();
                            selectedCity = iter->second.getName();
                            //cout << selectedCity << endl;
                            //selectedIndex = i;
                        }
                    }

                    //will turn off the previous selected avocado hopefully
                    for (auto iter = avocados.begin(); iter != avocados.end(); ++iter) {
                        if (iter->second.isSelected()) {
                            if (iter->first != selectedCity) {
                                iter->second.setSelected(false);
                            }
//                            if (selectedIndex != -1) {
//                                if (i != selectedIndex) {
//                                    avocados[i].setSelected(false);
//                                }
//                            }
                        }
                    }
                }
                else if (event.mouseButton.button == sf::Mouse::Right) {
                    sf::Vector2i position = sf::Mouse::getPosition(window);
                    //this for loop was used to position the avocados by eye
                    //also change shutdown procedure
//                    for (auto iter = avocados.begin(); iter != avocados.end(); ++iter) {
//                        if (iter->second.isSelected()) {
//                            sf::Vector2f newPos = sf::Vector2f((float) position.x, (float) position.y);
//                            iter->second.setPosition(newPos);
//                        }
//                    }
                }
            }
            else if (event.type == sf::Event::MouseMoved) {
                for (auto iter = avocados.begin(); iter != avocados.end(); ++iter) {
                    iter->second.isMouseOver(window);
                }
                sf::Vector2i position = sf::Mouse::getPosition(window);
                if (smilingAvocado.getGlobalBounds().contains(position.x, position.y)) {
                    if (!smileyBig) {
                        smilingAvocado.scale(1.2, 1.2);
                        scaredAvocado.scale(1.2, 1.2);
                        smileyBig = !smileyBig;
                    }
                }
                else {
                    if (smileyBig) {
                        smilingAvocado.scale(1 / 1.2, 1 / 1.2);
                        scaredAvocado.scale(1 / 1.2, 1 / 1.2);
                        smileyBig = !smileyBig;
                    }
                }
            }
        }

        if (currentMapIndex > maps.size() - 1) {
            currentMapIndex = 0;
        }

        userDayInput = userTensInput + userOnesInput;
        userDateInput = userMonthInput + " " + userDayInput + ", " + userYearInput;
        dateText.setString(userDateInput);
        lastDateText.setString("Date Displayed: " + lastUserDateInput);
        if (userOffsetInput == "") {
            offsetText.setString("Offset: 0");
        }
        else {
            offsetText.setString("Offset: " + userOffsetInput);
        }
        isOrganicText.setString(userIsOrganicInput);
        if (isOrganicText.getString() == "C") {
            isOrganicText.setString("Conventional");
        }
        else {
            isOrganicText.setString("Organic");
        }

        if (userMonthInput == "January") {
            userMonthInputNum = "01";
        }
        else if (userMonthInput == "February") {
            userMonthInputNum = "02";
        }
        else if (userMonthInput == "March") {
            userMonthInputNum = "03";
        }
        else if (userMonthInput == "April") {
            userMonthInputNum = "04";
        }
        else if (userMonthInput == "May") {
            userMonthInputNum = "05";
        }
        else if (userMonthInput == "June") {
            userMonthInputNum = "06";
        }
        else if (userMonthInput == "July") {
            userMonthInputNum = "07";
        }
        else if (userMonthInput == "August") {
            userMonthInputNum = "08";
        }
        else if (userMonthInput == "September") {
            userMonthInputNum = "09";
        }
        else if (userMonthInput == "October") {
            userMonthInputNum = "10";
        }
        else if (userMonthInput == "November") {
            userMonthInputNum = "11";
        }
        else if (userMonthInput == "December") {
            userMonthInputNum = "12";
        }

        userInputKey = userYearInput + "-" + userMonthInputNum + "-" + userTensInput + userOnesInput + userIsOrganicInput + userOffsetInput;

        //cout << userInputKey << endl;
        //cout << userDateInput << endl;
        //cout << userTensInput << userOnesInput << endl;

        //clear the window
        window.clear();

        //draw the new window
        //draw the background
        window.draw(mapBackground);
        window.draw(maps[currentMapIndex]);

        for (int i = 0; i < edges.size(); ++i) {
            edges[i].drawTo(window);
        }

        for (auto iter = avocados.begin(); iter != avocados.end(); ++iter) {
            iter->second.drawTo(window);
        }

        window.draw(userPanel);
        window.draw(userPanelBorder);
        window.draw(dividerA);
        window.draw(dividerB);
        window.draw(dividerC);
        window.draw(dividerD);
        window.draw(descriptionPanel);
        window.draw(descriptionPanelBorder);

        search.drawTo(window);
        for (int i = 0; i < numYearButtons; ++i) {
            yearButtons[i].drawTo(window);
        }
        for (int i = 0; i < numYears; ++i) {
            window.draw(yearTexts[i]);
        }
        for (int i = 0; i < numMonthButtons; ++i) {
            monthButtons[i].drawTo(window);
        }
        for (int i = 0; i < numMonths; ++i) {
            window.draw(monthTexts[i]);
        }
        for (int i = 0; i < numTensButtons; ++i) {
            tensButtons[i].drawTo(window);
        }
        for (int i = 0; i < numTens; ++i) {
            window.draw(tensTexts[i]);
        }
        for (int i = 0; i < numOnesButtons; ++i) {
            onesButtons[i].drawTo(window);
        }
        for (int i = 0; i < numOnes; ++i) {
            window.draw(onesTexts[i]);
        }
        for (int i = 0; i < numIsOrganicButtons; ++i) {
            isOrganicButtons[i].drawTo(window);
        }
        for (int i = 0; i < numIsOrganics; ++i) {
            window.draw(isOrganicTexts[i]);
        }
        for (int i = 0; i < numOffsetButtons; ++i) {
            offsetButtons[i].drawTo(window);
        }
        for (int i = 0; i < numOffsets; ++i) {
            window.draw(offsetTexts[i]);
        }
        for (int i = 0; i < numIsBFSButtons; ++i) {
            isBFSButtons[i].drawTo(window);
        }
        for (int i = 0; i < numIsBFSs; ++i) {
            window.draw(isBFSTexts[i]);
        }
        for (int i = 0; i < numRegions; ++i) {
            window.draw(regionTexts[i]);
        }

        searchButton.drawTo(window);
        window.draw(dateSearchText);
        window.draw(dateText);
        window.draw(searchText);
        window.draw(offsetText);
        window.draw(isOrganicText);
        if (graphCollection.find(userInputKey) == graphCollection.end()) {
            //the key does not exist, display an INVALID text
            window.draw(invalidText);
        }
        window.draw(lastDateText);

        if (isScared) {
            if (scaredCount < 1000) {
                window.draw(scaredAvocado);
                scaredCount += 1;
            }
            else {
                scaredCount = 0;
                isScared = false;
                window.draw(smilingAvocado);
            }
        }
        else {
            window.draw(smilingAvocado);
        }

        //if there is an avocado selected, look for the corresponding vertex using lastUserInputDate, and display all the info
        //in the right panel
        bool somethingSelected = false;
        for (auto iter = avocados.begin(); iter != avocados.end(); ++iter) {
            if (iter->second.isSelected()) {
                //we have found the selected avocado
                //display the information in the right panel.
                //if there is no information, say so
                if (graphCollection[lastUserInputKey].adjList.find(selectedCity) != graphCollection[lastUserInputKey].adjList.end()) {
                    //cout << graphCollection[lastUserInputKey].adjList[selectedCity][0].first.name << " ";
                    //cout << graphCollection[lastUserInputKey].adjList[selectedCity][0].first.price << " ";
                    //cout << graphCollection[lastUserInputKey].adjList[selectedCity][0].first.volume << endl;
                    //instead of coutting, put some text
                    selectedCityText.setString(graphCollection[lastUserInputKey].adjList[selectedCity][0].first.name);

                    string tempPrice = to_string(graphCollection[lastUserInputKey].adjList[selectedCity][0].first.price);
                    string tempPriceShort = tempPrice.substr(0, 4);
                    selectedCityPriceText.setString("$" + tempPriceShort);

                    string tempVolume = to_string(graphCollection[lastUserInputKey].adjList[selectedCity][0].first.volume);
                    int volumeDigitCount = 0;
                    for (int i = 0; i < tempVolume.size(); ++i) {
                        volumeDigitCount++;
                        if (tempVolume[i] == '.') {
                            break;
                        }
                    }
                    string tempVolumeShort = "";
                    if (volumeDigitCount != 0) {
                        if (volumeDigitCount + 2 < tempVolume.size()) {
                            tempVolumeShort = tempVolume.substr(0, volumeDigitCount + 2);
                        }
                    }
                    if (!tempVolumeShort.empty()) {
                        selectedCityVolumeText.setString(tempVolumeShort + " Units");
                    }
                    else {
                        selectedCityVolumeText.setString(tempVolume + " Units");
                    }
                }
                else {
                    //city does not have any information for this date
                    cout << iter->second.getName() << " " << "No info for this date" << endl;
                    //same here
                    selectedCityText.setString(iter->second.getName());
                    selectedCityPriceText.setString("No Data");
                    selectedCityVolumeText.setString("No Data");
                }
                somethingSelected = true;
            }
        }
        if (!somethingSelected) {
            selectedCityText.setString("");
            selectedCityPriceText.setString("");
            selectedCityVolumeText.setString("");
        }
        window.draw(selectedCityText);
        window.draw(selectedCityPriceText);
        window.draw(selectedCityVolumeText);
        window.draw(regionText);
        window.draw(priceText);
        window.draw(volumeText);

        //display the new window at the end
        window.display();
    }

    return 0;
}

//TODO
//have to store the date that was input by clicking the change button
//have to draw the edges once we have the unordered set of graphs
