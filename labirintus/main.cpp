#include <iostream>
#include <sstream>
#include <memory>
#include <vector>

#include <boost/algorithm/string.hpp>

namespace labyrinth
{

    enum class CellType : char
    {
        E_Wall = 'W',
        E_Monitor = 'M',
        E_Corridor = 'C'
    };

    struct Cell
    {
        Cell(CellType type_)
            : type{type_}, checked{false}, minPathNorthEast{-1}, minPathSouthEast{-1}, minPathSouthWest{-1}, minPathNorthWest{-1}
        {
        }

        CellType type;
        bool checked;
        int minPathNorthEast;
        int minPathSouthEast;
        int minPathSouthWest;
        int minPathNorthWest;
    };

    inline Cell& index(std::vector<Cell>& map, int row, int col, int cols)
    {
        return map[row * cols + col];
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

    auto map = std::make_unique<std::vector<Cell>>(rows * 2 * cols);

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



