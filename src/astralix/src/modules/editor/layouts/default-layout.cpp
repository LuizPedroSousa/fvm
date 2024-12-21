#include "layouts/default-layout.hpp"

namespace astralix
{
  DefaultLayout::DefaultLayout() : Layout(
    "Default Layout",
    R"(
[Window][DockSpace]
Pos=0,0
Size=2560,1011
Collapsed=0

[Window][Debug##Default]
Pos=60,60
Size=400,400
Collapsed=0

[Window][Scene Hierarchy]
Pos=0,24
Size=360,824
Collapsed=0

[Window][Viewport]
Pos=362,24
Size=1836,824
Collapsed=0

[Window][Console]
Pos=0,850
Size=2198,161
Collapsed=0

[Window][Content Browser]
Pos=0,850
Size=2198,161
Collapsed=0

[Window][Properties]
Pos=2200,24
Size=360,987
Collapsed=0
    )"
  ) {
  }
} // namespace astralix
