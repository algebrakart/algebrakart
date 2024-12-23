#pragma once

#ifndef MM_AUDIOSPECTRUMAXIS_H
#define MM_AUDIOSPECTRUMAXIS_H

#include <unordered_map>
#include <vector>
#include <string>

using namespace Urho3D;

#include <Urho3D/Container/Vector.h>
#include <Urho3D/Container/Str.h>
//=============================================================================
// AUDIO SPECTRUM AXIS
//=============================================================================

// Derived from https://github.com/agehama/minimal_spectrum_analyzer
// Agehama (https://twitter.com/agehama_)
class AudioSpectrumAxis
{
public:

    AudioSpectrumAxis() = default;

    static std::string PrintAxis(size_t characterSize, const std::vector<std::pair<std::string, float>>& labels)
    {
        std::string retStr = "";
        const size_t axisLength = characterSize + 2;
        std::string str(axisLength, ' ');
        std::string str2(axisLength, '-');

        const auto& minLabel = labels[0].first;
        const auto& maxLabel = labels[1].first;
        str.replace(0, minLabel.size(), minLabel.c_str());
        str2.replace(0, 1, "[");

        str.replace(str.size() - maxLabel.size(), maxLabel.size(), maxLabel.c_str());
        str2.replace(str2.size() - 1, 1, "]");

        const auto getCenterIndex = [&](float pos)
        {
            return static_cast<int>(pos * axisLength + 0.5f);
        };

        for (int i = 2; i < labels.size(); ++i)
        {
            const auto [label, pos] = labels[i];

            const int halfIndex = label.size() / 2;
            const int indexCenter = getCenterIndex(pos);
            const int indexBegin = indexCenter - halfIndex;
            const int indexEnd = indexBegin + label.size();

            if (0 <= indexBegin - 1 && indexEnd + 1 < str.size() - 1)
            {
                const size_t occupiedIndex = str.substr(indexBegin-1, label.size()+2).find_first_not_of(" ");
                if (occupiedIndex == std::string::npos)
                {
                    str.replace(indexBegin, label.size(), label.c_str());
                    str2.replace(indexCenter, 1, "+");
                }
            }
        }

        std::unordered_map<char, std::string> charMap;
        charMap['['] = "├";
        charMap[']'] = "┤";
        charMap['-'] = "─";
        charMap['+'] = "┴";

       // std::cout << str << " [Hz]\n";
        retStr += str + " [Hz]\n";

        for (char c: str2)
        {
//            std::cout << charMap[c];
            retStr += charMap[c];
        }

        return retStr;
    }
};




#endif // MM_AUDIOSPECTRUMAXIS_H
