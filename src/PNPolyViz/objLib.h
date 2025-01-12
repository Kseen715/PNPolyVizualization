// version 0.1.0
#pragma once
#ifndef __OBJ_LIB_H
#define __OBJ_LIB_H

#include <vector>
#include <fstream> // Файловый поток для .obj файлов
#include <sstream> // Строковый поток для чтения .obj файлов

#include "imgui.h" // НУЖНО ЗАКОММЕНТИРОВАТЬ ЕСЛИ НЕТ ГРАФИЧЕСКОЙ БИБЛИОТЕКИ ImGui !!!

#ifndef VEC_2
#define VEC_2
// Двумерный вектор (x, y) aka точка.
typedef struct vec2 {
    float x, y;
    vec2(float x, float y) : x(x), y(y) {}
} vec2;
#endif //VEC_2

#ifndef VEC_2_PNP_SET
#define VEC_2_PNP_SET
// Массивы двумерных векторов для pnpoly.
// In - лежит в полигоне.
// Out - не лежит в полигоне.
// Indef - не определено.
typedef struct vec2pnpSet {
    std::vector<vec2> In;
    std::vector<vec2> Out;
    std::vector<vec2> Indef;
} vec2pnpSet;
#endif //VEC_2_PNP_SET

#ifdef IMGUI_API

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

#endif //IMGUI_API

namespace OBJ
{
    // Записывает в файл filename.obj точки из массива points.
    void toObj(const std::vector<vec2>& points, std::string fileName);

    // Возвращает набор точек типа vec2 из файла fileName.
    std::vector<vec2> toVec2(std::string fileName);

#ifdef IMGUI_API


    // Записывает в файл filename.obj точки из массива points.
    void toObj(const std::vector<ImVec2>& points, std::string fileName);

    // Возвращает набор точек типа ImVec2 из файла fileName.
    std::vector<ImVec2> toImVec2(std::string fileName);

#endif //IMGUI_API
}

#endif //__OBJ_LIB_H
