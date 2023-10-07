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

typedef struct
{
    char name[LIMIT_STRING_COMPANY];
    unsigned char companyType;
} Conpany;

typedef struct
{
    unsigned int id;
    char nom[LIMIT_STRING_MISSION];
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
                       printf("Inscription realisee (%u)\n", countCompany + 1);
                       countCompany++;
                    }
                    
                    
                }

                else
                {
                    printf("Role incorrect\n");
                }
            }
            
            if (strcmp(currentMot.mot, "mission") == 0)
            {
                unsigned char exist = 0;
                currentMot = segmentation(currentMot.index, lenght, input);

                for (int k = 0; k < countMission; k++)
                    {
                        if(mission[k].id == currentMot.mot)
                        {
                            exist = 1;
                            printf("Identifiant incorrect\n");
                        }
                    }

                if (!exist)
                {
                    currentMot = segmentation(currentMot.index, lenght, input);
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