#pragma once

class Weapon {
public:
    Weapon();

    void update(float dt);
    bool canAttack() const;
    void resetCooldown();

    float getDamage() const;

private:
    float damage;
    float cooldown;
    float timer;
};