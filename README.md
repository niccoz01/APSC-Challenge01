# APSC-Challenge01
The goal of this repository is to provide multiple iterative methods to find the minimum of a function $f: \mathbb{R}^n \rightarrow \mathbb{R}$. The repository contains four files:
* `optimizer.cpp` and its header, which contain the declaration and definitions of the minimizing functions as well as the `step_strategy` function;
* `Makefile`, in which the shell commands are provided;
* `main.cpp`, the core file which is executed.

In particular, there are three available methods defined in `optimizer.cpp`:
* `GD`, i.e. the classic first-order gradient descent method;
* `heavy_ball`, a first-order gradient descent algorithm which takes into consideration also the momentum at each step;
* `ADAM`, an algorithm used for training deep learning models, which combines momentum and adaptive learning rates to efficiently find the optimal parameters of the model which corresponds to those which achieve the minimum of the so-called loss function, $f$ in our case.

There is also the possibility to decide the learning rate update to adopt when using the gradient descent or the heavy ball methods. Starting from an initial learning rate, $\alpha_0$, the user can pick one of the following:
* Exponential decay: $\alpha_k = \alpha_0 e^{\mu k}$
* Inverse decay: $\alpha_k = \frac{\alpha_0}{1+\mu k}$
* Approximate line search: here we use the Armijo rule. Note that it is not available for the heavy-ball method since the direction $\textbf{d}_k$ cannot be guaranteed to be a descent direction.
## How To Use
After having cloned the files, to run the program simply type in the terminal 
```
make
```
and it will compile and execute everything by itself using a `struct` of default parameters. 
There is also the possibility to run the program using parameters different from the default ones directly from the command line: typing 
```
make configure
```
the program will show which are the default values used and it will ask the user wheter they want to customize them or not. If the answer is positive, the user will be asked to provide the new parameters through the command line one by one.

Two **important** notes:
* The default function is $f(x_1, x_2) = x_1x_2+4x_1^2+x_2^2+3x_1$. To change it, modify it in the `main.cpp` as well as the gradient function (`grad_f`) and the starting point. It is not possible to modify These three parameters from the command line.
* Make sure you provide the right inputs when setting the parameters in the command line or there might occur conversion issues. As an example, passing a char to `eps_r` will automatically convert that char into a double much bigger than the possibly desired value to control the residual.
