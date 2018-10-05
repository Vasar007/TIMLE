#include "../Include/PlayerInfo.hpp"


PlayerInfo::PlayerInfo()
: _currentGameStatus(PlayerInfo::GameStatus::GameRunning)
, _currentLevelNumber(4)
, _player(nullptr)
, _defaultQuestValues{ { Quest::TalkWithOswald,        false },
                       { Quest::KillDwarvenCommanderM, false },
                       { Quest::TalkWithHeinrich,      false },
                       { Quest::FightGolemDark,        false },
                       { Quest::KillGolemDark,         false },
                       { Quest::FightShadow,           false },
                       { Quest::KillShadow,            false }
}
, _defaultSolutionValues{
    { Solution::LootKnightAndDwarven, 0 },
    { Solution::InteractWithGolem,    0 }
}
, mDialogNumber(0)
, mChoosingNumber(0)
, mChosenSolution(_defaultSolutionValues)
, mQuests(_defaultQuestValues)
, mLoaded(false)
, mLivesCount(3)
, mLastSavePoint(0.f, 0.f)
, mCanRessurect(false)
, mCanTransit(false)
, mNeedTransit(false)
, mDoTransit(false)
, mNumberOfDoor()
{
    // Set initial key bindings.
    _keyBinding.emplace(sf::Keyboard::Left,  PlayerInfo::Action::MoveLeft);
    _keyBinding.emplace(sf::Keyboard::Right, PlayerInfo::Action::MoveRight);
    _keyBinding.emplace(sf::Keyboard::Up,    PlayerInfo::Action::MoveUp);
    _keyBinding.emplace(sf::Keyboard::Down,  PlayerInfo::Action::MoveDown);
    _keyBinding.emplace(sf::Keyboard::Space, PlayerInfo::Action::Fire);
    _keyBinding.emplace(sf::Keyboard::M,     PlayerInfo::Action::LaunchMissile);
    _keyBinding.emplace(sf::Keyboard::E,     PlayerInfo::Action::UseDoor);

    // Set initial action bindings.
    initializeActions();

    // Assign all categories to player's hero
    //FOREACH(auto& pair, mActionBinding)
    //    pair.second.category = Category::PlayerHero;
}

void PlayerInfo::showDialog(const std::size_t number)
{
    mDialogNumber = number;

    bool isFind = false;
    for (const auto& dialog : mShowedDialogs)
    {
        if (dialog == mDialogNumber)
        {
            mDialogNumber = 0;
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
    _keyBinding.emplace(key, action);
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
    //                                                           { a.launchMissile(); });
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
    mQuests = _defaultQuestValues;
    mChosenSolution = _defaultSolutionValues;

    mLivesCount = 3;

    mLastSavePoint.x = 0.f;
    mLastSavePoint.y = 0.f;
}

void PlayerInfo::ressurectPlayer()
{
    if (mLivesCount == 0)
    {
        mCanRessurect = false;
        return;
    }

    mLivesCount--;
    mCanRessurect = true;

    // Save after finishing first quest with knights.
    if (mQuests.at(Quest::TalkWithHeinrich))
    {
        mLastSavePoint.x = 5056.f;
        mLastSavePoint.y = 1264.f;
    }
    // Save after fighting with first mini-boss.
    if (mQuests.at(Quest::KillGolemDark))
    {
        mLastSavePoint.x = 8060.f;
        mLastSavePoint.y = 1184.f;
    }
}

bool PlayerInfo::isFigthWithBoss() const
{
    const bool first_mini_boss = mQuests.at(Quest::FightGolemDark) &
                                 !mQuests.at(Quest::KillGolemDark);

    const bool first_boss = mQuests.at(Quest::FightShadow) & !mQuests.at(Quest::KillShadow);

    return first_mini_boss || first_boss;
}

