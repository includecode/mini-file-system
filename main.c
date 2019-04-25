#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "primitives.h"
#include "file_system.h"

int recup_saisi(char*** saisi_utilisateur);
ino_t mycreat(char* nom, mode_t mode);
ino_t myopen(char* nom, mode_t mode);
void myclose(ino_t inode);
int myread(ino_t inode, char **buffer, int nombre);
int myread(ino_t inode, char **buffer, int nombre);

int main(int nbarg, char* argv[])
{

harddisk_t* hdd = init_hdd();
    t_fichier *fichier = creer_un_fichier(hdd);
    inode_t *inode = allouer_inode(fichier, hdd);
    hdd->tab_blocs = allouer_blocs(hdd);
    ajouter_fichier_dans_bloc(hdd, fichier);

    t_fichier *fichier2 = creer_un_fichier(hdd);
    inode_t *inode2 = allouer_inode(fichier2, hdd);
    ajouter_fichier_dans_bloc(hdd, fichier2);
    return 0;
    /*
    
    int i,nbr_mot=0;
    char** saisi_utilisateur=(char**)malloc(sizeof(char*));
    
    saisi_utilisateur[0]=(char*)malloc(sizeof(char));
    
    do
    {
        nbr_mot=recup_saisi(&saisi_utilisateur);
        
        if(strstr(saisi_utilisateur[0],"touch"))
        {
            for(i=1;i<nbr_mot;i++)
                printf("nouveau inode %llu\n",mycreat(saisi_utilisateur[i],S_IROTH | S_IWUSR | S_IRUSR | S_IRGRP | S_IFREG));
        }
        else if(strstr(saisi_utilisateur[0],"open"))
        {
            for(i=1;i<nbr_mot;i++)
                printf("inode %llu\n",myopen(saisi_utilisateur[i],S_IROTH | S_IWUSR | S_IRUSR | S_IRGRP | S_IFREG));
        }
        else if(strstr(saisi_utilisateur[0],"close"))
        {
            for(i=1;i<nbr_mot;i++)
                myclose(myopen(saisi_utilisateur[i],S_IROTH | S_IWUSR | S_IRUSR | S_IRGRP | S_IFREG));
        }
    }while(!strstr(saisi_utilisateur[0],"quitter"));*/
}



