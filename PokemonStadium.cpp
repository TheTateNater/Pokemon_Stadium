#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <vector>
#include <sstream>

using namespace std;

enum MoveType {
    PHYSICAL,
    SPECIAL
};
struct Move {
    string name;
    int power;
    MoveType type;
};
struct Pokemon {
    string name;
    int hp;
    int attack;
    int defense;
    int spAttack;
    int spDefense;
    int speed;
    Move moves[4];

    bool operator==(const Pokemon& other) const {
    return name == other.name;  // Here we are just comparing names, but you could expand this if needed
}
};
vector<Pokemon> readPokemonFromFile(const string &filename);
vector<Pokemon> allPokemon = readPokemonFromFile("Poke_data.csv");
int displayMenu();
Pokemon selectPokemon(string chooser = "player");
void battle(Pokemon& playerPokemon, Pokemon& opponentPokemon);
void playerAttack(Pokemon& player, Pokemon& opponent);
void opponentAttack(Pokemon& player, Pokemon& opponent);
void executeMove(Pokemon& attacker, Pokemon& defender, Move move);
void resetHP(Pokemon& poke);

int main(){
    srand(time(NULL));
    bool keepPlaying = true;
    while (keepPlaying) {
        int menuChoice = displayMenu();
        switch (menuChoice) {
            case 1: // Battle
             {   // Player selects a Pokémon
                Pokemon playerPokemon = selectPokemon("player");
                // Opponent selects a Pokémon
                Pokemon opponentPokemon = selectPokemon("opponent");

                // Battle start
                battle(playerPokemon, opponentPokemon);
                break;
             }
            case 2: // Quit
            {   keepPlaying = false;
                break;
            }
            default:
                cout << "Invalid choice. Please try again."  << endl;
        }
    }
    cout << "Thanks for playing!" << endl;
    return 0;   
}
int displayMenu() {
    cout << "Welcome to Pokémon Stadium!" << endl;
    cout << "Would you like to: " << endl;
    cout << "1. Battle?" << endl;
    cout << "2. Quit?" << endl;
int choice;
cin >> choice;
cin.clear();          // Clear any errors
cin.ignore(INT_MAX, '\n');  // Discard unwanted characters from the stream
return choice;
}
Pokemon selectPokemon(string chooser) {
    int choice;
    vector<Pokemon> randomPokemonChoices;
    
    // Randomly select 3 distinct Pokémon
    while (randomPokemonChoices.size() < 3) {
        int randomIndex = rand() % allPokemon.size();
        // Check if this Pokémon is already in our choices
        if (find(randomPokemonChoices.begin(), randomPokemonChoices.end(), allPokemon[randomIndex]) == randomPokemonChoices.end()) {
            randomPokemonChoices.push_back(allPokemon[randomIndex]);
        }
    }
    
    if (chooser == "player") {
        do {
            cout << "Choose your Pokémon!" << endl;
            for (int i = 0; i < 3; i++) {
                cout << i + 1 << ". " << randomPokemonChoices[i].name << endl;
            }
            cin >> choice;

            if (choice < 1 || choice > 3) {
                cout << "Invalid choice. Please submit a valid number." << endl;
            }

        } while (choice < 1 || choice > 3);
    } else {
        choice = (rand() % 3) + 1;  // Randomly select a Pokémon from the three options
    }

    return randomPokemonChoices[choice - 1]; // Return the chosen Pokémon from the random choices
}
void battle(Pokemon& playerPokemon, Pokemon& opponentPokemon) {
    bool playerGoesFirst = (playerPokemon.speed > opponentPokemon.speed) ? true :
                           (playerPokemon.speed < opponentPokemon.speed) ? false :
                           (rand() % 2 == 0);

    while (playerPokemon.hp > 0 && opponentPokemon.hp > 0) {
    if (playerGoesFirst) {
        playerAttack(playerPokemon, opponentPokemon);
        if (opponentPokemon.hp <= 0) break;  // Check after player's attack
        
        opponentAttack(playerPokemon, opponentPokemon);
        if (playerPokemon.hp <= 0) break;  // Check after opponent's attack
    } else {
        opponentAttack(playerPokemon, opponentPokemon);
        if (playerPokemon.hp <= 0) break;  // Check after opponent's attack
        
        playerAttack(playerPokemon, opponentPokemon);
        if (opponentPokemon.hp <= 0) break;  // Check after player's attack
    }
}
if (playerPokemon.hp <= 0) {
    cout << playerPokemon.name << " has fainted!" << endl;
    cout << opponentPokemon.name << " has won the battle!" << endl;
} else {
    cout << opponentPokemon.name << " has fainted!" << endl;
    cout << playerPokemon.name << " has won the battle!" << endl;
}
    resetHP(playerPokemon); // reset the Pokémon's HP after the battle
    resetHP(opponentPokemon);
}
void playerAttack(Pokemon& player, Pokemon& opponent) {
    int choice;
    do {
        cout << "Choose a move:" << endl;
        for (int i = 0; i < 4; i++){
            cout << i + 1 << ". " << player.moves[i].name << endl;
        }
        cin >> choice;
        cin.clear();          // Clear any errors
        cin.ignore(INT_MAX, '\n');  // Discard unwanted characters from the stream
         if (choice < 1 || choice > 4) {
            cout << "Invalid move choice. Please choose again." << endl;
        }
    } while (choice < 1 || choice > 4);
    executeMove(player, opponent, player.moves[choice-1]);
}
void opponentAttack(Pokemon& player, Pokemon& opponent) {
    int randomChoice = rand() % 4; // Randomly selects a move
    cout << opponent.name << " used " << opponent.moves[randomChoice].name << "!" << endl;
    // Execute move function call
    executeMove(opponent, player, opponent.moves[randomChoice]);
}
void executeMove(Pokemon& attacker, Pokemon& defender, Move move) {
    int damage;
    if (move.type == PHYSICAL) {
        damage = move.power + attacker.attack - defender.defense;
    } else {
        damage = move.power + attacker.spAttack - defender.spDefense;
    }
    defender.hp -= damage;
    if (defender.hp < 0) defender.hp = 0;
    cout << defender.name << " took " << damage << " damage!" << endl;
    cout << defender.name << " has " << defender.hp << " HP left!" << endl;
}
void resetHP(Pokemon& poke) {
    if(poke.name == "Bulbasaur") poke.hp = 45;
    else if(poke.name == "Charmander") poke.hp = 39;
    else if(poke.name == "Squirtle") poke.hp = 44;
}
vector<Pokemon> readPokemonFromFile(const string &filename) {
    ifstream file(filename);
    string line;
    vector<Pokemon> pokemons;

    // Skip header line
    getline(file, line);

    while (getline(file, line)) {
        stringstream ss(line);
        string token;

        Pokemon poke;

        // Parse Name
        getline(ss, token, ',');
        poke.name = token;

        // Skip Type1 and Type2 for now
        getline(ss, token, ',');
        getline(ss, token, ',');

        // Parse HP
        getline(ss, token, ',');
        poke.hp = stoi(token);

        // Parse Attack
        getline(ss, token, ',');
        poke.attack = stoi(token);

        // Parse Defense
        getline(ss, token, ',');
        poke.defense = stoi(token);

        // Parse Special Attack
        getline(ss, token, ',');
        poke.spAttack = stoi(token);

        // Parse Special Defense
        getline(ss, token, ',');
        poke.spDefense = stoi(token);

        // Parse Speed
        getline(ss, token, ',');
        poke.speed = stoi(token);

        // Parse Moves (only storing the first 4 for simplicity)
        // Parse Moves (only storing the first 4 for simplicity)
        getline(ss, token, ',');
        stringstream moveStream(token);
        for (int i = 0; i < 4; ++i) {
        string moveName;
        getline(moveStream, moveName, ';');
        Move tempMove = {moveName, 50, PHYSICAL};  // Create a temporary Move object
             poke.moves[i] = tempMove;  // Assign the temporary Move object to the array
}


        pokemons.push_back(poke);
    }

    return pokemons;
}