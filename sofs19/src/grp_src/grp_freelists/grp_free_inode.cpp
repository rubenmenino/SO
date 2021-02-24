// 2/3


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
    void grpFreeInode(uint32_t in)
    {
        soProbe(402, "%s(%u)\n", __FUNCTION__, in);

        /* change the following line by your code */
        //binFreeInode(in);

        /*Coloca na lista de freeInodes
                * quero libertar o 100 -> o último que era o 15 tem nullReference -> passar para 100 
                * o 100 ficar com a referencia do nextNode para nullRefrence
                * o iTail que anteriormente estava a 15 colocar a 100
                * 
                * para abir o inode fazer -> soOpenInode, recebe o inodeHandler
                * depois chamar soGetInodePointer() e ficar com o ponteiro para o inode
                * só depois mexer no inode
                * */

                // cria ponteiro do SuperBlock
                SOSuperBlock* sbp = soGetSuperBlockPointer();

        // se a tail cache estiver cheia chama a depleteTaiCache 
                if(sbp -> tail_cache.idx == TAIL_CACHE_SIZE){
                        sofs19::soDepleteTailCache();
                }

                // o índice do inode que é para libertar (in) é o que terá 'tail_cahce.idx++' index
                sbp -> tail_cache.ref[sbp -> tail_cache.idx++] = in;

                // cria o inodeHandler sabendo o inode (in)
                int ih = soOpenInode(in);
                // cria ponteiro para mexer no Inode em questão através do inodeHandler
                SOInode* node = soGetInodePointer(ih);

                // coloca os dados do inode a libertar
                node -> mode = INODE_FREE;
                node -> lnkcnt = 0;
                node -> owner = 0;
                node -> group = 0;
                node -> size = 0;
                node -> blkcnt = 0;

                // atualiza o ponteiro do superBlock para ifree++ (inodes free)  
                sbp -> ifree++;

                // guardar alterações
                soSaveSuperBlock();
                soSaveInode(ih);
                soCloseInode(ih);

    }
};

