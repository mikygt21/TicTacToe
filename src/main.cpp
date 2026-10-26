#include "app/Application.h"
#include <exception>
#include <iostream>

int main()
{
    try {
        ttt::Application app;
        return app.run();
    } catch (const std::exception& e) {
        std::cerr << "Fatal error: " << e.what() << '\n';
        return 1;
    }
}
