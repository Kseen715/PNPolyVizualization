#include "PolyPoint.h"

ImVec2 castToImVec2(vec2 vec) {
    return ImVec2(vec.x, vec.y);
}

vec2 castToVec2(ImVec2 vec) {
    return vec2(vec.x, vec.y);
}

namespace pnpoly
{
    //nonzero winding rule
    char simpleSolution(vec2 point, const std::vector<vec2>& polygon)
    {
        int i, j, nvert = polygon.size();
        bool c = false;

        for (i = 0, j = nvert - 1; i < nvert; j = i++)
        {
            if (((polygon[i].y >= point.y) != (polygon[j].y >= point.y)) &&
                (point.x <= (polygon[j].x - polygon[i].x) * (point.y - polygon[i].y) / (polygon[j].y - polygon[i].y) + polygon[i].x))
                c = !c;
        }

        return c;
    }

    char nonZeroWindingRule(ImVec2 point, const std::vector<ImVec2>& polygon)
    {

        return 0;
    }

    // non-winding rule for point and polygon realization, with function choose
    imVec2pnpSet pnpolyUni(const std::vector<ImVec2>& points, const std::vector<ImVec2>& polygon, char(*func)(vec2, const std::vector<vec2>&))
    {
        imVec2pnpSet result;
        vec2pnpSet temp;

        // Получаем из библиотечного типа ImVec2 данные для функций
        std::vector<vec2> polygonVec2;
        for (auto& i : polygon)
            polygonVec2.push_back(castToVec2(i));

        std::vector<vec2> pointsVec2;
        for (auto& i : points)
            pointsVec2.push_back(castToVec2(i));

        //time.restart();
        for (auto& point : pointsVec2)
        {
            // 0 - out, 1 - in, other - indef

            char res = func(point, polygonVec2);
            if (res == 0)
                temp.Out.push_back(point);
            else if (res == 1)
                temp.In.push_back(point);
            else
                temp.Indef.push_back(point);
        }
        // time.stop();



         // Получаем из библиотечного типа vec2 данные для ImGui
        for (auto& i : temp.In)
            result.In.push_back(castToImVec2(i));

        for (auto& i : temp.Out)
            result.Out.push_back(castToImVec2(i));

        for (auto& i : temp.Indef)
            result.Indef.push_back(castToImVec2(i));

        return result;
    }



}


namespace PolyPoint
{
    // generate points
    std::vector<ImVec2> generateRandomPoints(size_t count)
    {
        std::vector<ImVec2> points;
        points.reserve(count);
        for (size_t i = 0; i < count; ++i)
        {
            //rand with rand signs
            points.push_back(ImVec2((rand() % 500 - 250) * 0.99, (rand() % 500 - 250) * 0.99));
        }
        return points;
    }

    // Draw a whiteboard with points and lines
    void WhiteBoard(const std::vector<ImVec2>& pointsIn,
        const std::vector<ImVec2>& pointsOut,
        const std::vector<ImVec2>& pointsIndef,
        const std::vector<std::vector<ImVec2>>& polygons)
    {
        std::vector<ImVec2> pointsInN = pointsIn;
        std::vector<ImVec2> pointsOutN = pointsOut;
        std::vector<ImVec2> pointsIndefN = pointsIndef;
        std::vector<std::vector<ImVec2>> polygonsN = polygons;

        // Get coordonates of center of whiteboard
        ImVec2 center = ImGui::GetCursorScreenPos();
        center.x += ImGui::GetWindowWidth() / 2;
        center.y += ImGui::GetWindowHeight() / 2;

        // Draw a cross in the center of whiteboard
        ImGui::GetWindowDrawList()->AddLine(
            ImVec2(center.x - ImGui::GetWindowWidth() / 2, center.y),
            ImVec2(center.x + ImGui::GetWindowWidth() / 2, center.y),
            crossColor);
        ImGui::GetWindowDrawList()->AddLine(
            ImVec2(center.x, center.y - ImGui::GetWindowHeight() / 2),
            ImVec2(center.x, center.y + ImGui::GetWindowHeight() / 2),
            crossColor);
        // Circle in the center
        ImGui::GetWindowDrawList()->AddCircleFilled(center, 4, crossColor);


        // get coordinates of down left corner of whiteboard
        ImVec2 whiteboardPos = ImGui::GetCursorScreenPos();

        // Add whiteboard coordinates to all points
        for (auto& point : pointsInN)
        {
            point.x += center.x;
            point.y *= -1;
            point.y += center.y;
        }
        for (auto& point : pointsOutN)
        {
            point.x += center.x;
            point.y *= -1;
            point.y += center.y;
        }
        for (auto& point : pointsIndefN)
        {
            point.x += center.x;
            point.y *= -1;
            point.y += center.y;
        }
        // Add whiteboard coordinates to all polygons
        for (auto& polygon : polygonsN)
        {
            for (auto& point : polygon)
            {
                point.x += center.x;
                point.y *= -1;
                point.y += center.y;

            }
        }
        // Draw points
        for (auto& point : pointsInN)
        {
            ImGui::GetWindowDrawList()->AddCircleFilled(point, pointRadius, greenColor);
        }
        for (auto& point : pointsOutN)
        {
            ImGui::GetWindowDrawList()->AddCircleFilled(point, pointRadius, redColor);
        }
        for (auto& point : pointsIndefN)
        {
            ImGui::GetWindowDrawList()->AddCircleFilled(point, pointRadius, purpleColor);
        }
        // Draw lines

        // Draw last line
        if (polygonsN.size() > 0)
        {
            for (auto& polygon : polygonsN)
            {
                for (unsigned i = 0; i < polygon.size() - 1; i++)
                {
                    ImGui::GetWindowDrawList()->AddLine(polygon[i], polygon[i + 1], whiteColor);
                }
            }
            ImGui::GetWindowDrawList()->AddLine(polygonsN[polygonsN.size() - 1]
                [polygonsN[polygonsN.size() - 1].size() - 1], polygonsN[polygonsN.size() - 1][0], whiteColor);
        }
        // ConvexFill 
        /*for (auto& polygon : polygonsN)
        {
            ImGui::GetWindowDrawList()->AddConvexPolyFilled(polygon.data(), polygon.size(), crossColor);
        }*/
    }

    void DoWork()
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
        //std::vector<ImVec2> points = generateRandomPoints(10000);

        std::vector<ImVec2> points = OBJ::toImVec2("points10000");

        // Prepare polygon
        std::vector<std::vector<ImVec2>> polygons;

        //std::vector<ImVec2> polygon = generateRandomPoints(0);

        std::vector<ImVec2> polygon = OBJ::toImVec2("polygonCrown");

        imVec2pnpSet pointSet = pnpoly::pnpolyUni(points, polygon, pnpoly::simpleSolution);

        for (auto& point : pointSet.In)
        {
            point.x *= screenScale;
            point.y *= screenScale;
        }
        for (auto& point : pointSet.Out)
        {
            point.x *= screenScale;
            point.y *= screenScale;
        }
        for (auto& point : pointSet.Indef)
        {
            point.x *= screenScale;
            point.y *= screenScale;
        }
        for (auto& point : polygon)
        {
            point.x *= screenScale;
            point.y *= screenScale;
        }

        if (polygon.size() > 0)
            polygons.push_back(polygon);

        // draw whiteboard
        ImGui::Begin("Whiteboard", nullptr);
        ImDrawList* drawList = ImGui::GetWindowDrawList();

        // time output



        //ImGui::Text("FPS: %.1f | pointsIn: %d | pointsOut: %d | pointsIndef: %d | time: %s",
        //    ImGui::GetIO().Framerate, pointSet.In.size(),
        //    pointSet.Out.size(), pointSet.Indef.size(), time.getStringTime());
        ImGui::Text("FPS: %.1f | pointsIn: %d | pointsOut: %d | pointsIndef: %d",
            ImGui::GetIO().Framerate, pointSet.In.size(),
            pointSet.Out.size(), pointSet.Indef.size());

        WhiteBoard(pointSet.In, pointSet.Out, pointSet.Indef, polygons);
        ImGui::End();
        //system("pause");
    }
}

