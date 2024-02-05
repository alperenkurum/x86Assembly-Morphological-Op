#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <iostream>
#include "image_processing.cpp"

using namespace std;

void Dilation(int n, int filter_size, short* resimadres_org);
void Erosion(int n, int filter_size, short* resimadres_org);

int main(void) {
	int M, N, Q, i, j, filter_size;
	bool type;
	int efile;
	char org_resim[100], dil_resim[] = "dilated.pgm", ero_resim[] = "eroded.pgm";
	do {
		printf("Orijinal resmin yolunu (path) giriniz:\n-> ");
		scanf("%s", org_resim);
		system("CLS");
		efile = readImageHeader(org_resim, N, M, Q, type);
	} while (efile > 1);
	int** resim_org = resimOku(org_resim);

	printf("Orjinal Resim Yolu: \t\t\t%s\n", org_resim);

	short *resimdizi_org = (short*) malloc(N*M * sizeof(short));

	for (i = 0; i < N; i++)
		for (j = 0; j < M; j++)
			resimdizi_org[i*N + j] = (short)resim_org[i][j];

	int menu;
	printf("Yapmak istediginiz islemi giriniz...\n");
	printf("1-) Dilation\n");
	printf("2-) Erosion\n");
	printf("3-) Cikis\n> ");
	scanf("%d", &menu);
	printf("Filtre boyutunu giriniz: ");
	scanf("%d", &filter_size);

	switch (menu){
		case 1:
			Dilation(N*M, filter_size, resimdizi_org);
			resimYaz(dil_resim, resimdizi_org, N, M, Q);
			break;
		case 2:
			Erosion(N*M, filter_size, resimdizi_org);
			resimYaz(ero_resim, resimdizi_org, N, M, Q);
			break;
		case 3:
			system("EXIT");
			break;
		default:
			system("EXIT");
			break;
	}

	system("PAUSE");
	return 0;
}

void Dilation(int n, int filter_size, short* resim_org) {
	__asm {
		MOV EDI,resim_org
		MOV EBX, filter_size
		SHR EBX, 1
		MOV EAX, 1026
		XOR EDX, EDX
		MUL EBX
		MOV EBX, EAX
		MOV ECX, 513
		SUB ECX, filter_size
L1:		MOV ESI,EBX
		MOV EDX,ECX
		MOV ECX, 513
		SUB ECX, filter_size
		
L2:		PUSH EDX
		PUSH EBX
		PUSH ECX
		MOV ECX, filter_size
		SHR ECX, 1
		MOV EAX, 1026
		XOR EDX, EDX
		MUL ECX
		SUB EBX,EAX
		MOV ECX, filter_size
		XOR AX,AX

L3:		PUSH EBX
		PUSH ECX
		MOV ECX,filter_size

L4:		CMP	WORD PTR[EBX+EDI],AX
		JBE F
		MOV AX,WORD PTR[EBX+EDI]

F:		ADD EBX,2
		LOOP L4
		POP ECX
		POP EBX
		ADD EBX, 1024
		LOOP L3
		POP ECX
		POP EBX
		ADD EBX, 2
		POP EDX
		PUSH AX
		LOOP L2
		MOV ECX,EDX
		MOV EBX,ESI
		ADD EBX, 1024
		LOOP L1

		
		MOV EBX, filter_size
		SHR EBX, 1
		MOV EAX, 1026
		XOR EDX, EDX
		MUL EBX
		MOV EBX,n
		DEC EBX
		SHL EBX,1
		SUB EBX,EAX

		MOV ECX, 513
		SUB ECX, filter_size

L5:		MOV ESI,EBX
		MOV EDX,ECX
		MOV ECX, 513
		SUB ECX, filter_size
L6:		
		POP AX
		MOV WORD PTR[ESI+EDI],AX

		SUB ESI,2
		LOOP L6
		SUB EBX, 1024
		MOV ECX,EDX
		LOOP L5
	}
	printf("\nDilation islemi sonucunda resim \"dilated.pgm\" ismiyle olusturuldu...\n");
}

void Erosion(int n, int filter_size, short* resim_org) {
	
	__asm {
		MOV EDI,resim_org
		MOV EBX, filter_size
		SHR EBX, 1
		MOV EAX, 1026
		XOR EDX, EDX
		MUL EBX
		MOV EBX, EAX
		MOV ECX, 513
		SUB ECX, filter_size
L1:		MOV ESI,EBX
		MOV EDX,ECX
		MOV ECX, 513
		SUB ECX, filter_size
		
L2:		PUSH EDX
		PUSH EBX
		PUSH ECX
		MOV ECX, filter_size
		SHR ECX, 1
		MOV EAX, 1026
		XOR EDX, EDX
		MUL ECX
		SUB EBX,EAX
		MOV ECX, filter_size
		MOV AX, 255

L3:		PUSH EBX
		PUSH ECX
		MOV ECX,filter_size

L4:		CMP	WORD PTR[EBX+EDI],AX
		JA F
		MOV AX,WORD PTR[EBX+EDI]

F:		ADD EBX,2
		LOOP L4
		POP ECX
		POP EBX
		ADD EBX, 1024
		LOOP L3
		POP ECX
		POP EBX
		ADD EBX, 2
		POP EDX
		PUSH AX
		LOOP L2
		MOV ECX,EDX
		MOV EBX,ESI
		ADD EBX, 1024
		LOOP L1

		
		MOV EBX, filter_size
		SHR EBX, 1
		MOV EAX, 1026
		XOR EDX, EDX
		MUL EBX
		MOV EBX,n
		DEC EBX
		SHL EBX,1
		SUB EBX,EAX

		MOV ECX, 513
		SUB ECX, filter_size

L5:		MOV ESI,EBX
		MOV EDX,ECX
		MOV ECX, 513
		SUB ECX, filter_size
L6:		
		POP AX
		MOV WORD PTR[ESI+EDI],AX

		SUB ESI,2
		LOOP L6
		SUB EBX, 1024
		MOV ECX,EDX
		LOOP L5

	}
	printf("\nErosion islemi sonucunda resim \"eroded.pgm\" ismiyle olusturuldu...\n");
}