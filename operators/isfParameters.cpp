// ISFParameters.cpp
#include "ISFParameters.hpp"
#include <sstream>
#include <fstream>
#include <iostream>
#include <unordered_map>
#include <vector>
#include <regex>

#include <typeindex>

#include "nlohmann/json.hpp"

using json = nlohmann::json;

// Private header
json mergeJsonBlocks(const std::vector<json> &jsonBlocks);
std::vector<json> parseJsonBlocks(const std::string &inputFileName);
std::vector<ISFParameter> constructParametersFromJson(const json &mergedJson);

// Implementations
ISFParameters *ISFParameters::parseISFShaderAndDisplayParams(const std::string &shaderCode)
{

    std::cout << "parsing ISF Json" << std::endl;
    std::vector<json> parsedData = parseJsonBlocks(shaderCode);
    std::cout << "Parsed " << parsedData.size() << " blocks" << std::endl;

    json mergedJson;
    try
    {
        // Merge all parsed JSON blocks into one
        mergedJson = mergeJsonBlocks(parsedData);

        // Process the merged JSON data
        std::cout << "Merged JSON" << mergedJson.dump(4) << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    ISFParameters *isfParms = new ISFParameters(constructParametersFromJson(mergedJson));
    return isfParms;
}

json mergeJsonBlocks(const std::vector<json> &jsonBlocks)
{
    json mergedJson;

    for (const auto &jsonObject : jsonBlocks)
    {
        // Merge the current JSON object into the mergedJson
        for (auto it = jsonObject.begin(); it != jsonObject.end(); ++it)
        {
            if (mergedJson.find(it.key()) != mergedJson.end())
            {
                // Duplicate key found, throw an error
                throw std::runtime_error("Duplicate key found: " + it.key());
            }
            mergedJson[it.key()] = it.value();
        }
    }
    return mergedJson;
}

std::vector<json> parseJsonBlocks(const std::string &inputString)
{

    /*
        std::cout << "parsing:" << std::endl;
        std::cout << inputString << std::endl;
    */
    std::vector<json> parsedJsonBlocks;
    std::istringstream inputStream(inputString); // Create a string stream

    std::string line;
    std::string jsonBlock = "";

    bool inBlock = false;
    std::regex commentPattern(R"(//.*)");

    while (std::getline(inputStream, line))
    {

        // std::cout << (inBlock ? "in-block" : "out-of-block") << " parsing:" << std::endl;
        // std::cout << line << std::endl;

        if (inBlock)
        {

            // Ending the JSON Block.
            if (line.find("}*/") != std::string::npos)
            {

                std::cout << "Found Block End" << std::endl;
                inBlock = false;

                jsonBlock += line.substr(0, line.find("}*/") + 1); // Include the closing */
                std::cout << "Gathered JSON Block:\n"
                          << jsonBlock << std::endl;

                try
                {
                    // Remove // comments within the JSON block
                    jsonBlock = std::regex_replace(jsonBlock, commentPattern, "");
                    std::cout << "Removing comment" << std::endl;

                    // Parse the JSON block
                    std::cout << "Parsing the block" << std::endl;
                    json parsedJson = json::parse(jsonBlock);
                    parsedJsonBlocks.push_back(parsedJson);
                }
                catch (const std::exception &e)
                {
                    std::cerr << "Error parsing JSON: " << e.what() << std::endl;
                }
                jsonBlock.clear();
            }
            else // Appending the line to the block
            {
                jsonBlock += line + "\n";
            }
        }
        // If not in a block, scan for one.
        else if (line.find("/*{") != std::string::npos)
        {

            std::cout << "Found Block Start" << std::endl;
            inBlock = true;
            jsonBlock = line.substr(line.find("/*{") + 2) + "\n"; // skip the opening /*{, add the rest of the line
        }
    }

    return parsedJsonBlocks;
}
// Function to construct ISFParameter objects from a JSON block
std::vector<ISFParameter> constructParametersFromJson(const json &mergedJson)
{
    std::vector<ISFParameter> params; // Vector to store ISFParameter objects

    const std::map<std::string, std::function<ParameterValueVariant()>> typeMap = {
        {"Float", []()
         { return FloatParameterValue{}; }},
        {"Point2D", []()
         { return Point2DParameterValue{}; }},
        {"Color", []()
         { return ColorParameterValue{}; }},
        {"Image", []()
         { return ImageParameterValue{}; }},
        {"Long", []()
         { return LongParameterValue{}; }},
        {"Bool", []()
         { return BoolParameterValue{}; }},
        {"Event", []()
         { return EventParameterValue{}; }},
        {"PopUpButton", []()
         { return PopUpButtonParameterValue{}; }},
        {"Range", []()
         { return RangeParameterValue{}; }},
        {"ColorAlpha", []()
         { return ColorAlphaParameterValue{}; }},
        {"Audio", []()
         { return AudioParameterValue{}; }},
        {"AudioFFT", []()
         { return AudioFFTParameterValue{}; }},
        {"Matrix4x4", []()
         { return Matrix4x4ParameterValue{}; }},
        {"Matrix3x3", []()
         { return Matrix3x3ParameterValue{}; }},
        {"Matrix2x2", []()
         { return Matrix2x2ParameterValue{}; }}};

    // Example for FloatParameterValue
    auto setFloatDefault = [](ISFParameter &param, const nlohmann::json &defaultValue)
    {
        FloatParameterValue floatParamValue;
        floatParamValue.value = defaultValue.get<float>();
        param.initialise(floatParamValue);
    };

    // Example for ColorParameterValue
    auto setColorDefault = [](ISFParameter &param, const nlohmann::json &defaultValue)
    {
        ColorParameterValue colorParamValue;
        // Assuming defaultValue is an array of floats
        colorParamValue.r = defaultValue[0].get<float>();
        colorParamValue.g = defaultValue[1].get<float>();
        colorParamValue.b = defaultValue[2].get<float>();
        colorParamValue.a = defaultValue[3].get<float>();
        param.initialise(colorParamValue);
    };

    // Define default-setting functions
    auto setPoint2DDefault = [](auto &param, const auto &defaultValue)
    {
        // Parse defaultValue and set it
    };

    auto setImageDefault = [](auto &param, const auto &defaultValue)
    {
        // Parse defaultValue and set it
    };

    auto setLongDefault = [](auto &param, const auto &defaultValue)
    {
        param.initialise(LongParameterValue{std::stol(defaultValue.template get<std::string>())});
    };

    auto setBoolDefault = [](auto &param, const auto &defaultValue)
    {
        param.initialise(BoolParameterValue{defaultValue.template get<bool>()});
    };

    auto setEventDefault = [](auto &param, const auto &defaultValue)
    {
        // Parse defaultValue and set it (if applicable)
    };

    auto setPopUpButtonDefault = [](auto &param, const auto &defaultValue)
    {
        // Parse defaultValue and set it
    };

    auto setRangeDefault = [](auto &param, const auto &defaultValue)
    {
        // Parse defaultValue and set it
    };

    auto setColorAlphaDefault = [](auto &param, const auto &defaultValue)
    {
        // Parse defaultValue and set it
    };

    auto setAudioDefault = [](auto &param, const auto &defaultValue)
    {
        // Parse defaultValue and set it (if applicable)
    };

    auto setAudioFFTDefault = [](auto &param, const auto &defaultValue)
    {
        // Parse defaultValue and set it (if applicable)
    };

    auto setMatrix4x4Default = [](auto &param, const auto &defaultValue)
    {
        // Parse defaultValue and set it
    };

    auto setMatrix3x3Default = [](auto &param, const auto &defaultValue)
    {
        // Parse defaultValue and set it
    };

    auto setMatrix2x2Default = [](auto &param, const auto &defaultValue)
    {
        // Parse defaultValue and set it
    };

    // Map type to default-setting function
    std::map<std::type_index, std::function<void(ISFParameter &, nlohmann::json &)>> defaultSetterMap = {
        {typeid(FloatParameterValue), setFloatDefault},
        {typeid(Point2DParameterValue), setPoint2DDefault},
        {typeid(ColorParameterValue), setColorDefault},
        {typeid(ImageParameterValue), setImageDefault},
        {typeid(LongParameterValue), setLongDefault},
        {typeid(BoolParameterValue), setBoolDefault},
        {typeid(EventParameterValue), setEventDefault},
        {typeid(PopUpButtonParameterValue), setPopUpButtonDefault},
        {typeid(RangeParameterValue), setRangeDefault},
        {typeid(ColorAlphaParameterValue), setColorAlphaDefault},
        {typeid(AudioParameterValue), setAudioDefault},
        {typeid(AudioFFTParameterValue), setAudioFFTDefault},
        {typeid(Matrix4x4ParameterValue), setMatrix4x4Default},
        {typeid(Matrix3x3ParameterValue), setMatrix3x3Default},
        {typeid(Matrix2x2ParameterValue), setMatrix2x2Default}
        // ... other types
    };

    auto setDefaultValue = [&](auto &param, const auto &defaultValue)
    {
        auto it = defaultSetterMap.find(typeid(std::decay_t<decltype(param)>));
        if (it != defaultSetterMap.end())
        {
            it->second(param, defaultValue);
        }
        else
        {
            // Handle the case where the type is not found in the map
        }
    };

    // Check if the JSON contains input parameters under the "INPUTS" field
    if (mergedJson.count("INPUTS") > 0)
    {
        // Extract the input parameters from the JSON
        const json &inputs = mergedJson["INPUTS"];

        // Iterate through the input parameters
        for (const auto &input : inputs)
        {
            ISFParameter param; // Create a new ISFParameter object

            // Extract and assign values from the JSON
            param.type = input["TYPE"];
            std::cout << param.type << std::endl;
            param.name = input["NAME"];

            if (input.contains("DEFAULT"))
            {
                nlohmann::json defaultValue = input["DEFAULT"];
                auto it = defaultSetterMap.find(typeid(param.value));
                if (it != defaultSetterMap.end())
                {
                    it->second(param, defaultValue);
                }
            }

            params.push_back(param);
        }
    }
    return params; // Return the vector of ISFParameter objects
}

// Define a mapping from ISF parameter types to ImGui widget constructors
const std::unordered_map<std::string, std::function<void(ISFParameter &)>> widgetConstructors = {
          {"float", [](ISFParameter &param)
        {
            // Assuming param.value is a std::variant and FloatParameterValue is one of the types
            if (auto *floatParam = std::get_if<FloatParameterValue>(&param.value)) {
                // Use the parameter's name as the label, and access the float value.
                ImGui::SliderFloat(param.name.c_str(), &(floatParam->value), /*min */ 0 , /* max value */ 1);
            }
        }
    },
    /*   {"int", [](const ISFParameter &param)
        { ImGui::SliderInt(param.name.c_str(), reinterpret_cast<int *>(&param.value), static_cast<int>(param.minValue), static_cast<int>(param.maxValue)); }},
       {"bool", [](const ISFParameter &param)
        { ImGui::Checkbox(param.name.c_str(), reinterpret_cast<bool *>(&param.value)); }}, */
    {"string", [](ISFParameter &param)
     {
         char inputBuffer[256]; // Adjust the buffer size as needed
                                //      strncpy(inputBuffer, param.value.c_str(), sizeof(inputBuffer));
         if (ImGui::InputText(param.name.c_str(), inputBuffer, sizeof(inputBuffer)))
         {
             //           param.value = inputBuffer;
         }
     }},
    {"event", [](ISFParameter &param)
     {
         if (ImGui::Button(param.name.c_str()))
         {
             // Handle the event when the button is pressed
         }
     }},
    /*
   {"point2D", [](const ISFParameter &param)
    { ImGui::DragFloat2(param.name.c_str(), &param.value.x); }},
   {"point3D", [](const ISFParameter &param)
    { ImGui::DragFloat3(param.name.c_str(), &param.value.x); }},
   {"color", [](const ISFParameter &param)
    {
        ImVec4 color(param.value, 0.0f, 0.0f, 1.0f); // Assuming RGBA format
        if (ImGui::ColorEdit4(param.name.c_str(), &color.x))
        {
         //   param.value = color.x;
        }
    }},*/
    {"image", []( ISFParameter &param)
     {
         // Assuming you have an image preview widget for selecting an image
         // if (ImGui::ImageButton(imageTextureID, ImVec2(50, 50)))
         // {
         // Handle image selection or input
         // }
     }},
    {"audio", []( ISFParameter &param)
     {
         // Assuming you have an audio waveform display widget
         // and controls for audio input
         // Handle audio input and visualization
     }},
    {"booleanOp", []( ISFParameter &param)
     {
         // Assuming you have a booleanOp parameter in your ISF shader
         // Example ISFParameter for booleanOp:
         // ISFParameter param;
         // param.type = "booleanOp";
         // param.name = "Enable Operation";
         // param.value = 0.0f; // 0 or 1 for representing boolean operations

         if (param.type == "booleanOp")
         {
             bool isChecked = std::get<BoolParameterValue>(param.value).value;
             if (ImGui::Checkbox(param.name.c_str(), &isChecked))
             {
                 // param.value = isChecked ? 1.0f : 0.0f; // Update the parameter value based on checkbox state
             }
         }
     }},
    {"curve", []( ISFParameter &param)
     {
         // Assuming you have a curve widget for selecting curves
         // and handles for curve manipulation
         // Handle curve input and visualization
     }},
    // Add more parameter types as needed
};

void ISFParameters::doImGuiWidgets()
{
    // Create ImGui widgets for each parsed parameter based on its type
    for (ISFParameter &param : params)
    {
        auto it = widgetConstructors.find(param.type);
        if (it != widgetConstructors.end())
        {
            // Call the appropriate widget constructor based on the parameter type
            it->second(param);
        }
        else
        {
            // Handle unsupported parameter types or provide default widgets
            ImGui::Text("Unsupported Parameter Type: %s", param.type.c_str());
        }
    }
}

void ISFParameters::addInputParameter(const std::string &inputName, const ISFParameter &param)
{
    inputParameterMap[inputName] = param;
}

ISFParameter *ISFParameters::getParameterByName(const std::string &inputName)
{
    auto it = inputParameterMap.find(inputName);
    if (it != inputParameterMap.end())
    {
        return &(it->second);
    }
    return nullptr; // Parameter with the specified name not found
}