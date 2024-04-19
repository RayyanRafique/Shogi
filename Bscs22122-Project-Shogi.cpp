#include<iostream>
#include<time.h>
#include<Windows.h>
#include<conio.h>
#include <fstream>
#include <math.h>

using namespace std;

enum MyEnum { White, Black };
using namespace std;
# define DBLACK 0
# define BLUE 1
# define GREEN 2
# define CYAN 3
# define RED 4
# define MAGENTA 5
# define BROWN 6
# define LIGHTGREY 7
# define DWHITE 15
# define size 9


struct Pieces {
	int ri;
	int ci;
};

void getRowColbyLeftClick(int& rpos, int& cpos)
{
	HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);
	DWORD Events;
	INPUT_RECORD InputRecord;
	SetConsoleMode(hInput, ENABLE_PROCESSED_INPUT | ENABLE_MOUSE_INPUT | ENABLE_EXTENDED_FLAGS);
	do
	{
		ReadConsoleInput(hInput, &InputRecord, 1, &Events);
		if (InputRecord.Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED)
		{
			cpos = InputRecord.Event.MouseEvent.dwMousePosition.X;
			rpos = InputRecord.Event.MouseEvent.dwMousePosition.Y;
			break;
		}
	} while (true);
}
void gotoRowCol(int rpos, int cpos)
{
	COORD scrn;
	HANDLE hOuput = GetStdHandle(STD_OUTPUT_HANDLE);
	scrn.X = cpos;
	scrn.Y = rpos;
	SetConsoleCursorPosition(hOuput, scrn);
}
void SetClr(int clr)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), clr);
}

//start
void Init(ifstream& rdr, char**& B, int& Dim, string PNames[], int& Turn) {
	ifstream RRdr("Bscs22122-loadcapture.txt");
	ifstream Rdr("Bscs22122-Grid.txt");
	int choose;
	cout << "Choose Between\n1: New Game (1) \n2: LoadGame (2)\n";
	cin >> choose;
	if (choose == 1) {
		if (!Rdr) {
			cout << "File could not be found!";
			exit(1);
		}
		Rdr >> Dim;
		B = new char* [Dim];
		int T = 2;
		for (int i = 0; i < T; i++) {
			cout << "Player " << i + 1 << "'s name: ";
			cin >> PNames[i];
		}
		for (int k = 0; k < Dim; k++) {
			B[k] = new char[Dim];
		}
		for (int ri = 0; ri < Dim; ri++) {
			for (int ci = 0; ci < Dim; ci++) {
				Rdr >> B[ri][ci];
			}
		}
		Turn = White;
	}
	else if (choose == 2) {
		rdr >> Dim;
		rdr >> Turn;
		B = new char* [Dim];
		for (int k = 0; k < Dim; k++) {
			B[k] = new char[Dim];
		}
		for (int ri = 0; ri < Dim; ri++) {
			for (int ci = 0; ci < Dim; ci++) {
				rdr >> B[ri][ci];
			}
		}
		for (int i = 0; i < 2; i++) {
			cout << "Player " << i + 1 << "'s name: ";
			cin >> PNames[i];
		}
		
	}

}
void TurnMsg(string name) {
	gotoRowCol(2, 82);
	cout << name << "'s Turn ";
	cout << "\n";
}
void SelectSC(Pieces& Sc) {
	gotoRowCol(4, 55);
	cout << "\t\t\t\t\t\t";
	gotoRowCol(4, 55);
	cout << "Enter Dimensions of Row Piece(1-9): ";
	cin >> Sc.ri;
	Sc.ri--;
	gotoRowCol(5, 55);
	cout << "\t\t\t\t\t\t";
	gotoRowCol(5, 55);
	cout << "Enter Dimensions of Column Piece(1-9): ";
	cin >> Sc.ci;
	Sc.ci--;
}
void SelectDC(Pieces& Dc) {
	gotoRowCol(6, 55);
	cout << "\t\t\t\t\t\t";
	gotoRowCol(6, 55);
	cout << "Enter Dimensions of Row Destination(1-9): ";
	cin >> Dc.ri;
	Dc.ri--;
	gotoRowCol(7, 55);
	cout << "\t\t\t\t\t\t";
	gotoRowCol(7, 55);
	cout << "Enter Dimensions of Column Destination(1-9): ";
	cin >> Dc.ci;
	Dc.ci--;
}
Pieces Mouse(Pieces Sc) {
	getRowColbyLeftClick(Sc.ri, Sc.ci);
	Sc.ri = Sc.ri / 6;
	Sc.ci = Sc.ci / 6;
	return Sc;
}

//Basic Validation
bool isValidPiece(char** B, int Dim, int Turn, Pieces P) {
	if (P.ri < 0 || P.ri >= Dim || P.ci < 0 || P.ci >= Dim) {
		return false;
	}
	if (B[P.ri][P.ci] == '-') {
		return false;
	}
	if (Turn == White) {
		if (B[P.ri][P.ci] >= 'a' && B[P.ri][P.ci] <= 'z') {
			return false;
		}
	}
	else {
		if (B[P.ri][P.ci] >= 'A' && B[P.ri][P.ci] <= 'Z') {
			return false;
		}
	}
}
bool isValidMove(char** B, int Dim, int Turn, Pieces P) {
	if (P.ri < 0 || P.ri >= Dim || P.ci < 0 || P.ci >= Dim) {
		return false;
	}
	if (B[P.ri][P.ci] != '-') {
		if (Turn == White) {
			if (B[P.ri][P.ci] >= 'a' && B[P.ri][P.ci] <= 'z') {
				return true;
			}
			else {
				return false;
			}
		}
		else {
			if (B[P.ri][P.ci] >= 'A' && B[P.ri][P.ci] <= 'Z') {
				return true;
			}
			else {
				return false;
			}
		}
	}
}
void PlaceMove(char**& B, Pieces Sc, Pieces Dc) {
	B[Dc.ri][Dc.ci] = B[Sc.ri][Sc.ci];
	B[Sc.ri][Sc.ci] = '-';
}
void TurnChange(int& Turn) {
	Turn = (Turn + 1) % 2;
}

//TemporaryUpdateBoard
void TempBoard(char**& B, Pieces Sc, Pieces Dc) {
	char piece = B[Sc.ri][Sc.ci];
	B[Sc.ri][Sc.ci] = '-';
	B[Dc.ri][Dc.ci] = piece;
}
void UndoTempBoard(char**& B, Pieces Sc, Pieces Dc) {
	char piece = B[Dc.ri][Dc.ci];
	B[Dc.ri][Dc.ci] = '-';
	B[Sc.ri][Sc.ci] = piece;
}

//Printing 
void PrintBox(int sr, int sc, int cols, int rows, int colr) {
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			SetClr(colr);
			gotoRowCol(sr + i, sc + j);
			cout << char(-37);
		}
	}
}
void DrawBoard(char** B, int Dim, int rows, int cols) {
	for (int i = 0; i < Dim; i++) {
		for (int j = 0; j < Dim; j++) {
			if ((i + j) % 2 == 0) {
				PrintBox(i * rows, j * cols, rows, cols, (7));
			}
			else {
				PrintBox(i * rows, j * cols, rows, cols, 15);
			}
		}
	}
}
void PrintPieces(char** B, int Dim, int rows, int cols) {
	for (int i = 0; i < Dim; i++) {
		for (int j = 0; j < Dim; j++) {
			gotoRowCol(((j * rows) + (j * rows) + rows) / 2, ((i * cols) + (i * cols) + cols) / 2);
			if (B[j][i] != '-')
				cout << B[j][i];
		}
	}
}
void removePieceFromHand(char hand[2][20], int pieceNumber, int turn) {
	hand[turn][pieceNumber] = '-';
}

//Option
bool Option(char input) {
	do {
		cin >> input;
		input = tolower(input);
	} while (input != 'y' && input != 'n');
	if (input == 'y') {
		gotoRowCol(8, 70);
		cout << "                                      ";
		return true;
	}
	else {
		gotoRowCol(8, 70);
		cout << "                                      ";
		return false;
	}
}

//path clears
bool IsVerticalPathCLear(char** B, Pieces Sc, Pieces Dc) {
	if (Sc.ri < Dc.ri) {
		for (int r = Sc.ri + 1; r < Dc.ri; r++) {
			if (B[r][Sc.ci] != '-')
				return false;
		}
		return true;
	}
	else {
		for (int ri = Sc.ri - 1; ri > Dc.ri; ri--) {
			if (B[ri][Sc.ci] != '-') {
				return false;
			}
		}
	}
	return true;
}
bool IsHorizontalPathClear(char** B, Pieces Sc, Pieces Dc) {
	if (Sc.ci < Dc.ci) {
		for (int r = Sc.ci + 1; r < Dc.ci; r++) {
			if (B[Sc.ri][r] != '-') {
				return false;
			}
		}
		return true;
	}
	else {
		for (int ri = Sc.ci - 1; ri > Dc.ci; ri--) {
			if (B[Sc.ri][ri] != '-') {
				return false;
			}
		}
	}
	return true;
}
bool isDiagonalPathClearL2R(char** B, Pieces Sc, Pieces Dc) {
	int dr = abs(Sc.ri - Dc.ri);
	if (Sc.ri < Dc.ri) {
		for (int i = 1; i < dr; i++) {
			if (B[Sc.ri + i][Sc.ci + i] != '-') {
				return false;
			}
		}
		return true;
	}
	if (Sc.ri > Dc.ri) {
		for (int i = 1; i < dr; i++) {
			if (B[Sc.ri - i][Sc.ci - i] != '-') {
				return false;
			}
		}
		return true;
	}
}
bool isDiagonalPathClearR2L(char** B, Pieces Sc, Pieces Dc) {
	int dr = abs(Sc.ri - Dc.ri);
	if (Dc.ri < Sc.ri) {
		for (int i = 1; i < dr; i++) {
			if (B[Sc.ri - i][Sc.ci + i] != '-') {
				return false;
			}
		}
		return true;
	}
	if (Dc.ri > Sc.ri) {
		for (int i = 1; i < dr; i++) {
			if (B[Sc.ri + i][Sc.ci - i] != '-') {
				return false;
			}
		}
		return true;
	}
}

// moves
bool Verticalmove(Pieces Sc, Pieces Dc) {
	return Sc.ci == Dc.ci;
}
bool Horizontalmove(Pieces Sc, Pieces Dc) {
	return Sc.ri == Dc.ri;
}
bool Diagonalmove(Pieces Sc, Pieces Dc) {
	int deltarow = Dc.ri - Sc.ri;
	int deltacol = Dc.ci - Sc.ci;
	return (abs(deltarow) == abs(deltacol));
}

// legalities
bool RookLegal(char** B, Pieces Sc, Pieces Dc) {
	return ((Verticalmove(Sc, Dc) && IsVerticalPathCLear(B, Sc, Dc)) || Horizontalmove(Sc, Dc) && IsHorizontalPathClear(B, Sc, Dc));
}
bool BishopLegal(char** B, Pieces Sc, Pieces Dc) {
	bool a, b, c;
	a = Diagonalmove(Sc, Dc);
	b = isDiagonalPathClearR2L(B, Sc, Dc);
	c = isDiagonalPathClearL2R(B, Sc, Dc);
	return (a && (b || c));
}
bool KnightLegal(char** B, Pieces Sc, Pieces Dc, int Turn) {
	int deltac = abs(Dc.ci - Sc.ci);
	if (Turn == White) {
		return  (Dc.ri == Sc.ri + 2 && deltac == 1);
	}
	else {
		return (Dc.ri == Sc.ri - 2 && deltac == 1);
	}
}
bool KingLegal(char** B, Pieces Sc, Pieces Dc) {
	int deltar = abs(Dc.ri - Sc.ri);
	int deltac = abs(Dc.ci - Sc.ci);
	return (deltar < 2 && deltac < 2);
}
bool lanceLegality(char** B, Pieces Sc, Pieces Dc, int turn) {
	if (turn == White) {
		return Verticalmove(Sc, Dc) && IsVerticalPathCLear(B, Sc, Dc) && Sc.ri < Dc.ri;
	}
	else {
		return Verticalmove(Sc, Dc) && IsVerticalPathCLear(B, Sc, Dc) && Sc.ri > Dc.ri;
	}
}
bool pawnLegality(char** B, Pieces Sc, Pieces Dc, int turn) {
	if (turn == White) {
		return (Verticalmove(Sc, Dc) && Dc.ri == Sc.ri + 1);
	}
	else {
		return (Verticalmove(Sc, Dc) && Dc.ri == Sc.ri - 1);
	}
}
bool silverGenLegality(char** B, Pieces Sc, Pieces Dc, int turn) {
	int deltaR = abs(Sc.ri - Dc.ri);
	int deltaC = abs(Sc.ci - Dc.ci);
	if (turn == White) {
		return ((deltaR < 2 && deltaC < 2) && (Diagonalmove(Sc, Dc) || (Dc.ri == Sc.ri + 1)));
	}
	else {
		return ((deltaR < 2 && deltaC < 2) && (Diagonalmove(Sc, Dc) || (Dc.ri == Sc.ri - 1)));
	}
}
bool goldenGenLegality(char** B, Pieces Sc, Pieces Dc, int turn) {
	int deltaR = abs(Sc.ri - Dc.ri);
	int deltaC = abs(Sc.ci - Dc.ci);
	if (turn == White) {
		return ((deltaR < 2 && deltaC < 2) && (Horizontalmove(Sc, Dc) || Verticalmove(Sc, Dc) || (Dc.ri == Sc.ri + 1 && deltaC == 1)));
	}
	else {
		return ((deltaR < 2 && deltaC < 2) && (Horizontalmove(Sc, Dc) || Verticalmove(Sc, Dc) || (Dc.ri == Sc.ri - 1 && deltaC == 1)));
	}
}
bool promotedBishopLegality(char** B, Pieces Sc, Pieces Dc) {
	return (KingLegal(B, Sc, Dc) || BishopLegal(B, Sc, Dc));
}
bool promotedRookLegality(char** B, Pieces Sc, Pieces Dc) {
	return (KingLegal(B, Sc, Dc) || RookLegal(B, Sc, Dc));
}

// Path legal
bool isLegalMove(char** B, Pieces Sc, Pieces Dc, int Turn) {
	switch (B[Sc.ri][Sc.ci]) {
	case 'r':
		return RookLegal(B, Sc, Dc);
		break;
	case 'R':
		return RookLegal(B, Sc, Dc);
		break;
	case 'b':
		return BishopLegal(B, Sc, Dc);
		break;
	case 'B':
		return BishopLegal(B, Sc, Dc);
		break;
	case 'l':
		return lanceLegality(B, Sc, Dc, Turn);
		break;
	case 'L':
		return lanceLegality(B, Sc, Dc, Turn);
		break;
	case 'k':
		return KingLegal(B, Sc, Dc);
		break;
	case 'K':
		return KingLegal(B, Sc, Dc);
		break;
	case 'n':
		return KnightLegal(B, Sc, Dc,Turn);
		break;
	case 'N':
		return KnightLegal(B, Sc, Dc,Turn);
		break;
	case 's':
		return silverGenLegality(B, Sc, Dc, Turn);
		break;
	case 'S':
		return silverGenLegality(B, Sc, Dc, Turn);
		break;
	case 'g':
		return goldenGenLegality(B, Sc, Dc, Turn);
		break;
	case 'G':
		return goldenGenLegality(B, Sc, Dc, Turn);
		break;
	case 'p':
		return pawnLegality(B, Sc, Dc, Turn);
		break;
	case 'P':
		return pawnLegality(B, Sc, Dc, Turn);
		break;
	case 'D':
		return promotedRookLegality(B, Sc, Dc);
		break;
	case 'd':
		return promotedRookLegality(B, Sc, Dc);
		break;
	case 'H':
		return promotedBishopLegality(B, Sc, Dc);
		break;
	case 'h':
		return promotedBishopLegality(B, Sc, Dc);
		break;
	}

}

//Highlight
void Highlight(char** B, int sr, int sc, int turn, int BoxSize) {
	for (int i = 0, j = 0; i < BoxSize; i++) {
		SetClr(3);
		gotoRowCol(sr + i, sc + j);
		cout << char(-37);
	}
	for (int i = 0, j = BoxSize - 1; i < BoxSize; i++) {
		SetClr(3);
		gotoRowCol(sr + i, sc + j);
		cout << char(-37);
	}
	for (int i = 0; i < 1; i++) {
		for (int j = 0; j < BoxSize; j++) {
			SetClr(3);
			gotoRowCol(sr + i, sc + j);
			cout << char(-37);
		}
	}
	for (int i = BoxSize - 1; i > BoxSize - 2; i--) {
		for (int j = 0; j < BoxSize; j++) {
			SetClr(3);
			gotoRowCol(sr + i, sc + j);
			cout << char(-37);
		}
	}
	SetClr(15);
}
void UnHighlight(char** B, int sr, int sc, int turn, int BoxSize) {
	if (sr + sc % 2 == 0) {
		int clr = 15;
		for (int i = 0, j = 0; i < BoxSize; i++) {
			SetClr(clr);
			gotoRowCol(sr + i, sc + j);
			cout << char(-37);
		}
		for (int i = 0, j = BoxSize - 1; i < BoxSize; i++) {
			SetClr(clr);
			gotoRowCol(sr + i, sc + j);
			cout << char(-37);
		}
		for (int i = 0; i < 1; i++) {
			for (int j = 0; j < BoxSize; j++) {
				SetClr(clr);
				gotoRowCol(sr + i, sc + j);
				cout << char(-37);
			}
		}
		for (int i = BoxSize - 1; i > BoxSize - 2; i--) {
			for (int j = 0; j < BoxSize; j++) {
				SetClr(clr);
				gotoRowCol(sr + i, sc + j);
				cout << char(-37);
			}
		}
	}
	else {
		int clr = 7;
		for (int i = 0, j = 0; i < BoxSize; i++) {
			SetClr(clr);
			gotoRowCol(sr + i, sc + j);
			cout << char(-37);
		}
		for (int i = 0, j = BoxSize - 1; i < BoxSize; i++) {
			SetClr(clr);
			gotoRowCol(sr + i, sc + j);
			cout << char(-37);
		}
		for (int i = 0; i < 1; i++) {
			for (int j = 0; j < BoxSize; j++) {
				SetClr(clr);
				gotoRowCol(sr + i, sc + j);
				cout << char(-37);
			}
		}
		for (int i = BoxSize - 1; i > BoxSize - 2; i--) {
			for (int j = 0; j < BoxSize; j++) {
				SetClr(clr);
				gotoRowCol(sr + i, sc + j);
				cout << char(-37);
			}
		}
	}
	SetClr(15);

}
bool** HighlightMap(char** B, Pieces Sc, int turn, int Dim) {
	bool** HighlightedMap = new bool* [Dim];
	for (int i = 0; i < Dim; i++) {
		HighlightedMap[i] = new bool[Dim] {};
	}
	for (int i = 0; i < Dim; i++) {
		for (int j = 0; j < Dim; j++) {
			Pieces Dc;
			Dc.ri = i; Dc.ci = j;
			if (isValidMove(B, Dim, turn, Dc) && isLegalMove(B, Sc, Dc, turn)) {
				HighlightedMap[i][j] = true;
			}
		}
	}
	return HighlightedMap;
}
void truehighlight(char** B, bool** map, int Turn, int Dim) {
	for (int i = 0; i < Dim; i++) {
		for (int j = 0; j < Dim; j++) {
			if (map[i][j] == true) {
				Highlight(B, i * 6, j * 6, Turn, 6);
			}
		}
	}
}
void trueunhighlight(char** B, bool** map, int Turn, int Dim) {
	for (int i = 0; i < Dim; i++) {
		for (int j = 0; j < Dim; j++) {
			if (map[i][j]) {
				UnHighlight(B, i * 6, j * 6, Turn, 6);
			}
		}
	}
}

//Promotion
bool isPiecePromotable(char pieces[12], char piece) {
	for (int i = 0; i < 12; i++) {
		if (pieces[i] == piece) {
			return true;
		}
	}
	return false;
}
void PieceReplacement(char** B, Pieces Sc, char piece) {
	piece = B[Sc.ri][Sc.ci];
	switch (piece) {
	case 'p':
		B[Sc.ri][Sc.ci] = 'g';
		break;
	case 'n':
		B[Sc.ri][Sc.ci] = 'g';
		break;
	case 'l':
		B[Sc.ri][Sc.ci] = 'g';
		break;
	case 's':
		B[Sc.ri][Sc.ci] = 'g';
		break;
	case 'b':
		B[Sc.ri][Sc.ci] = 'h';
		break;
	case 'r':
		B[Sc.ri][Sc.ci] = 'd';
		break;
	case 'P':
		B[Sc.ri][Sc.ci] = 'G';
		break;
	case 'N':
		B[Sc.ri][Sc.ci] = 'G';
		break;
	case 'L':
		B[Sc.ri][Sc.ci] = 'G';
		break;
	case 'S':
		B[Sc.ri][Sc.ci] = 'G';
		break;
	case 'B':
		B[Sc.ri][Sc.ci] = 'H';
		break;
	case 'R':
		B[Sc.ri][Sc.ci] = 'D';
		break;
	}
}
bool AskPromotion(char** B, Pieces Sc, char input, char piece) {

	piece = B[Sc.ri][Sc.ci];
	switch (piece) {
	case 'p':
		gotoRowCol(8, 70);
		cout << "Promote " << piece << " to " << "Golden General? (Y/N): ";
		return Option(input);
		break;
	case 'n':
		gotoRowCol(8, 70);
		cout << "Promote " << piece << " to " << "Golden General? (Y/N): ";
		return Option(input);
		break;
	case 'l':
		gotoRowCol(8, 70);
		cout << "Promote " << piece << " to " << "Golden General? (Y/N): ";
		return Option(input);
		break;
	case 's':
		gotoRowCol(8, 70);
		cout << "Promote " << piece << " to " << "Golden General? (Y/N): ";
		return Option(input);
		break;
	case 'b':
		gotoRowCol(8, 70);
		cout << "Promote " << piece << " to " << "Horse? (Y/N): ";
		return Option(input);
		break;
	case 'r':
		gotoRowCol(8, 70);
		cout << "Promote " << piece << " to " << "Dragon? (Y/N): ";
		return Option(input);
		break;
	case 'P':
		gotoRowCol(8, 70);
		cout << "Promote " << piece << " to " << "Golden General? (Y/N): ";
		return Option(input);
		break;
	case 'N':
		gotoRowCol(8, 70);
		cout << "Promote " << piece << " to " << "Golden General? (Y/N): ";
		return Option(input);
		break;
	case 'L':
		gotoRowCol(8, 70);
		cout << "Promote " << piece << " to " << "Golden General? (Y/N): ";
		return Option(input);
		break;
	case 'S':
		gotoRowCol(8, 70);
		cout << "Promote " << piece << " to " << "Golden General? (Y/N): ";
		return Option(input);
		break;
	case 'B':
		gotoRowCol(8, 70);
		cout << "Promote " << piece << " to " << "Horse? (Y/N): ";
		return Option(input);
		break;
	case 'R':
		gotoRowCol(8, 70);
		cout << "Promote " << piece << " to " << "Dragon? (Y/N): ";
		return Option(input);
		break;
	}
	return false;
}
void promotionCheck(char** B, Pieces Sc, int Turn, char pieces[12], char input) {
	char piece = B[Sc.ri][Sc.ci];
	if (Turn == White) {
		if (Sc.ri > 5) {
			if (isPiecePromotable(pieces, piece)) {
				if (AskPromotion(B, Sc, input, piece)) {
					PieceReplacement(B, Sc, piece);
				}
			}
		}
	}
	else {
		if (Sc.ri < 3) {
			if (isPiecePromotable(pieces, piece)) {
				if (AskPromotion(B, Sc, input, piece)) {
					PieceReplacement(B, Sc, piece);
				}
			}
		}
	}
}

//check and selfcheck
Pieces findKing(char** B, int Turn, int Dim, Pieces Dc) {
	if (Turn == White) {
		for (int i = 0; i < Dim; i++) {
			for (int j = 0; j < Dim; j++) {
				if (B[i][j] == 'K') {
					Dc.ri = i;
					Dc.ci = j;
					return Dc;
				}
			}
		}
	}
	else {
		for (int i = 0; i < Dim; i++) {
			for (int j = 0; j < Dim; j++) {
				if (B[i][j] == 'k') {
					Dc.ri = i;
					Dc.ci = j;
					return Dc;
				}
			}
		}
	}
	return Dc;
}
bool check(char** B, int Turn, int Dim) {
	TurnChange(Turn);
	Pieces dc{ 0,0 };
	dc = findKing(B, Turn, Dim, dc);
	TurnChange(Turn);
	for (int i = 0; i < Dim; i++) {
		for (int j = 0; j < Dim; j++) {
			Pieces sc{ i, j };
			if (isValidPiece(B, Dim, Turn, sc) && isLegalMove(B, sc, dc, Turn)) {
				return true;
			}
		}
	}
	return false;
}
bool selfCheck(char** B, int Turn, int Dim) {
	TurnChange(Turn);
	bool doesEnemyCheck = check(B, Turn, Dim);
	return doesEnemyCheck;
}

//Load/Save
void saveBoard(ofstream& writer, int &Turn, char** B, int Dim) {
	writer << Dim;
	writer << endl;
	writer << Turn;
	writer << endl;
	for (int i = 0; i < Dim; i++) {
		for (int j = 0; j < Dim; j++) {
			writer << B[i][j];
		}
		writer << endl;
	}
}
void saveCapture(ofstream& writer, char hand[2][20], int CapCounter[]) {
	writer << CapCounter[Black] << " " << CapCounter[White];
	writer << endl;
	for (int player = 0; player < 2; player++) {
		for (int pieceNo = 0; pieceNo < 19; pieceNo++) {
			writer << hand[player][pieceNo] << " ";
		}
		writer << endl;
	}
}
void LoadCapture(ifstream& RRdr, char hand[2][20], int CapCounter[]) {
	RRdr >> CapCounter[Black] >> CapCounter[White];
	for (int player = 0; player < 2; player++) {
		for (int pieceNo = 0; pieceNo < 19; pieceNo++) {
			RRdr >> hand[player][pieceNo];
		}
	}
}

//tempdrop
void tempDrop(char**& B, Pieces Dc, char piece)
{
	B[Dc.ri][Dc.ci] = piece;
}
void realDrop(char**& B, Pieces dc, char piece)
{
	B[dc.ri][dc.ci] = piece;
}
void undoTempDrop(char**& B, Pieces dc)
{
	B[dc.ri][dc.ci] = '-';
}

//capture
char CapturePiece(char** B, char capturewhite[], char captureblack[], Pieces Dc, int& capw, int& capb) {
	switch (B[Dc.ri][Dc.ci]) {
	case 'p':
		capturewhite[capw] = 'P';
		capw++;
		break;
	case 'g':
		capturewhite[capw] = 'G';
		capw++;
		break;
	case 'h':
		capturewhite[capw] = 'B';
		capw++;
		break;
	case 'n':
		capturewhite[capw] = 'N';
		capw++;
		break;
	case 'l':
		capturewhite[capw] = 'L';
		capw++;
		break;
	case 's':
		capturewhite[capw] = 'S';
		capw++;
		break;
	case 'r':
		capturewhite[capw] = 'R';
		capw++;
		break;
	case 'b':
		capturewhite[capw] = 'B';
		capw++;
		break;
	case 'd':
		capturewhite[capw] = 'R';
		capw++;
		break;
	case 'P':
		captureblack[capb] = 'p';
		capb++;
		break;
	case 'G':
		captureblack[capb] = 'g';
		capb++;
		break;
	case 'H':
		captureblack[capb] = 'b';
		capb++;
		break;
	case 'N':
		captureblack[capb] = 'n';
		capb++;
		break;
	case 'L':
		captureblack[capb] = 'l';
		capb++;
		break;
	case 'S':
		captureblack[capb] = 's';
		capb++;
		break;
	case 'R':
		captureblack[capb] = 'r';
		capb++;
		break;
	case 'B':
		captureblack[capb] = 'b';
		capb++;
		break;
	case 'D':
		captureblack[capb] = 'r';
		capb++;
		break;
	}
	return '+';
}
void potato(char input, char** B, char capturewhite[], char captureblack[], Pieces Dc, int& capw, int& capb) {
	gotoRowCol(60, 10);
	cout << "Capture the enemy's piece? (Y/N): ";
	if (Option(input)) {
		CapturePiece(B, capturewhite, captureblack, Dc, capw, capb);
	}
}
void printCapturewhite(char capturewhite[], char captureblack[], string names[], int capw, int capb) {
	gotoRowCol(20, 70);
	cout << names[White] << "'s hand: ";
	for (int i = 0; i < capw; i++) {
		cout << capturewhite[i] << " ";
	}
}
void printCaptureblack(char capturewhite[], char captureblack[], string names[], int capw, int capb) {
	gotoRowCol(30, 70);
	cout << names[Black] << "'s hand: ";
	for (int i = 0; i < capb; i++) {
		cout << captureblack[i] << " ";
	}
}

//drop
char Dropwhite(char capturewhite[], char captureblack[], string names[], int capw, int capb) {
	int pieceno;
	gotoRowCol(13, 70);
	cout << "Enter Piece Number: ";
	gotoRowCol(13, 100);
	cin >> pieceno;
	pieceno--;
	if (capturewhite[pieceno] != '-') {
		char d;
		d = capturewhite[pieceno];
		capturewhite[pieceno] = '-';
		return d;
	}
	return '+';

}
char Dropblack(char capturewhite[], char captureblack[], string names[], int capw, int capb) {
	int pieceno;
	gotoRowCol(13, 70);
	cout << "Enter Piece Number: ";
	gotoRowCol(13, 100);
	cin >> pieceno;
	pieceno--;
	if (captureblack[pieceno] != '-') {
		char d;
		d = captureblack[pieceno];
		captureblack[pieceno] = '-';
		return d;
	}
	return '+';

}
bool ThisPieceHasALegalMove(char** B, char piece, Pieces Sc, int Turn, int Dim) {
	for (int i = 0; i < Dim; i++) {
		for (int j = 0; j < Dim; j++) {
			Pieces dc{ i, j };
			if (isValidPiece(B, Dim, Turn, Sc) && isLegalMove(B, Sc, dc, Turn)) {
				return true;
			}
		}
	}
	return false;
}
bool isDropValid(char** B, Pieces Dc, int Turn, char piece, int Dim) {
	//if out of bounds
	if (!isValidMove(B, Dim, Turn, Dc)) {
		return false;
	}
	//if not empty
	if (B[Dc.ri][Dc.ci] != '-') {
		return false;
	}
	//dropped pawn can not have another friendly pawn in the same column
	if (Turn == Black) {
		if (piece == 'p') {
			for (int i = 0; i < Dim; i++) {
				if (B[Dc.ri][i] != piece) {
					return true;
				}
				else {
					gotoRowCol(75, 10);
					cout << "This pawn can not be placed in this column because another friendly pawn already exists.";
					return false;
				}
			}
		}
		else if (piece == 'n') {
			if (Dc.ri < 3) {
				gotoRowCol(75, 10);
				cout << "The knight cannot be placed at the given position";
				return false;
			}
		}
	}
	else {
		if (piece == 'P') {
			for (int i = 0; i < Dim; i++) {
				if (B[i][Dc.ci] != piece) {
					return true;
				}
				else {
					gotoRowCol(75, 10);
					cout << "This pawn can not be placed in this column because another friendly pawn already exists.";
					return false;
				}
			}
		}
		else if (piece == 'N') {
			if (Dc.ri > 6) {
				gotoRowCol(75, 10);
				cout << "The knight cannot be placed at the given position";
				return false;
			}
		}
	}
	//dropped piece has to have at least one legal move
	tempDrop(B, Dc, piece);
	if (!ThisPieceHasALegalMove(B, piece, Dc, Turn, Dim)) {
		undoTempDrop(B, Dc);
		gotoRowCol(75, 10);
		cout << "This piece does not have a valid legal move at that position.";
		return false;
	}
	undoTempDrop(B, Dc);

	return true;
}


//checkmypiece
bool isWhitePiece(char**B,int Turn,Pieces Sc,Pieces Dc) {
	switch (B[Sc.ri][Sc.ci]) {
	case 'P':
		return true;
		break;
	case 'G':
		return true;
		break;
	case 'H':
		return true;
		break;
	case 'N':
		return true;
		break;
	case 'L':
		return true;
		break;
	case 'S':
		return true;
		break;
	case 'R':
		return true;
		break;
	case 'B':
		return true;
		break;
	case 'D':
		return true;
		break;
	}
}
bool isBlackPiece(char** B, int Turn, Pieces Sc, Pieces Dc) {
	switch (B[Sc.ri][Sc.ci]) {
	case 'p':
		return true;
		break;
	case 'g':
		return true;
		break;
	case 'h':
		return true;
		break;
	case 'n':
		return true;
		break;
	case 'l':
		return true;
		break;
	case 's':
		return true;
		break;
	case 'r':
		return true;
		break;
	case 'b':
		return true;
		break;
	case 'd':
		return true;
		break;
	}
}
bool checkMyPiece(char **B,int Turn, Pieces Sc, Pieces Dc) {
	if (Turn == White) {
		return isWhitePiece(B, Turn, Sc, Dc);
	}
	else {
		return isBlackPiece(B, Turn, Sc, Dc);
	}
}


//checkmate
bool checkmate(char** B, Pieces Dc, int Dim, int Turn, char capturewhite[], char captureblack[], int capw, int capb) {
	if (check(B, Turn, Dim) == false) {
		return false;
	}
	Pieces MP, PM, prev;
	TurnChange(Turn);
	for (int i = 0; i < Dim; i++) {
		for (int j = 0; j < Dim; j++) {
			prev.ri = i, prev.ci = j;
			MP.ri = i, MP.ci = j;
			if (checkMyPiece(B, Turn,MP,prev)) {
				for (int k = 0; k < Dim ; k++) {
					for (int l = 0; l < Dim; l++) {
						PM.ri = k, PM.ci = l;
						if (isLegalMove(B, MP, PM,Turn)) {
							TempBoard(B, prev, PM);
							if (selfCheck(B, Turn, Dim) == false) {
								return false;
							}
							prev.ri = k, prev.ci = l;
						}
					}
				}
				UndoTempBoard(B, prev, PM);
			}
		}
	}
	return true;


}




// Main
int main()
{

	ifstream rdr("Bscs22122-Loadboard.txt");
	Pieces Sc{}, Dc{};
	char** B = 0;
	char input = 't';
	int F = 6;
	char** C = 0;
	/*Replay array[1000] = {};*/
	int counting = 0;
	int Dim = 0;
	int capw = 0;
	int capb = 0;
	int Turn = 0;
	char pieces[12] = { 'b', 'B', 'p',  'P',  'n',  'N',  's',  'S',  'l',  'L',  'r', 'R' };
	string PNames[2] = {};
	char capture[2][20] = { '-' };
	int capturecounter[2] = { 0, 0 };
	char CaptureWhite[20] = {};
	char CaptureBlack[20] = {};
	Init(rdr, B, Dim, PNames, Turn);
	DrawBoard(B, Dim, F, F);
	PrintPieces(B, Dim, F, F);
	while (checkmate(B,Dc,Dim,Turn,CaptureWhite,CaptureBlack,capw,capb) == false)
	{
		if (checkmate(B, Dc, Dim, Turn, CaptureWhite, CaptureBlack, capw, capb) == true) {
			cout << "Checkmate!";
			exit(1);
		}
		gotoRowCol(10, 70);
		TurnMsg(PNames[Turn]);
		bool drop = false;
		//___Drop a Piece
		if ((Turn == White && capw != 0) || (Turn == Black && capb != 0)) {
			gotoRowCol(70, 10);
			cout << "Drop piece from hand instead of moving? (Y/N): ";
			if (Option(input)) {
				drop = true;
				char piece;
				do {
					if (Turn == White) {
						piece = Dropwhite(CaptureWhite, CaptureBlack, PNames, capw, capb);
					}
					else {
						piece = Dropblack(CaptureWhite, CaptureBlack, PNames, capw, capb);
					}
					do {
						gotoRowCol(8, 70);
						cout << "Click where you want to place it on Board";
						Dc = Mouse(Dc);
					} while (!isDropValid(B, Dc, Turn, piece, Dim));
					tempDrop(B, Dc, piece);
					if (selfCheck(B, Turn, Dim)) {
						undoTempDrop(B, Dc);
						gotoRowCol(10, 70);
						cout << "That move places you in a check, try again!";
					}
				} while (selfCheck(B, Turn, Dim));
				gotoRowCol(8, 70);
				cout << "                                                       ";
				undoTempDrop(B, Dc);
				realDrop(B, Dc, piece);
				DrawBoard(B, Dim, F, F);
				PrintPieces(B, Dim, F, F);
			}
		}

		if (!drop) {
			bool** map;
			do {
				gotoRowCol(10, 70);
				cout << "                                                 ";
				do {
					do
					{
						gotoRowCol(6, 70);
						cout << "Click on the piece you want to move: ";
						Sc = Mouse(Sc);
					} while (!isValidPiece(B, Dim, Turn, Sc));
					map = HighlightMap(B, Sc, Turn, Dim);
					truehighlight(B, map, Turn, Dim);
					do
					{
						gotoRowCol(6, 70);
						cout << "Click Destination you want to place: ";
						Dc = Mouse(Dc);
					} while (!isValidMove(B, Dim, Turn, Dc));
					trueunhighlight(B, map, Turn, Dim);
				} while (!isLegalMove(B, Sc, Dc, Turn));
				if (B[Dc.ri][Dc.ci] != '-') {
					potato(input, B, CaptureWhite, CaptureBlack, Dc, capw, capb);
				}
				gotoRowCol(60, 10);
				cout << "                                   ";
				TempBoard(B, Sc, Dc);
				if (selfCheck(B, Turn, Dim) == true) {
					UndoTempBoard(B, Sc, Dc);
					DrawBoard(B, Dim, F, F);
					PrintPieces(B, Dim, F, F);
					gotoRowCol(10, 70);
					cout << "That move places you in a check, try again!";
				}
			} while (selfCheck(B, Turn, Dim));
			UndoTempBoard(B, Sc, Dc);
			PlaceMove(B, Sc, Dc);
			printCapturewhite(CaptureWhite, CaptureBlack, PNames, capw, capb);
			printCaptureblack(CaptureWhite, CaptureBlack, PNames, capw, capb);
			promotionCheck(B, Dc, Turn, pieces, input);
			DrawBoard(B, Dim, F, F);
			PrintPieces(B, Dim, F, F);
		}
		//___Writers
		ofstream boardWriter("Bscs22122-Loadboard.txt");
		ofstream CaptureWriter("Bscs22122-loadcapture.txt");
		saveBoard(boardWriter, Turn, B, Dim);
		saveCapture(CaptureWriter, capture, capturecounter);


		TurnChange(Turn);
	}
	return _getch();
}