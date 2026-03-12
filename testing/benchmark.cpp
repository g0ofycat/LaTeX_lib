#include "../core/latex_core.hpp"

int main()
{
    LatexCore core_impl(R"(f _ { 9 } ^ { A } = V _ { 1 1 } \sum _ { ( n ^ { 1 } , n ^ { 2 } ) \ne ( 0 , 0 ) } \left( n ^ { I } g _ { I J } n ^ { J } \right) ^ { - 3 / 2 } \ , \quad V _ { 1 1 } = \sqrt { \det g _ { I J } })");
    core_impl.print_analysis();

    return 0;
}