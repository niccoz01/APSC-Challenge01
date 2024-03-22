#include <vector>
#include <functional>
#include <string>

std::vector<double> GD(std::function<double(std::vector<double>)> const &f, std::function<std::vector<double>(std::vector<double>)> const &grad_f,
                        std::vector<double> const &x0, double const &eps_r, double const &eps_s, unsigned const &k_max, double const &alpha_0, 
                        double const &mu, std::string const &stepStrategy);

std::vector<double> heavy_ball(std::function<double(std::vector<double>)> const &f, std::function<std::vector<double>(std::vector<double>)> const &grad_f,
                        std::vector<double> const &x0, double const &eps_r, double const &eps_s, unsigned const &k_max, double const &alpha_0, 
                        double const &mu, double const &eta, std::string const &stepStrategy);

std::vector<double> ADAM(std::function<double(std::vector<double>)> const &f, std::function<std::vector<double>(std::vector<double>)> const &grad_f,
                        std::vector<double> const &x0, double const &eps_r, double const &eps_s, unsigned const &k_max, double const &alpha_0, 
                        double const &beta_1, double const &beta_2, double const &epsilon);

//! It evaluates the learning ratefor step k given as input the stepStrategy parameter
double step_strategy(std::string &method, std::string const &stepStrategy, double const &alpha_0, double const &mu, unsigned const &k, std::function<double(std::vector<double>)> const &f,
                     std::function<std::vector<double>(std::vector<double>)> const &grad_f, std::vector<double> const &x_k);

