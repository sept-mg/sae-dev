#include <stdio.h>
#include <string.h>
#pragma warning(disable: 4996)

enum //énumération des limites des chaines de caractères et du maximum d'entreprise de missions et de rapports
{
    LIMIT_STRING_COMPANY = 31,  //limite de taille du nom d'une entreprise
    LIMIT_STRING_MISSION = 31,  //limite de taille de nom d'une mission
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

enum //type de missions
{
    NATT = 0, //non attribuées
    ATT, // attribuées
    TER, // terminées
};

enum //type de rapport
{
    SUCCESS = 0, //Succes
    LNA, //Local non accessible 
    PSBG, //Pas de signal dans le boitier general
    RD, //Recepteur defectueux
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

void existCompanyByName(unsigned char* exist, char input[LIMIT_STRING], Company company[MAX_COMPANY], unsigned short* countCompany)
{
    for (unsigned short k = 0; k < *countCompany; k++)
    {
        if (strcmp(company[k].name, input) == 0) //vérifie si l'entreprise existe déja
        {
            *exist = 1;
            k = *countCompany;
            printf("Nom incorrect\n");
        }
    }

}

void makeMission(Mission missions[MAX_MISSION], unsigned short* countMission, unsigned short id, char name[LIMIT_STRING_MISSION], float remuneration, unsigned short company, unsigned char state ,unsigned char subcontractingCount, short idSubcontracting, unsigned long rapport, char acceptedBy)
{
    missions[*countMission].id = id;
    strcpy(missions[*countMission].name, name);
    missions[*countMission].remuneration = remuneration;
    missions[*countMission].company = company;
    missions[*countMission].state = state;
    missions[*countMission].subcontractingCount = subcontractingCount;
    missions[*countMission].idSubcontracting = idSubcontracting;
    missions[*countMission].rapport = rapport;
    missions[*countMission].acceptedBy = acceptedBy;
    (*countMission)++;
}

unsigned short findCompany(unsigned char* find, Company company[MAX_COMPANY], unsigned short* countCompany, unsigned short* currentID, unsigned char type[2])
{
    unsigned char notype = 0;
    if (type[0] == type[1] && type[0] == 0)
    {
        notype = 1;
    }

    unsigned short j = 0;
    while (j < *countCompany && !*find)
    {
        if (company[j].companyType != 0 && (company[j].companyType == type[0] || company[j].companyType == type[1] || notype) && j + 1 == *currentID) //il faut que la mission soit définie par un opérateur (OP)
        {
            *find = 1;
        }
        else
        {
            j++;
        }
    }

    return j;
}

unsigned short findMissions(unsigned char* findMission, Mission missions[MAX_MISSION], unsigned short* countMission, unsigned short* currentID, unsigned char state)
{
    unsigned short k = 0;
    while (k < *countMission && !*findMission)
    {
        /*printf("%d\n", missions[k].state == state && missions[k].id == *currentID);*/
        if (missions[k].state == state && missions[k].id == *currentID)
        {
            *findMission = 1;
        }
        else
        {
            k++;
        }
    }

    return k;
}

void inscription (Mission missions[MAX_MISSION], unsigned short* countMission, Company company[MAX_COMPANY], unsigned short* countCompany)
{
    char input[LIMIT_STRING];
    scanf("%s", &input); //choisir un type d'entreprise
    unsigned char currentCompanyType = (getInscriptionCompanyType(input));

    if (currentCompanyType)
    {
        
        scanf("%s", &input);
        unsigned char exist = 0;
        existCompanyByName(&exist, input, company, countCompany);
       
        if (!exist) //si l'entreprise n'existe pas il l'enregistre
        {
            strcpy(company[*countCompany].name, input);
            company[*countCompany].companyType = currentCompanyType;
            (*countCompany)++;
            printf("Inscription realisee (%u)\n", *countCompany);

        }


    }

    else
    {
        printf("Role incorrect\n");
    }
}

void mission(Mission missions[MAX_MISSION], unsigned short* countMission, Company company[MAX_COMPANY], unsigned short* countCompany)
{
    unsigned char find = 0;
    short currentID;
    scanf("%hu", &currentID); //on donne l'ID d'une entreprise
    unsigned char type[2] = {OP, 0};
    unsigned short j = findCompany(&find, company, countCompany, &currentID, type);

    //si il trouve un opérateur avec l'ID donné on donne les paramètres de mission
    if (find)
    {
        char input[LIMIT_STRING];
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
            makeMission(missions, countMission, *countMission + 1, input, currentRemuneration, j, NATT, 0, -1, 0, -1);
            printf("Mission publiee (%u)\n", *countMission);

        }
    }
    else
    {
        printf("Identifiant incorrect\n");
    }
}

void consultation(Mission missions[MAX_MISSION], unsigned short* countMission, Company company[MAX_COMPANY], unsigned short* countCompany) {
    unsigned short findMission = 0;
    for (unsigned short k = 0; k < *countMission; k++)
    {
        if (missions[k].state == NATT) // vérifie si la mission est de type 
        {
            showMission(missions[k], company[missions[k].company].name);
            findMission++;
        }
    }
    if (!findMission)
    {
        printf("Aucune mission disponible\n");
    }

}

void detail(Mission missions[MAX_MISSION], unsigned short* countMission, Company company[MAX_COMPANY])
{
    unsigned char findMission = 0;
    short currentID;
    scanf("%hu", &currentID); //ID de la mission dont on veut le détail
    unsigned short k = findMissions(&findMission, missions, countMission, &currentID, NATT);

    if (!findMission)
    {
        printf("Identifiant incorrect\n");
    } 
    else
    {
        showMission(missions[k], company[missions[k].company].name);
        showRapport(missions[k].rapport);
    }
}

void acceptation(Mission missions[MAX_MISSION], unsigned short* countMission, Company company[MAX_COMPANY], unsigned short* countCompany)
{
    unsigned char find = 0;
    short currentID;
    scanf("%hu", &currentID); //sélection de la société qui accepte la mission via son ID
    unsigned char type[2] = {AG, IN};
    unsigned short j = findCompany(&find, company, countCompany, &currentID, type);

    if (!find)
    {
        printf("Entreprise incorrecte\n");
    }

    else
    {
        unsigned char findMission = 0;
        short currentID;
        scanf("%hu", &currentID); //ID de la mission dont on veut le détail
        unsigned short k = findMissions(&findMission, missions, countMission, &currentID, NATT);

        if (!findMission)
        {
            printf("Mission incorrecte\n");
        }

        else
        {
            missions[k].state = ATT;
            missions[k].acceptedBy = j;
            printf("Acceptation enregistree\n");
        }
    }
}

void sousTraitance(Mission missions[MAX_MISSION], unsigned short* countMission, Company company[MAX_COMPANY], unsigned short* countCompany)
{
    unsigned char find = 0;
    short currentID;
    scanf("%hu", &currentID); //ID entreprise
    unsigned char type[2] = {AG, 0}; //l'entreprise doit être de type AG
    unsigned short j = findCompany(&find, company, countCompany, &currentID, type);
    
    if (!find)
    {
        printf("Entreprise incorrecte\n");
    }
    else
    {
        unsigned char findMission = 0;
        scanf("%hu", &currentID); //ID de mission
        unsigned short k = findMissions(&findMission, missions, countMission, &currentID, NATT);

        if (missions[k].subcontractingCount > 5)
        {
            findMission = 0;
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
                makeMission(missions, countMission, *countMission + 1, missions[k].name, currentRemuneration, j, NATT, missions[k].subcontractingCount + 1, missions[k].id - 1, missions[k].rapport, -1);
                missions[k].state = ATT;
                printf("Sous-traitance enregistree (%u)\n", *countMission);
            }
        }
    }
}

void rapport(const float rapportRem[MAX_RAPPORT], Mission missions[MAX_MISSION], unsigned short* countMission, Company company[MAX_COMPANY], unsigned short* countCompany)
{
    short currentID;
    unsigned char findMission = 0;
    scanf("%hu", &currentID); //ID de la mission dont on doit faire le rapport
    unsigned short k = findMissions(&findMission, missions, countMission, &currentID, ATT);//cherche la mission dont on a donné l'ID
    
    if (!findMission)//si il n'as pas trouvé
    {
        printf("Mission incorrect\n");
    }
    else// si il trouve
    {
        scanf("%hu", &currentID);
        switch (currentID)
        {
        case SUCCESS: //si la mission est un succès
            missions[k].state = TER;
            short id = missions[k].idSubcontracting;
            while (id > -1)
            {
                missions[id].state = TER;
                id = missions[id].idSubcontracting;
            }
            printf("Rapport enregistre\n");
            break;

        case LNA:
        case PSBG:
        case RD: //si la mission a pour code de retour autre que succès:
            makeMission(missions, countMission, *countMission + 1, missions[k].name, arroundFloat(missions[k].remuneration * rapportRem[currentID]), missions[k].company, NATT, missions[k].subcontractingCount, missions[k].id - 1, missions[k].rapport * 10 + currentID, -1);
            missions[k].state = TER;
            printf("Rapport enregistre (%u)\n", *countMission);
            break;

        default:
            printf("Code de retour incorrect");
            break;
        }
    }
}

void recapitulatif(Mission missions[MAX_MISSION], unsigned short* countMission, Company company[MAX_COMPANY], unsigned short* countCompany)
{
    unsigned char find = 0;
    short currentID;
    scanf("%hu", &currentID);//ID entreprise
    unsigned char type[2] = { 0, 0 }; //l'entreprise doit être de type AG
    unsigned short j = findCompany(&find, company, countCompany, &currentID, type);

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

        for (unsigned short k = 0; k < *countMission; k++)
        {
            if (missions[k].company + 1 == currentID) //cherche toutes le missions qu l'entreprise a publié
            {
                switch (missions[k].state)
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
            else if (missions[k].acceptedBy == currentID - 1)
            {
                if (missions[k].state == 1) //toutes les missions a réaliser
                {
                    a_realiser_len++;
                    a_realiser[a_realiser_len] = k;
                }
                else if (missions[k].state == 2) //toutes les missions qui ont étés réalisées
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
                showMission(missions[non_attribuees[i]], company[missions[non_attribuees[i]].company].name);
            }
        }

        if (attribuees_len > -1) //affiche toutes les missions attribuées
        {
            printf("* attribuees\n");
            for (short i = 0; i <= attribuees_len; i++)
            {
                printf("  ");
                showMission(missions[attribuees[i]], company[missions[attribuees[i]].company].name);
            }
        }

        if (terminees_len > -1) //affiche toutes les missions terminées
        {
            printf("* terminees\n");
            for (short i = 0; i <= terminees_len; i++)
            {
                printf("  ");
                showMission(missions[terminees[i]], company[missions[terminees[i]].company].name);
            }
        }

        if (a_realiser_len > -1) //affiche toutes les missions que l'entreprise dois réaliser
        {
            printf("* a realiser\n");
            for (short i = 0; i <= a_realiser_len; i++)
            {
                printf("  ");
                showMission(missions[a_realiser[i]], company[missions[a_realiser[i]].company].name);
            }
        }

        if (realisees_len > -1)
        {
            printf("* realisees\n"); //affiche toutes les missions que l'entreprise a réalisées
            for (short i = 0; i <= realisees_len; i++)
            {
                printf("  ");
                showMission(missions[realisees[i]], company[missions[realisees[i]].company].name);
            }
        }
    }
}

int main()
{
    //définition des variables des tableaux 

    char input[LIMIT_STRING] = "";

    Company company[MAX_COMPANY];
    unsigned short cc = 0;
    unsigned char* countCompany = &cc;

    Mission missions[MAX_MISSION];
    unsigned short cm = 0;
    unsigned short* countMission = &cm;
    const float rapportRem[MAX_RAPPORT] = { 1.0f, 1.0f, 1.055f, 1.04f };

    do
    {
        scanf("%s", &input); //entrée d'une chaine de caractères

        if (strcmp(input, "inscription") == 0) //débute le processus d'inscription d'une entreprise
        {
            inscription(missions, countMission, company, countCompany);
                
        }

        //ajout d'une mission
        else if (strcmp(input, "mission") == 0)
        {
            mission(missions, countMission, company, countCompany);
        }

        //affichage de toutes les missions non attribuées
        else if (strcmp(input, "consultation") == 0)
        {
            consultation(missions, countMission, company, countCompany);
        }

        //détail d'une mission non attribuée séléctionée grace a son ID
        else if (strcmp(input, "detail") == 0)
        {
            detail(missions, countMission, company);
        }

        //accaptation d'une mission par une agence (AG) ou un intervenant (IN)
        else if (strcmp(input, "acceptation") == 0)
        {
            acceptation(missions, countMission, company, countCompany);
        }

        //sous-traitance d'une mission
        else if (strcmp(input, "sous-traitance") == 0)
        {
            sousTraitance(missions, countMission, company, countCompany);
        }

        //rapport d'une mission
        else if (strcmp(input, "rapport") == 0)
        {
            rapport(rapportRem, missions, countMission, company, countCompany);
        }
        //la fonction récapitulatif nécessite la chaine de caractère "recapitulatif" et l'ID d'une entreprise 
        else if (strcmp(input, "recapitulatif") == 0)
        {
            recapitulatif(missions, countMission, company, countCompany);
        }

    } while (strcmp(input, "exit") != 0);

    return 0; //fin du programme
}