#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int recup_saisi(char*** saisi_utilisateur);
ino_t mycreat(char* nom, mode_t mode);

int main(int nbarg, char* argv[])
{
    int i,nbr_mot=0;
    char** saisi_utilisateur=(char**)malloc(sizeof(char*));
    
    saisi_utilisateur[0]=(char*)malloc(sizeof(char));
    
    do
    {
        nbr_mot=recup_saisi(&saisi_utilisateur);
        
        if(strstr(saisi_utilisateur[0],"touch"))
            for(i=1;i<nbr_mot;i++)
                printf("nouveau inode %llu\n",mycreat(saisi_utilisateur[i],S_IROTH | S_IWUSR | S_IRUSR | S_IRGRP | S_IFREG));
    }while(!strstr(saisi_utilisateur[0],"quitter"));
}


int recup_saisi(char*** saisi_utilisateur)
{
    char tmp[100];
    char mot[10][50];
    int i, nbr_mot=0;
    int taille=0;
    printf("[myshell] $ ");
    fgets(tmp, 100, stdin);
    for(i=0;i<strlen(tmp);i++)
    {
        if(tmp[i]==' ' || tmp[i]=='\n')
        {
            mot[nbr_mot][i-taille]='\0';
            taille+=strlen(mot[nbr_mot])+1;
            nbr_mot++;
        }
        else
        {
            mot[nbr_mot][i-taille]=tmp[i];
        }
    }
    *saisi_utilisateur=(char**)realloc(*saisi_utilisateur,sizeof(char*)*nbr_mot+1);
    for(i=0;i<nbr_mot;i++)
    {
        (*saisi_utilisateur)[i]=(char*)malloc(sizeof(char)*strlen(mot[i]));
        strcpy((*saisi_utilisateur)[i],mot[i]);
    }
    return nbr_mot;
}


ino_t mycreat(char* nom, mode_t mode)
{
    struct stat sb;
    creat(nom,mode);
    stat(nom, &sb);
    return sb.st_ino;
}
