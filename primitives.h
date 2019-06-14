#ifndef PRIMITIVES_H
#define PRIMITIVES_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "file_system.h"
#include <dirent.h>
inode_t* mycreat(char* nom, char mode[12]);
void mylink(char* nom1, char* nom2);
void mywrite(char * text, t_fichier *file);
void myread(t_fichier *file);
int recup_saisi(char*** saisi_utilisateur);
t_fichier* myopen(char* nom, mode_tt ouverture);
void myclose(t_fichier *file);
void init_hdd();
void allouer_blocs();
void ajouter_fichier_dans_bloc(int size_file);
void chkdsk();
t_fichier*  file_exist(char* file_name, t_fichier *directory);
BOOL bloc_plein(int size_file);
void boot();
harddisk_t hdd;
void mymkdir(char* nom);
void creer_racine_sgf();
void ls();
BOOL existance_fichier(char *name);
int position_rep(char* name);
void cd(char* name);
void mycp(char *nom1, char *nom2);
block_t retourne_bloc(int num_fichier);
t_fichier *url_exists(char* string, char path_type);
char *explode_last_from_path(char* string);
BOOL begins_with_root_folder(char *path);
void roll_back_prompt_once();
void myrm(char *nom);
void myrmdir(char *nom);
void mymv(char* nom1, char* nom2);
void echo(char** saisi_utilisateur,int nbr_mot);
void cat(char *nom);
void saveHDD();
BOOL check_current_dir(char* nom, t_fichier **newCurrentDir, char** name);
void pwd();
#endif // PRIMITIVES_H
