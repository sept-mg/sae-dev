#include <stdio.h>
#include <string.h>
#pragma warning(disable: 4996)

enum //énumération des limites des chaines de caractères et du maximum d'entreprise de missions et de rapports
{
    LIMIT_STRING_COMPANY = 30,  //limite de taille du nom d'une entreprise
    LIMIT_STRING_MISSION = 30,  //limite de taille de nom d'une mission
    LIMIT_STRING = 100,         //limite de taille d'une chaine de caractère en général
    MAX_COMPANY = 50,           //nombre maximal d'entreprise
    MAX_MISSION = 500,          //nombre maximal de missions
    MAX_RAPPORT = 4,            //nombre maximal de rapport de mission (pour une seule mission)
};

enum //type d'entreprises
{
    OP = 1, //opérateur
    AG,     //agence
    IN,     //intervenant
};

float arroundFloat(float input) //fonction d'arrondi au centième près
{
    int temp = input * 1000;
    if (temp % 10 > 4)
    {
        temp = temp / 10 + 1;
    }
    else
    {
        temp = temp / 10;
    }
    return (float)temp / 100;
}

typedef struct //structure contenant le nom d'une entreprise et son type (OP,AG,IN)
{
    char name[LIMIT_STRING_COMPANY]; //tableau qui contiens le nom
    unsigned char companyType;       //type d'entreprise
} Company;

typedef struct //structure contenant toutes les informations sur 
{
    unsigned short id;                  //ID de la mission
    char name[LIMIT_STRING_MISSION];    //taille max du nom de la mission
    unsigned short company;             //l'entreprise qui a mis la mission en ligne
    float remuneration;                 //rémunération de la mission
    unsigned char state;                //état de la mission (attribuée non attribuée etc...)
    unsigned char subcontractingCount;  //nombre de fois que la mission a étée soustraitée
    short idSubcontracting;             //ID de la mission lors de sa sous traitance
    unsigned long rapport;              //ce qu'il s'est passé lors de la mission (code 0 1 2 3)
    char acceptedBy;                    //ID de l'entreprise qui a accepté la mission
} Mission;

unsigned char getInscriptionCompanyType(char input[LIMIT_STRING_COMPANY]) //fonction d'inscription du type d'entreprise
{
    if (strlen(input) != 2)
    {
        return 0;
    }

    if (input[0] == 'O' && input[1] == 'P') //OP (opérateur)
    {
        return OP;
    }

    else if (input[0] == 'A' && input[1] == 'G') //AG (agence)
    {
        return AG;
    }

    else if (input[0] == 'I' && input[1] == 'N') //IN (intervenant)
    {
        return IN;
    }
    return 0;
}

void showMission(Mission mission, char companyName[LIMIT_STRING_COMPANY]) //affiche une mission
{
    printf("%u %s %s %.2f (%u)\n", mission.id, mission.name, companyName, mission.remuneration, mission.subcontractingCount);
    // company[mission[k].company].name
}

void showRapport(unsigned long rapport) //affiche le type de problèmes qui ont eus lieu pendant la mission grace au code de retour
{
    while (rapport > 0)
    {
        switch (rapport % 10)
        {
        case 1:
            printf("Local non accessible\n");
            break;

        case 2:
            printf("Pas de signal dans le boitier general\n");
            break;

        case 3:
            printf("Recepteur defectueux\n");
            break;
        default:
            break;
        }
        rapport = rapport / 10;
    }

}


int main()
{
    //définition des variables des tableaux 

    char on = 1;
    char input[LIMIT_STRING] = "";

    Company company[MAX_COMPANY];
    unsigned char countCompany = 0;

    Mission mission[MAX_MISSION];
    unsigned short countMission = 0;
    const float rapportRem[MAX_RAPPORT] = { 1.0f, 1.0f, 1.055f, 1.04f };

    while (on) //tant que on = 1 on laisse tourner la boucle
    {
        scanf("%s", &input); //entrée d'une chaine de caractères

        //mets un terme a la boucle en faisant passer "on" a la valeur 0
        if (strcmp(input, "exit") == 0)
        {
            on = 0;
        }
        else
        {

            if (strcmp(input, "inscription") == 0) //débute le processus d'inscription d'une entreprise
            {
                scanf("%s", &input); //choisir un type d'entreprise
                unsigned char currentCompanyType = (getInscriptionCompanyType(input));

                if (currentCompanyType)
                {
                    unsigned char exist = 0;
                    scanf("%s", &input);

                    for (unsigned short k = 0; k < countCompany; k++)
                    {
                        if (strcmp(company[k].name, input) == 0) //vérifie si l'entreprise existe déja
                        {
                            exist = 1;
                            k = countCompany;
                            printf("Nom incorrect\n");
                        }
                    }
                    if (!exist) //si l'entreprise n'existe pas il l'enregistre
                    {
                        strcpy(company[countCompany].name, input);
                        company[countCompany].companyType = currentCompanyType;
                        countCompany++;
                        printf("Inscription realisee (%u)\n", countCompany);

                    }


                }

                else
                {
                    printf("Role incorrect\n");
                }
            }

            //ajout d'une mission
            else if (strcmp(input, "mission") == 0)
            {
                unsigned char find = 0;
                short currentID;
                scanf("%hu", &currentID); //on donne l'ID d'une entreprise
                unsigned short j = 0;
                while (j < countCompany && !find)
                {
                    if (company[j].companyType == OP && j + 1 == currentID) //il faut que la mission soit définie par un opérateur (OP)
                    {
                        find = 1;
                    }
                    else
                    {
                        j++;
                    }
                }
                //si il trouve un opérateur avec l'ID donné on donne les paramètres de mission
                if (find)
                {
                    scanf("%s", &input); //c'est le nom de la mission

                    float currentRemuneration;
                    scanf("%f", &currentRemuneration); //entrée de la rémunération de la mission
                    if (currentRemuneration <= 0)//si la rémunération est inférieur ou égale a 0 elle est invalide
                    {
                        printf("Remuneration incorrecte\n");
                    }
                    else
                    {
                        //on intègre les paramètres de la mission a la structure "Mission"
                        mission[countMission].id = countMission + 1;
                        strcpy(mission[countMission].name, input);
                        mission[countMission].remuneration = currentRemuneration;
                        mission[countMission].company = j;
                        mission[countMission].state = 0;
                        mission[countMission].subcontractingCount = 0;
                        mission[countMission].idSubcontracting = -1;
                        mission[countMission].rapport = 0;
                        mission[countMission].acceptedBy = -1;
                        countMission++;
                        printf("Mission publiee (%u)\n", countMission);

                    }
                }
                else
                {
                    printf("Identifiant incorrect\n");
                }
            }

            //affichage de toutes les missions non attribuées
            else if (strcmp(input, "consultation") == 0)
            {
                unsigned short findMission = 0;
                for (unsigned short k = 0; k < countMission; k++)
                {
                    if (!mission[k].state)
                    {
                        showMission(mission[k], company[mission[k].company].name);
                        findMission++;
                    }
                }
                if (!findMission)
                {
                    printf("Aucune mission disponible\n");
                }
            }

            //détail d'une mission non attribuée séléctionée grace a son ID
            else if (strcmp(input, "detail") == 0)
            {
                unsigned char findMission = 0;
                short currentID;
                scanf("%hu", &currentID); //ID de la mission dont on veut le détail
                for (unsigned short k = 0; k < countMission; k++)
                {
                    if (!mission[k].state && mission[k].id == currentID)
                    {
                        showMission(mission[k], company[mission[k].company].name);
                        showRapport(mission[k].rapport);
                        findMission++;
                        k = countMission;
                    }
                }
                if (!findMission)
                {
                    printf("Identifiant incorrect\n");
                }

            }

            //accaptation d'une mission par une agence (AG) ou un intervenant (IN)
            else if (strcmp(input, "acceptation") == 0)
            {
                unsigned char find = 0;
                short currentID;
                scanf("%hu", &currentID); //sélection de la société qui accepte la mission via son ID
                unsigned short j = 0;
                while (j < countCompany && !find) //vérifie que l'entreprise existe
                {
                    if ((company[j].companyType == AG || company[j].companyType == IN) && j + 1 == currentID) //vérifie que l'entreprise est bien de type AG ou IN
                    {
                        find = 1;

                    }
                    else
                    {
                        j++;
                    }

                }
                if (!find)
                {
                    printf("Entreprise incorrecte\n");
                }

                else
                {
                    unsigned char findMission = 0;
                    scanf("%hu", &currentID);   //sélection de la mission via son ID
                    for (unsigned short k = 0; k < countMission; k++)
                    {
                        if (!mission[k].state && mission[k].id == currentID) //enregistre l'acceptation de la mission
                        {
                            mission[k].state = 1;
                            mission[k].acceptedBy = j;
                            printf("Acceptation enregistree\n");
                            findMission++;
                            k = countMission;
                        }
                    }
                    if (!findMission)
                    {
                        printf("Mission incorrecte\n");
                    }
                }


            }

            //sous-traitance d'une mission
            else if (strcmp(input, "sous-traitance") == 0)
            {
                unsigned char find = 0;
                short currentID;
                scanf("%hu", &currentID); //ID entreprise
                unsigned short j = 0;
                while (j < countCompany && !find)
                {
                    if (company[j].companyType == AG && j + 1 == currentID) //l'entreprise doit être de type AG
                    {
                        find = 1;
                    }
                    else
                    {
                        j++;
                    }
                }
                if (!find)
                {
                    printf("Entreprise incorrecte\n");
                }
                else
                {
                    unsigned char findMission = 0;
                    scanf("%hu", &currentID); //ID de mission
                    unsigned short k = 0;
                    while (k < countMission && !findMission)
                    {
                        if (mission[k].id == currentID && !mission[k].state)
                        {
                            if (mission[k].subcontractingCount < 5)
                            {
                                findMission = 1;
                            }
                            else
                            {
                                k = countMission;
                            }

                        }
                        else
                        {
                            k++;
                        }

                    }
                    if (!findMission)
                    {
                        printf("Mission incorrecte\n");
                    }
                    else
                    {
                        float currentRemuneration;
                        scanf("%f", &currentRemuneration); //définition de la rémunération
                        if (currentRemuneration <= 0)
                        {
                            printf("Remuneration incorrecte\n");
                        }
                        else
                        {
                            //mise en place d'une nouvelle mission
                            mission[countMission].id = countMission + 1;
                            strcpy(mission[countMission].name, mission[k].name);
                            mission[countMission].remuneration = currentRemuneration;
                            mission[countMission].company = j;
                            mission[countMission].state = 0;
                            mission[countMission].subcontractingCount = mission[k].subcontractingCount + 1;
                            mission[countMission].idSubcontracting = mission[k].id - 1;
                            mission[countMission].rapport = mission[k].rapport;
                            mission[countMission].acceptedBy = -1;
                            countMission++;
                            mission[k].state = 1;
                            printf("Sous-traitance enregistree (%u)\n", countMission);
                        }
                    }
                }
            }

            //rapport d'une mission
            else if (strcmp(input, "rapport") == 0)
            {
                unsigned char findMission = 0;
                short currentID;
                scanf("%hu", &currentID); //ID de la mission dont on doit faire le rapport
                unsigned short k = 0;
                //cherche la mission dont on a donné l'ID
                for (unsigned short j = 0; j < countMission; j++)
                {
                    if (mission[j].state == 1 && mission[j].id == currentID)
                    {
                        k = j;
                        j = countMission;
                        findMission++;
                    }
                }
                if (!findMission)//si il n'as pas trouvé
                {
                    printf("Mission incorrect\n");
                }
                else// si il trouve
                {
                    scanf("%hu", &currentID);
                    switch (currentID)
                    {
                    case 0: //si la mission est un succès
                        mission[k].state = 2;
                        short id = mission[k].idSubcontracting;
                        while (id > -1)
                        {
                            mission[id].state = 2;
                            id = mission[id].idSubcontracting;
                        }
                        printf("Rapport enregistre\n");
                        break;

                    case 1:
                    case 2:
                    case 3: //si la mission a pour code de retour 1, 2 ou 3:
                        mission[countMission].id = countMission + 1;
                        strcpy(mission[countMission].name, mission[k].name);
                        mission[countMission].remuneration = arroundFloat(mission[k].remuneration * rapportRem[currentID]);
                        mission[countMission].company = mission[k].company;
                        mission[countMission].state = 0;
                        mission[countMission].subcontractingCount = mission[k].subcontractingCount;
                        mission[countMission].idSubcontracting = mission[k].id - 1;
                        mission[countMission].rapport = mission[k].rapport * 10 + currentID;
                        mission[countMission].acceptedBy = -1;
                        countMission++;
                        mission[k].state = 2;
                        printf("Rapport enregistre (%u)\n", countMission);
                        break;

                    default:
                        printf("Code de retour incorrect");
                        break;
                    }
                }
            }
            //la fonction récapitulatif nécessite la chaine de caractère "recapitulatif" et l'ID d'une entreprise 
            else if (strcmp(input, "recapitulatif") == 0)
            {
                unsigned char find = 0;
                short currentID;
                scanf("%hu", &currentID);//ID entreprise
                unsigned short j = 0;
                while (j < countCompany && !find)//cherche l'ID d'entreprise donné
                {
                    if (j + 1 == currentID)
                    {
                        find = 1;
                    }
                    else
                    {
                        j++;
                    }
                }
                if (!find)//si il trouve pas l'ID d'entreprise donné
                {
                    printf("Entreprise incorrecte\n");
                }
                else//si il trouve l'ID
                {
                    //initialisation des tableaux dans lesquels les missions seront affichées
                    short non_attribuees_len = -1, attribuees_len = -1, terminees_len = -1, a_realiser_len = -1, realisees_len = -1;
                    short non_attribuees[MAX_MISSION];
                    short attribuees[MAX_MISSION];
                    short terminees[MAX_MISSION];
                    short a_realiser[MAX_MISSION];
                    short realisees[MAX_MISSION];

                    for (unsigned short k = 0; k < countMission; k++)
                    {
                        if (mission[k].company + 1 == currentID) //cherche toutes le missions qu l'entreprise a publié
                        {
                            switch (mission[k].state)
                            {
                            case 0: //missions non attribuées
                                non_attribuees_len++;
                                non_attribuees[non_attribuees_len] = k;
                                break;

                            case 1:
                                attribuees_len++; //missions attribuées
                                attribuees[attribuees_len] = k;
                                break;

                            case 2:
                                terminees_len++; //missions terminées
                                terminees[terminees_len] = k;
                                break;
                            default:
                                break;
                            }
                        }

                        //cherche toutes les missions acceptées par l'entreprise
                        else if (mission[k].acceptedBy == currentID - 1)
                        {
                            if (mission[k].state == 1) //toutes les missions a réaliser
                            {
                                a_realiser_len++;
                                a_realiser[a_realiser_len] = k;
                            }
                            else if (mission[k].state == 2) //toutes les missions qui ont étés réalisées
                            {
                                realisees_len++;
                                realisees[realisees_len] = k;
                            }
                        }
                    }

                    if (non_attribuees_len > -1) //affiche toutes les missions non attribuées
                    {
                        printf("* non attribuees\n");
                        for (short i = 0; i <= non_attribuees_len; i++) //ici le programme parcours le tableau et affiche toutes les missions qui y sont ce shéma est répété par la suite.
                        {
                            printf("  ");
                            showMission(mission[non_attribuees[i]], company[mission[non_attribuees[i]].company].name);
                        }
                    }

                    if (attribuees_len > -1) //affiche toutes les missions attribuées
                    {
                        printf("* attribuees\n");
                        for (short i = 0; i <= attribuees_len; i++)
                        {
                            printf("  ");
                            showMission(mission[attribuees[i]], company[mission[attribuees[i]].company].name);
                        }
                    }

                    if (terminees_len > -1) //affiche toutes les missions terminées
                    {
                        printf("* terminees\n");
                        for (short i = 0; i <= terminees_len; i++)
                        {
                            printf("  ");
                            showMission(mission[terminees[i]], company[mission[terminees[i]].company].name);
                        }
                    }

                    if (a_realiser_len > -1) //affiche toutes les missions que l'entreprise dois réaliser
                    {
                        printf("* a realiser\n");
                        for (short i = 0; i <= a_realiser_len; i++)
                        {
                            printf("  ");
                            showMission(mission[a_realiser[i]], company[mission[a_realiser[i]].company].name);
                        }
                    }

                    if (realisees_len > -1)
                    {
                        printf("* realisees\n"); //affiche toutes les missions que l'entreprise a réalisées
                        for (short i = 0; i <= realisees_len; i++)
                        {
                            printf("  ");
                            showMission(mission[realisees[i]], company[mission[realisees[i]].company].name);
                        }
                    }
                }
            }

        }

    }

    return 0; //fin du programme
}