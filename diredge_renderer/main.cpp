///////////////////////////////////////////////////
//
//	Hamish Carr
//	January, 2018
//
//	------------------------
//	main.cpp
//	------------------------
//
///////////////////////////////////////////////////

#include "GeometricWidget.h"
#include <QtWidgets/QApplication>
#include <stdio.h>

#include "Defines.h"

int main(int argc, char **argv) { // main()
    // initialize QT
    QApplication app(argc, argv);

    // the geometric surface
    GeometricSurfaceFaceDS surface;

#if !DIREDGE // !DIREDGE
    // check the args to make sure there's an input file
    if (argc == 2) { // two parameters - read a file
        if (!surface.ReadFileTriangleSoup(argv[1])) { // surface read failed
            printf("Read failed for file %s\n", argv[1]);
            exit(0);
        } // surface read failed
#else          // !DIREDGE
    // check the args to make sure there's an input file
    if (argc == 3) { // three parameters - original .tri file and .diredge file
        if (!surface.ReadFileTriangleSoup(argv[1])) { // surface read failed
            printf("Read failed for file %s\n", argv[1]);
            std::exit(EXIT_SUCCESS);
        } // surface read failed

        if (!surface.ReadDirectedEdgeFile(argv[2])) {
            printf("Read failed for file %s\n", argv[2]);
            std::exit(EXIT_SUCCESS);
        }

        // NOTE: More validation could be added here. E.g., by adding
        // indirection to indexing into the face vertex index array. This could
        // be done by indexing into the otherHalf array and then using those
        // indices to index into the face vertex index array. Therefore
        // cross-checking where indices point to.
        if (!surface.ValidateTriangleSoup()) {
            std::cerr << "Validation of directed edge file failed. Triangle "
                         "soup generated from vertices and faces does not "
                         "match original .tri file.\n";
            std::exit(EXIT_SUCCESS);
        }
#endif         // !DIREDGE
        else { // surface read succeeded
            //	create a window
            GeometricWidget aWindow(&surface, NULL);

            // 	set the initial size
            aWindow.resize(600, 600);

            // show the window
            aWindow.show();

            // set QT running
            return app.exec();
        } // surface read succeeded
    } // two parameters - read a file
    else {  // too many parameters
#if DIREDGE // DIREDGE
        printf("Usage: %s filename.tri filename.diredge\n", argv[0]);
#else  // DIREDGE
        printf("Usage: %s filename\n", argv[0]);
#endif // DIREDGE
        exit(0);
    } // too many parameters

    // paranoid return value
    exit(0);
} // main()
