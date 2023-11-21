// ISFParameters.hpp
#ifndef ISFPARAMETERS_HPP
#define ISFPARAMETERS_HPP

#include <string>
#include <vector>
#include <unordered_map>
#include <functional>
#include <imgui.h>
#include <variant>

#include <nlohmann/json.hpp>

struct FloatParameterValue {
    float value;
};

struct Point2DParameterValue {
    float x, y;
};

struct ColorParameterValue {
    float r, g, b, a; // Assuming RGBA format
};

struct Image{};
struct ImageParameterValue {
    // Assuming Image is a predefined class or struct
    Image value;
};

struct LongParameterValue {
    long value;
};

struct BoolParameterValue {
    bool value;
};

struct EventParameterValue {
    // Event-specific data
};

struct PopUpButtonParameterValue {
    int selectedIndex; // Index of the selected item
    std::vector<std::string> options;
};

struct RangeParameterValue {
    float min, max;
};

struct ColorAlphaParameterValue {
    float r, g, b, a; // Similar to Color, but might have different usage/context
};

struct AudioParameterValue {
    // Audio-specific data
};

struct AudioFFTParameterValue {
    // FFT-specific data
};

struct Matrix4x4ParameterValue {
    float matrix[4][4];
};

struct Matrix3x3ParameterValue {
    float matrix[3][3];
};

struct Matrix2x2ParameterValue {
    float matrix[2][2];
};

using ParameterValueVariant = std::variant<
    FloatParameterValue,
    Point2DParameterValue,
    ColorParameterValue,
    ImageParameterValue,
    LongParameterValue,
    BoolParameterValue,
    EventParameterValue,
    PopUpButtonParameterValue,
    RangeParameterValue,
    ColorAlphaParameterValue,
    AudioParameterValue,
    AudioFFTParameterValue,
    Matrix4x4ParameterValue,
    Matrix3x3ParameterValue,
    Matrix2x2ParameterValue
>;

struct ISFParameter {
    std::string type;
    std::string name;
    nlohmann::json properties;
    ParameterValueVariant value;
    ParameterValueVariant defaultValue;
    void initialise(ParameterValueVariant d){
        value = d;
        defaultValue = d;
    }
};

class ISFParameters {
public:
    static ISFParameters * parseISFShaderAndDisplayParams(const std::string& isfSource);

    void doImGuiWidgets();

    ISFParameters(const std::vector<ISFParameter>& parms){
        params = parms;
        }

    std::vector<ISFParameter> params;

    // Add a method to associate input names with parameters
    void addInputParameter(const std::string& inputName, const ISFParameter& param);
    ISFParameter* getParameterByName(const std::string& inputName);
    std::unordered_map<std::string, ISFParameter> inputParameterMap;

private:
    // Maintain a dictionary of input names to parameters
    //std::vector<ISFParameter> ISFParameters::constructParametersFromJson(const json &mergedJson)

};

#endif // ISFPARAMETERS_HPP
