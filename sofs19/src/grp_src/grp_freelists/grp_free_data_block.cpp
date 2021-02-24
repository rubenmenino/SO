/*
 *  \author António Rui Borges - 2012-2015
 *  \authur Artur Pereira - 2016-2019
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
    void grpFreeDataBlock(uint32_t bn)
    {
        soProbe(442, "%s(%u)\n", __FUNCTION__, bn);

        SOSuperBlock *sbPointer = soGetSuperBlockPointer();
 
        // cache full?
        if(sbPointer -> head_cache.idx == HEAD_CACHE_SIZE){
                sofs19::soReplenishHeadCache();
        }

        // add value to cache
        sbPointer -> head_cache.ref[sbPointer->head_cache.idx] = bn;

        // update idx
        sbPointer -> head_cache.idx++;

        // update DZ
        sbPointer -> dz_free++;

        soSaveSuperBlock();



        /* change the following line by your code
        * binFreeDataBlock(bn);
        */
    }
};