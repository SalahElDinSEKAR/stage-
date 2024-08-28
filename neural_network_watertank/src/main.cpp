// #include <torch/torch.h>
// #include <iostream>
// #include <vector>
// #include "neural_net.h"

// // Custom dataset class
// class CustomDataset : public torch::data::datasets::Dataset<CustomDataset> {
// private:
//     torch::Tensor data, targets;

// public:
//     CustomDataset(torch::Tensor data, torch::Tensor targets) 
//         : data(data), targets(targets) {}

//     torch::data::Example<> get(size_t index) override {
//         return {data[index], targets[index]};
//     }

//     torch::optional<size_t> size() const override {
//         return data.size(0);
//     }
// };

// int main() {
//     // Device
//     torch::Device device(torch::kCPU);

//     // Hyperparameters
//     const int64_t num_observations = 10;  // Example value
//     const int64_t hidden_layer_size = 128;  // Example value
//     const int64_t num_actions = 2;  // Example value
//     const double umax = 1.0;  // Example value

//     const int64_t batch_size = 512;
//     const double initial_learning_rate = 1e-5;  // Start with a very small learning rate
//     const int64_t max_epochs = 40;
//     const double gradient_threshold = 10.0;

//     // Instantiate the model
//     NeuralNet model(num_observations, hidden_layer_size, num_actions, umax);
//     model->to(device);

//     // Define optimizer
//     torch::optim::Adam optimizer(model->parameters(), torch::optim::AdamOptions(initial_learning_rate));

//     // Example dataset (replace with your own dataset)
//     auto data = torch::randn({1000, num_observations});
//     auto targets = torch::randn({1000, 1});

//     // Normalize data
//     data = (data - data.mean()) / data.std();
//     targets = (targets - targets.mean()) / targets.std();

//     std::cout << "Initial Data: " << data << std::endl;
//     std::cout << "Initial Targets: " << targets << std::endl;

//     auto dataset = CustomDataset(data, targets).map(torch::data::transforms::Stack<>());
//     auto data_loader = torch::data::make_data_loader(std::move(dataset), batch_size);

//     for (int epoch = 0; epoch < max_epochs; ++epoch) {
//         double epoch_loss = 0.0;
//         model->train();
//         for (auto& batch : *data_loader) {
//             auto data = batch.data.to(device);
//             auto target = batch.target.to(device);

//             // Forward pass
//             auto output = model->forward(data);

//             // std::cout << "Output: " << output << std::endl;

//             auto loss = torch::mse_loss(output, target);

//             // std::cout << "Loss: " << loss << std::endl;

//             if (std::isinf(loss.item<float>())) {
//                 std::cerr << "Infinite loss encountered, terminating training." << std::endl;
//                 return -1;
//             }

//             // Backward pass and optimization
//             optimizer.zero_grad();
//             loss.backward();

//             // Gradient clipping
//             for (auto& param : model->parameters()) {
//                 if (param.grad().defined()) {
//                     param.grad().data().clamp_(-gradient_threshold, gradient_threshold);
//                 }
//             }

//             optimizer.step();

//             epoch_loss += loss.template item<float>() * data.size(0);
//         }

//         epoch_loss /= dataset.size().value();
//         std::cout << "Epoch [" << (epoch + 1) << "/" << max_epochs << "], Loss: " << epoch_loss << '\n';
//     }

//     // Testing Loop
//     model->eval();
//     torch::NoGradGuard no_grad;  // Disable gradient computation for testing

//     double test_loss = 0.0;
//     for (const auto& batch : *data_loader) {
//         auto data = batch.data.to(device);
//         auto target = batch.target.to(device);

//         // Forward pass
//         auto output = model->forward(data);
//         auto loss = torch::mse_loss(output, target);

//         test_loss += loss.template item<float>() * data.size(0);
//     }

//     test_loss /= dataset.size().value();
//     std::cout << "Test Loss: " << test_loss << '\n';

//     std::cout << "Training and Testing finished!\n";
//     return 0;
// }

// // Copyright 2020-present pytorch-cpp Authors
// #include <torch/torch.h>
// #include <iostream>
// #include <vector>
// #include "neural_net.h"

// // Custom dataset class
// class CustomDataset : public torch::data::datasets::Dataset<CustomDataset> {
// private:
//     torch::Tensor data, targets;

// public:
//     CustomDataset(torch::Tensor data, torch::Tensor targets) 
//         : data(data), targets(targets) {}

//     torch::data::Example<> get(size_t index) override {
//         return {data[index], targets[index]};
//     }

//     torch::optional<size_t> size() const override {
//         return data.size(0);
//     }
// };

// int main() {
//     // Device
//     torch::Device device(torch::kCPU);

//     // Hyperparameters
//     const int64_t num_observations = 10;  // Example value
//     const int64_t hidden_layer_size = 128;  // Example value
//     const int64_t num_actions = 2;  // Example value
//     const double umax = 1.0;  // Example value

//     const int64_t batch_size = 512;
//     const double initial_learning_rate = 1e-5;
//     const int64_t max_epochs = 40;
//     const double gradient_threshold = 10.0;

//     // Instantiate the model
//     NeuralNet model(num_observations, hidden_layer_size, num_actions, umax);
//     model->to(device);

//     // Define optimizer
//     torch::optim::Adam optimizer(model->parameters(), torch::optim::AdamOptions(initial_learning_rate));

//     // Example dataset (replace with your own dataset)
//     auto data = torch::randn({1000, num_observations});
//     auto targets = torch::randn({1000, 1});
//     auto dataset = CustomDataset(data, targets).map(torch::data::transforms::Stack<>());
//     auto data_loader = torch::data::make_data_loader(std::move(dataset), batch_size);

//     for (int epoch = 0; epoch < max_epochs; ++epoch) {
//         double epoch_loss = 0.0;
//         model->train();
//         for (auto& batch : *data_loader) {
//             auto data = batch.data.to(device);
//             auto target = batch.target.to(device);

//             // Forward pass
//             auto output = model->forward(data);
//             auto loss = torch::mse_loss(output, target);

//             // Backward pass and optimization
//             optimizer.zero_grad();
//             loss.backward();

//             // Gradient clipping
//             for (auto& param : model->parameters()) {
//                 param.grad().data().clamp_(-gradient_threshold, gradient_threshold);
//             }

//             optimizer.step();

//             epoch_loss += loss.template item<float>() * data.size(0);
//         }

//         epoch_loss /= dataset.size().value();
//         std::cout << "Epoch [" << (epoch + 1) << "/" << max_epochs << "], Loss: " << epoch_loss << '\n';
//     }

//     std::cout << "Training finished!\n";
//     return 0;
// }

// #include <torch/torch.h>
// #include "neural_net.h"

// int main() {
//     // Set the parameters
//     int64_t num_observations = 10;
//     int64_t hidden_layer_size = 64;
//     int64_t num_actions = 5;
//     double umax = 1.0;

//     // Create an instance of the network
//     NeuralNet net(num_observations, hidden_layer_size, num_actions, umax);

//     // Create a dummy input tensor
//     torch::Tensor input = torch::rand({1, num_observations});

//     // Perform a forward pass through the network
//     torch::Tensor output = net->forward(input);

//     // Print the output
//     std::cout << "Input:" << std::endl << input << std::endl;
//     std::cout << "Output:" << std::endl << output << std::endl;

//     return 0;
// }

#include <torch/torch.h>
#include <iostream>
#include <iomanip>  // For std::setprecision

// Your NeuralNet class definition here
#include "neural_net.h"
using namespace std;

int main() {
    // Set up the training parameters
    int64_t num_observations = 10;
    int64_t hidden_layer_size = 64;
    int64_t num_actions = 5;
    double umax = 1.0;

    int64_t batch_size = 512;
    double learning_rate = 1e-3;
    int64_t max_epochs = 40;
    double gradient_threshold = 10.0;

    // Create an instance of the network
    NeuralNet net(num_observations, hidden_layer_size, num_actions, umax);
    cout<< net<< endl;
    // net->to(torch::kCPU);  // Use CPU for this example

    // // Create an optimizer
    // torch::optim::Adam optimizer(net->parameters(), torch::optim::AdamOptions(learning_rate));

    // // Dummy dataset
    // auto dataset = torch::rand({10000, num_observations});
    // auto target = torch::rand({10000, 1});
    
    // auto data_loader = torch::data::make_data_loader(
    //     torch::data::datasets::TensorDataset(dataset, target).map(
    //         torch::data::transforms::Stack<>()),
    //     batch_size);

    // // Training loop
    // for (int64_t epoch = 0; epoch < max_epochs; ++epoch) {
    //     net->train();
    //     double running_loss = 0.0;
    //     int64_t batch_count = 0;

    //     for (auto& batch : *data_loader) {
    //         auto data = batch.data;
    //         auto targets = batch.target;

    //         optimizer.zero_grad();
    //         auto output = net->forward(data);
    //         auto loss = torch::nn::functional::mse_loss(output, targets);
    //         loss.backward();

    //         // Gradient clipping
    //         torch::nn::utils::clip_grad_norm_(net->parameters(), gradient_threshold);

    //         optimizer.step();

    //         running_loss += loss.item<double>();
    //         batch_count++;
    //     }

    //     std::cout << "Epoch [" << epoch + 1 << "/" << max_epochs << "], "
    //               << "Loss: " << std::setprecision(4) << running_loss / batch_count << std::endl;
    // }

    return 0;
}
