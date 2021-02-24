#include "direntries.h"

#include "core.h"
#include "dal.h"
#include "fileblocks.h"
#include "bin_direntries.h"

#include <errno.h>
#include <string.h>
#include <sys/stat.h>

namespace sofs19
{
    bool grpCheckDirEmpty(int ih)
    {
        soProbe(205, "%s(%d)\n", __FUNCTION__, ih);

        /* change the following line by your code */
        //Reference to the node being handled
		SOInode* inode = soGetInodePointer(ih);
		
		uint32_t blk = (inode->size) / BlockSize;
		SODirEntry buff[DPB];

		uint32_t blkCount = 0;
		uint32_t blkNum = 0;
		uint32_t tmpBlk;

		while(blkCount < blk) {
			tmpBlk = sofs19::soGetFileBlock(ih,blkNum);

			if(tmpBlk != NullReference) {
				sofs19::soReadFileBlock(ih,blkNum,buff);
				
				for(uint32_t i = 2; i < DPB; i++) {
					if(strcmp(buff[i].name,"\0") != 0){
						return false;
					}
				}

				blkCount++;
			}

			blkNum++;
		}

		return true;
    }
};

