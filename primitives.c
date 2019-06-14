#include "primitives.h"
char prompt[100] = "myshell@user~:";
char chemin[100] = "/user";
inode_t* mycreat(char *nom, char mode[11])
{
    t_fichier *tmpCurrentDir=hdd.current_dir;
    char* name=NULL;
    BOOL continuer = true;
    if (hdd.super_block.nbre_fichiers > 1)
    {
        BOOL test=check_current_dir(nom, &tmpCurrentDir, &name);
        if (tmpCurrentDir->contenu[0] != NULL)
        {
            int i;
            for(i=0;i<tmpCurrentDir->nbr_fichier;i++)
            {
                if (test==true && strcmp(nom, tmpCurrentDir->contenu[i]) == false)
                {
                    printf("Error: /!\\ Un fichier existe déja avec le nom '%s'\n", name);
                    continuer = false;
                }
            }
        }
    }
    else
    {
        name=(char*)malloc(sizeof(char)*strlen(nom)+1);
        strcpy(name,nom);
    }
    
    if (continuer)
    {
        ajouter_fichier_dans_bloc(0);
        hdd.tab_blocs[hdd.super_block.count_used_blocks].files[hdd.tab_blocs[hdd.super_block.count_used_blocks].nbre_fichiers - 1]->nom = (char *)malloc(sizeof(char) * (strlen(name) + 1));
        hdd.tab_blocs[hdd.super_block.count_used_blocks].files[hdd.tab_blocs[hdd.super_block.count_used_blocks].nbre_fichiers - 1]->contenu = (char **)malloc(sizeof(char *));
        
        hdd.super_block.nbre_inodes++;
        
        hdd.tab_inodes = (inode_t**)realloc(hdd.tab_inodes, sizeof(inode_t*) * hdd.super_block.nbre_inodes);
        hdd.tab_inodes[hdd.super_block.nbre_inodes -1] = (inode_t*)malloc(sizeof(inode_t));
        hdd.tab_inodes[hdd.super_block.nbre_inodes -1]->numero_inode = hdd.super_block.nbre_inodes -1;
        hdd.tab_inodes[hdd.super_block.nbre_inodes -1]->file_type = NORMAL;
        hdd.tab_inodes[hdd.super_block.nbre_inodes -1]->file_size = 0;
        hdd.tab_inodes[hdd.super_block.nbre_inodes -1]->nbre_links = 0;
        hdd.tab_inodes[hdd.super_block.nbre_inodes -1]->ouverture=O_CLOSE;
        strcpy(hdd.tab_inodes[hdd.super_block.nbre_inodes -1]->mode, mode);
        
        hdd.tab_blocs[hdd.super_block.count_used_blocks].files[hdd.tab_blocs[hdd.super_block.count_used_blocks].nbre_fichiers - 1]->inode = hdd.tab_inodes[hdd.super_block.nbre_inodes -1];
        
        strcpy(hdd.tab_blocs[hdd.super_block.count_used_blocks].files[hdd.tab_blocs[hdd.super_block.count_used_blocks].nbre_fichiers - 1]->nom, name);
        hdd.tab_blocs[hdd.super_block.count_used_blocks].files[hdd.tab_blocs[hdd.super_block.count_used_blocks].nbre_fichiers - 1]->bloc = &(hdd.tab_blocs[hdd.super_block.count_used_blocks]);
        
        
        hdd.tab_blocs[hdd.super_block.count_used_blocks].files[hdd.tab_blocs[hdd.super_block.count_used_blocks].nbre_fichiers - 1]->nbr_fichier = 0;
        hdd.tab_blocs[hdd.super_block.count_used_blocks].files[hdd.tab_blocs[hdd.super_block.count_used_blocks].nbre_fichiers - 1]->nbr_ligne = 0;
        
        if (hdd.super_block.nbre_fichiers > 1)
        {
            tmpCurrentDir->nbr_fichier++;
            tmpCurrentDir->nbr_ligne++;
            tmpCurrentDir->contenu = (char **)realloc(tmpCurrentDir->contenu, sizeof(char *) * tmpCurrentDir->nbr_fichier);
            
            tmpCurrentDir->contenu[tmpCurrentDir->nbr_fichier-1] = (char *)malloc(sizeof(char) * (strlen(name) + 1));
            strcpy(tmpCurrentDir->contenu[tmpCurrentDir->nbr_fichier-1], name);
        }
        
        if (hdd.tab_blocs[0].nbre_fichiers > 1)
        {
            hdd.tab_blocs[hdd.super_block.count_used_blocks].files[hdd.tab_blocs[hdd.super_block.count_used_blocks].nbre_fichiers - 1]->dossier_parent = tmpCurrentDir;
        }
    }
    
    return hdd.tab_blocs[hdd.super_block.count_used_blocks].files[hdd.tab_blocs[hdd.super_block.count_used_blocks].nbre_fichiers - 1]->inode;
}

void mylink(char* nom1, char* nom2){

	BOOL continuer = False;
    t_fichier *tmpCurrentDirSource=NULL;
    t_fichier *tmpCurrentDirTarget=NULL;
    char* name1=NULL;
    char* name2=NULL;
    BOOL test=check_current_dir(nom1, &tmpCurrentDirSource, &name1);
    check_current_dir(nom2, &tmpCurrentDirTarget, &name2);
    if (hdd.super_block.nbre_fichiers > 1)
    {
        for(int i=0;i<tmpCurrentDirSource->nbr_fichier;i++)
        {
            if(test==true && strcmp(tmpCurrentDirSource->contenu[i],name1)==0)
            {
                continuer=true;
                break;
            }
        }
    }
    else
    {
		continuer = True;
    }
	if(continuer == True)
	{
		//On test si le disque est plein
		if (hdd.super_block.count_used_blocks < NBRE_BLOCK)
		{
			//On teste si la taille du fichier peur entrer dans le dernier bloc non plein
			if (bloc_plein(0))
				hdd.super_block.count_used_blocks++;

			hdd.tab_blocs[hdd.super_block.count_used_blocks].nbre_links++;
			hdd.super_block.nbre_links++;
			hdd.tab_blocs[hdd.super_block.count_used_blocks].links = (t_link **)realloc(hdd.tab_blocs[hdd.super_block.count_used_blocks].links, sizeof(t_link *) * hdd.tab_blocs[hdd.super_block.count_used_blocks].nbre_links); // +1 car le compteur commence à 0
			hdd.tab_blocs[hdd.super_block.count_used_blocks].links[hdd.tab_blocs[hdd.super_block.count_used_blocks].nbre_links - 1] = (t_link *)malloc(sizeof(t_link));
		
            hdd.tab_blocs[hdd.super_block.count_used_blocks].links[hdd.tab_blocs[hdd.super_block.count_used_blocks].nbre_links - 1]->file = file_exist(name1, tmpCurrentDirSource);
			hdd.tab_blocs[hdd.super_block.count_used_blocks].links[hdd.tab_blocs[hdd.super_block.count_used_blocks].nbre_links - 1]->name = (char *)malloc(sizeof(char) * (strlen(name2) + 1));
			strcpy(hdd.tab_blocs[hdd.super_block.count_used_blocks].links[hdd.tab_blocs[hdd.super_block.count_used_blocks].nbre_links - 1]->name, name2);
            
            hdd.tab_blocs[hdd.super_block.count_used_blocks].files[hdd.tab_blocs[hdd.super_block.count_used_blocks].nbre_fichiers - 1]->nbr_fichier = 0;
			if (hdd.super_block.nbre_fichiers > 1)
			{
                tmpCurrentDirTarget->nbr_fichier++;
                tmpCurrentDirTarget->contenu = (char **)realloc(tmpCurrentDirTarget->contenu, sizeof(char *) * tmpCurrentDirTarget->nbr_fichier);
                tmpCurrentDirTarget->contenu[tmpCurrentDirTarget->nbr_fichier-1] = (char *)malloc(sizeof(char) * (strlen(name2) + 1));
                strcpy(tmpCurrentDirTarget->contenu[tmpCurrentDirTarget->nbr_fichier-1], name2);
			}
		}
		else
		{
			printf("Disque dur plein!! exiting..\n");
			exit(1);
		}
		hdd.tab_blocs[hdd.super_block.count_used_blocks].links[hdd.tab_blocs[hdd.super_block.count_used_blocks].nbre_links - 1]->file->inode->nbre_links++;
	}
}

t_fichier* myopen(char* nom, mode_tt ouverture)
{
    t_fichier *file=NULL;
    t_fichier *tmpCurrentDir=hdd.current_dir;
    char* name=NULL;
    int test=check_current_dir(nom, &tmpCurrentDir, &name);
    if(test==true)
    {
        file = file_exist(name, tmpCurrentDir);
        if(file->nbr_fichier==-1)
            return NULL;
        else
        {
            file->inode->ouverture=ouverture;
            return file;
        }
    }
    else
        return NULL;
}

void mywrite(char * text, t_fichier *file)
{
    file->nbr_ligne++;
    file->contenu = (char**)realloc(file->contenu,sizeof(char*)* file->nbr_ligne);
    file->contenu[file->nbr_ligne-1] = (char*)malloc(sizeof(char)*strlen(text)+1);
    strcpy(file->contenu[file->nbr_ligne-1],text);
}

void myread(t_fichier *file)
{
    for(int j=0;j<file->nbr_ligne;j++)
    {
        printf("%s \n",file->contenu[j]);
    }
}

void myclose(t_fichier *file)
{
    file->inode->ouverture=O_CLOSE;
}

void echo(char** saisi_utilisateur,int nbr_mot)
{
    t_fichier *file=NULL;
    t_fichier *tmpCurrentDir=NULL;
    char* name=NULL;
    int redirection=-1,i;
    for(i=1;i<nbr_mot;i++)
    {
        if(strcmp(saisi_utilisateur[i],">")==0)
            redirection=i;
    }
    
    if(redirection>-1)
    {
        check_current_dir(saisi_utilisateur[nbr_mot-1], &tmpCurrentDir, &name);
        file=myopen(name,O_WONLY);
        if(file==NULL)
        {
            mycreat(saisi_utilisateur[nbr_mot-1],"-rw-r--r---");
            file=myopen(name,O_WONLY);
            
        }
        if(file->inode->ouverture==O_WONLY)
        {
            char * text=saisi_utilisateur[1];
            for(int i=2;i<redirection;i++)
            {
                strcat(text," ");
                strcat(text,saisi_utilisateur[i]);
            }
            mywrite(text,file);
        }
        else
            printf("Impossible d'écrire dans ce fichier\n");
        myclose(file);
    }
    else
    {
        for(i=1;i<nbr_mot;i++)
            printf("%s ",saisi_utilisateur[i]);
        printf("\n");
    }
}

void cat(char *nom)
{
    t_fichier *file=NULL;
    t_fichier *tmpCurrentDir=hdd.current_dir;
    char* name=NULL;
    check_current_dir(nom, &tmpCurrentDir, &name);
    file=myopen(name,O_RONLY);
    if(file==NULL)
        printf("Aucun fichier existant\n");
    else
    {
        if(file->inode->file_type!=NORMAL)
            printf("Ce fichier n'existe pas\n");
        else
        {
            if(file->inode->ouverture==O_RONLY)
                myread(file);
        }
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
    if (directory->inode->file_type == DIRECTORY)
    {
        for (i = 0; i < directory->bloc->nbre_fichiers; i++)
        {
            for(int j=0;j<directory->nbr_fichier;j++)
            {
                if (strcmp(file_name, directory->bloc->files[i]->nom)==0 && directory->bloc->files[i]->inode->file_type != DELETED)
                { // 0 => files are equal
                    return directory->bloc->files[i];
                }
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

void ls()
{
    for(int j=0;j<hdd.current_dir->nbr_fichier;j++)
    {
        
        if (strcmp(hdd.current_dir->contenu[j],"[DELETED]")!=0)
        {
            printf("%s ", hdd.current_dir->contenu[j]);
        }
    }
    printf("\n");
}

BOOL existance_fichier(char *name)
{
    //BOOL existance = false;
    for (int i = 0; i < hdd.tab_blocs[hdd.super_block.count_used_blocks].files[hdd.current_dir->nbr_fichier]->nbr_fichier; i++)
    {
        if (strcmp(hdd.tab_blocs[hdd.super_block.count_used_blocks].files[hdd.current_dir->nbr_fichier]->contenu[i], name)==0)
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
            if (hdd.tab_blocs[i].files[j]->inode->file_type == DIRECTORY && strstr(hdd.tab_blocs[i].files[j]->nom, name))
                return j;
        }
    }
    return -1;
}
void pwd()
{
    printf("%s\n",chemin);
}

void cd(char *name)
{
    if (name != NULL)
    {
        if (strcmp(name, "..")==0)
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
                if (testExist->inode->file_type == DIRECTORY)
                {
                    char next_prompt[100] = "/";
                    strcat(next_prompt, testExist->nom);
                    strcat(prompt, next_prompt);
                    strcat(chemin, next_prompt);
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
    hdd.tab_inodes[hdd.super_block.nbre_inodes -1]->file_type = DIRECTORY;
}

int recup_saisi(char ***saisi_utilisateur)
{
    char tmp[100];
    char mot[10][50];
    int i, nbr_mot = 0;
    int taille = 0;
    printf("%s $ ", prompt);
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


void init_hdd()
{
    hdd.tab_blocs = NULL;

    hdd.total_size = HDD_SIZE;
    hdd.super_block.nbre_fichiers = 0;
    hdd.super_block.nbre_links = 0;
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

    hdd.tab_inodes = (inode_t**)malloc(sizeof(inode_t*));
    mymkdir(root_folder_name);
    hdd.current_dir = hdd.tab_blocs[0].files[0];
    hdd.tab_blocs[0].files[0]->inode->file_type = DIRECTORY;
    printf("[Boot] Taille Disque: %d octets...\n", HDD_SIZE);
    hdd.current_dir->nbr_fichier = 0;
    printf("[Boot] Taille Disque: %d octets...\n", HDD_SIZE);
    hdd.current_dir->inode->file_type = DIRECTORY;
    printf("[Boot] Taille Disque: %d octets...\n", HDD_SIZE);
}

void allouer_blocs()
{
    int i;
    hdd.tab_blocs = (block_t *)malloc(NBRE_BLOCK * sizeof(block_t));

    for (i = 0; i < NBRE_BLOCK; i++)
    {
        hdd.tab_blocs[i].nbre_fichiers = 0;
        hdd.tab_blocs[i].files = (t_fichier **)malloc(sizeof(t_fichier*) * hdd.tab_blocs[hdd.super_block.count_used_blocks].nbre_fichiers);
        hdd.tab_blocs[i].files[0] = (t_fichier *)malloc(sizeof(t_fichier));
        hdd.tab_blocs[i].links = (t_link **)malloc(sizeof(t_link*) * hdd.tab_blocs[hdd.super_block.count_used_blocks].nbre_links);
        hdd.tab_blocs[i].links[0] = (t_link *)malloc(sizeof(t_link));
    }

    printf("%d BLOCK ont été crées\n", BLOCK_SIZE);
}

void mycp(char *nom1, char *nom2)
{
    BOOL continuer = false;
    t_fichier *tmpCurrentDirSource=NULL;
    t_fichier *tmpCurrentDirTarget=NULL;
    char* name1=NULL;
    char* name2=NULL;
    BOOL test=check_current_dir(nom1, &tmpCurrentDirSource, &name1);
    check_current_dir(nom2, &tmpCurrentDirTarget, &name2);
    if (hdd.super_block.nbre_fichiers > 1)
    {
        for(int i=0;i<tmpCurrentDirSource->nbr_fichier;i++)
        {
            if(test==true && strstr(tmpCurrentDirSource->contenu[i],name1))
            {
                continuer=true;
                break;
            }
        }
    }
    if(continuer == true)
    {
        mycreat(nom2,"-rw-r--r---");
        for(int j=0;j<tmpCurrentDirSource->bloc->nbre_fichiers;j++)
        {
            if(strcmp(tmpCurrentDirSource->bloc->files[j]->nom,name1)==0)
            {
                tmpCurrentDirTarget->bloc->files[tmpCurrentDirTarget->bloc->nbre_fichiers-1]->inode->file_type=tmpCurrentDirSource->bloc->files[j]->inode->file_type;
                tmpCurrentDirTarget->bloc->files[tmpCurrentDirTarget->bloc->nbre_fichiers-1]->inode->ouverture=tmpCurrentDirSource->bloc->files[j]->inode->ouverture;
                strcpy(tmpCurrentDirTarget->bloc->files[tmpCurrentDirTarget->bloc->nbre_fichiers-1]->inode->mode,tmpCurrentDirSource->bloc->files[j]->inode->mode);
                tmpCurrentDirTarget->bloc->files[tmpCurrentDirTarget->bloc->nbre_fichiers-1]->inode->date_created=tmpCurrentDirSource->bloc->files[j]->inode->date_created;
                tmpCurrentDirTarget->bloc->files[tmpCurrentDirTarget->bloc->nbre_fichiers-1]->inode->last_access=tmpCurrentDirSource->bloc->files[j]->inode->last_access;
                tmpCurrentDirTarget->bloc->files[tmpCurrentDirTarget->bloc->nbre_fichiers-1]->inode->last_modified=tmpCurrentDirSource->bloc->files[j]->inode->last_modified;
                tmpCurrentDirTarget->bloc->files[tmpCurrentDirTarget->bloc->nbre_fichiers-1]->inode->file_size=tmpCurrentDirSource->bloc->files[j]->inode->file_size;
                tmpCurrentDirTarget->bloc->files[tmpCurrentDirTarget->bloc->nbre_fichiers-1]->inode->user=tmpCurrentDirSource->bloc->files[j]->inode->user;
                tmpCurrentDirTarget->bloc->files[tmpCurrentDirTarget->bloc->nbre_fichiers-1]->nbr_ligne=tmpCurrentDirSource->bloc->files[j]->nbr_ligne;
                tmpCurrentDirTarget->bloc->files[tmpCurrentDirTarget->bloc->nbre_fichiers-1]->nbr_fichier=tmpCurrentDirSource->bloc->files[j]->nbr_fichier;
                tmpCurrentDirTarget->bloc->files[tmpCurrentDirTarget->bloc->nbre_fichiers-1]->contenu=(char**)realloc(tmpCurrentDirTarget->bloc->files[tmpCurrentDirTarget->bloc->nbre_fichiers-1]->contenu,sizeof(char*)*tmpCurrentDirSource->bloc->files[j]->nbr_fichier);
                
                for(int k=0;k<tmpCurrentDirSource->bloc->files[j]->nbr_ligne;k++)
                {
                    tmpCurrentDirTarget->bloc->files[tmpCurrentDirTarget->bloc->nbre_fichiers-1]->contenu[k]=(char*)malloc(sizeof(char)*strlen(tmpCurrentDirSource->bloc->files[j]->contenu[k])+1);
                    strcpy(tmpCurrentDirTarget->bloc->files[tmpCurrentDirTarget->bloc->nbre_fichiers-1]->contenu[k],tmpCurrentDirSource->bloc->files[j]->contenu[k]);
                }
                break;
            }
        }
    }
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
                    if ((current_block.files[j]->inode->file_type == DIRECTORY && must_be_directory == true) || must_be_directory == false)
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
        printf("Error: File  not found\n"/*, string*/);

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
    char *last_folder1;
    char *last_folder2;
    int i, pos_last_slash1,pos_last_slash2;

    //find the last slash position
    for (i = 0; i < strlen(prompt); i++)
    {
        last_folder1 = strrchr(prompt, '/');
        if (strcmp(&prompt[i], last_folder1) == 0)
        {
            pos_last_slash1 = i;
            break;
        }
    }
    for (int j = 0; j < strlen(chemin); j++)
    {
        last_folder2 = strrchr(chemin, '/');
        if (strcmp(&chemin[j], last_folder2) == 0)
        {
            pos_last_slash2 = j;
            break;
        }
    }
    prompt[pos_last_slash1] = '\0';
    chemin[pos_last_slash2] = '\0';
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
    //char *source_is_full_path = strtok(temp, "/");
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



BOOL bloc_plein(int file_size)
{
    //TODO tester si la taille du fichier peut entrez avec l'espace restant sur le bloc ou si le bloc est déja plein
    float block_used_size = 0;
    int i;
    for (i = 0; i < hdd.tab_blocs[hdd.super_block.count_used_blocks].nbre_fichiers; i++)
    {
        block_used_size += hdd.tab_blocs[hdd.super_block.count_used_blocks].files[i]->inode->file_size;
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
void myrm(char *nom)
{
    t_fichier *file=NULL;
    t_fichier *tmpCurrentDir=NULL;
    char* name=NULL;
    int test=check_current_dir(nom, &tmpCurrentDir, &name);
    if(test==true)
    {
        file= file_exist(name, tmpCurrentDir);
        if(file->nbr_fichier!=-1)
        {
            for(int i=0; i<file->dossier_parent->nbr_fichier;i++)
            {
                if(strcmp(file->dossier_parent->contenu[i],name)==0)
                {
                    file->inode->file_type = DELETED;
                    file->nom = "";
                    file->dossier_parent->contenu[i]=(char*)realloc(file->dossier_parent->contenu[i],sizeof(char)*strlen("[DELETED]")+1);
                    strcpy(file->dossier_parent->contenu[i],"[DELETED]");
                    printf("File '%s' has been deleted\n", name);
                }
            }
            for(int j=0;j<file->dossier_parent->bloc->nbre_fichiers;j++)
            {
                if(strstr(file->dossier_parent->bloc->files[j]->nom,name))
                {
                    file->dossier_parent->bloc->files[j] = NULL;
                }
            }
        }
    }
    else
        printf("Erreur dans le chemin\n");
}

void mymv(char* nom1, char* nom2)
{
    t_fichier *tmpCurrentDirSource=NULL;
    t_fichier *tmpCurrentDirTarget=NULL;
    t_fichier *file=NULL;
    char* name1=NULL;
    char* name2=NULL;
    BOOL test=check_current_dir(nom1, &tmpCurrentDirSource, &name1);
    check_current_dir(nom2, &tmpCurrentDirTarget, &name2);
    if (hdd.super_block.nbre_fichiers > 1)
    {
        for(int i=0;i<tmpCurrentDirSource->nbr_fichier;i++)
        {
            if(test==true && strcmp(tmpCurrentDirSource->contenu[i],name1)==0)
            {
                tmpCurrentDirSource->contenu[i]=(char*)realloc(tmpCurrentDirSource->contenu[i],sizeof(char)*strlen("[DELETED]")+1);
                strcpy(tmpCurrentDirSource->contenu[i],"[DELETED]");
                for(int j=0;j<tmpCurrentDirSource->bloc->nbre_fichiers;j++)
                {
                    if(strcmp(tmpCurrentDirSource->bloc->files[j]->nom,name1)==0)
                    {
                        file=file_exist(name2,tmpCurrentDirTarget);
                        if(file->nbr_fichier!=-1)
                        {
                            if(file_exist(name2,tmpCurrentDirTarget)->inode->file_type==DIRECTORY)
                            {
                                tmpCurrentDirTarget=file_exist(name2,tmpCurrentDirTarget);
                                tmpCurrentDirSource->bloc->files[j]->dossier_parent=tmpCurrentDirTarget;
                                tmpCurrentDirSource->bloc->files[j]->nom=(char*)realloc(tmpCurrentDirSource->bloc->files[j]->nom,sizeof(char)*strlen(name1)+1);
                                strcpy(tmpCurrentDirSource->bloc->files[j]->nom,name1);
                                tmpCurrentDirTarget->nbr_fichier++;
                                tmpCurrentDirTarget->contenu=(char**)realloc(tmpCurrentDirTarget->contenu,sizeof(char*)*tmpCurrentDirTarget->nbr_fichier);
                                tmpCurrentDirTarget->contenu[tmpCurrentDirTarget->nbr_fichier-1]=(char*)malloc(sizeof(char)*strlen(name1)+1);
                                strcpy(tmpCurrentDirTarget->contenu[tmpCurrentDirTarget->nbr_fichier-1],name1);
                            }
                            else
                            {
                                tmpCurrentDirSource->bloc->files[j]->dossier_parent=tmpCurrentDirTarget;
                                tmpCurrentDirSource->bloc->files[j]->nom=(char*)realloc(tmpCurrentDirSource->bloc->files[j]->nom,sizeof(char)*strlen(name2)+1);
                                strcpy(tmpCurrentDirSource->bloc->files[j]->nom,name2);
                                tmpCurrentDirTarget->nbr_fichier++;
                                tmpCurrentDirTarget->contenu=(char**)realloc(tmpCurrentDirTarget->contenu,sizeof(char*)*tmpCurrentDirTarget->nbr_fichier);
                                tmpCurrentDirTarget->contenu[tmpCurrentDirTarget->nbr_fichier-1]=(char*)malloc(sizeof(char)*strlen(name2)+1);
                                strcpy(tmpCurrentDirTarget->contenu[tmpCurrentDirTarget->nbr_fichier-1],name2);
                            }
                        }
                        else
                        {
                            tmpCurrentDirSource->bloc->files[j]->dossier_parent=tmpCurrentDirTarget;
                            tmpCurrentDirSource->bloc->files[j]->nom=(char*)realloc(tmpCurrentDirSource->bloc->files[j]->nom,sizeof(char)*strlen(name2)+1);
                            strcpy(tmpCurrentDirSource->bloc->files[j]->nom,name2);
                            tmpCurrentDirTarget->nbr_fichier++;
                            tmpCurrentDirTarget->contenu=(char**)realloc(tmpCurrentDirTarget->contenu,sizeof(char*)*tmpCurrentDirTarget->nbr_fichier);
                            tmpCurrentDirTarget->contenu[tmpCurrentDirTarget->nbr_fichier-1]=(char*)malloc(sizeof(char)*strlen(name2)+1);
                            strcpy(tmpCurrentDirTarget->contenu[tmpCurrentDirTarget->nbr_fichier-1],name2);
                        }
                        
                    }
                }
                break;
            }
        }
    }
}


void myrmdir(char *nom)
{
    
    t_fichier *file=NULL;
    t_fichier *tmpCurrentDir=NULL;
    char* name=NULL;
    int test=check_current_dir(nom, &tmpCurrentDir, &name);
    int i = 0, contenue_size = 0;
    if(test==true)
    {
        file= file_exist(name, tmpCurrentDir);
        if(file->nbr_fichier!=-1)
        {
            while (file->contenu[i] != NULL)
            {
                printf("%s\n", file->contenu[i]);
                if(strcmp(file->contenu[i],"[DELETED]")!=0)
                    contenue_size++;
                i++;
            }
    
            printf("counit size is %d\n", contenue_size);
            if (contenue_size == 0)
            {
                for(int i=0; i<file->dossier_parent->nbr_fichier;i++)
                {
                    if(strstr(file->dossier_parent->contenu[i],name))
                    {
                        file->inode->file_type = DELETED;
                        file->nom = "";
                        file->dossier_parent->contenu[i]=(char*)realloc(file->dossier_parent->contenu[i],sizeof(char)*strlen("[DELETED]")+1);
                        strcpy(file->dossier_parent->contenu[i],"[DELETED]");
                        printf("Directory '%s' has been deleted\n", name);
                    }
                }
                for(int j=0;j<file->dossier_parent->bloc->nbre_fichiers;j++)
                {
                    if(strstr(file->dossier_parent->bloc->files[j]->nom,name))
                    {
                        file->dossier_parent->bloc->files[j] = NULL;
                    }
                }
                
            }else
            {
                printf("Directory not empty \n");
            }
        }
    }
}


BOOL check_current_dir(char* nom, t_fichier **newCurrentDir, char** name)
{
    char tmp[50][50];
    int j=0;
    int debut=0;
    int taille=0;
    BOOL test=false;
    if(nom[0]=='/')
    {
        for(int i=1;i<strlen(nom);i++)
        {
            if(nom[i]=='/')
            {
                tmp[j][i-1-taille]='\0';
                taille+=(strlen(tmp[j])+1);
                j++;
            }
            else
                tmp[j][i-1-taille]=nom[i];
            if(i==strlen(nom)-1)
                tmp[j][i-taille]='\0';
        }
        
        if(strstr(tmp[0],"user"))
        {
            *newCurrentDir=hdd.tab_blocs[0].files[0];
            debut=1;
        }
        else
            *newCurrentDir=hdd.current_dir;
    }
    else
    {
        for(int i=0;i<strlen(nom);i++)
        {
            if(nom[i]=='/')
            {
                tmp[j][i-taille]='\0';
                taille+=(strlen(tmp[j])+1);
                j++;
            }
            else
                tmp[j][i-taille]=nom[i];
            if(i==strlen(nom)-1)
                tmp[j][i+1-taille]='\0';
        }
        
        *newCurrentDir=hdd.current_dir;
    }
    for(int k=debut;k<j+1;k++)
    {
        test=false;
        if(k==j)
        {
            *name=(char*)malloc(sizeof(char)*strlen(tmp[k])+1);
            strcpy(*name,tmp[k]);
            test=true;
        }
        for(int l=0;l<(*newCurrentDir)->nbr_fichier;l++)
        {
            if(strcmp((*newCurrentDir)->contenu[l],tmp[k])==0)
            {
                for(int m=0;m<hdd.super_block.nbre_inodes;m++)
                {
                    for(int n=0;n<hdd.super_block.count_used_blocks+1;n++)
                    {
                        for(int o=0;o<hdd.tab_blocs[n].nbre_fichiers;o++)
                        {
                            if(hdd.tab_inodes[m]==hdd.tab_blocs[n].files[o]->inode && hdd.tab_blocs[n].files[o]->inode->file_type==DIRECTORY && k<j && hdd.tab_blocs[n].files[o]->dossier_parent==*newCurrentDir && strcmp(hdd.tab_blocs[n].files[o]->nom,tmp[k])==0)
                            {
                                *newCurrentDir=hdd.tab_blocs[n].files[o];
                                o=hdd.tab_blocs[n].nbre_fichiers;
                                n=hdd.super_block.count_used_blocks;
                                m=hdd.super_block.nbre_inodes;
                                l=(*newCurrentDir)->nbr_fichier;
                                test=true;
                            }
                        }
                    }
                }
            }
        }
        if(test==false)
            return false;
    }
    return test;
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

void saveHDD()
{
    DIR* pDir;
    pDir = opendir("save");
    
    if(pDir == NULL)
    {
        mkdir("save", 0777);
    }
    
    FILE* fichier = NULL;
    
    fichier = fopen("save/hdd.txt", "w+");
    
    if(fichier != NULL)
    {
        
        int i, j, numero_parent, cpt;
        
        fprintf(fichier,
                "%d\n%d\n%d\n%f\n%d\n%d\n%d\n\n",
                hdd.total_size,
                hdd.super_block.count_used_blocks,
                hdd.super_block.count_used_inodes,
                hdd.super_block.free_disk_size,
                hdd.super_block.nbre_fichiers,
                hdd.super_block.nbre_links,
                hdd.super_block.nbre_inodes);
        
        for(i=0; i<NBRE_BLOCK; i++)
        {
            for(j=0; j<hdd.tab_blocs[i].nbre_fichiers; j++)
            {
                if(hdd.tab_blocs[0].files[0]->dossier_parent == NULL)
                {
                    numero_parent = -1;
                }
                else
                {
                    numero_parent = hdd.tab_blocs[i].files[j]->dossier_parent->inode->numero_inode;
                }
                
                fprintf(fichier,
                        "%d\n%d\n%d\n%d\n%d\n%s\n%d\n%d\n%d\n%s\n%d\n",
                        hdd.tab_blocs[i].files[j]->inode->numero_inode,
                        numero_parent,
                        hdd.tab_blocs[i].files[j]->inode->file_type,
                        hdd.tab_blocs[i].files[j]->inode->ouverture,
                        hdd.tab_blocs[i].files[j]->inode->file_size,
                        hdd.tab_blocs[i].files[j]->inode->mode,
                        hdd.tab_blocs[i].files[j]->inode->nbre_links,
                        hdd.tab_blocs[i].files[j]->inode->user.uid,
                        hdd.tab_blocs[i].files[j]->inode->user.gid,
                        hdd.tab_blocs[i].files[j]->nom,
                        hdd.tab_blocs[i].files[j]->nbr_ligne
                        );
                for(cpt=0; cpt<hdd.tab_blocs[i].files[j]->nbr_ligne; cpt++)
                {
                    fprintf(fichier,
                            "%s\n",
                            hdd.tab_blocs[i].files[j]->contenu[cpt]
                            );
                }
                fprintf(fichier, "\n");
                
            }
            for(j=0; j<hdd.tab_blocs[i].nbre_links; j++)
            {
                fprintf(fichier,
                        "%s\n%d\n\n",
                        hdd.tab_blocs[i].links[j]->name,
                        hdd.tab_blocs[i].links[j]->file->inode->numero_inode
                        );
            }
        }
        
        fclose(fichier);
    }
    else
    {
        printf("Erreur lors de la sauvegarde");
        exit(1);
    }
}


void loadHDD()
{
    
    FILE* fichier = NULL;
    
    fichier = fopen("save/hdd.txt", "r");
    
    if(fichier != NULL)
    {
        int charactere;
        
        while((char)charactere != EOF)
        {
            charactere = fgetc(fichier);
            //sleep(0.5);
            printf("%c", charactere);
        }
        printf("\n");
        
        fclose(fichier);
    }
    else
    {
        printf("Erreur lors de la sauvegarde\n");
        exit(1);
    }
}
