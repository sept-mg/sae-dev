#include <stdio.h>
#include <string.h>
#pragma warning(disable: 4996)

enum //�num�ration des limites des chaines de caract�res et du maximum d'entreprise de missions et de rapports
{
    LIMIT_STRING_COMPANY = 31,  //limite de taille du nom d'une entreprise
    LIMIT_STRING_MISSION = 31,  //limite de taille de nom d'une mission
    LIMIT_STRING = 100,         //limite de taille d'une chaine de caract�re en g�n�ral
    MAX_COMPANY = 50,           //nombre maximal d'entreprise
    MAX_MISSION = 500,          //nombre maximal de missions
    MAX_RAPPORT = 4,            //nombre maximal de rapport de mission (pour une seule mission)
    MAX_SUBCONTRACT = 5,        //nombre maximal de sous contrat
    MAX_RECAP_TITLE = 20,       //limite de taille d'un type de dans r�capitulatif
    UNDEF_ACCEPTED_BY = -1,     //quand une mission n'est pas accepter par une entreprise
    UNDEF_ID_SUBTRACT = -1,     //quand une mission n'a pas �t� sous trait� par aucune entreprise
    DEFAULT_SUBSTRACT_COUNT = 0,//d�but du compte de sous traitance
};

typedef enum //type d'entreprises
{
    ND = 0, //non d�finie ou pas important
    OP = 1, //op�rateur
    AG,     //agence
    IN,     //intervenant
} CompanyType;

typedef enum //type de missions
{
    NATT = 0, //non attribu�es
    ATT, // attribu�es
    TER, // termin�es
} MissionState;

enum //type de rapport
{
    SUCCESS = 0, //par d�faut donc si la mission passe en termin� elle passe en SUCCESS par d�faut
    LNA, //Local non accessible 
    PSBG, //Pas de signal dans le boitier general
    RD, //Recepteur defectueux
};

typedef struct //structure contenant le nom d'une entreprise et son type (OP,AG,IN)
{
    char name[LIMIT_STRING_COMPANY]; //tableau qui contiens le nom
    CompanyType companyType;       //type d'entreprise
} Company;

typedef struct
{
    Company companies[MAX_COMPANY];
    unsigned char countCompany;

} Companies;

typedef struct //structure contenant toutes les informations sur 
{
    char name[LIMIT_STRING_MISSION];    //taille max du nom de la mission
    unsigned short company;             //l'entreprise qui a mis la mission en ligne
    float remuneration;                 //r�mun�ration de la mission
    MissionState state;                 //�tat de la mission (attribu�e non attribu�e etc...)
    unsigned char subcontractingCount;  //nombre de fois que la mission a �t�e soustrait�e
    short idSubcontracting;             //ID de la mission lors de sa sous traitance
    unsigned long rapport;              //ce qu'il s'est pass� lors de la mission (code 0 1 2 3)
    char acceptedBy;                    //ID de l'entreprise qui a accept� la mission
} Mission;

typedef struct
{
    Mission missions[MAX_MISSION];
    unsigned short countMission;
} Missions;

typedef struct //structur de chaque type de de mission a mettre dans la commande recapitulatif
{
    short table[MAX_MISSION]; //tableau des indexs
    short len; //taille du tableau

} PrintReccapStruct;

typedef enum
{
    FALSE = 0,
    TRUE = 0,
} Boolean;

float arroundFloat(float input) //fonction d'arrondi au centi�me pr�s
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

unsigned char getInscriptionCompanyType(char input[LIMIT_STRING_COMPANY]) //fonction d'inscription du type d'entreprise
{
    if (strlen(input) != 2)
    {
        return ND;
    }

    if (input[0] == 'O' && input[1] == 'P') //OP (op�rateur)
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
    return ND;
}

void showMission(unsigned short *id, Mission *mission, char companyName[LIMIT_STRING_COMPANY]) //affiche une mission
{
    printf("%u %s %s %.2f (%u)\n", *id + 1, mission->name, companyName, mission->remuneration, mission->subcontractingCount);
}

void showRapport(unsigned long rapport) //affiche le type de probl�mes qui ont eus lieu pendant la mission grace au code de retour
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
        rapport /= 10;
    }

}

Boolean existCompanyByName(char input[LIMIT_STRING], Companies *cp)
{
    Boolean exist = FALSE;
    for (unsigned short k = 0; k < cp->countCompany; k++)
    {
        if (strcmp(cp->companies[k].name, input) == 0) //v�rifie si l'entreprise existe d�ja
        {
            exist = TRUE;
            k = cp->countCompany;
            printf("Nom incorrect\n");
        }
    }

    return exist;
}


void makeMission(Missions *m, char name[LIMIT_STRING_MISSION], float remuneration, unsigned short companyIndex ,unsigned char subcontractingCount, short idSubcontracting, unsigned long rapport)
{
    strcpy(m->missions[m->countMission].name, name);
    m->missions[m->countMission].remuneration = remuneration;
    m->missions[m->countMission].company = companyIndex;
    m->missions[m->countMission].state = NATT;
    m->missions[m->countMission].subcontractingCount = subcontractingCount;
    m->missions[m->countMission].idSubcontracting = idSubcontracting;
    m->missions[m->countMission].rapport = rapport;
    m->missions[m->countMission].acceptedBy = UNDEF_ACCEPTED_BY;
    (m->countMission)++;
}

Boolean findCompany(Companies *cp, unsigned short *companyIndex, CompanyType type[2])
{
                                                                                                    //pas de type interressant
    return (*companyIndex < cp->countCompany && cp->companies[*companyIndex].companyType != 0 && ((type[0] == type[1] && type[0] == 0) || cp->companies[*companyIndex].companyType == type[0] || cp->companies[*companyIndex].companyType == type[1]));
}

Boolean findMissions(Missions *m, unsigned short* indexMission, MissionState state)
{
    return (*indexMission < m->countMission && m->missions[*indexMission].state == state);
}

void inscription (Companies *cp)
{
    char input[LIMIT_STRING];
    scanf("%s", &input); //choisir un type d'entreprise
    unsigned char currentCompanyType = getInscriptionCompanyType(input);

    if (currentCompanyType)
    {
        
        scanf("%s", &input);
        
       
        if (!existCompanyByName(input, cp)) //si l'entreprise n'existe pas il l'enregistre
        {
            strcpy(cp->companies[cp->countCompany].name, input);
            cp->companies[cp->countCompany].companyType = currentCompanyType;
            (cp->countCompany)++;
            printf("Inscription realisee (%u)\n", cp->countCompany);

        }


    }

    else
    {
        printf("Role incorrect\n");
    }
}

void mission(Missions *m, Companies *cp)
{
    short currentID;
    scanf("%hu", &currentID); //on donne l'ID d'une entreprise
    CompanyType type[2] = {OP, ND}; //doit etre de type OP
    currentID--;

    //si il trouve un op�rateur avec l'ID donn� on donne les param�tres de mission
    if (findCompany(cp, &currentID, type))
    {
        char input[LIMIT_STRING];
        scanf("%s", &input); //c'est le nom de la mission

        float currentRemuneration;
        scanf("%f", &currentRemuneration); //entr�e de la r�mun�ration de la mission
        if (currentRemuneration <= 0)//si la r�mun�ration est inf�rieur ou �gale a 0 elle est invalide
        {
            printf("Remuneration incorrecte\n");
        }
        else
        {
            //on int�gre les param�tres de la mission a la structure "Mission
            makeMission(m, input, currentRemuneration, currentID, DEFAULT_SUBSTRACT_COUNT, UNDEF_ID_SUBTRACT, SUCCESS); //success car par d�faut mais la mission n'est pas r�alis�
            printf("Mission publiee (%u)\n", m->countMission);

        }
    }
    else
    {
        printf("Identifiant incorrect\n");
    }
}

void consultation(Missions *m, Companies *cp) {
    unsigned short findMission = 0;
    for (unsigned short k = 0; k < m->countMission; k++)
    {
        if (m->missions[k].state == NATT) // v�rifie si la mission est de type 
        {
            showMission(&k, &(m->missions[k]), cp->companies[m->missions[k].company].name);
            findMission++;
        }
    }
    if (!findMission)
    {
        printf("Aucune mission disponible\n");
    }

}

void detail(Missions *m, Company companies[MAX_COMPANY])
{
    short currentID;
    scanf("%hu", &currentID); //ID de la mission dont on veut le d�tail

    currentID--;

    if (!findMissions(m, &currentID, NATT))
    {
        printf("Identifiant incorrect\n");
    } 
    else
    {
        showMission(&currentID, &m->missions[currentID], companies[m->missions[currentID].company].name);
        showRapport(m->missions[currentID].rapport);
    }
}

void acceptation(Missions *m, Companies *cp)
{
    short currentID;
    scanf("%hu", &currentID); //s�lection de la soci�t� qui accepte la mission via son ID
    CompanyType type[2] = {AG, IN}; // doit etre de type AG ou IN
    currentID--;

    if (!findCompany(cp, &currentID, type))
    {
        printf("Entreprise incorrecte\n");
    }

    else
    {
        unsigned char j = currentID;
        scanf("%hu", &currentID); //ID de la mission dont on veut le d�tail

        currentID--;

        if (!findMissions(m, &currentID, NATT))
        {
            printf("Mission incorrecte\n");
        }

        else
        {
            m->missions[currentID].state = ATT;
            m->missions[currentID].acceptedBy = j;
            printf("Acceptation enregistree\n");
        }
    }
}

void sousTraitance(Missions *m, Companies *cp)
{
    short currentID;
    scanf("%hu", &currentID); //ID entreprise
    CompanyType type[2] = {AG, ND}; //l'entreprise doit �tre de type AG
    
    currentID--;

    if (!findCompany(cp, &currentID, type))
    {
        printf("Entreprise incorrecte\n");
    }
    else
    {
        unsigned char j = currentID;
        scanf("%hu", &currentID); //ID de mission

        currentID--;

        if (!findMissions(m, &currentID, NATT))
        {
            printf("Mission incorrecte\n");
        }

        else if (m->missions[currentID].subcontractingCount > MAX_SUBCONTRACT)
        {
            printf("Mission incorrecte\n");
        }
        else
        {
            float currentRemuneration;
            scanf("%f", &currentRemuneration); //d�finition de la r�mun�ration
            if (currentRemuneration <= 0)
            {
                printf("Remuneration incorrecte\n");
            }
            else
            {
                //mise en place d'une nouvelle mission
                makeMission(m, m->missions[currentID].name, currentRemuneration, j, m->missions[currentID].subcontractingCount + 1, currentID, m->missions[currentID].rapport);
                m->missions[currentID].state = ATT;
                printf("Sous-traitance enregistree (%u)\n", m->countMission);
            }
        }
    }
}

void rapport(const float rapportRem[MAX_RAPPORT], Missions *m)
{
    short currentID;
    scanf("%hu", &currentID); //ID de la mission dont on doit faire le rapport
    
    currentID--;

    //cherche la mission dont on a donn� l'ID
    
    if (!findMissions(m, &currentID, ATT))//si il n'as pas trouv�
    {
        printf("Mission incorrect\n");
    }
    else// si il trouve
    {
        unsigned short k = currentID;
        scanf("%hu", &currentID);
        switch (currentID)
        {
        case SUCCESS: //si la mission est un succ�s
            m->missions[k].state = TER;
            short id = m->missions[k].idSubcontracting;
            while (id > -1)
            {
                m->missions[id].state = TER;
                id = m->missions[id].idSubcontracting;
            }
            printf("Rapport enregistre\n");
            break;

        case LNA:
        case PSBG:
        case RD: //si la mission a pour code de retour autre que succ�s:
            makeMission(m, m->missions[k].name, arroundFloat(m->missions[k].remuneration * rapportRem[currentID]), m->missions[k].company, m->missions[k].subcontractingCount, k, m->missions[k].rapport * 10 + currentID);
            m->missions[k].state = TER;
            printf("Rapport enregistre (%u)\n", m->countMission);
            break;

        default:
            printf("Code de retour incorrect");
            break;
        }
    }
}

void printRecap(char t[MAX_RECAP_TITLE], PrintReccapStruct *prs, Mission missions[MAX_MISSION], Company companies[MAX_COMPANY])
{
    printf("%s\n", t);
    for (short i = 0; i <= prs->len; i++) //ici le programme parcours le tableau et affiche toutes les missions qui y sont ce sh�ma est r�p�t� par la suite.
    {
        printf("  ");
        showMission(&prs->table[i], &missions[prs->table[i]], companies[missions[prs->table[i]].company].name);
    }
}

void recapitulatif(Missions *m, Companies *cp)
{
    short currentID;
    scanf("%hu", &currentID);//ID entreprise
    CompanyType type[2] = { ND, ND }; //l'entreprise peut importe son type

    currentID--;


    if (!findCompany(cp, &currentID, type))//si il trouve pas l'ID d'entreprise donn�
    {
        printf("Entreprise incorrecte\n");
    }
    else //si il trouve l'ID
    {
        //initialisation des tableaux dans lesquels les missions seront affich�es
        PrintReccapStruct non_attribuees;
        PrintReccapStruct attribuees;
        PrintReccapStruct terminees;
        PrintReccapStruct a_realiser;
        PrintReccapStruct realisees;
        non_attribuees.len = -1; attribuees.len = -1; terminees.len = -1; a_realiser.len = -1; realisees.len = -1;



        for (unsigned short k = 0; k < m->countMission; k++)
        {
            if (m->missions[k].company == currentID) //cherche toutes le missions qu l'entreprise a publi�
            {
                switch (m->missions[k].state)
                {
                case NATT: //missions non attribu�es
                    non_attribuees.len++;
                    non_attribuees.table[non_attribuees.len] = k;
                    break;

                case ATT: //missions attribu�es
                    attribuees.len++;
                    attribuees.table[attribuees.len] = k;
                    break;

                case TER: //missions termin�es
                    terminees.len++;
                    terminees.table[terminees.len] = k;
                    break;

                default:
                    break;
                }
            }

            //cherche toutes les missions accept�es par l'entreprise
            else if (m->missions[k].acceptedBy == currentID)
            {
                if (m->missions[k].state == ATT) //toutes les missions a r�aliser
                {
                    a_realiser.len++;
                    a_realiser.table[a_realiser.len] = k;
                }
                else if (m->missions[k].state == TER) //toutes les missions qui ont �t�s r�alis�es
                {
                    realisees.len++;
                    realisees.table[realisees.len] = k;
                }
            }
        }

        

        if (non_attribuees.len > -1) //affiche toutes les missions non attribu�es
        {
            printRecap("* non attribuees", &non_attribuees, m->missions, cp->companies);
        }

        if (attribuees.len > -1) //affiche toutes les missions attribu�es
        {
            printRecap("* attribuees", &attribuees, m->missions, cp->companies);
        }

        if (terminees.len > -1) //affiche toutes les missions termin�es
        {
            printRecap("* terminees", &terminees, m->missions, cp->companies);
        }

        if (a_realiser.len > -1) //affiche toutes les missions que l'entreprise dois r�aliser
        {
            printRecap("* a realiser", &a_realiser, m->missions, cp->companies);
        }

        if (realisees.len > -1)  //affiche toutes les missions que l'entreprise a r�alis�es
        {
            printRecap("* realisees", &realisees, m->missions, cp->companies);
        }
    }
}

int main()
{
    //d�finition des variables des tableaux 

    char input[LIMIT_STRING] = "";

    Companies companies;
    companies.countCompany = 0;

    Missions missions;
    missions.countMission = 0;

    const float rapportRem[MAX_RAPPORT] = { 1.0f, 1.0f, 1.055f, 1.04f };

    do
    {
        scanf("%s", &input); //entr�e d'une chaine de caract�res

        if (strcmp(input, "inscription") == 0) //d�bute le processus d'inscription d'une entreprise
        {
            inscription(&companies);
        }

        //ajout d'une mission
        else if (strcmp(input, "mission") == 0)
        {
            mission(&missions, &companies);
        }

        //affichage de toutes les missions non attribu�es
        else if (strcmp(input, "consultation") == 0)
        {
            consultation(&missions, &companies);
        }

        //d�tail d'une mission non attribu�e s�l�ction�e grace a son ID
        else if (strcmp(input, "detail") == 0)
        {
            detail(&missions, companies.companies);
        }

        //accaptation d'une mission par une agence (AG) ou un intervenant (IN)
        else if (strcmp(input, "acceptation") == 0)
        {
            acceptation(&missions, &companies);
        }

        //sous-traitance d'une mission
        else if (strcmp(input, "sous-traitance") == 0)
        {
            sousTraitance(&missions, &companies);
        }

        //rapport d'une mission
        else if (strcmp(input, "rapport") == 0)
        {
            rapport(rapportRem, &missions);
        }
        //la fonction r�capitulatif n�cessite la chaine de caract�re "recapitulatif" et l'ID d'une entreprise 
        else if (strcmp(input, "recapitulatif") == 0)
        {
            recapitulatif(&missions, &companies);
        }

    } while (strcmp(input, "exit") != 0);

    return 0; //fin du programme
}