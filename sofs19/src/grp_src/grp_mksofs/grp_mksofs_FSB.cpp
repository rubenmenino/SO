// 3
#include "grp_mksofs.h"

#include "rawdisk.h"
#include "core.h"
#include "bin_mksofs.h"

#include <string.h>
#include <inttypes.h>

namespace sofs19
{
    void grpFillSuperBlock(const char *name, uint32_t ntotal, uint32_t itotal, uint32_t nbref)
    {
        soProbe(602, "%s(%s, %u, %u, %u)\n", __FUNCTION__, name, ntotal, itotal, nbref);

        // Header    
        SOSuperBlock sbp;
        sbp.magic = 0xFFFF;
        sbp.version = VERSION_NUMBER;
        strncpy(sbp.name,name,PARTITION_NAME_SIZE + 1);
        sbp.ntotal = ntotal;
        sbp.mntcnt = 0;
        sbp.mntstat = 1;
       

        // Inodes metadata
        sbp.it_size = itotal/IPB; // nº de blocos inode table
        sbp.ifree = itotal-1; // nº free inodes
        sbp.itotal = itotal;
        sbp.ihead = 1;
        sbp.itail = itotal-1;

        sbp.dz_start = (itotal/IPB)+1;
        sbp.dz_free = sbp.dz_total-nbref-1;
        sbp.dz_total = ntotal-sbp.dz_start;
       

        if(sbp.dz_free >= HEAD_CACHE_SIZE){
            sbp.head_cache.idx = 0;
        }else{
            sbp.head_cache.idx = HEAD_CACHE_SIZE - sbp.dz_free;
        }


        if(sbp.dz_free > HEAD_CACHE_SIZE){
            sbp.head_idx = 1;
            sbp.head_blk = 1;
            sbp.tail_blk = nbref;
            uint32_t occupied = sbp.dz_free - (HEAD_CACHE_SIZE + (nbref - 1) * RPB) + (nbref - 1);
            sbp.tail_idx = occupied + 1;
        }else{
            sbp.head_blk = NullReference;
            sbp.head_idx = NullReference;
            sbp.tail_blk = NullReference;
            sbp.tail_idx = NullReference;
        }
       
       
        // cache 
        uint32_t blockk = nbref+1;
        for (int i = 0; i < HEAD_CACHE_SIZE; i++){
            if(i < sbp.head_cache.idx){
                sbp.head_cache.ref[i] = NullReference;
            }else{
                sbp.head_cache.ref[i] = blockk;
                blockk++;
            }
        }

       sbp.tail_cache.idx = 0;
       for (int i = 0; i < TAIL_CACHE_SIZE; i++){
            sbp.tail_cache.ref[i] = NullReference;
       }
       

        soWriteRawBlock(0,&sbp);



    }
};