#include "grp_fileblocks.h"

#include "dal.h"
#include "core.h"
#include "fileblocks.h"
#include "bin_fileblocks.h"

#include <string.h>
#include <inttypes.h>

namespace sofs19
{
    void grpWriteFileBlock(int ih, uint32_t fbn, void *buf)
    {
        soProbe(332, "%s(%d, %u, %p)\n", __FUNCTION__, ih, fbn, buf);

         /* get file block */
        uint32_t numberBlock = sofs19::soGetFileBlock(ih,fbn);
        
        if(numberBlock == NullReference){
            /* alloc file datablock if it isn't */
            soAllocFileBlock (ih,fbn);
        }

        /* fill file with data */
        soWriteDataBlock(fbn,&buf);    

    }
};

