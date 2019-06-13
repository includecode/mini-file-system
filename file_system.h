#ifndef FILE_SYSTEM_H
#define FILE_SYSTEM_H

#include <time.h>

//MACRO  CONSTANTES DU  S.  G.  F
#define HDD_SIZE 300000 // 3Go ça fait plus réaliste
#define NBRE_BLOCK 30
#define BLOCK_SIZE 10000 // 10 Mo
#define TRUE 1
#define True 1
#define true 1
#define FALSE 0
#define False 0
#define false 0
#define root_folder_name "user"

typedef enum {NORMAL, DIRECTORY, BLOCK, LINK, PIPE, DELETED} file_type_t;
typedef enum {O_RONLY,O_WONLY,O_CLOSE} mode_tt;
typedef short BOOL;
typedef struct t_fichier t_fichier;

/*Structure d'un utilisateur*/
typedef struct utilisateur_t
{
    unsigned short uid; // user id
    unsigned short gid; // Group id
}utilisateur_t;

/*Structure des liens*/
typedef struct t_link
{
    t_fichier *file;
    char *name;
}t_link;

/*Structure d'un bloc*/
typedef struct block_t
{
    t_link **links;//Liste des links du bloc
    t_fichier **files;//Liste des fichiers du bloc
    int nbre_links;
    int nbre_fichiers;
}block_t;

/*Structure d'un i-node*/
typedef struct inode_t
{
    /*Considérons que le numéro d'inode c'est son index dans le tableau d'inodes*/
    int numero_inode;
    file_type_t file_type;
    mode_tt ouverture; // EX: {6, 4, 4} Privilèges par defaut
    time_t date_created;
    time_t last_access;
    time_t last_modified;
    int file_size;
    char mode[12];
    int nbre_links; //0 liens symboliques à la création
    utilisateur_t user; // the file owner
}inode_t;

/*Structure d'un fichier*/
typedef struct t_fichier
{
    inode_t* inode;
    t_fichier *dossier_parent;
    char *nom;
    char **contenu;
    int nbr_fichier;
    block_t *bloc;
    int nbr_ligne;
}t_fichier;

/*Structure du super bloc [0]*/
typedef struct super_block_t
{
    int count_used_blocks; // Le compteur de nombre de blocks utilisés
    int count_used_inodes; // Le compteur de nombre d'inodes utilisés
    float free_disk_size; // Espace disponible sur le HDD
    int nbre_fichiers;
    int nbre_links;
    int nbre_inodes;
}super_block_t;

/* STRUCTURE DU DISQUE DUR*/
typedef struct harddisk_t
{
    int total_size;
    inode_t** tab_inodes;//Liste des inodes
    block_t *tab_blocs; // Le tableau des blocks de 10Mo chacun (pointe sur le premier fichier du bloc)
    super_block_t super_block;
    t_fichier *current_dir; // contient le repertoire dans lequel l'utilisateur se trouve à chaque instant
}harddisk_t;

#endif // FILE_SYSTEM_H
