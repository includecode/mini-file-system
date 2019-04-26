#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "primitives.h"
#include "file_system.h"

/*
t_fichier* bd= creer_un_fichier();
ino_t mycreat(char* nom, mode_t mode)
{
    struct stat sb;
    creat(nom,mode);
    stat(nom, &sb);
    return sb.st_ino;
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
    int i;
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
    int i;
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
*/

harddisk_t* init_hdd(){
    harddisk_t *hdd =(harddisk_t*) malloc(sizeof(harddisk_t));
    hdd->tab_blocs = NULL;
    hdd->tab_fichiers = NULL;
    hdd->tab_inodes = (inode_t**)malloc(sizeof(inode_t*)); // Un seul inode est crée à l'initialisation
    hdd->total_size = HDD_SIZE;
    hdd->super_block.nbre_fichiers = 0;
    hdd->super_block.nbre_inodes = 0;
    hdd->super_block.count_used_blocks = 0;
    hdd->super_block.count_used_inodes = 0;
    return hdd;
}

block_t** allouer_blocs(harddisk_t *hdd){
    int i;
    hdd->tab_blocs = (block_t**)malloc(NBRE_BLOCK * sizeof(block_t*));
    for(i=0; i<NBRE_BLOCK; i++){
        hdd->tab_blocs[i] = (block_t*)malloc(BLOCK_SIZE * sizeof(block_t));
        hdd->tab_blocs[i]->nbre_fichiers = 0;
        hdd->tab_blocs[i]->files = (t_fichier**) malloc(sizeof(t_fichier*));
        hdd->tab_blocs[i]->files[0] = (t_fichier*) malloc(sizeof(t_fichier));
        hdd->tab_blocs[i]->precedent = NULL;
        hdd->tab_blocs[i]->suivant = NULL;
    }
    
    printf("%d BLOCK ont été crées\n", BLOCK_SIZE);
    return &(hdd->tab_blocs[0]);
}

inode_t* allouer_inode(t_fichier *fichier, harddisk_t *hdd){
    inode_t *inode = (inode_t*)malloc(sizeof(inode_t));
    //inode->file_size = taille
    //.
    //.
    fichier->inode = inode;
    hdd->tab_inodes[hdd->super_block.nbre_inodes] = inode;
    hdd->super_block.nbre_inodes += 1;

    //On reserve de l'espace pour le prochain inode
    hdd->tab_inodes = (inode_t**)realloc(hdd->tab_inodes, sizeof(inode_t*) * (hdd->super_block.nbre_inodes +1)); //+1 puisque les inodes commencent à 0
    printf("Une nouvel i-node alloué\n");
    return inode;
}

void ajouter_fichier_dans_bloc(harddisk_t* hdd, t_fichier *fichier){
    
    //Disque dur plein??
    if(hdd->super_block.count_used_blocks < NBRE_BLOCK){
    //On teste si la taille du fichier peur entrer dans le dernier bloc non plein
    int position;
    if(!bloc_plein(hdd, *fichier)){
        position = hdd->tab_blocs[hdd->super_block.count_used_blocks]->nbre_fichiers;

    }else
    {
        position = 0;
        hdd->super_block.count_used_blocks += 1;
    }


    fichier->inode->bloc = hdd->tab_blocs[hdd->super_block.count_used_blocks];

    hdd->tab_blocs[hdd->super_block.count_used_blocks]->files[position] = fichier;
    hdd->tab_blocs[hdd->super_block.count_used_blocks]->nbre_fichiers += 1;
    hdd->super_block.nbre_fichiers += 1;
    hdd->tab_blocs[hdd->super_block.count_used_blocks]->files = (t_fichier**) realloc(hdd->tab_blocs[hdd->super_block.count_used_blocks]->files, sizeof(t_fichier*) * (hdd->tab_blocs[hdd->super_block.count_used_blocks]->nbre_fichiers + 1)); // +1 car le compteur commence à 0
    hdd->tab_blocs[hdd->super_block.count_used_blocks]->files[position+1] = (t_fichier*) realloc(hdd->tab_blocs[hdd->super_block.count_used_blocks]->files[position+1], sizeof(t_fichier) * (hdd->tab_blocs[hdd->super_block.count_used_blocks]->nbre_fichiers + 1)); // +1 car le compteur commence à 0
    printf("Un fichier a été ajouté au bloc N°%d\n", hdd->super_block.count_used_blocks);
    }else
    {
        printf("Disque dur plein!! exiting..\n");
        exit(1);
    }
    
}

t_fichier* creer_un_fichier(harddisk_t *hdd){
    t_fichier* fichier = (t_fichier*)malloc(sizeof(t_fichier));
    printf("Un fichier  a été crée\n");
    return fichier;

}

BOOL file_exist(char* file_name, t_fichier *directory){
    if(directory->inode->file_type == DIRECTORY){
        int i;        
        for (i = 0; i < directory->inode->bloc->nbre_fichiers; i++)
        {
            //printf("cmp %s et %s \n", file_name, directory->inode->bloc[0].files[i]->nom);
            if(strcmp(file_name, directory->inode->bloc[0].files[i]->nom) == false)// 0 => files are equal
                return true;
        }
    }else
    {
        printf("Warning: You must be in a directory to search for files\n");
    }
    
    
    return false;
}

/**
 *  Crée une repertoire initial (racine) sur le bloc[0] inode[0]
 *  @param harddisk_t*  : un pointeur sue le disque dur
*/
void creer_racine_sgf(harddisk_t *hdd){
    //Le fichier << . >>
    t_fichier *fichier = creer_un_fichier(hdd);
    inode_t *inode = allouer_inode(fichier, hdd);
    inode->file_type = DIRECTORY;
    fichier->nom = "."; //RACINE de notre système*/
    hdd->current_dir = *fichier;
    fichier->parent = NULL;
    //TODO s'assurer que le block
    hdd->tab_blocs = allouer_blocs(hdd);
    ajouter_fichier_dans_bloc(hdd, fichier);

    //Le fichier << root >>
    t_fichier *root = creer_un_fichier(hdd);
    root->nom = "/root";
    root->parent = NULL;
    inode_t *inode_root = allouer_inode(root, hdd);
    ajouter_fichier_dans_bloc(hdd, root);
    printf("[Boot] Repertoire initial << . >> initialisé...\n");
}

/**
 *  Demarre le systeme virtuel (initialise le HDD et alloue les block, inodes..)
 *  @return harddisk_t*  : L'adresse du disque dur
 */
harddisk_t* boot(){
    harddisk_t* hdd = init_hdd();
    printf("[Boot] Disque dur virtuel crée...\n");
    printf("[Boot] Taille Disque: %ld octets...\n", HDD_SIZE);
    creer_racine_sgf(hdd);
    return hdd;
}


BOOL bloc_plein(harddisk_t *hdd, t_fichier nouveau_fichier){
    //TODO tester si la taille du fichier peut entrez avec l'espace restant sur le bloc ou si le bloc est déja plein
    float block_used_size = 0;
    int i;
    for (i = 0; i < hdd->tab_blocs[hdd->super_block.count_used_blocks]->nbre_fichiers; i++){
        block_used_size += hdd->tab_blocs[hdd->super_block.count_used_blocks]->files[i]->inode->file_size;
    }
    if(block_used_size < BLOCK_SIZE){
        //Taille de block pas dépassée apres ajout du fichier ??
        if(BLOCK_SIZE <= block_used_size + nouveau_fichier.inode->file_size){
            return true;
        }
    }
    return false;
}


void chkdsk(harddisk_t *hdd){
    printf("____________________________________________________\n");
    printf("Hard disk drive informations:\n");
    printf("\tSIZE:                             %ld octets\n", HDD_SIZE);
    printf("\tFREE SIZE:                        TODO\n");
    printf("\tNOMBRE DE BLOCS:                  %ld blocs\n", NBRE_BLOCK);
    printf("\tNOMBRE D'I-NODES:                 %d inodes\n", hdd->super_block.nbre_inodes);
    printf("\tTAILLE D'UN BLOC:                 %ld octets\n", BLOCK_SIZE);
    printf("\tNombre de blocs Libres:           %d\n", NBRE_BLOCK - hdd->super_block.count_used_blocks);
    printf("\tNombre de blocs occupés:          %d\n", hdd->super_block.count_used_blocks);
    printf("\tNombre de fichiers sur le HDD:    %d\n", hdd->super_block.nbre_fichiers);
    printf("____________________________________________________\n");

}