#include <iostream>
#include <string>
#include <fstream>

using namespace std;

const string colors[4] = { "Yesil", "Kirmizi", "Sari", "Mavi" };
const string yesil[4] = { "Y1", "Y2", "Y3", "Y4" };
const string mavi[4] = { "M1", "M2", "M3", "M4" };
const string kirmizi[4] = { "K1", "K2", "K3", "K4" };
const string sari[4] = { "S1", "S2", "S3", "S4" };

const string pathLocations[40] = //Gidebileceði yollar
{
	"6,10","6,9","6,8","6,7","6,6","7,6","8,6","9,6","10,6","10,5","10,4","9,4","8,4","7,4","6,4","6,3","6,2","6,1",
	"6,0","5,0","4,0","4,1","4,2","4,3","4,4","3,4","2,4","1,4","0,4","0,5","0,6","1,6","2,6","3,6","4,6","4,7","4,8","4,9","4,10","5,10"
};

bool yesil_aktif_piyonlar[4] = { false };
bool mavi_aktif_piyonlar[4] = { false };
bool kirmizi_aktif_piyonlar[4] = { false };
bool sari_aktif_piyonlar[4] = { false };

bool yesil_setted_piyonlar[4] = { false };
bool mavi_setted_piyonlar[4] = { false };
bool kirmizi_setted_piyonlar[4] = { false };
bool sari_setted_piyonlar[4] = { false };

int loadGameTempSira = 0;

int dice() {
	return rand() % 6 + 1;
}

void swap(int* xp, int* yp)
{
	int temp = *xp;
	*xp = *yp;
	*yp = temp;
}

void printBoard(string(&tahta)[11][11]) {
	cout << " ____ ____ ____ ____ ____ ____ ____ ____ ____ ____ ____" << endl;
	for (int i = 0; i < 11; i++) {
		cout << "|    |    |    |    |    |    |    |    |    |    |    |\n";
		for (int j = 0; j < 11; j++) {
			if (j == 0) {
				cout << "| ";
			}
			else {
				cout << " | ";
			}
			cout << tahta[i][j];
			if (j == 10) {
				cout << " |" << endl;
			}
		}
		cout << "|____|____|____|____|____|____|____|____|____|____|____|\n";
	}
}

void printPlayers(int playerCount, string* players, string* playersWithColors, int* orders) {
	cout << " _______________ _______________________ _______________" << endl;
	cout << "|               |                       |               |\n";
	cout << "|   Kullanici   |        Renk           |      Sira     |\n";
	cout << "|_______________|_______________________|_______________|\n";
	for (int i = 0; i < playerCount; i++) {
		cout << "|		|			|		|\n";
		for (int j = 0; j < 3; j++) {
			if (j == 0) {
				cout << "|	" << players[i] << "	|	" << playersWithColors[i] << "		|	" << orders[i] << "	|";
			}
			if (j == 2) {
				cout << endl;
			}
		}
		cout << "|_______________|_______________________|_______________|\n";
	}
}

void setPlayers(int playerCount, string* players, int* playersListWithIndex) {
	string tempNickName = "";
	bool exists = false, firstRun = true;
	for (int i = 0; i < playerCount; i++) {
		if (firstRun) {
			cout << endl << 1 << ". Oyuncunun kullanici adini giriniz: ";
			cin >> tempNickName;
			players[0] = tempNickName;
			firstRun = false;
		}
		else {
			do {
				cout << i + 1 << ". Oyuncunun kullanici adini giriniz: ";
				cin >> tempNickName;
				exists = find(players, players + playerCount, tempNickName) != players + playerCount;
				if (!exists) {
					players[i] = tempNickName;
				}
				else {
					cout << endl << "!!! Bu kullanici adina sahip bir oyuncu zaten var." << endl;
				}
			} while (exists);
		}
		playersListWithIndex[i] = i + 1;
	}
}

void setColors(int playerCount, string* players, string* playersWithColors) {
	int randomNum = 0;
	int* randoms = new int[playerCount];
	bool exists = false, firstRun = true;
	srand((unsigned int)time(NULL));
	for (int i = 0; i < playerCount; i++) {
		if (firstRun) {
			randomNum = rand() % 4;
			randoms[0] = randomNum;
			firstRun = false;
		}
		else {
			do {
				randomNum = rand() % 4;
				exists = find(randoms, randoms + playerCount, randomNum) != randoms + playerCount;
				if (!exists) {
					randoms[i] = randomNum;
				}
			} while (exists);
		}
		playersWithColors[i] = colors[randoms[i]];
	}
}

void setOrder(int playerCount, string* players, string* playersWithColors, int* orders, int* playersListWithIndex) {
	srand((unsigned int)time(NULL));
	int* tempDices = new int[playerCount];
	int tempDice = 0;
	bool exists = false, firstRun = true;
	cout << "Sira Olusturuyor." << endl << endl;
	for (int i = 0; i < playerCount; i++) {
		if (firstRun) {
			tempDice = dice();
			tempDices[0] = tempDice;
			firstRun = false;
		}
		else {
			do {
				tempDice = dice();
				exists = find(tempDices, tempDices + playerCount, tempDice) != tempDices + playerCount;
				if (!exists) {
					tempDices[i] = tempDice;
				}
			} while (exists);
		}
		cout << i + 1 << ". Oyuncu: " << players[i] << " Zar Atiyor: " << tempDice << endl;
	}
	//Atýlan Zara Göre Bubble Sort
	for (int i = 0; i < playerCount - 1; i++) {
		for (int j = 0; j < playerCount - i - 1; j++) {
			if (tempDices[j] < tempDices[j + 1]) {
				swap(tempDices[j], tempDices[j + 1]);
				swap(players[j], players[j + 1]);
				swap(playersWithColors[j], playersWithColors[j + 1]);
				swap(playersListWithIndex[j], playersListWithIndex[j + 1]);
			}
		}
	}
	for (int i = 0; i < playerCount; i++) {
		orders[i] = i + 1;
	}
}

void writePlayersToFile(int playerCount, string* players, string* playersWithColors, int* orders, int* playersListWithIndex) {
	try
	{
		ofstream myfile;
		myfile.open("sira.txt");
		myfile << "Oyuncu Sayisi: " << playerCount << endl;
		myfile << " _______________ _______________________ _______________" << endl;
		myfile << "|               |                       |               |\n";
		myfile << "|    Oyuncu     |        Renk           |      Sira     |\n";
		myfile << "|_______________|_______________________|_______________|\n";
		for (int i = 0; i < playerCount; i++) {
			myfile << "|		|			|		|\n";
			for (int j = 0; j < 3; j++) {
				if (j == 0) {
					myfile << "|	" << players[i] << "	|	" << playersWithColors[i] << "		|	" << orders[i] << "	|";
				}
				if (j == 2) {
					myfile << endl;
				}
			}
			myfile << "|_______________|_______________________|_______________|\n";
		}
		myfile.close();
	}
	catch (const std::exception&)
	{

	}
}

void writeActionsToFile(string oyuncu,string renk,int zar,string action,int minIndex) {
	try
	{
		ofstream myfile;
		myfile.open("sira.txt", std::ios_base::app);
		if(action=="PIYON_SAL" && minIndex<4)
			myfile << "\nEn son oynayan oyuncu: " << oyuncu << " rengi: " << renk << " attigi zar: " << zar << " action: " << action  << " salinan piyon: " << renk[0] << minIndex+1 << endl;
		else if(action != "PIYON_SAL")
			myfile << "\nEn son oynayan oyuncu: " << oyuncu << " rengi: " << renk << " attigi zar: " << zar << " action: " << action << endl;
		myfile.close();
	}
	catch (const std::exception&)
	{

	}
}

void setBoard(string(&tahta)[11][11])
{
	int calismaSayisi = 0;
	for (int i = 0; i < 11; i++)
	{
		for (int j = 0; j < 11; j++)
		{
			tahta[i][j] = "  ";
		}
	}
	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			tahta[i][j] = mavi[calismaSayisi];
			calismaSayisi++;
		}
		calismaSayisi = calismaSayisi - 2;
		for (int j = 9; j < 11; j++)
		{
			tahta[i][j] = sari[calismaSayisi];
			calismaSayisi++;
		}
	}
	calismaSayisi = 0;
	for (int i = 10; i > 8; i--)
	{
		for (int j = 0; j < 2; j++)
		{
			tahta[i][j] = yesil[calismaSayisi];
			calismaSayisi++;
		}
		calismaSayisi = calismaSayisi - 2;
		for (int j = 9; j < 11; j++)
		{
			tahta[i][j] = kirmizi[calismaSayisi];
			calismaSayisi++;
		}
	}
	printBoard(tahta);
}

bool input() {
	cout << "Oynamak icin herhangi bir tusa basiniz...";
	string x = "";
	cin >> x;
	return true;
}

bool aktifPiyonVarMi(string color) {
	switch (color[0])
	{
	case 'K':
		for (int i = 0; i < 4; i++) {
			if (kirmizi_aktif_piyonlar[i] && !kirmizi_setted_piyonlar[i]) {
				return true;
			}
		}
		break;
	case 'M':
		for (int i = 0; i < 4; i++) {
			if (mavi_aktif_piyonlar[i] && !mavi_setted_piyonlar[i]) {
				return true;
			}
		}
		break;
	case 'Y':
		for (int i = 0; i < 4; i++) {
			if (yesil_aktif_piyonlar[i] &&  !yesil_setted_piyonlar[i])  {
				return true;
			}
		}
		break;
	case 'S':
		for (int i = 0; i < 4; i++) {
			if (sari_aktif_piyonlar[i] && !sari_setted_piyonlar[i]) {
				return true;
			}
		}
		break;
	}
	return false;
}

void replacePiyonToKale(string text, string(&tahta)[11][11],string currentText) {
	if (text != "  ") {
		if (text == "K1") {
			tahta[10][9] = "K1";
			kirmizi_aktif_piyonlar[0] = false;
		}
		if (text == "K2") {
			tahta[10][10] = "K2";
			kirmizi_aktif_piyonlar[1] = false;
		}
		if (text == "K3") {
			tahta[9][9] = "K3";
			kirmizi_aktif_piyonlar[2] = false;
		}
		if (text == "K4") {
			tahta[9][10] = "K4";
			kirmizi_aktif_piyonlar[3] = false;
		}

		if (text == "M1") {
			tahta[0][0] = "M1";
			mavi_aktif_piyonlar[0] = false;
		}
		if (text == "M2") {
			tahta[0][1] = "M2";
			mavi_aktif_piyonlar[1] = false;
		}
		if (text == "M3") {
			tahta[1][0] = "M3";
			mavi_aktif_piyonlar[2] = false;
		}
		if (text == "M4") {
			tahta[1][1] = "M4";
			mavi_aktif_piyonlar[3] = false;
		}

		if (text == "S1") {
			tahta[0][9] = "S1";
			sari_aktif_piyonlar[0] = false;
		}
		if (text == "S2") {
			tahta[0][10] = "S2";
			sari_aktif_piyonlar[1] = false;
		}
		if (text == "S3") {
			tahta[1][9] = "S3";
			sari_aktif_piyonlar[2] = false;
		}
		if (text == "S4") {
			tahta[1][10] = "S4";
			sari_aktif_piyonlar[3] = false;
		}

		if (text == "Y1") {
			tahta[10][0] = "Y1";
			yesil_aktif_piyonlar[0] = false;
		}
		if (text == "Y2") {
			tahta[10][1] = "Y2";
			yesil_aktif_piyonlar[1] = false;
		}
		if (text == "Y3") {
			tahta[9][0] = "Y3";
			yesil_aktif_piyonlar[2] = false;
		}
		if (text == "Y4") {
			tahta[9][1] = "Y4";
			yesil_aktif_piyonlar[3] = false;
		}
		try
		{
			ofstream myfile;
			myfile.open("sira.txt", std::ios_base::app);
			myfile << text << " adlý piyon " << currentText << " tarafýndan kaleye geri gönderildi." << endl;
			myfile.close();
		}
		catch (const std::exception&)
		{

		}
	}
}

void edgeControl(string(&tahta)[11][11], int atilanZar, string x, int currentI, int currentJ) {
	string location = to_string(currentI) + "," + to_string(currentJ);
	string newLocation = "";
	int parserLocation = 0;
	int newI = 0, newJ = 0;
	string tempStr = "";
	bool set = false;
	for (int i = 0; i < 40; i++) {
		if (pathLocations[i] == location) {
			newLocation = pathLocations[(i + atilanZar) % 40];
			if (i + atilanZar > 39 && x.find('K') != string::npos) {
				cout << "Kirmizi Bir Turu Tamamladi." << endl;
				kirmizi_aktif_piyonlar[int(x[1]) - 49] = false;
				switch (int(x[1]) - 48)
				{
				case 1:
					tahta[5][6] = x;
					break;
				case 2:
					tahta[5][7] = x;
					break;
				case 3:
					tahta[5][8] = x;
					break;
				case 4:
					tahta[5][9] = x;
					break;
				default:
					break;
				}
				tahta[currentI][currentJ] = "  ";
				set = true;
			}
			if (i < 20 && i + atilanZar>19 && x.find('M') != string::npos) {
				cout << "Mavi Bir Turu Tamamladi." << endl;
				mavi_aktif_piyonlar[int(x[1]) - 49] = false;
				switch (int(x[1]) - 48)
				{
				case 1:
					tahta[5][4] = x;
					break;
				case 2:
					tahta[5][3] = x;
					break;
				case 3:
					tahta[5][2] = x;
					break;
				case 4:
					tahta[5][1] = x;
					break;
				default:
					break;
				}
				tahta[currentI][currentJ] = "  ";
				set = true;
			}
			if (i < 30 && i + atilanZar>29 && x.find('S') != string::npos) {
				cout << "Sari Bir Turu Tamamladi." << endl;
				sari_aktif_piyonlar[int(x[1]) - 49] = false;
				switch (int(x[1]) - 48)
				{
				case 1:
					tahta[4][5] = x;
					break;
				case 2:
					tahta[3][5] = x;
					break;
				case 3:
					tahta[2][5] = x;
					break;
				case 4:
					tahta[1][5] = x;
					break;
				default:
					break;
				}
				tahta[currentI][currentJ] = "  ";
				set = true;
			}
			if (i < 10 && i + atilanZar>9 && x.find('Y') != string::npos) {
				cout << "Yesil Bir Turu Tamamladi." << endl;
				yesil_aktif_piyonlar[int(x[1]) - 49] = false;
				switch (int(x[1]) - 48)
				{
				case 1:
					tahta[6][5] = x;
					break;
				case 2:
					tahta[7][5] = x;
					break;
				case 3:
					tahta[8][5] = x;
					break;
				case 4:
					tahta[9][5] = x;
					break;
				default:
					break;
				}
				tahta[currentI][currentJ] = "  ";
				set = true;
			}
		}
	}
	if (!set) {
		for (int i = 0; i < newLocation.length(); i++) {
			if (newLocation[i] == ',') {
				parserLocation = i;
			}
		}
		for (int i = 0; i < parserLocation; i++) {
			tempStr += newLocation[i];
		}
		newI = stoi(tempStr);
		tempStr = "";
		for (int i = parserLocation + 1; i < newLocation.length(); i++) {
			tempStr += newLocation[i];
		}
		newJ = stoi(tempStr);
		string temps = tahta[newI][newJ];
		if (temps[0] == x[0]) {
			cout << "Kendi piyonunuzu yiyemezsiniz." << endl;
			try
			{
				ofstream myfile;
				myfile.open("sira.txt", std::ios_base::app);
				myfile << temps << " adlý piyon "<< x <<" ile ayný renk olduðu için hamle yapýlamadý." << endl;
				myfile.close();
			}
			catch (const std::exception&)
			{

			}
		}
		else {
			tahta[currentI][currentJ] = "  ";
			replacePiyonToKale(tahta[newI][newJ], tahta, x);
			tahta[newI][newJ] = x;
		}
	}
	else {
		try
		{
			ofstream myfile;
			myfile.open("sira.txt", std::ios_base::app);
			myfile << x << " adlý piyon set edildi." << endl;
			myfile.close();
		}
		catch (const std::exception&)
		{

		}
	}
}

void updateTable(string(&tahta)[11][11], int atilanZar, string color, string action,int minIndex) {
	if (action == "PIYON_SAL") {
		if (color == "Kirmizi") {
			switch (minIndex)
			{
			case 0:
				replacePiyonToKale(tahta[6][10], tahta, "K1");
				tahta[6][10] = "K1";
				tahta[10][9] = "  ";
				break;
			case 1:
				replacePiyonToKale(tahta[6][10], tahta, "K2");
				tahta[6][10] = "K2";
				tahta[10][10] = "  ";
				break;
			case 2:
				replacePiyonToKale(tahta[6][10], tahta, "K3");
				tahta[6][10] = "K3";
				tahta[9][9] = "  ";
				break;
			case 3:
				replacePiyonToKale(tahta[6][10], tahta, "K4");
				tahta[6][10] = "K4";
				tahta[9][10] = "  ";
				break;
			}
		}
		if (color == "Mavi") {
			switch (minIndex)
			{
			case 0:
				replacePiyonToKale(tahta[4][0], tahta, "M1");
				tahta[0][0] = "  ";
				tahta[4][0] = "M1";
				break;
			case 1:
				replacePiyonToKale(tahta[4][0], tahta, "M2");
				tahta[4][0] = "M2";
				tahta[0][1] = "  ";
				break;
			case 2:
				replacePiyonToKale(tahta[4][0], tahta, "M3");
				tahta[1][0] = "  ";
				tahta[4][0] = "M3";
				break;
			case 3:
				replacePiyonToKale(tahta[4][0], tahta, "M4");
				tahta[1][1] = "  ";
				tahta[4][0] = "M4";
				break;
			}
		}
		if (color == "Sari") {
			switch (minIndex)
			{
			case 0:
				replacePiyonToKale(tahta[0][6], tahta, "S1");
				tahta[0][9] = "  ";
				tahta[0][6] = "S1";
				break;
			case 1:
				replacePiyonToKale(tahta[0][6], tahta, "S2");
				tahta[0][10] = "  ";
				tahta[0][6] = "S2";
				break;
			case 2:
				replacePiyonToKale(tahta[0][6], tahta, "S3");
				tahta[1][9] = "  ";
				tahta[0][6] = "S3";
				break;
			case 3:
				replacePiyonToKale(tahta[0][6], tahta, "S4");
				tahta[1][10] = "  ";
				tahta[0][6] = "S4";
				break;
			}
		}
		if (color == "Yesil") {
			switch (minIndex)
			{
			case 0:
				replacePiyonToKale(tahta[10][4], tahta, "Y1");
				tahta[10][0] = "  ";
				tahta[10][4] = "Y1";
				break;
			case 1:
				replacePiyonToKale(tahta[10][4], tahta, "Y2");
				tahta[10][1] = "  ";
				tahta[10][4] = "Y2";
				break;
			case 2:
				replacePiyonToKale(tahta[10][4], tahta, "Y3");
				tahta[9][0] = "  ";
				tahta[10][4] = "Y3";
				break;
			case 3:
				replacePiyonToKale(tahta[10][4], tahta, "Y4");
				tahta[9][1] = "  ";
				tahta[10][4] = "Y4";
				break;
			}
		}
	}
	else if (action == "AKTIF_PIYON_ILERLET") {
		bool isFound = false;
		int tempI = 0, tempJ = 0;
		int anlikAktif = 0;
		int lastTrue=0;
		string tempX = "";
		if (color == "Kirmizi") {
			for (int i = 0; i < 4; i++) {
				if (kirmizi_aktif_piyonlar[i] && !kirmizi_setted_piyonlar[i]) {
					anlikAktif++;
					lastTrue = i+1;
				}
			}
			string x = "";
			if (anlikAktif > 1) {
				cout << "Hangi piyonu oynamak istersiniz?" << endl;
				for (int i = 0; i < 4; i++) {
					if (kirmizi_aktif_piyonlar[i] && !kirmizi_setted_piyonlar[i]) {
						cout << i + 1 << ". " << "K" + to_string(i + 1) << endl;
					}
				}
				int z; cin >> z;
				if (z != 1 && z != 2 && z != 3 && z != 4)
					return;
				x = "K" + to_string(z);
			}
			else {
				x = "K" + to_string(lastTrue);
			}
			for (int i = 0; i < 11; i++) {
				for (int j = 0; j < 11; j++) {
					if (tahta[i][j] == x) {
						tempI = i;
						tempJ = j;
						tempX = x;
						isFound = true;
					}
				}
			}
		}
		else if (color == "Mavi") {
			for (int i = 0; i < 4; i++) {
				if (mavi_aktif_piyonlar[i] && !mavi_setted_piyonlar[i]) {
					anlikAktif++;
					lastTrue = i+1;
				}
			}
			string x = "";
			if (anlikAktif > 1) {
				cout << "Hangi piyonu oynamak istersiniz?" << endl;
				for (int i = 0; i < 4; i++) {
					if (mavi_aktif_piyonlar[i] && !mavi_setted_piyonlar[i]) {
						cout << i + 1 << ". " << "M" + to_string(i + 1) << endl;
					}
				}
				int z; cin >> z;
				if (z != 1 && z != 2 && z != 3 && z != 4)
					return;
				x = "M" + to_string(z);
			}
			else {
				x = "M" + to_string(lastTrue);
			}
			for (int i = 0; i < 11; i++) {
				for (int j = 0; j < 11; j++) {
					if (tahta[i][j] == x) {
						tempI = i;
						tempJ = j;
						tempX = x;
						isFound = true;
					}
				}
			}
		}
		else if (color == "Sari") {
			for (int i = 0; i < 4; i++) {
				if (sari_aktif_piyonlar[i] && !sari_setted_piyonlar[i]) {
					anlikAktif++;
					lastTrue = i+1;
				}
			}
			string x = "";
			if (anlikAktif > 1) {
				cout << "Hangi piyonu oynamak istersiniz?" << endl;
				for (int i = 0; i < 4; i++) {
					if (sari_aktif_piyonlar[i] && !sari_setted_piyonlar[i]) {
						cout << i + 1 << ". " << "S" + to_string(i + 1) << endl;
					}
				}
				int z; cin >> z;
				if (z != 1 && z != 2 && z != 3 && z != 4)
					return;
				x = "S" + to_string(z);
			}
			else {
				x = "S" + to_string(lastTrue);
			}
			for (int i = 0; i < 11; i++) {
				for (int j = 0; j < 11; j++) {
					if (tahta[i][j] == x) {
						tempI = i;
						tempJ = j;
						tempX = x;
						isFound = true;
					}
				};
			}

		}
		else if (color == "Yesil") {
			for (int i = 0; i < 4; i++) {
				if (yesil_aktif_piyonlar[i] && !yesil_setted_piyonlar[i]) {
					anlikAktif++;
					lastTrue = i+1;
				}
			}
			string x = "";
			if (anlikAktif > 1) {
				cout << "Hangi piyonu oynamak istersiniz?" << endl;
				for (int i = 0; i < 4; i++) {
					if (yesil_aktif_piyonlar[i] && !yesil_setted_piyonlar[i]) {
						cout << i + 1 << ". " << "Y" + to_string(i+1) << endl;
					}
				}
				int z; cin >> z;
				if (z != 1 && z != 2 && z != 3 && z != 4)
					return;
				x = "Y" + to_string(z);
			}
			else {
				x = "Y" + to_string(lastTrue);
			}
			for (int i = 0; i < 11; i++) {
				for (int j = 0; j < 11; j++) {
					if (tahta[i][j] == x) {
						tempI = i;
						tempJ = j;
						tempX = x;
						isFound = true;
					}
				}
			}
		}
		edgeControl(tahta, atilanZar, tempX, tempI, tempJ);
		cout << "Atilan zar:" << atilanZar << endl;
	}
	printBoard(tahta);
}

void writeTableToFile(string action, string(&tahta)[11][11], string* players, string* playersWithColors, int tempSira, int playerCount, int atilanZar) {
	try
	{
		ofstream myfile;
		myfile.open("tahta.txt", std::ios_base::app);
		myfile << "En son oynayan oyuncu " << players[tempSira % playerCount] << " rengi " << playersWithColors[tempSira % playerCount] << " attigi zar " << (atilanZar) << endl;
		myfile << "Action: " << action << endl;
		myfile << " ____ ____ ____ ____ ____ ____ ____ ____ ____ ____ ____" << endl;
		for (int i = 0; i < 11; i++) {
			myfile << "|    |    |    |    |    |    |    |    |    |    |    |-\n";
			for (int j = 0; j < 11; j++) {
				if (j == 0) {
					myfile << "| ";
				}
				else {
					myfile << " | ";
				}
				myfile << tahta[i][j];
				if (j == 10) {
					myfile << " |" << endl;
				}
			}
			myfile << "|____|____|____|____|____|____|____|____|____|____|____|\n";
		}

		myfile.close();
	}
	catch (const std::exception&)
	{

	}
}

void writeLastTableToFile(string kazanan, string(&tahta)[11][11], string* players, int tempSira, int playerCount) {
	cout << kazanan << " Oyuncu: " << players[(tempSira - 1) % playerCount] << " kazandi! Tebrikler" << endl;
	try
	{
		ofstream myfile;
		myfile.open("tahta.txt", std::ios_base::app);
		myfile << "En son oynayan oyuncu " << players[(tempSira - 1) % playerCount] << " rengi " << kazanan << endl;
		myfile << "Action: " << "OYUN_BITTI" << endl;
		myfile << "Kazanan: " << kazanan << " Oyuncu " << players[(tempSira - 1) % playerCount] << endl;
		myfile << " ____ ____ ____ ____ ____ ____ ____ ____ ____ ____ ____" << endl;
		for (int i = 0; i < 11; i++) {
			myfile << "|    |    |    |    |    |    |    |    |    |    |    |-\n";
			for (int j = 0; j < 11; j++) {
				if (j == 0) {
					myfile << "| ";
				}
				else {
					myfile << " | ";
				}
				myfile << tahta[i][j];
				if (j == 10) {
					myfile << " |" << endl;
				}
			}
			myfile << "|____|____|____|____|____|____|____|____|____|____|____|\n";
		}

		myfile.close();
	}
	catch (const std::exception&)
	{

	}
}

int checkGameStatus(string(&tahta)[11][11]) {
	if (tahta[5][9] != "  " && tahta[5][8] != "  " && tahta[5][7] != "  " && tahta[5][6] != "  ") {
		return 0; //Kirmizi
	}
	if (tahta[5][1] != "  " && tahta[5][2] != "  " && tahta[5][3] != "  " && tahta[5][4] != "  ") {
		return 1; //Mavi
	}
	if (tahta[1][5] != "  " && tahta[2][5] != "  " && tahta[3][5] != "  " && tahta[4][5] != "  ") {
		return 2; //Sari
	}
	if (tahta[9][5] != "  " && tahta[8][5] != "  " && tahta[7][5] != "  " && tahta[6][5] != "  ") {
		return 3; //Yesil
	}
	return 4;
}

void setAktifPiyons(char x, string (&tahta)[11][11]) {
	switch (x)
	{
	case 'K':
		if (tahta[10][9] == "  ") {
			if(tahta[5][6] == "  ")
				kirmizi_aktif_piyonlar[0] = true;
			if (tahta[5][6] == "K1")
				kirmizi_setted_piyonlar[0] = true;
		}
		if (tahta[10][10] == "  ") {
			if(tahta[5][7] == "  ")
				kirmizi_aktif_piyonlar[1] = true;
			if (tahta[5][7] == "K2") 
				kirmizi_setted_piyonlar[1] = true;
			
		}
		if (tahta[9][9] == "  ") {
			if(tahta[5][8] == "  ")
				kirmizi_aktif_piyonlar[2] = true;
			if (tahta[5][8] == "K3") 
				kirmizi_setted_piyonlar[2] = true;
		}
		if (tahta[9][10] == "  " ) {
			if(tahta[5][9] == "  ")
				kirmizi_aktif_piyonlar[3] = true;
			if (tahta[5][9] == "K4") 
				kirmizi_setted_piyonlar[3] = true;
		}
		break;
	case 'M':
		if (tahta[0][0] == "  ") {
			if(tahta[5][4] == "  ")
				mavi_aktif_piyonlar[0] = true;
			if (tahta[5][4] == "M1")
				mavi_setted_piyonlar[0] = true;
		}
		if (tahta[0][1] == "  ") {
			if(tahta[5][3] == "  ")
				mavi_aktif_piyonlar[1] = true;
			if(tahta[5][3] == "M2")
				mavi_setted_piyonlar[1] = true;
		}
		if (tahta[1][0] == "  ") {
			if (tahta[5][2] == "  ")
				mavi_aktif_piyonlar[2] = true;
			if (tahta[5][2] == "M3")
				mavi_setted_piyonlar[2] = true;
		}
		if (tahta[1][1] == "  ") {
			if (tahta[5][1] == "  ")
				mavi_aktif_piyonlar[3] = true;
			if (tahta[5][1] == "M4")
				mavi_setted_piyonlar[3] = true;
		}
		break;
	case 'Y':
		if (tahta[10][0] == "  ") {
			if (tahta[6][5] == "  ")
				yesil_aktif_piyonlar[0] = true;
			if (tahta[6][5] == "Y1")
				yesil_setted_piyonlar[0] = true;
		}
		if (tahta[10][1] == "  ") {
			if (tahta[7][5] == "  ")
				yesil_aktif_piyonlar[1] = true;
			if (tahta[7][5] == "Y2")
				yesil_setted_piyonlar[1] = true;
		}
		if (tahta[9][0] == "  " ) {
			if (tahta[8][5] == "  ")
				yesil_aktif_piyonlar[2] = true;
			if (tahta[8][5] == "Y3")
				yesil_setted_piyonlar[2] = true;
		}
		if (tahta[9][1] == "  " ) {
			if (tahta[9][5] == "  ")
				yesil_aktif_piyonlar[3] = true;
			if (tahta[9][5] == "Y4")
				yesil_setted_piyonlar[3] = true;
		}
		break;
	case 'S':
		if (tahta[0][9] == "  ") {
			if (tahta[4][5] == "  ")
				sari_aktif_piyonlar[0] = true;
			if (tahta[4][5] == "S1")
				sari_setted_piyonlar[0] = true;
		}
		if (tahta[0][10] == "  " ) {
			if (tahta[3][5] == "  ")
				sari_aktif_piyonlar[1] = true;
			if (tahta[3][5] == "S2")
				sari_setted_piyonlar[1] = true;
		}
		if (tahta[1][9] == "  ") {
			if (tahta[2][5] == "  ")
				sari_aktif_piyonlar[2] = true;
			if (tahta[2][5] == "S3")
				sari_setted_piyonlar[2] = true;
		}
		if (tahta[1][10] == "  ") {
			if (tahta[1][5] == "  ")
				sari_aktif_piyonlar[3] = true;
			if (tahta[1][5] == "S4")
				sari_setted_piyonlar[3] = true;
		}
		break;
	}
}

int getMinDeaktifPiyon(string color) {
	switch (color[0])
	{
	case 'K':
		for (int i = 0; i < 4; i++) {
			if (!kirmizi_aktif_piyonlar[i] && !kirmizi_setted_piyonlar[i]) {
				return i;
			}
		}
		break;
	case 'M':
		for (int i = 0; i < 4; i++) {
			if (!mavi_aktif_piyonlar[i] && !mavi_setted_piyonlar[i]) {
				return i;
			}
		}
		break;
	case 'S':
		for (int i = 0; i < 4; i++) {
			if (!sari_aktif_piyonlar[i] && !sari_setted_piyonlar[i]) {
				return i;
			}
		}
		break;
	case 'Y':
		for (int i = 0; i < 4; i++) {
			if (!yesil_aktif_piyonlar[i] && !yesil_setted_piyonlar[i]) {
				return i;
			}
		}
		break;
	}
}

bool checkIfExist(string(&tahta)[11][11],string color) {
	bool result = true;
	string x = "";
	switch (color[0])
	{
	case 'K':
		x = tahta[6][10];
		if (x[0] == 'K') {
			result=false;
		}
		break;
	case 'M':
		x = tahta[4][0];
		if (x[0] == 'M') {
			result=false;
		}
		break;
	case 'S':
		x = tahta[0][6];
		if (x[0] == 'S') {
			result=false;
		}
		break;
	case 'Y':
		x = tahta[10][4];
		if (x[0] == 'Y') {
			result=false;
		}
		break;
	}
	return result;
}

void startGame(int playerCount, string* players, string* playersWithColors, int* orders, string(&tahta)[11][11], bool isLoadedGame) {
	int tempSira = 0;
	int atilanZar = 0;
	if (isLoadedGame)
		tempSira = loadGameTempSira;

	cout << "Sira " << players[tempSira % 4] << " isimli kullanicida, renk: " << playersWithColors[tempSira % 4] << endl;
	srand((unsigned int)time(NULL));
	while (1) {
		if (input()) {
			atilanZar = dice();

			system("cls");
			cout << "1306190028_EMIRHAN_KURT || Kizma Bilader || File Organization" << endl << endl; //Header

			printPlayers(playerCount, players, playersWithColors, orders);

			/* OYUNUN DURUMUNU KONTROL ET */
			int gameStatus = checkGameStatus(tahta);
			if (gameStatus != 4) {
				switch (gameStatus)
				{
				case 0:
					writeLastTableToFile("Kirmizi", tahta, players, tempSira, playerCount);
					break;
				case 1:
					writeLastTableToFile("Mavi", tahta, players, tempSira, playerCount);
					break;
				case 2:
					writeLastTableToFile("Sari", tahta, players, tempSira, playerCount);
					break;
				case 3:
					writeLastTableToFile("Yesil", tahta, players, tempSira, playerCount);
					break;
				}
				return;
			}
			/* OYUNUN DURUMUNU KONTROL ET */
			cout << "Oyuncu " << players[tempSira % playerCount] << " attigi zar " << (atilanZar) << endl;
			string color = playersWithColors[tempSira % playerCount];
			string action = "ZAR_ATTI";
			setAktifPiyons(color[0], tahta);
			int minIndex = getMinDeaktifPiyon(color);
			bool result = checkIfExist(tahta, color);

			if (!aktifPiyonVarMi(color) && minIndex < 4) {
				if (atilanZar == 6) {
					switch (color[0])
					{
					case 'K':
						replacePiyonToKale(tahta[6][10], tahta, color);
						action = "PIYON_SAL";
						kirmizi_aktif_piyonlar[minIndex] = true;
						break;
					case 'M':
						replacePiyonToKale(tahta[4][0], tahta, color);
						action = "PIYON_SAL";
						mavi_aktif_piyonlar[minIndex] = true;
						break;
					case 'Y':
						replacePiyonToKale(tahta[10][4], tahta, color);
						action = "PIYON_SAL";
						yesil_aktif_piyonlar[minIndex] = true;
						break;
					case 'S':
						replacePiyonToKale(tahta[0][6], tahta, color);
						action = "PIYON_SAL";
						sari_aktif_piyonlar[minIndex] = true;
						break;
					}
				}
			}
			else if (atilanZar == 6 && minIndex < 4) {
				if (result) {
					cout << "Yeni piyon oyuna sokmak ister misiniz?\n1. Evet\n2. Hayir" << endl;
					int x = 0; cin >> x;
					if (x == 1) {
						switch (color[0])
						{
						case 'K':
							replacePiyonToKale(tahta[6][10], tahta, color);
							action = "PIYON_SAL";
							kirmizi_aktif_piyonlar[minIndex] = true;
							break;
						case 'M':
							replacePiyonToKale(tahta[4][0], tahta, color);
							action = "PIYON_SAL";
							mavi_aktif_piyonlar[minIndex] = true;
							break;
						case 'Y':
							replacePiyonToKale(tahta[10][4], tahta, color);
							action = "PIYON_SAL";
							yesil_aktif_piyonlar[minIndex] = true;
							break;
						case 'S':
							replacePiyonToKale(tahta[0][6], tahta, color);
							action = "PIYON_SAL";
							sari_aktif_piyonlar[minIndex] = true;
							break;
						}
					}
					else {
						action = "AKTIF_PIYON_ILERLET";
					}
				}
				else {
					cout << "Kendi piyonunuzu yiyemezsiniz."<<endl;
					action = "AKTIF_PIYON_ILERLET";
				}
			}
			else{
				action = "AKTIF_PIYON_ILERLET";
			}
			writeActionsToFile(players[tempSira % playerCount], color, atilanZar, action, minIndex);
			updateTable(tahta, atilanZar, color, action, minIndex);
			writeTableToFile(action, tahta, players, playersWithColors, tempSira, playerCount, atilanZar);

			tempSira++;
			cout << "Sira " << players[tempSira % playerCount] << " isimli kullanicida, renk: " << playersWithColors[tempSira % playerCount] << endl;
		}
	}
}

void readPlayerCountFromFile(int& playerCount) {
	ifstream file("sira.txt");
	bool firstRun = true;
	try
	{
		if (file.is_open()) {
			string line;
			while (getline(file, line)) {
					int x = line.find(':');
					if (x != -1) {
						playerCount = int(line[x + 2]) - 48;
					}
					break;
			}
			file.close();
		}

	}
	catch (const std::exception&)
	{

	}
}

string readPlayersColor(string line) {
	int kirmiziIndex = line.find("Kirmizi");
	int maviIndex = line.find("Mavi");
	int yesilIndex = line.find("Yesil");
	int sariIndex = line.find("Sari");
	if (kirmiziIndex > 0) {
		return "Kirmizi";
	}
	else if (maviIndex > 0) {
		return "Mavi";
	}
	else if (yesilIndex > 0) {
		return "Yesil";
	}
	else if (sariIndex > 0) {
		return "Sari";
	}
}

void readPlayersFromFile(string* players, string* playersWithColors, int* orders) {
	ifstream file("sira.txt");
	try
	{
		if (file.is_open()) {
			string line;
			string color;
			int currentLine = 0;
			while (getline(file, line)) {
				if (currentLine == 6 || currentLine == 9 || currentLine == 12 || currentLine == 15) {
					line.erase(remove(line.begin(), line.end(), '|'), line.end());
					line.erase(remove(line.begin(), line.end(), '	'), line.end());
					line.erase(remove(line.begin(), line.end(), ' '), line.end());
					line.pop_back();
					color = readPlayersColor(line);
					int indexOfColor = 0;
					string name = "";
					if (currentLine == 6) {
						orders[0] = 1;
						switch (color[0])
						{
						case 'K':
							indexOfColor = line.find_first_of('K');
							playersWithColors[0] = "Kirmizi";
							break;
						case 'M':
							indexOfColor = line.find_first_of('M');
							playersWithColors[0] = "Mavi";
							break;
						case 'S':
							indexOfColor = line.find_first_of('S');
							playersWithColors[0] = "Sari";
							break;
						case 'Y':
							indexOfColor = line.find_first_of('Y');
							playersWithColors[0] = "Yesil";
							break;
						}
						name = line.substr(0, indexOfColor);
						players[0] = name;
					}
					else if (currentLine == 9) {
						orders[1] = 2;
						switch (color[0])
						{
						case 'K':
							indexOfColor = line.find_first_of('K');
							playersWithColors[1] = "Kirmizi";
							break;
						case 'M':
							indexOfColor = line.find_first_of('M');
							playersWithColors[1] = "Mavi";
							break;
						case 'S':
							indexOfColor = line.find_first_of('S');
							playersWithColors[1] = "Sari";
							break;
						case 'Y':
							indexOfColor = line.find_first_of('Y');
							playersWithColors[1] = "Yesil";
							break;
						}
						name = line.substr(0, indexOfColor);
						players[1] = name;
					}
					else if (currentLine == 12) {
						orders[2] = 3;
						switch (color[0])
						{
						case 'K':
							indexOfColor = line.find_first_of('K');
							playersWithColors[2] = "Kirmizi";
							break;
						case 'M':
							indexOfColor = line.find_first_of('M');
							playersWithColors[2] = "Mavi";
							break;
						case 'S':
							indexOfColor = line.find_first_of('S');
							playersWithColors[2] = "Sari";
							break;
						case 'Y':
							indexOfColor = line.find_first_of('Y');
							playersWithColors[2] = "Yesil";
							break;
						}
						name = line.substr(0, indexOfColor);
						players[2] = name;
					}
					else if (currentLine == 15) {
						orders[3] = 4;
						switch (color[0])
						{
						case 'K':
							indexOfColor = line.find_first_of('K');
							playersWithColors[3] = "Kirmizi";
							break;
						case 'M':
							indexOfColor = line.find_first_of('M');
							playersWithColors[3] = "Mavi";
							break;
						case 'S':
							indexOfColor = line.find_first_of('S');
							playersWithColors[3] = "Sari";
							break;
						case 'Y':
							indexOfColor = line.find_first_of('Y');
							playersWithColors[3] = "Yesil";
							break;
						}
						name = line.substr(0, indexOfColor);
						players[3] = name;
					}
				}
				currentLine++;
			}
			file.close();
		}

	}
	catch (const std::exception&)
	{

	}
}

int getLineCount() {
	ifstream file("tahta.txt");
	int lineCount = 0;
	try
	{
		if (file.is_open()) {
			string line;
			while (getline(file, line)) {
				lineCount++;
			}
			file.close();
		}

	}
	catch (const std::exception&)
	{

	}
	return lineCount;
}

void readBoardFromFile(string(&tahta)[11][11], string* players, int playerCount) {
	for (int i = 0; i < 11; i++) {
		for (int j = 0; j < 11; j++) {
			tahta[i][j] = "  ";
		}
	}
	ifstream file("tahta.txt");
	int totalLineCount = getLineCount();
	int lineCount = 0;
	int insideLineCount = 0;
	try
	{
		if (file.is_open()) {
			string line;
			string color;
			string tempText = "";
			int currentLine = 0;
			while (getline(file, line)) {
				if (totalLineCount - lineCount == 36) {
					int y = line.find(" rengi");
					string sub = line.substr(22, y - 22);
					for (int i = 0; i < playerCount; i++) {
						if (players[i] == sub) {
							loadGameTempSira = i + 1;
						}
					}
				}
				if (totalLineCount - lineCount < 33) {
					if (line != "|    |    |    |    |    |    |    |    |    |    |    |-" && line != "|____|____|____|____|____|____|____|____|____|____|____|") {
						tempText = "";
						for (int i = 0; i < line.length(); i++) {
							if (i == 2 || i == 3) {
								tempText += line[i];
								if (i == 3) {
									tahta[insideLineCount][0] = tempText;
									if (insideLineCount == 0 && tempText != "M1") {
										mavi_aktif_piyonlar[0] = true;
									}
									else if (insideLineCount == 1 && tempText != "M3") {
										mavi_aktif_piyonlar[1] = false;
										mavi_aktif_piyonlar[2] = true;
									}


									if (insideLineCount == 10 && tempText != "Y1") {
										yesil_aktif_piyonlar[0] = true;
									}
									else if (insideLineCount == 9 && tempText != "Y3") {
										yesil_aktif_piyonlar[1] = false;
										yesil_aktif_piyonlar[2] = true;
									}
									tempText = "";
								}
							}
							else if (i == 7 || i == 8) {
								tempText += line[i];
								if (i == 8) {
									tahta[insideLineCount][1] = tempText;
									if (insideLineCount == 0 && tempText != "M2") {
										mavi_aktif_piyonlar[0] = false;
										mavi_aktif_piyonlar[1] = true;
									}
									else if (insideLineCount == 1 && tempText != "M4") {
										mavi_aktif_piyonlar[2] = false;
										mavi_aktif_piyonlar[3] = true;
									}

									if (insideLineCount == 10 && tempText != "Y2") {
										yesil_aktif_piyonlar[0] = false;
										yesil_aktif_piyonlar[1] = true;
									}
									else if (insideLineCount == 9 && tempText != "Y4") {
										yesil_aktif_piyonlar[2] = false;
										yesil_aktif_piyonlar[3] = true;
									}
									tempText = "";
								}
							}
							else if (i == 12 || i == 13) {
								tempText += line[i];
								if (i == 13) {
									tahta[insideLineCount][2] = tempText;
									tempText = "";
								}
							}
							else if (i == 17 || i == 18) {
								tempText += line[i];
								if (i == 18) {
									tahta[insideLineCount][3] = tempText;
									tempText = "";
								}
							}
							else if (i == 22 || i == 23) {
								tempText += line[i];
								if (i == 23) {
									tahta[insideLineCount][4] = tempText;
									tempText = "";
								}
							}
							else if (i == 27 || i == 28) {
								tempText += line[i];
								if (i == 28) {
									tahta[insideLineCount][5] = tempText;
									tempText = "";
								}
							}
							else if (i == 32 || i == 33) {
								tempText += line[i];
								if (i == 33) {
									tahta[insideLineCount][6] = tempText;
									tempText = "";
								}
							}
							else if (i == 37 || i == 38) {
								tempText += line[i];
								if (i == 38) {
									tahta[insideLineCount][7] = tempText;
									tempText = "";
								}
							}
							else if (i == 42 || i == 43) {
								tempText += line[i];
								if (i == 43) {
									tahta[insideLineCount][8] = tempText;
									tempText = "";
								}
							}
							else if (i == 47 || i == 48) {
								tempText += line[i];
								if (i == 48) {
									tahta[insideLineCount][9] = tempText;
									if (insideLineCount == 0 && tempText != "S1") {
										sari_aktif_piyonlar[0] = true;
										if (tahta[4][5] == "S1") {
											sari_aktif_piyonlar[0] = false;
										}
									}
									else if (insideLineCount == 1 && tempText != "S3") {
										sari_aktif_piyonlar[1] = false;
										sari_aktif_piyonlar[2] = true;
									}

									if (insideLineCount == 10 && tempText != "K1") {
										kirmizi_aktif_piyonlar[0] = true;
									}
									else if (insideLineCount == 9 && tempText != "K3") {
										kirmizi_aktif_piyonlar[1] = false;
										kirmizi_aktif_piyonlar[2] = true;
									}
									tempText = "";
								}
							}
							else if (i == 52 || i == 53) {
								tempText += line[i];
								if (i == 53) {
									tahta[insideLineCount][10] = tempText;
									if (insideLineCount == 0 && tempText != "S2") {
										sari_aktif_piyonlar[0] = false;
										sari_aktif_piyonlar[1] = true;
										if (tahta[3][5] == "S2") {
											sari_aktif_piyonlar[1] = false;
										}
									}
									else if (insideLineCount == 1 && tempText != "S4") {
										sari_aktif_piyonlar[2] = false;
										sari_aktif_piyonlar[3] = true;
									}

									if (insideLineCount == 10 && tempText != "K2") {
										kirmizi_aktif_piyonlar[0] = false;
										kirmizi_aktif_piyonlar[1] = true;
									}
									else if (insideLineCount == 9 && tempText != "K4") {
										kirmizi_aktif_piyonlar[2] = false;
										kirmizi_aktif_piyonlar[3] = true;
									}
									tempText = "";
								}
							}
						}
						insideLineCount++;
					}
				}
				lineCount++;
			}
			file.close();
		}
	}
	catch (const std::exception&)
	{

	}
}

void clearFiles()
{
	try
	{
		ofstream file("tahta.txt");
		file << "";
		file.close();
		ofstream myFile("sira.txt");
		myFile << "";
		myFile.close();
	}
	catch (const std::exception&)
	{

	}
}

int main() {
	cout << "1306190028_EMIRHAN_KURT || Kizma Bilader || File Organization" << endl << endl << endl; //Header
	cout << "1. Yeni Oyun\n2. Oyuna Devam Et\n";
	string tahta[11][11];
	int x; cin >> x;
	if (x == 1) {
		clearFiles();
		cout << "Oyunu kac oyuncu ile oynamak istersiniz. \n1- 2 oyunculu\n2- 4 Oyunculu" << endl << endl;
		int playerSelection = 0, playerCount = 0;
		cin >> playerSelection;
		if (playerSelection == 1) {
			playerCount = 2;
		}
		else if (playerSelection == 2) {
			playerCount = 4;
		}
		else {
			cout << "Giris Algilanmadi.";
			return 0;
		}
		string* players = new string[playerCount];
		int* playersListWithIndex = new int[playerCount];
		string* playersWithColors = new string[playerCount];
		int* orders = new int[playerCount];
		cout << endl << playerCount << " oyuncu secimi yapildi." << endl;
		setPlayers(playerCount, players, playersListWithIndex);
		setColors(playerCount, players, playersWithColors);
		setOrder(playerCount, players, playersWithColors, orders, playersListWithIndex);
		writePlayersToFile(playerCount, players, playersWithColors, orders, playersListWithIndex);
		printPlayers(playerCount, players, playersWithColors, orders);
		setBoard(tahta);
		startGame(playerCount, players, playersWithColors, orders, tahta, false);
	}
	else if (x == 2) {
		int playerCount = 0;
		readPlayerCountFromFile(playerCount);
		string* players = new string[playerCount];
		string* playersWithColors = new string[playerCount];
		int* orders = new int[playerCount];
		readPlayersFromFile(players, playersWithColors, orders);
		cout << "Oyuncu Listesi Basariyla Alindi" << endl;
		printPlayers(playerCount, players, playersWithColors, orders);
		readBoardFromFile(tahta, players, playerCount);
		printBoard(tahta);
		startGame(playerCount, players, playersWithColors, orders, tahta, true);
	}
	else {
		cout << "Giris Algilanmadi.";
		return 0;
	}
	return 0;
}