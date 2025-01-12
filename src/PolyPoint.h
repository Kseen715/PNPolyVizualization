#pragma once

#include "imgui.h"
#include "imgui_internal.h"
#include "workTimeLib.h"

#include <vector>
#include <algorithm>
#include <thread>
#include <chrono>
#include <string>
#include <iostream>

#include "objLib.h"

#ifndef IM_VEC_2_PNP_SET
#define IM_VEC_2_PNP_SET
// ������� ��������� �������� ��� pnpoly.
// In - ����� � ��������.
// Out - �� ����� � ��������.
// Indef - �� ����������.
typedef struct imVec2pnpSet {
    std::vector<ImVec2> In;
    std::vector<ImVec2> Out;
    std::vector<ImVec2> Indef;
} imVec2pnpSet;
#endif //IM_VEC_2_PNP_SET


namespace pnpoly
{
    char simpleSolution(vec2 point, const std::vector<vec2>& polygon);
    //non-winding rule for points and polygon realization
    imVec2pnpSet pnpolyUni(const std::vector<ImVec2>& points, const std::vector<ImVec2>& polygon, char(*func)(vec2, const std::vector<vec2>&));
}

namespace PNPolyViz
{
    constexpr float pointRadius = 1.5;
    constexpr float screenScale = 2.5;
    const ImU32 crossColor = IM_COL32(255, 255, 255, 25);
    const ImU32 inColor = IM_COL32(55, 255, 55, 255);
    const ImU32 outColor = IM_COL32(255, 55, 55, 255);
    const ImU32 indefColor = IM_COL32(255, 0, 255, 255);
    const ImU32 polygonColor = IM_COL32(255, 255, 255, 255);

    void doWork();

    // draw a whiteboard with points from vector and polygons from vector
    void WhiteBoard(const std::vector<ImVec2>& pointsIn, const std::vector<ImVec2>& pointsOut, const std::vector<ImVec2>& pointsIndef, const std::vector<std::vector<ImVec2>>& polygons);

    //void WhiteBoard(const imVec2pnpSet& points, const std::vector<std::vector<ImVec2>>& polygons);
}
