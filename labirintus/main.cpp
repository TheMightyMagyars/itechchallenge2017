#include <iostream>
#include <map>
#include <memory>
#include <sstream>
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
    bool is_any_of(T expected, T first)
    {
        return expected == first;
    }

    template<typename T, typename... U>
    bool is_any_of(T expected, U... others)
    {
        return is_any_of(expected, others...);
    }

    // cell structure
    struct cell
    {
        // full ctor
        cell(celltype type_ = celltype::E_Wall, bool checked_ = false, int minPathNorth_ = -1,
            int minPathNorthEast_ = -1, int minPathSouthEast_ = -1, int minPathSouth_ = -1, int minPathSouthWest_ = -1,
            int minPathNorthWest_ = -1)
            : type(type_), checked(checked_)
        {
            minPaths[direction::E_North] = minPathNorth_;
            minPaths[direction::E_NorthEast] = minPathNorthEast_;
            minPaths[direction::E_SouthEast] = minPathSouthEast_;
            minPaths[direction::E_South] = minPathSouth_;
            minPaths[direction::E_SouthWest] = minPathSouthWest_;
            minPaths[direction::E_NorthWest] = minPathNorthWest_;
        }

        cell(const std::string& type) : cell(type[0] == 'W' ? celltype::E_Wall : type[0] == 'C' ? celltype::E_Corridor : celltype::E_Monitor)
        {}

        friend std::ostream& operator<<(std::ostream& os, const cell& cell)
        {
            os << "[" << (char)cell.type << "" << cell.checked << "]";
            return os;
        }

        celltype type;
        bool checked;
        std::map<direction, int> minPaths;
    };

    // (row,col) index to array index
    constexpr int translate_index(int row, int col, int cols)
    {
        return row * cols + col;
    }

    // get cell reference from row/column indices
    inline cell* index(std::vector<cell>& map, int row, int col, int cols)
    {
        return &map[translate_index(row, col, cols)];
    }

    // get cell neighbour in the given direction
    inline cell* get_neighbour(std::vector<cell>& map, int row, int col, direction dir, int rows, int cols)
    {
        using d = direction;

        // first column
        if (col == 0)
        {
            if ((is_any_of(dir, d::E_NorthWest, d::E_SouthWest))
                ||
                (row == 0 && is_any_of(dir, d::E_North, d::E_NorthEast))
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
            if ((is_any_of(dir, d::E_NorthEast, d::E_SouthEast))
                ||
                (row == 0 && dir == d::E_North)
                ||
                (row == rows - 1 && is_any_of(dir, d::E_South, d::E_SouthWest)))
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
            if ((row == 0 && is_any_of(dir, d::E_NorthWest, d::E_North, d::E_NorthEast))
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
                (row == rows - 1 && is_any_of(dir, d::E_SouthWest, d::E_South, d::E_SouthEast)))
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

    // calculate shortest escape path for a given cell
    int calculate_shortest_path(std::vector<cell>& map, int row, int col, direction dir, int rows, int cols)
    {
        // wall -> no escape
        if (map[translate_index(row, col, cols)].type == celltype::E_Wall)
        {
            return -1;
        }

        auto neighbour = get_neighbour(map, row, col, dir, rows, cols);

        // no neighbour there -> edge, 1 cost
        if (neighbour == nullptr)
        {
            return 1;
        }
        else
        {
            switch (neighbour->type)
            {
            case labyrinth::celltype::E_Corridor:
            {
                return neighbour->minPaths[dir];
            }
            case labyrinth::celltype::E_Monitor:
            {
                return neighbour->minPaths[dir] + 1;
            }
            case labyrinth::celltype::E_Wall:
            {
                return -1;
            }
            default:
                break;
            }
        }
    }

}

int main()
{
    using namespace labyrinth;
    using t = celltype;
    /// setup
    int K = 0, N = 0;   // K = rows, 2 * N = columns
    std::string line;
    std::getline(std::cin, line);
    std::istringstream iss(line);
    iss >> K >> N;

    int rows = K, columns = 2 * N;

    // labyrinth
    auto mapPtr = std::make_unique<std::vector<cell>>(rows * columns);
    auto& map = *mapPtr;

    /// read map
    std::vector<std::string> parts;
    parts.reserve(columns);
    for (int k = 0; k < rows; k++)
    {
        std::getline(std::cin, line);
        boost::algorithm::split(parts, line, boost::is_any_of(" "));
        // reorder columns: 0 1 2 3 4 5 <-> 0 3 1 4 2 5
        for (int i = 0; i < N; i++)
        {
            map[translate_index(k, i * 2, columns)] = cell(parts[i]);
            map[translate_index(k, i * 2 + 1, columns)] = cell(parts[N + i]);
        }
        parts.clear();
    }

    // calculate min path values for the cells
    // sort of dynamic programming: process the outer cells first and continue inwards
    for (size_t i = 0; i < length; i++)
    {
        // calculate min paths in 6 directions

        // set min

        // set checked

    }

    // return min

    for (const auto& cell : map)
    {
        std::cout << cell << '\n';
    }

}



