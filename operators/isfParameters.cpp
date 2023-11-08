// ISFParameters.cpp
// ISFParameters.cpp
#include "ISFParameters.hpp"
#include <sstream>
#include <fstream>
#include <iostream>
#include <unordered_map>
#include <vector>

std::vector<ISFParameter> ISFParameters::parseISFShaderAndDisplayParams(const std::string &shaderPath)
{
    std::vector<ISFParameter> params;

    std::ifstream shaderFile(shaderPath);
    if (!shaderFile.is_open())
    {
        std::cerr << "Failed to open shader file: " << shaderPath << std::endl;
        return params;
    }

    bool parsingGLSL = false;
    std::stringstream glslCode;
    std::string jsonBlock;

    std::string line;
    while (std::getline(shaderFile, line))
    {
        if (!parsingGLSL)
        {
            // Check for the beginning of the JSON metadata block
            if (line.find("/*{") == 0)
            {
                parsingGLSL = true;
                jsonBlock = "";
            }
        }
        else
        {
            // Check for the end of the JSON metadata block
            if (line.find("}*/") == 0)
            {
                parsingGLSL = false;
                // Parse the JSON metadata block manually
                if (!jsonBlock.empty())
                {
                    std::istringstream jsonStream(jsonBlock);
                    json metadata;
                    try
                    {
                        jsonStream >> metadata;
                    }
                    catch (const std::exception &e)
                    {
                        std::cerr << "Error parsing JSON metadata: " << e.what() << std::endl;
                        jsonBlock = ""; // Clear the JSON block
                        continue;
                    }

                    // Extract input parameters from the JSON block
                    if (metadata.count("INPUTS") > 0)
                    {
                        const json &inputs = metadata["INPUTS"];
                        for (const auto &input : inputs)
                        {
                            ISFParameter param;
                            param.type = input["TYPE"];
                            param.name = input["NAME"];
                            param.description = input["LABEL"];
                            param.value = input["DEFAULT"];
                            param.minValue = input["MIN"];
                            param.maxValue = input["MAX"];
                            params.push_back(param);
                        }
                    }

                    // Clear the JSON block
                    jsonBlock = "";
                }
            }
            else
            {
                // Append the line to the JSON block
                jsonBlock += line + "\n";
            }
        }
    }

    shaderFile.close();
    return params;
}

// Define a mapping from ISF parameter types to ImGui widget constructors
const std::unordered_map<std::string, std::function<void(const ISFParameter &)>> widgetConstructors = {
    {"float", [](const ISFParameter &param)
     { ImGui::SliderFloat(param.name.c_str(), &param.value, param.minValue, param.maxValue); }},
    {"int", [](const ISFParameter &param)
     { ImGui::SliderInt(param.name.c_str(), reinterpret_cast<int *>(&param.value), static_cast<int>(param.minValue), static_cast<int>(param.maxValue)); }},
    {"bool", [](const ISFParameter &param)
     { ImGui::Checkbox(param.name.c_str(), reinterpret_cast<bool *>(&param.value)); }},
    {"string", [](const ISFParameter &param)
     {
         char inputBuffer[256]; // Adjust the buffer size as needed
         strncpy(inputBuffer, param.value.c_str(), sizeof(inputBuffer));
         if (ImGui::InputText(param.name.c_str(), inputBuffer, sizeof(inputBuffer)))
         {
             param.value = inputBuffer;
         }
     }},
    {"event", [](const ISFParameter &param)
     {
         if (ImGui::Button(param.name.c_str()))
         {
             // Handle the event when the button is pressed
         }
     }},
    {"point2D", [](const ISFParameter &param)
     { ImGui::DragFloat2(param.name.c_str(), &param.value.x); }},
    {"point3D", [](const ISFParameter &param)
     { ImGui::DragFloat3(param.name.c_str(), &param.value.x); }},
    {"color", [](const ISFParameter &param)
     {
         ImVec4 color(param.value, 0.0f, 0.0f, 1.0f); // Assuming RGBA format
         if (ImGui::ColorEdit4(param.name.c_str(), &color.x))
         {
             param.value = color.x;
         }
     }},
    {"image", [](const ISFParameter &param)
     {
         // Assuming you have an image preview widget for selecting an image
         if (ImGui::ImageButton(imageTextureID, ImVec2(50, 50)))
         {
             // Handle image selection or input
         }
     }},
    {"audio", [](const ISFParameter &param)
     {
         // Assuming you have an audio waveform display widget
         // and controls for audio input
         // Handle audio input and visualization
     }},
    {"booleanOp", [](const ISFParameter &param)
     {
         // Assuming you have a booleanOp parameter in your ISF shader
         // Example ISFParameter for booleanOp:
         // ISFParameter param;
         // param.type = "booleanOp";
         // param.name = "Enable Operation";
         // param.value = 0.0f; // 0 or 1 for representing boolean operations

         if (param.type == "booleanOp")
         {
             bool isChecked = (param.value != 0.0f);
             if (ImGui::Checkbox(param.name.c_str(), &isChecked))
             {
                 param.value = isChecked ? 1.0f : 0.0f; // Update the parameter value based on checkbox state
             }
         }
     }},
    {"curve", [](const ISFParameter &param)
     {
         // Assuming you have a curve widget for selecting curves
         // and handles for curve manipulation
         // Handle curve input and visualization
     }},
    // Add more parameter types as needed
};

void ISFParameters::createImGuiWidgets(const std::vector<ISFParameter> &params)
{
    // Create ImGui widgets for each parsed parameter based on its type
    for (const ISFParameter &param : params)
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
