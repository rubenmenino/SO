/*
 *  \author António Rui Borges - 2012-2015
 *  \authur Artur Pereira - 2016-2019
 */

#include "grp_freelists.h"

#include <string.h>
#include <errno.h>
#include <iostream>

#include "core.h"
#include "dal.h"
#include "freelists.h"
#include "bin_freelists.h"

namespace sofs19
{
    void grpReplenishHeadCache(void)
    {
        soProbe(443, "%s()\n", __FUNCTION__);

        SOSuperBlock *superBlock = soGetSuperBlockPointer();

        if(superBlock->head_cache.idx != HEAD_CACHE_SIZE ) {
            return;
        }
        /* change the following line by your code */
        binReplenishHeadCache();
    }
};
