#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

typedef struct t_fichier
{
    ino_t inode;
    char *nom;
}

int recup_saisi(char*** saisi_utilisateur);
ino_t mycreat(char* nom, mode_t mode);
ino_t myopen(char* nom, mode_t mode);
void myclose(ino_t inode);
int myread(ino_t inode, char **buffer, int nombre);
int myread(ino_t inode, char **buffer, int nombre);

t_fichier* bd=(t_fichier*)malloc(sizeof(t_fichier));

int main(int nbarg, char* argv[])
{
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
                
ino_t myopen(char* nom, mode_t mode)
{
    struct stat sb;
    ino_t inode;
    if(open(nom, O_RDONLY | O_WDONLY)==-1)
    {
        inode=mycreat(nom,mode);
        open(nom, O_RDONLY | O_WDONLY);
        return inode;
    }
    stat(nom, &sb);
    return sb.st_ino;
}

void myclose(ino_t inode)
{
    close(inode);
}

int myread(ino_t inode, char **buffer, int nombre)
{
    *buffer=(char*)realloc(*buffer,sizeof(char)*nombre);
    for(i=0;i<strlen(bd);i++)
    {
        if(bd[i].inode==inode)
        {
            fgets(*buffer, nombre, bd[i].nom);
            printf("%s",*buffer);
            return nombre;
        }
    }
    return 0;
}


int myread(ino_t inode, char **buffer, int nombre)
{
    *buffer=(char*)realloc(*buffer,sizeof(char)*nombre);
    fgets(*buffer, nombre, stdin);
    for(i=0;i<strlen(bd);i++)
    {
        if(bd[i].inode==inode)
        {
            fputs(*buffer, nombre, bd[i].nom);
            return nombre;
        }
    }
    return 0;
}
