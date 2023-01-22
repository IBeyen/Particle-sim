#pragma once
#include <cmath>

#define M_PI 3.14159265358979323846

class Particle
{
public:
	Particle() : size(1), pos_x(0), pos_y(0), vel_x(0), vel_y(0), elasticity(1), density(1) {}
	Particle(unsigned int size, double initial_pos_x, double initial_pos_y, double initial_vel_x, double initial_vel_y, double elasticity, unsigned int density = 1) : size(size), pos_x(initial_pos_x), pos_y(initial_pos_y), vel_x(initial_vel_x), vel_y(initial_vel_y), elasticity(elasticity), density(density) {}
	unsigned int size, density;
	double pos_x, pos_y, vel_x, vel_y, elasticity;
	unsigned int mass = pow(size, 2) * density * 200;
	
	void move(double delta_t)
	{
		pos_x += vel_x * delta_t;
		pos_y += vel_y * delta_t;
	}

	void apply_gravity(Particle *other, double G, double delta_t)
	{
		double distance = get_distance(other);
		double force = (G * mass * other->mass) / pow(distance, 2);
		double angle = atan2(other->pos_y - pos_y, other->pos_x - pos_x);
		vel_x += force/mass * cos(angle) * delta_t;
		vel_y += force/mass * sin(angle) * delta_t;
		other->apply_gravity(force, angle + M_PI, delta_t);
	}

	void apply_gravity(double force, double angle, double delta_t)
	{
		vel_x += force / mass * cos(angle) * delta_t;
		vel_y += force / mass * sin(angle) * delta_t;
	}
	
	void collision(Particle *other)
	{
		if (check_collision(other))
		{
			double phi = atan2(other->pos_y - pos_y, other->pos_x - pos_x);
			double v1 = sqrt(pow(vel_x, 2) + pow(vel_y, 2));
			double theta1 = atan2(vel_y, vel_x);
			double v2 = sqrt(pow(other->vel_x, 2) + pow(other->vel_y, 2));
			double theta2 = atan2(other->vel_y, other->vel_x);
			double m1 = mass, m2 = other->mass;
			// https://en.wikipedia.org/wiki/Elastic_collision#Two-dimensional_collision_with_two_moving_objects
			double fraction = (v1 * cos(theta1 - phi) * (m1 - m2) + 2 * m2 * v2 * cos(theta2 - phi)) / (m1 + m2);
			double vx_prime = fraction * cos(phi) + v1 * sin(theta1 - phi) * cos(phi + M_PI / 2);
			double vy_prime = fraction * sin(phi) + v1 * sin(theta1 - phi) * sin(phi + M_PI / 2);
			other->collision2(this);
			vel_x = vx_prime * elasticity;
			vel_y = vy_prime * elasticity;
		}
	}
	void collision2(const Particle *other)
	{
		double phi = atan2(other->pos_y - pos_y, other->pos_x - pos_x);
		double v1 = sqrt(pow(vel_x, 2) + pow(vel_y, 2));
		double theta1 = atan2(vel_y, vel_x);
		double v2 = sqrt(pow(other->vel_x, 2) + pow(other->vel_y, 2));
		double theta2 = atan2(other->vel_y, other->vel_x);
		double m1 = mass, m2 = other->mass;
		// https://en.wikipedia.org/wiki/Elastic_collision#Two-dimensional_collision_with_two_moving_objects
		double fraction = (v1 * cos(theta1 - phi) * (m1 - m2) + 2 * m2 * v2 * cos(theta2 - phi)) / (m1 + m2);
		vel_x = (fraction * cos(phi) + v1 * sin(theta1 - phi) * cos(phi + M_PI / 2)) * elasticity;
		vel_y = (fraction * sin(phi) + v1 * sin(theta1 - phi) * sin(phi + M_PI / 2)) * elasticity;
	}

private:
	double get_distance(const Particle *other) const
	{
		return sqrt(pow(pos_x - other->pos_x, 2) + pow(pos_y - other->pos_y, 2));
	}
	
	bool check_collision(const Particle *other) const
	{
		return get_distance(other) <= size + other->size;
	}
};