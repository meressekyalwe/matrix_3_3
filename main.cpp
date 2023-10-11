#include <iostream>
#include <fstream>
#include <array>
#include <vector>
#include <string>
#include <map>
#include <iterator>
#include <algorithm>

std::map<int, std::vector<int>> relation;


void SortInAlph(std::string& str)
{
    for (int i = 0; i < str.length(); i++)
    {
        for (int j = i + 1; j < str.length(); j++)
        {
            if (str[i] > str[j])
            {
                char temp = str[i];
                str[i] = str[j];
                str[j] = temp;
            }
        }
    }
}

struct Comb
{
    std::string array;
    std::vector<int> indexes;

    Comb(std::string& array, std::vector<int>& indexes)
    {
        this->array = array;
        this->indexes = indexes;
    }

    bool operator == (const Comb &Other)
    {
        if (this->array.length() != Other.array.length())
        {
            return false;
        }

        for (size_t i = 0; i < this->array.length(); i++)
        {
            if (this->array[i] != Other.array[i])
            {
                return false;
            }

            return (this->array == Other.array);
        }

        return true;
    }
};

bool Validation(std::vector<int>& vec)
{
    auto rab = std::find(relation[vec[0]].begin(), relation[vec[0]].end(), vec[1]); 
    auto rac = std::find(relation[vec[0]].begin(), relation[vec[0]].end(), vec[2]); 
    auto rbc = std::find(relation[vec[1]].begin(), relation[vec[1]].end(), vec[2]); 

    if ((rab != relation[vec[0]].end() && ((rac != relation[vec[0]].end()) || rbc != relation[vec[1]].end())) || ((rab != relation[vec[0]].end() || rac != relation[vec[0]].end()) && rbc != relation[vec[1]].end()) || ((rab != relation[vec[0]].end() || rbc != relation[vec[1]].end()) && rac != relation[vec[0]].end()))
    {
        return true;
    }

    return false;
}


struct Table
{
    std::string board;
    std::vector<Comb> combs;
    std::string current_comb;
    std::vector<int> current_index;

    Table(std::string& board_str)
    {
        board = board_str;

        if (board.length() == 9) GenerateCombinations(3, 0);
    }
    
private:
 
    void GenerateCombinations(int k, int start)
    { 
        if (k == 0)
        {
            if (Validation(current_index))
            {
                
                combs.emplace_back(current_comb, current_index);
            }

            return;
        }
        for (size_t i = start; i < board.size(); i++)
        {
            current_comb += board[i]; 
            current_index.push_back(i);
            GenerateCombinations(k - 1, i + 1);
            current_comb.pop_back();
            current_index.pop_back();
        }
    }

    void Print()
    {
        for (size_t i = 0; i < 3; i++)
        {
            for (size_t j = 0; j < 3; j++)
            {
                std::cout << board[(i * 3 + j)] << " ";
            }

            std::cout << std::endl;
        }
    }
};


int main()
{
    relation[0] = {1, 3, 4};
    relation[1] = {0, 2, 3, 4, 5};
    relation[2] = {1, 4, 5};
    relation[3] = {0, 1, 4, 6, 7};
    relation[4] = {0, 1, 2, 3, 5, 6, 7, 8};
    relation[5] = {1, 2, 4, 7, 8};
    relation[6] = {3, 4, 7};
    relation[7] = {3, 4, 5, 6, 8};
    relation[8] = {4, 5, 7};

    std::vector<Table> boards; 

    std::ifstream input_file("matrix.txt");
    std::ofstream output_file("results.txt");

    std::string str, board_str;

    do
    {
        board_str += str;

        if (board_str.length() == 9)
        {
            Table T(board_str);
            boards.push_back(T);
            board_str.clear();
        }

    } while (getline(input_file, str));

    input_file.close();

    std::map <std::string, int> results;
    for (const Table& T : boards)
    {
        for (const Comb& C : T.combs)
        {
            std::string str = C.array;
            SortInAlph(str);
            results[str]++;
        }     
    }

    for (auto x : results)
    {
        if (x.second > 1) output_file << x.first << " " << x.second << std::endl;
    }

    return 0;
}

