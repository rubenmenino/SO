#include "grp_mksofs.h"

#include "rawdisk.h"
#include "core.h"
#include "bin_mksofs.h"

#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/stat.h>
#include <inttypes.h>

namespace sofs19
{
    void grpFillInodeTable(uint32_t itotal, bool set_date)
    {
        soProbe(604, "%s(%u)\n", __FUNCTION__, itotal);

        uint32_t totalBlocks = itotal / IPB;
        SOInode inodes[IPB];
        uint32_t next = 2;

        for(uint32_t block = 1; block < totalBlocks + 1; block++) {
            for(uint32_t i = 0 ; i < IPB ; i++) {
                // Para o inode 0 (directory)
                if(i == 0 && block == 1) {
                    inodes[0].mode = S_IFDIR;
                    inodes[0].lnkcnt = 2;
                    inodes[0].owner = getuid();
                    inodes[0].group = getgid();
                    inodes[0].size = BlockSize;
                    inodes[0].blkcnt = 1;
                    inodes[0].d[0] = 0;

                    if(set_date == true) {
                        inodes[0].atime = time(NULL);
                        inodes[0].mtime = time(NULL);
                        inodes[0].ctime = time(NULL);
                    }
                    else {
                        inodes[0].atime = 0;
                        inodes[0].mtime = 0;
                        inodes[0].ctime = 0;
                    }

                    for(int i = 0; i < N_DOUBLE_INDIRECT; i++)
                        inodes[0].i2[i] = NullReference;
                    for(int i = 1; i < N_DIRECT; i++)
                        inodes[0].d[i] = NullReference;
                    for(int i = 0; i < N_INDIRECT; i++)
                        inodes[0].i1[i] = NullReference;

                }
                else {
                    inodes[i].mode = INODE_FREE;
                    inodes[i].lnkcnt = 0;
                    inodes[i].owner = 0;
                    inodes[i].group = 0;
                    inodes[i].size = 0;
                    inodes[i].blkcnt = 0;
                    inodes[i].atime = 0;
                    inodes[i].mtime = 0;
                    inodes[i].ctime = 0;
                    inodes[i].next = next;
                    next++;

                    for(int j = 0; j < N_DOUBLE_INDIRECT; j++)
                        inodes[i].i2[j] = NullReference;
                    for(int j = 0; j < N_DIRECT; j++)
                        inodes[i].d[j] = NullReference;
                    for(int j = 0; j < N_INDIRECT; j++)
                        inodes[i].i1[j] = NullReference;
                }
            }
            soWriteRawBlock(block, &inodes);
        }
        // return binFillInodeTable(itotal, set_date);
    }
};
