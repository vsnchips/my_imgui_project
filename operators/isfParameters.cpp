// ISFParameters.cpp
#include "ISFParameters.hpp"
#include <sstream>
#include <fstream>
#include <iostream>
#include <unordered_map>
#include <vector>
#include <regex>
#include "nlohmann/json.hpp"

using json = nlohmann::json;

// Private header
json mergeJsonBlocks(const std::vector<json>& jsonBlocks);
std::vector<json> parseJsonBlocks(const std::string& inputFileName);
std::vector<ISFParameter> constructParametersFromJson(const json &mergedJson);

//Implementations
std::vector<ISFParameter> ISFParameters::parseISFShaderAndDisplayParams(const std::string &shaderCode){

    std::vector<json> parsedData = parseJsonBlocks(shaderCode);
    json mergedJson;
    try {
        // Merge all parsed JSON blocks into one
        mergedJson = mergeJsonBlocks(parsedData);

        // Process the merged JSON data
        std::cout << mergedJson.dump(4) << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
    std::vector<ISFParameter> params = constructParametersFromJson(mergedJson);

    return params;
}

json mergeJsonBlocks(const std::vector<json>& jsonBlocks) {
    json mergedJson;

    for (const auto& jsonObject : jsonBlocks) {
        // Merge the current JSON object into the mergedJson
        for (auto it = jsonObject.begin(); it != jsonObject.end(); ++it) {
            if (mergedJson.find(it.key()) != mergedJson.end()) {
                // Duplicate key found, throw an error
                throw std::runtime_error("Duplicate key found: " + it.key());
            }
            mergedJson[it.key()] = it.value();
        }
    }
    return mergedJson;
}

std::vector<json> parseJsonBlocks(const std::string& inputString) {

    std::vector<json> parsedJsonBlocks;
    std::istringstream inputStream(inputString); // Create a string stream

    std::string line;
    std::string jsonBlock;
    bool inBlock = false;
    std::regex commentPattern(R"(//.*)");

    while (std::getline(inputStream, line)) {
        if (inBlock) {

            // Check for the end of the JSON block
            if (line.find("}*/") != std::string::npos) {

                // End the JSON Block.
                inBlock = false;
                jsonBlock += line.substr(0, line.find("}*/") + 1); // Include the closing */
                std::cout << "Gathered JSON Block:\n" << jsonBlock << std::endl;

                try {
                    // Remove // comments within the JSON block
                    jsonBlock = std::regex_replace(jsonBlock, commentPattern, "");
                    std::cout << "Removing comment" << std::endl;

                    // Parse the JSON block
                    std::cout << "parsing the block" << std::endl;
                    json parsedJson = json::parse(jsonBlock);
                    parsedJsonBlocks.push_back(parsedJson);

                } catch (const std::exception& e) {
                    std::cerr << "Error parsing JSON: " << e.what() << std::endl;
                }
                jsonBlock.clear();
            } else {
                jsonBlock += line + "\n";
            }
        } else if (line.find("/*{") != std::string::npos) {
            inBlock = true;
            jsonBlock = line.substr(line.find("/*{") + 3) + "\n"; // skip the opening /*{, add the rest of the line
        }
    }


    return parsedJsonBlocks;
}

// Function to construct ISFParameter objects from a JSON block
std::vector<ISFParameter> constructParametersFromJson(const json &mergedJson)
{
    std::vector<ISFParameter> params; // Vector to store ISFParameter objects

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
            param.type = input["TYPE"];             // Parameter data type
            param.name = input["NAME"];             // Parameter name
            param.description = input["LABEL"];     // Parameter description (label)
            param.value = input["DEFAULT"];         // Default parameter value
            param.minValue = input["MIN"];          // Minimum parameter value
            param.maxValue = input["MAX"];          // Maximum parameter value

            // Add the constructed ISFParameter object to the vector
            params.push_back(param);
        }
    }
    return params; // Return the vector of ISFParameter objects
}

// Define a mapping from ISF parameter types to ImGui widget constructors
const std::unordered_map<std::string, std::function<void(const ISFParameter &)>> widgetConstructors = {
 /*   {"float", [](const ISFParameter &param)
     { ImGui::SliderFloat(param.name.c_str(), &param.value, param.minValue, param.maxValue); }},
    {"int", [](const ISFParameter &param)
     { ImGui::SliderInt(param.name.c_str(), reinterpret_cast<int *>(&param.value), static_cast<int>(param.minValue), static_cast<int>(param.maxValue)); }},
    {"bool", [](const ISFParameter &param)
     { ImGui::Checkbox(param.name.c_str(), reinterpret_cast<bool *>(&param.value)); }}, */
    {"string", [](const ISFParameter &param)
     {
         char inputBuffer[256]; // Adjust the buffer size as needed
 //      strncpy(inputBuffer, param.value.c_str(), sizeof(inputBuffer));
         if (ImGui::InputText(param.name.c_str(), inputBuffer, sizeof(inputBuffer)))
         {
  //           param.value = inputBuffer;
         }
     }},
    {"event", [](const ISFParameter &param)
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
    {"image", [](const ISFParameter &param)
     {
         // Assuming you have an image preview widget for selecting an image
        // if (ImGui::ImageButton(imageTextureID, ImVec2(50, 50)))
        // {
             // Handle image selection or input
        // }
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
                 //param.value = isChecked ? 1.0f : 0.0f; // Update the parameter value based on checkbox state
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
