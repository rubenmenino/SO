#include "grp_mksofs.h"

#include "core.h"
#include "bin_mksofs.h"

#include <inttypes.h>
#include <iostream>
#include <assert.h>

namespace sofs19
{
    void grpComputeStructure(uint32_t ntotal, uint32_t & itotal, uint32_t & nbref)
    {	
        soProbe(601, "%s(%u, %u, ...)\n", __FUNCTION__, ntotal, itotal);

        uint32_t it;        // inode table size
        //uint32_t refbl;      // free reference data blocks list table size
        //uint32_t dz_free;      // free data blocks list table size
        uint32_t dz_total;      // data blocks total size



        /* change the following line by your code */
        //binComputeStructure(ntotal, itotal, nbref);
		
		if (itotal == 0){
            itotal = ntotal/16;
        }
        if(itotal > (ntotal/8) ){
            itotal = ntotal/8;
        }
        if(itotal < IPB) {
                itotal = IPB;
        }    
        itotal = ((itotal + (IPB -1))/IPB) * IPB;


        it = itotal/IPB;
        dz_total = ntotal - it - 2;
        if(dz_total < HEAD_CACHE_SIZE) {nbref = 0;}
        else{
            dz_total = dz_total - HEAD_CACHE_SIZE;
            for(nbref=1; nbref*RPB < dz_total; nbref++) {
                if(dz_total-RPB*nbref==1){
                    itotal = itotal + IPB;
                    break;
                }
            }
        }

    }
};

