#pragma once

class Snake
{
	//ICI vont les variable privés:
	//Oui, elle sont tous publiques pour le moment


public: //Met TOUTES le variables membres publiques
	POINT* m_Position; //Tableau circulaire des segments du corp
	int m_IdexTete;
	int m_Vitesse;

	//Trois constructeur surchargés
	Snake()
	{
		m_Position = new POINT[5]{ 0 };
		m_IdexTete = 0;
		m_Vitesse = 100;
	} 
	Snake(int Vitesse) : Snake()
	{
		m_Vitesse = Vitesse;
	}
	Snake(int Vitesse, int Longueur)
	{
		m_Position = new POINT[Longueur]{ 0 };
		m_IdexTete = 0;
		m_Vitesse = Vitesse;
	}

	//Les methodes utilisables:
	int LongueurDuSerpent();  //retourne la longueur du serpent (nombre de segments)
	void Allonger(int Longueur); //ajoute n(Longueur) nombre de segments
	void Update(char DirectionDuDeplacement);
	

};