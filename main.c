#include <stdio.h>
#include <string.h>

enum
{
    LIMIT_STRING_COMPANY = 30,
    LIMIT_STRING = 100,
};

typedef struct
{
    char name[LIMIT_STRING_COMPANY];
    int id;
} Conpany;

typedef struct
{
    Conpany conpany;
} Operator;

typedef struct
{
    Conpany conpany;
} Agence;

typedef struct
{
    Conpany conpany;
} Intervenant;

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

unsigned char operation(char input[LIMIT_STRING_COMPANY])
{
    if (strcmp(input, "inscription") == 0)
    {
        return 1;
    }

    return 0;
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

            switch (operation(currentMot.mot))
            {
            case 1: /* inscription */
                currentMot = segmentation(currentMot.index, lenght, input);

                int companyType = (getInscriptionCompanyType(currentMot.mot));
                if (companyType)
                {
                    /* code */
                }

                else
                {
                    printf("Role incorrect\n");
                }
                
                break;

            default:
                printf("operation non prise en charge\n");
                break;
            }

        }

    }



    return 0;
}