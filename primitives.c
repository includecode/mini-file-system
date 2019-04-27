#include "primitives.h"


inode_t mycreat(char* nom, char mode[11])
{
    ajouter_fichier_dans_bloc(0);
    hdd.tab_blocs[hdd.super_block.count_used_blocks].files[hdd.tab_blocs[hdd.super_block.count_used_blocks].nbre_fichiers-1].nom=(char*)malloc(sizeof(char)*(strlen(nom)+1));
    hdd.tab_blocs[hdd.super_block.count_used_blocks].files[hdd.tab_blocs[hdd.super_block.count_used_blocks].nbre_fichiers-1].contenu=(char**)malloc(sizeof(char*));
    
    strcpy(hdd.tab_blocs[hdd.super_block.count_used_blocks].files[hdd.tab_blocs[hdd.super_block.count_used_blocks].nbre_fichiers-1].nom,nom);
    hdd.tab_blocs[hdd.super_block.count_used_blocks].files[hdd.tab_blocs[hdd.super_block.count_used_blocks].nbre_fichiers-1].inode.file_type=NORMAL;
    hdd.tab_blocs[hdd.super_block.count_used_blocks].files[hdd.tab_blocs[hdd.super_block.count_used_blocks].nbre_fichiers-1].inode.file_size=0;
    hdd.tab_blocs[hdd.super_block.count_used_blocks].files[hdd.tab_blocs[hdd.super_block.count_used_blocks].nbre_fichiers-1].inode.nbre_links=0;
    strcpy(hdd.tab_blocs[hdd.super_block.count_used_blocks].files[hdd.tab_blocs[hdd.super_block.count_used_blocks].nbre_fichiers-1].inode.privileges,"644");
    strcpy(hdd.tab_blocs[hdd.super_block.count_used_blocks].files[hdd.tab_blocs[hdd.super_block.count_used_blocks].nbre_fichiers-1].inode.mode,mode);
    hdd.super_block.nbre_inodes++;
    
    hdd.tab_blocs[hdd.super_block.count_used_blocks].files[hdd.tab_blocs[hdd.super_block.count_used_blocks].nbre_fichiers-1].nbr_fichier=0;
    
    hdd.tab_blocs[hdd.super_block.count_used_blocks].files[hdd.tab_blocs[hdd.super_block.count_used_blocks].nbre_fichiers-1].num_dossier_parent=hdd.num_current_dir;
    
    if(hdd.super_block.nbre_fichiers>1)
    {
        hdd.tab_blocs[hdd.super_block.count_used_blocks].files[hdd.num_current_dir].nbr_fichier++;
        hdd.tab_blocs[hdd.super_block.count_used_blocks].files[hdd.num_current_dir].contenu=(char**)realloc(hdd.tab_blocs[hdd.super_block.count_used_blocks].files[hdd.num_current_dir].contenu,sizeof(char*)*hdd.tab_blocs[hdd.super_block.count_used_blocks].files[hdd.num_current_dir].nbr_fichier);
        
        hdd.tab_blocs[hdd.super_block.count_used_blocks].files[hdd.num_current_dir].contenu[hdd.tab_blocs[hdd.super_block.count_used_blocks].files[hdd.num_current_dir].nbr_fichier-1]=(char*)malloc(sizeof(char)*(strlen(nom)+1));
        strcpy(hdd.tab_blocs[hdd.super_block.count_used_blocks].files[hdd.num_current_dir].contenu[hdd.tab_blocs[hdd.super_block.count_used_blocks].files[hdd.num_current_dir].nbr_fichier-1],nom);
    }
    return hdd.tab_blocs[hdd.super_block.count_used_blocks].files[hdd.tab_blocs[hdd.super_block.count_used_blocks].nbre_fichiers-1].inode;
}

void ls()
{
    for(int i=0;i<hdd.tab_blocs[hdd.super_block.count_used_blocks].files[hdd.num_current_dir].nbr_fichier;i++)
        printf("%s ",hdd.tab_blocs[hdd.super_block.count_used_blocks].files[hdd.num_current_dir].contenu[i]);
    printf("\n");
}

BOOL existance_fichier(char *name)
{
    BOOL existance=false;
    for(int i=0;i<hdd.tab_blocs[hdd.super_block.count_used_blocks].files[hdd.num_current_dir].nbr_fichier;i++)
    {
        if(strstr(hdd.tab_blocs[hdd.super_block.count_used_blocks].files[hdd.num_current_dir].contenu[i],name))
            return true;
    }
    return false;
}

int position_rep(char* name)
{
    for(int i=0;i<hdd.super_block.count_used_blocks+1;i++)
    {
        for(int j=0;j<hdd.tab_blocs[i].nbre_fichiers;j++)
        {
            if(hdd.tab_blocs[i].files[j].inode.file_type==DIRECTORY && strstr(hdd.tab_blocs[i].files[j].nom,name))
               return j;
        }
    }
    return -1;
}

void cd(char* name)
{
    int pos;
    if(strstr(name,".."))
        hdd.num_current_dir=hdd.tab_blocs[hdd.super_block.count_used_blocks].files[hdd.num_current_dir].num_dossier_parent;
    else
    {
        pos=position_rep(name);
        if(pos!=-1 && existance_fichier(name))
            hdd.num_current_dir=pos;
    }
}

void mymkdir(char* nom)
{
    mycreat(nom, "drwxr-xr-x");
    hdd.tab_blocs[hdd.super_block.count_used_blocks].files[hdd.tab_blocs[hdd.super_block.count_used_blocks].nbre_fichiers-1].inode.file_type=DIRECTORY;
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

                
/*inode_t myopen(char* nom, char mode[12])
{
    for(int i=0;i<hdd.super_block.count_used_blocks+1;i++)
    {
        for(int j=0;j<hdd.tab_blocs[i].nbre_fichiers;j++)
        {
            if(hdd.tab_blocs[i].files[j].name,nom))
            
        }
    
    return hdd.tab_blocs[hdd.super_block.count_used_blocks].files[hdd.tab_blocs[hdd.super_block.count_used_blocks].nbre_fichiers-1].inode;
}*/

/*void myclose(ino_t inode)
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

void init_hdd(){
    hdd.tab_blocs = NULL;
    
    hdd.total_size = HDD_SIZE;
    hdd.super_block.nbre_fichiers = 0;
    hdd.super_block.nbre_inodes = 0;
    hdd.super_block.count_used_blocks = 0;
    hdd.super_block.count_used_inodes = 0;
    allouer_blocs();
}

/**
 *  Demarre le systeme virtuel (initialise le HDD et alloue les block, inodes..)
 *  @return harddisk_t*  : L'adresse du disque dur
 */
void boot(){
    init_hdd();
    printf("[Boot] Disque dur virtuel crée...\n");
    printf("[Boot] Taille Disque: %d octets...\n", HDD_SIZE);
    creer_racine_sgf();
}

/**
 *  Crée une repertoire initial (racine) sur le bloc[0] inode[0]
 *  @param harddisk_t*  : un pointeur sue le disque dur
 */
void creer_racine_sgf(){
    mymkdir("user");
    hdd.num_current_dir = 0;
}


void allouer_blocs(){
    int i;
    hdd.tab_blocs = (block_t*)malloc(NBRE_BLOCK * sizeof(block_t));
    for(i=0; i<NBRE_BLOCK; i++){
        hdd.tab_blocs[i].nbre_fichiers = 0;
        hdd.tab_blocs[i].files = (t_fichier*) malloc(sizeof(t_fichier)*hdd.tab_blocs[hdd.super_block.count_used_blocks].nbre_fichiers);
    }
    
    printf("%d BLOCK ont été crées\n", BLOCK_SIZE);
}

void ajouter_fichier_dans_bloc(int file_size){
    
    //Disque dur plein??
    if(hdd.super_block.count_used_blocks < NBRE_BLOCK){
    //On teste si la taille du fichier peur entrer dans le dernier bloc non plein
        if(bloc_plein(file_size))
            hdd.super_block.count_used_blocks++;

        hdd.tab_blocs[hdd.super_block.count_used_blocks].nbre_fichiers++;
        hdd.super_block.nbre_fichiers++;
        hdd.tab_blocs[hdd.super_block.count_used_blocks].files = (t_fichier*) realloc(hdd.tab_blocs[hdd.super_block.count_used_blocks].files, sizeof(t_fichier) * hdd.tab_blocs[hdd.super_block.count_used_blocks].nbre_fichiers); // +1 car le compteur commence à 0
        printf("Un fichier a été ajouté au bloc N°%d\n", hdd.super_block.count_used_blocks);
    }
    else
    {
        printf("Disque dur plein!! exiting..\n");
        exit(1);
    }
    
}


BOOL file_exist(char* file_name, t_fichier directory){
    int i;
    if(directory.inode.file_type == DIRECTORY)
    {
        for (i = 0; i < directory.inode.bloc->nbre_fichiers; i++)
        {
            //printf("cmp %s et %s \n", file_name, directory->inode->bloc[0].files[i]->nom);
            if(strcmp(file_name, directory.inode.bloc[0].files[i].nom) == false)// 0 => files are equal
                return true;
        }
    }
    else
    {
        printf("Warning: You must be in a directory to search for files\n");
    }
    
    
    return false;
}



BOOL bloc_plein(int file_size){
    //TODO tester si la taille du fichier peut entrez avec l'espace restant sur le bloc ou si le bloc est déja plein
    float block_used_size = 0;
    int i;
    for (i = 0; i < hdd.tab_blocs[hdd.super_block.count_used_blocks].nbre_fichiers; i++){
        block_used_size += hdd.tab_blocs[hdd.super_block.count_used_blocks].files[i].inode.file_size;
    }
    if(block_used_size <= BLOCK_SIZE){
        //Taille de block pas dépassée apres ajout du fichier ??
        if(BLOCK_SIZE <= block_used_size + file_size){
            return true;
        }
    }
    else
        return true;
    return false;
}


void chkdsk(){
    printf("____________________________________________________\n");
    printf("Hard disk drive informations:\n");
    printf("\tSIZE:                             %d octets\n", HDD_SIZE);
    printf("\tFREE SIZE:                        TODO\n");
    printf("\tNOMBRE DE BLOCS:                  %d blocs\n", NBRE_BLOCK);
    printf("\tNOMBRE D'I-NODES:                 %d inodes\n", hdd.super_block.nbre_inodes);
    printf("\tTAILLE D'UN BLOC:                 %d octets\n", BLOCK_SIZE);
    printf("\tNombre de blocs Libres:           %d\n", NBRE_BLOCK - hdd.super_block.count_used_blocks);
    printf("\tNombre de blocs occupés:          %d\n", hdd.super_block.count_used_blocks);
    printf("\tNombre de fichiers sur le HDD:    %d\n", hdd.super_block.nbre_fichiers);
    printf("____________________________________________________\n");

}
