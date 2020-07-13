#include <iostream>
#include <random>
#include <cassert>

using namespace std;

class Player {
		static vector<Player> players;
		string _name;
		int _score;

	public:
		static Player& addPlayer(const string& name, int score);
		static void resetPlayerScores(); 
		static Player& getPlayer(const string& name);
		static Player& getPlayer(size_t pos);
		static vector<Player> getPlayers();

		Player(const string& name, int score);

		// Player(const Player&) = delete;
		Player& operator=(const Player&) = delete;

		bool updateScore(int delta);
		int score() const;
		void score(int score);
		const string& name() const;

};

Player DefaultPlayer("", 0);

vector<Player> Player::players;

Player& Player::addPlayer(const string& name, int score) {
	Player p(name, score);
	players.push_back(p);
	return players.back();
}

void Player::resetPlayerScores() {
	for (Player& p: players) {
		p.score(301);
	}
}

Player& Player::getPlayer(const string& name) {
	for (Player& p: players) {
		if (p.name() == name) {
			return p;
		}
	}
	// assert(false);
	return DefaultPlayer;
}

Player& Player::getPlayer(size_t pos) {
	if (pos > players.size()) {
		return DefaultPlayer;
	}
	return players[pos];
}

vector<Player> Player::getPlayers() {
	return players;
}

Player::Player(const string& name, int score)
	: _name(name), _score(score)
{
}

bool Player::updateScore(int delta) {
	if (_score - delta >= 0) {
		_score -= delta;
		return true;
	}
	return false;
}

int Player::score() const {
	return _score;
}

void Player::score(int score) {
	_score = score;
}

const string& Player::name() const {
	return _name;
}

// from: https://stackoverflow.com/a/36527160/6104519 
float get_random() {
	static default_random_engine e;
	static uniform_real_distribution<> dis(0, 1);
	return dis(e);
}

/*
 * Which ring on the board has been hit. Radius from center to outside.
 * Something like:
 *   > .95 = center (50 points)
 *   > .9  = outter center (25 points)
 *   > .6 && <= 0.65 tripple ring
 *   ... and so on
 */
int roll_ring() {
	float random = get_random();
	assert(random >= 0 && random < 1.0);
	if (random > 0.95) {
		return 50;
	} else if (random > 0.9) {
		return 25;
	} else if (random > 0.6 && random <= 0.65) {
		return 3;
	} else if (random < .1) {
		return 2;
	} else {
		return 1;
	}
	assert(false);
}

int roll_number() {
	float random = get_random();
	return floor(20 * random) + 1;
}

string bool_str(bool f) {
	return (f) ? "yes" : "no";
}

int manual_attempt() {
	cout << "Enter your score: ";
	string score;
	getline(cin, score);
	cout << "YOUR SCORE: " << score << endl;

	int result = 0;
	bool is_double = false, is_triple = false;
	if (score.find("2*") != string::npos) {
		is_double = true;
		score = score.substr(score.find("2*") + 2);
		cout << "score is now: " << score << endl;
	} else if (score.find("3*") != string::npos) {
		is_triple = true;
		score = score.substr(score.find("3*") + 2);
		cout << "score is now: " << score << endl;
	}
	int iscore = atoi(score.c_str()); // TODO: ???
	if (iscore < 0 || (iscore > 20 && iscore != 25 && iscore != 50)) {
		cout << "invalid score: " << iscore << endl;
	} else {
		if (iscore == 50) {
			result = 25;
			is_double = true;
		} else {
			result = iscore;
			if (is_double) {
				result *= 2;
			} else if (is_triple) {
				result *= 3;
			}
		}
	}
	cout << "++ " << result << "  2* " << bool_str(is_double) << "  3* " << bool_str(is_triple) << endl;

	//throw runtime_error("ABORT. ABEND.");
	return result;
}

int attempt() {
	int result;
	int r = roll_ring();
	if (r != 50 && r != 25) {
		result = roll_number();
		assert(result > 0 && result <= 20);
		if (r == 2 || r == 3) {
			result *= r;
		}
	} else {
		result = r;
	}
	cout << "Wow, you hit: " << r << "  result: " << result << endl;
	return result;
}

void play() {
	Player::resetPlayerScores();
	int loopCount = 20;
	while (--loopCount >= 0) {
		cout << "Round: " << (20 - loopCount) << endl;
		// Loop Over Players
		// for (Player& p: Player::getPlayers()) {

		size_t NUM_PLAYERS = Player::getPlayers().size();
		for (size_t i=0; i<NUM_PLAYERS; ++i) {
			Player&p = Player::getPlayer(i);
			cout << p.name() << "'s score is: " << p.score() << endl;
			int attempts = 3;
			while (--attempts >= 0) {
				cout << "DBG: Attempt #" << (3 - attempts) << endl;
				int score;
				if (p.name() != "computer") {
					score = manual_attempt();
				} else {
					score = attempt();
				}
				if (!p.updateScore(score)) {
					// TODO: rename vars:
					cout << "Too High: You have " << p.score() << " left, but your"
						" attempt was: " << score << endl;
					// next player
					break;
				}
				if (p.score() == 0) {
					cout << p.name() << " has won, in round #" << (20 - loopCount) << endl;
					return;
				} else {
					cout << p.name() << " has " << p.score() << " left." << endl;
				}
			}
		}
	}
	cout << "No winner. Better luck next time." << endl;
}

void initPlayers() {
	string name;
	// while (players.size() < 9) {
	cout << "Please give the names of the players. Empty to quit adding new players." << endl;
	size_t numPlayers = 0;
	while (numPlayers < 9) {
		cout << "Player name: ";
		getline(cin, name);
		if ("" == name) break;
		Player& p = Player::addPlayer(name, 0);
		cout << "Hello " << p.name() << "! Good to see you." << endl;
		++numPlayers;
	}
	assert(Player::getPlayers().size() == numPlayers);
	cout << "Let's play darts." << endl;
	cout << "Playing with " << Player::getPlayers().size() << " players." << endl;
}

void run() {
	// TODO:
	//   How Many Players?
	//   Enter name or (computer) for automated user
	//   Choose Game Type (501, 301, ...) Masters In/Out, Double/Tripple In/Out
	//
	// TODO:
	//   Make 'roll_ring', 'roll_number' interactive
	//
	// TODO:
	//   HighScore List
	//
	// Let's just play plain 301
	initPlayers();
	//initGameType();

	bool bAgain = true;
	do {
		play();
		string again;
		cout << "Play again (y/N)? ";
		getline(cin, again);

		cout << "DBG: again: " << again << endl;
		if (again.size() == 0) {
			break;
		}
		string sYesNo = again.substr(0, 1);
		bAgain = (!sYesNo.compare("y") || !sYesNo.compare("Y"));
	} while (bAgain);
	cout << "GAME OVER" << endl;
}

int main(int argc, const char** argv) {
	try {
		run();
	} catch (...) {
		cerr << "Bekackt!" << endl;
		return 1;
	}
	return 0;
}
