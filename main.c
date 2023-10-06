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
    while (input[i]!= ' ' && i < limit)
    {
        currentChar[i] = input[i];
        i++;
    }
    currentChar[i+1] = '\0';
    strcpy(result.mot, currentChar);
    result.index = i+1;
    return result;
}

unsigned char operation(char input[LIMIT_STRING_COMPANY])
{
    if (strcmp (input, "inscription") == 0)
    {
        return 1;
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
        if(strcmp(input, "exit") == 0)
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
                    printf("%s\n", currentMot.mot);
                    break;
            
                default:
                    printf("operation non prise en charge\n");
                    break;
            }
            
        }
        
    }
    
    
    
    return 0;
}