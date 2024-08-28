#pragma once

#include <torch/torch.h>
#include "scaling_layer.h"

class NeuralNetImpl : public torch::nn::Module {
 public:
    NeuralNetImpl(int64_t num_observations, int64_t hidden_layer_size, int64_t num_actions, double umax);

    torch::Tensor forward(torch::Tensor x);

 private:
    torch::nn::Linear fc1;
    torch::nn::Linear fc2;
    torch::nn::Linear fc3;
    torch::nn::Linear fc4;
    torch::nn::Linear fc5;
    torch::nn::Linear fc6;
    torch::nn::Linear fcLast;
    torch::nn::Tanh tanhLast;
    // torch::nn::Scaling scalingLayer;
    Scaling scalingLayer;
    torch::nn::Linear routput;
};

TORCH_MODULE(NeuralNet);
