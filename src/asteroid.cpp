#include "asteroid.h"
#include "constants.h"
#include "utils.h"
#include "spaceship.h"
#include "raymath.h"

Asteroid Asteroid::createAsteroid(int asteroidLevel, float asteroidX, float asteroidY) {
    Asteroid newAsteroid;

    newAsteroid.level = asteroidLevel;
    newAsteroid.x = asteroidX;
    newAsteroid.y = asteroidY;
    newAsteroid.dx = Utils::randomFloat(-Constants::ASTEROID_SPEED, Constants::ASTEROID_SPEED);
    newAsteroid.dy = Utils::randomFloat(-Constants::ASTEROID_SPEED, Constants::ASTEROID_SPEED);
    newAsteroid.radius = newAsteroid.level * 10.0f;

    float angleStep = 2.0f * PI / Constants::ASTEROID_POINTS;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dist(0.5f, 1.5f);

    for (int i = 0; i < Constants::ASTEROID_POINTS; ++i) {
        float randomRadius = newAsteroid.radius * dist(gen);
        Vector2 point = { static_cast<float>(cos(angleStep * i) * randomRadius), static_cast<float>(-sin(angleStep * i) * randomRadius) };
        float distance = Vector2Length(point);
        if (distance > newAsteroid.radius) {
            point = Vector2Scale(point, newAsteroid.radius / distance);
        }
        newAsteroid.points.push_back(point);
    }

    return newAsteroid;
}


void Asteroid::update(Spaceship& spaceship) {
    x += dx;
    y += dy;

    // Wrap around screen edges
    if (x < -radius) x += static_cast<float>(GetScreenWidth()) + radius * 2;
    if (x > static_cast<float>(GetScreenWidth()) + radius) x -= static_cast<float>(GetScreenWidth()) + radius * 2;
    if (y < -radius) y += static_cast<float>(GetScreenHeight()) + radius * 2;
    if (y > static_cast<float>(GetScreenHeight()) + radius) y -= static_cast<float>(GetScreenHeight()) + radius * 2;
}

void Asteroid::draw() const {
    if (points.empty()) return;

    std::vector<Vector2> translated_points = points;
    for (Vector2& point : translated_points) {
        point.x += x;
        point.y += y;
    }
    translated_points.push_back(translated_points[0]);

    DrawLineStrip(translated_points.data(), translated_points.size(), Constants::ASTEROID_COLOR);
}
