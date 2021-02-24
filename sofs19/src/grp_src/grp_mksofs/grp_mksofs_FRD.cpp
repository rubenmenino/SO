#include "grp_mksofs.h"

#include "rawdisk.h"
#include "core.h"
#include "bin_mksofs.h"

#include <string.h>
#include <inttypes.h>

namespace sofs19
{
    /*
       filling in the contents of the root directory:
       the first 2 entries are filled in with "." and ".." references
       the other entries are empty.
       */

    void grpFillRootDir(uint32_t itotal)
    {
        soProbe(606, "%s(%u)\n", __FUNCTION__, itotal);

        SODirEntry direntries[DPB];

        uint32_t numInodeBlocks = itotal / IPB;
        uint32_t firstBlock = 1 + numInodeBlocks;

        // "limpar o array" colocar todos os campos a 0
        memset(direntries, 0, BlockSize);

        strcpy(direntries[0].name, ".");
        strcpy(direntries[1].name, "..");

        // Restantes campos com NullReference
        for(uint32_t i = 2; i < DPB; i++) {
            direntries[i].in = NullReference;
        }

        soWriteRawBlock(firstBlock, direntries);

        // return binFillRootDir(itotal);
    }
};
