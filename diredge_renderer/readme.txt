# University of Leeds 2024-25
# COMP 5893M Assignment 1 Triangle Soup Renderer

This is a very simple renderer for triangle soup. It makes no attempt to be efficient, which keeps the code clearer.  It should work with either QT 5 or QT 6.

UNIVERSITY LINUX:
=================

To compile on the University Linux machines, you will need to do the following:

[userid@machine diredge_renderer]$ qmake
[userid@machine diredge_renderer]$ make

To execute the renderer, pass the file name on the command line:

[userid@machine diredge_renderer]$ ./diredge_renderer ../handout_models/filename.tri ../output_models/filename.diredge

#define DIREDGE 1:
==================

This is a guard which disables/enables parts of the original triangle renderer.
If the guard is 0 then all of the new directed edge renderer code should be disabled and the
og triangle renderer should run as normal.
