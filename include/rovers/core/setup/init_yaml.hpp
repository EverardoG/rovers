#ifndef THYME_ENVIRONMENTS_ROVERS_INIT_YAML
#define THYME_ENVIRONMENTS_ROVERS_INIT_YAML

#include <cmath>
// #include <ranges>
#include <iostream>
#include <fstream>
#include "yaml-cpp/yaml.h"

namespace rovers {

/*
 *
 * agent/entity initialization policy based on parameters specified in a yaml file
 *
 */
class YamlInit {
   public:
    YamlInit(std::string yaml_file_path = "hello.yaml") : m_yaml_file_path(yaml_file_path) {
        // Load the yaml file in and store all the initialization variables
        // std::ifstream fin(yaml_file_path);
        
        // // Error check
        // if (!fin.is_open()) {
        //     std::cerr << "Error opening file: " << yaml_file_path << std::endl;
        // }
        std::cout << yaml_file_path << std::endl;

        // Parse YAML
        YAML::Node config = YAML::LoadFile(yaml_file_path);
        // try {
        //     std::cout << yaml_file_path << std::endl;
        //     doc = YAML::LoadFile(yaml_file_path);
        // } catch (const YAML::ParserException& e) {
        //     std::cerr << "Error parsing YAML file: " << e.what() << std::endl;
        // }

    //     // Accessing rovers data
    //     if (doc["rovers"]) {
    //         const YAML::Node& rovers_node = doc["rovers"];
    //         if (rovers_node.IsSequence()) {
    //             for (unsigned i = 0; i < rovers_node.size(); ++i) {
    //                 const YAML::Node& rover = rovers_node[i];
    //                 if (rover.IsMap()) {
    //                     // Access x and y coordinates and add them to vectors
    //                     double x = rover["x"].as<double>();
    //                     double y = rover["y"].as<double>();
    //                     m_rover_xs.push_back(x);
    //                     m_rover_ys.push_back(y);
    //                 } else {
    //                     std::cerr << "Error: Invalid format for rover data at index " << i << std::endl;
    //                 }
    //             }
    //         } else {
    //             std::cerr << "Error: 'rovers' node is not a sequence" << std::endl;
    //         }
    //     } else {
    //         std::cerr << "Error: 'rovers' key not found in YAML file" << std::endl;
    //     }

    //     // Accessing POIs data (similar approach as rovers)
    //     if (doc["pois"]) {
    //         const YAML::Node& pois_node = doc["pois"];
    //         if (pois_node.IsSequence()) {
    //             for (unsigned i = 0; i < pois_node.size(); ++i) {
    //                 const YAML::Node& poi = pois_node[i];
    //                 if (poi.IsMap()) {
    //                     // Access x and y coordinates and add them to vectors
    //                     double x = poi["x"].as<double>();
    //                     double y = poi["y"].as<double>();
    //                     m_poi_xs.push_back(x);
    //                     m_poi_ys.push_back(y);
    //                 } else {
    //                     std::cerr << "Error: Invalid format for POI data at index " << i << std::endl;
    //                 }
    //             }
    //         } else {
    //             std::cerr << "Error: 'pois' node is not a sequence" << std::endl;
    //         }
    //     } else {
    //         std::cerr << "Error: 'pois' key not found in YAML file" << std::endl;
    //     }
    }

    template <typename RoverContainer, typename POIContainer>
    void initialize(RoverContainer& rovers, POIContainer& pois) {
        initialize_rovers(rovers);
        initialize_poi(pois);
    }

   private:
    template <typename RoverContainer>
    void initialize_rovers(RoverContainer& rovers) {
        for (std::size_t i = 0; i < rovers.size(); ++i) {
            rovers[i]->set_position(0.0,0.0);
        //     if (i < m_rover_xs.size() && i < m_rover_ys.size()) {
        //         // Check if indices are within vector bounds to avoid potential out-of-range access
        //         rovers[i]->set_position(m_rover_xs[i], m_rover_ys[i]);
        //     } else {
        //         // Handle potential case where vector sizes don't match the number of rovers
        //         std::cerr << "Warning: Rover count (" << rovers.size() << ") exceeds data in m_rover_xs or m_rover_ys. Setting rover position to default values of <0.0,0.0>" << std::endl;
        //         rovers[i]->set_position(0.0,0.0);
        //     }
        }
    }

    template <typename POIContainer>
    void initialize_poi(POIContainer& pois) {
        for (std::size_t i = 0; i < pois.size(); ++i) {
            pois[i]->set_position(0.0,0.0);
        //     if (i < m_poi_xs.size() && i < m_poi_ys.size()) {
        //         // Check if indices are within vector bounds
        //         pois[i]->set_position(m_poi_xs[i], m_poi_ys[i]);
        //     } else {
        //         // Handle potential case where vector sizes don't match the number of POIs
        //         std::cerr << "Warning: POI count (" << pois.size() << ") exceeds data in m_poi_xs or m_poi_ys. Setting poi position to default values of <0.0,0.0>" << std::endl;
        //         pois[i]->set_position(0.0,0.0);
        //     }
        }
    }

   private:
    // double m_span;
    std::string m_yaml_file_path;
    std::vector<double> m_rover_xs;
    std::vector<double> m_rover_ys;
    std::vector<double> m_poi_xs;
    std::vector<double> m_poi_ys;
};
}  // namespace rovers

#endif
