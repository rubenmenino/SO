#include "direntries.h"

#include "core.h"
#include "dal.h"
#include "fileblocks.h"
#include "bin_direntries.h"

#include <string.h>
#include <errno.h>
#include <sys/stat.h>

namespace sofs19
{
    void grpRenameDirEntry(int pih, const char *name, const char *newName)
    {
        soProbe(204, "%s(%d, %s, %s)\n", __FUNCTION__, pih, name, newName);

        SOInode* inodePointer = soGetInodePointer(pih);

        if(!strcmp(newName, " ")){
            throw SOException(EINVAL, __FUNCTION__);
        }

        if(strlen(newName) > SOFS19_MAX_NAME){
            throw SOException(ENOTDIR, __FUNCTION__);
        }

        if(!S_ISDIR(inodePointer -> mode)){
            throw SOException(ENOTDIR, __FUNCTION__);
        }

        SOInode* newPointer = soGetInodePointer(pih);
        int rename = -1;
        int renameIndex = -1;
        SODirEntry renameSlotBlock[DPB];

        SODirEntry d[DPB];
        uint32_t i = 0;

        for (; i < (newPointer -> size / BlockSize); i++){
            sofs19::soReadFileBlock(pih, i, d);

            uint32_t j = 0;
            for(; j < DPB; j++){
                if(rename < 0 && strcmp(d[j].name, name) == 0){
                    sofs19::soReadFileBlock(pih, i, renameSlotBlock);
                    renameIndex = i;
                    rename = j;
                }

                if(strcmp(d[j].name, newName) == 0){
                    throw SOException(EEXIST, __FUNCTION__);
                }
            }
        }

        if (rename >= 0){
            memcpy(renameSlotBlock[rename].name, name, SOFS19_MAX_NAME+1);
            sofs19::soWriteFileBlock(pih, renameIndex, renameSlotBlock);
        }

        if(rename < 0){
            throw SOException(ENOENT, __FUNCTION__);
        }
    }


        /* change the following line by your code
        *binRenameDirEntry(pih, name, newName);
        */
}