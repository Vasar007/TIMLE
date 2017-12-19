#include "../Include/PlayerInfo.hpp"


PlayerInfo::PlayerInfo()
: _currentGameStatus(GameRunning)
, _currentLevelNumber(0)
, _player(nullptr)
, mDialogNumber(0)
, mShowedDialogs()
, mChoosingNumber(0)
, mChosenSolution(2)
, mQuests(5)
, mLoaded(false)
, mLivesCount(0u)
, mLastSavePoint(0.f, 0.f)
, mCanRessurect(false)
{
	// Set initial key bindings.
	_keyBinding[sf::Keyboard::Left] = MoveLeft;
	_keyBinding[sf::Keyboard::Right] = MoveRight;
	_keyBinding[sf::Keyboard::Up] = MoveUp;
	_keyBinding[sf::Keyboard::Down] = MoveDown;
	_keyBinding[sf::Keyboard::Space] = Fire;
	_keyBinding[sf::Keyboard::M] = LaunchMissile;

	// Set initial action bindings.
	initializeActions();

	// Assign all categories to player's aircraft
	//FOREACH(auto& pair, mActionBinding)
		//pair.second.category = Category::PlayerAircraft;
}

void PlayerInfo::showDialog(const std::size_t number)
{
	mDialogNumber = number;

	bool isFind = false;
	for (const auto& dialog : mShowedDialogs)
	{
		if (dialog == mDialogNumber)
		{
			mDialogNumber = 0u;
			isFind = true;
			break;
		}
	}

	if (!isFind)
	{
		mShowedDialogs.push_back(mDialogNumber);
	}
}


void PlayerInfo::assignKey(const Action action, const sf::Keyboard::Key key)
{
	// Remove all keys that already map to action.
	for (auto itr = _keyBinding.begin(); itr != _keyBinding.end();)
	{
		if (itr->second == action)
		{
			_keyBinding.erase(itr++);
		}
		else
		{
			++itr;
		}
	}

	// Insert new binding.
	_keyBinding[key] = action;
}

sf::Keyboard::Key PlayerInfo::getAssignedKey(const Action action) const
{
	for (const auto& pair : _keyBinding)
	{
		if (pair.second == action)
		{
			return pair.first;
		}
	}

	return sf::Keyboard::Unknown;
}

void PlayerInfo::setGameStatus(const GameStatus status)
{
	_currentGameStatus = status;
}

PlayerInfo::GameStatus PlayerInfo::getGameStatus() const
{
	return _currentGameStatus;
}

void PlayerInfo::setLevelNumber(const std::size_t number)
{
	_currentLevelNumber = number;
}

std::size_t PlayerInfo::getLevelNumber() const
{
	return _currentLevelNumber;
}



void PlayerInfo::initializeActions()
{
	//mActionBinding[MoveLeft].action = derivedAction<Hero>(AircraftMover(-1, 0));
	//mActionBinding[MoveRight].action = derivedAction<Hero>(AircraftMover(+1, 0));
	//mActionBinding[MoveUp].action = derivedAction<Hero>(AircraftMover(0, -1));
	//mActionBinding[MoveDown].action = derivedAction<Hero>(AircraftMover(0, +1));
	//mActionBinding[Fire].action = derivedAction<Hero>([](Hero& a, sf::Time) { a.fire(); });
	//mActionBinding[LaunchMissile].action = derivedAction<Hero>([](Hero& a, sf::Time) 
	//															 { a.launchMissile(); });
}

bool PlayerInfo::isRealtimeAction(const Action action)
{
	switch (action)
	{
		case PlayerInfo::Action::MoveLeft: 
			[[fallthrough]];
		case PlayerInfo::Action::MoveRight:
			[[fallthrough]];
		case PlayerInfo::Action::MoveDown:
			[[fallthrough]];
		case PlayerInfo::Action::MoveUp:
			[[fallthrough]];
		case PlayerInfo::Action::Fire:
			return true;

		default:
			return false;
	}
}

void PlayerInfo::setPlayer(Player* player)
{
	_player = player;
}

Player* PlayerInfo::getPlayer() const
{
	return _player;
}


void PlayerInfo::resetData()
{
	std::fill(mQuests.begin(), mQuests.end(), false);

	std::fill(mChosenSolution.begin(), mChosenSolution.end(), 0);

	mLivesCount = 3u;

	mLastSavePoint.x = 0.f;
	mLastSavePoint.y = 0.f;
}

void PlayerInfo::ressurectPlayer()
{
	if (mLivesCount == 0u)
	{
		mCanRessurect = false;
		return;
	}

	mLivesCount--;
	mCanRessurect = true;

	if (mQuests.at(2))
	{
		mLastSavePoint.x = 5056.f;
		mLastSavePoint.y = 1264.f;
	}
	if (mQuests.at(3))
	{
		mLastSavePoint.x = 8060.f;
		mLastSavePoint.y = 1184.f;
	}
}

