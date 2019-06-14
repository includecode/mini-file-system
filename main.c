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

        if (strcmp(saisi_utilisateur[0], "touch")==0)
        {
            for (i = 1; i < nbr_mot; i++)
                mycreat(saisi_utilisateur[i], "-rw-r--r---");
        }
        else if (strcmp(saisi_utilisateur[0], "mkdir")==0)
        {
            for (i = 1; i < nbr_mot; i++)
                mymkdir(saisi_utilisateur[i]);
        }
        else if (strcmp(saisi_utilisateur[0], "ls")==0)
        {
            ls();
        }
        else if (strcmp(saisi_utilisateur[0], "cd")==0)
        {
            cd(saisi_utilisateur[1]);
        }
        else if(strcmp(saisi_utilisateur[0], "link")==0)
        {
            if(nbr_mot==3)
                mylink(saisi_utilisateur[1], saisi_utilisateur[2]);
		}
        else if (strcmp(saisi_utilisateur[0], "cp")==0)
        {
            if(nbr_mot==3)
                mycp(saisi_utilisateur[1],saisi_utilisateur[2]);
        }
        else if (strcmp(saisi_utilisateur[0], "rm")==0)
        {
            for (i = 1; i < nbr_mot; i++)
                myrm(saisi_utilisateur[i]);
        }
        else if(strcmp(saisi_utilisateur[0], "rmdir")==0)
        {
            for (i = 1; i < nbr_mot; i++)
                myrmdir(saisi_utilisateur[i]);
        }
        else if(strcmp(saisi_utilisateur[0],"echo")==0)
        {
            echo(saisi_utilisateur,nbr_mot);
        }
        else if (strcmp(saisi_utilisateur[0],"cat")==0)
        {
            if(nbr_mot>1)
                cat(saisi_utilisateur[1]);
        }
        else if (strcmp(saisi_utilisateur[0], "mv")==0)
        {
            if(nbr_mot==3)
                mymv(saisi_utilisateur[1],saisi_utilisateur[2]);
        }
        else if (strcmp(saisi_utilisateur[0], "pwd")==0)
        {
            pwd();
        }
        else if (strcmp(saisi_utilisateur[0], "quit")==0)
        {
            printf("[Root] Saving HDD changes... \n");
            sleep(2);
            printf("[Root] Shutting down...\n");
            sleep(4);
            printf("[Root] ***GOOD BYE***\n");
            saveHDD();
            exit(EXIT_SUCCESS);
        }
        else if (strcmp(saisi_utilisateur[0], "df")==0)
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
