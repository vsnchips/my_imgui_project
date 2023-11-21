#ifndef _WIDGET_OP_HPP
#define _WIDGET_OP_HPP

#include <iostream>
#include <vector>
#include <string>

#define DEBUG

#ifdef DEBUG
#include <boost/stacktrace.hpp>
#endif

class WidgetOp
{
public:
    virtual ~WidgetOp() {}    // Virtual destructor
    virtual void doGui() = 0; // Pure virtual function

    static void registerWidget(WidgetOp *widget)
    {
        registeredWidgets.push_back(widget);
    }

    static void renderWidgets()
    {
        for (auto &widget : registeredWidgets)
        {
            widget->doGui(); // Call doGui on each registered widget
        }
    }

    std::string errorMessage;

protected:
    // Method to execute a lambda with exception handling
    template <typename Lambda>
    void executeWithExceptionHandling(Lambda &&lambda)
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

            #ifdef DEBUG
                std::cerr << "Stacktrace:\n" << boost::stacktrace::stacktrace() << std::endl;
            #endif

        }
    }

    static std::vector<WidgetOp *> registeredWidgets;
};

#endif