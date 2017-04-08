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
        E_SouthEast = 'C',
        E_SouthWest = 'D',
        E_South = 'E',
        E_NorthWest = 'F',
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

        cell() : cell(celltype::E_Wall, false)
        {}

        cell(const std::string& type) : cell(type[0] == 'W' ? celltype::E_Wall : type[0] == 'C' ? celltype::E_Corridor : celltype::E_Monitor)
        {}

        friend std::ostream& operator<<(std::ostream& os, const cell& cell)
        {
            os << "[" << (char)cell.type << "" << cell.checked << ",(" << (char)cell.minPath.first << "," << cell.minPath.second << ")]";
            return os;
        }

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
    int K = 0, N = 0;   // K = rows, 2 * N = columns
    std::string line;
    std::getline(std::cin, line);
    std::istringstream iss(line);
    iss >> K >> N;

    // labyrinth
    auto map = std::make_unique<std::vector<cell>>(K * 2 * N);

    /// read map
    std::vector<std::string> parts;
    parts.reserve(N * 2);
    for (int k = 0; k < K; k++)
    {
        std::getline(std::cin, line);
        boost::algorithm::split(parts, line, boost::is_any_of(" "));
        // reorder columns: 0 1 2 3 4 5 <-> 0 3 1 4 2 5
        for (int i = 0; i < N; i++)
        {
            (*map)[k * 2 * N + i * 2] = cell(parts[i]);
            (*map)[k * 2 * N + i * 2 + 1] = cell(parts[N + i]);
        }
        parts.clear();
    }


}



