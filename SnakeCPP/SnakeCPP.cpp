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

CRITICAL_SECTION cSect;
HANDLE ThreadHandle;
HANDLE g_mut;

DWORD AutreProcId;
STARTUPINFO sinfo;
PROCESS_INFORMATION infop;

SJoueur Joueur1;
Snake serp = Snake(11, 20);
Point m_Limite;
char Direction = 'D';
bool JoueurPret = false, AutreJoueurPret = false, JoueurPrinc = false;


// Pré-déclarations des fonctions :
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
BOOL				LectureFichier(char Nom[], int Joueur); //cherche le joueur si ne trouve pas retourne faux
BOOL				EcritureFichier(bool Joueur, SJoueur P);//écrit dans le fichier à la fin si je joueur n'existe pas
BOOL CALLBACK		EnumFen(HWND, LPARAM);
DWORD WINAPI ThreadAllonge(LPVOID);

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

	char myPath[_MAX_PATH + 1];
	GetModuleFileName(NULL, myPath, _MAX_PATH);

	// Largeur et hauteur de l'écran :
	HDC hDC = ::GetWindowDC(NULL);
	int L = GetDeviceCaps(hDC, HORZRES);
	int H = GetDeviceCaps(hDC, VERTRES);

	// Limites de déplacement du serpent :
	m_Limite = Point(L / 2, H);

	//Set la position de la fenetre
	::SetWindowPos(hWndJ1, NULL, L - L / 2, 0, L / 2, H, SWP_NOOWNERZORDER | SWP_FRAMECHANGED);;

	//Génère un erreur "ERROR_ALREADY_EXISTS" si le process existe déjà (recommandé par aide windows WinMain)
	g_mut = CreateMutex(NULL, TRUE, "poil");

	//Si un 2e process n'existe pas déjà crée un 2e process
	if (GetLastError() != ERROR_ALREADY_EXISTS)
	{
		::SetWindowPos(hWndJ1, NULL, 0, 0, L / 2, H, SWP_FRAMECHANGED);
		CreateProcess(myPath,NULL, 0, 0, TRUE, NORMAL_PRIORITY_CLASS, NULL, NULL, &sinfo, &infop);
		AutreProcId = infop.dwProcessId;
		JoueurPrinc = true;
	}

	Sleep(500);

	//EnumWindows(EnumFen, AutreProcId);
	EnumWindows(EnumFen, NULL);
	// Ouvre le DialogBox About à l'ouverture de la fenêtre J1 :
	DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWndJ1, About);

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_SNAKECPP));

	

	//Initialisation de la section critique déplacement/Allonger
	InitializeCriticalSection(&cSect);


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

	// Affichage des fenêtres :
	ShowWindow(hWndJ1, nCmdShow);
	UpdateWindow(hWndJ1);

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
		EnterCriticalSection(&cSect);
		if (serp.Update(Direction, m_Limite))
		{
			KillTimer(hWndJ1, 1);
			TerminateThread(ThreadHandle, 0);
			SendMessage(hWndJ2, 0x8002, 0, 0);
		}
		LeaveCriticalSection(&cSect);

		InvalidateRect(hWnd, NULL, true);
		UpdateWindow(hWnd);
	}
	break;

	case WM_LBUTTONDOWN:
	{
		if (JoueurPrinc)
		{
			SetForegroundWindow(hWndJ1);
		}
		else
		{
			SetForegroundWindow(hWndJ2);
		}
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
		SendMessage(hWndJ2, 0x8001,'W', NULL);
		break;
		case VK_DOWN:
		SendMessage(hWndJ2, 0x8001,'S', NULL);
		break;
		case VK_LEFT:
		SendMessage(hWndJ2, 0x8001,'A', NULL);
		break;
		case VK_RIGHT:
		SendMessage(hWndJ2, 0x8001,'D', NULL);
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

		EndPaint(hWnd, &ps);
	}
	break;

	case 0x8000: //Autre joueur prêt
	{
		AutreJoueurPret = true;
		if (AutreJoueurPret && JoueurPret)
		{
			SetTimer(hWndJ1, 1, 200, NULL);
			ThreadHandle = CreateThread(NULL, 0, ThreadAllonge, 0, 0, NULL);
			SetThreadPriority(ThreadHandle, 1);
		}
	}
	break;
	case 0x8001: //envoi de direction
	{
		Direction = (char)wParam;
	}
	break;
	case 0x8002: //Crash de l'adversaire
	{
		KillTimer(hWndJ1, 1);
		TerminateThread(ThreadHandle, 0);
		Joueur1.NbreVictoire++;
		AutreJoueurPret = false;
		int res;
		if (JoueurPrinc)
		{
			res = MessageBox(hWndJ1, "Victoire de joueur 1. Voulez vous rejouer?", "Fin", MB_YESNO);
		}
		else
		{
			res = MessageBox(hWndJ1, "Victoire de joueur 2. Voulez vous rejouer?", "Fin", MB_YESNO);
		}
		if (res == IDYES)
		{
			SendMessage(hWndJ1, 0x8003, 0, 0);
			SendMessage(hWndJ2, 0x8003, 0, 0);
		}
		else
		{
			SendMessage(hWndJ1, WM_CLOSE, 0, 0);
			SendMessage(hWndJ2, WM_CLOSE, 0, 0);
		}
	}
	break;
	case 0x8003: //Reset du jeu
	{
		JoueurPret = true;
		Direction = 'D';
		serp = Snake(11, 20);
		SendMessage(hWndJ2, 0x8000, 0, 0);
		if (JoueurPrinc)
		{
			SetForegroundWindow(hWndJ1);
		}
		else
		{
			SetForegroundWindow(hWndJ2);
		}
	}
	break;

	case WM_DESTROY:
		WaitForSingleObject(g_mut, 100);
		EcritureFichier(true, Joueur1);
		ReleaseMutex(g_mut);
		SendMessage(hWndJ2, WM_CLOSE, 0, 0);
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

			J1[0] = 0;


			GetDlgItemText(hDlg, IDC_TXTJ1, J1, 50);


			//Vérifie si un joueur a été saisi
			if (J1[0] == 0)
			{
				MessageBoxA(hDlg, "Veuillez entrer le joueur 1", "Erreur", MB_OK);
				return (INT_PTR)TRUE;
			}

			//Vérifie si les joueurs sont présents et entre les info
			if (!LectureFichier(J1, 1))
			{
				EcritureFichier(false, Joueur1);
			}

			JoueurPret = true;
			SendMessage(hWndJ2, 0x8000, 0, 0);
			if (JoueurPret && AutreJoueurPret)
			{
				SetTimer(hWndJ1, 1, 200, NULL);
				ThreadHandle = CreateThread(NULL, 0, ThreadAllonge, 0, 0, NULL);
				SetThreadPriority(ThreadHandle, 1);
			}

			EndDialog(hDlg, LOWORD(wParam));
			if (!JoueurPrinc)
			{
				SetForegroundWindow(hWndJ2);
			}
			return (INT_PTR)TRUE;
		}
		if (LOWORD(wParam) == IDCANCEL)
		{
			SendMessage(hWndJ2, WM_CLOSE, 0, 0);
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
	CloseHandle(Fichier);
	Fichier = NULL;
return true;
}

//Écriture des joueurs dans le fichier
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
		while (ReadFile(Fichier, &J, StructLong, &NbByte, NULL) && strcmp(J.Nom, P.Nom) && NbByte != 0) {}
		//écriture du joueur à sa position
		if (NbByte > 0)
		{
			SetFilePointer(Fichier, -StructLong, 0, 1);
			WriteFile(Fichier, &P, StructLong, &NbByte, NULL);//réécrit joueur à sa position
		}

	}
	CloseHandle(Fichier);
	Fichier = NULL;
	return true;
}


//Recherche de la fenêtre de l'autre application
BOOL CALLBACK EnumFen(HWND hwnd, LPARAM lParam)
{
	char Nom1[80];
	char Nom2[80];
	GetWindowText(hwnd, Nom1, 80);
	GetWindowText(hWndJ1, Nom2, 80);

	if (strcmp(Nom1,Nom2) == 0 &&  hwnd != hWndJ1)
	{
		hWndJ2 = hwnd;
		return FALSE;
	}
	return TRUE;
}

//Thread servant à allonger le serpent
DWORD WINAPI ThreadAllonge(LPVOID lParam)
{
	int Vitesse = 200;
	while(JoueurPret && AutreJoueurPret)
	{
		Sleep(5000);
		EnterCriticalSection(&cSect);
		serp.Allonger(1);
		LeaveCriticalSection(&cSect);
		if (JoueurPret && AutreJoueurPret)
		{
			KillTimer(hWndJ1, 1);
			SetTimer(hWndJ1, 1, Vitesse, NULL);
			if (Vitesse > 50)
			{
				Vitesse -= 10;
			}
		}
	}

	return true;
}