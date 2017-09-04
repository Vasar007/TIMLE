#ifndef PLAYERINFO_HPP
#define PLAYERINFO_HPP

#include "DataTables.hpp"
#include <SFML/Window/Event.hpp>
#include "Foreach.hpp"
#include <map>


class PlayerInfo 
{
	public:
		enum Action
		{
			MoveLeft,
			MoveRight,
			MoveUp,
			MoveDown,
			Fire,
			LaunchMissile,
			ActionCount
		};
	
		enum GameStatus
		{
			GameRunning,
			LevelComplete,
			GameOver
		};
	
	
	private:
		void				initializeActions();
		static bool			isRealtimeAction(Action action);
	
	
	private:
		std::map<sf::Keyboard::Key, Action>	mKeyBinding;
		//std::map<Action, Command>			mActionBinding;
		GameStatus 							mCurrentGameStatus;
	

	public:
		int									mDialogNumber;
		std::vector<int>					mShowedDialogs;

		int									mChoosingNumber;
		std::vector<int>					mChosenSolution;

		std::vector<bool>					mQuests;

		Player*								mPlayer;

		bool								mLoaded;


	public:



							PlayerInfo();
	
		void				showDialog(int number);
		void				assignKey(Action action, sf::Keyboard::Key key);
		sf::Keyboard::Key	getAssignedKey(Action action) const;
	
		void 				setGameStatus(GameStatus status);
		GameStatus 			getGameStatus() const;

		void				setPlayer(Player* player);
		void				resetData();
};

#endif // PLAYERINFO_HPP
