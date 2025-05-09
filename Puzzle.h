#include <bits/stdc++.h>
using namespace std;

enum class Heuristics
{
    UNIFORM_COST_SEARCH, // 1
    MISPLACED_TILE,      // 2
    MANHATTAN            // 3
};

Heuristics get_heuristic(int heuristic)
{
    if (heuristic == 2)
        return Heuristics::MISPLACED_TILE;
    if (heuristic == 3)
        return Heuristics::MANHATTAN;
    return Heuristics::UNIFORM_COST_SEARCH;
}

// This shoukd be dynamic as per the dimensions. Kept constant to make 8 puzzle faster.
vector<vector<int>> get_goal_state()
{
    return {{1, 2, 3}, {4, 5, 6}, {7, 8, 0}};
}

string convert(vector<vector<int>> &board)
{
    int dim = board.size();
    string res = "";
    for (int i = 0; i < dim; i++)
    {
        for (int j = 0; j < dim; j++)
        {
            res += (char)board[i][j];
        }
    }
    return res;
}

int get_huerestic_cost(vector<vector<int>> &board, Heuristics heuristic)
{
    int heuristic_cost = 0;
    int dim = board.size();
    if (heuristic == Heuristics::MISPLACED_TILE)
    {
        for (int i = 0; i < dim; i++)
        {
            for (int j = 0; j < dim; j++)
            {
                int goal_value = dim * i + j + 1;
                if (goal_value == dim * dim)
                    goal_value = 0;
                if (board[i][j] != goal_value)
                    heuristic_cost++;
            }
        }
    }
    else if (heuristic == Heuristics::MANHATTAN)
    {

        for (int i = 0; i < dim; i++)
        {
            for (int j = 0; j < dim; j++)
            {
                if (board[i][j] == 0)
                    continue;
                int val = board[i][j] - 1;
                int gx = val / dim;
                int gy = val % dim;
                heuristic_cost += abs(gx - i) + abs(gy - j);
            }
        }
    }
    return heuristic_cost;
}

array<int, 2> get_empty_space_coordinated(vector<vector<int>> &puzzle)
{
    int dim = puzzle.size();
    for (int x = 0; x < dim; x++)
    {
        for (int y = 0; y < dim; y++)
        {
            if (puzzle[x][y] == 0)
                return {x, y};
        }
    }
    return {-1, -1}; // Error: Blank not found, wrong puzzle input.
}

class Puzzle
{
public:
    int dimension;
    int cost;
    Heuristics heuristic;
    vector<vector<int>> current_state;
    int empty_x;
    int empty_y;
    int depth;

    Puzzle(vector<vector<int>> &current_state, int heuristic, int depth)
    {
        this->dimension = current_state.size();
        this->current_state = current_state;
        this->heuristic = get_heuristic(heuristic);
        array<int, 2> blank_coordinate = get_empty_space_coordinated(current_state);
        this->empty_x = blank_coordinate[0];
        this->empty_y = blank_coordinate[1];
        this->depth = depth;
        this->cost = depth + get_huerestic_cost(current_state, this->heuristic);
    }
};

vector<vector<vector<int>>> expand(Puzzle *node, unordered_map<string, int> visited)
{
    vector<vector<vector<int>>> valid_states;
    int dirx[4] = {1, 0, -1, 0};
    int diry[4] = {0, 1, 0, -1};
    int dim = node->dimension;
    for (int dir = 0; dir < 4; dir++)
    {
        int old_x = node->empty_x;
        int old_y = node->empty_y;

        int new_x = old_x + dirx[dir];
        int new_y = old_y + diry[dir];

        if (new_x >= 0 && new_y >= 0 && new_x < dim && new_y < dim)
        {
            vector<vector<int>> new_state = node->current_state;
            swap(new_state[new_x][new_y], new_state[old_x][old_y]);
            if (!visited[convert(new_state)]) // Has not been visited yet.
            {
                valid_states.push_back(new_state);
            }
        }
    }
    return valid_states;
}

Puzzle *make_node(vector<vector<int>> &state, int heuristic, int depth)
{
    return new Puzzle(state, heuristic, depth);
}

void print_state(Puzzle *board)
{
    int dim = board->current_state.size();
    cout << "For this state g(n) is " << board->depth << " and h(n) is " << board->cost - board->depth << " and f(n) is " << board->cost << endl;
    cout << "The current state is: " << endl;
    for (int i = 0; i < dim; i++)
    {
        for (int j = 0; j < dim; j++)
        {
            cout << board->current_state[i][j] << " ";
        }
        cout << endl;
    }
    cout << endl;
}