#include <WidgetOp.hpp>

// Define the static member outside the class
std::vector<WidgetOp *> WidgetOp::registeredWidgets;

template <typename Lambda>
void WidgetOp::executeWithExceptionHandling(Lambda &&lambda)
{
    try
    {
        lambda();
    }
    catch (const std::exception &e)
    {
        // Handle the exception
        std::cerr << "Error: " << e.what() << std::endl;
    }
    catch (...)
    {
        // Handle any other exceptions (not derived from std::exception)
        std::cerr << "An unknown error occurred." << std::endl;
    }
}