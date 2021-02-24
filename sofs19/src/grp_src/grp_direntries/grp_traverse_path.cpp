// 5
#include "direntries.h"

#include "core.h"
#include "dal.h"
#include "fileblocks.h"
#include "direntries.h"
#include "bin_direntries.h"

#include <errno.h>
#include <string.h>
#include <libgen.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>

namespace sofs19
{
    uint32_t grpTraversePath(char *path)
    {
        soProbe(221, "%s(%s)\n", __FUNCTION__, path);

        if(strcmp(path, "/") == 0){
            return 0;
        }

        char *pathCopy = strdupa(path);
        char *base = basename(pathCopy);
        char *directory = dirname(pathCopy);

        uint32_t xac = soOpenInode(soTraversePath(directory));
        SOInode *ip = soGetInodePointer(xac);

        /* check this conditions */
        if(strlen(path) < 1 || path[0] != '/'){
            throw SOException(EINVAL, __FUNCTION__);
        }

        if ((ip -> mode & S_IFDIR) != S_IFDIR){
            throw SOException(ENOENT,__FUNCTION__);
        }

        /////////////////////////////////////////////

        if(!(sofs19::soCheckInodeAccess(xac,X_OK))){
            throw SOException(EACCES,__FUNCTION__);
        }

        uint32_t x = soGetDirEntry(xac,base);

        if((int)x > 0){
            throw SOException(ENOENT, __FUNCTION__);
        }

        soCloseInode(xac);


        return x;

        

    
    }
};