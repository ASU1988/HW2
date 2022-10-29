#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define THIRD     2
#define SECOND    1
#define FIRST     0

const int TabCp1251[64] = {0x402, 0x403,  0x201a,  0x453,  0x201e, 0x2026, 0x2020, 0x2021, 0x20ac, 0x2030, 0x409,  0x2039, 0x40a,  0x40c, 0x40b, 0x40f,
                           0x452, 0x2018, 0x2019,  0x201c, 0x201d, 0x2022, 0x2013, 0x2014, 0xFFFF, 0x2122, 0x459,  0x203a, 0x45a,  0x45c, 0x45b, 0x45f,
                           0xa0,  0x40e,  0x45e,   0x408,  0xa4,   0x490,  0xa6,   0xa7,   0x401,  0xa9,   0x404,  0xab,   0xac,   0xad,  0xae,  0x407,
                           0xb0,  0xb1,   0x406,   0x456,  0x491,  0xb5,   0xb6,   0xb7,   0x451,  0x2116, 0x454,  0xbb,   0x458,  0x405, 0x455, 0x457};

const int TabKoi8[128] = {0x2500, 0x2502, 0x250c, 0x2510, 0x2514, 0x2518, 0x251c, 0x2524, 0x252c, 0x2534, 0x253c, 0x2580, 0x2584, 0x2588, 0x258c, 0x2590,
                            0x2591, 0x2592, 0x2593, 0x2320, 0x25a0, 0x2219, 0x221a, 0x2248, 0x2264, 0x2265, 0xA0  , 0x2321, 0xb0  , 0xb2  , 0xb7  , 0xf7  ,
                            0x2550, 0x2551, 0x2552, 0x451,  0x2553, 0x2554, 0x2555, 0x2556, 0x2557, 0x2558, 0x2559, 0x255a, 0x255b, 0x255c, 0x255d, 0x255e,
                            0x255f, 0x2560, 0x2561, 0x401,  0x2562, 0x2563, 0x2564, 0x2565, 0x2566, 0x2567, 0x2568, 0x2569, 0x256a, 0x256b, 0x256c, 0xa9  ,
                            0x44e , 0x430 , 0x431 , 0x446 , 0x434 , 0x435 , 0x444 , 0x433 , 0x445 , 0x438 , 0x439 , 0x43a , 0x43b , 0x43c , 0x43d , 0x43e ,
                            0x43f , 0x44f , 0x440 , 0x441 , 0x442 , 0x443 , 0x436 , 0x432 , 0x44c , 0x44b , 0x437 , 0x448 , 0x44d , 0x449 , 0x447 , 0x44a ,
                            0x42e , 0x410 , 0x411 , 0x426 , 0x414 , 0x415 , 0x424 , 0x413 , 0x425 , 0x418 , 0x419 , 0x41a , 0x41b , 0x41c , 0x41d , 0x41e ,
                            0x41f , 0x42f , 0x420 , 0x421 , 0x422 , 0x423 , 0x416 , 0x412 , 0x42c , 0x42b , 0x417 , 0x428 , 0x42d , 0x429 , 0x427 , 0x42a};


// Преобразование cp1251 в Unicode
int ConvCp1251ToUni(int Cp1251Simbol)
{
    int UniSimbol;
    if((Cp1251Simbol >= 0x80) && (Cp1251Simbol <= 0xbf))
        UniSimbol = TabCp1251[Cp1251Simbol - 0x80];
    else if((Cp1251Simbol >= 0xc0) && (Cp1251Simbol <= 0xff))
            {
                UniSimbol = 0x410 + Cp1251Simbol - 0xc0;
            }
         else UniSimbol = Cp1251Simbol;
     return(UniSimbol);
}

// Преобразование ISO-8859-5 в Unicode
int ConvIso88595ToUni(int Iso88595Simbol)
{
    int UniSimbol;
    if((Iso88595Simbol >= 0xa1) && (Iso88595Simbol <= 0xff))
        UniSimbol = 0x401 + Iso88595Simbol - 0xa1;
    else UniSimbol = Iso88595Simbol;
    if(Iso88595Simbol == 0xad) UniSimbol = 0xad;
    if(Iso88595Simbol == 0xf0) UniSimbol = 0x2116;
    if(Iso88595Simbol == 0xfd) UniSimbol = 0xa7;
    return(UniSimbol);
}

// Преобразование KOI8-R в Unicode
int ConvKoi8rToUni(int Koi8Simbol)
{
    int UniSimbol;
    if((Koi8Simbol >= 0x80) && (Koi8Simbol <= 0xff))
        UniSimbol = TabKoi8[Koi8Simbol - 0x80];
    else UniSimbol = Koi8Simbol;
    return(UniSimbol);
}

// Преобразование Unicode в UTF8
int ConvUniToUtf8(int UniSimb, unsigned char *Utf8)
{
    if(UniSimb < 128)
    {
        Utf8[FIRST] = (unsigned char)UniSimb;
        return(1);
    }
    if((UniSimb >= 128) && (UniSimb < 2048)) // число значащих разрядов меньше или равно 11 и больше 7, кодируем в два символа
    {
        Utf8[SECOND] = (unsigned char) ((UniSimb | 0b11000000) & 0b10111111);
        Utf8[FIRST] = (unsigned char) (((UniSimb >> 6) | 0b11100000) & 0b11011111);
        return(2);
    }
    if((UniSimb >= 2048) && (UniSimb < 65536)) // число значащих разрядов меньше или равно 16 и больше 11, кодируем в два символа
    {
        Utf8[THIRD] = (unsigned char) ((UniSimb | 0b11000000) & 0b10111111);
        Utf8[SECOND] = (unsigned char) (((UniSimb >> 6) | 0b11000000) & 0b10111111);
        Utf8[FIRST] = (unsigned char) (((UniSimb >> 12) | 0b11110000) & 0b11101111);
        return(3);
    }
    return(0);
}

int main(int argc, char *argv[])
{
    /*****************************************************************/
    //  Алгоритм конвертации:
    //  1 Открываем исходный файл с заданной кодировкой;
    //  2 Определяем количество символов исходного файла;
    //  3 Читаем символы, сохраняем их в подготовленный масив;
    //  4 Закрываем исходный файл;
    //  5 Создаем выходной файл;
    //  6 Переводим считанный символ из заданной кодировки в Unicode;
    //  7 Переводим символ из Unicode в UTF8;
    //  8 Записываем символ(ы) UTF8 в выходной файл;
    //  9 Повторяем пп 7 - 9 до окончания массива символов;
    //  10 Закрываем выходной файл.
    /*****************************************************************/


    int Unicod_simbol;
    unsigned char UTF8_simbol[3] = {0};
    int n = 0;

    FILE *fin = fopen(argv[1], "r");

    if(fin == NULL)
    {
        printf("ERROR openning file\n");
        return 0;
    }

    if(strcmp(argv[2], "cp1251") && strcmp(argv[2], "iso-8859-5") && strcmp(argv[2], "koi8"))
    {
        printf("ERROR. The encoding type is not written correctly\n");
        return 0;
    }

    fseek(fin, 0, SEEK_END); // Переводим курсор файла в конец файла
    n = ftell(fin);
    fseek(fin, 0, SEEK_SET); // Переводим курсор файла в начало файла

    unsigned char *p = (unsigned char*)malloc(n * sizeof(unsigned char));

    // Читаем символы, сохраняем их в подготовленный масив, и закрываем файл
    fread(p, sizeof(unsigned char), n, fin);
    fclose(fin);

    //for(int i = 0; i < n; i++)
    //    printf("%d\t", p[i]);

    // Создаем выходной файл
    FILE *fout = fopen(argv[3], "w");
    if(fout == NULL)
    {
        printf("\nError creating out file\n");
        return 0;
    }

    // Записываем в файл UTF8 символы
    for(int i = 0; i < n; i++)
    {
        Unicod_simbol = (!strcmp(argv[2], "cp1251"))     ?  ConvCp1251ToUni(p[i])    :
                        (!strcmp(argv[2], "iso-8859-5")) ?  ConvIso88595ToUni(p[i])  :
                        (!strcmp(argv[2], "koi8"))       ?  ConvKoi8rToUni(p[i])     : 0;


        // Переводим символ из Unicode в UTF8
        int NumeOktet = ConvUniToUtf8(Unicod_simbol, UTF8_simbol);

        for(int j = 0; j < NumeOktet; j++)
            fputc(UTF8_simbol[j], fout);
    }
    fclose(fout);

    printf("\nThe file %s was created\n", argv[3]);

    return 1;
}

