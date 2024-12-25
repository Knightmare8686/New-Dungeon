#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <cmath>

using namespace std;

int dungeonSize;  // Größe des Dungeons
vector<vector<char>> dungeon;  // Dynamisches Dungeon
int playerX, playerY;  // Position des Spielers
int treasureX, treasureY;  // Position des Schatzes
vector<pair<int, int>> enemies;  // Positionen der Gegner
int score = 0;  // Punktestand
int level = 1;  // Level
time_t startTime;  // Startzeit
int countdown = 60;  // Timer in Sekunden

// Dungeon initialisieren
void generateDungeon() {
    dungeonSize = 5 + level;  // Dungeon wird pro Level größer
    dungeon.assign(dungeonSize, vector<char>(dungeonSize, '.'));  // Dungeon mit '.' füllen

    // Hindernisse hinzufügen
    for (int i = 0; i < dungeonSize; i++) {
        int randX = rand() % dungeonSize;
        int randY = rand() % dungeonSize;
        dungeon[randX][randY] = '#';  // Hindernis
    }

    // Spieler setzen
    playerX = rand() % dungeonSize;
    playerY = rand() % dungeonSize;
    dungeon[playerX][playerY] = 'P';

    // Schatz setzen
    do {
        treasureX = rand() % dungeonSize;
        treasureY = rand() % dungeonSize;
    } while (dungeon[treasureX][treasureY] != '.');  // Stelle sicher, dass der Platz frei ist
    dungeon[treasureX][treasureY] = 'T';

    // Gegner setzen
    enemies.clear();
    for (int i = 0; i < level + 1; i++) {  // Gegneranzahl basierend auf dem Level
        int enemyX, enemyY;
        do {
            enemyX = rand() % dungeonSize;
            enemyY = rand() % dungeonSize;
        } while (dungeon[enemyX][enemyY] != '.');  // Stelle sicher, dass der Platz frei ist
        enemies.push_back({enemyX, enemyY});
        dungeon[enemyX][enemyY] = 'E';
    }
}

// Dungeon anzeigen
void printDungeon() {
    for (int i = 0; i < dungeonSize; i++) {
        for (int j = 0; j < dungeonSize; j++) {
            cout << dungeon[i][j] << " ";
        }
        cout << endl;
    }
}

// Gegner bewegen mit KI (verfolgen den Spieler)
void moveEnemies() {
    for (auto& enemy : enemies) {
        dungeon[enemy.first][enemy.second] = '.';  // Lösche alte Position
        int dx = playerX - enemy.first;
        int dy = playerY - enemy.second;

        if (abs(dx) > abs(dy)) {
            // Bewegung in x-Richtung
            if (dx > 0 && enemy.first < dungeonSize - 1) enemy.first++;
            else if (dx < 0 && enemy.first > 0) enemy.first--;
        } else {
            // Bewegung in y-Richtung
            if (dy > 0 && enemy.second < dungeonSize - 1) enemy.second++;
            else if (dy < 0 && enemy.second > 0) enemy.second--;
        }

        // Spieler gefunden?
        if (enemy.first == playerX && enemy.second == playerY) {
            cout << "Ein Gegner hat dich gefangen! Game Over!\n";
            cout << "Dein Punktestand: " << score << endl;
            exit(0);
        }

        dungeon[enemy.first][enemy.second] = 'E';  // Neue Position
    }
}

// Spieler bewegen
void movePlayer(char move) {
    dungeon[playerX][playerY] = '.';
    if (move == 'w' && playerX > 0 && dungeon[playerX - 1][playerY] != '#') playerX--;
    else if (move == 's' && playerX < dungeonSize - 1 && dungeon[playerX + 1][playerY] != '#') playerX++;
    else if (move == 'a' && playerY > 0 && dungeon[playerX][playerY - 1] != '#') playerY--;
    else if (move == 'd' && playerY < dungeonSize - 1 && dungeon[playerX][playerY + 1] != '#') playerY++;
    else {
        cout << "Bewegung blockiert! Ungültige Aktion.\n";
        score -= 5;  // Punkteabzug für ungültige Bewegung
        return;
    }
    score += 10;  // Punkte für gültige Bewegung

    // Schatz gefunden?
    if (playerX == treasureX && playerY == treasureY) {
        cout << "Du hast den Schatz gefunden! Weiter zum nächsten Level!\n";
        score += 100;
        level++;
        generateDungeon();  // Nächstes Level erstellen
        return;
    }

    dungeon[playerX][playerY] = 'P';  // Spieler neu setzen
}

// Spiel starten
void startGame() {
    generateDungeon();
    startTime = time(0);  // Timer starten
    while (true) {
        // Timer prüfen
        time_t currentTime = time(0);
        if (difftime(currentTime, startTime) >= countdown) {
            cout << "Die Zeit ist abgelaufen! Game Over!\n";
            cout << "Dein Punktestand: " << score << endl;
            break;
        }
        cout << "Level: " << level << " | Punktestand: " << score << endl;
        cout << "Verbleibende Zeit: " << countdown - difftime(currentTime, startTime) << " Sekunden\n";
        printDungeon();
        cout << "Bewege dich (w = hoch, s = runter, a = links, d = rechts): ";
        char move;
        cin >> move;
        movePlayer(move);
        moveEnemies();
    }
}

int main() {
    srand(time(0));
    startGame();
    return 0;
}
