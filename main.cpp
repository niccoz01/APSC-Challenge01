#include <iostream>
#include <vector>
#include <cmath>
#include <functional>
#include <cstdlib> // For std::atof
#include "optimizer.hpp"

int main(int argc, char **argv) {
     // Initializing a struct containing all the parameters
    struct Parameters {
    std::string method; // Default method is gradient descent
        unsigned int k_max;
        double eps_r;
        double eps_s;
        double alpha_0;
        double mu;
        double eta;
        double beta_1;
        double beta_2;
        double epsilon;
        std::string stepStrategy;
    };

    Parameters params;
    
    // Set default values
    params.method = "gd";
    params.k_max = 1000;
    params.eps_r = 1e-6;
    params.eps_s = 1e-6;
    params.alpha_0 = 0.1;
    params.mu = 0.2;
    params.eta = 0.9;
    params.stepStrategy = "inv";
    params.beta_1 = 0.9;
    params.beta_2 = 0.9;
    params.epsilon = 1e-8;

    std::vector<double> x0{0., 0.}; // Starting point
    std::vector<double> sol;
    std::string choice;

    auto f = [](std::vector<double> const &x)
    { return x[0] * x[1] + 4 * std::pow(x[0], 4) + std::pow(x[1], 2) + 3 * x[0]; };

    auto grad_f = [](std::vector<double> const &x)
    { return std::vector<double>{x[1] + 16 * std::pow(x[0], 3) + 3, x[0] + 2 * x[1]}; };

    // Check if custom parameters are provided
    if (argc >= 2) {
        choice = argv[1];
        params.method = argv[2];

        if (params.method == "hb" || params.method == "gd" || params.method == "adam") {
            // Check if user wants to customize parameters
            if (argc >= 3 && choice == "c") {
                if (argc < 9 && params.method == "gd") {
                    std::cerr << "Error: Insufficient parameters for gradient descent method." << std::endl;
                    return 1; // Exit with error code
                }
                if (argc < 10 && params.method == "hb") {
                    std::cerr << "Error: Insufficient parameters for heavy ball method." << std::endl;
                    return 1; // Exit with error code
                }
                if (argc < 10 && params.method == "adam") {
                    std::cerr << "Error: Insufficient parameters for ADAM method." << std::endl;
                    return 1; // Exit with error code
                }
                try {
                    if (params.method == "gd") {
                        // Parse parameters for gradient descent method
                        params.k_max = std::stoi(argv[5]);
                        params.eps_r = std::stof(argv[3]);
                        params.eps_s = std::stof(argv[4]);
                        params.alpha_0 = std::stof(argv[6]);
                        params.mu = std::stof(argv[7]);
                        params.stepStrategy = argv[8];
                    } else if (params.method == "hb") {
                        // Parse parameters for heavy ball method
                        params.k_max = std::stoi(argv[5]);
                        params.eps_r = std::stof(argv[3]);
                        params.eps_s = std::stof(argv[4]);
                        params.alpha_0 = std::stof(argv[6]);
                        params.mu = std::stof(argv[7]);
                        params.stepStrategy = argv[8];
                        params.eta = std::stof(argv[9]);
                    }
                    else if (params.method == "adam") {
                        // Parse parameters for heavy ball method
                        params.k_max = std::stoi(argv[5]);
                        params.eps_r = std::stof(argv[3]);
                        params.eps_s = std::stof(argv[4]);
                        params.alpha_0 = std::stof(argv[6]);
                        params.beta_1 = std::stof(argv[7]);
                        params.beta_2 = std::stof(argv[8]);
                        params.epsilon = std::stof(argv[9]);
                    }
                } catch (const std::invalid_argument& e) {
                    std::cerr << "Error: Invalid parameter value." << std::endl;
                    return 1; // Exit with error code
                }
            } else if (argc == 3 && argv[1] != "d") {
                std::cerr << "Error: Invalid choice. Please enter 'd' for default parameters, or 'c' to customize." << std::endl;
                return 1; // Exit with error code
            }
        } else {
            std::cerr << "Error: Invalid optimization method." << std::endl;
            return 1; // Exit with error code
        }
    }

    if (params.method == "gd") {
        // Call gradient descent function with specified parameters
        sol = GD(f, grad_f, x0, params.eps_r, params.eps_s, params.k_max, params.alpha_0, params.mu, params.stepStrategy);
    } else if (params.method == "hb") {
        // Call heavy ball function with specified parameters
        sol = heavy_ball(f, grad_f, x0, params.eps_r, params.eps_s, params.k_max, params.alpha_0, params.mu, params.eta, params.stepStrategy);
    } else if (params.method == "adam"){
        // Call ADAM function with specified parameters
        sol = ADAM(f, grad_f, x0, params.eps_r, params.eps_s, params.k_max, params.alpha_0, params.beta_1, params.beta_2, params.epsilon);
    } else {
        std::cerr << "Error: Invalid optimization method." << std::endl;
        return 1; // Exit with error code
    }

    // Output result
    if (std::isnan(sol[0]))
        return 0;
    else {
        std::cout << "---------------- Results -----------------" << std::endl;
        std::cout << "Minimum found at: " << std::endl;
    
        for (auto const &val : sol)
            std::cout << val << " ";
        std::cout << "\nValue for the function at its minimum: "<< std::endl;
        std::cout << f(sol) << std::endl;
        std::cout << "------------------------------------------" << std::endl;
    }
    return 0;
}
