#include <iostream>
#include <vector>
#include <map>
#include <iomanip>
#include <string>
#include <regex>
#include <thread>
#include <chrono>
using namespace std;

// regex that match expresion numbers with or without sign and decimal point
regex rgx("([+-]?[0-9]*\\.?[0-9]*)(x+\\d)");

// regex that match expresion numbers with or without sign and decimal point after <=
regex rgx2("<=\\s*([+-]?\\d*\\.?\\d+)");

vector<string> first_row, first_col;

// Template of all functions
template <typename T>
void print_table(vector<vector<T>> table);

void elementary_operation(vector<vector<double>> &table, int c, int f);

int index_of_pivot_column(vector<vector<double>> &table);

int index_of_pivot_row(vector<vector<double>> &table);

void convert_to_one(vector<vector<double>> &table);

bool all_non_negative(vector<vector<double>> &table);

void simplex_maximize(vector<vector<double>> &table);

// Function that print table
template <typename T>
void print_table(vector<vector<T>> table)
{
    std::cout << showpos;
    std::cout << fixed << setprecision(8);
    cout << "\t";
    for (auto e : first_row)
    {
        std::cout << "\t";
        std::cout << e;
        std::cout << "\t";
    }
    std::cout << "\n";
    for (int i = 0; i < table.size(); i++)
    {
        cout << first_col[i] << "\t";
        for (int j = 0; j < table[i].size(); j++)
        {

            std::cout << table[i][j] << "\t";
        }
        std::cout << "\n";
    }
    for (int i = 0; i < 100; i++)
        std::cout << "-";
    std::cout << "\n";
}

// Function that make elementary operation
void elementary_operation(vector<vector<double>> &table, int c, int f)
{
    double element = table[f][c];
    for (int i = 0; i < table.size(); i++)
    {
        if (i != f)
        {
            double r = -table[i][c] / element;
            for (int j = 0; j < table[0].size(); j++)
            {
                table[i][j] += r * table[f][j];
            }
        }
    }
}

// Function that return index of pivot column
int index_of_pivot_column(vector<vector<double>> &table)
{
    int ind = -1;
    int fil = table.size();
    double mn = 1;
    for (int i = 0; i < table[0].size(); i++)
    {
        if (mn > table[fil - 1][i])
        {
            ind = i;
            mn = table[fil - 1][i];
        }
    }
    return ind;
}

// Function that return index of pivot row
int index_of_pivot_row(vector<vector<double>> &table)
{
    int index_of_pv_col = index_of_pivot_column(table);
    if (index_of_pv_col == -1)
        return -1;
    int row = table.size();
    int col = table[0].size();
    bool exist = false;
    double r = -1;
    int answer_ind = -1;
    for (int i = 0; i < row - 1; i++)
    {
        if (table[i][index_of_pv_col] != 0 && table[i][col - 1] * table[i][index_of_pv_col] > 0)
        {
            if (!exist)
            {
                exist = true;
                r = table[i][col - 1] / table[i][index_of_pv_col];
                answer_ind = i;
            }
            else
            {
                if (r > table[i][col - 1] / table[i][index_of_pv_col])
                {
                    r = table[i][index_of_pv_col];
                    answer_ind = i;
                }
            }
        }
    }
    return answer_ind;
}

// Function that convert to one the pivot element and make elementary operation
void convert_to_one(vector<vector<double>> &table)
{
    int c = index_of_pivot_column(table);
    int f = index_of_pivot_row(table);
    std::cout << "Pivot element in position [" << f << "][" << c << "] = " << table[f][c] << "\n";
    first_col[f] = first_row[c];
    if (f != -1 && c != -1)
    {
        double factor = table[f][c];
        for (int i = 0; i < table[0].size(); i++)
        {
            table[f][i] /= factor;
        }
        elementary_operation(table, c, f);
    }
}

// Function that return true if all elements of last row are non negative 
bool all_non_negative(vector<vector<double>> &table)
{
    int fil = table.size();
    int col = table[0].size();
    bool is = true;
    for (int i = 1; i < col; i++)
    {
        is = is & (table[fil - 1][i] >= 0);
    }
    return is;
}

// Function that make simplex algorithm
void simplex_maximize(vector<vector<double>> &table)
{
    cout << "Simplex process\n";
    int col = table[0].size();
    while (!all_non_negative(table))
    {
        print_table(table);
        cout << flush;
        std::this_thread::sleep_for(std::chrono::milliseconds(1500));
        try {
            convert_to_one(table);
        } catch (const std::exception& e) {
            cout << "Error: " << e.what() << "\n";
            break;
        }
    }
    print_table(table);
}


int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    // // Example 1
    // // Objective function
    string objective_function = "7x1 + 9x2";

    // Restrictions
    vector<string> restrictions = {
        "-x1 + 3x2 <= 6",
        "7x1 + x2 <= 35"
    };
    // Example 2
    // Objective function
    // string objective_function = "2x1 + x2 + 3x3";

    // // Restrictions
    // vector<string> restrictions = {
    //     "2x1 + x2 + x3 <= 4",
    //     "x1 + 2x2 + 3x3 <= 5",
    //     "2x1 + 2x2 + x3 <= 8"
    // };

    // // Example 3
    // // Objective function
    // string objective_function = "3x1 + x2 + 3x3";

    // // Restrictions
    // vector<string> restrictions = {
    //     "2x1 + x2 + x3 <= 2",
    //     "x1 + 2x2 + 3x3 <= 5",
    //     "2x1 + 2x2 + x3 <= 6"};

    // Vector of limits of restrictions
    vector<double> r_col;

    // Vector of coefficients of objective function

    vector<double> coef_objFunction;

    // Table of coefficients
    vector<vector<double>> tableCoef, tmp_tableCoef;

    // Non basic variables
    vector<string> non_basics_vars;
    map<string, double> non_basics_vars_map;
    // Basic variables
    vector<string> basics_vars;
    ;
    // Extracting limits of restrictions
    for (auto e : restrictions)
    {
        sregex_iterator current(e.begin(), e.end(), rgx2);
        sregex_iterator last;
        while (current != last)
        {
            smatch match = *current;
            string limit_str = match[1].str();
            double limit_d = stod(limit_str);
            r_col.push_back(limit_d);
            current++;
        }
    }
    // Show limits of restrictions
    std::cout << flush;
    std::this_thread::sleep_for(std::chrono::milliseconds(1500));
    std::cout << "Limits of restrictions: ";
    for (auto e : r_col)
    {
        std::cout << e << " ";
    }
    std::cout << "\n";

    // Create basics variables according to number of restrictions
    for (int i = 0; i < restrictions.size(); i++)
    {
        basics_vars.push_back("s" + to_string(i + 1));
    }

    // Show basic variables
    std::cout << flush;
    std::this_thread::sleep_for(std::chrono::milliseconds(1500));
    std::cout << "Basic vars: ";
    for (auto e : basics_vars)
    {
        std::cout << e << " ";
    }
    std::cout << "\n";

    // Extracting coefficients and variables from objective function
    sregex_iterator current_obj(objective_function.begin(), objective_function.end(), rgx);
    sregex_iterator last_obj;
    while (current_obj != last_obj)
    {
        smatch match = *current_obj;
        string coef_str = match[1].str();
        string var_str = match[2].str();
        // cout << "coef_str: " << coef_str << " var_str: " << var_str << "\n";
        if (coef_str == "-")
            coef_str = "-1";
        double coef_d = coef_str.empty() ? 1 : stod(coef_str);
        current_obj++;
        coef_objFunction.push_back(coef_d);
        non_basics_vars.push_back(var_str);
    }
    // Show basic variables
    std::cout << flush;
    std::this_thread::sleep_for(std::chrono::milliseconds(1500));
    std::cout << "Non basic vars: ";
    for (auto e : non_basics_vars)
    {
        non_basics_vars_map[e] = 0;
        std::cout << e << " ";
    }

    std::cout << "\n";

    // Add basic and non basic variables to first_row and first_col
    first_row.push_back("z");
    for (auto e : non_basics_vars)
    {
        first_row.push_back(e);
    }
    for (auto e : basics_vars)
    {
        first_row.push_back(e);
        first_col.push_back(e);
    }
    first_row.push_back("r");
    first_col.push_back("z");

    // Show first row and first column
    std::cout << flush;
    std::this_thread::sleep_for(std::chrono::milliseconds(1500));
    std::cout << "First row: ";
    for (auto e : first_row)
    {
        std::cout << e << " ";
    }
    std::cout << "\n";
    std::cout << flush;
    std::this_thread::sleep_for(std::chrono::milliseconds(1500));
    std::cout << "First column: ";
    for (auto e : first_col)
    {
        std::cout << e << " ";
        non_basics_vars_map[e] = 0;
    }

    std::cout << "\n";

    // Extracting coefficients and variables from restrictions
    for (auto e : restrictions)
    {
        vector<double> coef;
        vector<string> var;
        sregex_iterator current(e.begin(), e.end(), rgx);
        sregex_iterator last;
        while (current != last)
        {
            smatch match = *current;
            string coef_str = match[1].str();
            string var_str = match[2].str();
            if (coef_str == "-")
                coef_str = "-1";
            double coef_d = coef_str.empty() ? 1 : stod(coef_str);
            coef.push_back(coef_d);
            var.push_back(var_str);
            // non_basics_vars.push_back(var_str);
            current++;
        }
        tmp_tableCoef.push_back(coef);
    }
    // Fill tableCoef
    int rows = restrictions.size() + 1;
    int cols = 1 + non_basics_vars.size() + basics_vars.size() + 1;
    vector<double> zeros(cols, 0);
    vector<vector<double>> table(rows, zeros);
    tableCoef = table;
    // Fill z row
    tableCoef[rows - 1][0] = 1;
    for (int i = 0; i < non_basics_vars.size(); i++)
    {
        tableCoef[rows - 1][i + 1] = -coef_objFunction[i];
    }
    // Fill r_col
    for (int i = 0; i < restrictions.size(); i++)
    {
        tableCoef[i][cols - 1] = r_col[i];
    }
    // Fill restrictions
    for (int i = 0; i < restrictions.size(); i++)
    {
        for (int j = 0; j < non_basics_vars.size(); j++)
        {
            tableCoef[i][j + 1] = tmp_tableCoef[i][j];
        }
    }
    for (int i = 0; i < restrictions.size(); i++)
    {
        for (int j = 0; j < non_basics_vars.size(); j++)
        {
            tableCoef[i][j] = tableCoef[i][j];
        }
        tableCoef[i][cols - 1] = tableCoef[i][cols - 1];
    }
    for (int i = 0; i < non_basics_vars.size(); i++)
    {
        tableCoef[rows - 1][i] = tableCoef[rows - 1][i];
    }
    for (int i = 0; i < basics_vars.size(); i++)
    {
        tableCoef[i][non_basics_vars.size() + i + 1] = 1;
    }

    // Simplex algorithm
    simplex_maximize(tableCoef);
    cout << flush;
    std::this_thread::sleep_for(std::chrono::milliseconds(1500));
    // Show final table
    cout << "Final table: \n";
    print_table(tableCoef);

    // Show results
    std::cout << "Results: \n";

    for (int i = 0; i < first_col.size(); i++)
    {
        non_basics_vars_map[first_col[i]] = tableCoef[i][tableCoef[0].size() - 1];
    }

    for (auto var : non_basics_vars)
    {
        cout << var << " = " << non_basics_vars_map[var] << "\n";
    }

    cout << "z = " << tableCoef[tableCoef.size() - 1][tableCoef[0].size() - 1] << "\n";
    return 0;
}