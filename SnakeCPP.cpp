// SnakeCPP.cpp : définit le point d'entrée pour l'application.
//

#include "stdafx.h"
#include "SnakeCPP.h"

#define MAX_LOADSTRING 100
//déclaration de la structure joueur
struct SJoueur
{
	char Nom[50];
	int NbreVictoire;
};

// Variables globales :
HINSTANCE hInst;                                // instance actuelle
WCHAR szTitle[MAX_LOADSTRING];                  // Le texte de la barre de titre
WCHAR szWindowClass[MAX_LOADSTRING];            // le nom de la classe de fenêtre principale
HWND hWnd, hWnd2;//fenêtre joueur 1 et joueur 2
SJoueur Joueur1, Joueur2;
HANDLE Fichier;



// Pré-déclarations des fonctions incluses dans ce module de code :
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
BOOL				LectureFichier(char Nom[], int Joueur);
BOOL				EcritureFichier();

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: placez ici le code.

    // Initialise les chaînes globales
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_SNAKECPP, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Effectue l'initialisation de l'application :
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }
	DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);//ouvre le dialog à l'ouverture de la fenêtre J1
	DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd2, About); //ouvre le dialog à l'ouverture de la fenêtre J2

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_SNAKECPP));

    MSG msg;

    // Boucle de messages principale :
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  FONCTION : MyRegisterClass()
//
//  BUT : inscrit la classe de fenêtre.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SNAKECPP));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_SNAKECPP);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FONCTION : InitInstance(HINSTANCE, int)
//
//   BUT : enregistre le handle de l'instance et crée une fenêtre principale
//
//   COMMENTAIRES :
//
//        Dans cette fonction, nous enregistrons le handle de l'instance dans une variable globale, puis
//        créons et affichons la fenêtre principale du programme.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Stocke le handle d'instance dans la variable globale

   hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      10, 10, 500, 500, nullptr, nullptr, hInstance, nullptr);

   hWnd2 = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
	   520, 10, 500, 500, nullptr, nullptr, hInstance, nullptr);



   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   ShowWindow(hWnd2, nCmdShow);
   UpdateWindow(hWnd2);

   return TRUE;
}

//
//  FONCTION : WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  BUT :  traite les messages pour la fenêtre principale.
//
//  WM_COMMAND - traite le menu de l'application
//  WM_PAINT - dessine la fenêtre principale
//  WM_DESTROY - génère un message d'arrêt et retourne
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Analyse les sélections de menu :
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
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: ajoutez le code de dessin qui utilise hdc ici...
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

// Gestionnaire de messages pour la boîte de dialogue À propos de.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
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
	while (ReadFile(Fichier, &J, StructLong, &NbByte, NULL) && J.Nom != Nom && NbByte != 0)
	
		
	if (NbByte == 0)//Joueur non trouvé
	{
		return false;
	}
	else
	{
		if (Joueur == 1)
		{
			Joueur1 = J; //complie la structure du joueur 1
		}
		else
		{
			Joueur2 = J; //remplie la structure du joueur 2
		}
	}

	return true;
}


BOOL EcritureFichier()
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

	SetFilePointer(Fichier, 0, 0, 0);
	while (ReadFile(Fichier, &J, StructLong, &NbByte, NULL) && NbByte != 0)
	{
		if (J.Nom == Joueur1.Nom)
		{
			SetFilePointer(Fichier, -StructLong, 0, 1);//recule d'une position et écrit par dessus
			WriteFile(Fichier, &Joueur1, StructLong, &NbByte, NULL);//écrit joueur 1 à ca position
		}
		else
		{
			if (J.Nom == Joueur2.Nom)
			{
				SetFilePointer(Fichier, -StructLong, 0, 1);//recule d'une position et écrit par dessus
				WriteFile(Fichier, &Joueur2, StructLong, &NbByte, NULL);//écrit joueur 2 à ca position
			}
		}
	}

	return true;
}