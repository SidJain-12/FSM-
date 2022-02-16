#include <vector>
#include <set>
#include <iostream>
#include <ctype.h>
using namespace std;

typedef struct transition
{
    int start;
    char symbol;
    int end;
} transition;

void print_array(vector<int> arr)
{
    for (int i = 0; i < arr.size(); i++)
        cout << arr[i] << " ";
    cout << endl;
    cout << endl;
}

void bfs(int node, vector<transition> &step, int k, int n, vector<int> &is_visited, vector<vector<int> > &adj)
{
    if (is_visited[node] == 1)
        return;

    is_visited[node] = 1;

    // visit all its neighbpur which havent been visited yet
    for (int i = 0; i < adj[node].size(); i++)
    {
        if (adj[node][i] != -1)
        {
            bfs(adj[node][i], step, k, n, is_visited, adj);
        }
    }
}


int main()
{
    int n, k, a;
    cin >> n >> k >> a;

    vector<int> accepted_state(a);

    // tells if a state is accepted state or not
    vector<int> is_accepted_state(n, -1);

    for (int i = 0; i < a; i++)
    {
        cin >> accepted_state[i];
        is_accepted_state[accepted_state[i]] = 1;
    }

    // stores the neighbours of a node
    vector<vector<int> > adj(n);

    // vector of vector that stores what node transition to if that particulr symobol is given
    vector<vector<int> > transitions_to(n, vector<int>(26, -1));

    vector<transition> step(k);
    for (int i = 0; i < k; i++)
    {
        cin >> step[i].start >> step[i].symbol >> step[i].end;
        adj[step[i].start].push_back(step[i].end);

        transitions_to[step[i].start][step[i].symbol - 'a'] = step[i].end;
    }

    // if -1 after , that means that node is not reachable
    vector<int> is_visited(n, -1);

    int start_node = 0;
    // perform start node from 0
    bfs(start_node, step, k, n, is_visited, adj);


    // create two state
    // one set include all non accepted state and other set include all accepted state

    // this stores the array of node
    // in each node , it also contain info to which set it belongs
    vector<int> set_no(n, -1);

    for (int i = 0; i < n; i++)
    {
        if (is_visited[i] == 1)
        {
            if (is_accepted_state[i] == 1)
            {
                set_no[i] = 1;
            }
            else
            {
                set_no[i] = 0;
            }
        }
    }

    int no_of_set = 2;

    // now loop through checking their equivalence till no set becomes constant

    while (1)
    {
        int prev_no_of_set = no_of_set;

        vector<int> old_set_no = set_no;

        // selecting which set to check
        for (int set_id = 0; set_id < no_of_set; set_id++)
        {
            vector<int> set1;
            for (int i = 0; i < n; i++)
            {
                if (is_visited[i] && old_set_no[i] == set_id)
                {
                    set1.push_back(i);
                }
            }

            // set1 now contains all those sets which have set_no same as set_id

            vector<vector<int> > check_vect;
            vector<int> check_set_no;

            if (set1.size() > 0)
            {
                // store the transition array of set1[0] into it
                check_vect.push_back(transitions_to[set1[0]]);
                check_set_no.push_back(set_no[set1[0]]);

                for (int i = 0; i < set1.size(); i++)
                {
                    int flag = 0;
                    int node = set1[i];

                    // check to which set it belong
                    for (int j = 0; j < check_vect.size(); j++)
                    {
                        // check if both vector transition_to same group no
                        int is_equal = 1;

                        // if group number is same
                        for (int k = 0; k < 26; k++)
                        {
                            if ((old_set_no[transitions_to[node][k]] != old_set_no[check_vect[j][k]]))
                            {
                                is_equal = 0;
                                break;
                            }
                        }

                        if (is_equal == 1)
                        {
                            flag = 1;
                            set_no[node] = check_set_no[j];
                            break;
                        }
                    }

                    // it does not belong to any set
                    // add it to a new set
                    if (flag == 0)
                    {
                        set_no[node] = no_of_set++;
                        check_vect.push_back(transitions_to[node]);
                        check_set_no.push_back(set_no[node]);
                    }
                }
            }
        }

        if (no_of_set == prev_no_of_set)
            break;
    }


    // print output
    int n_final = 0, k_final = 0, a_final = 0;
    set<int> accepted_final;

    n_final = no_of_set;

    for (int i = 0; i < n; i++)
    {
        if ((is_visited[i] == 1) && (is_accepted_state[i] == 1))
        {
            accepted_final.insert(set_no[i]);
        }
    }

    a_final = accepted_final.size();

    // this is to ensure that we dont add two same transition

    int check[no_of_set][no_of_set][26];

    for (int i = 0; i < no_of_set; i++)
    {
        for (int j = 0; j < no_of_set; j++)
        {
            for (int k = 0; k < 26; k++)
                check[i][j][k] = 0;
        }
    }


    // find no of transition
    for (int i = 0; i < n; i++)
    {
        if (is_visited[i])
        {
            for (int j = 0; j < 26; j++)
            {
                if (transitions_to[i][j] != -1)
                {
                    int start = set_no[i];
                    int end = set_no[transitions_to[i][j]];
                    int symbol = j;

                    if(start == -1 || end == -1)
                    continue;

                    if (check[start][end][symbol] == 0)
                    {
                        k_final++;
                    }

                    check[start][end][symbol]++;
                }
            }
        }
    }

    cout << "Minimized DFA" << endl;
    cout << n_final << " " << k_final << " " << a_final << endl;

    // print accepted state
    for (set<int>::iterator it = accepted_final.begin(); it != accepted_final.end(); it++)
    {
        int x = *it;
        cout << *it << " ";
    }

    cout << endl;

    // print transitions
    for (int i = 0; i < n; i++)
    {
        if (is_visited[i])
        {
            for (int j = 0; j < 26; j++)
            {
                if (transitions_to[i][j] != -1)
                {
                    int start = set_no[i];
                    char symbol = 'a' + j;
                    int end = set_no[transitions_to[i][j]];

                    if (check[start][end][symbol - 'a'] == 1)
                        cout << start << " " << symbol << " " << end << endl;

                    check[start][end][symbol - 'a']--;
                }
            }
        }
    }
}