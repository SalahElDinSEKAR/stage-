#include "neural_net.h"
#include "scaling_layer.h"
#include <torch/torch.h>

NeuralNetImpl::NeuralNetImpl(int64_t num_observations, int64_t hidden_layer_size, int64_t num_actions, double umax)
    : fc1(num_observations, hidden_layer_size),
      fc2(hidden_layer_size, hidden_layer_size),
      fc3(hidden_layer_size, hidden_layer_size),
      fc4(hidden_layer_size, hidden_layer_size),
      fc5(hidden_layer_size, hidden_layer_size),
      fc6(hidden_layer_size, hidden_layer_size),
      fcLast(hidden_layer_size, num_actions),
      tanhLast(),
    //   scalingLayer(torch::nn::ScalingOptions(umax)),
      scalingLayer(umax),  
    //   routput(torch::nn::LinearOptions(num_actions, 1))
    routput(num_actions, 1) {
    register_module("fc1", fc1);
    register_module("fc2", fc2);
    register_module("fc3", fc3);
    register_module("fc4", fc4);
    register_module("fc5", fc5);
    register_module("fc6", fc6);
    register_module("fcLast", fcLast);
    register_module("tanhLast", tanhLast);
    register_module("scalingLayer", scalingLayer);
    register_module("routput", routput);
}

torch::Tensor NeuralNetImpl::forward(torch::Tensor x) {
    x = torch::nn::functional::relu(fc1->forward(x));
    x = torch::nn::functional::relu(fc2->forward(x));
    x = torch::nn::functional::relu(fc3->forward(x));
    x = torch::nn::functional::relu(fc4->forward(x));
    x = torch::nn::functional::relu(fc5->forward(x));
    x = torch::nn::functional::relu(fc6->forward(x));
    // x = torch::nn::functional::relu(fcLast->forward(x));
    x = fcLast->forward(x);
    x = tanhLast->forward(x);
    x = scalingLayer->forward(x);
    x = routput->forward(x);
    return x;
}
