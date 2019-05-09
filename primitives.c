#include "primitives.h"
char prompt[100] = "myshell@user~:";
inode_t mycreat(char *nom, char mode[11])
{
    BOOL continuer = True;
    if (hdd.super_block.nbre_fichiers > 1)
    {
        if (hdd.current_dir->contenu[0] != NULL)
        {
            int i;
            int count_fichiers = 0;
            int j = 0;
            while (hdd.current_dir->contenu[j] != NULL)
            {
                j++;
                count_fichiers++;
            }
            for (i = 0; i < count_fichiers; i++)
            {
                if (strcmp(nom, hdd.current_dir->contenu[i]) == false)
                {
                    printf("Error: /!\\ Un fichier existe déja avec le nom '%s'\n", nom);
                    continuer = false;
                }
            }
        }
    }

    if (continuer)
    {
        ajouter_fichier_dans_bloc(0);
        hdd.tab_blocs[hdd.super_block.count_used_blocks].files[hdd.tab_blocs[hdd.super_block.count_used_blocks].nbre_fichiers - 1]->nom = (char *)malloc(sizeof(char) * (strlen(nom) + 1));
        hdd.tab_blocs[hdd.super_block.count_used_blocks].files[hdd.tab_blocs[hdd.super_block.count_used_blocks].nbre_fichiers - 1]->contenu = (char **)malloc(sizeof(char *));

        strcpy(hdd.tab_blocs[hdd.super_block.count_used_blocks].files[hdd.tab_blocs[hdd.super_block.count_used_blocks].nbre_fichiers - 1]->nom, nom);
        hdd.tab_blocs[hdd.super_block.count_used_blocks].files[hdd.tab_blocs[hdd.super_block.count_used_blocks].nbre_fichiers - 1]->inode.file_type = NORMAL;
        hdd.tab_blocs[hdd.super_block.count_used_blocks].files[hdd.tab_blocs[hdd.super_block.count_used_blocks].nbre_fichiers - 1]->inode.file_size = 0;
        hdd.tab_blocs[hdd.super_block.count_used_blocks].files[hdd.tab_blocs[hdd.super_block.count_used_blocks].nbre_fichiers - 1]->bloc = &(hdd.tab_blocs[hdd.super_block.count_used_blocks]);
        hdd.tab_blocs[hdd.super_block.count_used_blocks].files[hdd.tab_blocs[hdd.super_block.count_used_blocks].nbre_fichiers - 1]->inode.nbre_links = 0;
        strcpy(hdd.tab_blocs[hdd.super_block.count_used_blocks].files[hdd.tab_blocs[hdd.super_block.count_used_blocks].nbre_fichiers - 1]->inode.privileges, "644");
        strcpy(hdd.tab_blocs[hdd.super_block.count_used_blocks].files[hdd.tab_blocs[hdd.super_block.count_used_blocks].nbre_fichiers - 1]->inode.mode, mode);
        hdd.super_block.nbre_inodes++;

        if (hdd.tab_blocs[hdd.super_block.count_used_blocks].nbre_fichiers < 1)
            hdd.tab_blocs[hdd.super_block.count_used_blocks].files[hdd.tab_blocs[hdd.super_block.count_used_blocks].nbre_fichiers - 1]->nbr_fichier = 0;
        else
            hdd.tab_blocs[hdd.super_block.count_used_blocks].files[hdd.tab_blocs[hdd.super_block.count_used_blocks].nbre_fichiers - 1]->nbr_fichier = hdd.tab_blocs[hdd.super_block.count_used_blocks].nbre_fichiers - 1;

        if (hdd.super_block.nbre_fichiers > 1)
        {
            hdd.tab_blocs[hdd.super_block.count_used_blocks].files[hdd.tab_blocs[hdd.super_block.count_used_blocks].nbre_fichiers - 1]->nbr_fichier++;
            hdd.current_dir->contenu = (char **)realloc(hdd.current_dir->contenu, sizeof(char *) * hdd.tab_blocs[hdd.super_block.count_used_blocks].files[hdd.tab_blocs[hdd.super_block.count_used_blocks].nbre_fichiers - 1]->nbr_fichier);
            int count_fichiers = 0;
            int j = 0;
            while (hdd.current_dir->contenu[j] != NULL)
            {
                j++;
                count_fichiers++;
            }
            hdd.current_dir->contenu[count_fichiers] = (char *)malloc(sizeof(char) * (strlen(nom) + 1));
            strcpy(hdd.current_dir->contenu[count_fichiers], nom);
        }

        if (hdd.tab_blocs[0].nbre_fichiers > 1)
        {
            hdd.tab_blocs[hdd.super_block.count_used_blocks].files[hdd.tab_blocs[hdd.super_block.count_used_blocks].nbre_fichiers - 1]->dossier_parent = hdd.current_dir;
        }
    }

    return hdd.tab_blocs[hdd.super_block.count_used_blocks].files[hdd.tab_blocs[hdd.super_block.count_used_blocks].nbre_fichiers - 1]->inode;
}

void ls()
{
    int i, j, debut, count_fichiers;
    count_fichiers = 0;
    debut = hdd.current_dir->bloc->nbre_fichiers - 2;
    j = 0;
    while (hdd.current_dir->contenu[j] != NULL)
    {
        if (hdd.current_dir->contenu[j] != "[DELETED]")
        {
            printf("%s ", hdd.current_dir->contenu[j]);
        }
        j++;
    }
    printf("\n");
}

BOOL existance_fichier(char *name)
{
    BOOL existance = false;
    for (int i = 0; i < hdd.tab_blocs[hdd.super_block.count_used_blocks].files[hdd.current_dir->nbr_fichier]->nbr_fichier; i++)
    {
        if (strstr(hdd.tab_blocs[hdd.super_block.count_used_blocks].files[hdd.current_dir->nbr_fichier]->contenu[i], name))
            return true;
    }
    return false;
}

int position_rep(char *name)
{
    for (int i = 0; i < hdd.super_block.count_used_blocks + 1; i++)
    {
        for (int j = 0; j < hdd.tab_blocs[i].nbre_fichiers; j++)
        {
            if (hdd.tab_blocs[i].files[j]->inode.file_type == DIRECTORY && strstr(hdd.tab_blocs[i].files[j]->nom, name))
                return j;
        }
    }
    return -1;
}

void cd(char *name)
{
    if (name != NULL)
    {
        if (strstr(name, ".."))
        {
            if (hdd.tab_blocs[0].files[0] != hdd.current_dir)
            {
                roll_back_prompt_once();
                hdd.current_dir = hdd.current_dir->dossier_parent;
            }
        }
        else
        {
            t_fichier *testExist = file_exist(name, hdd.current_dir);
            if (testExist->nbr_fichier != -1)
            {
                if (testExist->inode.file_type == DIRECTORY)
                {
                    char next_prompt[100] = "/";
                    strcat(next_prompt, testExist->nom);
                    strcat(prompt, next_prompt);
                    hdd.current_dir = testExist;
                }
                else
                {
                    printf("%s is not a directory.\n", name);
                }
            }
            else
            {
                printf("Error: %s not found.\n", name);
            }

            testExist = NULL;
        }
    }
    else
    {
        hdd.current_dir = hdd.tab_blocs[0].files[0];
    }
}

void mymkdir(char *nom)
{
    mycreat(nom, "drwxr-xr-x");
    hdd.tab_blocs[hdd.super_block.count_used_blocks].files[hdd.tab_blocs[hdd.super_block.count_used_blocks].nbre_fichiers - 1]->inode.file_type = DIRECTORY;
}

int recup_saisi(char ***saisi_utilisateur)
{
    char tmp[100];
    char mot[10][50];
    int i, nbr_mot = 0;
    int taille = 0;
    printf("%s $", prompt);
    fgets(tmp, 100, stdin);
    for (i = 0; i < strlen(tmp); i++)
    {
        if (tmp[i] == ' ' || tmp[i] == '\n')
        {
            mot[nbr_mot][i - taille] = '\0';
            taille += strlen(mot[nbr_mot]) + 1;
            nbr_mot++;
        }
        else
        {
            mot[nbr_mot][i - taille] = tmp[i];
        }
    }
    *saisi_utilisateur = (char **)realloc(*saisi_utilisateur, sizeof(char *) * nbr_mot + 1);
    for (i = 0; i < nbr_mot; i++)
    {
        (*saisi_utilisateur)[i] = (char *)malloc(sizeof(char) * strlen(mot[i]));
        strcpy((*saisi_utilisateur)[i], mot[i]);
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

void init_hdd()
{
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
void boot()
{
    init_hdd();
    printf("[Boot] Disque dur virtuel crée...\n");
    printf("[Boot] Taille Disque: %d octets...\n", HDD_SIZE);
    creer_racine_sgf();
}

/**
 *  Crée une repertoire initial (racine) sur le bloc[0] inode[0]
 *  @param harddisk_t*  : un pointeur sue le disque dur
 */
void creer_racine_sgf()
{
    mymkdir(root_folder_name);

    hdd.current_dir = hdd.tab_blocs[0].files[0];
    hdd.tab_blocs[0].files[0]->inode.file_type = DIRECTORY;
    hdd.current_dir->nbr_fichier = 0;
    hdd.current_dir->inode.file_type = DIRECTORY;
}

void allouer_blocs()
{
    int i;
    hdd.tab_blocs = (block_t *)malloc(NBRE_BLOCK * sizeof(block_t));
    for (i = 0; i < NBRE_BLOCK; i++)
    {
        hdd.tab_blocs[i].nbre_fichiers = 0;
        hdd.tab_blocs[i].files = (t_fichier **)malloc(sizeof(t_fichier) * hdd.tab_blocs[hdd.super_block.count_used_blocks].nbre_fichiers);
        hdd.tab_blocs[i].files[0] = (t_fichier *)malloc(sizeof(t_fichier));
    }

    printf("%d BLOCK ont été crées\n", BLOCK_SIZE);
}
/**
  * Copy a file
  * @param file         : the file to be copied
  * @return             : void
  * @param  directory   : The destination folder
  * 
  */
void mycp(t_fichier file, t_fichier *directory)
{
    char *previous_name = file.nom;
    t_fichier *temp = hdd.current_dir;
    hdd.current_dir = directory;
    //NOTE: avoid adress args here, if not the comparaison will always return true
    short taille = strlen(file.nom);
    char nom[taille];
    int i;
    for (i = 0; i < taille; i++)
    {
        nom[i] = file.nom[i];
    }
    nom[i] = '\0';
    while (file_exist(nom, directory)->nbr_fichier != -1)
    {
        strcat(nom, "(copie)");
    }
    if (file.inode.file_type == DIRECTORY)
    {
        mymkdir(nom);
        hdd.tab_blocs[hdd.super_block.count_used_blocks].files[hdd.tab_blocs[hdd.super_block.count_used_blocks].nbre_fichiers - 1]->dossier_parent = directory;
    }
    else
    {
        mycreat(nom, file.inode.mode);
        hdd.tab_blocs[hdd.super_block.count_used_blocks].files[hdd.tab_blocs[hdd.super_block.count_used_blocks].nbre_fichiers - 1]->dossier_parent = directory;
    }
    printf("Le fichier '%s' à été copié dans %s\n", previous_name, directory->nom);

    hdd.current_dir = temp;
}
/**
 * Teste si le chemin de copie est correct (existe) 
 * @Return t_fichier    : le fichier correspodant au repertoire destination
 * @Return NULL         : si le chemin n'existe pas
 * @Param path_type     : Le type de chemin à tester (source = 'S' ou  destination = 'D')
 **/
t_fichier *url_exists(char *string, char path_type)
{
    BOOL must_be_directory = true;
    if (path_type != 'D')
        must_be_directory = false;

    char *pch;
    pch = strtok(string, "/");
    //k = error code
    int i, j, k;
    block_t current_block;
    BOOL is_first_iteration_end = false;
    t_fichier *testExist;
    t_fichier *destination = NULL;
    printf("pch %s\n", pch);
    if (pch != NULL)
    {

        //On parcours tous les blocks pour vérifier que tous le dossiers du chemin existent
        for (i = 0; i < hdd.super_block.count_used_blocks + 1; i++)
        {
            current_block = hdd.tab_blocs[i];
            for (j = 0; j < current_block.nbre_fichiers; j++)
            {
                if (strcmp(pch, current_block.files[j]->nom) == 0)
                {
                    k = 0;
                    if ((current_block.files[j]->inode.file_type == DIRECTORY && must_be_directory == true) || must_be_directory == false)
                    {
                        pch = strtok(NULL, "/");
                        if (pch == NULL)
                        {
                            if (i == 0 && j == 0)
                                is_first_iteration_end = True;
                            break;
                        }
                        testExist = file_exist(pch, current_block.files[j]);
                        if (testExist->nbr_fichier != -1)
                        {
                            if (testExist->dossier_parent == current_block.files[j])
                            {
                                destination = testExist;
                                current_block = *hdd.tab_blocs[i].files[j]->bloc;
                                j = 0;
                            }
                            else
                                destination = NULL;
                        }
                        else
                        {
                            printf("Error: %s not found\n", pch);
                            k = 0;
                            destination = NULL;
                        }
                    }
                    else
                    {
                        printf("%s is  not a directory\n", current_block.files[j]->nom);
                        k = 0;
                        destination = NULL;
                    }
                }
                else
                    k = -1;
            }
        }
    }
    if (destination == NULL && k == -1)
        printf("Error: File  not found\n", string);

    if (is_first_iteration_end)
        destination = hdd.tab_blocs[0].files[0]; //root folder

    return destination;
}
char *explode_last_from_path(char *string)
{
    char *pch, *last_pch;
    char *temp = (char *)malloc(sizeof(char));
    int i;
    for (i = 0; i < strlen(string); i++)
    {
        temp[i] = string[i];
    }

    pch = strtok(temp, "/");
    while (pch != NULL)
    {
        pch = strtok(NULL, "/");
        if (pch != NULL)
            last_pch = pch;
    }

    return last_pch;
}
/**
 * Supprime un repertoire du chemin prompt
 * Ex: a/b/c/d => a/b/c 
 * */
void roll_back_prompt_once()
{
    char *last_folder;
    int i, pos_last_slash;

    //find the last slash position
    for (i = 0; i < strlen(prompt); i++)
    {
        last_folder = strrchr(prompt, '/');
        if (strcmp(&prompt[i], last_folder) == 0)
        {
            pos_last_slash = i;
            break;
        }
    }
    prompt[pos_last_slash] = '\0';
}
/**
 * Verify if the path starts with the defined "root_folder_name"
 * @Return  : boolean
 * */
BOOL begins_with_root_folder(char *path)
{

    char *temp = (char *)malloc(sizeof(char));
    int i;
    for (i = 0; i < strlen(path); i++)
    {
        temp[i] = path[i];
    }
    char *source_is_full_path = strtok(temp, "/");
    if (strcmp(root_folder_name, temp) == 0)
    {
        free(temp);
        return True;
    }
    else
    {
        free(temp);
        return False;
    }
}
void ajouter_fichier_dans_bloc(int file_size)
{

    //Disque dur plein??
    if (hdd.super_block.count_used_blocks < NBRE_BLOCK)
    {
        //On teste si la taille du fichier peur entrer dans le dernier bloc non plein
        if (bloc_plein(file_size))
            hdd.super_block.count_used_blocks++;

        hdd.tab_blocs[hdd.super_block.count_used_blocks].nbre_fichiers++;
        hdd.super_block.nbre_fichiers++;
        hdd.tab_blocs[hdd.super_block.count_used_blocks].files = (t_fichier **)realloc(hdd.tab_blocs[hdd.super_block.count_used_blocks].files, sizeof(t_fichier *) * hdd.tab_blocs[hdd.super_block.count_used_blocks].nbre_fichiers); // +1 car le compteur commence à 0
        hdd.tab_blocs[hdd.super_block.count_used_blocks].files[hdd.tab_blocs[hdd.super_block.count_used_blocks].nbre_fichiers - 1] = (t_fichier *)malloc(sizeof(t_fichier));
    }
    else
    {
        printf("Disque dur plein!! exiting..\n");
        exit(1);
    }
}

/**
 *  Cherche si un fichier existe et retourne son adresse
 *  Sinon retourne un t_fichier avec comme num_dossier_parent -1 (code d'erreur)
 * 
*/
t_fichier *file_exist(char *file_name, t_fichier *directory)
{
    int i;
    if (directory->inode.file_type == DIRECTORY)
    {
        for (i = 0; i < directory->bloc->nbre_fichiers; i++)
        {
            //printf("%d cmp %s et %s \n", i, file_name, directory->bloc->files[i]->nom);
            if (strcmp(file_name, directory->bloc->files[i]->nom) == false)
            { // 0 => files are equal
                return directory->bloc->files[i];
            }
        }
    }
    else
    {
        printf("Warning: You must be in a directory to search for files\n");
    }

    t_fichier *error_code = (t_fichier *)malloc(sizeof(t_fichier));
    error_code->nbr_fichier = -1;
    return error_code;
}

BOOL bloc_plein(int file_size)
{
    //TODO tester si la taille du fichier peut entrez avec l'espace restant sur le bloc ou si le bloc est déja plein
    float block_used_size = 0;
    int i;
    for (i = 0; i < hdd.tab_blocs[hdd.super_block.count_used_blocks].nbre_fichiers; i++)
    {
        block_used_size += hdd.tab_blocs[hdd.super_block.count_used_blocks].files[i]->inode.file_size;
    }
    if (block_used_size <= BLOCK_SIZE)
    {
        //Taille de block pas dépassée apres ajout du fichier ??
        if (BLOCK_SIZE <= block_used_size + file_size)
        {
            return true;
        }
    }
    else
        return true;
    return false;
}
/**
 * Deletes a file
 * @Param file  : file to delete
 * NOTE a deleted file is just hidden
 * */
void myrm(t_fichier *file)
{
    char *name = file->nom;
    int i = 0, j, contenue_size = 0;
    while (file->dossier_parent->contenu[i] != NULL)
    {
        printf("%s\n", file->dossier_parent->contenu[i]);
        contenue_size++;
        i++;
    }

    printf("counit size is %d\n", contenue_size);
    //Delete the file name in the parent's 'contenu' entry
    if (contenue_size > 0)
    {
        for (i = 0; i < contenue_size; i++)
        {
            if (strcmp(file->nom, file->dossier_parent->contenu[i]) == 0)
            {
                file->dossier_parent->contenu[i] = "[DELETED]";
                file->nom = "[DELETED]";
                break;
            }
        }
    }
    /* 
    //Delete the file's I-node
    file->bloc->nbre_fichiers;/
    printf("ddd %d\n", file->bloc->nbre_fichiers);
    */
    hdd.super_block.nbre_fichiers--;
    hdd.super_block.nbre_inodes--;
    hdd.super_block.count_used_inodes--;
    //free(file->contenu);
    /*
    if (file->bloc->nbre_fichiers == 0)
        hdd.super_block.count_used_blocks--;

    free(file); */
    printf("File '%s' has been deleted\n", name);
}
void mymv(t_fichier *file, t_fichier *dest)
{
    char *name = file->nom;
    int i = 0, j, contenue_size = 0;
    while (file->dossier_parent->contenu[i] != NULL)
    {
        contenue_size++;
        i++;
    }

    //Delete the file name in the parent's 'contenu' entry
    if (contenue_size > 0)
    {
        for (i = 0; i < contenue_size; i++)
        {
            if (strcmp(file->nom, file->dossier_parent->contenu[i]) == 0)
            {
                file->dossier_parent->contenu[i] = "[DELETED]";
                file->dossier_parent = dest;
                int count_fichiers = 0;
                int j = 0;
                while (dest->contenu[j] != NULL)
                {
                    j++;
                    count_fichiers++;
                }
                dest->contenu = (char **)realloc(dest->contenu, sizeof(char *) * (count_fichiers+1));
                
                dest->contenu[count_fichiers] = (char *)malloc(sizeof(char) * (strlen(file->nom) + 1));
                strcpy(dest->contenu[count_fichiers], file->nom);
                printf("Le fichier %s 'a été déplacé.\n", file->nom);
                break;
            }
        }
    }
}
void chkdsk()
{
    printf("____________________________________________________\n");
    printf("Hard disk drive informations:\n");
    printf("\tSIZE:                             %d octets\n", HDD_SIZE);
    printf("\tFREE SIZE:                        TODO\n");
    printf("\tNOMBRE DE BLOCS:                  %d blocs\n", NBRE_BLOCK);
    printf("\tNOMBRE D'I-NODES:                 %d inodes\n", hdd.super_block.nbre_inodes);
    printf("\tTAILLE D'UN BLOC:                 %d octets\n", BLOCK_SIZE);
    printf("\tNombre de blocs Libres:           %d\n", NBRE_BLOCK - hdd.super_block.count_used_blocks);
    printf("\tNombre de blocs pleins:           %d\n", hdd.super_block.count_used_blocks);
    printf("\tNombre de fichiers sur le HDD:    %d\n", hdd.super_block.nbre_fichiers);
    printf("____________________________________________________\n");
}
