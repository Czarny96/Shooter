#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <sstream>

using namespace sf;

int points = 0;
int lose = 0;

class Collision : public CircleShape
{
public:
	unsigned int speed = 2 + rand() % 4;
	float rot = rand() % 21 - 10;
	float x = 0;
	Time start;

	Collision(float r, Clock &clock)
	{
		this->setRadius(r);
		this->start = clock.getElapsedTime();
		this->setOrigin(r, r);
	}

	bool isCollisionEnemyithEnemy(Collision &potentialObject)
	{
		float checkCollision = sqrt(pow(this->getPosition().x - potentialObject.getPosition().x, 2) + pow(this->getPosition().y - potentialObject.getPosition().y, 2));
		if ((checkCollision < this->getRadius() + potentialObject.getRadius()))
		{
			checkCollision = (this->getRadius() + potentialObject.getRadius() - checkCollision) / 2.0;
			if (this->getPosition().x < potentialObject.getPosition().x)
			{
				this->setRadius(this->getRadius() - 1);
				potentialObject.setRadius(potentialObject.getRadius() - 1);
				this->x = -(float)(this->speed); potentialObject.x = potentialObject.speed;
				this->move(-checkCollision, 0); potentialObject.move(checkCollision, 0);
			}
			else
			{
				this->setRadius(this->getRadius() - 1);
				potentialObject.setRadius(potentialObject.getRadius() - 1);
				this->x = this->speed; potentialObject.x = -(float)(potentialObject.speed);
				this->move(checkCollision, 0); potentialObject.move(-checkCollision, 0);
			}
			return true;
		}
		return false;
	}

	bool isCollisionHeroithEnemy(Sprite &potentialObject)
	{
		float checkCollision = sqrt(pow(this->getPosition().x - potentialObject.getPosition().x, 2) + pow(this->getPosition().y - potentialObject.getPosition().y, 2));
		if ((checkCollision  < this->getRadius()))
		{
			points /= 2;
			this->setPosition(rand() % 775 + 25, -(rand() % 200));

			return true;
		}
		return false;
	}

	float MoveObject()
	{
		if (this->x == 0 || abs(this->x) < 0.2) { return this->x = 0; }
		else if (this->x < 0) { this->x += 0.1; return this->x; }
		else if (this->x > 0) { this->x -= 0.1;  return this->x; }
		else return 0;
	}
};


class Bullet : public CircleShape
{
public:
	unsigned int speed = 4;
	float x = 0;
	Time start;
	int color = 0;

	Bullet(float r, Clock &clock)
	{
		//radius = r;
		this->setRadius(r);
		this->start = clock.getElapsedTime();
		this->setOrigin(r, r);
	}

	int GetSpeed()
	{
		return speed;
	}

	void SetSpeed(int newSpeed)
	{
		speed = newSpeed;
	}

	bool isCollisionBulletWithEnemy(Collision &potentialObject)
	{
		float checkCollision = sqrt(pow(this->getPosition().x - potentialObject.getPosition().x, 2) + pow(this->getPosition().y - potentialObject.getPosition().y, 2));
		if ((checkCollision < this->getRadius() + potentialObject.getRadius()))
		{
			points++;
			this->SetSpeed(0);
			this->setPosition(-100, -100);
			potentialObject.setPosition(rand() % 775 + 25, -(rand() % 200));

			return true;
		}
		return false;
	}
};

int main()
{
	RenderWindow windowObject;
	windowObject.create(VideoMode(800, 800, 30), "Shooter");
	windowObject.setActive(true);

	windowObject.setFramerateLimit(60);

	windowObject.setPosition(Vector2i(10, 10));

	Vector2u getWindowSize = windowObject.getSize();

	View camera;
	camera.setCenter(getWindowSize.x / 2, getWindowSize.y / 2);
	camera.setSize(getWindowSize.x, getWindowSize.y);

	windowObject.setKeyRepeatEnabled(false);

	Texture texture;
	texture.create(30, 30);

	texture.loadFromFile("images/hero.png");

	Sprite heroImg(texture, IntRect(0, 0, 30, 30));
	heroImg.setPosition(Vector2f(getWindowSize.x / 2 - heroImg.getTextureRect().width / 2, getWindowSize.y - heroImg.getTextureRect().height));

	Font arialFont;
	arialFont.loadFromFile("Fonts/arial.ttf");
	
	Text pointText;
	pointText.setFont(arialFont);

	Text loseText;
	loseText.setFont(arialFont);
		
	pointText.setPosition(10, 770);
	pointText.setCharacterSize(16);
	pointText.setFillColor(Color::White);

	loseText.setPosition(100, 770);
	loseText.setCharacterSize(16);
	loseText.setFillColor(Color::White);

	std::vector <Collision> Enemy;
	Enemy.reserve(10);
	Texture* textureEnemy = new Texture;
	textureEnemy->loadFromFile("images/blocks.png", IntRect(0, 0, 100, 100));
	Clock clock;
	Event e;

	const int bulletSize = 100;

	std::vector <Bullet> Bullets;
	Bullets.reserve(bulletSize);
	Texture* textureBullet = new Texture;
	textureBullet->loadFromFile("images/bullet.png", IntRect(0, 0, 5, 20));
	Clock clock1;
	Event e1;

	int moveSpeed = 6;

	bool spacepress = false;
	Event action;

	unsigned long long int steps = 0;
	while (windowObject.isOpen())
	{
		while (windowObject.pollEvent(action))
		{
			if ((action.type == Event::Closed) || (Keyboard::isKeyPressed(Keyboard::Escape)))
				windowObject.close();

			if ((action.type == Event::KeyPressed) && (action.key.code == Keyboard::Space) && (Bullets.size() < bulletSize) && !spacepress)
			{
					Bullet bullet(10, clock1);
					bullet.setTexture(textureBullet);
					bullet.setPosition((heroImg.getPosition().x + 15), heroImg.getPosition().y);
					Bullets.push_back(bullet);
					spacepress = true;
			}
			else if ((action.type == Event::KeyPressed) && (action.key.code == Keyboard::Space) && !spacepress)
			{
				for (std::vector<Bullet>::iterator bulletUP = Bullets.begin(); bulletUP != Bullets.end(); bulletUP++)
				{
					if ((Bullets.size() == bulletSize) && bulletUP->GetSpeed() == 0)
					{
						bulletUP->setPosition((heroImg.getPosition().x + 15), heroImg.getPosition().y);
						bulletUP->SetSpeed(4);
					}
				}
				spacepress = true;
			}
			else if ((action.type == Event::KeyReleased) && (action.key.code == Keyboard::Space))
			{
				spacepress = false;
			}
		}

		if ((Keyboard::isKeyPressed(Keyboard::Left)) && (heroImg.getPosition().x > 0))
		{
			heroImg.setPosition(Vector2f(heroImg.getPosition().x - moveSpeed, getWindowSize.y - heroImg.getTextureRect().height));

		}
		else if ((Keyboard::isKeyPressed(Keyboard::Right)) && (heroImg.getPosition().x < getWindowSize.x - 30))
		{
			heroImg.setPosition(Vector2f(heroImg.getPosition().x + moveSpeed, getWindowSize.y - heroImg.getTextureRect().height));
		}

		windowObject.clear();

		steps++;
		if (!(steps % 20) && Enemy.size() < 10) {
			Collision createEnemy(30, clock);
			createEnemy.setTexture(textureEnemy);
			createEnemy.setPosition(rand() % (getWindowSize.x - 25) + 25, -(rand() % 300));
			Enemy.push_back(createEnemy);
		}

		for (std::vector<Bullet>::iterator bulletUP = Bullets.begin(); bulletUP != Bullets.end(); bulletUP++)
		{

			bulletUP->setPosition(Vector2f(bulletUP->getPosition().x, bulletUP->getPosition().y - 4));
			windowObject.draw(*bulletUP);

			if (bulletUP->getPosition().y < 0)
			{
				bulletUP->SetSpeed(0);
			}
		}

		if (Enemy.size())
		{
			
			for (std::vector<Collision>::iterator collisionObject = Enemy.begin(); collisionObject != Enemy.end(); collisionObject++)
			{
				collisionObject->move(collisionObject->MoveObject(), collisionObject->speed);
				collisionObject->rotate(collisionObject->rot);

				for (std::vector<Collision>::iterator collisionObject2 = Enemy.begin(); collisionObject2 != Enemy.end(); collisionObject2++)
				{
					if (collisionObject == collisionObject2) continue;
					collisionObject->isCollisionEnemyithEnemy(*collisionObject2);
				}

				for (std::vector<Bullet>::iterator bulletObject = Bullets.begin(); bulletObject != Bullets.end(); bulletObject++)
				{
					bulletObject->isCollisionBulletWithEnemy(*collisionObject);
				}

				collisionObject->isCollisionHeroithEnemy(heroImg);

				windowObject.draw(*collisionObject);

				if (collisionObject->getPosition().y > getWindowSize.y)
				{
					lose++;
					collisionObject->setRadius(30);
					collisionObject->setPosition(rand() % (getWindowSize.x + 25) - 25, -(rand() % 200));
				}
			}
		}
		windowObject.draw(heroImg);

		pointText.setString("Points: " + std::to_string(points));
		windowObject.draw(pointText);

		loseText.setString("Lose: " + std::to_string(lose));
		windowObject.draw(loseText);

		windowObject.display();
	}

	return 0;
}