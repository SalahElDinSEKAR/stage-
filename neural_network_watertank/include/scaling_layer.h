#pragma once

#include <torch/torch.h>

class ScalingImpl : public torch::nn::Module {
 public:
    explicit ScalingImpl(double scale) : scale(scale) {}
    torch::Tensor forward(torch::Tensor x) {
        return x * scale;
    }

 private:
    double scale;
};

TORCH_MODULE(Scaling);