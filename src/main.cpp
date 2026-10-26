#include "app/Application.h"
#include <exception>
#include <iostream>

int main()
{
    try {
        ttt::Application app;
        return app.run();
    } catch (const std::exception& e) {
        std::cerr << "Fatal error: " << e.what() << std::endl;
        return 1;
    }
}
