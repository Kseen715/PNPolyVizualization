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
// Массивы двумерных векторов для pnpoly.
// In - лежит в полигоне.
// Out - не лежит в полигоне.
// Indef - не определено.
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

namespace PolyPoint
{
    constexpr float pointRadius = 2;
    constexpr float screenScale = 1;
    const ImU32 crossColor = IM_COL32(255, 255, 255, 25);
    const ImU32 greenColor = IM_COL32(55, 255, 55, 255);
    const ImU32 redColor = IM_COL32(255, 55, 55, 255);
    const ImU32 purpleColor = IM_COL32(255, 0, 255, 255);
    const ImU32 whiteColor = IM_COL32(255, 255, 255, 255);

    void DoWork();

    // draw a whiteboard with points from vector and polygons from vector
    void WhiteBoard(const std::vector<ImVec2>& pointsIn, const std::vector<ImVec2>& pointsOut, const std::vector<ImVec2>& pointsIndef, const std::vector<std::vector<ImVec2>>& polygons);

    //void WhiteBoard(const imVec2pnpSet& points, const std::vector<std::vector<ImVec2>>& polygons);
}
