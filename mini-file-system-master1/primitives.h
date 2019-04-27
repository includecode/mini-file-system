#ifndef PRIMITIVES_H
#define PRIMITIVES_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "file_system.h"
inode_t mycreat(char* nom, char mode[12]);
int recup_saisi(char*** saisi_utilisateur);
inode_t myopen(char* nom, char mode[12]);
void myclose(ino_t inode);
int myread(ino_t inode, char **buffer, int nombre);
void init_hdd();
void allouer_blocs();
void ajouter_fichier_dans_bloc(int size_file);
void chkdsk();
BOOL file_exist(char* file_name, t_fichier directory);
BOOL bloc_plein(int size_file);
void boot();
harddisk_t hdd;
void mymkdir(char* nom);
void creer_racine_sgf();
void ls();
#endif // PRIMITIVES_H
