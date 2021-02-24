/*
 *  \author António Rui Borges - 2012-2015
 *  \authur Artur Pereira - 2016-2019
 */

#include "grp_freelists.h"

#include "core.h"
#include "dal.h"
#include "freelists.h"
#include "bin_freelists.h"

#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <iostream>
using namespace std;

namespace sofs19
{
    /* only fill the current block to its end */
    void grpDepleteTailCache(void)
    {
        soProbe(444, "%s()\n", __FUNCTION__);
        
        SOSuperBlock* sbPointer= soGetSuperBlockPointer();
        
        // cache not full
        if (sbPointer -> tail_cache.idx != TAIL_CACHE_SIZE){
            return;
        }

        uint32_t tail_blk = sbPointer -> tail_blk;
        uint32_t tail_idx = sbPointer -> tail_idx;
        uint32_t head_blk = sbPointer -> head_blk;
        uint32_t head_idx = sbPointer -> head_idx;


        // no tail reference data block
        if (tail_blk == NullReference){

            uint32_t bb = soAllocDataBlock();
            uint32_t ref[RPB];

            soReadDataBlock(bb, ref);

            memset(&ref, 
                   NullReference, sizeof(ref));
            tail_idx = 1; 
            head_blk = bb;
            tail_blk = bb; 
            head_idx = 1;
            
            soWriteDataBlock(bb, ref);

        // when tail reference data block is full
        } else if (tail_idx == RPB){
                uint32_t bb = soAllocDataBlock();
                uint32_t ref[RPB];

                soReadDataBlock(tail_blk, ref);

                ref[0]= bb;

                soWriteDataBlock(tail_blk, ref);

                tail_blk = bb; 
                memset(&ref, 
                       NullReference, 
                       sizeof(ref));

                soWriteDataBlock(bb,ref);

                tail_idx = 1;
        }

        uint32_t disponible = RPB - tail_idx;
        // Tail cache will not be empty
        if (disponible < TAIL_CACHE_SIZE){
            uint32_t ref[RPB];
            uint32_t notEmptyRefs= TAIL_CACHE_SIZE - disponible; 

            soReadDataBlock(tail_blk,ref);

            memcpy (&(ref[tail_idx]), &(sbPointer->tail_cache.ref[0]),
                    disponible * sizeof(uint32_t));


            memcpy (&(sbPointer->tail_cache.ref[0]) ,&(sbPointer->tail_cache.ref[disponible]), 
                    notEmptyRefs * sizeof(uint32_t));


            memset(&(sbPointer->tail_cache.ref[notEmptyRefs]), NullReference, 
                   (TAIL_CACHE_SIZE-notEmptyRefs) * sizeof(uint32_t));


            soWriteDataBlock(tail_blk,ref);


            tail_idx= RPB;
            sbPointer->tail_cache.idx= notEmptyRefs;

        } else {
            uint32_t ref[RPB];
            soReadDataBlock(tail_blk,ref);

            memcpy (&(ref[tail_idx]), &(sbPointer->tail_cache.ref[0]), 
                    TAIL_CACHE_SIZE * sizeof(uint32_t));


            memset(&(sbPointer->tail_cache.ref[0]), NullReference, TAIL_CACHE_SIZE * sizeof(uint32_t));


            soWriteDataBlock(tail_blk,ref);


            tail_idx+= TAIL_CACHE_SIZE; 
            sbPointer->tail_cache.idx= 0;
        }

        soSaveSuperBlock();
    }
};