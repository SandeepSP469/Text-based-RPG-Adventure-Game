#include <iostream>
using namespace std;

class Character
{
protected:
    int health;
    int meleeDamage;
public:
    Character(int _health) 
    {
        health = _health; 
    }
    void SetHealth(int _health)
    {
        health = _health;
    }
    int GetHealth() 
    { 
        return health;
    }
    bool IsAlive() 
    { 
        return health > 0;
    }
    virtual void TakeDamage(int _damageReceived) 
    { 
        health -= _damageReceived;
    }
    virtual float Attack() 
    { 
        return meleeDamage;
    }
    virtual void Stats() = 0;
};

class SpecialAbility
{
private:
    string name = "none";
    bool isActive = false;
    float probability;
public:
    SpecialAbility(string _name, float _prob = 10, bool _isActive = false)
    {
        name = _name;
        isActive = _isActive;
        probability = _prob;
    }
    void Enable()
    {
        cout << name << "\n\n";
        isActive = true;
    }
    void SetProb(float _prob)
    {
        probability = _prob;
    }

    bool Use()
    {
        srand(time(0));
        if (rand() % 100 <= probability && isActive)
        {
            cout << name << " is Activated\n";
            return true;
        }
        return false;
    }
};

class Player : public Character
{
    SpecialAbility* sp[4];
private:
    int maxHealth;
    int rangedDamage;
    int heal;
    int specialAttackProbability = 0;
    bool rangedActive;
    bool inVulnerable;
    string Items[5];
public:
    Player(int _health, int _meleeDamage, int _rangedDamage, int _heal) : Character(_health)
    {
        maxHealth = _health;
        meleeDamage = _meleeDamage;
        rangedDamage = _rangedDamage;
        heal = _heal;
        rangedActive = false;
        inVulnerable = false;
        CreateAbility();
        Colletibles();
    }

    int MeleeAttack() 
    {
        return meleeDamage;
    }

    int RangedAttack()
    { 
        return rangedDamage;
    }
    
    void OverHeal()
    {
        if (maxHealth <= health)
        health = maxHealth;
    }

    void PlayerUpdate(int _health, int _meleeDamage, int _heal)
    {
        health = _health;
        meleeDamage = _meleeDamage;
        heal = _heal;
    }

    void Colletibles()
    {
        Items[0] = "Map";
        Items[1] = "Sword";
        Items[2] = "Shield";
        Items[3] = "Armour";
        Items[4] = "Bow";
    }

    void SpecialAbilityActivated(int number) 
    { 
        if (number < 5)
        {
            cout << "Congratulations! You have found the " << Items[number] << "\n";
        }
        if (number <= 3)
        {
            cout << "\nYou have unlocked the special ability: ";
            sp[number]->Enable();
        }
    }

    void CreateAbility() {
        sp[0] = new SpecialAbility("Critical Hit");
        sp[1] = new SpecialAbility("Blocker");
        sp[2] = new SpecialAbility("Life Steal");
        sp[3] = new SpecialAbility("Ranged Attack");
    }

    void Stats()
    {
        cout << "Player's Health = " << GetHealth() << endl;
        cout << "Player's Melee Attack Strength = " << MeleeAttack() << endl;
        cout << "Player's Ranged Attack Strength = " << RangedAttack() << endl;
        cout << "Player's Healing Rate = " << heal << endl;
    }

    void Heal()
    {
        srand(time(0));
        SetHealth(health + heal);
        OverHeal();
    }

    void TakeDamage(int _damageReceived)
    {
        if (inVulnerable)
        {
            cout << "Invulnerabilty Active";
            inVulnerable = false;
            return;
        }
        if (sp[1]->Use())
        {
            cout << "Attack Blocked";
            return;
        }

        health -= _damageReceived;
    }

    float Attack()
    {
        if (rangedActive)
        {
            inVulnerable = true;
            rangedActive = false;
        }

        float effectivePower = meleeDamage;

        if (sp[3]->Use()) {
            effectivePower = rangedDamage;
            rangedActive = true;
        }

        if (sp[0]->Use())
        {
            effectivePower += 50;
        }

        if (sp[2]->Use()) {
            health += effectivePower;
            OverHeal();
        }

        return effectivePower;
    }
};

class Enemy : public Character
{
protected:
    int maxDamage = 0;
    int rangedAttack = 0;
    int damageRange = 10;
public:
int Randomnumber(int range, int max)
{
    int randNum = max - (rand() % range);
    return randNum;
}
    Enemy(int _health, int _maxDamage) :Character(_health)
    {
        maxDamage = _maxDamage;
    }
    int RangedAttack()
    {
        srand(time(0));
        rangedAttack = Randomnumber(damageRange, maxDamage);
        cout << "Enemy's Attack Strength = " << rangedAttack << endl << endl;
        return rangedAttack;
    }

    void EnemyReset()
    {
        health = 10;
        maxDamage = 10;
    }

    void Stats()
    {
        cout << "Enemy's Health = " << GetHealth() << endl;
        cout << "Enemy's max Damage = " << maxDamage << endl;
    }
};

class Murlock : public Enemy
{  
private:
    int maxHealth;
    bool rangedActive;
    bool inVulnerable;
    SpecialAbility* sa[7];
public:
    Murlock(int _health, int _meleeDamage,int _maxDamage) :Enemy(_health, _maxDamage)
    {
        maxDamage = _maxDamage;
        meleeDamage = _meleeDamage;
        rangedActive = false;
        inVulnerable = false;
        Abilities();
    }

    void Abilities()
    {
        sa[0] = new SpecialAbility("Critical Hit", 20, true);
        sa[1] = new SpecialAbility("Blocker", 20, true);
        sa[2] = new SpecialAbility("Life Stealer", 20, true);
        sa[3] = new SpecialAbility("Ranged Attack", 20, true);
    }

    void TakeDamage(int _damageReceived)
    {
        if (sa[1]->Use())
        {
            cout << "Attack Blocked";
            return;
        }
        health -= _damageReceived;
    }

    float Attack()
    {
        if (rangedActive)
        {
            inVulnerable = true;
            rangedActive = false;
        }
        float effectivePower = meleeDamage;

        if (sa[3]->Use()) {
            effectivePower = meleeDamage;
            rangedActive = true;
        }

        if (sa[0]->Use())
        {
            effectivePower += 50;
        }

        if (sa[2]->Use()) {
            health += effectivePower;
        }
        return effectivePower;
    }

    void Stats()
    {
        cout << "Murlock's Health = " << GetHealth() << endl;
        cout << "Murlock's Attack Strength = " << meleeDamage << endl;
    }
};

class Levels
{
    int levelNum = 1;
    bool playerWin;
    Player* _player;
    Murlock* _boss;
public:
    Levels(Player* player)
    {
        _player = player;
    }
    void PlayerInput(Enemy* _enemy)
    {
        int input;
        cout << "\nEnter\n1 to Attack\n2 for Heal\nEnter your choice:";
        cin >> input;
        cout << "\n\n";
        
        switch (input)
        {
        case (1):
        {
            _enemy->TakeDamage(_player->MeleeAttack());
            break;
        }
        case (2):
        {
            _player->Heal();
            break;
        }

        default:
            break;
        }
    }

    void EnemyInput(Enemy* _enemy)
    {
        _player->TakeDamage(_enemy->RangedAttack());
    }


    void GameLoop(Enemy* _enemy)
    {
        cout << "\n\n";
        _player->Stats();
        cout << "\n\n";
        _enemy->Stats();

        while (_player->IsAlive() && _enemy->IsAlive())
        {
            PlayerInput(_enemy);
            _enemy->Stats();
            cout << "\n\n";
            if (_enemy->IsAlive())
            {
                EnemyInput(_enemy);
            }
            _player->Stats();
            cout << "\n\n";

            _enemy->Stats();
            cout << "\n\n";

            if (!_player->IsAlive())
            {
                playerWin = false;
                cout << "Player Died\n\n";
                cout << "\nLevel failed, please restart the game\n";
            }
            if (!_enemy->IsAlive())
            {
                playerWin = true;
                cout << "\nEnemy is Dead\n";
            }
        }
    }

    void Level(int number)
    {
        Enemy* e1;
        cout << "\nYou are now in Village " << number << "\n";
        cout << "Defeat all the enemies to continue\n\n";
        levelNum = number;
        for (int j = 1; j <= number; j++)
        {
            e1 = new Enemy(40 * number, 10 * number);
            while (_player->IsAlive()&& e1->IsAlive())
            {
                GameLoop(e1);
            }
            if (j == 6)
            {
                cout << "\n\nCongratulations! You have found where Murlocks is hiding. Defeat Murlocks to win back our village.\n\n";
                e1 = new Murlock(300, 50, 10);
                GameLoop(e1);
            }
            delete e1;
        }
        if(_player->IsAlive())
            _player->SpecialAbilityActivated(levelNum - 1);
    }
};

class Game
{
private:
    Player* player1;
    Levels* level;
    char restart;
public:
    Game()
    {
        player1 = new Player(80, 15, 10, 10);
    }
    void story()
    {
        cout << "Hi," << endl;
        cout << "Welcome To 'Dholakpur' " << endl;
        cout << "Our land under attack by a monster named 'Murlocs' and we need your help to defeat him" << endl;
        cout << "To reach the monster you have to cross 5 villages which are full of enemies" << endl << endl << endl;
    }
    void start()
    {
        do
        {  
            level = new Levels(player1);
            for (int i = 1; i <= 6; i++)
            {
                if (player1->IsAlive())
                {
                    player1->PlayerUpdate(50 + (50 * i), 10 + (20 * i), 10 + (30 * i));
                    level->Level(i);
                }
                else
                {
                    break;
                }
            }
            std::cout << "Enter R to play again: ";
            std::cin >> restart;
        } while (restart == 'R' || restart == 'r');
    }
};

int main()
{
    Game play;
    play.story();
    play.start();
    return 0;
}