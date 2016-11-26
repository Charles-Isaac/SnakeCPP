#pragma once
#include "Types.h"
class Snake
{
	//ICI vont les variable privés:
	//Oui, elle sont tous publiques pour le moment


public: //Met TOUTES le variables membres publiques
	Point *m_Position; //Tableau circulaire des segments du corp
	int m_IndexTete;
	int m_Vitesse;
	int m_LongueurDuSerpentCourant;
	char m_Direction;

	//Trois constructeur surchargés
	Snake::Snake()
	{
		m_Position = new Point[5];
		m_LongueurDuSerpentCourant = 5;
		m_IndexTete = 0;
		m_Vitesse = 100;
		m_Direction = 'D';
	} 
	Snake::Snake(int Vitesse, int Longueur)
	{
		m_Vitesse = Vitesse;
		m_Position = new Point[Longueur];
		m_LongueurDuSerpentCourant = Longueur;
		m_IndexTete = 0;
		m_Direction = 'D';
	}
	Snake::Snake(int Longueur)
	{
		m_Position = new Point[Longueur];
		m_LongueurDuSerpentCourant = Longueur;
		m_IndexTete = 0;
		m_Vitesse = 100;
		m_Direction = 'D';
	}

	//Les methodes utilisables:
	//int Snake::LongueurDuSerpent();  //retourne la longueur du serpent (nombre de segments)
	void Snake::Allonger(int Longueur); //ajoute n(Longueur) nombre de segments
	bool Snake::Update(char DirectionDuDeplacement, Point WindowSize);
	bool Snake::Update(Point WindowSize);
	

};