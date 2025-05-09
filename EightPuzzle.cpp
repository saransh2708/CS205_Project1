#include <bits/stdc++.h>
#include "Puzzle.h"
using namespace std;

int expanded_nodes;
struct ComparePuzzle
{
    bool operator()(Puzzle *p1, Puzzle *p2)
    {
        return p1->cost > p2->cost; // PQ in C++ works in opposite way. But this will sort basis of low cost.
    }
};

Puzzle *general_search(vector<vector<int>> &problem, int heuristic)
{

    Puzzle *initial_node = make_node(problem, heuristic, 0);

    priority_queue<Puzzle *, vector<Puzzle *>, ComparePuzzle> queue;
    queue.push(initial_node);

    unordered_map<string, int> visited;
    visited[convert(problem)] = 1;

    while (!queue.empty())
    {
        Puzzle *node = queue.top();
        int current_depth = node->depth;
        queue.pop();
        print_state(node);
        expanded_nodes++;

        if (node->current_state == get_goal_state())
            return node;

        vector<vector<vector<int>>> valid_states = expand(node, visited);

        for (auto valid_state : valid_states)
        {
            Puzzle *valid_node = make_node(valid_state, heuristic, current_depth + 1);
            queue.push(valid_node);
            visited[convert(valid_state)] = 1;
        }
    }
    return NULL;
}
int main()
{
    int dimension = 3;
    int choice = 1;
    cout << "Hello World! Type 1 to solve default 8 - Puzzle Solver or 2 to give your own input\n";
    cin >> choice;
    vector<vector<int>> initial_state;
    int heur = 1;
    if (choice == 1)
    {
        initial_state = {{1, 2, 0}, {4, 5, 3}, {7, 8, 6}};
    }
    else
    {
        cout << "Please Enter numbers of the puzzle, where 0 means the blank\n";

        for (int i = 0; i < dimension; i++)
        {
            vector<int> row;
            cout << "Please enter the " << i + 1 << " row of the puzzle\n";
            for (int j = 0; j < dimension; j++)
            {
                int num;
                cin >> num;
                row.push_back(num);
            }
            initial_state.push_back(row);
        }
    }
    cout << "Please specify which algorithm to solve it by: \n1. Uniform Cost Search\n2. Misplaced Tile Heuristics\n3. Manhattan Distance Heuristics\n";
    cin >> heur;
    auto start = chrono::high_resolution_clock::now();
    Puzzle *result = general_search(initial_state, heur);
    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);
    cout << "Time taken to solve the puzzle is: " << duration.count() << " milliseconds\n";
    cout << "Depth of the solution: " << result->depth << endl;
    cout << "Total number of expanded nodes: " << expanded_nodes << endl;
    cout << "Final state of the puzzle is:\n";
    for (int i = 0; i < dimension; i++)
    {

        for (int j = 0; j < dimension; j++)
        {
            cout << result->current_state[i][j] << " ";
        }
        cout << endl;
    }
}