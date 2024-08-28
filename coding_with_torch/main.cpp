#include <torch/torch.h>
#include <iostream>

uisng namespace std

int main()
{
    torch::Tensor t1= torch::rand({1,2});
    cout<<t1<<endl;
    return 0;
}