#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <locale.h>
#include <windows.h>

//struct for image
typedef struct
{
	char name[50];
	char standard[3];
	int dim[2];
	int gray;
	int** dimPixels;
}pgm;

//read from file
void read(pgm* tmpImg)
{
	int retScanf = 0;
	int x = 0;
	int y = 0;

	while (1)
	{
		printf("Podaj nazwę pliku do wczytania: ");
		scanf("%s", &tmpImg->name);
		FILE* pic;
		if (pic = fopen(&tmpImg->name, "r"))
		{
			fopen_s(&pic, &tmpImg->name, "r");
			fscanf(pic, "%s", &tmpImg->standard);
			//get dimensions of image
			while (retScanf == 0)
			{
				retScanf = fscanf(pic, "%d %d", &tmpImg->dim[0], &tmpImg->dim[1]);
				if (retScanf == 0)
				{
					char trash = 0;
					do
					{
						trash = fgetc(pic);
					} while (trash != '\n');
				}
			}
			retScanf = 0;
			//get gray scale from image
			while (retScanf == 0)
			{
				retScanf = fscanf(pic, "%d", &tmpImg->gray);
				if (retScanf == 0)
				{
					char trash = 0;
					do
					{
						trash = fgetc(pic);
					} while (trash != '\n');
				}
			}
			retScanf = 0;
			//get pixels from image
			tmpImg->dimPixels = malloc(tmpImg->dim[0] * sizeof(int*));
			for (x = 0; x < tmpImg->dim[0]; x++)
			{
				tmpImg->dimPixels[x] = (int*)malloc(tmpImg->dim[1] * sizeof(int));
			}
			int i = -1;
			int j = 0;
			while (!feof(pic))
			{
				int pix;
				retScanf = fscanf(pic, "%d", &pix);
				if (retScanf == 0)
				{
					char trash = 0;
					do
					{
						trash = fgetc(pic);
					} while (trash != '\n');
				}
				else
				{
					i++;
					if (i == tmpImg->dim[0]) {
						i = 0;
						j += 1;
					}
					tmpImg->dimPixels[i][j] = pix;
				}
			}
			fclose(pic);
			break;
		}
		else
		{
			printf("Plik nie istnieje :/");
			Sleep(1000);
			system("CLS");
		}
	}
} 

//print image from database
void print(pgm img)
{
	int x, y;

	printf("%s\n", img.standard);
	printf("%d %d\n", img.dim[0], img.dim[1]);
	printf("%d\n", img.gray);
	for (y = 0; y < img.dim[1]; y++)
	{
		for (x = 0; x < img.dim[0]; x++)
		{
			printf("%3d ", img.dimPixels[x][y]);
		}
		printf("\n");
	}
	printf("\nWciśnij Enter, aby zakończyć przegląd\n");
	while (getchar() != '\n');
	getchar();
}

//delete image from database
void delete(pgm* base, int pos, int curIndex)
{
	free(base[pos].dimPixels);
	for (int i = pos; i < curIndex; i++)
	{
		base[i] = base[i + 1];
	}
}

//save image from database to file
void save(pgm img)
{
	int x, y;

	char name[50] = { 0 };
	printf("Podaj nazwę dla nowego pliku: ");
	scanf("%s", &name);
	FILE* fsave;
	fsave = fopen(name, "w");
	fprintf_s(fsave, "%s\n", img.standard);
	fprintf_s(fsave, "%d %d\n", img.dim[0], img.dim[1]);
	fprintf_s(fsave, "%d\n", img.gray);
	for (y = 0; y < img.dim[1]; y++)
	{
		for (x = 0; x < img.dim[0]; x++)
		{
			fprintf_s(fsave, "%d ", img.dimPixels[x][y]);
		}
	}
	fclose(fsave);
}

//lists images from database
void list(int curIndex, pgm* baza)
{
	printf("Lista obrazów\n");
	for (int i = 0; i < curIndex; i++)
	{
		printf("%d. %s\n", i, &baza[i].name);
	}
}

//rotate image from database
void rotate(pgm * img) {
	pgm tmp;
	for (int x = 0; x < 50; x++) { tmp.name[x] = img->name[x]; }
	for (int x = 0; x < 3; x++) { tmp.standard[x] = img->standard[x]; }
	tmp.dim[0] = img->dim[1];
	tmp.dim[1] = img->dim[0];
	tmp.gray = img->gray;

	tmp.dimPixels = malloc(tmp.dim[0] * sizeof(int));
	for (int x = 0; x < tmp.dim[0]; x++) {
		tmp.dimPixels[x] = malloc(tmp.dim[1] * sizeof(int));
	}

	for (int i = 0; i < img->dim[0]; i++) {
		for (int j = 0; j < img->dim[1]; j++) {
			tmp.dimPixels[j][i] = img->dimPixels[i][j];
		}
	}

	for (int i = 0; i < tmp.dim[0]; i++) {
		int start = 0;
		int end = tmp.dim[1] - 1;
		while (start < end) {
			int temp = tmp.dimPixels[i][start];
			tmp.dimPixels[i][start] = tmp.dimPixels[i][end];
			tmp.dimPixels[i][end] = temp;

			start++;
			end--;
		}
	}
	*img = tmp;
	}

//multiplier for rotating
void rotatek(pgm * img, int k) {
	if (k % 4 == 0) {
		return;
	}
	else if (k % 3 == 0) {
		rotate(img);
	}
	else if (k % 2 == 0) {
		rotate(img);
		rotate(img);
	}
	else if (k % 1 == 0) {
		rotate(img);
		rotate(img);
		rotate(img);
	}
}

//make histogram file from image in
void histogram(pgm img)
{
	int pix;
	int hist[256] = { 0 };
	for (int i = 0; i < img.dim[1]; i++)
	{
		for (int j = 0; j < img.dim[0]; j++)
		{
			pix = img.dimPixels[j][i];
			hist[pix]++;
		}
	}
	char* name = (char*)malloc(50 * sizeof(char));
	sprintf(name, "%s_hist.csv", img.name);
	FILE* fhist;
	fhist = fopen(name, "w");
	for (int i = 0; i < 256; i++)
	{
		fprintf_s(fhist, "%d, %d\n", i, hist[i]);
	}
	fclose(fhist);
}

//treshold image from database
void thresholding(pgm img, int thres_var)
{
	int i, j;
	for (i = 0; i < img.dim[0]; i++) {
		for (j = 0; j < img.dim[1]; j++) {
			if (img.dimPixels[i][j] <= thres_var) {
				img.dimPixels[i][j] = 0;
			}
			else
				img.dimPixels[i][j] = img.gray;
		}
	}
}

//noise image from database
void pep_sal(pgm img)
{
	int size; 
	int noise;
	int x, y;
	int random;
	int var;

	size = img.dim[0] * img.dim[1];

	srand(time(NULL));

	while (1)
	{
		printf("Podaj procent zaszumienia: ");
		scanf("%d", &var);
		noise = size * 0.02 * var;
		if (var > 100 || var < 0)
		{
			printf("Wartość zaszumienia musi być w przedziale od %s do %s\n", "0%", "100%");
			Sleep(1000);
		}
		else
		{
			for (int i = 0; i < noise; i++)
			{
				x = (rand() % img.dim[0]);
				y = (rand() % img.dim[1]);
				random = rand() % 2;
				img.dimPixels[x][y] = random * img.dim[1];
			}
			break;
		}
	}
}

//entry point
int main()
{
	setlocale(LC_ALL, "Polish");

	int x;
	int k;
	int end;
	int i;
	int pos;
	int thres_var;
	int curIndex = 0;
	pgm* base = NULL;
	pgm aktywny;

	printf("MENU\n");
	printf("1. Dodaj obraz\n");
	printf("2. Wyświetl obraz\n");
	printf("3. Edytuj obraz\n");
	printf("4. Zapisz kopię obrazu\n");
	printf("5. Usuń obraz\n");
	printf("6. Wyjście\n\n");

	while (1)
	{
		scanf_s("%d", &x);
		switch (x)
		{
		case 1:
			system("CLS");
			read(&aktywny);
			base = realloc(base, (curIndex + 1) * sizeof(pgm));
			base[curIndex] = aktywny;
			curIndex++;
			break;
		case 2:
			while (1)
			{
				system("CLS");
				list(curIndex, base);
				printf("Podaj numer obrazu do wyświetlenia: ");
				scanf("%d", &i);
				if (i > curIndex-1 || i < 0)
				{
					printf("Nie ma takiego obrazu.");
					Sleep(1000);
				}
				else
				{
					print(base[i]);
					break;
				}
			}
			break;
		case 3:
			end = 1;
			do
			{
				system("CLS");
				printf("TRYB EDYCJI\n");
				printf("1. Obracanie\n");
				printf("2. Histogram\n");
				printf("3. Progowanie\n");
				printf("4. Szum Pieprz-Sól\n");
				printf("5. Powrót do Menu\n\n");

				scanf_s("%d", &x);
				switch (x)
				{
				case 1:
					while (1)
					{
						system("CLS");
						list(curIndex, base);
						printf("Podaj numer obrazu do obrócenia: ");
						scanf("%d", &i);
						if (i > curIndex - 1 || i < 0)
						{
							printf("Nie ma takiego obrazu.");
							Sleep(1000);
						}
						else
						{
							while (1)
							{
								printf("Podaj współczynnik obrotu: ");
								scanf("%d", &k);
								if (k < 0)
								{
									printf("Współczynnik obrotu musi być większy od zera.\n");
									Sleep(1000);
								}
								else
								{
									rotatek(&base[i], k);
									break;
								}
							}
							break;
						}
					}
					break;
				case 2:
					while (1)
					{
						system("CLS");
						list(curIndex, base);
						printf("Podaj numer obrazu, dla którego wykonać histogram: ");
						scanf("%d", &i);
						if (i > curIndex - 1 || i < 0)
						{
							printf("Nie ma takiego obrazu.");
							Sleep(1000);
						}
						else
						{
							histogram(base[i]);
							break;
						}
					}
					break;
				case 3:
					while (1)
					{
						system("CLS");
						list(curIndex, base);
						printf("Podaj numer obrazu do progowania: ");
						scanf("%d", &i);
						if (i > curIndex - 1 || i < 0)
						{
							printf("Nie ma takiego obrazu.");
							Sleep(1000);
						}
						else
						{
							while (1)
							{
								printf("Podaj wartość progowania: ");
								scanf("%d", &thres_var);
								if (thres_var < 0 || thres_var > 255)
								{
									printf("Progowanie występuje w zakresie od 0 do 255.\n");
									Sleep(1000);
								}
								else
								{
									thresholding(base[i], thres_var);
									break;
								}
							}
							break;
						}
					}
					break;
				case 4:
					while (1)
					{
						system("CLS");
						list(curIndex, base);
						printf("Podaj numer obrazu do wyświetlenia: ");
						scanf("%d", &i);
						if (i > curIndex - 1 || i < 0)
						{
							printf("Nie ma takiego obrazu.");
							Sleep(1000);
						}
						else
						{
							pep_sal(base[i]);
							break;
						}
					}
					break;
				case 5:
					end = 0;
					break;
				default:
					printf("Nie ma takiej opcji :/");
					Sleep(1000);
				}
			} while (end != 0);
			break;
		case 4:
			while (1)
			{
				system("CLS");
				list(curIndex, base);
				printf("Podaj numer obrazu do zapisania: ");
				scanf("%d", &i);
				if (i > curIndex - 1 || i < 0)
				{
					printf("Nie ma takiego obrazu.");
					Sleep(1000);
				}
				else
				{
					save(base[i]);
					break;
				}
			}
			break;
		case 5:
			while (1)
			{
				system("CLS");
				list(curIndex, base);
				printf("Podaj numer obrazu do usunięcia: ");
				scanf("%d", &pos);
				if (pos > curIndex - 1 || pos < 0)
				{
					printf("Nie ma takiego obrazu.");
					Sleep(1000);
				}
				else
				{
					delete(base, pos, curIndex);
					base = realloc(base, (curIndex - 1) * sizeof(pgm));
					curIndex -= 1;
					break;
				}
			}
			break;
		case 6:
			for (int i = 0; i < curIndex; i++)
			{
				free(base[i].dimPixels);
			}
			free(base);
			exit(EXIT_SUCCESS);
		default:
			printf("Nie ma takiej opcji :/");
		}
		system("CLS");
		printf("MENU\n");
		printf("1. Dodaj obraz\n");
		printf("2. Wyświetl obraz\n");
		printf("3. Edytuj obraz\n");
		printf("4. Zapisz kopię obrazu\n");
		printf("5. Usuń obraz\n");
		printf("6. Wyjście\n\n");
	}
}