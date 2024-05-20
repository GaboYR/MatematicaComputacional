#include <iostream>
#include <vector>
#include <map>
#include <iomanip>
#include <string>
#include <regex>
using namespace std;
#define sz(x) (int)x.size()

const double INF = double(1e9);

regex rgx("([+-]?[0-9]*\\.?[0-9]*)(x+\\d)");
regex rgx2(">=\\s*([+-]?\\d*\\.?\\d+)");

vector<string> first_row, first_col,second_col;

vector<pair<double,double>> Z,CS;
vector<double> init_col;
int counter = 0;
// Function that print table
void print_table(vector<vector<double>> &table) {
    cout << noshowpos <<"\nTabla Simplex \n";
    for (int i = 0; i < 100; i++) {
        cout << "*\n"[i == 99];
    }
    cout << showpos << fixed << setprecision(4);
    cout << "\t\t";
    for (auto e : first_row) {
        cout << "\t" << e << "\t";
    }
    cout << "\n";
    for (int i = 0; i < table.size(); i++) {
        if (init_col[i] == INF) {
            cout << "M\t  " << "  ";
        } 
        else {
            cout << noshowpos << init_col[i] << "  ";
        }
        cout << first_col[i] << "\t";
        for (int j = 0; j < table[i].size(); j++) {
            cout << noshowpos << table[i][j] << "\t";
        }
        cout << "\n";
    }
    for (int i = 0; i < 100; i++) {
        cout << "-\n"[i == 99];
    }
    cout << "\n";
    // Show Z
    cout << "\t\tZ\t";
    for (int i = 0; i < Z.size(); i++) {
        if (Z[i].second == 1) {
            cout << "\tM" << "\t";
        }
        else {
            cout << Z[i].first << "\t";
        }
    }
    cout << "\n";
    // Show CS
    cout << "\t\tCS\t";
    for (int i = 0; i < CS.size(); i++) {
        cout << CS[i].first <<  "\t";
    }
    cout << "\n";
    cout << "    \t(M)\t";
    for (int i = 0; i < CS.size(); i++) {
        cout << CS[i].second << "\t";
    }
    cout << "\n";
    for (int i = 0; i < 100; i++) {
        cout << "*\n"[i == 99];
    }
    cout << "\n";
}
// New functions for minimization
bool all_non_negative(vector<pair<double,double>> &cs) {
    for (int i = 0; i < cs.size(); i++) {
        if (cs[i].second < 0) {
            return false;
        }
        else if (cs[i].second == 0 && cs[i].first < 0) {
            return false;
        }
    }
    return true;
}
int index_of_pivot_column(vector<pair<double,double>> &cs, bool print) {
    int ind = -1;
    double compsecond = 1;
    double compfirst;
    for (int i = 0; i < cs.size(); i++) {
        // No salia xq era <= en vez de < 
        if (cs[i].second <= 0) {
            if (compsecond > cs[i].second) {
                compsecond = cs[i].second;
                compfirst = cs[i].first;
                ind = i;
            }
            else if (cs[i].second == compsecond && cs[i].first < compfirst) {
                if (compfirst > cs[i].first) {
                    compfirst = cs[i].first;
                    ind = i;
                }
            }
        }
    }
    if (!print) {
        cout  <<"Menor elemento : " << compfirst << (compsecond >= 0 ? " + " : " ") << compsecond << "M (Columna " << ind + 1 << ")\n";
    }
    return ind;
}
int index_of_pivot_row(vector<vector<double>> &table,vector<pair<double,double>> &cs, bool print) {
    int index_col = index_of_pivot_column(cs,1);
    if (index_col == -1) return -1;
    int row = table.size();
    int col = table[0].size();
    double r = -1;
    int answer_ind = -1;
    for (int i = 0; i < row; i ++) {
        if (table[i][index_col] > 0) {
            if (r == -1) {
                r = table[i][col - 1] / table[i][index_col];
                if (!print)
                cout << "Razon en la fila " << i + 1 << " : " << table[i][col - 1] << " / " << table[i][index_col] << " = " << r << "\n";
                answer_ind = i;
            }
            else {
                if (!print)
                cout << "Razon en la fila " << i + 1 << " : " << table[i][col - 1] << " / " << table[i][index_col] << " = "<< table[i][col - 1] / table[i][index_col] << "\n";
                if (r > table[i][col - 1] / table[i][index_col]) {
                    r = table[i][col - 1] / table[i][index_col];
                    answer_ind = i;
                }
            }
            
        }
    }
    if (!print) {
        cout << "Menor razon =  " << r << " (Fila " << answer_ind + 1 << ")\n";
    }
    return answer_ind;
}
void convert_pivot_to_one(vector<vector<double>> &table,vector<pair<double,double>> &cs) {
    int index_col = index_of_pivot_column(cs,0);
    int index_row = index_of_pivot_row(table,cs,0);
    if (index_col == -1 || index_row == -1) return;
    cout << "Elemento pivote: " << table[index_row][index_col] << " (Posicion[" << index_row + 1 << "][" << index_col + 1 << "]" << ")\n";
    first_col[index_row] = first_row[index_col];
    init_col[index_row] = Z[index_col].first;
    
    double pivot = table[index_row][index_col];
    for (int i = 0; i < table[0].size(); i++) {
        table[index_row][i] /= pivot;
    }
    cout << "Fila " << index_row + 1 << " convertida a 1\n";
    print_table(table);
}
void elemental_operation(vector<vector<double>> &table, int pivot_row, int pivot_col) {
    int row = table.size();
    int col = table[0].size();
    if (pivot_row == -1 || pivot_col == -1) return;
    cout << "Operaciones elementales\n";
    for (int i = 0; i < row; i++) {
        if (i != pivot_row) {
            double pivot = table[i][pivot_col];
            cout << "Fila " << i + 1 << " = Fila " << i + 1 << " - " << pivot << " * Fila " << pivot_row + 1 << "\n";
            for (int j = 0; j < col; j++) {
                table[i][j] -= pivot * table[pivot_row][j];
            }
        }
    }
    // Operation in CS
    for (int i = 0; i < CS.size(); i ++) {
        CS[i].first = Z[i].first;
        CS[i].second = Z[i].second;
    }
    for (int i = 0; i < CS.size(); i++) {
        for (int j = 0; j < row; j++) {    
            if (init_col[j] != INF) {
                CS[i].first -= init_col[j] * table[j][i];
            }
            else {
                CS[i].second -= table[j][i];
            }
        }
    }
}
void simplex_method(vector<vector<double>> &table,vector<pair<double,double>> &cs) {
    try{
        int cnt = 0;
        while (!all_non_negative(cs) && cnt < 10) {
            cout << "\n--------------------------Iteracion " << cnt + 1 << "-------------------------------\n";
            print_table(table);
            convert_pivot_to_one(table,cs);
            int pivot_row = index_of_pivot_row(table,cs,1);
            int pivot_col = index_of_pivot_column(cs,1);
            elemental_operation(table,pivot_row,pivot_col);
            print_table(table);
            cnt ++;
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Problema no acotado\n"; 
    }
}
int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    // // Example 1
    // // Objective function
    
    string objective_function = "3x1 + 8x2";
    // Restrictions
    vector<string> restrictions = {
        "x1 + 4x2 >= 3.5",
        "x1 + 2x2 >= 2.5"
    };
    
    
    // Example 2
    // Objective function
    /*
    string objective_function = "4x1 + 10x2 + 6x3";

    // Restrictions
    vector<string> restrictions = {
        "2x1 + 2x2 + 3x3 >= 3",
        "x1 + 3x2 + x3 >= 2",
    };
    */
    /*
    // Example 3
    // Objective function
    string objective_function = "3x1 + 4x2 + 5x3";
    vector<string> restrictions = {
        "x1 + 2x2 + 3x3 >= 5",
        "2x1 + 2x2 + x3 >= 6",
    };
    */
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
    // Artificial variables
    vector<string> artificial_vars;
    // Extracting limits of restrictions
    for (auto e : restrictions) {
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
    cout << "Funcion objetivo: " << objective_function << "\n\n";
    cout << "Restricciones: \n";
    for (auto e : restrictions) {
        cout << e << "\n";
    }
    std::cout << "\nLimites de las restricciones: ";
    for (auto e : r_col) {
        std::cout << e << " ";
    }
    std::cout << "\n";
    // Create basics variables according to number of restrictions
    for (int i = 0; i < restrictions.size(); i++) {
        basics_vars.push_back("s" + to_string(i + 1));
    }
    // Create artificial variables according to number of restrictions
    for (int i = 0; i < restrictions.size(); i++) {
        artificial_vars.push_back("A" + to_string(i + 1));
        second_col.push_back("A" + to_string(i + 1));
    }
    // Show basic variables
    std::cout << "Variables basicas: ";
    for (auto e : basics_vars) {
        std::cout << e << " ";
    }
    std::cout << "\n";
    // Extracting coefficients and variables from objective function
    sregex_iterator current_obj(objective_function.begin(), objective_function.end(), rgx);
    sregex_iterator last_obj;
    while (current_obj != last_obj) {
        smatch match = *current_obj;
        string coef_str = match[1].str();
        string var_str = match[2].str();
        if (coef_str == "-")
            coef_str = "-1";
        double coef_d = coef_str.empty() ? 1 : stod(coef_str);
        current_obj++;
        coef_objFunction.push_back(coef_d);
        non_basics_vars.push_back(var_str);
    }
    std::cout << "Variables no basicas: ";
    for (auto e : non_basics_vars) {
        non_basics_vars_map[e] = 0;
        std::cout << e << " ";
    }
    std::cout << "\n";
    cout << "Variables artificiales:";
    // Show artificial variables
    for (auto e : artificial_vars) {
        cout << e << " ";
    }
    cout << "\n";
    for (auto e : non_basics_vars) {
        first_row.push_back(e);
    }
    for (int i = 0; i < sz(basics_vars); i++)
    {
        first_row.push_back(basics_vars[i]);
        first_row.push_back(artificial_vars[i]);
        first_col.push_back(artificial_vars[i]);
    }

    first_row.push_back("r");
    first_col.push_back("z");
    first_col.push_back("CS");
    std::cout << "Primera fila con variables: ";
    for (auto e : first_row) {
        std::cout << e << " ";
    }
    std::cout << "\n";
    std::cout << "Primera columna con variables: ";
    for (auto e : first_col) {
        std::cout << e << " ";
    }
    std::cout << "\n";
    // Extracting coefficients and variables from restrictions
    for (auto e : restrictions) {
        vector<double> coef;
        vector<string> var;
        sregex_iterator current(e.begin(), e.end(), rgx);
        sregex_iterator last;
        while (current != last) {
            smatch match = *current;
            string coef_str = match[1].str();
            string var_str = match[2].str();
            if (coef_str == "-") {
                coef_str = "-1";
            }
            double coef_d = coef_str.empty() ? 1 : stod(coef_str);
            coef.push_back(coef_d);
            var.push_back(var_str);
            // non_basics_vars.push_back(var_str);
            current++;
        }
        tmp_tableCoef.push_back(coef);
    }
    // Fill tableCoef
    int rows = restrictions.size();
    int cols = non_basics_vars.size() + basics_vars.size() + artificial_vars.size() + 1;
    vector<double> zeros(cols, 0);
    vector<vector<double>> table(rows, zeros);
    //cout << "Size of table: " << table.size() << " " << table[0].size() << "\n";

    // Fill table with coefficients
    for (int i = 0; i < tmp_tableCoef.size(); i++) {
        for (int j = 0; j < tmp_tableCoef[i].size(); j++) {
            table[i][j] = tmp_tableCoef[i][j];
        }
    }
    // Fill basics and artificial variables
    for (int i = 0; i < rows; i++) {
        table[i][non_basics_vars.size() + 2 * i] = -1;
        table[i][non_basics_vars.size() + 2 * i + 1] = 1;
    }
    // Fill init_col
    for (int i = 0; i < rows; i ++) {
        init_col.push_back(INF);
    }
    // Fill r column
    for (int i = 0; i < rows; i++) {
        table[i][cols - 1] = r_col[i];
    }
    // Fill Z
    for (int i = 0; i < cols - 1; i++) {
        if (i < non_basics_vars.size()) {
            Z.push_back({coef_objFunction[i], 0});
        }    
        else {
            if ((i - non_basics_vars.size()) % 2 == 0) {
                Z.push_back({0, 0});
            } else {
                Z.push_back({0, 1});
            } 
        }
    }
    // Fill CS
    for (int i = 0; i < cols - 1; i++) {
        CS.push_back({0, 0});
    }
    // Operation in CS first
    for (int i = 0; i < CS.size(); i++) {
        CS[i].first = Z[i].first;
        CS[i].second = Z[i].second;
    }
    // Operation in CS second
    for (int i = 0; i < CS.size(); i++) {
        for (int j = 0; j < rows; j++) {
            //cout << "CS: " << CS[i].first << " " << CS[i].second << ' ' << table[j][i] << "\n";
            if (init_col[j] != INF) {
                CS[i].first -= init_col[j] * table[j][i];
            }
            else {
                CS[i].second -= table[j][i];
            }
        }
    }
    //print_table(table);
    simplex_method(table,CS);
    // Show results
    cout << "Resultados: \n";
    for (int i = 0; i < rows; i++) {
        non_basics_vars_map[first_col[i]] =  table[i][cols - 1];
    }
    for (auto e : non_basics_vars_map) {
        cout << e.first << " = " << e.second << "\n";
    }
    cout << "Z : " << objective_function << "\n";
    double ans = 0;
    cout << "Z = ";
    for (int i = 0; i < non_basics_vars.size(); i++) {
        if (i)  cout << " + ";
        cout << Z[i].first << " * (" << non_basics_vars_map["x"+ to_string(i + 1)] << ") ";
        ans += Z[i].first * non_basics_vars_map["x" + to_string(i + 1)];
    }
    cout << "\nZ = " << ans << "\n";
    return 0;
}