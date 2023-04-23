#include <cstdlib>
#include "Map.h"
#include "internal.h"
#include "loi2proba.h"
#include "p6/p6.h"
#define DOCTEST_CONFIG_IMPLEMENT
#include "doctest/doctest.h"

int main(int argc, char* argv[])
{
    { // Run the tests
        if (doctest::Context{}.run() != 0)
            return EXIT_FAILURE;
        // The CI does not have a GPU so it cannot run the rest of the code.
        const bool no_gpu_available = argc >= 2 && strcmp(argv[1], "-nogpu") == 0; // NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic)
        if (no_gpu_available)
            return EXIT_SUCCESS;
    }

    // Actual app
    auto ctx = p6::Context{{.title = "Simple-p6-Setup"}};
    ctx.maximize_window();

    Map map(ctx.aspect_ratio());

    // Declare your infinite update loop.
    ctx.update = [&]() {
        ctx.background(p6::Color(39, 110, 188));
        map.update(ctx);
        map.draw(ctx);
        // std::cout << X1(9) << std::endl;
        // std::cout << fact(5) << std::endl;
        // std::cout
        //     << X2(2.5) << std::endl;
    };

    // Should be done last. It starts the infinite loop.
    ctx.start();
}