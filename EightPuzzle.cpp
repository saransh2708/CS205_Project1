#include <bits/stdc++.h>
#include "Puzzle.h"
using namespace std;

struct ComparePuzzle
{
    bool operator()(Puzzle *p1, Puzzle *p2)
    {
        return p1->cost > p2->cost; // PQ in C++ works in opposite way. But this will sort basis of low cost.
    }
};

Puzzle *general_search(vector<vector<int>> problem, int heurestic)
{
    Puzzle *initial_node = make_node(problem, heurestic, 0);

    priority_queue<Puzzle *, vector<Puzzle *>, ComparePuzzle> queue;
    queue.push(initial_node);

    set<vector<vector<int>>> visited;
    visited.insert(problem);

    while (!queue.empty())
    {
        Puzzle *node = queue.top();
        int current_depth = node->depth;
        queue.pop();

        if (node->current_state == get_goal_state(3))
            return node; // TODO: fix hardcoded 3

        vector<vector<vector<int>>> valid_states = expand(node, visited);

        for (auto valid_state : valid_states)
        {
            Puzzle *valid_node = make_node(valid_state, heurestic, current_depth + 1);
            queue.push(valid_node);
            visited.insert(valid_state);
        }
    }
}
int main()
{
    int dimension = 3;
    cout << "Please Enter number of dimension of the puzzle\n";
    cin >> dimension;
    cout << "Please Enter numbers of the puzzle, where 0 means the blank\n";
    vector<vector<int>> initial_state;
    for (int i = 0; i < dimension; i++)
    {
        vector<int> row;
        for (int j = 0; j < dimension; j++)
        {
            int num;
            cin >> num;
            row.push_back(num);
        }
        initial_state.push_back(row);
    }
    Puzzle *result = general_search(initial_state, 0);
    for (int i = 0; i < dimension; i++)
    {

        for (int j = 0; j < dimension; j++)
        {
            cout << result->current_state[i][j] << " ";
        }
        cout << endl;
    }
    // board->current_state = {{1, 2, 3}, {4, 5, 6}, {7, 0, 8}};
}