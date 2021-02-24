// pih = parent inode handler
// for { read file block }
// Complexidade 3

#include "direntries.h"

#include "core.h"
#include "dal.h"
#include "fileblocks.h"
#include "bin_direntries.h"

#include <errno.h>
#include <string.h>
#include <sys/stat.h>

namespace sofs19
{
    uint32_t grpGetDirEntry(int pih, const char *name)
    {
        soProbe(201, "%s(%d, %s)\n", __FUNCTION__, pih, name);

        SOInode* inodePointer = soGetInodePointer(pih);
        SODirEntry direntries[DPB];

        if (strcmp(name, "") == 0)
            throw SOException(EINVAL, __FUNCTION__);


        for (uint32_t i = 0; i <= inodePointer->size / BlockSize; i++) {
            soReadFileBlock(pih, i, direntries);
            for (uint32_t j = 0; j < DPB; j++) {
                if (strcmp(direntries[j].name, name) == 0)
                    return direntries[j].in;
            }
        }
        return -1;
        // return binGetDirEntry(pih, name);
    }
};
