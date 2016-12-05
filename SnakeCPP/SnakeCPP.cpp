#include "stdafx.h"
#include "SnakeCPP.h"
#include "Snake.h"
#include <time.h>
#include <stdio.h>

#define MAX_LOADSTRING 100

//Déclaration de la structure joueur :
struct SJoueur
{
	char Nom[50];
	int NbreVictoire;
};


// Variables globales :
HINSTANCE hInst;
WCHAR szTitle[MAX_LOADSTRING];
WCHAR szWindowClass[MAX_LOADSTRING];

HANDLE Fichier;
HWND hWndJ1;
HWND hWndJ2;
INT APP;

SJoueur Joueur1, Joueur2;
Snake serp = Snake(11, 20);
Snake serp2 = Snake(11, 20);
Point m_Limite;
char Direction = 'D';


// Pré-déclarations des fonctions :
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
BOOL				LectureFichier(char Nom[], int Joueur); //cherche le joueur si ne trouve pas retourne faux
BOOL				EcritureFichier(bool Joueur, SJoueur P);//écrit dans le fichier à la fin si je joueur n'existe pas
//DWORD WINAPI		GestionGame(LPVOID lParam); //méthode pour thread															//sinon écrit a sa position



int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// Initialise les chaînes globales :
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_SNAKECPP, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Effectue l'initialisation de l'application :
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	// Ouvre le DialogBox About à l'ouverture de la fenêtre J1 :
	DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWndJ1, About);

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_SNAKECPP));

	MSG msg;

	// Boucle de messages principale :
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int)msg.wParam;
}




ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SNAKECPP));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_SNAKECPP);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}




BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance;

	// Création de la fenêtre de droite :
	hWndJ1 = CreateWindowExW(WS_EX_APPWINDOW | WS_EX_CLIENTEDGE | WS_EX_DLGMODALFRAME, szWindowClass,
		szTitle, WS_OVERLAPPED | WS_POPUP, CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

	// Ouverture du même processus :
	DWORD IdentifiantProc = GetProcessId(hWndJ1);
	HANDLE h2 = OpenProcess(IdentifiantProc, TRUE, GetCurrentProcessId());
	hWndJ2 = GetActiveWindow();

	// Création de la fenêtre de gauche :
	hWndJ2 = CreateWindowExW(WS_EX_APPWINDOW | WS_EX_CLIENTEDGE | WS_EX_DLGMODALFRAME, szWindowClass,
		szTitle, WS_OVERLAPPED | WS_POPUP, CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

	// Largeur et hauteur de l'écran :
	HDC hDC = ::GetWindowDC(NULL);
	int L = GetDeviceCaps(hDC, HORZRES);
	int H = GetDeviceCaps(hDC, VERTRES);

	// Limites de déplacement du serpent :
	m_Limite = Point(L / 2, H);

	// Postion des fenêtres :
	::SetWindowPos(hWndJ1, NULL, 0, 0, L / 2, H, SWP_NOZORDER | SWP_FRAMECHANGED);
	::SetWindowPos(hWndJ2, NULL, L - L / 2, 0, L / 2, H, SWP_NOZORDER | SWP_FRAMECHANGED);

	//DWORD IDThread;
	//HANDLE handle = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)&GestionGame, NULL,NULL, &IDThread);  

	if (!hWndJ1)
	{
		return FALSE;
	}

	// Affichage des fenêtres :
	ShowWindow(hWndJ1, nCmdShow);
	UpdateWindow(hWndJ1);
	ShowWindow(hWndJ2, nCmdShow);
	UpdateWindow(hWndJ2);

	return TRUE;
}




LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);

		// Analyse les sélections de menu :
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
	break;
	case WM_TIMER:
	{
		// Si le serpent entre en collision :
		if (serp.Update(Direction, m_Limite) || serp2.Update(serp2.m_Direction, m_Limite))
		{
			KillTimer(hWndJ1, 1);
			KillTimer(hWndJ2, 2);
			MessageBoxA(hWnd, "boom bitch", "Collision genre", NULL);
		}

		InvalidateRect(hWnd, NULL, true);
		UpdateWindow(hWnd);
	}
	break;
	case WM_KEYDOWN:
	{
		// Direction du serpent du joueur 1 selon " W A S D " :
		Direction = (char)wParam;

		// Direction du serpent du joueur 2 selon les flèches :
		switch (Direction)
		{
		case VK_UP:
		SendMessage(hWndJ2, WM_SETTEXT, serp2.m_Direction = 'W', NULL);
		break;
		case VK_DOWN:
		SendMessage(hWndJ2, WM_SETTEXT, serp2.m_Direction = 'S', NULL);
		break;
		case VK_LEFT:
		SendMessage(hWndJ2, WM_SETTEXT, serp2.m_Direction = 'A', NULL);
		break;
		case VK_RIGHT:
		SendMessage(hWndJ2, WM_SETTEXT, serp2.m_Direction = 'D', NULL);
		break;
		}
	}
	break;
	case WM_PAINT:
	{
		char Aff[100];
		int n;

		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);

		//Joueur1
		if (hWnd == hWndJ1)
		{
			//Dessine le serpent
			for (int I = 0; I < serp.m_LongueurDuSerpentCourant; I++)
			{
				Rectangle(hdc, serp.m_Position[I].X, serp.m_Position[I].Y, 
					  serp.m_Position[I].X + 10, serp.m_Position[I].Y + 10);
			}

			//Affiche les informations
			n = sprintf_s(Aff, "Nom: %s, Nombre de victoires: %i \0", Joueur1.Nom, Joueur1.NbreVictoire);
			TextOut(hdc ,m_Limite.X / 2, 10, Aff,n);
		}

		//Joueur2
		if (hWnd == hWndJ2)
		{
			//Dessine le serpent
			for (int I = 0; I < serp2.m_LongueurDuSerpentCourant; I++)
			{
				Rectangle(hdc, serp2.m_Position[I].X, serp2.m_Position[I].Y, 
					  serp2.m_Position[I].X + 10, serp2.m_Position[I].Y + 10);
			}

			//Affiche les informations
			n = sprintf_s(Aff, "Nom: %s, Nombre de victoires: %i \0", Joueur2.Nom, Joueur2.NbreVictoire);
			TextOut(hdc, m_Limite.X / 2, 10, Aff, n);
		}

		EndPaint(hWnd, &ps);
	}
	break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}




// Boîte de dialogue À propos de :
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK)
		{
			char J1[50];
			char J2[50];
			J1[0] = 0;
			J2[0] = 0;

			GetDlgItemText(hDlg, IDC_TXTJ1, J1, 50);
			GetDlgItemText(hDlg, IDC_TXTJ2, J2, 50);

			//Vérifie si un joueur a été saisi
			if (J1[0] == 0)
			{
				MessageBoxA(hDlg, "Veuillez entrer le joueur 1", "Erreur", MB_OK);
				return (INT_PTR)TRUE;
			}
			if (J2[0] == 0)
			{
				MessageBoxA(hDlg, "Veuillez entrer le joueur 2", "Erreur", MB_OK);
				return (INT_PTR)TRUE;
			}
			//Vérifie s'il s'agit de joueurs différents
			if (strcmp(J1, J2) == 0)
			{
				MessageBoxA(hDlg, "Veuillez entrer 2 joueurs differents", "Erreur", MB_OK);
				return (INT_PTR)TRUE;
			}

			//Vérifie si les joueurs sont présents et entre les info
			if (!LectureFichier(J1, 1))
			{
				EcritureFichier(false, Joueur1);
			}
			if (!LectureFichier(J2, 2))
			{
				EcritureFichier(false, Joueur2);
			}

			// Initialise le Timer :
			SetTimer(hWndJ1, 1, 200, NULL);
			SetTimer(hWndJ2, 2, 200, NULL);

			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		if (LOWORD(wParam) == IDCANCEL)
		{
			SendMessage(hWndJ1, WM_CLOSE, 0, 0);
		}
		break;
	}
	return (INT_PTR)FALSE;
}

//fonction de lecture dans le fichier binaire
BOOL LectureFichier(char Nom[], int Joueur)
{
DWORD NbByte;
SJoueur J;
int StructLong = sizeof(SJoueur);
if (Fichier == NULL)
{
Fichier = CreateFile("Joueur.bd", GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
}
if (Fichier == INVALID_HANDLE_VALUE)
{
return false;
}
SetFilePointer(Fichier, 0, 0, 0);
//Boucle de recherche
while (ReadFile(Fichier, &J, StructLong, &NbByte, NULL) && strcmp(J.Nom,Nom) != 0 && NbByte != 0) {}
//while (ReadFile(Fichier, &J, StructLong, &NbByte, NULL) && J.Nom != Nom && NbByte != 0) {}


if (Joueur == 1)
{
	if (NbByte == 0)//Joueur non trouvé
	{
		strcpy_s(Joueur1.Nom, Nom);
		Joueur1.NbreVictoire = 0;
		return false;
	}
	else
	{
		Joueur1 = J; //remplit la structure du joueur 1
	}
}
else
{
	if (NbByte == 0)//Joueur non trouvé
	{
		strcpy_s(Joueur2.Nom, Nom);
		Joueur2.NbreVictoire = 0;
		return false;
	}
	else
	{
		Joueur2 = J; //remplit la structure du joueur 2
	}
	
}
return true;
}


BOOL EcritureFichier(bool Joueur, SJoueur P)
{
	SJoueur J;
	DWORD NbByte;
	int StructLong = sizeof(SJoueur);
	if (Fichier == NULL)
	{
		Fichier = CreateFile("Joueur.bd", GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	}
	if (Fichier == INVALID_HANDLE_VALUE)
	{
		return false;
	}
	//si joueur n'est pas dans le fichier l'écrit à la fin du fichier
	if (Joueur == false)
	{
		SetFilePointer(Fichier, 0, 0, 2);
		WriteFile(Fichier, &P, StructLong, &NbByte, NULL);//écrit joueur à la fin du fichier
	}
	else
	{
		SetFilePointer(Fichier, 0, 0, 0);//remet le pointer au début du fichier
		//boucle qui cherche le joueur
		while (ReadFile(Fichier, &J, StructLong, &NbByte, NULL) && J.Nom != P.Nom && NbByte != 0) {}
		//écriture du joueur à sa position
		if (NbByte > 0)
		{
			SetFilePointer(Fichier, -StructLong, 0, 1);
			WriteFile(Fichier, &P, StructLong, &NbByte, NULL);//réécrit joueur à sa position
		}

	}
	return true;
}
/*
DWORD WINAPI GestionGame(LPVOID lParam)
{
}
*/
