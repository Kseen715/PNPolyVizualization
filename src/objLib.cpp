#include "objLib.h"


namespace OBJ
{
    void toObj(const std::vector<vec2>& points, std::string fileName)
    {
        fileName += ".obj";
        std::ofstream file(fileName);
        if (file.is_open())
        {
            file << "# " << fileName;
            for (auto& point : points) {
                file << std::endl << "v " << point.x << " " << point.y << " " << 0.0 << " " << 1.0;
            }
            file.close();
            file.close();
        }
    }

    std::vector<vec2> toVec2(std::string fileName)
    {
        fileName += ".obj";
        std::vector<vec2> points;
        std::ifstream file(fileName);

        if (file.is_open())
        {
            std::string line;
            while (std::getline(file, line))
            {
                if (line[0] == 'v')
                {
                    std::string x, y, z, w;
                    std::istringstream iss(line);
                    iss >> x >> y >> z >> w;
                    points.push_back({ std::stof(y), std::stof(z) });
                }
            }
            file.close();
        }
        return points;
    }

#ifdef IMGUI_API
    //vector<ImVec2> to obj file (x,y,z[,w]), w = 1
    void toObj(const std::vector<ImVec2>& points, std::string fileName)
    {
        fileName += ".obj";
        std::ofstream file(fileName);
        if (file.is_open())
        {
            file << "# " << fileName;
            for (auto& point : points) {
                file << std::endl << "v " << point.x << " " << point.y << " " << 0.0 << " " << 1.0;
            }
            file.close();
        }
    }

    std::vector<ImVec2> toImVec2(std::string fileName)
    {
        fileName += ".obj";
        std::vector<ImVec2> points;
        std::ifstream file(fileName);
        if (file.is_open())
        {
            std::string line;
            while (std::getline(file, line))
            {
                if (line[0] == 'v')
                {
                    std::string x, y, z, w;
                    std::istringstream iss(line);
                    iss >> x >> y >> z >> w;
                    points.push_back(ImVec2(std::stof(y), std::stof(z)));
                }
            }
            file.close();
        }
        std::cout << "Points: " << points.size() << std::endl;
        return points;
    }

#endif //IMGUI_API
}
