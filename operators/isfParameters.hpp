// ISFParameters.hpp
#ifndef ISFPARAMETERS_HPP
#define ISFPARAMETERS_HPP

#include <string>
#include <vector>
#include <unordered_map>
#include <functional>
#include <imgui.h>

struct ISFParameter {
    std::string type;
    std::string name;
    std::string description;
    float value;
    float minValue;
    float maxValue;
};

class ISFParameters {
public:
    static std::vector<ISFParameter> parseISFShaderAndDisplayParams(const std::string& shaderPath);
    static void createImGuiWidgets(const std::vector<ISFParameter>& params);

    // Add a method to associate input names with parameters
    void addInputParameter(const std::string& inputName, const ISFParameter& param);
    ISFParameter* getParameterByName(const std::string& inputName);

private:
    // Maintain a dictionary of input names to parameters
    std::unordered_map<std::string, ISFParameter> inputParameterMap;
};

#endif // ISFPARAMETERS_HPP
