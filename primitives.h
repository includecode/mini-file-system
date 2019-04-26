#ifndef PRIMITIVES_H
#define PRIMITIVES_H

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "file_system.h"
ino_t mycreat(char* nom, mode_t mode);
int recup_saisi(char*** saisi_utilisateur);
ino_t myopen(char* nom, mode_t mode);
void myclose(ino_t inode);
int myread(ino_t inode, char **buffer, int nombre);
harddisk_t* init_hdd();
block_t** allouer_blocs();
void ajouter_fichier_dans_bloc(harddisk_t* hdd, t_fichier *fichier);
void ajouter_block_dans_inode(block_t* parent_block, inode_t *parent_inode);
inode_t* allouer_inode();
void chkdsk(harddisk_t *hdd);
BOOL file_exist(char* file_name, inode_t directory);
harddisk_t* boot();
t_fichier* creer_un_fichier();
#endif // PRIMITIVES_H
