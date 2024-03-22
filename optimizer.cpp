#include "optimizer.hpp"
#include <iostream>
#include <cmath>
#include <vector>
#include <functional>
#include <limits> // For NaN



std::vector<double> GD(std::function<double(std::vector<double>)> const &f, std::function<std::vector<double>(std::vector<double>)> const &grad_f,
                        std::vector<double> const &x0, double const &eps_r, double const &eps_s, unsigned const &k_max, double const &alpha_0, 
                        double const &mu, std::string const &stepStrategy)
{
    std::string method = "gd"; 
    auto x_k = x0;
    std::vector<double> x_next(x_k.size(), 0.0); // Next parameters
    double residual = 0.0;
    for (unsigned k=0; k < k_max; ++k)
    {
        auto grad = grad_f(x_k);
        double alpha_k = step_strategy(method, stepStrategy, alpha_0, mu, k, f, grad_f, x_k);
        double step_length = 0.0;
        for (size_t i=0; i<x_k.size(); i++){
            x_next[i] = x_k[i] - alpha_k * grad[i];
            step_length += std::pow((x_next[i] - x_k[i]),2);
        }

        residual = std::abs(f(x_next) - f(x_k));
    
        x_k = x_next;

        // Checking convergence conditions
        if (std::pow(step_length,0.5) < eps_s || residual < eps_r || k >= k_max) {
            std::cout << "Convergence reached at step "<< k << std::endl;
            break; // Exit loop
        }
        unsigned dummy = k;
        if (dummy == k_max-1) {
            std::cout << "The method did not converge." << std::endl;
            std::vector<double> nan(1);
            nan[0] = std::numeric_limits<double>::quiet_NaN();
            return nan;
        }
    }

    return x_k;   

}

std::vector<double> heavy_ball(std::function<double(std::vector<double>)> const &f, std::function<std::vector<double>(std::vector<double>)> const &grad_f,
                        std::vector<double> const &x0, double const &eps_r, double const &eps_s, unsigned const &k_max, double const &alpha_0, 
                        double const &mu, double const &eta, std::string const &stepStrategy)
{
    std::string method = "hb"; 
    auto x_k = x0;
    std::vector<double> x_next(x_k.size(), 0.0); // Next parameters
    std::vector<double> x_prev = x0;
    double residual = 0.0;
    auto grad_0 = grad_f(x0);

    if (stepStrategy=="arm"){
            std::cout << "Error: The chosen step strategy is not applicable to the heavy-ball method since the direction cannot be guaranteed to be a descent direction."<< std::endl;
            std::vector<double> nan(1);
            nan[0] = std::numeric_limits<double>::quiet_NaN();
            return nan;
    }
    // Iterating through all the function's variables
    for (size_t i=0; i<x_k.size(); i++){
            x_next[i] = x_k[i] - alpha_0 * grad_0[i];
        }

    for (unsigned k=1; k < k_max; ++k)
    {
        auto grad = grad_f(x_k);
        double alpha_k = step_strategy(method, stepStrategy, alpha_0, mu, k, f, grad_f, x_k);
        double step_length = 0.0;

        // Iterating through all the function's variables
        for (size_t i=0; i<x_k.size(); i++){
            x_next[i] = x_k[i] - alpha_k * grad[i] + eta * (x_k[i] - x_prev[i]);
            step_length += std::pow((x_next[i] - x_k[i]),2);
        }

        residual = std::abs(f(x_next) - f(x_k));
        x_prev = x_k;
        x_k = x_next;

        // Check convergence conditions
        if (std::pow(step_length,0.5) < eps_s || residual < eps_r || k >= k_max) {
            std::cout << "Convergence reached at step "<< k << std::endl;
            break; // Exit loop
        }
        unsigned dummy = k;
        if (dummy == k_max-1) {
            std::cout << "The method did not converge " << std::endl;
            std::vector<double> nan(1);
            nan[0] = std::numeric_limits<double>::quiet_NaN();
            return nan;
        }
    }

    return x_k;     
}
                    

std::vector<double> ADAM(std::function<double(std::vector<double>)> const &f, std::function<std::vector<double>(std::vector<double>)> const &grad_f,
                        std::vector<double> const &x0, double const &eps_r, double const &eps_s, unsigned const &k_max, double const &alpha_0, 
                        double const &beta_1, double const &beta_2, double const &epsilon)
{
    std::string method = "adam"; 
    auto x_k = x0;
    std::vector<double> m(x_k.size(), 0.0); // First moment vector
    std::vector<double> v(x_k.size(), 0.0); // Second moment vector
    std::vector<double> x_next(x_k.size(), 0.0); // Next parameters
    double residual = 0.0;

    for (unsigned k = 0; k < k_max; ++k)
    {
        auto grad = grad_f(x_k);

        // Iterating through all the function's variables
        for (size_t i = 0; i < x_k.size(); ++i)
        {
            m[i] = beta_1 * m[i] + (1 - beta_1) * grad[i];
            v[i] = beta_2 * v[i] + (1 - beta_2) * grad[i] * grad[i];
        }

        // Correct bias in moment estimates
        double bias_correction1 = 1.0 / (1 - std::pow(beta_1, k + 1));
        double bias_correction2 = 1.0 / (1 - std::pow(beta_2, k + 1));
        for (size_t i = 0; i < x_k.size(); ++i)
        {
            double m_hat = m[i] * bias_correction1;
            double v_hat = v[i] * bias_correction2;

            // Update parameters
            x_next[i] = x_k[i] - alpha_0 * m_hat / (std::sqrt(v_hat) + epsilon); // Epsilon used to avoid division by zero
        }

        residual = std::abs(f(x_next) - f(x_k));
        x_k = x_next;
    
        // Checking convergence conditions
        if (residual < eps_r || k >= k_max) {
            std::cout << "Convergence reached at step " << k << std::endl;
            break; // Exit loop
        }
    }

    return x_k;   
}


double step_strategy(std::string &method, std::string const &stepStrategy, double const &alpha_0, double const &mu, unsigned const &k,std::function<double(std::vector<double>)> const &f,
                     std::function<std::vector<double>(std::vector<double>)> const &grad_f, std::vector<double> const &x_k)
{
    if(stepStrategy=="exp")                                 // Exponential decay
        return alpha_0 * std::exp(-mu * k);
    else if(stepStrategy=="inv")                            // Inverse decay
        return alpha_0 / (1 + mu * k);
    else if (stepStrategy=="arm"){
        double alpha_k = alpha_0;
        // Set default value for sigma
        double sigma = 0.25;
        while(true){
            auto grad = grad_f(x_k);

            // Armijo rule condition
            std::vector<double> x_next(x_k.size(), 0.0);
            double grad_norm = 0.0;
            for (size_t i = 0; i < x_k.size(); ++i){
                x_next[i] =  x_k[i] - alpha_k * grad[i];
                grad_norm += std::pow(grad[i],2);
            }
            grad_norm = std::sqrt(grad_norm);
            if((f(x_k)-f(x_next))>=(sigma*alpha_k*std::pow(grad_norm,2)))
                break;
            
            // Halving alpha_k for the next iteration
            alpha_k = alpha_k/2;
        }
        return alpha_k;
    }
    else{                                             
        std::cout << "Error: The chosen step strategy is not available. The available strategies are the following: \n 'exp', 'inv', 'arm'" << std::endl;
        return 0;
    }
}
