#include "primitives.h"

int main(int nbarg, char* argv[])
{
    int i,nbr_mot=0;
    char** saisi_utilisateur=(char**)malloc(sizeof(char*));
    //Dans le boot() 2 fichiers (repertoires) sont crées sur l'inode N° 0 ( . et root)
    boot();
    
    saisi_utilisateur[0]=(char*)malloc(sizeof(char));
    
    do
    {
        nbr_mot=recup_saisi(&saisi_utilisateur);
        
        if(strstr(saisi_utilisateur[0],"touch"))
        {
            for(i=1;i<nbr_mot;i++)
                mycreat(saisi_utilisateur[i],"-rw-r--r---");
        }
        else if(strstr(saisi_utilisateur[0],"mkdir"))
        {
            for(i=1;i<nbr_mot;i++)
                mymkdir(saisi_utilisateur[i]);
        }
        else if(strstr(saisi_utilisateur[0],"ls"))
        {
            ls();
        }
        else if(strstr(saisi_utilisateur[0],"cd"))
        {
            cd(saisi_utilisateur[1]);
        }
        /*else if(strstr(saisi_utilisateur[0],"open"))
        {
            for(i=1;i<nbr_mot;i++)
                printf("inode %llu\n",myopen(saisi_utilisateur[i],S_IROTH | S_IWUSR | S_IRUSR | S_IRGRP | S_IFREG));
        }
        else if(strstr(saisi_utilisateur[0],"close"))
        {
            for(i=1;i<nbr_mot;i++)
                myclose(myopen(saisi_utilisateur[i],S_IROTH | S_IWUSR | S_IRUSR | S_IRGRP | S_IFREG));
        }*/
        chkdsk();
    }while(!strstr(saisi_utilisateur[0],"quitter"));


    //TEST DE LA CREATION D'UN FICHIER // ce fichier sera le 3ieme fichier de l'inode 0 et ainsi de suite jusu'a
    //ce que la taille de l'inode 0 soit totlament occupée ensuite l'inode 1, 2..
    
    //*****************TEST DE LA FONCTION DE RECHERCHE D'UN FICHIER
    /*printf("\n\nTEST DE LA FONCTION DE RECHERCHE D'UN FICHIER\n");
    if(file_exist("Un_ficier", &(hdd->current_dir))){
        printf("existe\n");
        //stuff
    }else
    {
        printf("n'existe pas\n");
    }*/
    
    return EXIT_SUCCESS;
}



