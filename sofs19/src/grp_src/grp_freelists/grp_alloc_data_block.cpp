/*
 *  \author AntÃ³nio Rui Borges - 2012-2015
 *  \authur Artur Pereira - 2009-2019
 */

#include "grp_freelists.h"

#include <stdio.h>
#include <errno.h>
#include <inttypes.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>

#include "core.h"
#include "dal.h"
#include "freelists.h"
#include "bin_freelists.h"

namespace sofs19
{
    uint32_t grpAllocDataBlock()
    {
        soProbe(441, "%s()\n", __FUNCTION__);

            SOSuperBlock *sbPointer = soGetSuperBlockPointer();
 

            if(sbPointer -> dz_free == 0){
                throw SOException(ENOSPC,__FUNCTION__);
            }

            if(sbPointer -> head_cache.idx == HEAD_CACHE_SIZE){
                sofs19::soReplenishHeadCache();
            }

            uint32_t idx = sbPointer -> head_cache.idx;
            uint32_t bap = sbPointer -> head_cache.ref[idx];
            sbPointer -> head_cache.ref[idx] = NullReference;
            sbPointer -> head_cache.idx = idx + 1;

            sbPointer -> dz_free--;

            soSaveSuperBlock();
        
            return bap;
    }
}
