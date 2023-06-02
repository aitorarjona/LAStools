#include <iostream>
#include <string>

#include "lasreader.hpp"
#include "lasindex.hpp"

std::string XY_BOUNDS_DELIMITER = ",";

int main(int argc, char *argv[])
{
    if (!(argc > 2))
    {
        std::cout << "Usage: " << argv[0] << " <laxfile> [<xy_bounds>,...]" << std::endl;
        std::cout << "Example: " << argv[0] << " lidar.lax 0.0,0.0,100.0,100.0" << std::endl;
        std::cout << "XY Bounds in format x_min;y_min;x_max;ymax" << std::endl;
        exit(1);
    }

    FILE *file = fopen(argv[1], "rb");
    LASindex lasindex;
    lasindex.read(file);

    float x_min, y_min, x_max, y_max;

    for (int i = 2; i < argc; i++)
    {
        std::string xy_bounds_str(argv[i]);
        size_t pos = 0;
        std::string token;
        int j = 0;
        for (j = 0; j < 4; j++)
        {
            pos = xy_bounds_str.find(XY_BOUNDS_DELIMITER);
            if (pos == std::string::npos)
            {
                pos = xy_bounds_str.length();
            }
            token = xy_bounds_str.substr(0, pos);
            xy_bounds_str.erase(0, pos + XY_BOUNDS_DELIMITER.length());
            switch (j)
            {
            case 0:
                x_min = std::stof(token);
                // std::cout << "x_min " << x_min << std::endl;
                break;
            case 1:
                y_min = std::stof(token);
                // std::cout << "y_min " << y_min << std::endl;
                break;
            case 2:
                x_max = std::stof(token);
                // std::cout << "x_max " << x_max << std::endl;
                break;
            case 3:
                y_max = std::stof(token);
                // std::cout << "y_max " << y_max << std::endl;
                break;
            default:
                perror("Invalid xy_bounds format");
                break;
            }
        }

        auto res = lasindex.intersect_rectangle(x_min, y_min, x_max, y_max);
        if (!res)
        {
            std::cout << "No points found" << std::endl;
            continue;
        }

        // Iterate over intervals and print the offset and size of each interval for the opened LAS file
        std::string intervals;
        while (lasindex.has_intervals())
        {
            std::cout << lasindex.start << "," << lasindex.end << ";";
        }
        std::cout << std::endl;
    }

    return 0;
}