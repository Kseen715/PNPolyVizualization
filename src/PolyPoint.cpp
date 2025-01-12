#include "PolyPoint.h"

namespace pnpoly
{
    // Test algorithm of Jordan curve theorem from
    // https://wrfranklin.org/Research/Short_Notes/pnpoly.html#The%20C%20Code
    char simpleSolution(vec2 point, const std::vector<vec2>& polygon)
    {
        size_t i, j, nvert = polygon.size();
        bool c = false;

        for (i = 0, j = nvert - 1; i < nvert; j = i++)
        {
            if (((polygon[i].y >= point.y) != (polygon[j].y >= point.y)) &&
                (point.x <= (polygon[j].x - polygon[i].x) * (point.y - polygon[i].y) / (polygon[j].y - polygon[i].y) + polygon[i].x))
                c = !c;
        }

        return (char)c;
    }

    // Returns 1 if point if it to the left of the line, 0 if to the right
    bool isLeft(vec2 pStart, vec2 pEnd, vec2 point)
    {
        return ((pEnd.x - pStart.x) * (point.y - pStart.y) -
            (pEnd.y - pStart.y) * (point.x - pStart.x)) > 0;
    }

    // Non-zero winding algorithm for pnpoly
    char nonZeroWindingRule(vec2 point, const std::vector<vec2>& polygon)
    {
        int wn = 0; // Winding number counter
        size_t nvert = polygon.size(); // Count of polygon vertices

        for (size_t i = 0; i < nvert; i++)
        {
            if (polygon[i].y <= point.y) // If first point of polygon is below point
            {
                if (polygon[(i + 1) % nvert].y > point.y) // If second point of polygon is above point
                {
                    if (isLeft(polygon[i], polygon[(i + 1) % nvert], point))
                    {
                        wn++; // Increase winding number
                    }
                }
            }
            else // If first point in polygon is above point
            {
                if (polygon[(i + 1) % nvert].y <= point.y) // If second point in polygon is below point
                {
                    if (!isLeft(polygon[i], polygon[(i + 1) % nvert], point))
                    {
                        wn--; // Decrease winding number
                    }
                }
            }
        }
        return wn ? 1 : 0; // If winding number equals 0, point is outside polygon, else inside
    }

    // Convert vec2 to a ImVec2
    ImVec2 castToImVec2(vec2 vec) {
        return ImVec2(vec.x, vec.y);
    }

    // Convert ImVec2 to a vec2
    vec2 castToVec2(ImVec2 vec) {
        return vec2(vec.x, vec.y);
    }

    workTime time;

    // Universal function for pnpoly algorithm visualization.
    // Accepts a function pointer to the algorithm to use.
    imVec2pnpSet pnpolyUni(const std::vector<ImVec2>& points, const std::vector<ImVec2>& polygon, char(*func)(vec2, const std::vector<vec2>&))
    {
        imVec2pnpSet result;
        vec2pnpSet temp;

        // Get vec2 from ImVec2 for the algorithm
        std::vector<vec2> polygonVec2;
        for (auto& i : polygon)
            polygonVec2.push_back(castToVec2(i));

        std::vector<vec2> pointsVec2;
        for (auto& i : points)
            pointsVec2.push_back(castToVec2(i));

        // Call the algorithm
        time.start();
        for (auto& point : pointsVec2)
        {
            char res = func(point, polygonVec2);
            if (res == 0)
                temp.Out.push_back(point);
            else if (res == 1)
                temp.In.push_back(point);
            else
                temp.Indef.push_back(point);
        }
        time.stop();

        // Get ImVec2 from vec2 for the result output
        for (auto& i : temp.In)
            result.In.push_back(castToImVec2(i));

        for (auto& i : temp.Out)
            result.Out.push_back(castToImVec2(i));

        for (auto& i : temp.Indef)
            result.Indef.push_back(castToImVec2(i));
        return result;
    }
}


namespace PNPolyViz
{
    // Returns an array of randomly generated ImVec2 points
    std::vector<ImVec2> generateRandomPoints(size_t count)
    {
        std::vector<ImVec2> points;
        points.reserve(count);
        for (size_t i = 0; i < count; ++i)
        {
            //rand with rand signs
            points.push_back(ImVec2((rand() % 500 - 250) * (float)0.99,
                (rand() % 500 - 250) * (float)0.99));
        }
        return points;
    }

    // Returns coordonates of center of whiteboard
    ImVec2 getCenterOfWhiteboard()
    {
        ImVec2 center = ImGui::GetCursorScreenPos();
        center.x += ImGui::GetWindowWidth() / 2;
        center.y += ImGui::GetWindowHeight() / 2;
        return center;
    }

    // Draws axis in the center of whiteboard
    void drawAxis(ImU32 color)
    {
        ImVec2 center = getCenterOfWhiteboard();
        ImGui::GetWindowDrawList()->AddLine(
            ImVec2(center.x - ImGui::GetWindowWidth() / 2, center.y),
            ImVec2(center.x + ImGui::GetWindowWidth() / 2, center.y),
            color);
        ImGui::GetWindowDrawList()->AddLine(
            ImVec2(center.x, center.y - ImGui::GetWindowHeight() / 2),
            ImVec2(center.x, center.y + ImGui::GetWindowHeight() / 2),
            color);
    }

    // Centers points on whiteboard
    void toCenterAndScale(std::vector<ImVec2>& points)
    {
        ImVec2 center = getCenterOfWhiteboard();
        for (auto& i : points)
        {
            i.x = i.x * screenScale + center.x;
            i.y = i.y * -screenScale + center.y; // Minus for a horizontal flip of the y axis
        }
    }

    // Draws points with a given color
    void drawPoints(std::vector<ImVec2> points, ImU32 color)
    {
        toCenterAndScale(points);
        for (auto& point : points)
        {
            ImGui::GetWindowDrawList()->AddCircleFilled(point, 2, color);
        }
    }

    // Draws a polygon with a given color
    void drawPolygon(std::vector<ImVec2> polygon, ImU32 color)
    {
        toCenterAndScale(polygon);
        for (size_t i = 0; i < polygon.size(); ++i)
        {
            ImGui::GetWindowDrawList()->AddLine(polygon[i], polygon[(i + 1) % polygon.size()], color);
        }
    }

    // Draws a whiteboard (actually it's black color) with points and lines
    void WhiteBoard(const std::vector<ImVec2>& pointsIn,
        const std::vector<ImVec2>& pointsOut,
        const std::vector<ImVec2>& pointsIndef,
        const std::vector<std::vector<ImVec2>>& polygons)
    {
        std::vector<ImVec2> pointsInN = pointsIn;
        std::vector<ImVec2> pointsOutN = pointsOut;
        std::vector<ImVec2> pointsIndefN = pointsIndef;
        std::vector<std::vector<ImVec2>> polygonsN = polygons;

        // Draw axis
        drawAxis(crossColor);

        // Draw all points
        drawPoints(pointsInN, inColor);
        drawPoints(pointsOutN, outColor);
        drawPoints(pointsIndefN, indefColor);

        // Draw polygons
        for (auto& polygon : polygonsN)
        {
            drawPolygon(polygon, polygonColor);
        }
    }

    void doWork()
    {
        // DockSpaceOverview
        ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->Pos);
        ImGui::SetNextWindowSize(viewport->Size);
        ImGui::SetNextWindowViewport(viewport->ID);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        ImGui::Begin("DockSpaceOverview", nullptr, ImGuiWindowFlags_NoTitleBar
            | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove
            | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus
            | ImGuiWindowFlags_NoBackground);
        ImGui::PopStyleVar(3);
        ImGuiID dockspace_id = ImGui::GetID("DockSpaceOverview");
        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_PassthruCentralNode);
        ImGui::End();

        // Prepare points
        std::vector<ImVec2> points = OBJ::toImVec2("points");

        // Prepare polygon
        std::vector<std::vector<ImVec2>> polygons;
        std::vector<ImVec2> polygon = OBJ::toImVec2("polygon");

        imVec2pnpSet pointSet = pnpoly::pnpolyUni(points, polygon, pnpoly::simpleSolution);

        // Draw whiteboard
        ImGui::Begin("Whiteboard", nullptr);
        ImGui::GetWindowDrawList();

        if (polygon.size() > 0)
            polygons.push_back(polygon);
        WhiteBoard(pointSet.In, pointSet.Out, pointSet.Indef, polygons);

        ImGui::Text("FPS: %.1f | pointsIn: %lld | pointsOut: %lld | pointsIndef: %lld | algTime: %s",
            ImGui::GetIO().Framerate, pointSet.In.size(),
            pointSet.Out.size(), pointSet.Indef.size(),
            pnpoly::time.getStringTime().c_str());

        ImGui::End();

        //system("pause");
    }
}

