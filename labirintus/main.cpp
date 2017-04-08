#include <iostream>
#include <sstream>
#include <memory>
#include <vector>

#include <boost/algorithm/string.hpp>

namespace labyrinth
{

    // possible cell types: Wall, Monitor, Corridor
    enum class celltype : char
    {
        E_Wall = 'W',
        E_Monitor = 'M',
        E_Corridor = 'C'
    };

    // 6 possible directions
    enum class direction : char
    {
        E_North = 'A',
        E_NorthEast = 'B',
        E_NorthWest = 'C',
        E_SouthEast = 'D',
        E_SouthWest = 'E',
        E_South = 'F'
    };

    // template sweetness for easier direction comparison
    template<typename T>
    bool is_any_dir(direction expected, T first)
    {
        return expected == first;
    }

    template<typename... T>
    bool is_any_dir(direction expected, T... others)
    {
        return is_any_dir(expected, others...);
    }

    // cell strcture
    struct cell
    {
        cell(celltype type_, bool checked_ = false) : type{type_}, checked{checked_}, minPath{direction::E_North, -1}
        {}

        celltype type;
        bool checked;
        std::pair<direction, int> minPath;
    };

    // get cell reference from row/column indices
    inline cell* index(std::vector<cell>& map, int row, int col, int cols)
    {
        return &map[row * cols + col];
    }

    // get cell neighbour in the given direction
    inline cell* get_neighbour(std::vector<cell>& map, int row, int col, direction dir, int rows, int cols)
    {
        using d = direction;

        // first column
        if (col == 0)
        {
            if ((is_any_dir(dir, d::E_NorthWest, d::E_SouthWest))
                ||
                (row == 0 && is_any_dir(dir, d::E_North, d::E_NorthEast))
                ||
                (row == rows - 1 && dir == d::E_South))
            {
                return nullptr;
            }
            else
            {
                switch (dir)
                {
                case labyrinth::direction::E_North:
                    return index(map, row - 1, col, cols);
                case labyrinth::direction::E_NorthEast:
                    return index(map, row - 1, col + 1, cols);
                case labyrinth::direction::E_SouthEast:
                    return index(map, row, col + 1, cols);
                case labyrinth::direction::E_South:
                    return index(map, row + 1, col, cols);
                default:
                    throw std::runtime_error("getneighbour(): should not have gotten here.");
                }
            }
        }
        // last column
        else if (col == cols - 1)
        {
            if ((is_any_dir(dir, d::E_NorthEast, d::E_SouthEast))
                ||
                (row == 0 && dir == d::E_North)
                ||
                (row == rows - 1 && is_any_dir(dir, d::E_South, d::E_SouthWest)))
            {
                return nullptr;
            }
            else
            {
                switch (dir)
                {
                case labyrinth::direction::E_North:
                    return index(map, row - 1, col, cols);
                case labyrinth::direction::E_NorthWest:
                    return index(map, row, col - 1, cols);
                case labyrinth::direction::E_SouthWest:
                    return index(map, row + 1, col - 1, cols);
                case labyrinth::direction::E_South:
                    return index(map, row + 1, col, cols);
                default:
                    throw std::runtime_error("getneighbour(): should not have gotten here.");
                }
            }
        }
        // 'higher' columns
        else if (col % 2 == 0)
        {
            if ((row == 0 && is_any_dir(dir, d::E_NorthWest, d::E_North, d::E_NorthEast))
                ||
                (row == rows - 1 && dir == d::E_South))
            {
                return nullptr;
            }
            else
            {
                switch (dir)
                {
                case labyrinth::direction::E_North:
                    return index(map, row - 1, col, cols);
                case labyrinth::direction::E_NorthEast:
                    return index(map, row - 1, col + 1, cols);
                case labyrinth::direction::E_NorthWest:
                    return index(map, row - 1, col - 1, cols);
                case labyrinth::direction::E_SouthEast:
                    return index(map, row, col + 1, cols);
                case labyrinth::direction::E_SouthWest:
                    return index(map, row, col - 1, cols);
                case labyrinth::direction::E_South:
                    return index(map, row + 1, col, cols);
                default:
                    throw std::runtime_error("getneighbour(): should not have gotten here.");
                }
            }
        }
        // 'lower columns'
        else
        {
            if ((row == 0 && dir == d::E_North)
                ||
                (row == rows - 1 && is_any_dir(dir, d::E_SouthWest, d::E_South, d::E_SouthEast)))
            {
                return nullptr;
            }
            else
            {
                switch (dir)
                {
                case labyrinth::direction::E_North:
                    return index(map, row - 1, col, cols);
                case labyrinth::direction::E_NorthEast:
                    return index(map, row, col + 1, cols);
                case labyrinth::direction::E_NorthWest:
                    return index(map, row, col - 1, cols);
                case labyrinth::direction::E_SouthEast:
                    return index(map, row + 1, col + 1, cols);
                case labyrinth::direction::E_SouthWest:
                    return index(map, row + 1, col - 1, cols);
                case labyrinth::direction::E_South:
                    return index(map, row + 1, col, cols);
                default:
                    throw std::runtime_error("getneighbour(): should not have gotten here.");
                }
            }
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

    auto map = std::make_unique<std::vector<cell>>(rows * 2 * cols, cell(celltype::E_Wall));

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



