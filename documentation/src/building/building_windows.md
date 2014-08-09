# Building for Windows

MathFu releases include a [Visual Studio][] solution
(`MathFu/Unittests/MathFu.sln`) and project files to build the tests
on [Windows][].  The Visual Studio solution has been generated using
[cmake][], and is free of any host-specific dependencies.  It has been
tested with Microsoft [Visual Studio][] 2012.

### Version Requirements

Following are the minimum required versions for the tools and libraries you
need for building LiquidFun for Windows:

-   Windows: 7
-   Visual Studio: 2012
-   cmake: 2.8.12.1

### Building with [Visual Studio][]

-   Double-click on `MathFu/MathFu.sln` to open the solution.
-   Select "Build-->Build Solution" from the menu.

### Building using [cmake][]

When working directly with the source, use [cmake][] to generate the
[Visual Studio][] solution and project files.  For example, the following
generates the [Visual Studio][] solution in the `MathFu/build` directory:

    cd MathFu\build
    cmake -G "Visual Studio 11" ..

Running [cmake][] under [cygwin][] requires empty TMP, TEMP, tmp and temp
variables.  To generate a [Visual Studio][] from a [cygwin][] bash shell use:

    $ cd MathFu/build
    $ ( unset {temp,tmp,TEMP,TMP} ; cmake -G "Visual Studio 11" .. )

### Running Unit Tests

Developers modifying the LiquidFun library can verify that it is still working
correctly by executing the provided unit tests.

Use the run\_tests.bat batch file to execute unit tests:

    MathFu\Unittests\run_tests.bat

  [cmake]: http://www.cmake.org
  [Visual Studio]: http://www.visualstudio.com/
  [Windows]: http://windows.microsoft.com/
  [cygwin]: http://www.cygwin.com/
