#ifndef IRENDERSURFACE
#define IRENDERSURFACE

#include <d3d9.h>

class IRenderSurface
{
public:
    virtual IDirect3DSurface9 *getD3DSurfaceHandle() = 0;
    virtual bool makeCurrent() = 0;
};

#endif // IRENDERSURFACE

