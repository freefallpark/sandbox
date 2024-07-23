#include <iostream>
#include <memory>

void func(std::shared_ptr<int> d){
    std::cout << "func: " << d << d.use_count() << std::endl;
}
void func(int* d){
    std::cout << "func: " << d << std::endl;
}

int main() {
    std::shared_ptr<int> sharedPtr_int(new int);
    int *ptr_int;
    std::cout << "main_shared: " << sharedPtr_int << std::endl;
    func(sharedPtr_int);
    std::cout << "main_ptr: " << ptr_int << std::endl;
    func(ptr_int);
    return 0;
};