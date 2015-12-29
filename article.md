
Intro, how they work.

## Setup

unit test

## Reporting


Cmock is designed to mock interactions, not necessarily to test if certain
functions were called.

CMock is much better at mocking return values ahead of time, not for flexibility
in verifying that functions were called.

Because you're not using the unit test framework to test for calls, you don't
get the actual line number when a call chain dies. You get the line number of
the function.

Testing sequences of calls is harder in fff... but it should be. CMock makes
this too easy in my opinion.

Had to include <string.h> to get fff to work.

Had to include <stdbool.h> directly in fff test file, rather than through header
files? Maybe a build problem.

Return data through a pointer requires a lot of knowledge of the CMock API.
With FFF, you have to do a little more work, but you just use C. Writing the
CMock version, I had to refer back to the documentation a few times, but with
FFF, I got it working on the first try. Note that the top-level CMock documentation
mentions the existence of the return_thru_ptr plugin, but not a whole lot of
help with how to use it. It's actually documented in a github issue report. I'll
leave finding it as an exercise for the reader. It's acutally a nice feature
that CMock really needs in my opinion.
