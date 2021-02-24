#include "grp_fileblocks.h"

#include "dal.h"
#include "core.h"
#include "fileblocks.h"
#include "bin_fileblocks.h"

#include <string.h>
#include <inttypes.h>

namespace sofs19
{
    void grpReadFileBlock(int ih, uint32_t fbn, void *buf)
    {
        soProbe(331, "%s(%d, %u, %p)\n", __FUNCTION__, ih, fbn, buf);

        uint32_t block = soGetFileBlock(ih, fbn);

        if(block == NullReference)
            memset(buf, 0, BlockSize);
        else
            soReadDataBlock(block, buf);

        // binReadFileBlock(ih, fbn, buf);
    }
};
