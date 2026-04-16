#include "Weapon.hpp"

Weapon::Weapon()
    : damage(25.f)
    , cooldown(0.5f)
    , timer(0.f)
{
}

void Weapon::update(float dt) {
    if (timer > 0.f)
        timer -= dt;
}

bool Weapon::canAttack() const {
    return timer <= 0.f;
}

void Weapon::resetCooldown() {
    timer = cooldown;
}

float Weapon::getDamage() const {
    return damage;
}