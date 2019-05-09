#include "primitives.h"
int main(int nbarg, char *argv[])
{
    int i, nbr_mot = 0;
    char **saisi_utilisateur = (char **)malloc(sizeof(char *));
    //Dans le boot() 1 fichier (repertoire) est crée sur l'inode N° 0 ( user)
    boot();

    saisi_utilisateur[0] = (char *)malloc(sizeof(char));

    do
    {

        nbr_mot = recup_saisi(&saisi_utilisateur);

        if (strstr(saisi_utilisateur[0], "touch"))
        {
            for (i = 1; i < nbr_mot; i++)
                mycreat(saisi_utilisateur[i], "-rw-r--r---");
        }
        else if (strstr(saisi_utilisateur[0], "mkdir"))
        {
            for (i = 1; i < nbr_mot; i++)
                mymkdir(saisi_utilisateur[i]);
        }
        else if (strstr(saisi_utilisateur[0], "ls"))
        {
            ls();
        }
        else if (strstr(saisi_utilisateur[0], "cd"))
        {
            cd(saisi_utilisateur[1]);
        }
        else if (strstr(saisi_utilisateur[0], "cp"))
        {
            if (begins_with_root_folder(saisi_utilisateur[1]))
            {
                char *source_file_name = explode_last_from_path(saisi_utilisateur[1]);
                t_fichier *fichier = file_exist(source_file_name, hdd.current_dir);
                t_fichier *source;
                if (fichier->nbr_fichier != -1)
                {
                    if (saisi_utilisateur[2] != NULL)
                    {
                        if (begins_with_root_folder(saisi_utilisateur[2]))
                        {
                            t_fichier *destUrl = url_exists(saisi_utilisateur[2], 'D');
                            if (destUrl != NULL)
                            {
                                t_fichier *source = url_exists(saisi_utilisateur[1], 'S');
                                if (source != NULL)
                                {
                                    //printf("s = %s d=%s\n", source->nom, destUrl->nom);
                                    if(source != hdd.tab_blocs[0].files[0])
                                        mycp(*source, destUrl);
                                    else
                                    {
                                        printf("Interdit de copier le dossier racine !\n");
                                    }
                                    
                                }
                            }
                        }
                        else
                        {
                            printf("File destination must be absolute path ! ex: %s\\folder1\\folder2  %s\\folder3\\folder4\n", root_folder_name);
                        }
                    }
                    else
                    {
                        printf("/!\\ Destination missing\n");
                    }
                }
                else
                {
                    free(fichier);
                    printf("Le fichier %s n'existe pas !\n", saisi_utilisateur[1]);
                }
            }
            else
            {
                printf("File source must be absolute path ! ex:  cp %s\\folder1\\folder2 %s\\folder3\\folder4\n", root_folder_name, root_folder_name);
            }
        }
        else if (strstr(saisi_utilisateur[0], "rm"))
        {
            t_fichier *testExist = file_exist(saisi_utilisateur[1], hdd.current_dir);
            if (testExist->nbr_fichier != -1)
            {
                myrm(testExist);
            }
            else
            {
                printf("File %s not found.\n", saisi_utilisateur[1]);
            }
        }
        else if (strstr(saisi_utilisateur[0], "mv"))
        {
            if (begins_with_root_folder(saisi_utilisateur[1]))
            {
                char *source_file_name = explode_last_from_path(saisi_utilisateur[1]);
                t_fichier *fichier = file_exist(source_file_name, hdd.current_dir);
                t_fichier *source;
                if (fichier->nbr_fichier != -1)
                {
                    if (saisi_utilisateur[2] != NULL)
                    {
                        if (begins_with_root_folder(saisi_utilisateur[2]))
                        {
                            t_fichier *destUrl = url_exists(saisi_utilisateur[2], 'D');
                            if (destUrl != NULL)
                            {
                                t_fichier *source = url_exists(saisi_utilisateur[1], 'S');
                                if (source != NULL)
                                {
                                    if(source != hdd.tab_blocs[0].files[0])
                                        mymv(source, destUrl);
                                    else
                                    {
                                        printf("Interdit de déplacer le dossier racine !\n");
                                    }
                                }
                            }
                        }
                        else
                        {
                            printf("File destination must be absolute path ! ex: %s\\folder1\\folder2  %s\\folder3\\folder4\n", root_folder_name, root_folder_name);
                        }
                    }
                    else
                    {
                        printf("/!\\ Destination missing\n");
                    }
                }
                else
                {
                    free(fichier);
                    printf("Le fichier %s n'existe pas !\n", saisi_utilisateur[1]);
                }
            }
            else
            {
                printf("File source must be absolute path ! ex:  cp %s\\folder1\\folder2 %s\\folder3\\folder4\n", root_folder_name);
            }
        }
        else if (strstr(saisi_utilisateur[0], "quit"))
        {
            printf("[Root] Saving HDD changes... \n");
            sleep(2);
            printf("[Root] Shutting down...\n");
            sleep(4);
            printf("[Root] ***GOOD BYE***\n");
            exit(EXIT_SUCCESS);
        }
        else if (strstr(saisi_utilisateur[0], "df"))
        {
            chkdsk();
        }
        else
        {
            printf("Commande inconnue\n");
        }
    } while (true);

    return EXIT_SUCCESS;
}
