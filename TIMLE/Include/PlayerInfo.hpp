#ifndef PLAYERINFO_HPP
#define PLAYERINFO_HPP

#include "DataTables.hpp"
#include "Foreach.hpp"

#include <SFML/Window/Event.hpp>

#include <map>


/**
 * \brief Class, which contains and controls all player's data.
 */
class PlayerInfo final
{
	public:
		/**
		 * \brief Array of action's player states.
		 */
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

		/**
		 * \brief Array of the game status, according to the player's state.
		 */
		enum GameStatus
		{
			GameRunning,
			LevelComplete,
			GameOver
		};
	
	
	private:
		/**
		 * \brief Associative array of the key buttons for each player's action.
		 */
		std::map<sf::Keyboard::Key, Action>	mKeyBinding;

		//std::map<Action, Command>			mActionBinding;	// Not using now.

		/**
		 * \brief Current game status, using for interactiving with world.
		 */
		GameStatus 							mCurrentGameStatus;
	

	public:
		/**
		 * \brief Current dialog number, which we need to show.
		 */
		size_t								mDialogNumber;

		/**
		 * \brief Array of all showed dialogs, which we have shown yet.
		 */
		std::vector<size_t>					mShowedDialogs;

		/**
		 * \brief Variable for choosing number in the choosing state of dialog.
		 */
		size_t								mChoosingNumber;

		/**
		 * \brief All choices, which player has done yet.
		 */
		std::vector<size_t>					mChosenSolution;

		/**
		 * \brief Array of the quests, which player can carry out.
		 */
		std::vector<bool>					mQuests;

		/**
		 * \brief Current player's class for interacting.
		 */
		Player*								mPlayer;

		/**
		 * \brief Boolean flag for notification of completion of download level and resources.
		 */
		bool								mLoaded;


	private:
		void				initializeActions();
		static bool			isRealtimeAction(Action action);


	public:
		/**
		 * \brief Default constructor.
		 */
							PlayerInfo();
	
		/**
		 * \brief			Sends a notification to dialog state to show a certain dialog.
		 * \param number	Number of the necessary dialog.
		 */
		void				showDialog(size_t number);

		void				assignKey(Action action, sf::Keyboard::Key key);
		sf::Keyboard::Key	getAssignedKey(Action action) const;
	
		void 				setGameStatus(GameStatus status);
		GameStatus 			getGameStatus() const;

		/**
		 * \brief			Changes current player's class.
		 * \param player	Pointer to a new player's class for interacting.
		 */
		void				setPlayer(Player* player);

		/**
		 * \brief Resets all data about current game session.
		 */
		void				resetData();
};

#endif // PLAYERINFO_HPP
