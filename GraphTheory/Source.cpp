///
/// Graph Theory: Done By Daniel Gil & Shelly Jurbinsky
///

#include <queue>
#include <string>
#include <ctime>
#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

vector<string> color;
vector<int> distanceFromVertex;
vector<int> parent;

//Checks whether we have an isolated vertex or not.
int Is_Isolated(const vector<vector<int> >& graphRepresentation)
{
    for (int i = 0; i < graphRepresentation.size(); i++)
    {
        if (graphRepresentation[i].empty())
        {
            return 1;
        }
    }

    return 0;
}

//Generates a random number according to arr which saved the (mone, mehane *values*) 
bool RandomNumber(string probability, int* arr, int n)
{
    int randomNumber;
    int numerator = arr[0];
    int denominator = arr[1];

    if (denominator != 0)
    {
        // generate a random between 1 to upper bound (mehane)
        randomNumber = rand() % denominator + 1;
    }
    else
    {
        //denominator was 0;
        return false;
    }

    //return true if the random less or equal to mone, otherwise it'll return false
    return randomNumber <= arr[0];
}
//computing the lower and the upper bound of the random according to the probabilty
int* CalculateNumeratorDenominator(string probabilty)
{
    int* arr{ new int[2]{0,0} };
    int* a = arr;
    int i = 0;
    string temp = probabilty;
    int probabiltyRange = 0;
    int denominator = 0;
    int numerator = 0;
    int randomNumber = 0;

    if (probabilty.length() == 1)
    {
        if (probabilty[0] == '1')
        {
            arr[0] = 1;
            arr[1] = 1;
            return a;
        }
        else
        {
            return a;
        }
    }
    else
    {
        denominator = (int)pow(10, probabilty.length() - 2);
        for (i = 2; i < probabilty.length(); i++)
        {
            if (probabilty[i] != 0)
            {
                break;
            }
        }

        temp = probabilty.substr(i, probabilty.length());
        numerator = stoi(temp);
    }
    arr[0] = numerator;
    arr[1] = denominator;

    return arr;
}
///
///build_random_graph builds the graph according to the probability and the number of vertices
///and taking in consideration the numerator and denominator
///
vector<vector<int> > build_random_graph(int numberOfVertices, string probability)
{
    vector<vector<int> > graphRepresentation;

    int* numeratorDenominatorArray = CalculateNumeratorDenominator(probability);
    graphRepresentation.assign(numberOfVertices, vector<int>());
    for (int i = 0; i < numberOfVertices; i++)
    {
        for (int j = i + 1; j < numberOfVertices; j++)
        {
            if (RandomNumber(probability, numeratorDenominatorArray, 2))
            {
                graphRepresentation[i].push_back(j + 1);
                graphRepresentation[j].push_back(i + 1);
            }
        }
    }

    delete[] numeratorDenominatorArray;

    return graphRepresentation;
}

// Time complexity O(V + E)
void bfsAlgorithm(const vector<vector<int> >& graphRepresentation, int currentVertex)
{
    // The Queue used as part of the bfs computing
    queue<int> q;
    // pushing the root into the queue
    q.push(currentVertex);
    /* Distance of root node is 0 &
     * root color is gray because it was just visited */
    distanceFromVertex[currentVertex] = 0;
    color[currentVertex] = "grey";

    /*Traversing through the queue until it's not empty */
    while (!q.empty())
    {

        int x = q.front();
        q.pop();
        /* This loop traverses all the child nodes of x */
        for (auto i = graphRepresentation[x].begin(); i != graphRepresentation[x].end(); i++)
        {
            /*Correcting the position of the right vertex , to avoid vector range exception*/
            int correctPosition = *i - 1;
            /* If the color is white then the said node
            is not traversed. */
            if (color[correctPosition] == "white")
            {
                color[correctPosition] = "grey";
                distanceFromVertex[correctPosition] = distanceFromVertex[x] + 1;
                parent[correctPosition] = x;

                /* Pushing the node inside queue
                to traverse its children. */
                q.push(correctPosition);
            }
        }
        /* Now the node x is completely traversed
        and color is changed to black. */
        color[x] = "black";
    }
}

/*Initialize all vectors (color, distanceFromVertex, parent) after we finish every bfs run */
void InitialAllVectors(int numberOfVertices)
{
    color.assign(numberOfVertices, "white");
    distanceFromVertex.assign(numberOfVertices, 0);
    parent.assign(numberOfVertices, -1);
}
// Checks if the graph is connected , returns 1 if connected otherwise 0
int connectivity(const vector<vector<int> >& graphRepresentation)
{
    int numberOfZeros = 0;

    InitialAllVectors(graphRepresentation.size());
    bfsAlgorithm(graphRepresentation, 0);
    for (int i = 0; i < distanceFromVertex.size(); i++)
    {
        if (distanceFromVertex[i] == 0)
        {
            numberOfZeros++;
        }
    }

    if (numberOfZeros > 1)//NO OF NULLS
    {
        return 0;
    }

    return 1;
}

///
///After every finish of bfs run we find the current diam through the vector
///saved the distances from this vertex , time complexity - (n - 1)
///
int currentMaxDiameter(int sizeOfTheGraph)
{
    int diam = 0;
    for (int i = 0; i < sizeOfTheGraph; i++)
    {
        if (distanceFromVertex[i] > diam)
        {
            diam = distanceFromVertex[i];
        }
    }

    return diam;
}

/// This function does bfs for each vertex which takes V * O(V + E) and then computes the diameter and returns zero if not connected
/// otherwise return the diameter
int diameter(const vector<vector<int> >& graphRepresentation)
{
    int checkIsGraphConnected = 0;

    checkIsGraphConnected = connectivity(graphRepresentation);
    if (checkIsGraphConnected)
    {
        int maxDiameter = 0;
        int tempMaxDiameter = currentMaxDiameter(graphRepresentation.size());

        if (tempMaxDiameter > maxDiameter)
        {
            maxDiameter = tempMaxDiameter;
        }

        for (int i = 1; i < graphRepresentation.size(); i++)
        {
            //Initial all global vectors (color, distanceFromVertex, parent) before the bfs run 
            InitialAllVectors(graphRepresentation.size());
            bfsAlgorithm(graphRepresentation, i);
            tempMaxDiameter = currentMaxDiameter(graphRepresentation.size());
            if (tempMaxDiameter > maxDiameter)
            {
                maxDiameter = tempMaxDiameter;
            }
        }

        return maxDiameter;
    }

    return 0;
}

///
///printing the neighbors of each vertex
///
void print(const vector<vector<int> >& graphRepresentation)
{
    for (int i = 0; i < graphRepresentation.size(); i++)
    {
        for (int j = 0; j < graphRepresentation[i].size(); j++)
        {
            cout << graphRepresentation[i][j] << " ";
        }
        cout << endl;
    }
}

void startingPoint()
{
    string firstArrayForFirstThreshold[10] = { "0.001", "0.0036", "0.0047" ,"0.0052", "0.0058", "0.0075", "0.0085", "0.0095", "0.015", "0.025" };
    string secondArrayForSecondThreshold[10] = { "0.065", "0.075", "0.085" , "0.102", "0.112" , "0.12" , "0.123" , "0.129" , "0.135" , "0.140" };
    string thirdArrayForThirdThreshold[10] = { "0.001", "0.0034", "0.0055" ,"0.0060", "0.0068", "0.0072", "0.0082", "0.0085", "0.012", "0.03" };

    float Threshold1 = log(1000) / 1000; //0.00690776
    float Threshold2 = sqrt(2 * log(1000) / 1000); //0.117539
    float Threshold3 = log(1000) / 1000; //0.00690776 
    
    srand(time(NULL));
    int userInput;
    int numberOfVertices = 0;
    int isolatedVertex = 0;
    int diameterOfTheGraph = 0;
    double counterForThreshold1 = 0;
    int counterForThreshold2 = 0;
    int counterForThreshold3 = 0;
    int counterForDiameterUpTo2 = 0;
    double numberOfGraphsToRunOn = 0;
    double result;
    bool checkWhetherTheGraphIsConnected = false;
    string probabilityThatEdgeExists;
    vector<vector<int> > graphRepresentation;

    ///defining the csv files
    ofstream ourFileForThreshold1;
    ofstream ourFileForThreshold2;
    ofstream ourFileForThreshold3;

    cout << "Please enter the number of vertices you'd like" << endl;
    cin >> numberOfVertices;
    cout << "Please enter the number of graphs you'd like to run on" << endl;
    cin >> numberOfGraphsToRunOn;
    cout << "Please choose which test would you like to run: \n1 - Connectivity\n2 - Diameter\n3 - Is_Isolated?" << endl;
    cin >> userInput;

    int startTime = clock();

    try
    {
        if (userInput == 1)
        {
            ourFileForThreshold1.open("Threshold1.csv");
            // check if the file opened
            if (ourFileForThreshold1.is_open())
            {
                ourFileForThreshold1 << "P" << "," << "Probability For Connectivity" << endl;
                for (int i = 0; i < 10; i++)
                {
                    counterForThreshold1 = 0;
                    for (int j = 0; j < numberOfGraphsToRunOn; j++)
                    {
                        graphRepresentation = build_random_graph(numberOfVertices, firstArrayForFirstThreshold[i]);
                        //print(graphRepresentation);
                        if (connectivity(graphRepresentation))
                        {
                            counterForThreshold1++;
                        }
                    }
                    result = counterForThreshold1 / numberOfGraphsToRunOn;
                    ourFileForThreshold1 << firstArrayForFirstThreshold[i] << "," << result << endl;
                }
                //closing the file
                ourFileForThreshold1.close();
                cout << "YOUR FILE HAS BEEN CREATED !" << endl;
            }
            else
            {
                throw "couldn't open your file";
            }
        }
        else if (userInput == 2)
        {
            ourFileForThreshold2.open("Threshold2.csv");
            // check if the file opened
            if (ourFileForThreshold2.is_open())
            {
                ourFileForThreshold2 << "P" << "," << "Diameter <= 2" << endl;
                for (int i = 0; i < 10; i++)
                {
                    counterForDiameterUpTo2 = 0;

                    for (int j = 0; j < numberOfGraphsToRunOn; j++)
                    {
                        graphRepresentation = build_random_graph(numberOfVertices, secondArrayForSecondThreshold[i]);
                        diameterOfTheGraph = diameter(graphRepresentation);
                        if (diameterOfTheGraph > 0 && diameterOfTheGraph <= 2)
                        {
                            counterForDiameterUpTo2++;
                        }
                    }
                    ourFileForThreshold2 << secondArrayForSecondThreshold[i] << "," << ((double)counterForDiameterUpTo2 / (double)numberOfGraphsToRunOn) << endl;
                }
                //closing the file
                ourFileForThreshold2.close();
                cout << "YOUR FILE HAS BEEN CREATED !" << endl;
            }

            else
            {
                throw "couldn't open your file";
            }
        }
        else if (userInput == 3)
        {
            ourFileForThreshold3.open("Threshold3.csv");
            // check if the file opened
            if (ourFileForThreshold3.is_open())
            {
                ourFileForThreshold3 << "P" << "," << "Probability For An Isolated Vertex" << endl;
                for (int i = 0; i < 10; i++)
                {
                    counterForThreshold3 = 0;
                    for (int j = 0; j < numberOfGraphsToRunOn; j++)
                    {
                        graphRepresentation = build_random_graph(numberOfVertices, thirdArrayForThirdThreshold[i]);
                        //print(graphRepresentation);
                        if (Is_Isolated(graphRepresentation))
                        {
                            counterForThreshold3++;
                        }
                    }
                    result = counterForThreshold3 / numberOfGraphsToRunOn;
                    ourFileForThreshold3 << thirdArrayForThirdThreshold[i] << "," << result << endl;
                }
                //closing the file
                ourFileForThreshold3.close();
                cout << "YOUR FILE HAS BEEN CREATED !" << endl;
            }
            else
            {
                throw "couldn't open your file";
            }
        }
        else
        {
            throw "Wrong input please restart the program";
        }
    }
    catch (string error)
    {
        cout << error << endl;
    }
}

int main()
{
    startingPoint();   
}
