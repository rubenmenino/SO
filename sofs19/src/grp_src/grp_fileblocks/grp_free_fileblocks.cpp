#include "grp_fileblocks.h"

#include "freelists.h"
#include "dal.h"
#include "core.h"
#include "bin_fileblocks.h"

#include <inttypes.h>
#include <errno.h>
#include <assert.h>
#include <cmath>

namespace sofs19
{

    /* free all blocks between positions ffbn and RPB - 1
     * existing in the block of references given by i1.
     * Return true if, after the operation, all references become NullReference.
     * It assumes i1 is valid.
     */
    static bool grpFreeIndirectFileBlocks(SOInode * ip, uint32_t i1, uint32_t ffbn);

    /* free all blocks between positions ffbn and RPB**2 - 1
     * existing in the block of indirect references given by i2.
     * Return true if, after the operation, all references become NullReference.
     * It assumes i2 is valid.
     */
    static bool grpFreeDoubleIndirectFileBlocks(SOInode * ip, uint32_t i2, uint32_t ffbn);


    /* ********************************************************* */

    void grpFreeFileBlocks(int ih, uint32_t ffbn)
    {
        soProbe(303, "%s(%d, %u)\n", __FUNCTION__, ih, ffbn);
        uint32_t bn = grpGetFileBlock(ih, ffbn);

        uint32_t buffer[RPB];
            
        if(ffbn < N_DIRECT){
	        soReadDataBlock(bn, buffer);
	        for(uint32_t i=0; i<RPB; i++){
	            if(buffer[i] != NullReference){
	                buffer[i] = NullReference;
	            }
	        }
	            soWriteDataBlock(bn, buffer);
        	}
            else if (ffbn < (N_DIRECT + N_INDIRECT*RPB)){
        		SOInode* ip = soGetInodePointer(ih);

        		if(grpFreeIndirectFileBlocks(ip, bn, ffbn)){
        		    bn = NullReference;
        		}
        	}
            else{
        		SOInode* ip = soGetInodePointer(ih);
        		if(grpFreeDoubleIndirectFileBlocks(ip, bn, ffbn)){
        			bn = NullReference;
        		}
        	}
	        soSaveSuperBlock();
        /* change the following line by your code
        *binFreeFileBlocks(ih, ffbn);
        */
    }

    /* ********************************************************* */

//if false
    static bool grpFreeIndirectFileBlocks(SOInode * ip, uint32_t i1, uint32_t ffbn)
    {
        soProbe(303, "%s(..., %u, %u)\n", __FUNCTION__, i1, ffbn);

        bool allNull= false;
        uint32_t bn;
        uint32_t buffer[RPB];

        for(uint32_t i=0; i<N_INDIRECT; i++){
            bn = grpGetFileBlock(ip->i1[i], ffbn);
            allNull = false;
            soReadDataBlock(bn, buffer);

            for(uint32_t j=0; j<RPB; j++){
                if(buffer[i] != NullReference){
                    buffer[i] = NullReference;
                }
            }
            soWriteDataBlock(bn, buffer);
            allNull = true;
        }

            soSaveSuperBlock();
            return allNull;

        // change the following line by your code
        //throw SOException(ENOSYS, __FUNCTION__);
    }
//endif

    /* ********************************************************* */

//if false
    static bool grpFreeDoubleIndirectFileBlocks(SOInode * ip, uint32_t i2, uint32_t ffbn)
    {
        soProbe(303, "%s(..., %u, %u)\n", __FUNCTION__, i2, ffbn);

        bool allNull= false;
        uint32_t bn;
        uint32_t tmp = pow(RPB, 2);
        uint32_t buffer[tmp];

        for(uint32_t i=0; i<N_DOUBLE_INDIRECT; i++){
            bn = grpGetFileBlock(ip->i2[i], ffbn);
            allNull = false;

            soReadDataBlock(bn, buffer); 
            for(uint32_t j=0; j<tmp; j++){
                if(buffer[i] != NullReference){
                    buffer[i] = NullReference;
                }
            }
        
            soWriteDataBlock(bn, buffer);
            allNull = true;
        }
            
            soSaveSuperBlock();
            return allNull;


        // change the following line by your code 
        throw SOException(ENOSYS, __FUNCTION__); 
        
    }
//endif

    /* ********************************************************* */
};
