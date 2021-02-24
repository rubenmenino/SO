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
    uint32_t grpDeleteDirEntry(int pih, const char *name)
    {
        soProbe(203, "%s(%d, %s)\n", __FUNCTION__, pih, name);

        /* change the following line by your code */
        //return binDeleteDirEntry(pih, name);
        SOInode* ind  = soGetInodePointer(pih);
        SODirEntry DE[DPB];
        uint32_t tmp;

        for(uint32_t i = 0; i < (ind->size)/BlockSize; i++){
            sofs19::soReadFileBlock(pih, i, DE);
            for(uint32_t j = 0; j < DPB; j++){
                if(strcmp(DE[j].name, name) == 0){
                    memset(DE[j].name, '\0', (SOFS19_MAX_NAME));
                    tmp = DE[j].in;
                    DE[j].in = NullReference;
                    sofs19::soWriteFileBlock(pih, i, DE);
                    return tmp;
                }
            }
        }
        throw SOException(ENOENT, __FUNCTION__);
    }
};

