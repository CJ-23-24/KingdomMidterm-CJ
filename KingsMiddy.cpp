///CJ Shay
///COSC-2436
///10-20-2026




#include <iostream>
#include <string>
#include <vector>
#include <random>
#include <fstream>
#include <algorithm>
#include <limits>

using namespace std;

mt19937 rng(random_device{}());

int randomInt(int low, int high)
{
    uniform_int_distribution<int> dist(low, high);
    return dist(rng);
}

void line()
{
    cout << "========================================\n";
}

class Player
{
public:
    string name;
    int level = 1;
    int xp = 0;
    int xpNeeded = 100;
    int health = 100;
    int maxHealth = 100;
    int mana = 30;
    int maxMana = 30;
    int attack = 12;
    int defense = 4;
    int gold = 50;
    int potions = 3;
    int victories = 0;
    string weapon = "Rusty Sword";
    string armor = "Traveler Clothes";

    void stats()
    {
        line();
        cout << "HERO: " << name << "\n";
        cout << "Level: " << level << "\n";
        cout << "XP: " << xp << "/" << xpNeeded << "\n";
        cout << "Health: " << health << "/" << maxHealth << "\n";
        cout << "Mana: " << mana << "/" << maxMana << "\n";
        cout << "Attack: " << attack << "\n";
        cout << "Defense: " << defense << "\n";
        cout << "Gold: " << gold << "\n";
        cout << "Potions: " << potions << "\n";
        cout << "Weapon: " << weapon << "\n";
        cout << "Armor: " << armor << "\n";
        line();
    }

    void heal(int amount)
    {
        int oldHealth = health;
        health = min(maxHealth, health + amount);
        cout << "You recovered " << health - oldHealth
            << " health!\n";
    }

    void restoreMana(int amount)
    {
        int oldMana = mana;
        mana = min(maxMana, mana + amount);
        cout << "You recovered " << mana - oldMana
            << " mana!\n";
    }

    void gainXP(int amount)
    {
        xp += amount;
        cout << "You gained " << amount << " XP!\n";

        while (xp >= xpNeeded)
        {
            xp -= xpNeeded;
            level++;
            xpNeeded += 50;
            maxHealth += 20;
            maxMana += 10;
            attack += 3;
            defense += 2;
            health = maxHealth;
            mana = maxMana;

            line();
            cout << "LEVEL UP! You are now level "
                << level << "!\n";
            cout << "Your health and mana are restored!\n";
            line();
        }
    }

    bool usePotion()
    {
        if (potions <= 0)
        {
            cout << "You have no potions!\n";
            return false;
        }

        if (health == maxHealth)
        {
            cout << "Your health is already full!\n";
            return false;
        }

        potions--;
        heal(35);
        return true;
    }

    void save()
    {
        ofstream file("kingdom_save.txt");

        if (!file)
        {
            cout << "Could not save the game.\n";
            return;
        }

        file << name << '\n';
        file << level << '\n';
        file << xp << '\n';
        file << xpNeeded << '\n';
        file << health << '\n';
        file << maxHealth << '\n';
        file << mana << '\n';
        file << maxMana << '\n';
        file << attack << '\n';
        file << defense << '\n';
        file << gold << '\n';
        file << potions << '\n';
        file << victories << '\n';
        file << weapon << '\n';
        file << armor << '\n';

        cout << "Game saved successfully!\n";
    }

    bool load()
    {
        ifstream file("kingdom_save.txt");

        if (!file)
        {
            cout << "No save file found.\n";
            return false;
        }

        Player temp;

        getline(file, temp.name);

        if (!(file >> temp.level
            >> temp.xp
            >> temp.xpNeeded
            >> temp.health
            >> temp.maxHealth
            >> temp.mana
            >> temp.maxMana
            >> temp.attack
            >> temp.defense
            >> temp.gold
            >> temp.potions
            >> temp.victories))
        {
            cout << "Save file is invalid.\n";
            return false;
        }

        file.ignore(
            numeric_limits<streamsize>::max(), '\n');

        getline(file, temp.weapon);
        getline(file, temp.armor);

        if (temp.level < 1 ||
            temp.maxHealth < 1 ||
            temp.health < 0 ||
            temp.health > temp.maxHealth ||
            temp.maxMana < 0 ||
            temp.mana < 0 ||
            temp.mana > temp.maxMana ||
            temp.gold < 0 ||
            temp.potions < 0)
        {
            cout << "Save data is invalid.\n";
            return false;
        }

        *this = temp;
        cout << "Welcome back, " << name << "!\n";
        return true;
    }
};

class Enemy
{
public:
    string name;
    int health;
    int attack;
    int defense;
    int xpReward;
    int goldReward;
    bool boss;

    Enemy(string n, int h, int a, int d,
        int xp, int g, bool isBoss = false)
    {
        name = n;
        health = h;
        attack = a;
        defense = d;
        xpReward = xp;
        goldReward = g;
        boss = isBoss;
    }
};
Enemy createEnemy(int level, int danger)
{
    int power = level + danger;

    switch (randomInt(1, 5))
    {
    case 1:
        return Enemy("Goblin Scout",
            25 + power * 5, 6 + power * 2,
            1 + power, 25 + power * 5,
            10 + power * 3);

    case 2:
        return Enemy("Skeleton Warrior",
            35 + power * 6, 8 + power * 2,
            3 + power, 35 + power * 5,
            15 + power * 3);

    case 3:
        return Enemy("Cave Spider",
            30 + power * 5, 10 + power * 2,
            2 + power, 30 + power * 5,
            12 + power * 3);

    case 4:
        return Enemy("Dark Wolf",
            40 + power * 5, 11 + power * 2,
            2 + power, 40 + power * 5,
            18 + power * 3);

    default:
        return Enemy("Orc Brute",
            55 + power * 7, 12 + power * 2,
            5 + power, 50 + power * 5,
            22 + power * 3);
    }
}

int calculateDamage(int attack, int defense)
{
    int damage = attack + randomInt(-3, 5) - defense;
    return max(1, damage);
}

void enemyAttack(Player& player, Enemy& enemy)
{
    int damage = calculateDamage(
        enemy.attack, player.defense);

    if (randomInt(1, 100) <= 10)
    {
        damage *= 2;
        cout << "Critical hit from the enemy!\n";
    }

    player.health = max(0, player.health - damage);

    cout << enemy.name << " dealt "
        << damage << " damage!\n";

    cout << "Your health: " << player.health
        << "/" << player.maxHealth << "\n";
}

void playerAttack(Player& player, Enemy& enemy)
{
    int damage = calculateDamage(
        player.attack, enemy.defense);

    if (randomInt(1, 100) <= 15)
    {
        damage *= 2;
        cout << "CRITICAL HIT!\n";
    }

    enemy.health = max(0, enemy.health - damage);

    cout << "You dealt " << damage
        << " damage to " << enemy.name << "!\n";
}

bool fireball(Player& player, Enemy& enemy)
{
    if (player.mana < 10)
    {
        cout << "Not enough mana!\n";
        return false;
    }

    player.mana -= 10;

    int damage = 25 + player.level * 8
        + randomInt(0, 12);

    enemy.health = max(0, enemy.health - damage);

    cout << "Your fireball dealt " << damage
        << " magical damage!\n";

    return true;
}

bool combat(Player& player, Enemy enemy)
{
    line();
    cout << "A " << enemy.name << " appears!\n";
    cout << "Enemy health: " << enemy.health << "\n";
    line();

    while (player.health > 0 && enemy.health > 0)
    {
        cout << "\n1. Attack\n";
        cout << "2. Cast Fireball (10 mana)\n";
        cout << "3. Drink Potion\n";
        cout << "4. Escape\n";
        cout << "5. View Stats\n";
        cout << "Choose: ";

        int choice;

        if (!(cin >> choice))
        {
            cin.clear();
            cin.ignore(
                numeric_limits<streamsize>::max(), '\n');
            cout << "Enter a valid number.\n";
            continue;
        }

        bool usedTurn = false;

        switch (choice)
        {
        case 1:
            playerAttack(player, enemy);
            usedTurn = true;
            break;

        case 2:
            usedTurn = fireball(player, enemy);
            break;

        case 3:
            usedTurn = player.usePotion();
            break;

        case 4:
            if (enemy.boss)
            {
                cout << "You cannot escape the boss!\n";
            }
            else if (randomInt(1, 100) <= 60)
            {
                cout << "You escaped!\n";
                return true;
            }
            else
            {
                cout << "Escape failed!\n";
                usedTurn = true;
            }
            break;

        case 5:
            player.stats();
            break;

        default:
            cout << "Invalid choice.\n";
            break;
        }

        if (enemy.health > 0 && usedTurn)
        {
            enemyAttack(player, enemy);
        }

        cout << enemy.name << " health: "
            << enemy.health << "\n";
    }

    if (player.health <= 0)
    {
        cout << "\nYou were defeated!\n";
        return false;
    }

    cout << "\nYou defeated " << enemy.name << "!\n";
    cout << "You earned " << enemy.goldReward << " gold!\n";

    player.gold += enemy.goldReward;
    player.victories++;
    player.gainXP(enemy.xpReward);

    if (randomInt(1, 100) <= 25)
    {
        player.potions++;
        cout << "You found a health potion!\n";
    }

    return true;
}

struct Location
{
    string name;
    string description;
    int danger;
};

vector<Location> locations =
{
    {
        "Whispering Woods",
        "Dark trees surround you.",
        1
    },
    {
        "Crystal Caverns",
        "Glowing crystals light the tunnels.",
        2
    },
    {
        "Ashen Mountains",
        "Ash falls from the volcanic peaks.",
        3
    },
    {
        "Forgotten Ruins",
        "Ancient statues guard lost treasures.",
        4
    },
    {
        "Shadow Fortress",
        "The final enemy waits inside.",
        5
    }
};

void treasureEvent(Player& player)
{
    int reward = randomInt(15, 60);

    cout << "You discovered a treasure chest!\n";
    cout << "You found " << reward << " gold!\n";

    player.gold += reward;

    if (randomInt(1, 100) <= 30)
    {
        player.potions++;
        cout << "You also found a potion!\n";
    }
}

void trapEvent(Player& player)
{
    int damage = randomInt(8, 20);

    cout << "A hidden trap activates!\n";

    player.health = max(0, player.health - damage);

    cout << "You lost " << damage << " health!\n";
}

void randomEvent(Player& player, Location location)
{
    int event = randomInt(1, 100);

    if (event <= 50)
    {
        Enemy enemy = createEnemy(
            player.level, location.danger);

        combat(player, enemy);
    }
    else if (event <= 70)
    {
        treasureEvent(player);
    }
    else if (event <= 82)
    {
        trapEvent(player);
    }
    else if (event <= 92)
    {
        cout << "You find a peaceful clearing.\n";
        player.heal(15);
        player.restoreMana(8);
    }
    else
    {
        cout << "A traveler gives you 20 gold!\n";
        player.gold += 20;
    }
}
void explore(Player& player)
{
    line();
    cout << "CHOOSE YOUR DESTINATION\n";

    for (size_t i = 0; i < locations.size(); i++)
    {
        cout << i + 1 << ". "
            << locations[i].name
            << " (Danger " << locations[i].danger
            << ")\n";
    }

    cout << "0. Cancel\n";
    cout << "Destination: ";

    int choice;

    if (!(cin >> choice))
    {
        cin.clear();
        cin.ignore(
            numeric_limits<streamsize>::max(), '\n');
        return;
    }

    if (choice == 0)
        return;

    if (choice < 1 ||
        choice > static_cast<int>(locations.size()))
    {
        cout << "Invalid destination!\n";
        return;
    }

    Location location = locations[choice - 1];

    line();
    cout << location.name << "\n";
    cout << location.description << "\n";
    line();

    if (location.danger == 5 && player.level < 3)
    {
        cout << "Warning: this place is very dangerous!\n";
    }

    randomEvent(player, location);

    if (player.health <= 0)
        return;

    if (location.danger == 5 && player.level >= 3)
    {
        cout << "\nYou discover the Shadow King's chamber!\n";
        cout << "1. Fight the final boss\n";
        cout << "2. Retreat\n";
        cout << "Choice: ";

        int answer;

        if (!(cin >> answer))
        {
            cin.clear();
            cin.ignore(
                numeric_limits<streamsize>::max(), '\n');
            return;
        }

        if (answer == 1)
        {
            Enemy boss(
                "Shadow King",
                180 + player.level * 25,
                18 + player.level * 3,
                8 + player.level,
                500,
                500,
                true
            );

            if (combat(player, boss) && player.health > 0)
            {
                line();
                cout << "YOU SAVED THE KINGDOM!\n";
                cout << "The Shadow King has fallen!\n";
                cout << "Your name will live forever!\n";
                line();
            }
        }
    }
}

void shop(Player& player)
{
    while (true)
    {
        line();
        cout << "TRAVELING MERCHANT\n";
        cout << "Your gold: " << player.gold << "\n\n";

        cout << "1. Iron Sword (+8 attack) - 60 gold\n";
        cout << "2. Steel Sword (+15 attack) - 130 gold\n";
        cout << "3. Knight Armor (+9 defense) - 90 gold\n";
        cout << "4. Dragon Armor (+16 defense) - 200 gold\n";
        cout << "5. Health Potion - 20 gold\n";
        cout << "6. Leave shop\n";
        cout << "Choice: ";

        int choice;

        if (!(cin >> choice))
        {
            cin.clear();
            cin.ignore(
                numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        if (choice == 6)
            break;

        int price = 0;

        switch (choice)
        {
        case 1: price = 60; break;
        case 2: price = 130; break;
        case 3: price = 90; break;
        case 4: price = 200; break;
        case 5: price = 20; break;

        default:
            cout << "Invalid choice!\n";
            continue;
        }

        if (player.gold < price)
        {
            cout << "Not enough gold!\n";
            continue;
        }

        player.gold -= price;

        switch (choice)
        {
        case 1:
            if (player.attack < 20)
            {
                player.attack = 20;
                player.weapon = "Iron Sword";
            }
            else
            {
                cout << "You already have a stronger weapon.\n";
                player.gold += price;
            }
            break;

        case 2:
            if (player.attack < 27)
            {
                player.attack = 27;
                player.weapon = "Steel Sword";
            }
            else
            {
                cout << "You already have a stronger weapon.\n";
                player.gold += price;
            }
            break;

        case 3:
            if (player.defense < 13)
            {
                player.defense = 13;
                player.armor = "Knight Armor";
            }
            else
            {
                cout << "You already have stronger armor.\n";
                player.gold += price;
            }
            break;

        case 4:
            if (player.defense < 20)
            {
                player.defense = 20;
                player.armor = "Dragon Armor";
            }
            else
            {
                cout << "You already have stronger armor.\n";
                player.gold += price;
            }
            break;

        case 5:
            player.potions++;
            break;
        }

        cout << "Purchase complete!\n";
    }
}

void rest(Player& player)
{
    cout << "Resting costs 15 gold.\n";
    cout << "1. Rest\n";
    cout << "2. Cancel\n";
    cout << "Choice: ";

    int choice;

    if (!(cin >> choice))
    {
        cin.clear();
        cin.ignore(
            numeric_limits<streamsize>::max(), '\n');
        return;
    }

    if (choice == 1)
    {
        if (player.gold < 15)
        {
            cout << "You don't have enough gold.\n";
            return;
        }

        player.gold -= 15;
        player.health = player.maxHealth;
        player.mana = player.maxMana;

        cout << "You feel completely refreshed!\n";
    }
}

void showInventory(Player& player)
{
    line();
    cout << "INVENTORY\n";
    cout << "Weapon: " << player.weapon << "\n";
    cout << "Armor: " << player.armor << "\n";
    cout << "Health potions: " << player.potions << "\n";
    cout << "Gold: " << player.gold << "\n";
    line();
}

void showQuests(Player& player)
{
    line();
    cout << "QUEST JOURNAL\n";

    cout << "1. Goblin Hunter\n";
    cout << "   Defeat 3 enemies.\n";
    cout << "   Progress: " << player.victories << "/3\n";

    if (player.victories >= 3)
    {
        cout << "Quest complete! Reward: 60 gold.\n";
        cout << "Claim reward? (1 = yes, 2 = no): ";

        int answer;

        if (cin >> answer && answer == 1)
        {
            player.gold += 60;
            player.victories = -1000000;
            cout << "Reward claimed!\n";
        }
    }

    cout << "\n2. Reach level 3\n";

    if (player.level >= 3)
        cout << "Completed!\n";
    else
        cout << "Current level: " << player.level << "/3\n";

    line();
}

void story()
{
    line();

    cout << "THE LEGEND OF THE LOST KINGDOM\n\n";

    cout << "Long ago, Eldoria was protected by a magical crystal.\n";
    cout << "A powerful sorcerer stole its magic and plunged\n";
    cout << "the kingdom into darkness.\n\n";

    cout << "Monsters now roam the forests, treasures lie\n";
    cout << "buried beneath ancient ruins, and the Shadow King\n";
    cout << "waits inside his terrible fortress.\n\n";

    cout << "You are the hero destined to restore the crystal.\n";
    cout << "Your adventure begins now!\n";

    line();
}
void gameMenu(Player& player)
{
    bool playing = true;

    while (playing && player.health > 0)
    {
        line();

        cout << "THE LOST KINGDOM\n";
        cout << "Hero: " << player.name
            << " | Level: " << player.level
            << " | Gold: " << player.gold << "\n\n";

        cout << "1. Explore the world\n";
        cout << "2. Visit the merchant\n";
        cout << "3. View character stats\n";
        cout << "4. View inventory\n";
        cout << "5. View quests\n";
        cout << "6. Rest at camp\n";
        cout << "7. Save game\n";
        cout << "8. Return to title screen\n";
        cout << "Choice: ";

        int choice;

        if (!(cin >> choice))
        {
            cin.clear();
            cin.ignore(
                numeric_limits<streamsize>::max(), '\n');
            cout << "Please enter a number.\n";
            continue;
        }

        switch (choice)
        {
        case 1:
            explore(player);
            break;

        case 2:
            shop(player);
            break;

        case 3:
            player.stats();
            break;

        case 4:
            showInventory(player);
            break;

        case 5:
            showQuests(player);
            break;

        case 6:
            rest(player);
            break;

        case 7:
            player.save();
            break;

        case 8:
            playing = false;
            break;

        default:
            cout << "Invalid choice!\n";
            break;
        }

        if (player.health <= 0)
        {
            line();
            cout << "GAME OVER\n";
            cout << "Your adventure has ended.\n";
            line();
        }
    }
}

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    while (true)
    {
        line();

        cout << R"(
 _   _   _____   _____   _____
| | | | |  _  | /  ___| |_   _|
| |_| | | | | | | |       | |
|  _  | | | | | | |       | |
| | | | \ \_/ / | |___    | |
\_| |_/  \___/   \____|   \_/

       THE LOST KINGDOM
)";
        line();

        cout << "1. New Game\n";
        cout << "2. Load Saved Game\n";
        cout << "3. Quit\n";
        cout << "Choice: ";

        int choice;

        if (!(cin >> choice))
        {
            cin.clear();
            cin.ignore(
                numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        if (choice == 1)
        {
            Player player;

            cin.ignore(
                numeric_limits<streamsize>::max(), '\n');

            cout << "Enter your hero's name: ";
            getline(cin, player.name);

            if (player.name.empty())
                player.name = "Hero";

            if (player.name.size() > 40)
                player.name.resize(40);

            story();
            gameMenu(player);
        }
        else if (choice == 2)
        {
            Player player;

            if (player.load())
                gameMenu(player);
        }
        else if (choice == 3)
        {
            cout << "Thanks for playing!\n";
            break;
        }
        else
        {
            cout << "Invalid choice!\n";
        }
    }

    return 0;
}