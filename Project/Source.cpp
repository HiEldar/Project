#include <SFML/Graphics.hpp>
#include<iostream>
#include <array>
#include <random>
#include <stdlib.h> 


int random_number(int m)
{
	int num = rand() % m;

	return num;
}

using namespace sf;

constexpr float window_width = 53;
constexpr float window_height = 30;
View view;

struct tile
{
	float center_x, center_y;
	std::array<bool, 6> connections; // clockwise from the top left
	unsigned layer;
	unsigned layer_index;

	unsigned neighbour[6];
	std::array<bool, 6> broken_wall;
	unsigned visited;
	bool coin = true;

};

int main()
{
	constexpr float d = 130;
	constexpr double f = 0.0015;
	bool conn = true;
	bool key = false;
	bool way_out = false;

	RenderWindow window(VideoMode(1000, 1000), "New World", Style::Fullscreen);
	view.setSize(window_width * 10, window_height * 10);
	Texture back_texture;
	back_texture.loadFromFile("images\\bottomwall1.png");
	Sprite back_sprite;
	back_sprite.setTexture(back_texture);
	//back_sprite.setOrigin(300, 300);

	Texture end_texture;
	end_texture.loadFromFile("images\\win.png");
	Sprite end_sprite;
	end_sprite.setTexture(end_texture);
	end_sprite.setPosition(0, 0);

	Texture hero_texture;  //hero
	hero_texture.loadFromFile("images\\bluehero.png");
	Sprite hero_sprite;
	hero_sprite.setTexture(hero_texture);
	hero_sprite.setTextureRect(IntRect(0, 0, 60, 60));
	constexpr float hero_width = 60;
	constexpr float hero_height = 60;
	hero_sprite.setPosition(-hero_width / 2.0, -hero_height / 2.0);
	view.setCenter(hero_sprite.getPosition().x + hero_width / 2.0, hero_sprite.getPosition().y + hero_height / 2.0);

	Texture key_texture;
	key_texture.loadFromFile("images\\key.png");
	Sprite key_sprite;
	key_sprite.setTexture(key_texture);
	key_sprite.setTextureRect(IntRect(0, 0, 65, 65));
	const float key_width = 65;
	const float key_height = 65;

	Texture coin_texture;
	coin_texture.loadFromFile("images\\Coin_Spin_Animation_A.png");
	Sprite coin_sprite;
	coin_sprite.setTexture(coin_texture);
	coin_sprite.setScale(0.07, 0.07);
	coin_sprite.setTextureRect(IntRect(0, 0, 500, 500));
	const float coin_width = 40;
	const float coin_height = 45;

	Texture wall_texture;
	wall_texture.loadFromFile("images\\wall.png");
	Sprite wall_sprite;
	wall_sprite.setTexture(wall_texture);
	wall_sprite.setScale(0.5, 0.5);
	wall_sprite.setTextureRect(IntRect(280, 0, 280, 35));
	const float wall_width = 280;
	const float wall_height = 40;

	Texture broken_wall_texture;
	broken_wall_texture.loadFromFile("images\\wall.png");
	Sprite broken_wall_sprite;
	broken_wall_sprite.setTexture(broken_wall_texture);
	broken_wall_sprite.setScale(0.5, 0.5);
	broken_wall_sprite.setTextureRect(IntRect(280, 80, 280, 35));

	const int N = 3;
	int size = 3 * N * N - 3 * N + 1;
	std::array<tile, 3 * N * N - 3 * N + 2> tiles;

	Vector2f const u(0, -d);
	Vector2f const v(0.8660254 * d, 0.5 * d);
	Vector2f const w(-0.8660254 * d, 0.5 * d);

	int spr[6];
	spr[0] = 330;
	spr[1] = 198;
	spr[2] = 140;
	spr[3] = 78;
	spr[4] = 451;
	spr[5] = 390;

	std::array<Vector2f, 6> mov;
	mov[0] = u - v;
	mov[1] = u - w;
	mov[2] = v - w;
	mov[3] = v - u;
	mov[4] = w - u;
	mov[5] = w - v;

	std::array<Vector2f, 6> vertexes;
	vertexes[0] = -v;
	vertexes[1] = u;
	vertexes[2] = -w;
	vertexes[3] = v;
	vertexes[4] = -u;
	vertexes[5] = w;

	Vector2f const u1(0, -10);
	Vector2f const v1(0.8660254 * 10, 0.5 * 10);
	Vector2f const w1(-0.8660254 * 10, 0.5 * 10);

	std::array<Vector2f, 6> vertexes1;
	vertexes1[0] = -v1;
	vertexes1[1] = u1;
	vertexes1[2] = -w1;
	vertexes1[3] = v1;
	vertexes1[4] = -u1;
	vertexes1[5] = w1;


	float curr_centerx = 0.f;
	float curr_centery = 0.f;

	float layer_firstx = 0.f;
	float layer_firsty = 0.f;

	unsigned index = 0;
	std::srand(std::time(NULL));


	for (int i = 0; i < N; ++i)  //border
	{
		curr_centerx = layer_firstx;
		curr_centery = layer_firsty;
		for (int j = 0; j < N + i; ++j)
		{
			tiles[index].center_x = curr_centerx;
			tiles[index].center_y = curr_centery;
			tiles[index].layer = i;
			tiles[index].layer_index = j;
			for (int c = 0; c < 6; ++c)
				tiles[index].connections[c] = false;
			curr_centerx += mov[2].x;
			++index;
		}
		layer_firstx += mov[4].x;
		layer_firsty += mov[4].y;
	}
	layer_firstx -= mov[4].x;
	layer_firsty -= mov[4].y;
	layer_firstx += mov[3].x;
	layer_firsty += mov[3].y;
	for (int i = N - 2; i >= 0; --i)
	{
		curr_centerx = layer_firstx;
		curr_centery = layer_firsty;
		for (int j = 0; j < N + i; ++j)
		{
			tiles[index].center_x = curr_centerx;
			tiles[index].center_y = curr_centery;
			tiles[index].layer = 2 * N - 2 - i;
			tiles[index].layer_index = j;
			for (int c = 0; c < 6; ++c)
				tiles[index].connections[c] = false;
			curr_centerx += mov[2].x;
			++index;
		}
		layer_firstx += mov[3].x;
		layer_firsty += mov[3].y;
	}

	key_sprite.setPosition(Vector2f(tiles[size - 1].center_x - key_width / 2, tiles[size - 1].center_y - key_height / 2));

	for (int t = 0; t < size; t++)  //neighbours 
	{
		tiles[t].neighbour[2] = t + 1;
		tiles[t].neighbour[5] = t - 1;
		if (tiles[t].layer <= N - 1)
			tiles[t].neighbour[0] = t - N - tiles[t].layer;
		else
			tiles[t].neighbour[0] = t - 3 * N + tiles[t].layer + 1;
		if (tiles[t].layer <= N - 1)
			tiles[t].neighbour[1] = t - N - tiles[t].layer + 1;
		else
			tiles[t].neighbour[1] = t - 3 * N + 2 + tiles[t].layer;
		if (tiles[t].layer < N - 1)
			tiles[t].neighbour[3] = t + N + tiles[t].layer + 1;
		else
			tiles[t].neighbour[3] = t + 3 * N - 2 - tiles[t].layer;
		if (tiles[t].layer < N - 1)
			tiles[t].neighbour[4] = t + N + tiles[t].layer;
		else
			tiles[t].neighbour[4] = t + 3 * N - tiles[t].layer - 3;
	}



	index = 0;
	for (int i = 0; i < N; ++i)
	{
		for (int j = 0; j < N + i; ++j)
		{
			tiles[index].layer = i;
			tiles[index].layer_index = j;
			if (tiles[index].layer == 0)
			{
				tiles[index].neighbour[0] = size;
				tiles[index].neighbour[1] = size;
			}
			if (tiles[index].layer_index == 0)
			{
				tiles[index].neighbour[0] = size;
				tiles[index].neighbour[5] = size;
				if (tiles[index].layer == N - 1)
					tiles[index].neighbour[4] = size;
			}
			if (tiles[index].layer_index == (N - 1 + tiles[index].layer))
			{
				tiles[index].neighbour[1] = size;
				tiles[index].neighbour[2] = size;
				if (tiles[index].layer == N - 1)
					tiles[index].neighbour[3] = size;
			}
			++index;
		}
	}
	for (int i = N - 2; i >= 0; --i)
	{
		for (int j = 0; j < N + i; ++j)
		{
			tiles[index].layer = 2 * N - 2 - i;
			tiles[index].layer_index = j;
			if (tiles[index].layer_index == 0)
			{
				tiles[index].neighbour[4] = size;
				tiles[index].neighbour[5] = size;
			}
			if (tiles[index].layer == 2 * N - 2)
			{
				tiles[index].neighbour[3] = size;
				tiles[index].neighbour[4] = size;
			}
			if (tiles[index].layer_index == (3 * N - tiles[index].layer - 3))
			{
				tiles[index].neighbour[2] = size;
				tiles[index].neighbour[3] = size;
			}
			++index;
		}
	}


	for (int i = 0; i < size; ++i)
	{
		for (int c = 0; c < 6; ++c)
		{
			if (tiles[i].neighbour[c] == size)
				tiles[i].connections[c] = false;
			else if (tiles[i].connections[c] == true)
			{
				int b = c - 3;
				if (b < 0)
					b = b + 6;
				tiles[tiles[i].neighbour[c]].connections[b] = true;
			}
		}
	}


	const unsigned move_count = 1 / f;
	unsigned current_shift = 0;
	short int hero_dir = 3;
	double hero_frame = 0;
	unsigned shoot = false;
	unsigned hero_tile = 0;
	unsigned key_tile = size - 1;
	double coin_frame = 0;
	double key_frame = 0;

	//creation
	std::vector <int> neigh;
	std::vector <int> mark;
	int t1, t2;
	int random_tile, random_neighbour;
	for (int i = 0; i < size; i++)
		tiles[i].visited = 0;
	tiles[0].visited = 1;
	mark.push_back(0);
	while (mark.size() > 0)
	{
		random_tile = random_number(mark.size());
		t1 = mark[random_tile];
		neigh.clear();
		for (int j = 0; j < 6; j++)
		{
			if (tiles[t1].neighbour[j] != size)
			{
				if (tiles[tiles[t1].neighbour[j]].visited == 0)
				{
					neigh.push_back(j);
				}
			}
		}
		if (neigh.size() > 0)
		{
			random_neighbour = neigh[random_number(neigh.size())];
			t2 = tiles[t1].neighbour[random_neighbour];
			if (neigh.size() <= 1)
			{
				tiles[t1].visited = 2;
				mark.erase(mark.begin() + random_tile);
			}
			neigh.clear();
			for (int j = 0; j < 6; j++)
			{
				if (tiles[t2].neighbour[j] != size)
				{
					if (tiles[tiles[t2].neighbour[j]].visited == 0)
					{
						neigh.push_back(j);
					}
				}
			}
			if (neigh.size() > 0)
			{
				mark.push_back(t2);
				tiles[t2].visited = 1;
			}
			else
				tiles[t2].visited = 2;
			tiles[t1].connections[random_neighbour] = true;
			tiles[t2].connections[(random_neighbour + 3) % 6] = true;
		}
		else
		{
			tiles[t1].visited = 2;
			mark.erase(mark.begin() + random_tile);
		}
		if (mark.size() == 1)
			key_tile = mark[0];
	}

	for (int tile = 0; tile < size; tile++)
	{
		for (int c = 0; c < 6; ++c)
		{
			if (c == 2 || c == 3 || c == 4)
			{
				if (random_number(2) == 0 && tiles[tile].neighbour[c] != size)
					tiles[tile].broken_wall[c] = true;
				else
					tiles[tile].broken_wall[c] = false;
			}
			else
			{
				if (tiles[tile].neighbour[c] != size)
					tiles[tile].broken_wall[c] = tiles[tiles[tile].neighbour[c]].broken_wall[(c + 3) % 6];
				else
					tiles[tile].broken_wall[c] = false;
			}
		}
	}

	std::array<Keyboard::Key, 6> keys;
	keys[0] = Keyboard::W;
	keys[1] = Keyboard::E;
	keys[2] = Keyboard::D;
	keys[3] = Keyboard::X;
	keys[4] = Keyboard::Z;
	keys[5] = Keyboard::A;


	bool moving = false;

	while (!Keyboard::isKeyPressed(Keyboard::Escape) && window.isOpen())  //window
	{
		if (way_out == false)
		{
			if (current_shift != 0)
			{
				hero_sprite.move(f * mov[hero_dir].x, f * mov[hero_dir].y);
				++current_shift;
				hero_frame += 0.01;

				if (current_shift >= move_count)
				{
					current_shift = 0;
					hero_sprite.setPosition(Vector2f(tiles[hero_tile].center_x - hero_width / 2, tiles[hero_tile].center_y - hero_height / 2));
					key_sprite.setPosition(Vector2f(tiles[key_tile].center_x - key_width / 2, tiles[key_tile].center_y - key_height / 2));
					moving = false;
					if (hero_tile == size)
					{
						way_out = true;
					}
				}
			}
			else
			{
				if (Keyboard::isKeyPressed(Keyboard::S) && current_shift == 0)
				{
					shoot = true;
					tiles[hero_tile].broken_wall[hero_dir] = false;
					if (tiles[hero_tile].neighbour[hero_dir] != size)
						tiles[tiles[hero_tile].neighbour[hero_dir]].broken_wall[(hero_dir + 3) % 6] = false;
				}
				for (int i = 0; i < 6; ++i)
				{
					if (Keyboard::isKeyPressed(keys[i]) && current_shift == 0)
					{
						hero_dir = i;
						if (tiles[hero_tile].connections[i] && !tiles[hero_tile].broken_wall[i])
						{
							++current_shift;
							hero_tile = tiles[hero_tile].neighbour[i];
							moving = true;
						}
						shoot = false;
					}
				}
			}
			if (hero_frame >= 4)
				hero_frame -= 4;
			if (!moving)
				hero_sprite.setTextureRect(IntRect(spr[hero_dir], 0, 55, 60));
			else
				hero_sprite.setTextureRect(IntRect(spr[hero_dir], int(hero_frame + 1) * 60, 55, 60));
			if (shoot == true && !moving)
				hero_sprite.setTextureRect(IntRect(spr[hero_dir], 5 * 60, 55, 60));
			if (hero_tile == key_tile && !key && current_shift == 0)
			{
				tiles[size - random_number(N) - 1].connections[3 + random_number(2)] = true;  //way out
				key = true;
			}
			if (current_shift == 0)
				tiles[hero_tile].coin = false;

			coin_frame += 0.01;
			coin_sprite.setTextureRect(IntRect(((int(coin_frame) / 4) % 2) * 500, int(coin_frame) % 4 * 500, 500, 500));

			key_frame += 0.01;
			key_sprite.setTextureRect(IntRect((int(key_frame) % 5) * 65, 0, 65, 65));

			//view
			sf::Event event;
			while (window.pollEvent(event))
			{
				if (event.type == sf::Event::Closed)
					window.close();
			}
			view.setCenter(hero_sprite.getPosition().x + hero_width / 2.0, hero_sprite.getPosition().y + hero_height / 2.0);

			//draw
			window.clear();
			for (int i = -N; i < N + 2; i++)
			{
				for (int j = -N; j < N + 2; j++)
				{
					back_sprite.setPosition(Vector2f(346.5 * i, 400 * j));
					window.draw(back_sprite);
				}
			}
			for (int tile = 0; tile < size; tile++)
			{
				for (int c = 0; c < 6; ++c)
				{
					if (!tiles[tile].connections[c])
					{
						/*VertexArray wall(Lines, 2);
						wall[0].position = Vector2f(tiles[tile].center_x, tiles[tile].center_y) + vertexes[c];
						wall[1].position = Vector2f(tiles[tile].center_x, tiles[tile].center_y) + vertexes[(c + 1) % 6];
						window.draw(wall);*/
						wall_sprite.setPosition(Vector2f(tiles[tile].center_x, tiles[tile].center_y) + vertexes[c] + vertexes1[c]);
						wall_sprite.setRotation(60 * c - 30);
						window.draw(wall_sprite);
					}
					else if (c == 2 || c == 3 || c == 4)
					{
						if (tiles[tile].broken_wall[c])
						{
							broken_wall_sprite.setPosition(Vector2f(tiles[tile].center_x, tiles[tile].center_y) + vertexes[c] + vertexes1[c]);
							broken_wall_sprite.setRotation(60 * c - 30);
							window.draw(broken_wall_sprite);
						}
					}
				}
			}
			window.draw(hero_sprite);
			if (!key)
				window.draw(key_sprite);
			for (int i = 1; i < size; i++)
			{
				if (tiles[i].coin && i != key_tile)
				{
					coin_sprite.setPosition(Vector2f(tiles[i].center_x - coin_width / 2, tiles[i].center_y - coin_height / 2));
					window.draw(coin_sprite);
				}
			}
		}
		else
		{
			//view
			Event event;
			while (window.pollEvent(event))
			{
				if (event.type == Event::Closed)
					window.close();
			}
			view.setSize(1000, 1000);
			view.setCenter(420, 320);
			window.clear();
			window.draw(end_sprite);
		}
		window.display();
		window.setView(view);
	}
	return 0;
}