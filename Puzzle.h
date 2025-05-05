#include <bits/stdc++.h>
using namespace std;

enum class Heurestics
{
    UNIFORM_COST_SEARCH, // 1
    MISPLACED_TILE,      // 2
    MANHATTAN            // 3
};

Heurestics get_heurestic(int heurestic)
{
    if (heurestic == 2)
        return Heurestics::MISPLACED_TILE;
    if (heurestic == 3)
        return Heurestics::MANHATTAN;
    return Heurestics::UNIFORM_COST_SEARCH;
}

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

int get_huerestic_cost(vector<vector<int>> &board, Heurestics heurestic)
{
    int heurestic_cost = 0;
    int dim = board.size();
    if (heurestic == Heurestics::MISPLACED_TILE)
    {
        for (int i = 0; i < dim; i++)
        {
            for (int j = 0; j < dim; j++)
            {
                int goal_value = dim * i + j + 1;
                if (goal_value == dim * dim)
                    goal_value = 0;
                if (board[i][j] != goal_value)
                    heurestic_cost++;
            }
        }
    }
    else if (heurestic == Heurestics::MANHATTAN)
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
                heurestic_cost += abs(gx - i) + abs(gy - j);
            }
        }
    }
    return heurestic_cost;
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
    Heurestics heurestic;
    vector<vector<int>> current_state;
    int empty_x;
    int empty_y;
    int depth;

    Puzzle(vector<vector<int>> &current_state, int heurestic, int depth)
    {
        this->dimension = current_state.size();
        this->current_state = current_state;
        this->heurestic = get_heurestic(heurestic);
        array<int, 2> blank_coordinate = get_empty_space_coordinated(current_state);
        this->empty_x = blank_coordinate[0];
        this->empty_y = blank_coordinate[1];
        this->depth = depth;
        this->cost = depth + get_huerestic_cost(current_state, this->heurestic);
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

Puzzle *make_node(vector<vector<int>> &state, int heurestic, int depth)
{
    return new Puzzle(state, heurestic, depth);
}
