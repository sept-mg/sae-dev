#include <stdio.h>
#include <string.h>
#pragma warning(disable: 4996)

enum
{
    LIMIT_STRING_COMPANY = 30,
    LIMIT_STRING_MISSION = 30,
    LIMIT_STRING = 100,
    MAX_COMPANY = 100,
    MAX_MISSION = 100,
};

float stringToFloat(char input[LIMIT_STRING])
{
    float result = 0;
    unsigned int shift = 1;
    unsigned int lenght = strlen(input);
    char signe = (input[0] == '-') ? -1 : 1;
    unsigned int i = (input[0] == '-' || input[0] == '+') ? 1 : 0;


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
            i = lenght*2;
            break;
        }
    }

    if (i == lenght*2)
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
    
    return temp / 100.0;
}

typedef struct
{
    char name[LIMIT_STRING_COMPANY];
    unsigned char companyType;
} Conpany;

typedef struct
{
    unsigned int id;
    char nom[LIMIT_STRING_MISSION];
    unsigned char company;
    float remuneration;
} Mission;


typedef struct
{
    char mot[LIMIT_STRING_COMPANY];
    int index;
} mot;

mot segmentation(unsigned int i, unsigned int limit, char input[LIMIT_STRING])
{
    mot result;
    char currentChar[LIMIT_STRING_COMPANY] = "";
    int j = 0; // Separate index for currentChar array

    while (input[i] != ' ' && i < limit && input[i] != '\0')
    {
        currentChar[j] = input[i];
        i++;
        j++;
    }
    currentChar[j] = '\0'; // Null-terminate the current word
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
        return 1;
    }
    
    else if (input[0] == 'A' && input[1] == 'G')
    {
        return 2;
    }

    else if (input[0] == 'I' && input[1] == 'N')
    {
        return 3;
    }
    return 0;
}



int main()
{
    char on = 1;
    char input[LIMIT_STRING];
    unsigned int lenght = 0;

    Conpany company[MAX_COMPANY];
    unsigned char countCompany = 0;

    Mission mission[MAX_MISSION];
    unsigned char countMission = 0;


    while (on)
    {
        gets(input);
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

            if (strcmp(currentMot.mot, "inscription") == 0)
            {
                currentMot = segmentation(currentMot.index, lenght, input);
                unsigned char currentCompanyType = (getInscriptionCompanyType(currentMot.mot));
                if (currentCompanyType)
                {
                    unsigned char exist = 0;
                    currentMot = segmentation(currentMot.index, lenght, input);
                    
                    for (int k = 0; k < countCompany; k++)
                    {
                        if(strcmp(company[k].name, currentMot.mot) == 0)
                        {
                            exist = 1;
                            printf("Nom incorrect\n");
                        }
                    }
                    if (!exist)
                    {
                       strcpy(company[countCompany].name, currentMot.mot);
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
            
            else if (strcmp(currentMot.mot, "mission") == 0)
            {
                unsigned char find = 0;
                currentMot = segmentation(currentMot.index, lenght, input);
                int currentID = (int)stringToFloat(currentMot.mot);
                int j = 0;
                for (; j < countCompany; j++)
                    {
                        if((company[j].companyType == 1 || company[j].companyType == 2) && j+1 == currentID)
                        {
                            find = 1;
                            
                        }
                    }

                if (find)
                {
                    currentMot = segmentation(currentMot.index, lenght, input);
                    char currentName[LIMIT_STRING_MISSION];
                    strcpy(currentName, currentMot.mot);

                    currentMot = segmentation(currentMot.index, lenght, input);
                    float currentRemuneration = stringToFloat(currentMot.mot);
                    if (currentRemuneration < 0)
                    {
                        printf("Remuneration incorrecte\n");
                    }
                    else
                    {
                        countMission++;
                        mission[countMission].id = countMission;
                        strcpy(mission[countMission].nom, currentName);
                        mission[countMission].remuneration = currentRemuneration;
                        mission[countMission].company = j;
                        printf("Mission publiee (%u)\n", countMission);
                        
                    }
                }
                else
                {
                    printf("Identifiant incorrect\n");
                }
            }
            
            else if (strcmp(currentMot.mot, "consultation") == 0)
            {
                if (countMission == 0)
                {
                    printf("Aucune mission disponible\n");
                }
                else
                {
                    for (unsigned char k = 0; k < countMission; k++)
                    {
                        printf("%u %s %s %.2f\n", mission[k].id, mission[k].nom, company[mission[k].company].name, mission[k].remuneration);
                    }
                    
                }
                
            }

            else
            {
                printf("Commande incorrect\n");
            }

        }

    }

    return 0;
}