#include <stdio.h>
#include <string.h>
#pragma warning(disable: 4996)

enum
{
    LIMIT_STRING_COMPANY = 30,
    LIMIT_STRING_MISSION = 30,
    LIMIT_STRING = 100,
    MAX_COMPANY = 50,
    MAX_MISSION = 500,
    MAX_RAPPORT = 4,
};

enum
{
    OP = 1,
    AG,
    IN,
};

float stringToFloat(char input[LIMIT_STRING])
{
    float result = 0;
    unsigned int shift = 1;
    int lenght = strlen(input);
    char signe = (input[0] == '-') ? -1 : 1;
    int i = (input[0] == '-' || input[0] == '+') ? 1 : 0;


    for (; i < lenght; i++)
    {
        switch (input[i])
        {
        case '0':
            result = result * 10;
            break;

        case '1':
            result = result * 10 + 1;
            break;

        case '2':
            result = result * 10 + 2;
            break;

        case '3':
            result = result * 10 + 3;
            break;

        case '4':
            result = result * 10 + 4;
            break;

        case '5':
            result = result * 10 + 5;
            break;

        case '6':
            result = result * 10 + 6;
            break;

        case '7':
            result = result * 10 + 7;
            break;

        case '8':
            result = result * 10 + 8;
            break;

        case '9':
            result = result * 10 + 9;
            break;

        case '.':
            for (int k = 0; k < lenght - i - 1; k++)
            {
                shift *= 10;
            }

            break;

        default:
            i = lenght * 2;
            break;
        }
    }

    if (i == lenght * 2)
    {
        return 0;
    }

    int temp = result / shift * signe * 1000;
    if (temp % 10 > 4)
    {
        temp = temp / 10 + 1;
    }
    else
    {
        temp = temp / 10;
    }

    return (float) temp / 100;
}

float arroundFloat(float input)
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

    return (float) temp / 100;
}

typedef struct
{
    char name[LIMIT_STRING_COMPANY];
    unsigned char companyType;
} Company;

typedef struct
{
    unsigned short id;
    char name[LIMIT_STRING_MISSION];
    unsigned short company;
    float remuneration;
    unsigned char state;
    unsigned char subcontractingCount;
    short idSubcontracting;
    unsigned long rapport;
    char acceptedBy;
} Mission;


typedef struct
{
    char mot[LIMIT_STRING_COMPANY];
    unsigned short index;
} mot;

mot segmentation(unsigned short i, unsigned short limit, char input[LIMIT_STRING])
{
    mot result;
    char currentChar[LIMIT_STRING_COMPANY] = "";
    unsigned short j = 0;

    while (input[i] != ' ' && i < limit && input[i] != '\0')
    {
        currentChar[j] = input[i];
        i++;
        j++;
    }
    currentChar[j] = '\0';
    strcpy(result.mot, currentChar);
    result.index = i + 1;
    return result;
}

unsigned char getInscriptionCompanyType(char input[LIMIT_STRING_COMPANY])
{
    if (strlen(input) != 2)
    {
        return 0;
    }


    if (input[0] == 'O' && input[1] == 'P')
    {
        return OP;
    }

    else if (input[0] == 'A' && input[1] == 'G')
    {
        return AG;
    }

    else if (input[0] == 'I' && input[1] == 'N')
    {
        return IN;
    }
    return 0;
}

void showMission(Mission mission, char companyName[LIMIT_STRING_COMPANY])
{
    printf("%u %s %s %.2f (%u)\n", mission.id, mission.name, companyName, mission.remuneration, mission.subcontractingCount);
    // company[mission[k].company].name
}

void showRapport(unsigned long rapport)
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
    char on = 1;
    char input[LIMIT_STRING] = "";
    unsigned char lenght = 0;

    Company company[MAX_COMPANY];
    unsigned char countCompany = 0;

    Mission mission[MAX_MISSION];
    unsigned short countMission = 0;
    const float rapportRem[MAX_RAPPORT] = {1.0, 1.0, 1.055, 1.04};
    
    while (on)
    {
        scanf("%s", &input);
        if (strcmp(input, "exit") == 0)
        {   
            on = 0;
        }
        else
        {
            mot currentMot;
            currentMot.index = 0;
            lenght = strlen(input);
            currentMot = segmentation(currentMot.index, lenght, input);

            if (strcmp(input, "inscription") == 0)
            {
                scanf("%s", &input);
                unsigned char currentCompanyType = (getInscriptionCompanyType(input));
                
                if (currentCompanyType)
                {
                    unsigned char exist = 0;
                    scanf("%s", &input);

                    for (unsigned short k = 0; k < countCompany; k++)
                    {
                        if (strcmp(company[k].name, input) == 0)
                        {
                            exist = 1;
                            k = countCompany;
                            printf("Nom incorrect\n");
                        }
                    }
                    if (!exist)
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

            else if (strcmp(input, "mission") == 0)
            {
                unsigned char find = 0;
                short currentID;
                scanf("%hu", &currentID);
                unsigned short j = 0;
                while (j < countCompany && !find)
                {
                    if (company[j].companyType == OP && j + 1 == currentID)
                    {
                        find = 1;

                    }
                    else
                    {
                        j++;
                    }
                    
                }

                if (find)
                {
                    scanf("%s", &input);

                    float currentRemuneration;
                    scanf("%f", &currentRemuneration);
                    if (currentRemuneration <= 0)
                    {
                        printf("Remuneration incorrecte\n");
                    }
                    else
                    {
                        
                        mission[countMission].id = countMission+1;
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

            else if (strcmp(input, "consultation") == 0)
            {
                unsigned short findMission = 0;
                for (unsigned short k = 0; k < countMission; k++)
                {
                    if(!mission[k].state) 
                    {
                        showMission(mission[k], company[mission[k].company].name);
                        findMission++;
                    }
                }
                if(!findMission) 
                {
                    printf("Aucune mission disponible\n");
                }
            }

            else if (strcmp(input, "detail") == 0)
            {
                unsigned char findMission = 0;
                short currentID;
                scanf("%hu", &currentID);
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

            else if (strcmp(input, "acceptation") == 0)
            {
                unsigned char find = 0;
                short currentID;
                scanf("%hu", &currentID);
                unsigned short j = 0;
                while (j < countCompany && !find)
                {
                    if ((company[j].companyType == AG || company[j].companyType == IN) && j + 1 == currentID)
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
                    scanf("%hu", &currentID);
                    for (unsigned short k = 0; k < countMission; k++)
                    {
                        if (!mission[k].state && mission[k].id == currentID)
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

            else if (strcmp(input, "sous-traitance") == 0)
            {
                unsigned char find = 0;
                short currentID;
                scanf("%hu", &currentID);
                unsigned short j = 0;
                while (j < countCompany && !find)
                {
                    if (company[j].companyType == AG && j + 1 == currentID)
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
                    scanf("%hu", &currentID);
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
                        scanf("%f", &currentRemuneration);
                        if (currentRemuneration <= 0)
                        {
                            printf("Remuneration incorrecte\n");
                        }
                        else
                        {
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

            else if (strcmp(input, "rapport") == 0)
            {
                unsigned char findMission = 0;
                short currentID;
                scanf("%hu", &currentID);
                unsigned short k = 0;
                for (unsigned short j = 0; j < countMission; j++)
                {
                    if (mission[j].state == 1 && mission[j].id == currentID)
                    {
                        k = j;
                        j = countMission;
                        findMission++;
                    }
                }
                if (!findMission)
                {
                    printf("Mission incorrect\n");
                }
                else
                {
                    scanf("%hu", &currentID);
                    switch (currentID)
                    {
                        case 0:
                            mission[k].state = 2;
                            short id = mission[k].idSubcontracting;
                            while(id > -1)
                            {
                                mission[id].state = 2;
                                id = mission[id].idSubcontracting;
                            }
                            printf("Rapport enregistre\n");
                            break;
                         
                        case 1:
                        case 2:
                        case 3:
                            mission[countMission].id = countMission + 1;
                            strcpy(mission[countMission].name, mission[k].name);
                            mission[countMission].remuneration = arroundFloat(mission[k].remuneration * rapportRem[currentID]);
                            mission[countMission].company = mission[k].company;
                            mission[countMission].state = 0;
                            mission[countMission].subcontractingCount = mission[k].subcontractingCount;
                            mission[countMission].idSubcontracting = mission[k].id -1;
                            mission[countMission].rapport = mission[k].rapport*10 + currentID;
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

            else if (strcmp(input, "recapitulatif") == 0)
            {
                unsigned char find = 0;
                short currentID;
                scanf("%hu", &currentID);
                unsigned short j = 0;
                while (j < countCompany && !find)
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
                if (!find)
                {
                    printf("Entreprise incorrecte\n");
                }
                else
                {
                    short non_attribuees_len = -1, attribuees_len = -1, terminees_len = -1, a_realiser_len = -1, realisees_len = -1;
                    short non_attribuees[MAX_MISSION];
                    short attribuees[MAX_MISSION];
                    short terminees[MAX_MISSION];
                    short a_realiser[MAX_MISSION];
                    short realisees[MAX_MISSION];

                    for (unsigned short k = 0; k < countMission; k++)
                    {
                        if (mission[k].company + 1 == currentID)
                        {
                            switch (mission[k].state)
                            {
                                case 0:
                                    non_attribuees_len++;
                                    non_attribuees[non_attribuees_len] = k;
                                    break;

                                case 1:
                                    attribuees_len++;
                                    attribuees[attribuees_len] = k;
                                    break;
                                
                                case 2:
                                    terminees_len++;
                                    terminees[terminees_len] = k;
                                    break;
                                default:
                                    break;
                            }
                        }
                        else if(mission[k].acceptedBy == currentID - 1)
                        {
                            if (mission[k].state == 1)
                            {
                                a_realiser_len++;
                                a_realiser[a_realiser_len] = k;
                            }
                            else if (mission[k].state == 2)
                            {
                                realisees_len++;
                                realisees[realisees_len] = k;
                            }
                        }
                    }
                    
                    if (non_attribuees_len > -1)
                    {
                        printf("* non attribuees\n");
                        for (short i = 0; i <= non_attribuees_len; i++)
                        {
                            printf("  ");
                            showMission(mission[non_attribuees[i]], company[mission[non_attribuees[i]].company].name);
                        }
                    }

                    if (attribuees_len > -1)
                    {
                        printf("* attribuees\n");
                        for (short i = 0; i <= attribuees_len; i++)
                        {
                            printf("  ");
                            showMission(mission[attribuees[i]], company[mission[attribuees[i]].company].name);
                        }
                    }

                    if (terminees_len > -1)
                    {
                        printf("* terminees\n");
                        for (short i = 0; i <= terminees_len; i++)
                        {
                            printf("  ");
                            showMission(mission[terminees[i]], company[mission[terminees[i]].company].name);
                        }
                    }

                    if (a_realiser_len > -1)
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
                        printf("* realisees\n");
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

    return 0;
}
