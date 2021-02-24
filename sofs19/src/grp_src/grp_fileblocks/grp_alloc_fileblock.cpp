#include "grp_fileblocks.h"

#include "freelists.h"
#include "dal.h"
#include "core.h"
#include "bin_fileblocks.h"

#include <errno.h>

#include <iostream>

namespace sofs19
{


    static uint32_t grpAllocIndirectFileBlock(SOInode * ip, uint32_t afbn);
    static uint32_t grpAllocDoubleIndirectFileBlock(SOInode * ip, uint32_t afbn);


    /* ********************************************************* */

    uint32_t grpAllocFileBlock(int ih, uint32_t fbn)
    {
        soProbe(302, "%s(%d, %u)\n", __FUNCTION__, ih, fbn);

        /* change the following two lines by your code */
        //return binAllocFileBlock(ih, fbn);
        SOInode* ip = soGetInodePointer(ih);
        uint32_t dblIndStart = N_INDIRECT * RPB + N_DIRECT;
        uint32_t dblIndEnd = RPB*RPB*N_DOUBLE_INDIRECT + dblIndStart;
        uint32_t allocated_block;

        if(fbn < 0 || fbn >= dblIndEnd){
            throw SOException(EINVAL, __FUNCTION__);
        }

        if(fbn<N_DIRECT){
            allocated_block = sofs19::soAllocDataBlock();
            (*ip).d[fbn] = allocated_block;
            (*ip).blkcnt +=1;
        }
        if(fbn >=N_DIRECT && fbn < dblIndStart){
            allocated_block = grpAllocIndirectFileBlock(ip,fbn - N_DIRECT);
        }
        if(fbn >=dblIndStart && fbn < dblIndEnd){
            allocated_block = grpAllocDoubleIndirectFileBlock(ip,fbn - dblIndStart);
        }
        soSaveInode(ih);
        return allocated_block;

    }

    /* ********************************************************* */

    /*
    */
    static uint32_t grpAllocIndirectFileBlock(SOInode * ip, uint32_t afbn)
    {
        soProbe(302, "%s(%d, ...)\n", __FUNCTION__, afbn);

        /* change the following two lines by your code */
        uint32_t idx_ind = afbn/RPB;
        uint32_t db[RPB];
        uint32_t allocated_block;

        for(uint32_t i=0; i<RPB;i++){
            db[i] = NullReference;
        }

        if(ip->i1[idx_ind] == NullReference){
            allocated_block= sofs19::soAllocDataBlock();
            ip->i1[idx_ind] = allocated_block;
     
            ip->blkcnt+=1;

        }
        else{
            //soReadDataBlock(ip->i1[idx_ind], &db);
            
            allocated_block = ip->i1[idx_ind];
            soReadDataBlock(allocated_block, db);
        }

        uint32_t bloco = sofs19::soAllocDataBlock();
        db[afbn%RPB] = bloco;
        ip->blkcnt+=1;

        soWriteDataBlock(allocated_block, db);
        return bloco;
        
    }


    /* ********************************************************* */


    /*
    */
    static uint32_t grpAllocDoubleIndirectFileBlock(SOInode * ip, uint32_t afbn)
    {
        soProbe(302, "%s(%d, ...)\n", __FUNCTION__, afbn);

        /* change the following two lines by your code */

        uint32_t i2idx = (afbn/ RPB)/RPB;
        uint32_t db[RPB];
        uint32_t db2[RPB];
        uint32_t allocated_block;
        uint32_t bloco;
        uint32_t bloco2;

        for(uint32_t i=0; i<RPB;i++){
            db[i] = NullReference;
            db2[i] = NullReference;
        }

        if (ip->i2[i2idx] == NullReference) {
            allocated_block = sofs19::soAllocDataBlock();

            ip->i2[i2idx] = allocated_block;
            ip->blkcnt += 1;
            bloco = soAllocDataBlock();
            db[(afbn/RPB)%RPB] = bloco;
            soWriteDataBlock(allocated_block, db);
            ip->blkcnt+=1;
            bloco2 = soAllocDataBlock();
            db2[afbn%RPB]=bloco2;
            soWriteDataBlock(bloco, db2);
            ip->blkcnt++;
        }
        else {
            allocated_block=ip->i2[i2idx];
            soReadDataBlock(allocated_block,db);

            if(db[(afbn/RPB)%RPB] == NullReference){
                bloco=soAllocDataBlock();
                db[(afbn/RPB)%RPB]=bloco;
                soWriteDataBlock(allocated_block,db);
                ip->blkcnt++;
                bloco2=soAllocDataBlock();
                db2[afbn%RPB]=bloco2;
                soWriteDataBlock(bloco,db2);
                ip->blkcnt++;
            }
            else{
                bloco=db[(afbn/RPB)%RPB];
                soReadDataBlock(bloco, db2);
                bloco2= soAllocDataBlock();
                db2[afbn%RPB]=bloco2;
                soWriteDataBlock(bloco, db2);
                ip->blkcnt++;
            }
        }
        return bloco2;
    }
};
     