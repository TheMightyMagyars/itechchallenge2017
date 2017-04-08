#include <iostream>
#include <sstream>
#include <memory>
#include <vector>

#include <boost/algorithm/string.hpp>

namespace labyrinth
{

    enum class celltype : char
    {
        E_Wall = 'W',
        E_Monitor = 'M',
        E_Corridor = 'C'
    };

    enum class direction : char
    {
        E_NorthEast = 'A',
        E_SouthEast = 'B',
        E_SouthWest = 'C',
        E_NorthWest = 'D'
    };

    struct cell
    {
        cell(celltype type_, bool checked_ = false)
            : type{type_}, checked{checked_}, minPathNorthEast{-1}, minPathSouthEast{-1}, minPathSouthWest{-1},
            minPathNorthWest{-1}
        {
        }

        celltype type;
        bool checked;
        int minPathNorthEast;
        int minPathSouthEast;
        int minPathSouthWest;
        int minPathNorthWest;
    };

    inline cell* index(std::vector<cell>& map, int row, int col, int cols)
    {
        return &map[row * cols + col];
    }

    inline cell* get_neighbour(direction dir, int row, int col, int rows, int cols)
    {
        if ((row == 0 && (dir == direction::E_NorthWest || dir == direction::E_SouthWest))
            ||
            (row == rows - 1 && (dir == direction::E_NorthEast || dir == direction::E_SouthEast))
            ||
            ())
        {
            return nullptr;
        }
    }

}

int main()
{
    using namespace labyrinth;

    /// setup
    int rows = 0, cols = 0;
    std::string line;
    std::getline(std::cin, line);
    std::istringstream iss(line);
    iss >> rows >> cols;

    auto map = std::make_unique<std::vector<cell>>(rows * 2 * cols);

    /// read map
    std::vector<std::string> parts;
    parts.reserve(cols * 2);
    for (int i = 0; i < rows; i++)
    {
        std::getline(std::cin, line);
        boost::algorithm::split(parts, line, boost::is_any_of(" "));


        parts.clear();
    }


}



