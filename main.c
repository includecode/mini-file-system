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
    //Dans le boot() 2 fichiers (repertoires) sont crées sur l'inode N° 0 ( . et root)
    harddisk_t* hdd = boot();


    //TEST DE LA CREATION D'UN FICHIER // ce fichier sera le 3ieme fichier de l'inode 0 et ainsi de suite jusu'a
    //ce que la taille de l'inode 0 soit totlament occupée ensuite l'inode 1, 2.. 
    t_fichier *fichier3 = creer_un_fichier(hdd);
    fichier3->nom = "Un_ficier";
    inode_t *inode3 = allouer_inode(fichier3, hdd);
    ajouter_fichier_dans_bloc(hdd, fichier3);
    
    //*****************TEST DE LA FONCTION DE RECHERCHE D'UN FICHIER
    printf("\n\nTEST DE LA FONCTION DE RECHERCHE D'UN FICHIER\n");
    if(file_exist("Un_ficier", &(hdd->current_dir))){
        printf("existe\n");
        //stuff
    }else
    {
        printf("n'existe pas\n");
    }

    //*****************TEST DE LA FONCTION chkdsk
    printf("\n\nTEST DE LA FONCTION chkdsk\n");
    chkdsk(hdd);
    
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



