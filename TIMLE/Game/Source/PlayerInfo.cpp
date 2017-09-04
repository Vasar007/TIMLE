#include "../Include/PlayerInfo.hpp"


PlayerInfo::PlayerInfo()
: mCurrentGameStatus(GameRunning)
, mDialogNumber(0)
, mShowedDialogs()
, mChoosingNumber(0)
, mChosenSolution(2)
, mQuests(5)
, mPlayer(nullptr)
, mLoaded(false)
{
	/*
	 *	Ќабор квестов:
	 *	0 Ц разговор с Oswald
	 *	1 Ц убийство DwarvenCommanderM и получение ключа-активатора
	 *	2 Ц разговор с Heinrich
	 *	3 Ц победа над боссом Shadow
	 *	4 Ц победа над мини-боссом GolemDark
	 */
	for (size_t i = 0; i < mQuests.size(); i++)
		mQuests[i] = false;

	/*
	 *	—писок сделанных выборов:
	 *	0 - мародЄрство трупов рыцар€ и дворфов
	 *	1 - действи€ с тЄмным големом
	 */
	for (size_t i = 0; i < mChosenSolution.size(); i++)
		mChosenSolution[i] = 0;

	// Set initial key bindings
	mKeyBinding[sf::Keyboard::Left] = MoveLeft;
	mKeyBinding[sf::Keyboard::Right] = MoveRight;
	mKeyBinding[sf::Keyboard::Up] = MoveUp;
	mKeyBinding[sf::Keyboard::Down] = MoveDown;
	mKeyBinding[sf::Keyboard::Space] = Fire;
	mKeyBinding[sf::Keyboard::M] = LaunchMissile;

	// Set initial action bindings
	initializeActions();

	// Assign all categories to player's aircraft
	//FOREACH(auto& pair, mActionBinding)
		//pair.second.category = Category::PlayerAircraft;
}

void PlayerInfo::showDialog(int number)
{
	mDialogNumber = number;

	bool isFind = false;
	for (size_t i = 0; i < mShowedDialogs.size(); i++)
	{
		if (mShowedDialogs[i] == mDialogNumber)
		{
			mDialogNumber = 0;
			isFind = true;
			break;
		}
	}

	if (!isFind)
		mShowedDialogs.push_back(mDialogNumber);
}


void PlayerInfo::assignKey(Action action, sf::Keyboard::Key key)
{
	// Remove all keys that already map to action
	for (auto itr = mKeyBinding.begin(); itr != mKeyBinding.end(); )
	{
		if (itr->second == action)
			mKeyBinding.erase(itr++);
		else
			++itr;
	}

	// Insert new binding
	mKeyBinding[key] = action;
}

sf::Keyboard::Key PlayerInfo::getAssignedKey(Action action) const
{
	FOREACH(auto pair, mKeyBinding)
	{
		if (pair.second == action)
			return pair.first;
	}

	return sf::Keyboard::Unknown;
}

void PlayerInfo::setGameStatus(GameStatus status)
{
	mCurrentGameStatus = status;
}

PlayerInfo::GameStatus PlayerInfo::getGameStatus() const
{
	return mCurrentGameStatus;
}

void PlayerInfo::initializeActions()
{
	//mActionBinding[MoveLeft].action = derivedAction<Hero>(AircraftMover(-1, 0));
	//mActionBinding[MoveRight].action = derivedAction<Hero>(AircraftMover(+1, 0));
	//mActionBinding[MoveUp].action = derivedAction<Hero>(AircraftMover(0, -1));
	//mActionBinding[MoveDown].action = derivedAction<Hero>(AircraftMover(0, +1));
	//mActionBinding[Fire].action = derivedAction<Hero>([](Hero& a, sf::Time) { a.fire(); });
	//mActionBinding[LaunchMissile].action = derivedAction<Hero>([](Hero& a, sf::Time) { a.launchMissile(); });
}

bool PlayerInfo::isRealtimeAction(Action action)
{
	switch (action)
	{
	case MoveLeft:
	case MoveRight:
	case MoveDown:
	case MoveUp:
	case Fire:
		return true;

	default:
		return false;
	}
}

void PlayerInfo::setPlayer(Player* player)
{
	mPlayer = player;
}

void PlayerInfo::resetData()
{
	for (size_t i = 0; i < mQuests.size(); i++)
		mQuests[i] = false;

	for (size_t i = 0; i < mChosenSolution.size(); i++)
		mChosenSolution[i] = 0;
}
