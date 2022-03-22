#include <algorithm>
#include <fstream>
#include <iostream>
#include <set>
#include <sstream>
#include <string>
#include <vector>

std::string ordinal(int n)
{
    std::string suffix;
    std::string arr[] = {"th", "st", "nd", "rd", "th"};
    if (11 <= (n % 100) && (n % 100) <= 13)
    {
        suffix = "th";
    }
    else
    {
        suffix = arr[std::min(n % 10, 4)];
    }
    std::string res = std::to_string(n) + suffix;
    return res;
}

int parse_DIMACS(const char *filename, std::vector<std::pair<int, int>> *edges)
{
    std::ifstream ifs;
    ifs.open(filename);
    if (!ifs)
    {
        std::cerr << "File " << filename << " cannot be opened." << std::endl;
        exit(1);
    }

    std::istream &ist = ifs;

    int num_vertices = -1;

    std::string s;
    int line_number = 0;
    while (ist && std::getline(ist, s))
    {
        ++line_number;
        if (s[0] == 'c')
        {
            // skip
        }
        else if (s[0] == 'p')
        {
            std::string st;
            std::istringstream iss(s);
            iss >> st;
            iss >> num_vertices;
            iss >> st;
        }
        else if (s[0] == 'e')
        {
            std::istringstream iss(s);
            std::string kind;
            int e1, e2;
            iss >> kind >> e1 >> e2;
            edges->push_back(std::make_pair(e1, e2));
        }
        else
        {
            std::cerr << "illegal input format in line "
                      << line_number << std::endl;
            exit(1);
        }
    }
    return num_vertices;
}

void parse_stfile(const char *filename, std::vector<int> *start_set,
                  std::vector<int> *goal_set)
{
    std::ifstream ifs;
    ifs.open(filename);
    if (!ifs)
    {
        std::cerr << "File " << filename << " cannot be opened." << std::endl;
        exit(1);
    }

    std::string s;
    while (ifs && std::getline(ifs, s))
    {
        if (s[0] == 's' || s[0] == 't')
        {
            std::vector<int> *vec = (s[0] == 's' ? start_set : goal_set);
            std::istringstream iss(s);
            std::string st;
            int bv;
            iss >> st; // skip first char
            while (iss >> bv)
            {
                vec->push_back(bv);
            }
            std::sort(vec->begin(), vec->end());
        }
    }
}

bool is_independent_set(int num_vertices, std::vector<std::pair<int, int>> *edges,
                        std::vector<int> *state)
{
    std::vector<bool> v(num_vertices + 1, false);
    for (const auto &s : *state)
    {
        v[s] = true;
    }
    for (const auto &e : *edges)
    {
        if (v[e.first] && v[e.second])
        {
            return false;
        }
    }
    return true;
}

std::string vtos(std::vector<int> *v)
{
    std::string res = "";
    for (int i = 0; i < v->size(); ++i)
    {
        res += std::to_string(v->at(i)) + " ";
    }
    return res;
}

void check_answer(const char *filename, int num_vertices,
                  std::vector<std::pair<int, int>> *edges,
                  std::vector<int> *start_set,
                  std::vector<int> *goal_set)
{
    std::ifstream ifs;
    ifs.open(filename);
    if (!ifs)
    {
        std::cerr << "File " << filename << " cannot be opened." << std::endl;
        exit(1);
    }

    std::string s;
    bool is_warn = false;
    bool yn_flag = false;
    std::vector<int> prev, now;
    std::set<std::string> ans_set;
    int count = 0;
    while (ifs && std::getline(ifs, s))
    {
        if (s[0] == 'a' && !yn_flag)
        {
            std::istringstream iss(s);
            std::string st, yn;
            iss >> st >> yn;
            // If there is no reconfiguration sequence, then the validation succeeds.
            if (yn == "NO")
            {
                std::cout << "[Code00] (Answer: NO) Validation success "
                          << "(Note: This validator cannot ensure that "
                          << "whether a correct reconfiguration sequence does not exist for the input)"
                          << std::endl;
                return;
            }
            yn_flag = true;
        }
        else if (s[0] == 'a')
        {
            std::vector<int> answer;
            std::istringstream iss(s);
            std::string st;
            int a;
            iss >> st; // skip first char
            while (iss >> a)
            {
                answer.push_back(a);
            }
            std::sort(answer.begin(), answer.end());
            std::string ans_str = vtos(&answer);
            count++;
            if (count == 1)
            {
                std::string s_str = vtos(start_set);
                if (ans_str != s_str)
                {
                    std::cerr << "ValidationError: "
                              << "[Code10] The initial state must be equal to the start state"
                              << std::endl;
                    return;
                }
            }
            // Check that each state is an independent set.
            if (!is_independent_set(num_vertices, edges, &answer))
            {
                std::cerr << "ValidationError: "
                          << "[Code12] The "
                          << ordinal(count)
                          << " state is not an independent set"
                          << std::endl;
                return;
            }
            prev = now;
            now = answer;
            if (prev.size() != 0)
            {
                std::vector<int> diff;
                std::set_difference(prev.begin(), prev.end(), now.begin(), now.end(),
                                    std::inserter(diff, diff.begin()));
                // Check that the difference between successive states is equal to one
                if (diff.size() != 1)
                {
                    std::cerr << "ValidationError: "
                              << "[Code13] Each independent set in the sequence results from "
                              << "the previous one by moving exactly one token to another node"
                              << std::endl;
                    return;
                }
            }
            // Check that the same state does not appear more than once
            if (!is_warn && ans_set.find(ans_str) != ans_set.end())
            {
                is_warn = true;
                std::cout << "Warning: The same state appears multiple times" << std::endl;
            }
            ans_set.insert(ans_str);
        }
    }

    std::string t_str = vtos(goal_set);
    std::string now_str = vtos(&now);
    // Check that the last state match the target state
    if (now_str != t_str)
    {
        std::cerr << "ValidationError: "
                  << "[Code11] The last state must be equal to the target state"
                  << std::endl;
        return;
    }

    // If it passes all the checks, then the validation succeeds.
    if (is_warn)
    {
        std::cout << "[Code02] (Answer: YES) Validation success, but there is some warning" << std::endl;
    }
    else
    {
        std::cout << "[Code01] (Answer: YES) Validation success without any warning" << std::endl;
    }
}

int main(int argc, char *argv[])
{
    std::vector<std::pair<int, int>> edges;
    int num_vertices = parse_DIMACS(argv[1], &edges);
    std::vector<int> start_set, goal_set;
    parse_stfile(argv[2], &start_set, &goal_set);
    check_answer(argv[3], num_vertices, &edges, &start_set, &goal_set);
}