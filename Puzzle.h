#include <bits/stdc++.h>
using namespace std;

enum class Heurestics
{
    UNIFORM_COST_SEARCH, // 0
    MISPLACED_TILE,      // 1
    MANHATTAN            // 2
};

Heurestics get_heurestic(int heurestic)
{
    if (heurestic == 1)
        return Heurestics::MISPLACED_TILE;
    if (heurestic == 2)
        return Heurestics::MANHATTAN;
    return Heurestics::UNIFORM_COST_SEARCH;
}

vector<vector<int>> get_goal_state(int dimension)
{
    vector<vector<int>> goal_state;
    int curr = 1;
    for (int i = 0; i < dimension; i++)
    {
        vector<int> row;
        for (int j = 0; j < dimension; j++)
        {
            row.push_back(curr);
            curr++;
        }
        goal_state.push_back(row);
    }
    goal_state[dimension - 1][dimension - 1] = 0; // Make last element 0
    return goal_state;
}

int get_huerestic_cost(vector<vector<int>> board, Heurestics heurestic)
{
    int heurestic_cost = 0;
    int dim = board.size();
    vector<vector<int>> goal_state = get_goal_state(dim);
    if (heurestic == Heurestics::UNIFORM_COST_SEARCH)
    {
        heurestic_cost = 0;
    }
    else if (heurestic == Heurestics::MISPLACED_TILE)
    {
        for (int i = 0; i < dim; i++)
        {
            for (int j = 0; j < dim; j++)
            {
                if (board[i][j] != goal_state[i][j])
                    heurestic_cost++;
            }
        }
    }
    else if (heurestic == Heurestics::MANHATTAN)
    {
        // TODO: Manhattan
        // for(int i=0;i<board->dimension;i++){
        //     for(int j=0;j<board->dimension;j++){

        //     }
        // }
    }
    return heurestic_cost;
}

array<int, 2> get_empty_space_coordinated(vector<vector<int>> puzzle)
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
    vector<vector<int>> goal_state;
    int empty_x;
    int empty_y;
    int depth;

    // Puzzle(int dimension, int heurestic)
    // {
    //     this->dimension = dimension;
    //     this->heurestic = Heurestics::UNIFORM_COST_SEARCH;
    //     if (heurestic == 1)
    //         this->heurestic = Heurestics::MISPLACED_TILE;
    //     else if (heurestic == 2)
    //         this->heurestic = Heurestics::MANHATTAN;
    //     goal_state = get_goal_state(dimension);

    //     this->cost = 0; // TODO: Call get_heurestic
    // }
    Puzzle(vector<vector<int>> current_state, int heurestic, int depth)
    {
        this->dimension = current_state.size();
        this->current_state = current_state;
        this->heurestic = get_heurestic(heurestic);
        auto [x, y] = get_empty_space_coordinated(current_state);
        this->empty_x = x;
        this->empty_y = y;
        this->depth = depth;
        this->cost = depth + get_huerestic_cost(current_state, this->heurestic);
    }
};

vector<vector<vector<int>>> expand(Puzzle *node, set<vector<vector<int>>> visited)
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
            if (visited.find(new_state) == visited.end()) // Has not been visited yet.
            {
                valid_states.push_back(new_state);
            }
        }
    }
    return valid_states;
}

Puzzle *make_node(vector<vector<int>> state, int heurestic, int depth)
{
    return new Puzzle(state, heurestic, depth);
}