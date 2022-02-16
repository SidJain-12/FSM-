#include <stack>
#include <iostream>
#include <fstream>
#include <ctype.h>
using namespace std;

int no_of_regex_exp = 0; // stores how many regex expression are given

// Function to check if char is alphabet or not
int is_alphabet(char x)
{
    if ((x >= 'a' && x <= 'z') || (x >= 'A' && x <= 'Z'))
    {
        return 1;
    }
    else
        return 0;
}

// Function to return precedence of operators
int prec(char c)
{
    if (c == '*')
        return 3;
    else if (c == '&')
        return 2;
    else if (c == '+')
        return 1;
    else
        return -1;
}

// infix to postfix function
string infixToPostfix(string s)
{
    string result;

    stack<char> st;

    for (int i = 0; i < s.length(); i++)
    {
        char c = s[i];

        // If the scanned character is
        // an operand, add it to output string.
        if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9'))
            result += c;

        // If the scanned character is an
        // ‘(‘, push it to the stack.
        else if (c == '(')
            st.push('(');

        // If the scanned character is an ‘)’,
        // pop and to output string from the stack
        // until an ‘(‘ is encountered.
        else if (c == ')')
        {
            while (st.top() != '(')
            {
                result += st.top();
                st.pop();
            }
            st.pop();
        }

        // If an operator is scanned
        else
        {
            while (!st.empty() && prec(s[i]) <= prec(st.top()))
            {
                result += st.top();
                st.pop();
            }
            st.push(c);
        }
    }

    // Pop all the remaining elements from the stack
    while (!st.empty())
    {
        result += st.top();
        st.pop();
    }

    return result;
}

void solve(string regex)
{
    int stack_ptr = 0;
    string stack[50];

    int state = 1;

    int len = regex.size();

    for (int i = 0; i < len; i++)
    {
        char x = regex[i];

        if (is_alphabet(x))
        {
            stack[stack_ptr] = "";
            stack[stack_ptr] += state;
            state++;
            stack[stack_ptr] += x;
            stack[stack_ptr] += state;
            state++;

            stack_ptr++;
        }
        else if (x == '&')
        {
            if (stack_ptr < 2)
            {
                printf("input data is wrong\n");
                return;
            }

            string temp;

            // store a->
            temp = stack[stack_ptr - 2];

            // add state from b to c using E
            int size1 = stack[stack_ptr - 2].size();
            int size2 = stack[stack_ptr - 1].size();

            temp += stack[stack_ptr - 2][size1 - 1];
            temp += 'E';
            temp += stack[stack_ptr - 1][size2 - 1];

            // store c->d
            temp += stack[stack_ptr - 1];

            // remove a->b and c->d from stack and add a->b ->E -> c->d
            stack[stack_ptr - 1] = "";
            stack[stack_ptr - 2] = "";

            stack[stack_ptr - 2] = temp;
            stack_ptr--;
        }
        else if (x == '+')
        {
            if (stack_ptr < 2)
            {
                printf("Input data is wrong\n");
                return;
            }

            string temp;
            // add a state to a in temp
            temp += state;
            temp += 'E';
            temp += stack[stack_ptr - 1][0];

            // add a state to b in temp
            temp += state;
            temp += 'E';
            temp += stack[stack_ptr - 2][0];

            // add already stored part (a->b) and (c->d)
            temp += stack[stack_ptr - 1];
            temp += stack[stack_ptr - 2];

            // making the f state
            state++;

            int size1 = stack[stack_ptr - 1].size();
            int size2 = stack[stack_ptr - 2].size();

            // adding transition from b and d to f
            temp += stack[stack_ptr - 1][size1 - 1];
            temp += 'E';
            temp += state;

            temp += stack[stack_ptr - 2][size2 - 1];
            temp += 'E';
            temp += state;

            // remove previous two stacks
            stack[stack_ptr - 1] = "";
            stack[stack_ptr - 2] = temp;

            state++;

            stack_ptr--;
        }
        else if (x == '*')
        {
            if (stack_ptr < 1)
            {
                printf("Error in input data\n");
                return;
            }

            string temp;

            int size1 = stack[stack_ptr - 1].size();

            // adding c -> a
            temp += state;
            temp += 'E';
            temp += stack[stack_ptr - 1][0];

            // adding a->b
            temp += stack[stack_ptr - 1];

            // adding b->
            temp += stack[stack_ptr - 1][size1 - 1];
            temp += 'E';
            temp += state;

            state++;

            // replace a->b with c->a and a->b and b->c
            stack[stack_ptr - 1] = temp;
        }
    }

    if (stack_ptr == 1)
    {
        int len = stack[stack_ptr - 1].size();

        // change name to match start state = 0
        int new_name = 1;
        int old_name = stack[stack_ptr - 1][0];

        int no_of_state = INT_MIN;
        int no_of_accepted_state = 1;
        int no_of_transition = 0;

        for (int i = 0; i < stack[0].size(); i++)
        {
            if (stack[0][i] == old_name)
            {
                stack[0][i] = new_name;
            }
            else if (stack[0][i] == new_name)
            {
                stack[0][i] = old_name;
            }
        }

        int j;
        for (j = 0; j < len; j += 3)
        {
            no_of_state = max(no_of_state, (int)stack[stack_ptr - 1][j]);
            no_of_state = max(no_of_state, (int)stack[stack_ptr - 1][j + 2]);
            no_of_transition++;
        }

        cout << "n=" << no_of_state << ", "
             << "k=" << no_of_transition << ", "
             << "a=" << no_of_accepted_state << endl;

        printf("Start State:%d\nAccept State:%d\n", stack[stack_ptr - 1][0] - 1, stack[stack_ptr - 1][j - 1] - 1);

        for (j = 0; j < len; j += 3)
        {
            printf("(q%d, %c) --> q%d\n", stack[stack_ptr - 1][j] - 1, stack[stack_ptr - 1][j + 1], stack[stack_ptr - 1][j + 2] - 1);
        }
    }
    else
    {
        printf("wrong data\n");
        return;
    }
}

int main(int argc, char **argv)
{
    // store the name of input and output file
    char input_file[100], output_file[100];

    strcpy(input_file, argv[1]);
    strcpy(output_file, argv[2]);

    string regex_expression[100]; // each line stores the regex expression from the file

    // open the file
    fstream read_file, write_file;
    read_file.open(input_file, ios::in);
    //    write_file.open(output_file , ios::out);
    freopen(output_file, "w", stdout);

    // read data from the file into strings
    if (read_file.is_open())
    {
        while (getline(read_file, regex_expression[no_of_regex_exp]))
            no_of_regex_exp++;
    }
    else
    {
        printf("Error\n");
        exit(EXIT_FAILURE);
    }

    // add the concat sign between two consecutive alphabets
    for (int i = 0; i < no_of_regex_exp; i++)
    {
        string temp;
        int len = regex_expression[i].size();

        for (int j = 0; j < len; j++)
        {
            if (j == 0)
            {
                temp += regex_expression[i][j];
                continue;
            }
            else
            {
                char a = regex_expression[i][j - 1];
                char b = regex_expression[i][j];

                if ((isalpha(a) || a == '*' || a == ')') && (isalpha(b) || b == '('))
                    temp += '&';
                temp += b;
            }
        }

        regex_expression[i] = temp;
    }

    // solving and writting the output to a file
    for (int i = 0; i < no_of_regex_exp; i++)
    {
        // first convert the regex to postfic

        string regex = infixToPostfix(regex_expression[i]);

        // pass it to solver function
        solve(regex);
    }

    fclose(stdout);
}