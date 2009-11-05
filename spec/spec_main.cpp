/* Specific - Minimal C++ spec framework.
 

The zlib/libpng License


Copyright (c) 2008 Mikko Lehtonen

This software is provided 'as-is', without any express or implied
warranty. In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

    1. The origin of this software must not be misrepresented; you must not
    claim that you wrote the original software. If you use this software
    in a product, an acknowledgment in the product documentation would be
    appreciated but is not required.

    2. Altered source versions must be plainly marked as such, and must not be
    misrepresented as being the original software.

    3. This notice may not be removed or altered from any source
    distribution.
*/


#include "spec.h"
#include <cstdlib>

int main(int argc, char *argv[]) 
{

    std::string subset("");

    specific::ProgressWriter progressWriter;
    specific::SpecdocWriter specdocWriter;
    specific::SpecWriter* writer = &progressWriter;

    for(size_t i = 1; i < size_t(argc); ++i) {
        if( std::string("-s") == argv[i] ) {
            writer = &specdocWriter;
        } else {
            subset = argv[i];
        }
    }


    bool success = specific::SpecRunner::getInstance().run(*writer, subset);

    return success ? EXIT_SUCCESS : EXIT_FAILURE;
}

