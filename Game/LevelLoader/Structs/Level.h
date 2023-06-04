#ifndef LEVEL_LOADER_STRUCTS_H
#define LEVEL_LOADER_STRUCTS_H

#include <string>

namespace Galaga
{
    struct Layout
    {
        int row             {1};
        int row_span        {1};
        int count           {3};
        int space_between   {10};
    };

    struct Level
    {
        std::string name        {};
        int rows                {5};
        Layout boss               {};
        Layout butterfly     {};
        Layout bee                 {};
    };
}


#endif // !LEVEL_LOADER_STRUCTS_H