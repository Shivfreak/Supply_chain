#include <iostream>
#include <malloc.h>
#include <string.h>
#include <cstring>
#include <windows.h>
#include <iomanip>
#include <cmath>
#include<bits/stdc++.h>
#include<map>
#include<list>
using namespace std;
struct MinHeapNode
{
    char data;
    unsigned freq;
    MinHeapNode *left, *right;
    MinHeapNode(char data, unsigned freq) : data(data), freq(freq), left(nullptr), right(nullptr) {}
};

// Comparator function for the priority queue
struct compare
{
    bool operator()(MinHeapNode *l, MinHeapNode *r)
    {
        return (l->freq > r->freq);
    }
};
void encode(MinHeapNode *root, string str, unordered_map<char, string> &huffmanCode)
{
    if (root == nullptr)
        return;
    
    if (!root->left && !root->right)
    {
        huffmanCode[root->data] = str;
    }
    encode(root->left, str + "0", huffmanCode);
    encode(root->right, str + "1", huffmanCode);
}

void buildHuffmanTree(const string &text, ofstream &outputFile)
{
    unordered_map<char, unsigned> freq;
    for (char ch : text)
    {
        freq[ch]++;
    }

    priority_queue<MinHeapNode *, vector<MinHeapNode *>, compare> minHeap;
    for (auto &pair : freq)
    {
        minHeap.push(new MinHeapNode(pair.first, pair.second));
    }

    while (minHeap.size() != 1)
    {
        MinHeapNode *left = minHeap.top();
        minHeap.pop();
        MinHeapNode *right = minHeap.top();
        minHeap.pop();

        unsigned sumFreq = left->freq + right->freq;
        MinHeapNode *newNode = new MinHeapNode('\0', sumFreq);
        newNode->left = left;
        newNode->right = right;
        minHeap.push(newNode);
    }

    MinHeapNode *root = minHeap.top();

    unordered_map<char, string> huffmanCode;
    encode(root, "", huffmanCode);
    outputFile << "Huffman Codes:\n";
    for (auto &pair : huffmanCode)
    {
        if (pair.first == ' ')
        {
            outputFile << "S"
                       << " : " << pair.second << '\n';
        }
        else if (pair.first == '\n')
        {
            outputFile << "N"
                       << " : " << pair.second << '\n';
        }
        else
        {
            outputFile << pair.first << " : " << pair.second << '\n';
        }
    }
    outputFile << '\n';
    string encodedText;
    for (char ch : text)
    {
        if (ch == ' ')
        {
            encodedText += huffmanCode['S'];
        }
        else if (ch == '\n')
        {
            encodedText += huffmanCode['N'];
        }
        else
        {
            encodedText += huffmanCode[ch];
        }
    }
    outputFile << "Encoded Text:\n"
               << encodedText << '\n';
}

unordered_map<string, char> readHuffmanCodesFromFile(const string &fileName)
{
    ifstream huffmanFile(fileName);
    unordered_map<string, char> huffmanCodeMap;

    if (!huffmanFile)
    {
        cerr << "Error: Could not open the Huffman code file.\n";
        return huffmanCodeMap;
    }

    string line;
    bool readCodes = false;
    while (getline(huffmanFile, line))
    {
        if (line.empty())
            break;
        if (line == "Huffman Codes:")
        {
            readCodes = true;
            continue;
        }
        if (readCodes)
        {
            string character, code;
            size_t pos = line.find(":");
            if (pos != string::npos)
            {
                character = line.substr(0, pos - 1);
                code = line.substr(pos + 2);         
                huffmanCodeMap[code] = character[0]; 
                cout << character << " " << code << endl;
            }
        }
    }

    huffmanFile.close();
    return huffmanCodeMap;
}

void compressTextFile(const vector<int> &inputVector, ofstream &outputFile, string FileName)
{
    string inputText;
    for (int num : inputVector)
    {
        inputText += to_string(num) + " ";
    }
    buildHuffmanTree(inputText, outputFile);
    cout << "Text has been compressed and saved to " << FileName << ".txt" << endl;
    outputFile.close();
}

struct Node
{
    int id;
    string name;
    Node(int i, string n) : id(i), name(n) {}
};

struct Edge
{
    int src, dest, weight;
    Edge(int s, int d, int w) : src(s), dest(d), weight(w) {}
};

class Graph
{
private:
    int V;                                  
    vector<Node> nodes;                     
    vector<vector<pair<int, int>>> adjList; 

public:
    Graph(int vertices) : V(vertices)
    {
        adjList.resize(V);
    }
    void addNode(int id, string name)
    {
        nodes.push_back(Node(id, name));
    }
    void addEdge(int src, int dest, int weight)
    {
        adjList[src].push_back(make_pair(dest, weight));
    }

    vector<int> dijkstra(int src, int dest)
    {
        vector<int> dist(V, INT_MAX);                                                      
        vector<int> parent(V, -1);                                                          
        priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq; 

        dist[src] = 0;            
        pq.push(make_pair(0, src)); 

        while (!pq.empty())
        {
            int u = pq.top().second;
            pq.pop();

            for (auto &neighbor : adjList[u])
            {
                int v = neighbor.first;
                int weight = neighbor.second;

                if (dist[u] + weight < dist[v])
                {
                    dist[v] = dist[u] + weight;
                    parent[v] = u;
                    pq.push(make_pair(dist[v], v));
                }
            }
        }
        vector<int> shortestPath;
        int current = dest;
        while (current != -1)
        {
            shortestPath.push_back(current);
            current = parent[current];
        }
        reverse(shortestPath.begin(), shortestPath.end());

        return shortestPath;
    }
    void printShortestPath(ofstream &outputFile, int src, int dest)
    {
        vector<int> shortestPath = dijkstra(src, dest);
        outputFile << "Shortest Path from " << nodes[src].name << " to " << nodes[dest].name << ":\n";
        cout << "Shortest Path from " << nodes[src].name << " to " << nodes[dest].name << ":\n";
        for (int node : shortestPath)
        {
            outputFile << nodes[node].name << " ";
            cout << nodes[node].name << " ";
        }
        outputFile << "\n\n";
        cout << "\n\n";
    }

    void printGraph(ofstream &outputFile)
    {
        outputFile << "Graph Representation:\n";
        for (int i = 0; i < V; ++i)
        {
            outputFile << nodes[i].name << " -> ";
            cout << nodes[i].name << " -> ";
            for (auto &neighbor : adjList[i])
            {
                outputFile << nodes[neighbor.first].name << "(" << neighbor.second << ") ";
                cout << nodes[neighbor.first].name << "(" << neighbor.second << ") ";
            }
            outputFile << "\n";
            cout << "\n";
        }
        outputFile << "\n";
        cout << "\n";
    }
};

void calculateProductionInventoryDeadline(const vector<int> &weights, const vector<int> &profits, const vector<int> &deadlines, ofstream &outputFile)
{
    int n = weights.size();
    vector<pair<int, pair<int, int>>> items; 

    for (int i = 0; i < n; ++i)
    {
        items.push_back({deadlines[i], {weights[i], profits[i]}});
    }

    sort(items.begin(), items.end()); 

    vector<bool> filled(n, false); 
    vector<int> filledItems(n);
    int totalProfit = 0;

    cout << "\nItems selected for production scheduling based on deadlines:\n";
    for (int i = 0; i < n; ++i)
    {
        int deadline = items[i].first;
        int weight = items[i].second.first;
        int profit = items[i].second.second;
        
        for (int j = min(n, deadline) - 1; j >= 0; --j)
        {
            if (!filled[j])
            {
                filled[j] = true;
                totalProfit += profit;
                filledItems.push_back(i);
                cout << "Item " << i << " (Weight: " << weight << ", Profit: " << profit << ", Deadline: " << deadline << ")\n";
                break;
            }
        }
    }
    cout << "Total Profit: " << totalProfit << "\n\n";
    cout << "Save data to log?(y/n)" << endl;
    char x;
    cin >> x;
    if (x == 'y')
    {
        vector<int> data;
        for (auto i : filledItems)
        {
            int id = filledItems[i];
            int deadline = items[filledItems[i]].first;
            int weight = items[filledItems[i]].second.first;
            int profit = items[filledItems[i]].second.second;
            data.push_back(id);
            
        }
        compressTextFile(filledItems, outputFile, "output");
    }
    else
    {
        cout << "Data not saved!" << endl;
    }
}

void calculateProductionInventoryCapacity(const vector<int> &weights, const vector<int> &profits, const vector<int> &deadlines, int capacity, ofstream &outputFile)
{
    int n = weights.size();
    vector<vector<int>> dp(n + 1, vector<int>(capacity + 1, 0));

    for (int i = 1; i <= n; ++i)
    {
        for (int w = 1; w <= capacity; ++w)
        {
            if (weights[i - 1] > w)
            {
                dp[i][w] = dp[i - 1][w];
            }
            else
            {
                dp[i][w] = max(dp[i - 1][w], profits[i - 1] + dp[i - 1][w - weights[i - 1]]);
            }
        }
    }

    vector<int> selectedItems;
    int remainingCapacity = capacity;

    for (int i = n; i > 0 && remainingCapacity > 0; --i)
    {
        if (dp[i][remainingCapacity] != dp[i - 1][remainingCapacity])
        {
            selectedItems.push_back(i - 1);
            remainingCapacity -= weights[i - 1];
        }
    }

    for (const auto &item : selectedItems)
    {
        cout << "Item " << item << " (Weight: " << weights[item] << ", Profit: " << profits[item] << ")\n";
    }
    cout << "Save data to log?(y/n)" << endl;
    char x;
    cin >> x;
    if (x == 'y')
    {
        vector<int> data;
        for (const auto &i : selectedItems)
        {
            int id = selectedItems[i];
            int deadline = deadlines[i];
            int weight = weights[i];
            int profit = profits[i];
            data.push_back(id);
           
        }
        compressTextFile(selectedItems, outputFile, "output");
    }
    else
    {
        cout << "Data not saved!" << endl;
    }
}
void calculateProductionInventoryDeadlineandCapacity(vector<int> &weights, vector<int> &profits, vector<int> &deadlines, ofstream &outputFile)
{
    int n = weights.size();
    vector<pair<int, pair<int, int>>> items; 

    for (int i = 0; i < n; ++i)
    {
        items.push_back({deadlines[i], {weights[i], profits[i]}});
    }

    sort(items.begin(), items.end()); 

    vector<bool> filled(n, false); 
    vector<int> filledItems;
    int totalProfit = 0;
    vector<int>weightsI;
    vector<int>ProfitsI;
    vector<int>deadlineI;
    cout << "\nItems selected for production scheduling based on deadlines:\n";
    for (int i = 0; i < n; ++i)
    {
        int deadline = items[i].first;
        int weight = items[i].second.first;
        int profit = items[i].second.second;
        
        for (int j = min(n, deadline) - 1; j >= 0; --j)
        {
            if (!filled[j])
            {
                filled[j] = true;
                totalProfit += profit;
                filledItems.push_back(i);
                weightsI.push_back(items[i].second.first);
                ProfitsI.push_back(items[i].second.second);
                deadlineI.push_back(items[i].first);
                cout << "Item " << i << " (Weight: " << weight << ", Profit: " << profit << ", Deadline: " << deadline << ")\n";
                break;
            }
        }
    }
    cout << "Total Profit: " << totalProfit << "\n\n";
    cout << "Now calculating the items that can be transported today on basis of capacity" << endl;
    int capacity;
    cout << "Enter capacity: ";
    cin >> capacity;
    calculateProductionInventoryCapacity(weightsI,ProfitsI,deadlineI,capacity,outputFile);
}

string readEncodedTextFromFile(const string &fileName)
{
    ifstream inputFile(fileName);
    string line;
    bool foundEncodedText = false;
    string encodedText;

    if (!inputFile)
    {
        cerr << "Error: Could not open the input file.\n";
        return encodedText;
    }
    while (getline(inputFile, line))
    {
        if (line.find("Encoded Text:") != string::npos)
        {
            foundEncodedText = true;
            break;
        }
    }

    if (foundEncodedText)
    {
        getline(inputFile, encodedText);
    }
    else
    {
        cerr << "Error: Encoded text not found in the input file.\n";
    }

    inputFile.close();
    return encodedText;
}
string decodeHuffmanText(const string &encodedText, const unordered_map<string, char> &huffmanMap)
{
    string decodedText;
    string currentCode;
    for (char bit : encodedText)
    {
        currentCode += bit;
        if (huffmanMap.find(currentCode) != huffmanMap.end())
        {
            decodedText += huffmanMap.at(currentCode);
            currentCode = "";
        }
    }
    return decodedText;
}

int main()
{
    Graph graph(10);
    graph.addNode(0, "SupplierA");
    graph.addNode(1, "SupplierB");
    graph.addNode(2, "SupplierC");
    graph.addNode(3, "SupplierD");
    graph.addNode(4, "SupplierE");
    graph.addNode(5, "SupplierF");
    graph.addNode(6, "SupplierG");
    graph.addNode(7, "SupplierH");
    graph.addNode(8, "SupplierI");
    graph.addNode(9, "SupplierJ");
    graph.addEdge(0, 1, 4);
    graph.addEdge(0, 2, 2);
    graph.addEdge(1, 3, 5);
    graph.addEdge(2, 1, 1);
    graph.addEdge(2, 3, 8);
    graph.addEdge(2, 4, 10);
    graph.addEdge(3, 4, 2);
    graph.addEdge(3, 5, 3);
    graph.addEdge(4, 6, 7);
    graph.addEdge(5, 7, 6);
    graph.addEdge(6, 8, 4);
    graph.addEdge(6, 9, 9);
    graph.addEdge(7, 8, 2);
    graph.addEdge(8, 9, 3);
    graph.addEdge(9, 0, 1);
    graph.addEdge(9, 1, 2);
    graph.addEdge(8, 5, 8);
    graph.addEdge(7, 3, 5);
    graph.addEdge(6, 2, 7);
    graph.addEdge(5, 1, 6);
    int option;
    cout << "Choose an option:" << endl;
    cout << "1. Calculate Shortest Paths using Dijkstra's Algorithm" << endl;
    cout << "2. Calculate Production Inventory" << endl;
    cout << "3. Display previous logs (using Huffman Decoding)" << endl;
    cout << "4. Exit" << endl;
    cin >> option;
    while (option != 4)
    {

        switch (option)
        {
        case 1:
           
            {
                int src, dest;
                cout << "Enter the source node id (0-9): ";
                cin >> src;
                cout << "Enter the destination node id (0-9): ";
                cin >> dest;

                ofstream outputFile("graph_info.txt", ios::app);
                if (!outputFile)
                {
                    cerr << "Error: Could not open the file.\n";
                    return 1;
                }
                graph.printGraph(outputFile);
                graph.printShortestPath(outputFile, src, dest);

                cout << "Graph information and shortest path have been saved to 'graph_info.txt'.\n";
                outputFile.close();
            }
            break;

        case 2:
        
            {
                vector<int> weights = {2, 3, 1, 4};
                vector<int> profits = {10, 20, 15, 30};
                vector<int> deadlines = {3, 1, 2, 4};

                cout << "Choose an option for production inventory:" << endl;
                cout << "1. Based on Deadline (Greedy Algorithm)" << endl;
                cout << "2. Based on Capacity (0/1 Knapsack Algorithm)" << endl;
                cout << "3. Based on both" << endl;

                int prodOption;
                cin >> prodOption;
                ofstream outputFile("output.txt");
                if (!outputFile)
                {
                    cerr << "Error: Could not open the file for writing.\n";
                    return 1;
                }

                switch (prodOption)
                {
                case 1:
                    calculateProductionInventoryDeadline(weights, profits, deadlines, outputFile);
                    break;
                case 2:
                {
                    int capacity;
                    cout << "Enter the capacity: ";
                    cin >> capacity;
                    calculateProductionInventoryCapacity(weights, profits, deadlines, capacity, outputFile);
                }
                break;
                case 3:
                {
                    calculateProductionInventoryDeadlineandCapacity(weights, profits, deadlines, outputFile);
                }
                break;
                default:
                    cout << "Invalid production inventory option." << endl;
                    break;
                }
                cout << "Production inventory information has been saved to 'output.txt'.\n";
                outputFile.close();
            }
            break;

        case 3:
        {
            ifstream huffmanFile("output.txt");
            if (!huffmanFile)
            {
                cerr << "Error: Could not open the output file.\n";
                return 1;
            }
            unordered_map<string, char> huffmanMap = readHuffmanCodesFromFile("output.txt");

            cout << "Huffman Codes:\n";
            for (const auto &pair : huffmanMap)
            {
                cout << "Character: " << pair.second << ", Code: " << pair.first << endl;
            }
            string encodedText = readEncodedTextFromFile("output.txt");
            cout << encodedText << endl;
            string decodedText = decodeHuffmanText(encodedText, huffmanMap);
            cout << "Decoded Text:\n"
                 << decodedText << endl;
        }
        break;
        default:
            cout << "Invalid production inventory option." << endl;
            break;
        }
        cout << "Choose an option:" << endl;
        cout << "1. Calculate Shortest Paths using Dijkstra's Algorithm" << endl;
        cout << "2. Calculate Production Inventory" << endl;
        cout << "3. Display previous logs (using Huffman Decoding)" << endl;
        cout << "4. Exit" << endl;
        cin >> option;
    }
//return 0;
}