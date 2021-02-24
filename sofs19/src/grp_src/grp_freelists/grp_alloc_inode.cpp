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
#include <sys/stat.h>
#include <string.h>

#include <iostream>

#include "core.h"
#include "dal.h"
#include "freelists.h"
#include "bin_freelists.h"

namespace sofs19
{
    uint32_t grpAllocInode(uint32_t type, uint32_t perm)
    {
        soProbe(401, "%s(0x%x, 0%03o)\n", __FUNCTION__, type, perm);

        /* change the following line by your code */
        //return binAllocInode(type, perm);
        SOSuperBlock *sb = soGetSuperBlockPointer();
			
		if(type != S_IFREG && type != S_IFDIR && type != S_IFLNK){
			throw SOException(EINVAL,__FUNCTION__); 
		}

		if(sb->ifree <= 0){
			throw SOException(ENOSPC,__FUNCTION__);
		}

		if(sb->head_cache.idx == HEAD_CACHE_SIZE){
			sofs19::soReplenishHeadCache(); 
		}
		if(perm < 0000 || perm > 0777){
			throw SOException(EINVAL,__FUNCTION__); 
		}
					
		SOSuperBlock::HeadCache cache = sb->head_cache;
			
		uint32_t inoderef = sb -> ihead;
			
		int inode_Handler = soOpenInode(inoderef);
		SOInode* in = soGetInodePointer(inode_Handler);

		sb->head_cache.ref[cache.idx] = NullReference;
		sb->head_cache.idx += 1;
		sb->ifree -= 1;
		if(sb->ifree == 0){
        	sb->ihead = NullReference;
		}
    	else{
         	sb->ihead = in->next;
    	}
    	soSaveSuperBlock();

		in->mode = type | perm;
		in->atime = time(NULL);
		in->mtime = time(NULL);
		in->ctime = time(NULL);
		in->owner = getuid();
		in->group = getgid();

		soSaveInode(inode_Handler);
		soCloseInode(inode_Handler);

		soSaveSuperBlock();
					
		return inoderef;	

    }
};

